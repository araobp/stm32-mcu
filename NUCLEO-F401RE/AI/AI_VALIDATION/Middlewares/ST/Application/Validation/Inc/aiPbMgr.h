/**
  ******************************************************************************
  * @file    aiPbMgr.h
  * @author  MCD Vertical Application Team
  * @brief   Helper function for AI ProtoBuffer support
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

#ifndef _AI_PB_MGR_H_
#define _AI_PB_MGR_H_

#include "ai_platform.h"

#include "pb.h"
#include "stm32msg.pb.h"

#ifndef AI_PB_TEST
// #define AI_PB_TEST 1
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------- */

typedef struct _aiPbCmdFunc {
        EnumCmd cmd;
        void (*process)(const reqMsg *req, respMsg *resp, void *param);
        void *param;
} aiPbCmdFunc;

void aiPbMgrInit(const aiPbCmdFunc *funcs);

int aiPbMgrWaitAndProcess(void);

/* --------------------------- */

void aiPbMgrSendAck(const reqMsg *req, respMsg *resp,
        EnumState state, uint32_t param);

void aiPbMgrSendResp(const reqMsg *req, respMsg *resp, EnumState state);

bool aiPbMgrReceiveAiBuffer(const reqMsg *req, respMsg *resp,
        EnumState state, ai_buffer *buffer, bool check);

#define PB_BUFFER_TYPE_SEND_WITHOUT_DATA ((uint32_t)(1U << 31))

bool aiPbMgrSendAiBuffer(const reqMsg *req, respMsg *resp, EnumState state,
        uint32_t type, uint32_t id, ai_float dur_ms, const ai_buffer *buffer);

bool aiPbMgrSendLog(const reqMsg *req, respMsg *resp,
        EnumState state, uint32_t lvl, const char *str);

void aiPbMgrSendNNInfo(const reqMsg *req, respMsg *resp,
        EnumState state, const ai_network_report *nn);

bool aiPbMgrWaitAck(void);

/* --------------------------- */

uint32_t aiPbAiBufferSize(ai_buffer *buffer);
void aiPbStrCopy(const char *src, char *dst, uint32_t max);
uint32_t aiPbVersionToUint32(const ai_platform_version *ver);

/* --------------------------- */

void aiPbCmdSync(const reqMsg *req, respMsg *resp, void *param);
#define AI_PB_CMD_SYNC(par) { EnumCmd_CMD_SYNC, &aiPbCmdSync, (par) }

void aiPbCmdSysInfo(const reqMsg *req, respMsg *resp, void *param);
#define AI_PB_CMD_SYS_INFO(par) { EnumCmd_CMD_SYS_INFO, &aiPbCmdSysInfo, (par) }

#define AI_PB_CMD_END      { (EnumCmd)0, NULL, NULL }

#if defined(AI_PB_TEST) && (AI_PB_TEST == 1)
void aiPbCmdTest(const reqMsg *req, respMsg *resp, void *param);
#define AI_PB_CMD_TEST(par) { EnumCmd_CMD_TEST, &aiPbCmdTest, (par) }
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _AI_PB_MGR_H_ */
