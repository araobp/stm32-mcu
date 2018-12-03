/*
 * chirp.h
 *
 *  Created on: 2018/06/07
 */

#ifndef CHIRP_H_
#define CHIRP_H_

// Amplitude of reference chirp
#define AMPLITUDE 1.0f;

// Chirp sweep range
#define F1 15000.0f
#define F2 18000.0f

#define SCALE_DOWN 0.001f

void init_ref_chirp(float fs);
void compress_chirp(float32_t *signal);
void compress_ref_chirp(float32_t *signal);
void get_ref_chirp(float32_t *signal);

#endif /* CHIRP_H_ */
