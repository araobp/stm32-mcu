/*
 * chirp.c
 *
 *  Created on: 2018/06/07
 */

#include "arm_math.h"
#include "arm_const_structs.h"
#include "chirp.h"
#include "main.h"
#include "stdbool.h"

const float32_t WINDOW_SCALE = 2.0f * PI / (float)NN;
float hann_window[NN] = { 0.0f };

float32_t signal_buf[NN*2] = { 0.0f };

// FFT of reference down-chirp as a matched filter
float32_t matched_filter_down[NN*2] = { 0.0f };
// FFT of reference up-chirp as a matched filter
float32_t matched_filter_up[NN*2] = { 0.0f };

arm_rfft_fast_instance_f32 S;

void generate_ref_chirp(float *ref_chirp, bool up, float fs, float phase) {

  float freq;
  float arg;
  float t = 0.0;

  float time_frame = (float)NN / (float)fs;
  float delta_f = (F2 - F1) / time_frame;
  float delta_t = time_frame / (time_frame * (float)fs);

  float value;

  for (int n = 0; n < NN; n++) {
    if (up) freq = F1 + delta_f * t;  // Up chirp
    else freq = F2 - delta_f * t;  // Down chirp
    arg = 2.0 * PI * freq * t + phase;
    t = t + delta_t;
    value = arm_cos_f32(arg);
    ref_chirp[n] = value * AMPLITUDE;
  }
}

void windowing(float32_t *signal) {
  // Windowing
  arm_mult_f32(signal, hann_window, signal, NN);
}

void init_ref_chirp(float fs) {

  // RFFT initialization
  arm_rfft_fast_init_f32(&S, NN*2);

  // Generate reference chirp
  generate_ref_chirp(matched_filter_down, false, fs, -PI/2.0);

  // Generate Hanning window
  for (uint32_t n = 0; n < NN; n++) {
    hann_window[n] = 0.5f - 0.5f * arm_cos_f32((float)n * WINDOW_SCALE);
  }

  // Apply the window to reference chirp
  windowing(matched_filter_down);

  // Then FFT
  arm_rfft_fast_f32(&S, matched_filter_down, signal_buf, 0);
  arm_copy_f32(signal_buf, matched_filter_down, NN*2);
}

// Chirp compression with a matched filter via frequency domain
void compress_chirp(float32_t *signal) {
  //windowing(signal);
  arm_rfft_fast_f32(&S, signal, signal_buf, 0);
  arm_cmplx_mult_cmplx_f32(matched_filter_down, signal_buf, signal_buf, NN);
  arm_rfft_fast_f32(&S, signal_buf, signal, 1);
  arm_mult_f32(signal, signal, signal, NN*2);
  arm_scale_f32(signal, SCALE_DOWN, signal, NN*2);
}
