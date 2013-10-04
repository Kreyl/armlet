/*
 * usb_vcp.h
 *
 *  Created on: Oct 4, 2013
 *      Author: g.kruglov
 */

#ifndef USB_VCP_H_
#define USB_VCP_H_

struct LineCoding_t {
    uint32_t Bitrate;
    uint8_t  Format;
    uint8_t  ParityType;
    uint8_t  Datatype;
};




#endif /* USB_VCP_H_ */
