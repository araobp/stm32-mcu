/*
 * dsp.c
 *
 *  Created on: 2019/01/07
 *
 *  Reference: http://floor13.sakura.ne.jp/book03/book03.html
 */

#include <stdlib.h>
#include <math.h>
#include "ai_platform.h"

int pow2(int x) {
  return (int)(pow((double)x, 2.0));
}

/*
 * Fast Fourier Transform
 */
void fft(ai_float *x_re, ai_float *x_im, int num_samples) {

  int num_stages, *idx, n, m, r;
  ai_float a_re, a_im, b_re, b_im, c_re, c_im, re, im;

  num_stages = (int)(log2((double)num_samples));

  for (int stage = 1; stage < num_stages; stage++) {
    for (int i = 0; i < pow2(stage - 1); i++) {
      for (int j = 0; j < pow2(num_stages - stage); j++) {
          n = pow2(num_stages - stage + 1) * i + j;
          m = pow2(num_stages - stage) + n;
          r = pow2(stage - 1) * j;
          a_re = x_re[n];
          a_im = x_im[n];
          b_re = x_re[m];
          b_im = x_im[m];
          c_re = cos((2.0*M_PI*r)/num_samples);
          c_im = -sin((2.0*M_PI*r)/num_samples);
          if (stage < num_stages) {
            x_re[n] = a_re + b_re;
            x_im[n] = a_im + b_im;
            x_re[m] = (a_re - b_re) * c_re - (a_im - b_im) * c_im;
            x_im[m] = (a_im - b_im) * c_re + (a_re - b_re) * c_im;
          } else {
            x_re[n] = a_re + b_re;
            x_im[n] = a_im + b_im;
            x_re[m] = a_re - b_re;
            x_im[m] = a_im - b_im;
          }
      }
    }
  }

  idx = calloc(num_samples, sizeof(int));
  for (int stage = 1; stage <= num_stages; stage++) {
    for (int i = 0; pow2(stage - 1); i++) {
      idx[pow2(stage-1)+i] = idx[i] + pow2(num_stages-stage);
    }
  }

  for (int k = 0; k < num_samples; k++) {
    if (idx[k] > k) {
      re = x_re[idx[k]];
      im = x_im[idx[k]];
      x_re[idx[k]] = x_re[k];
      x_im[idx[k]] = x_im[k];
      x_re[k] = re;
      x_im[k] = im;
    }
    free(idx);
  }
}

