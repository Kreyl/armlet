/*
 * evt_mask.h
 *
 *  Created on: Apr 12, 2013
 *      Author: g.kruglov
 */

#ifndef EVT_MASK_H_
#define EVT_MASK_H_

// Event masks
#define EVTMASK_RADIO_TX    EVENT_MASK(0)
#define EVTMASK_RADIO_RX    EVENT_MASK(1)
#define EVTMASK_KEYS        EVENT_MASK(2)
#define EVTMASK_IR          EVENT_MASK(3)
#define EVTMASK_TIMER       EVENT_MASK(4)
#define EVTMASK_KEYLOCK     EVENT_MASK(5)
#define EVTMASK_NEWSECOND   EVENT_MASK(6)
#define EVTMASK_USB_READY   EVENT_MASK(7)

#define EVTMSK_TABLE_READY  EVENT_MASK(8)

#define EVTMSK_NEW_CYCLE    EVENT_MASK(9)
#define EVTMSK_UPDATE_CYCLE EVENT_MASK(10)
#define EVTMSK_MESH_RX      EVENT_MASK(11)
#define EVTMSK_MESH_TX      EVENT_MASK(12)


#endif /* EVT_MASK_H_ */
