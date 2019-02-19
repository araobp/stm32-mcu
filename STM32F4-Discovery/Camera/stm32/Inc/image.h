/*
 * image.h
 *
 *  Created on: 2019/02/19
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdint.h>
#include "ov7670.h"

void qcif_to_128x128(uint16_t src[QCIF_HEIGHT][QCIF_WIDTH], uint16_t dst[128][128]);

void qcif_to_32x32(uint16_t src[QCIF_HEIGHT][QCIF_WIDTH], uint16_t dst[32][32]);

#endif /* IMAGE_H_ */
