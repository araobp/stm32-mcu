#ifndef __AI_H__
#define __AI_H__

#include <stdint.h>
#include "ai_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

int ai_init(void);
int ai_infer(ai_float *raw_data);

#ifdef __cplusplus
}
#endif

#endif /* __AI_H__ */
