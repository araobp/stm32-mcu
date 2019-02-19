/*
 * image.c
 *
 *  Created on: 2019/02/19
 */

#include "image.h"

/**
 * QCIF to 128x128
 *
 * This function just cuts out a square of 128x128 pixels from QCIF.
 */
void qcif_to_128x128(uint16_t src[QCIF_HEIGHT][QCIF_WIDTH], uint16_t dst[128][128]) {
  for (int j=0; j<128; j++) {
    for (int i=0; i<128; i++) {
      dst[j][i] = src[j+8][i+22];
    }
  }
}

/**
 * QCIF to 32x32
 *
 * This function applies 128x128 pixel window to cut out a square from QCIF,
 * then takes averages of each 4x4 pixels for RGB.
 */
void qcif_to_32x32(uint16_t src[QCIF_HEIGHT][QCIF_WIDTH], uint16_t dst[32][32]) {
  uint16_t red, green, blue, pixel;
  for (int j=0; j<32; j++) {
    for (int i=0; i<32; i++) {
      red = 0;
      green = 0;
      blue = 0;
      // Sum of 16 pixels
      for (int l=0; l<4; l++) {
        for (int k=0; k<4; k++) {
          pixel = src[j*4+l+8][i*4+k+22];
          red += (pixel & 0b1111100000000000) >> 11;
          green += (pixel & 0b0000011111100000) >> 5;
          blue += blue & 0b0000000000011111;
        }
      }
      // Average of 16 pixels
      dst[j][i] = ((red/16) << 11) + ((green/16) << 5) + blue/16;
    }
  }
}
