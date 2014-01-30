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

// Endpoint Sizes for Full-Speed devices
#define EP0_SZ              64  // Control Endpoint must have a packet size of 64 bytes
#define EP_INTERRUPT_SZ     8   // Max size is 64 bytes
#define EP_BULK_SZ          64  // Max size is 64 bytes

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

// Device' Endpoints Configuration type
typedef struct {
    uint16_t Type;
    uint16_t InMaxsize;
    uint16_t OutMaxsize;
} EpCfg_t;

#define EP_CNT                  3   // Control, In & Out
extern const EpCfg_t EpCfg[EP_CNT];
// Endpoint addresses. Nothing to do with inner Indx value.
#define EP_BULK_OUT_ADDR        1
#define EP_BULK_IN_ADDR         2

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

#if 1 // ================= Mass Storage Device descriptors =====================
typedef struct {
    ConfigHeader_t          ConfigHeader;       // Standard config header
    // Mass Storage Interface
    InterfaceDescriptor_t   MS_Interface;
    EndpointDescriptor_t    MS_DataInEndpoint;
    EndpointDescriptor_t    MS_DataOutEndpoint;
} __attribute__ ((__packed__)) ConfigDescriptor_t;
#endif


#ifdef __cplusplus
}
#endif

#endif /* DESCRIPTORS_H_ */
