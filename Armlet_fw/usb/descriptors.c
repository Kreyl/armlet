/*
 * descriptors.cpp
 *
 *  Created on: Jul 5, 2013
 *      Author: g.kruglov
 */

#include "descriptors.h"

#if 1 // ==== Endpoints config ====
const EpCfg_t EpCfg[EP_CNT] = {
        // Control endpoint, Indx = 0
        {
            Type:       EP_TYPE_CONTROL,
            InMaxsize:  EP0_SZ,
            OutMaxsize: EP0_SZ,
        },
        // Bulk Out endpoint, Indx = 1
        {
            Type:       EP_TYPE_BULK,
            InMaxsize:  0,
            OutMaxsize: EP_BULK_SZ,
        },
        // Bulk In endpoint, Indx = 2
        {
            Type:       EP_TYPE_BULK,
            InMaxsize:  EP_BULK_SZ,
            OutMaxsize: 0,
        }
};
#endif

// ============================== Descriptors ==================================
// Macro to calculate the power value for the configuration descriptor, from a given number of milliamperes.
#define USB_CONFIG_POWER_MA(mA) ((mA) >> 1)

//Macro to calculate the Unicode length of a string with a given number of Unicode characters
#define USB_STRING_LEN(UnicodeChars)      (2 + ((UnicodeChars) << 1))

#if 1 // ==== Device Descriptor ====
static const DeviceDescriptor_t DeviceDescriptor = {
        bLength:            sizeof(DeviceDescriptor_t),   // Size
        bDescriptorType:    dtDevice,        // Descriptor type
        bcdUSB:             0x0200,          // USB 2.0
        bDeviceClass:       0x00,            // No class
        bDeviceSubClass:    0x00,            // No SubClass
        bDeviceProtocol:    0x00,            // No Protocol
        bMaxPacketSize0:    EP0_SZ,          // bMaxPacketSize0
        idVendor:           0x21BB,          // idVendor (WWPass)
        idProduct:          11,              // idProduct; #11 means MassStorageDevice
        bcdDevice:          0x0001,          // bcdDevice
        iManufacturer:      1,               // iManufacturer
        iProduct:           2,               // iProduct
        iSerialNumber:      3,               // iSerialNumber
        bNumConfigurations: 1                // bNumConfigurations
};
#endif

#if 1 // ==== Configuration descriptor ====
static const ConfigDescriptor_t ConfigDescriptor = {
        ConfigHeader: {
            bLength:            sizeof(ConfigHeader_t),
            bDescriptorType:    dtConfiguration,
            wTotalLength:       sizeof(ConfigDescriptor_t),
            bNumInterfaces:     1,
            bConfigurationValue:1,
            iConfiguration:     0,      // No descriptor
            bmAttributes:       0x80,   // USB_CONFIG_ATTR_RESERVED
            bMaxPower:          USB_CONFIG_POWER_MA(100)
        },
        // Mass Storage Interface
        MS_Interface: {
            bLength:            sizeof(InterfaceDescriptor_t),
            bDescriptorType:    dtInterface,
            bInterfaceNumber:   0,
            bAlternateSetting:  0,
            bNumEndpoints:      2,
            bInterfaceClass:    0x08, // Mass Storage class
            bInterfaceSubClass: 0x06, // SCSI Transparent Command Set subclass of the Mass storage class
            bInterfaceProtocol: 0x50, // Bulk Only Transport protocol of the Mass Storage class
            iInterface:         0     // No descriptor
        },

        MS_DataOutEndpoint: {
            bLength:            sizeof(EndpointDescriptor_t),
            bDescriptorType:    dtEndpoint,
            bEndpointAddress:   (EP_DIR_OUT | EP_BULK_OUT_ADDR),
            bmAttributes:       (EP_TYPE_BULK | EP_ATTR_NO_SYNC | EP_USAGE_DATA),
            wMaxPacketSize:     EP_BULK_SZ,
            bInterval:          0
        },

        MS_DataInEndpoint: {
            bLength:            sizeof(EndpointDescriptor_t),
            bDescriptorType:    dtEndpoint,
            bEndpointAddress:   (EP_DIR_IN | EP_BULK_IN_ADDR),
            bmAttributes:       (EP_TYPE_BULK | EP_ATTR_NO_SYNC | EP_USAGE_DATA),
            wMaxPacketSize:     EP_BULK_SZ,
            bInterval:          0
        }
};
#endif

#if 1 // ==== Strings ====
// U.S. English language identifier
static const StringDescriptor_t LanguageString = {
        bLength: USB_STRING_LEN(2),
        bDescriptorType: dtString,
        bString: {0x09, 0x04}   // == 0409
};

// Vendor string
static const StringDescriptor_t ManufacturerString = {
        bLength: USB_STRING_LEN(6),
        bDescriptorType: dtString,
        bString: {'W', 'W', 'P', 'a', 's', 's'}
};

// Device Description string
static const StringDescriptor_t ProductString = {
        bLength: USB_STRING_LEN(12),
        bDescriptorType: dtString,
        bString: {'M','a','s','s',' ','S','t','o','r','a','g','e'}
};

// Device Description string
static const StringDescriptor_t SerialNumber = {
        bLength: USB_STRING_LEN(12),
        bDescriptorType: dtString,
        bString: {'0','1','2','3','4','5','6','7','8','9','A','B'}
};
#endif

#if 1 // ==== GetDescriptor function ====
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
                case 3:
                    *PPtr = (uint8_t*)&SerialNumber;
                    *PLen = SerialNumber.bLength;
                    break;
                default: break;
            }
            break;
        default:
            *PLen = 0;
            break;
    }
}
#endif
