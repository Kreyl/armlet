/*
 * descriptors.cpp
 *
 *  Created on: Jul 5, 2013
 *      Author: g.kruglov
 */

#include "descriptors.h"
#include "buf_sizes.h"

#define EP_DIR_IN           0x80
#define EP_DIR_OUT          0x00

// Descriptor-type endpoint codes
#define EP_TYPE_ISOCHRONOUS 0x01
#define EP_TYPE_BULK        0x02
#define EP_TYPE_INTERRUPT   0x03

const EpCfg_t EpCfg[EP_CNT] = {
        // Control endpoint
        {
            Indx:       0,
            Type:       0,//EPR_EP_TYPE_CONTROL,
            InMaxsize:  EP0_SZ,
            OutMaxsize: EP0_SZ,
        },
        // Interrupt endpoint
        {
            Indx:       1,
            Type:       0,//EPR_EP_TYPE_INTERRUPT,
            InMaxsize:  EP_INTERRUPT_SZ,
            OutMaxsize: 0,
        },
        // Bulk Out endpoint
        {
            Indx:       2,
            Type:       0,//EPR_EP_TYPE_BULK,
            InMaxsize:  0,
            OutMaxsize: EP_BULK_SZ,
        },
        // Bulk In endpoint
        {
            Indx:       3,
            Type:       0,//EPR_EP_TYPE_BULK,
            InMaxsize:  EP_BULK_SZ,
            OutMaxsize: 0,
        }
};

// ============================== Descriptors ==================================
// Macro to calculate the power value for the configuration descriptor, from a given number of milliamperes.
#define USB_CONFIG_POWER_MA(mA) ((mA) >> 1)
// Macro to encode a given four digit floating point version number (e.g. 01.23) into Binary Coded Decimal format for descriptor fields requiring BCD encoding, such as the USB version number in the standard device descriptor.
#define VERSION_BCD(x)          (((((VERSION_TENS(x) << 4) | VERSION_ONES(x)) << 8) | ((VERSION_TENTHS(x) << 4) | VERSION_HUNDREDTHS(x))))
//Macro to calculate the Unicode length of a string with a given number of Unicode characters
#define USB_STRING_LEN(UnicodeChars)      (2 + ((UnicodeChars) << 1))

// Device
static const DeviceDescriptor_t DeviceDescriptor = {
        bLength:            sizeof(DeviceDescriptor_t),   // Size
        bDescriptorType:    dtDevice,        // Descriptor type
        bcdUSB:             0x0200,          // USB 2.0
        bDeviceClass:       0x00,            // bDeviceClass
        bDeviceSubClass:    0x00,            // bDeviceSubClass
        bDeviceProtocol:    0x00,            // bDeviceProtocol
        bMaxPacketSize0:    EP0_SZ,          // bMaxPacketSize0
        idVendor:           0x21BB,          // idVendor (WWPass)
        idProduct:          0x0002,          // idProduct; #2 means USB Key
        bcdDevice:          0,               // bcdDevice
        iManufacturer:      1,               // iManufacturer
        iProduct:           2,               // iProduct
        iSerialNumber:      0,               // iSerialNumber
        bNumConfigurations: 1                // bNumConfigurations
};

// Configuration
static const ConfigDescriptor_t ConfigDescriptor = {
        ConfigHeader: {
            bLength:            sizeof(ConfigHeader_t),
            bDescriptorType:    dtConfiguration,
            wTotalLength:       sizeof(ConfigDescriptor_t),
            bNumInterfaces:     1,
            bConfigurationValue: 0,
            iConfiguration:     0,
            bmAttributes:       0x80,   // USB_CONFIG_ATTR_RESERVED
            bMaxPower:          USB_CONFIG_POWER_MA(100)
        },

        SCR_Interface: {
            bLength:            sizeof(InterfaceDescriptor_t),
            bDescriptorType:    dtInterface,
            bInterfaceNumber:   0,
            bAlternateSetting:  0,
            bNumEndpoints:      3,
            bInterfaceClass:    0x0B,
            bInterfaceSubClass: 0,
            bInterfaceProtocol: 0,
            iInterface:         0
        },

        SCR_Functional: {
            bLength:                sizeof(SCRFuncDescriptor_t),
            bDescriptorType:        0x21,
            bcdCCID:                0x0110,
            bMaxSlotIndex:          0,
            bVoltageSupport:        0x02,       // 3.0 V
            dwProtocols:            0x00000003, // Indicates the supported protocol types: 00000001h T=0, 00000002h T=1
            dwDefaultClock:         4000,       // 4MHz, not relevant, fixed for legacy reason
            dwMaximumClock:         4000,       // 4MHz, not relevant, fixed for legacy reason
            bNumClockSupported:     0,          // Default clock, not relevant, fixed for legacy reason
            dwDataRate:             9600,       // 9600 bps, not relevant, fixed for legacy reason
            dwMaxDataRate:          9600,       // 9600 bps, not relevant, fixed for legacy reason
            bNumDataRatesSupported: 0,          // Default data rate, not relevant, fixed for legacy reason
            dwMaxIFSD:              CCID_DATA_SZ, // Maximum size of a block accepted from card by the reader. Indicates the maximum IFSD supported by the USB-ICC for protocol T=1. For T=0 any value may be given.
            dwSynchProtocols:       0,          // ISO7816-3, not relevant, fixed to for legacy reason
            dwMechanical:           0,          // No special characteristic, not relevant, fixed to for legacy reason
            //dwFeatures: 0x00020842, = 2+40+800+20000
            //dwFeatures:             (0x02+0x08 + 0x10+0x20+0x40 + 0x020000), // Page 19
            dwFeatures: (0x02+0x04+0x08+0x20+0x40+0x020000),
            //dwFeatures:             (0x02+0x08 + 0x10+0x20+0x80 + 0x020000), // Page 19
            /* For extended APDU level the value shall be between 261 + 10 (header)
             * and 65544 +10, otherwise the minimum value is the wMaxPacketSize
             * of the Bulk-OUT endpoint */
            dwMaxCCIDMessageLength: (CCID_HEADER_SZ + CCID_DATA_SZ), // The value shall be between (dwMaxIFSD + 10) and (65544 +10)
            bClassGetResponse:      0xFF,       // Echoes the class of the APDU
            bClassEnvelope:         0xFF,       // Echoes the class of the APDU
            wLcdLayout:             0,          // No LCD, not relevant, fixed for legacy reason
            bPinSupport:            0,          // No PIN pad, not relevant, fixed for legacy reason
            bMaxCCIDBusySlots:      1           // One slot is busy, the USB-ICC is regarded as a single slot CCID
        },

        // Endpoints
        InterruptEndpoint: {
            bLength:            sizeof(EndpointDescriptor_t),
            bDescriptorType:    dtEndpoint,
            bEndpointAddress:   (EP_DIR_IN | 1),
            bmAttributes:       EP_TYPE_INTERRUPT,
            wMaxPacketSize:     EP_INTERRUPT_SZ,
            bInterval:          0x10
        },

        DataOutEndpoint: {
            bLength:            sizeof(EndpointDescriptor_t),
            bDescriptorType:    dtEndpoint,
            bEndpointAddress:   (EP_DIR_OUT | 2),
            bmAttributes:       EP_TYPE_BULK,
            wMaxPacketSize:     EP_BULK_SZ,
            bInterval:          0x00
        },

        DataInEndpoint: {
            bLength:            sizeof(EndpointDescriptor_t),
            bDescriptorType:    dtEndpoint,
            bEndpointAddress:   (EP_DIR_IN | 3),
            bmAttributes:       EP_TYPE_BULK,
            wMaxPacketSize:     EP_BULK_SZ,
            bInterval:          0x00
        }
};


// ================================= Strings ===================================
// U.S. English language identifier
static const StringDescriptor_t LanguageString = {
        bLength: USB_STRING_LEN(1),
        bDescriptorType: dtString,
        bString: {0x0409}
};

// Vendor string
static const StringDescriptor_t ManufacturerString = {
        bLength: USB_STRING_LEN(6),
        bDescriptorType: dtString,
        bString: {'W', 'W', 'P', 'a', 's', 's'}
};

// Device Description string
static const StringDescriptor_t ProductString = {
        bLength: USB_STRING_LEN(3),
        bDescriptorType: dtString,
        bString: {'K', 'e', 'y'}
};

void GetDescriptor(uint8_t Type, uint8_t Indx, uint8_t **PPtr, uint32_t *PLen) {
    switch(Type) {
        case dtDevice:
            *PPtr = (uint8_t*)&DeviceDescriptor;
            *PLen = sizeof(DeviceDescriptor);
            break;
        case dtConfiguration:
            *PPtr = (uint8_t*)&ConfigDescriptor;
            *PLen = sizeof(ConfigDescriptor);
            break;
        case dtString:
            switch(Indx) {
                case 0:
                    *PPtr = (uint8_t*)&LanguageString;
                    *PLen = LanguageString.bLength;
                    break;
                case 1:
                    *PPtr = (uint8_t*)&ManufacturerString;
                    *PLen = ManufacturerString.bLength;
                    break;
                case 2:
                    *PPtr = (uint8_t*)&ProductString;
                    *PLen = ProductString.bLength;
                    break;
                default: break;
            }
            break;
        default:
            *PLen = 0;
            break;
    }
}
