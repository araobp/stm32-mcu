/*
 * dct.c
 *
 *  Created on: 2019/01/08
 *
 *  Discrete Cosine Transform
 *
 */

#include <stdlib.h>
#include "math.h"
#include "dct.h"

float32_t c_k(int k) {
  return (k == 0) ? 1.0 / sqrt(2.0) : 1.0;
}

/**
 * @brief  Initialization function for the DCT2.
 * @param[in]     *S         points to an instance of floating-point DCT2 structure.
 * @param[in]     dctLen     length of the DCT2.
 * @return        none.
 */
void dct2_init_f32(dct2_instance_f32 *S, uint16_t dctLen) {
  float32_t *pDataW = NULL;
  float32_t *pDataW_I = NULL;
  float32_t *pDataX = NULL;
  S->len = dctLen;

  arm_mat_init_f32(&(S->Y), dctLen, 1, NULL);

  pDataW = (float32_t *) (calloc(dctLen * dctLen, sizeof(float32_t)));
  arm_mat_init_f32(&(S->W), dctLen, dctLen, pDataW);

  pDataW_I = (float32_t *) (calloc(dctLen * dctLen, sizeof(float32_t)));
  arm_mat_init_f32(&(S->W_I), dctLen, dctLen, pDataW_I);

  pDataX = (float32_t *) (calloc(dctLen, sizeof(float32_t)));
  arm_mat_init_f32(&(S->X), dctLen, 1, pDataX);

  for (int k = 0; k < S->len; k++) {
    for (int n = 0; n < S->len; n++) {
      S->W.pData[k * S->len + n] = arm_cos_f32(
          (k * (2 * n + 1) * M_PI) / (2 * S->len));
    }
  }

  for (int n = 0; n < S->len; n++) {
    for (int k = 0; k < S->len; k++) {
      S->W_I.pData[n * S->len + k] = c_k(k)
          * arm_cos_f32((k * (2 * n + 1) * M_PI) / (2 * S->len));
    }
  }

}

/**
 * @brief DCT Type-II. The definition is same as scipy.fftpack.dct's definition.
 * @param[in]     *S         points to an instance of floating-point DCT2 structure.
 * @param[in]     *pSrc      points to the input buffer.
 * @param[out]    *pDst      points to the output buffer.
 * @param[in]     idctFlag   DCT if flag is 0, IDCT if flag is 1.
 * @return        none.
 */
void dct2_f32(dct2_instance_f32 *S, float32_t *pSrc, float32_t *pDst,
    uint8_t idctFlag) {
  arm_copy_f32(pSrc, S->X.pData, S->len);
  S->Y.pData = pDst;

  if (idctFlag == 0) {
    arm_mat_mult_f32(&(S->W), &(S->X), &(S->Y));
    pDst[0] = pDst[0] * c_k(0);
    arm_scale_f32(pDst, (float32_t) (sqrt(2.0 / S->len)), pDst, S->len);
  } else {
    arm_mat_mult_f32(&(S->W_I), &(S->X), &(S->Y));
    arm_scale_f32(pDst, (float32_t) (sqrt(2.0 / S->len)), pDst, S->len);
  }
}
