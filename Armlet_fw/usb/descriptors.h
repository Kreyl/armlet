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

typedef struct {
    uint8_t Indx;
    uint16_t Type;
    uint16_t InMaxsize;
    uint16_t OutMaxsize;
} EpCfg_t;

#define EP_CNT              4
extern const EpCfg_t EpCfg[EP_CNT];
// See descriptors.c
#define EP_INTERRUPT_INDX   1
#define EP_BULK_OUT_INDX    2
#define EP_BULK_IN_INDX     3

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

// Descriptor types
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

typedef struct {
    uint8_t  bLength;                // Size of the descriptor, in bytes
    uint8_t  bDescriptorType;        // Type of the descriptor, given by the specific class
    uint16_t bcdCCID;                // Integrated Circuit(s) Cards Interface Devices Specification Release Number in binary coded decimal
    uint8_t  bMaxSlotIndex;
    uint8_t  bVoltageSupport;
    uint32_t dwProtocols;
    uint32_t dwDefaultClock;
    uint32_t dwMaximumClock;
    uint8_t  bNumClockSupported;
    uint32_t dwDataRate;
    uint32_t dwMaxDataRate;
    uint8_t  bNumDataRatesSupported;
    uint32_t dwMaxIFSD;
    uint32_t dwSynchProtocols;
    uint32_t dwMechanical;
    uint32_t dwFeatures;
    uint32_t dwMaxCCIDMessageLength;
    uint8_t  bClassGetResponse;
    uint8_t  bClassEnvelope;
    uint16_t wLcdLayout;
    uint8_t  bPinSupport;
    uint8_t  bMaxCCIDBusySlots;
} __attribute__ ((__packed__)) SCRFuncDescriptor_t;

typedef struct {
    uint8_t  bLength;           // Size of the descriptor, in bytes
    uint8_t  bDescriptorType;   // Type of the descriptor, given by the specific class
    uint8_t  bEndpointAddress;  // Logical address of the endpoint within the device for the current configuration, including direction mask
    uint8_t  bmAttributes;      // Endpoint attributes
    uint16_t wMaxPacketSize;    // Size of the endpoint bank, in bytes
    uint8_t  bInterval;         // Polling interval in milliseconds for the endpoint if it is an INTERRUPT or ISOCHRONOUS type
} __attribute__ ((__packed__)) EndpointDescriptor_t;

typedef struct {
    ConfigHeader_t          ConfigHeader;       // Standard config header
    InterfaceDescriptor_t   SCR_Interface;      // Standard interface descriptor
    SCRFuncDescriptor_t     SCR_Functional;     // USB-ICC class specific descriptor
    EndpointDescriptor_t    InterruptEndpoint;
    EndpointDescriptor_t    DataOutEndpoint;
    EndpointDescriptor_t    DataInEndpoint;
} __attribute__ ((__packed__)) ConfigDescriptor_t;

typedef struct {
    uint8_t  bLength;                // Size of the descriptor, in bytes
    uint8_t  bDescriptorType;        // Type of the descriptor, given by the specific class
    uint16_t  bString[];
} __attribute__ ((__packed__)) StringDescriptor_t;

#ifdef __cplusplus
}
#endif

#endif /* DESCRIPTORS_H_ */
