/*
 * descriptors.cpp
 *
 *  Created on: Jul 5, 2013
 *      Author: g.kruglov
 */

#include "descriptors.h"
#include "buf_sizes.h"

/*
 * Descriptors of VCP
 */

#if 1 // ==== Constants (not to change) ====
#define EP_DIR_IN           0x80
#define EP_DIR_OUT          0x00
// Descriptor-type endpoint codes
#define EP_TYPE_CONTROL     0x00
#define EP_TYPE_ISOCHRONOUS 0x01
#define EP_TYPE_BULK        0x02
#define EP_TYPE_INTERRUPT   0x03

// Endpoint attributes
#define EP_ATTR_NO_SYNC     (0 << 2)
#define EP_ATTR_ASYNC       (1 << 2)
#define EP_USAGE_DATA       (0 << 4)
#define EP_USAGE_FEEDBACK   (1 << 4)
#endif

#if 1 // ==== Endpoints config ====
const EpCfg_t EpCfg[EP_CNT] = {
        // Control endpoint
        {
            Indx:       0,
            Type:       EP_TYPE_CONTROL,
            InMaxsize:  EP0_SZ,
            OutMaxsize: EP0_SZ,
        },
        // Interrupt endpoint
        {
            Indx:       1,
            Type:       EP_TYPE_INTERRUPT,
            InMaxsize:  EP_INTERRUPT_SZ,
            OutMaxsize: 0,
        },
        // Bulk Out endpoint
        {
            Indx:       2,
            Type:       EP_TYPE_BULK,
            InMaxsize:  0,
            OutMaxsize: EP_BULK_SZ,
        },
        // Bulk In endpoint
        {
            Indx:       3,
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

// Device
static const DeviceDescriptor_t DeviceDescriptor = {
        bLength:            sizeof(DeviceDescriptor_t),   // Size
        bDescriptorType:    dtDevice,        // Descriptor type
        bcdUSB:             0x0200,          // USB 2.0
        bDeviceClass:       0x02,            // bDeviceClass (CDC)
        bDeviceSubClass:    0x00,            // bDeviceSubClass
        bDeviceProtocol:    0x00,            // bDeviceProtocol
        bMaxPacketSize0:    EP0_SZ,          // bMaxPacketSize0
        idVendor:           0x0483,          // idVendor ()
        idProduct:          0x5740,          // idProduct; #9 means virtual COM
        bcdDevice:          0x0200,               // bcdDevice
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
            bNumInterfaces:     2,
            bConfigurationValue: 1,
            iConfiguration:     0,
            bmAttributes:       0x80,   // USB_CONFIG_ATTR_RESERVED
            bMaxPower:          USB_CONFIG_POWER_MA(100)
        },

        // CDC Control Interface
        CCI_Interface: {
            bLength:            sizeof(InterfaceDescriptor_t),
            bDescriptorType:    dtInterface,
            bInterfaceNumber:   0,
            bAlternateSetting:  0,
            bNumEndpoints:      1,
            bInterfaceClass:    0x02, // Descriptor Class value indicating that the device or interface belongs to the CDC class
            bInterfaceSubClass: 0x02, // Descriptor Subclass value indicating that the device or interface belongs to the Abstract Control Model CDC subclass
            bInterfaceProtocol: 0x01, // Descriptor Protocol value indicating that the device or interface belongs to the AT Command protocol of the CDC class
            iInterface:         0     // No string
        },

        FuncHeader: {
            bFunctionLength:    sizeof(CDCFuncHeader_t),
            bDescriptorType:    dtCSInterface,
            bDescriptorSubType: 0x00, // CDC class-specific Header functional descriptor
            bcdCDC:             0x0110
        },

        FuncAcm: {
            bFunctionLength:    sizeof(CDCFuncACM_t),
            bDescriptorType:    dtCSInterface,
            bDescriptorSubType: 0x02, // CDC class-specific Abstract Control Model functional descriptor
            bmCapabilities:     0x06
        },

        FuncUnion: {
            bFunctionLength:    sizeof(CDCFuncUnion_t),
            bDescriptorType:    dtCSInterface,
            bDescriptorSubType: 0x06, // CDC class-specific Union functional descriptor
            bMasterInterface:   0,
            bSlaveInterface0:   1
        },

        NotificationEndpoint: {
            bLength:            sizeof(EndpointDescriptor_t),
            bDescriptorType:    dtEndpoint,
            bEndpointAddress:   (EP_DIR_IN | EP_NOTIFICATION_INDX),
            bmAttributes:       (EP_TYPE_INTERRUPT | EP_ATTR_NO_SYNC | EP_USAGE_DATA),
            wMaxPacketSize:     EP_INTERRUPT_SZ,
            bInterval:          0xFF
        },

        // CDC Data interface
        DCI_Interface: {
            bLength:            sizeof(InterfaceDescriptor_t),
            bDescriptorType:    dtInterface,
            bInterfaceNumber:   1,
            bAlternateSetting:  0,
            bNumEndpoints:      2,
            bInterfaceClass:    0x0A, // Descriptor Class value indicating that the device or interface belongs to the CDC Data class
            bInterfaceSubClass: 0x00, // Descriptor Subclass value indicating that the device or interface belongs to no specific subclass of the CDC data class
            bInterfaceProtocol: 0x00, // Descriptor Protocol value indicating that the device or interface belongs to no specific protocol of the CDC data class
            iInterface:         0     // No string
        },

        DataOutEndpoint: {
            bLength:            sizeof(EndpointDescriptor_t),
            bDescriptorType:    dtEndpoint,
            bEndpointAddress:   (EP_DIR_OUT | EP_BULK_OUT_INDX),
            bmAttributes:       (EP_TYPE_BULK | EP_ATTR_NO_SYNC | EP_USAGE_DATA),
            wMaxPacketSize:     EP_BULK_SZ,
            bInterval:          0x05
        },

        DataInEndpoint: {
            bLength:            sizeof(EndpointDescriptor_t),
            bDescriptorType:    dtEndpoint,
            bEndpointAddress:   (EP_DIR_IN | EP_BULK_IN_INDX),
            bmAttributes:       (EP_TYPE_BULK | EP_ATTR_NO_SYNC | EP_USAGE_DATA),
            wMaxPacketSize:     EP_BULK_SZ,
            bInterval:          0x05
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
