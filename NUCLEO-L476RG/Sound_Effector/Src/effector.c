/*
 * effector.c
 *
 *  Created on: 2018/10/11
 *
 *  Reference: http://floor13.sakura.ne.jp/book03/book03.html
 */

#include "effector.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "main.h"

const float32_t MAX_AMPLITUDE = 0x7fff;  // int16_t
float32_t fs = 0.0f;  // Sampling frequency

arm_rfft_fast_instance_f32 S;  // RFFT instance

// impulse responses
float32_t impulse_response1[NN*2] = { 0.0f };
float32_t impulse_response2[NN*2] = { 0.0f };

// Buffers
float32_t buf1[NN * 2] = { 0.0f };
float32_t buf2[NN * 2] = { 0.0f };

// Hann window
float32_t hann_window[NN*2] = { 0.0f };

// hann window generation
void hann(int num) {
  arm_fill_f32(0.0f, hann_window, NN*2);
  float32_t scale = 2.0f * PI / (float32_t) num;
  for (int n = 0; n < num; n++) {
    hann_window[n] = 0.5f - 0.5f * arm_cos_f32((float) n * scale);
  }
}

// sinc function
float32_t sinc(float32_t x) {
  float32_t y;
  if (x == 0.0) {
    y = 1.0;
  } else {
    y = arm_sin_f32(x) / x;
  }
  return y;
}

/*
 * Effector initialization
 */
void init_effector(float32_t sampling_frequency) {
  // Generate Hanning window
  fs = sampling_frequency;
  arm_rfft_fast_init_f32(&S, NN*2);
}

/*
 * Distortion hard clipping
 */
void distortion_hard(float32_t *signal, float32_t gain, float32_t level) {
  float32_t s;
  for (int n = 0; n < NN; n++) {
    // Amplify
    s = signal[n] * gain;
    // Hard-clip
    if (s > MAX_AMPLITUDE) {
      s = MAX_AMPLITUDE;
    } else if (s < -MAX_AMPLITUDE) {
      s = -MAX_AMPLITUDE;
    }
    // Level control
    signal[n] = s * level;
  }
}

/*
 * Compressor
 */
void compressor(float32_t *signal, float32_t threshold, float32_t ratio,
    float32_t gain) {
  threshold = threshold * MAX_AMPLITUDE;
  for (int n = 0; n < NN; n++) {
    if (signal[n] > threshold) {
      signal[n] = threshold + (signal[n] - threshold) * ratio;
    } else if (signal[n] < threshold) {
      signal[n] = -threshold + (signal[n] + threshold) * ratio;
    }
    signal[n] *= gain;
  }
}

/*
 * Auto panorama
 */
void auto_pan(float32_t *signal1, float32_t *signal2, float32_t depth, float32_t rate) {
  static uint32_t nn = 0;
  uint32_t m;
  float32_t arg;
  float32_t a_l;
  float32_t a_r;

  arg = 2.0*PI*rate/fs;
  m = (uint32_t)(fs/rate);

  for (int n = 0; n < NN; n++) {
    if (nn == m) nn = 0;  // i.e., arg*nn == 2*PI
    a_l = 1.0 + depth * arm_sin_f32(arg*nn);
    a_r = 1.0 - depth * arm_sin_f32(arg*nn);
    signal1[n] = signal1[n] * a_l;
    signal2[n] = signal2[n] * a_r;
    nn++;
  }
}

// overlap addition for fir filter
void overlap_add(float32_t *in, float32_t *out, int dev_id) {
  static float32_t wave1[NN] = { 0.0f };
  static float32_t wave2[NN] = { 0.0f };
  if (dev_id == 1){
    arm_add_f32(in, wave1, in, NN);
    arm_copy_f32(in+NN, wave1, NN);
  } else if (dev_id == 2) {
    arm_add_f32(in, wave2, in, NN);
    arm_copy_f32(in+NN, wave2, NN);
  }
  arm_copy_f32(in, out, NN);
}

/*
 * FIR filter initialization
 *
 * fe: edge frequency
 * j: number of filter coefficients - 1
 * mode: 0 for LPR
 */
void init_fir(float32_t fe1, float32_t fe2, int j, fir_filter mode) {

  int m;
  int offset;
  int num_delay = 0;

  // Hann-window generation
  offset = j / 2;
  hann(j + 1);

  // Normalization
  fe1 = fe1/fs;
  fe2 = fe2/fs;

  // Zero clear
  arm_fill_f32(0.0f, impulse_response1, NN*2);
  arm_fill_f32(0.0f, impulse_response2, NN*2);
  arm_fill_f32(0.0f, buf1, NN*2);

  switch (mode) {

  case LPF:
    for (m = -j / 2; m <= j / 2; m++) {
      impulse_response1[offset + m] = 2.0 * fe1 * sinc(2.0 * PI * fe1 * m);
    }
    arm_copy_f32(impulse_response1, impulse_response2, j+1);
    break;

  case HPF:
    for (m = -j / 2; m <= j / 2; m++) {
      impulse_response1[offset + m] = sinc(PI * m) - 2.0 * fe1 * sinc(2.0 * PI * fe1 * m);
    }
    arm_copy_f32(impulse_response1, impulse_response2, j+1);
    break;

  case BPF:
    for (m = -j / 2; m <= j / 2; m++) {
      impulse_response1[offset + m] = 2.0 * fe2 * sinc(2.0 * PI * fe2 * m) - 2.0 * fe1 * sinc(2.0 * PI * fe1 * m);
    }
    arm_copy_f32(impulse_response1, impulse_response2, j+1);
    break;

  case PSEUDO_STEREO:
    num_delay = PSEUDO_STEREO_DELAY * fs;
    impulse_response1[0] = 1.0f;
    impulse_response2[0] = 1.0f;
    impulse_response1[num_delay] = 1.0f;
    impulse_response2[num_delay] = -1.0f;
    break;

  default:
    break;
  }

  // Windowing (Hann-window)
  arm_mult_f32(impulse_response1, hann_window, impulse_response1, j+1);
  arm_mult_f32(impulse_response2, hann_window, impulse_response2, j+1);

  // FFT of impulse response
  arm_rfft_fast_f32(&S, impulse_response1, buf1, 0);
  arm_copy_f32(buf1, impulse_response1, NN*2);
  arm_rfft_fast_f32(&S, impulse_response2, buf1, 0);
  arm_copy_f32(buf1, impulse_response2, NN*2);

}

/*
 * FIR filter
 */
void fir(float32_t *signal1, float32_t *signal2) {

  arm_fill_f32(0.0f, buf1 + NN, NN);
  arm_copy_f32(signal1, buf1, NN);
  arm_rfft_fast_f32(&S, buf1, buf2, 0);
  arm_cmplx_mult_cmplx_f32(impulse_response1, buf2, buf1, NN);
  arm_rfft_fast_f32(&S, buf1, buf2, 1);
  overlap_add(buf2, signal1, 1);

  arm_fill_f32(0.0f, buf1 + NN, NN);
  arm_copy_f32(signal2, buf1, NN);
  arm_rfft_fast_f32(&S, buf1, buf2, 0);
  arm_cmplx_mult_cmplx_f32(impulse_response2, buf2, buf1, NN);
  arm_rfft_fast_f32(&S, buf1, buf2, 1);
  overlap_add(buf2, signal2, 2);

}

/* CMSIS DSP does not support atan2, so this function takes time too much.
 void spectrum_subtraction(float32_t *s, float32_t offset) {

 float32_t theta[NN];
 float32_t magnitude[NN];
 int re = 0;
 int im = 0;

 // Windowing
 //arm_cmplx_mult_real_f32(s, hann_window, s, NN);

 // to frequency domain
 arm_cfft_f32(&arm_cfft_sR_f32_len1024, s, 0, 1);

 // poler coordinate: calculate angle
 for (int n = 0; n < NN; n++) {
 re = n * 2;
 im = re + 1;
 theta[n] = atan2(s[im], s[re]);
 }

 // poler coordinate: calculate magnitude
 arm_cmplx_mag_f32(s, magnitude, NN);

 // white noise subtraction
 for (int n = 0; n < NN; n++) {
 magnitude[n] = magnitude[n] - offset;
 if (magnitude[n] < 0.0) magnitude[n] = 0.0;
 }

 // turn it into rectangular coordinate
 for (int n = 0; n < NN; n++) {
 re = n * 2;
 im = re + 1;
 s[re] = magnitude[n] * arm_cos_f32(theta[n]);
 s[im] = magnitude[n] * arm_sin_f32(theta[n]);
 //s[re] = magnitude[n] * cos(theta[n]);
 //s[im] = magnitude[n] * sin(theta[n]);
 }

 // back to time domain
 arm_cfft_f32(&arm_cfft_sR_f32_len1024, s, 1, 1);
 }

 void noise_supressor(float32_t *signal, float32_t threshold) {

 float32_t s[NN*2] = { 0.0f };  // complex version of signal
 float32_t temp[NN*2] = { 0.0f };
 float32_t a[NN] = { 0.0f };
 float32_t b[NN] = { 0.0f };
 int re = 0;
 int im = 0;

 // real -> complex conversion
 for (int n = 0; n < NN; n++) {
 re = n * 2;
 im = re + 1;
 s[re] = signal[n];
 s[im] = 0.0f;
 }

 arm_copy_f32(a, temp, NN);
 arm_copy_f32(&s[NN], a, NN);
 arm_copy_f32(&s[NN], &temp[NN], NN);

 spectrum_subtraction(temp, threshold);
 spectrum_subtraction(s, threshold);
 arm_copy_f32(&s[NN], b, NN);

 arm_add_f32(b, temp, temp, NN);
 arm_add_f32(s, &temp[NN], &temp[NN], NN);

 for (int n = 0; n < NN; n++) {
 re = n * 2;
 signal[n] = s[re];
 }
 }
 */
