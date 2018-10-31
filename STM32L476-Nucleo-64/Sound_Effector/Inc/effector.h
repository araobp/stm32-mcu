/*
 * effector.h
 *
 *  Created on: 2018/10/11
 */

#ifndef EFFECTOR_H_
#define EFFECTOR_H_

#include "arm_math.h"

// FIR filters
typedef enum {
  LPF,  // Low Pass Filter
  HPF,  // High Pass Filter
  BPF,   // Band Pass filter
  PSEUDO_STEREO
} fir_filter;

#define PSEUDO_STEREO_DELAY 0.005f  // 5msec

void init_effector(float32_t sampling_frequency);

void distortion_hard(float32_t *signal, float32_t gain, float32_t level);

void compressor(float32_t *signal, float32_t threshold, float32_t ratio, float32_t gain);

void auto_pan(float32_t *signal1, float32_t *signal2, float32_t depth, float32_t rate);

void init_fir(float32_t fe1, float32_t fe2, int j, fir_filter mode);

void fir(float32_t *signal1, float32_t *signal2);

//void noise_supressor(float32_t *signal, float32_t threshold);

#endif /* EFFECTOR_H_ */
