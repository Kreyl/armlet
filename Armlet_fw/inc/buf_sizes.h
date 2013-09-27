/*
 * buf_sizes.h
 *
 *  Created on: Jul 24, 2013
 *      Author: g.kruglov
 */

#ifndef BUF_SIZES_H_
#define BUF_SIZES_H_

// ==== USB ====
#define EP0_SZ              64
#define EP_INTERRUPT_SZ     8
#define EP_BULK_SZ          64

// ==== CCID ====
#define CCID_HEADER_SZ      10  // Defined by Standard

/* "For extended APDU level the value shall be between 261 + 10 (header) and
 * 65544 +10, otherwise the minimum value is the wMaxPacketSize of the Bulk-OUT
 * endpoint", p.20 of CCID
 * 261 = 5 bytes of header in T0 + possible 256bytes of data
 * For reference, the absolute maximum block size for a
 * TPDU T=0 block is 260 bytes (5 bytes command; 255 bytes data), or for a
 * TPDU T=1 block is 259 bytes, or for a
 * short APDU T=1 block is 261 bytes, or for an
 * extended APDU T=1 block is 65544 bytes. (p.30)
 */
#define CCID_DATA_SZ        261 // May be greater than IFSC (254 max), this means chaining in Card.cpp
#define CCID_QUEUE_SZ       (CCID_HEADER_SZ + CCID_DATA_SZ) // USB OUT queue size

#define CCID_MSGBOX_SZ      4 // Count of message buffer

#endif /* BUF_SIZES_H_ */
