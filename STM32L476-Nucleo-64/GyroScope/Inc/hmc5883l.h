/*
 * hmc5883l.h
 *
 *  Created on: 2018/07/20
 */

#ifndef HMC5883L_H_
#define HMC5883L_H_

#include <stdint.h>

// HMC5883L calibration
#define X_MAX 263
#define X_MIN -326
#define Y_MAX 55
#define Y_MIN -537

void init_hmc5883l(void);
double get_radian(void);
int16_t get_degree(void);

#endif /* HMC5883L_H_ */
