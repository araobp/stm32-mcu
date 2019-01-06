/**
  ******************************************************************************
  * @file    aiTestUtility.h
  * @author  MCD Vertical Application Team
  * @brief   Implementation of the helper functions for AI test application
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) YYYY STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifndef _AI_TEST_UTILITY_H_
#define _AI_TEST_UTILITY_H_

#include <stdint.h>
#include <ai_platform.h>


/* device-related functions */

struct dwtTime {
    uint32_t fcpu;
    int s;
    int ms;
    int us;
};

void crcIpInit(void);
void dwtIpInit(void);
void dwtReset(void);
uint32_t dwtGetCycles(void);
uint32_t systemCoreClock(void);
int dwtCyclesToTime(uint64_t clks, struct dwtTime *t);
float dwtCyclesToFloatMs(uint64_t clks);

void logDeviceConf(void);

uint32_t getFlashCacheConf(void);

uint32_t disableInts(void);
void restoreInts(uint32_t state);


/* low-level I/O functions */

void ioDisableWrite(void);
void ioFlushWrite(void);
void ioFlushRead(void);

/* AI-related functions */

void aiLogErr(const ai_error err, const char *fct);
ai_u32 aiBufferSize(const ai_buffer* buffer);
void aiPrintNetworkInfo(const ai_network_report* report);


/* test-related functions */

void aiTestHeader(const char* tname, int major, int minor);


/* nanopb-related functions */

#include <pb.h>

pb_ostream_t pb_ostream_to_uart(int fd);
pb_istream_t pb_istream_from_uart(int fd);

#endif /* _AI_TEST_UTILITY_H_ */
