/*
 * lvl1_assym.cpp
 *
 *  Created on: 28.03.2013
 *      Author: kreyl
 */

#include "lvl1_assym.h"
#include "cc1101.h"

//#define DBG_PINS

#ifdef DBG_PINS
#define DBG_GPIO1   GPIOD
#define DBG_PIN1    14
#define DBG1_SET()  PinSet(DBG_GPIO1, DBG_PIN1)
#define DBG1_CLR()  PinClear(DBG_GPIO1, DBG_PIN1)
#endif

static Thread *PThr;
rLevel1_t rLevel1;

// ==== Common ====
// Report about data pkt transmission result
void rLevel1_t::ICompleteTx(uint8_t AState) {
    // Flush remaining data
    if(ITxHdr.Length) ITxBuf.Flush(ITxHdr.Length);
    ITxHdr.State = AState;
    ITransmitted.Put((rPktIDState_t*)&ITxHdr);    // Put in state queue
    chEvtBroadcast(&IEvtSrcRadioTxEnd);
}

// ================================ Gate task ==================================
#ifdef GATE
VirtualTimer rTmr;
void rTmrCallback(void *p);

// Prepare either rpkt with data, or rpkt just to ping
void rLevel1_t::PrepareDataPkt() {
    //Uart.Printf("PTx\r");
    PktTx.rID = ITxHdr.rID;
    // Check if last pkt
    if(ITxHdr.Length <= RLASTPKT_DATASZ) {
        ITxHdr.State = LAST;
        ITxBuf.Get(PktTx.Data, ITxHdr.Length);      // Put data to pkt
        PktTx.Data[RDATA_CNT-1] = ITxHdr.Length;    // Set data count in last byte
        PktTx.Srv = R_DIR_GATE2DEV | R_CMD_DATA;    // Do not listen next slot
        ITxHdr.Length = 0;
    }
    else {
        ITxHdr.State = IN_PROGRESS;                 // Signal that first rpkt was sent
        ITxBuf.Get(PktTx.Data, RDATA_CNT);          // Put data to pkt
        PktTx.Srv = R_DIR_GATE2DEV | R_CMD_DATA | R_NXTSLT; // Listen next slot
        ITxHdr.Length -= RDATA_CNT;
    }
    //Uart.Printf("%A\r", (uint8_t*)&PktTx, RPKT_LEN, ' ');
}
void rLevel1_t::PreparePingPkt() {
    PktTx.rID = SLOT2ID(SlotN);
    PktTx.Srv = R_DIR_GATE2DEV | R_CMD_PING;
}

// Handler of received rPkt. Just one rPkt at a time allowed.
uint8_t rLevel1_t::HandleRxDataPkt() {
    PktRxData.rID = PktRx.rID;
    // Get effective length from last byte
    PktRxData.Length = PktRx.Data[RDATA_CNT-1];
    TRIM_VALUE(PktRxData.Length, (RDATA_CNT-1));
    memcpy(PktRxData.Data, PktRx.Data, PktRxData.Length);
    uint8_t Rslt = IRxBuf.Put(&PktRxData);
    if(Rslt == OK) chEvtBroadcast(&IEvtSrcRadioRx);    // Pkt received completely
    return Rslt;
}

// Radiotask
void rLevel1_t::Task() {
    chSysLock();
    chSchGoSleepS(THD_STATE_SUSPENDED); // Will wake up by rTmr
    chSysUnlock();
    // ==== Sleep ended, transmit new pkt ====
    if(++SlotN >= RSLOT_CNT) SlotN = 0;
    PktTx.SlotN = SlotN;
    // If not transmitting long pkt already, try to get new data pkt
    if((ITxHdr.State == OK) or (ITxHdr.State == FAILURE) or (ITxHdr.State == BUSY)) {
        if(ITxBuf.Get((uint8_t*)&ITxHdr, sizeof(rPktHeader_t)) == OK) ITxHdr.State = NEW;   // Will not be transmitted until correct slot
    }
    // Transmit data if new and in correct slot, or if first pkt is already sent
    if(((ITxHdr.State == NEW) and InsideCorrectSlot()) or (ITxHdr.State == IN_PROGRESS)) PrepareDataPkt();
    else PreparePingPkt();

    CC.Transmit(&PktTx);

    // ==== Pkt transmitted, enter RX ====
    uint8_t RxRslt = CC.Receive(R_RX_WAIT_MS, &PktRx);
    //Uart.Printf("Rx: %u\r", RxRslt);
    // Process result
    if(RxRslt == OK) {
        //Uart.Printf("Rx Slot=%u; ID=%u; Srv=%X\r", SlotN, PktRx.rID, PktRx.Srv);
        // If data received, put it to queue
        if(PktRx.Srv & R_CMD_DATA) HandleRxDataPkt();
        // Check reply to data containing pkt
        if((ITxHdr.State == LAST) and (PktRx.Srv & R_ACK)) ICompleteTx(OK);    // If transmission completed
        if(((ITxHdr.State == IN_PROGRESS) or (ITxHdr.State == LAST)) and !(PktRx.Srv & R_ACK)) ICompleteTx(BUSY);
    } // if ok
    else {  // No answer
        // If data pkt in progress, fail it
        if((ITxHdr.State == IN_PROGRESS) or (ITxHdr.State == LAST)) ICompleteTx(FAILURE);
    } // if rx
}

// Timer callback
void rTmrCallback(void *p) {
    // Restart the timer, resume thread if needed
    chSysLockFromIsr();
    chVTSetI(&rTmr, MS2ST(RTIMESLOT_MS), rTmrCallback, p);
    if(PThr->p_state == THD_STATE_SUSPENDED) chSchReadyI(PThr);
    chSysUnlockFromIsr();
}
#endif

// ============================= Device task ===================================
#ifdef DEVICE
enum rMode_t {rmAlone, rmInSync} rMode;
extern void RFillPkt(uint8_t *Ptr, uint8_t *PLen);

// Calculates how long to wait for our timeslot
uint32_t rLevel1_t::ICalcWaitRx_ms(uint8_t RcvdSlot) {
    uint32_t SlotsToWait;
    uint32_t SelfSlot = ID2SLOT(SelfID);
    if(SelfSlot >= RcvdSlot) SlotsToWait = SelfSlot - RcvdSlot;
    else SlotsToWait = RDEVICE_CNT - (RcvdSlot - SelfSlot);
    // Add some reserve
    if(SlotsToWait >= RRX_START_RESERVE) SlotsToWait -= RRX_START_RESERVE;
    //Uart.Printf("Self:%u; Rc: %u; TS: %u\r", ISelfID, RcvdID, TimeslotsToWait);
    // Convert timeslots to ms
    return (SlotsToWait * RTIMESLOT_MS);
}

void rLevel1_t::ISleepIfLongToWait(uint8_t RcvdSlot) {
    // Calculate how long to wait to enter RX
    uint32_t msToWaitRx = ICalcWaitRx_ms(RcvdSlot);
    //Uart.Printf("ms: %u\r", msToWaitRx);
    if(msToWaitRx > RMIN_TIME_TO_SLEEP_MS) {    // If enough time to sleep
        CC.Sleep();                             // Shutdown CC
        chThdSleepMilliseconds(msToWaitRx);
    }
}

// Called time after time to discover best gate to communicate with
void rLevel1_t::IDiscovery() {
    //Uart.Printf("Dsc\r");
    bool SomeoneIsNear = false, Retry = false;
    GateRssi = -126; // Lowest possible
    systime_t fTime;
    uint8_t RxRslt;
    uint32_t GateSlot = 0;
    // Iterate all channels
    for(uint16_t i=0; i<RGATE_CNT; i++) {
        CC.SetChannel(i);
        do {
            RxRslt = CC.Receive(RDISCOVERY_RX_MS, &PktRx);
            switch(RxRslt) {
                case TIMEOUT:   // Noone is near, do not listen this channel anymore
                    //Uart.Printf("TO\r");
                    Retry = false;
                    break;
                case FAILURE:
                    //Uart.Printf("Fl\r");
                    Retry = true; // Someone is near, listen again
                    break;
                case OK:
                    //Uart.Printf("Ok\r");
                    // Check if rpkt is gate-to-device
                    if(PktRx.Srv & R_DIR_GATE2DEV) {
                        Retry = false;                  // No need to retry
                        //Uart.Printf("N: %u; Rssi: %d\r", i, pktRx.RSSI);
                        SomeoneIsNear = true;
                        if(PktRx.RSSI > GateRssi) {     // Check if switch to this channel
                            GateN = i;                  // Note, number of concentrator != ID of concentrator
                            GateSlot = PktRx.SlotN;     // Save current slot of the gate
                            GateRssi = PktRx.RSSI;
                            fTime = chTimeNow();        // Save current time
                        }
                    }
                    else Retry = true;  // If not gate, try to receive again
                    break;
            } // switch
        } while(Retry);
    } // for

    // Decide which channel to use
    if(SomeoneIsNear == false) {
        rMode = rmAlone; // Silence answers our cries
        GateN = R_NO_ID;
    }
    else {  // Some gate is near
        //Uart.Printf("Dsc: %u\r", CntrN);
        rMode = rmInSync;
        CC.SetChannel(GateN);
        // Calculate how much time passed since we found concentrator
        uint32_t t = chTimeNow() - fTime;
        // Calculate current gate's slot
        GateSlot += t / RTIMESLOT_MS;
        if(GateSlot > RDEV_TOP_ID) GateSlot -= RDEVICE_CNT;
        ISleepIfLongToWait(GateSlot); // Sleep if needed
    } // if gate is near
}

// Rx data handling
void rLevel1_t::IStartRx() {
    RxPktState = IN_PROGRESS;   // Set state
    PktRxData.rID = PktRx.rID;  // Set ID
    PktRxData.Length = 0;       // Reset length
    PRxData = PktRxData.Data;   // Set pointer to data start
}
uint8_t rLevel1_t::IAppendRx(uint8_t ALength) {
    // Check if overflow
    if((PktRxData.Length + ALength) > RRX_PKT_DATA_SZ) return FAILURE;
    memcpy(PRxData, PktRx.Data, ALength);
    PktRxData.Length += ALength;    // Update length
    PRxData += ALength;             // Update pointer
    return OK;
}

void rLevel1_t::IEndRx(uint8_t ARslt) {
    if(ARslt == OK) {   // Add pkt to buffer and rise event if needed
        IRxBuf.Put(&PktRxData);
        chEvtBroadcast(&IEvtSrcRadioRx);
    }
    RxPktState = FAILURE;
}

uint8_t rLevel1_t::HandleRxDataPkt() {
    if(RxPktState != IN_PROGRESS) IStartRx();  // Start new reception
    // Check if this pkt is last
    if(!(PktRx.Srv & R_NXTSLT)) {   // No NXT_SLT => last pkt
        // Get effective length
        uint8_t Length = PktRx.Data[RDATA_CNT-1];
        TRIM_VALUE(Length, RLASTPKT_DATASZ);
        RxPktState = FAILURE;
        //Uart.Printf(" L%u", Length);
        if(IAppendRx(Length) == OK) {   // Check if fits in pkt
            // Check if there is room for even one rpkt. Do not copy now to save precious time before transmission
            if(IRxBuf.GetEmptyCount() >= 1) RxPktState = OK;
        }
        return RxPktState;
    }
    // Not a last pkt
    else return IAppendRx(RDATA_CNT);
}

// Called periodically when gate is successfully discovered
void rLevel1_t::IInSync() {
    uint8_t RxRslt = CC.Receive(RDISCOVERY_RX_MS, &PktRx);
    if(RxRslt == OK) {
        GateRssi = PktRx.RSSI;
        //Uart.Printf("Rx Slot=%u; ID=%u; Srv=%X\r", PktRx.SlotN, PktRx.rID, PktRx.Srv);
        RxRetryCounter = 0;     // Something was successfully received, reset counter
        // Check if pkt is ours
        if(PktRx.rID == SelfID) {
            if(PktRx.Srv & R_CMD_DATA) {
                if(HandleRxDataPkt() == OK) PktTx.Srv = R_DIR_DEV2GATE | R_ACK;
                else                        PktTx.Srv = R_DIR_DEV2GATE;        // No ACK
            }
            else {
                PktTx.Srv = R_DIR_DEV2GATE | R_ACK;             // Just ping, just ACK
                if(RxPktState == IN_PROGRESS) IEndRx(FAILURE);  // Ping instead of data
            }

            // Reply with Data if TxQueue is not empty
            // Commented out to use RFillPkt()
//            if(ITxBuf.Get((uint8_t*)&ITxHdr, sizeof(rPktHeader_t)) == OK) {
//                ITxBuf.Get(PktTx.Data, ITxHdr.Length);    // Put data to pkt
//                PktTx.Data[RDATA_CNT-1] = ITxHdr.Length;  // Set data count in last byte
//                PktTx.Srv |= R_CMD_DATA;
//            }
            uint8_t FLen;
            RFillPkt(PktTx.Data, &FLen);
            PktTx.Data[RDATA_CNT-1] = FLen;  // Set data count in last byte
            PktTx.Srv |= R_CMD_DATA;
            CC.Transmit(&PktTx);

            // If data was transmitted, report TX ok
            if(PktTx.Srv & R_CMD_DATA) ICompleteTx(OK); // Always ok
            // Copy received data in buffer if needed
            if(RxPktState == OK) IEndRx(OK);
            // Enter discovery if no need to listen next slot
            if(!(PktRx.Srv & R_NXTSLT)) IDiscovery();
        } // if pkt is ours
        else {  // Not our pkt => sync failure or our slot is occupied. Sleep if needed.
            // if from dev => sync failure, wait next pkt, do not sleep
            // if from gate and wrong ID and wrong   slot => sync failure, sleep(slot)
            // if from gate and wrong ID and correct slot => occupied, sleep(slot)
            if(PktRx.Srv & R_DIR_GATE2DEV) ISleepIfLongToWait(PktRx.SlotN);
        }
    } // if received ok
    else {
        // Reset ongoing reception if any
        if(RxPktState == IN_PROGRESS) IEndRx(FAILURE);
        // Check if we get lost
        if(RxRetryCounter++ > R_IN_SYNC_RETRY_CNT) rMode = rmAlone;
    }
}

void rLevel1_t::Task() {
    chBSemWait(&bsemShutdown);
    switch(rMode) {
        case rmAlone:
            CC.Sleep();  // Shutdown CC
            chThdSleepMilliseconds(RDISCOVERY_PERIOD_MS);
            IDiscovery();
            break;

        case rmInSync:
            IInSync();
            break;
    } // switch
    chBSemSignal(&bsemShutdown);
}

void rLevel1_t::Shutdown() {
    chBSemWait(&bsemShutdown);
    CC.Sleep();  // Shutdown CC
}
#endif

// ================================= Thread ====================================
static WORKING_AREA(warLvl1Thread, 1024);
static msg_t rLvl1Thread(void *arg) {
    (void)arg;
    chRegSetThreadName("rLvl1");
    while(1) rLevel1.Task();
    return 0;
}

// ================================= Common ====================================
void rLevel1_t::Init(uint16_t ASelfID) {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull, pudNone);
#endif
    // Init Rx & Tx buffers
    chEvtInit(&IEvtSrcRadioRx);     // }
    chEvtInit(&IEvtSrcRadioTxEnd);  // } Events
    IRxBuf.Init();                  // Buffer of received pkts
    ITxBuf.Init();                  // Buffer of pkts to transmit
    ITransmitted.Init();            // Buffer of transmitted pkt headers
    // Init radioIC
    CC.Init();
    CC.SetTxPower(PwrPlus12dBm);
    //CC.SetTxPower(PwrMinus6dBm);
    // Variables
    ITxHdr.State = FAILURE;
#ifdef GATE
    SlotN = 0; // Slot number to start from
    SetID(ASelfID);
    CC.SetChannel(ASelfID);
    // Timeslot Timer
    chVTSet(&rTmr, MS2ST(RTIMESLOT_MS), rTmrCallback, NULL);  // Start timer
#else
    chBSemInit(&bsemShutdown, NOT_TAKEN);
    SelfID = ASelfID;
    rMode = rmAlone;
    PktTx.rID = SelfID; // Always the same
    RxPktState = FAILURE;
    CC.SetChannel(1);
#endif
    // Init thread. High priority is required to satisfy timings.
    PThr = chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, rLvl1Thread, NULL);
}

#ifdef GATE
void rLevel1_t::SetID(uint16_t ASelfID) {
    SelfID = ASelfID;
    CC.SetChannel(ASelfID);
}
#endif

uint8_t rLevel1_t::AddPktToTx(uint8_t rID, uint8_t *Ptr, uint32_t Length) {
    TRIM_VALUE(Length, RTX_PKT_DATA_SZ);
    if(ITxBuf.GetEmptyCount() >= (sizeof(rPktHeader_t) + Length)) {
        rPktHeader_t Hdr;
        Hdr.rID = rID;
        Hdr.Length = Length;
        chSysLock();
        ITxBuf.Put((uint8_t*)&Hdr, sizeof(rPktHeader_t));   // Put header
        ITxBuf.Put(Ptr, Length);                            // Put data
        chSysUnlock();
        return OK;
    }
    else return FAILURE;
}
