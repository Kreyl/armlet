/*
 * descriptors.h
 *
 *  Created on: Jul 5, 2013
 *      Author: g.kruglov
 */

#ifndef DESCRIPTORS_H_
#define DESCRIPTORS_H_

#include <inttypes.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Sizes
#define EP0_SZ              64
#define EP_INTERRUPT_SZ     8
#define EP_BULK_SZ          64

typedef struct {
    uint8_t Indx;
    uint16_t Type;
    uint16_t InMaxsize;
    uint16_t OutMaxsize;
} EpCfg_t;

#define EP_CNT                  4   // Control, Notification, In & Out
extern const EpCfg_t EpCfg[EP_CNT];
// See descriptors.c
#define EP_NOTIFICATION_INDX    1
#define EP_BULK_OUT_INDX        2
#define EP_BULK_IN_INDX         3

void GetDescriptor(uint8_t Type, uint8_t Indx, uint8_t **PPtr, uint32_t *PLen);

enum USB_DescriptorTypes_t {
    dtDevice                    = 0x01, // device descriptor
    dtConfiguration             = 0x02, // configuration descriptor
    dtString                    = 0x03, // string descriptor
    dtInterface                 = 0x04, // interface descriptor
    dtEndpoint                  = 0x05, // endpoint descriptor
    dtDeviceQualifier           = 0x06, // device qualifier descriptor
    dtOther                     = 0x07, // other type
    dtInterfacePower            = 0x08, // interface power descriptor
    dtInterfaceAssociation      = 0x0B, // interface association descriptor
    dtCSInterface               = 0x24, // class specific interface descriptor
    dtCSEndpoint                = 0x25, // class specific endpoint descriptor
};

#if 1 // ================= Standard descriptor types ===========================
// Device
typedef struct {
    uint8_t  bLength;               // Size of the descriptor, in bytes
    uint8_t  bDescriptorType;       // Type of the descriptor
    uint16_t bcdUSB;                // BCD of the supported USB specification
    uint8_t  bDeviceClass;          // USB device class
    uint8_t  bDeviceSubClass;       // USB device subclass
    uint8_t  bDeviceProtocol;       // USB device protocol
    uint8_t  bMaxPacketSize0;       // Size of the control (address 0) endpoint's bank in bytes
    uint16_t idVendor;              // Vendor ID for the USB product
    uint16_t idProduct;             // Unique product ID for the USB product
    uint16_t bcdDevice;             // Product release (version) number
    uint8_t  iManufacturer;         // String index for the manufacturer's name
    uint8_t  iProduct;              // String index for the product name/details
    uint8_t  iSerialNumber;         // String index for the product's globally unique hexadecimal serial number, in uppercase Unicode ASCII.
    uint8_t  bNumConfigurations;    // Total number of configurations supported by the device
} __attribute__ ((__packed__)) DeviceDescriptor_t ;

// Configuration header
typedef struct {
    uint8_t  bLength;               // Size of the descriptor, in bytes
    uint8_t  bDescriptorType;       // Type of the descriptor
    uint16_t wTotalLength;          // Size of the configuration descriptor header
    uint8_t  bNumInterfaces;        // Total number of interfaces in the configuration
    uint8_t  bConfigurationValue;   // Configuration index of the current configuration
    uint8_t  iConfiguration;        // Index of a string descriptor describing the configuration
    uint8_t  bmAttributes;          // Configuration attributes. On all devices, this should include USB_CONFIG_ATTR_RESERVED at a minimum
    uint8_t  bMaxPower;             // Maximum power consumption of the device
} __attribute__ ((__packed__)) ConfigHeader_t;

// Interface descriptor
typedef struct {
    uint8_t  bLength;                // Size of the descriptor, in bytes
    uint8_t  bDescriptorType;        // Type of the descriptor, given by the specific class
    uint8_t  bInterfaceNumber;       // Index of the interface in the current configuration
    uint8_t  bAlternateSetting;      // Alternate setting for the interface number
    uint8_t  bNumEndpoints;          // Total number of endpoints in the interface
    uint8_t  bInterfaceClass;        // Interface class ID
    uint8_t  bInterfaceSubClass;     // Interface subclass ID
    uint8_t  bInterfaceProtocol;     // Interface protocol ID
    uint8_t  iInterface;             // Index of the string descriptor describing the interface
} __attribute__ ((__packed__)) InterfaceDescriptor_t;

// Endpoint descriptor
typedef struct {
    uint8_t  bLength;           // Size of the descriptor, in bytes
    uint8_t  bDescriptorType;   // Type of the descriptor, given by the specific class
    uint8_t  bEndpointAddress;  // Logical address of the endpoint within the device for the current configuration, including direction mask
    uint8_t  bmAttributes;      // Endpoint attributes
    uint16_t wMaxPacketSize;    // Size of the endpoint bank, in bytes
    uint8_t  bInterval;         // Polling interval in milliseconds for the endpoint if it is an INTERRUPT or ISOCHRONOUS type
} __attribute__ ((__packed__)) EndpointDescriptor_t;

typedef struct {
    uint8_t  bLength;                // Size of the descriptor, in bytes
    uint8_t  bDescriptorType;        // Type of the descriptor, given by the specific class
    uint16_t  bString[];
} __attribute__ ((__packed__)) StringDescriptor_t;
#endif

#if 1 // ========================== CDC descriptors ============================
typedef struct {
    uint8_t  bFunctionLength;   // Size of the descriptor, in bytes
    uint8_t  bDescriptorType;   // Type of the descriptor, either a value in USB_DescriptorTypes_t or a value given by the specific class.
    uint8_t  bDescriptorSubType;// Sub type value used to distinguish between CDC class-specific descriptors, must be CDC_DSUBTYPE_CSInterface_Header.
    uint16_t bcdCDC;            // Version number of the CDC specification implemented by the device, encoded in BCD format.
} __attribute__ ((__packed__)) CDCFuncHeader_t;

typedef struct {
    uint8_t bFunctionLength;    // Size of the descriptor, in bytes.
    uint8_t bDescriptorType;    // Type of the descriptor, either a value in USB_DescriptorTypes_t or a value given by the specific class.
    uint8_t bDescriptorSubType; // Sub type value used to distinguish between CDC class-specific descriptors, must be CDC_DSUBTYPE_CSInterface_ACM.
    uint8_t bmCapabilities;     // Capabilities of the ACM interface, given as a bit mask. For most devices, this should be set to a fixed value of 0x06 - for other capabilities, refer to the CDC ACM specification.
} __attribute__ ((__packed__)) CDCFuncACM_t;

typedef struct {
    uint8_t bFunctionLength;    // Size of the descriptor, in bytes.
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubType; // Sub type value used to distinguish between CDC class-specific descriptors, must be CDC_DSUBTYPE_CSInterface_Union.
    uint8_t bMasterInterface;   // Interface number of the CDC Control interface.
    uint8_t bSlaveInterface0;   // Interface number of the CDC Data interface.
} __attribute__ ((__packed__)) CDCFuncUnion_t;

typedef struct {
    ConfigHeader_t          ConfigHeader;       // Standard config header
    // CDC Control Interface
    InterfaceDescriptor_t   CCI_Interface;      // Standard interface descriptor
    CDCFuncHeader_t         FuncHeader;
    CDCFuncACM_t            FuncAcm;
    CDCFuncUnion_t          FuncUnion;
    EndpointDescriptor_t    NotificationEndpoint;
    // CDC Data Interface
    InterfaceDescriptor_t   DCI_Interface;      // Standard interface descriptor
    EndpointDescriptor_t    DataOutEndpoint;
    EndpointDescriptor_t    DataInEndpoint;
} __attribute__ ((__packed__)) ConfigDescriptor_t;
#endif


#ifdef __cplusplus
}
#endif

#endif /* DESCRIPTORS_H_ */
