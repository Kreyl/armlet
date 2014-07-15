/*
 * flashloader_support.h
 *
 *  Created on: 27.01.2013
 *      Author: Roma Jam
 */

#ifndef FLASHLOADER_SUPPORT_H_
#define FLASHLOADER_SUPPORT_H_


static const uint32_t FLASHLOADER_START_ADDR = 0x1FFF0000; // System memory Information Block

void JumpToFlashLdr(void);
#endif /* FLASHLOADER_SUPPORT_H_ */
