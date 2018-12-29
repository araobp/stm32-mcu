/**
  ******************************************************************************
  * @file    aiValidation.c
  * @author  MCD Vertical Application Team
  * @brief   AI Validation application
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

/* Description:
 *
 * Main entry points for AI validation on-target process.
 *
 * History:
 *  - v1.0 - Initial version
 *
 */

#ifndef HAS_INSPECTOR
#define HAS_INSPECTOR
#endif

/* System headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include <aiValidation.h>
#include <aiTestUtility.h>
#include <bsp_ai.h>

/* AI header files */
#include "ai_platform.h"
#include "core_datatypes.h"   /* AI_PLATFORM_RUNTIME_xxx definition */

#ifdef HAS_INSPECTOR
#include "ai_network_inspector.h"
#include "core_net_inspect.h"
#include "log.h"
#endif

#ifndef AI_MNETWORK_NUMBER
#define AI_MNETWORK_NUMBER (1)
#include "network.h"
#include "network_data.h"
#endif

#ifdef HAS_INSPECTOR
#if MIN_HEAP_SIZE < 0x800
#error Minimum HEAP size is expected (>=2KiB). Conf. should be modified in CubeMX.
#endif
#endif

/* */
#include "aiPbMgr.h"


/* -----------------------------------------------------------------------------
 * TEST-related definitions
 * -----------------------------------------------------------------------------
 */

/* APP configuration 0: disabled 1: enabled */
#define _APP_DEBUG_         			0

#define _APP_VERSION_MAJOR_     (0x01)
#define _APP_VERSION_MINOR_     (0x00)
#define _APP_VERSION_   ((_APP_VERSION_MAJOR_ << 8) | _APP_VERSION_MINOR_)

#define _APP_NAME_   "AI Validation"

/* -----------------------------------------------------------------------------
 * AI-related functions
 * -----------------------------------------------------------------------------
 */

static struct ai_network_exec_ctx {
    ai_handle network;
    ai_network_report report;
#ifdef HAS_INSPECTOR
    ai_handle inspector;
    ai_inspector_entry_id net_id;
    ai_inspector_net_report inspector_report;
    ai_inspector_net_entry entry;
    ai_u32 n_cb_in;
    ai_u32 n_cb_out;
    const reqMsg *creq;
    respMsg *cresp;
    bool no_data;
    uint64_t tcom;
    uint64_t tnodes;
#endif
} net_exec_ctx[AI_MNETWORK_NUMBER] = {0};


#define AI_BUFFER_NULL(ptr_)  \
  AI_BUFFER_OBJ_INIT( \
    AI_BUFFER_FORMAT_NONE|AI_BUFFER_FMT_FLAG_CONST, \
    0, 0, 0, 0, \
    AI_HANDLE_PTR(ptr_))


AI_ALIGNED(4)
static ai_u8 activations[AI_MNETWORK_DATA_ACTIVATIONS_SIZE];

static ai_float in_data[AI_MNETWORK_IN_1_SIZE];
static ai_float out_data[AI_MNETWORK_OUT_1_SIZE];

#ifdef HAS_INSPECTOR
static void aiOnExecNode_cb(const ai_handle cookie,
        const ai_inspect_node_info* node_info,
        ai_node_exec_stage stage) {

    struct ai_network_exec_ctx *ctx = (struct ai_network_exec_ctx*)cookie;

    if (stage == AI_NODE_EXEC_PRE_FORWARD_STAGE) {
        ctx->n_cb_in++;
        ctx->tcom += dwtGetCycles();
        dwtReset();
    } else if (stage == AI_NODE_EXEC_POST_FORWARD_STAGE) {
        uint32_t type;
        uint32_t dur = dwtGetCycles();
        dwtReset();
        ctx->tnodes += dur;
        ctx->n_cb_out++;
        if (ctx->n_cb_out == ctx->report.n_nodes)
            type = EnumLayerType_LAYER_TYPE_INTERNAL_LAST;
        else
            type = EnumLayerType_LAYER_TYPE_INTERNAL;
        type = type << 16;
        if (ctx->no_data)
            type |= PB_BUFFER_TYPE_SEND_WITHOUT_DATA;

        aiPbMgrSendAiBuffer(ctx->creq, ctx->cresp, EnumState_S_PROCESSING,
                type | node_info->type,
                node_info->id,
                dwtCyclesToFloatMs(dur),
                &node_info->out);
    }
}
#endif


static struct ai_network_exec_ctx *aiExecCtx(const char *nn_name, int pos)
{
    struct ai_network_exec_ctx *cur = NULL;

    if (!nn_name)
        return NULL;

    if (!nn_name[0]) {
        if ((pos >= 0) && (pos < AI_MNETWORK_NUMBER))
            cur = &net_exec_ctx[pos];
    } else {
        int idx;
        for (idx=0; idx < AI_MNETWORK_NUMBER; idx++) {
            cur = &net_exec_ctx[idx];
            if (cur->network &&
                    (strlen(cur->report.model_name) == strlen(nn_name)) &&
                    (strncmp(cur->report.model_name, nn_name,
                            strlen(cur->report.model_name)) == 0)) {
                break;
            }
            cur = NULL;
        }
    }
    return cur;
}

__STATIC_INLINE
void aiSetPbContext(struct ai_network_exec_ctx *ctx,
        const reqMsg *creq, respMsg *cresp)
{
#ifdef HAS_INSPECTOR
    if (!ctx)
        return;

    ctx->creq = creq;
    ctx->cresp = cresp;
#endif
}

static int aiBootstrap(const char *nn_name, const int idx)
{
    ai_error err;

    /* Creating the network */
    printf("Creating the network \"%s\"..\r\n", nn_name);
    err = ai_mnetwork_create(nn_name, &net_exec_ctx[idx].network, NULL);
    if (err.type) {
        aiLogErr(err, "ai_mnetwork_create");
        return -1;
    }

    /* Query the created network to get relevant info from it */
    if (ai_mnetwork_get_info(net_exec_ctx[idx].network,
            &net_exec_ctx[idx].report)) {
        aiPrintNetworkInfo(&net_exec_ctx[idx].report);
    } else {
        err = ai_mnetwork_get_error(net_exec_ctx[idx].network);
        aiLogErr(err, "ai_mnetwork_get_info");
        ai_mnetwork_destroy(net_exec_ctx[idx].network);
        net_exec_ctx[idx].network = AI_HANDLE_NULL;
        return -2;
    }

    /* Initialize the instance */
    printf("Initializing the network\r\n");
    /* build params structure to provide the reference of the
     * activation and weight buffers */
    const ai_network_params params = {
            AI_BUFFER_NULL(NULL),
            AI_BUFFER_NULL(activations) };

    if (!ai_mnetwork_init(net_exec_ctx[idx].network, &params)) {
        err = ai_mnetwork_get_error(net_exec_ctx[idx].network);
        aiLogErr(err, "ai_mnetwork_init");
        ai_mnetwork_destroy(net_exec_ctx[idx].network);
        net_exec_ctx[idx].network = AI_HANDLE_NULL;
        return -4;
    }
    return 0;
}

#ifdef HAS_INSPECTOR
static int aiInspectorGetReport(struct ai_network_exec_ctx *ctx)
{
    int res = 0;
    if (!ctx)
        return -1;

    if ((ctx->inspector != AI_HANDLE_NULL) &&
            (ctx->net_id != AI_INSPECTOR_NETWORK_BIND_FAILED))
    {
        if (ai_inspector_get_report(ctx->inspector,
                ctx->net_id, &ctx->inspector_report) != true)
            res = -2;
    }
    return res;
}
#endif


static void aiInspectorSendReport(const reqMsg *req, respMsg *resp,
        EnumState state, struct ai_network_exec_ctx *ctx,
        const ai_float dur_ms)
{
#ifdef HAS_INSPECTOR
    if (aiInspectorGetReport(ctx) == 0) {
        resp->which_payload = respMsg_report_tag;
        resp->payload.report.id = ctx->inspector_report.id;
        resp->payload.report.elapsed_ms = dur_ms;
        resp->payload.report.n_nodes = ctx->inspector_report.n_nodes;
        resp->payload.report.signature = ctx->inspector_report.signature;
        resp->payload.report.num_inferences = ctx->inspector_report.num_inferences;
        aiPbMgrSendResp(req, resp, state);
        aiPbMgrWaitAck();
    } else {
        aiPbMgrSendAck(req, resp, EnumState_S_ERROR,
                EnumError_E_GENERIC);
    }
#endif
}


static void aiInspectorUnBind(struct ai_network_exec_ctx *ctx)
{
#ifdef HAS_INSPECTOR
    if (!ctx)
        return;

    if ((ctx->inspector != AI_HANDLE_NULL) &&
            (ctx->net_id != AI_INSPECTOR_NETWORK_BIND_FAILED))
        ai_inspector_unbind_network(ctx->inspector,
                ctx->net_id);

    ctx->net_id = AI_INSPECTOR_NETWORK_BIND_FAILED;
#endif
}

static int aiInspectorBind(struct ai_network_exec_ctx *ctx)
{
    int res = 0;
#ifdef HAS_INSPECTOR
    if (!ctx)
        return 0;

    aiInspectorUnBind(ctx);

    ctx->n_cb_in  = 0;
    ctx->n_cb_out = 0;

    if (ctx->inspector) {
        ctx->net_id = ai_inspector_bind_network(
                ctx->inspector,
                &ctx->entry);

        if (ctx->net_id == AI_INSPECTOR_NETWORK_BIND_FAILED)
            res = -1;
    }

#endif
    return res;
}

static int aiInspectorInitAndBind(struct ai_network_exec_ctx *ctx)
{
#ifdef HAS_INSPECTOR
    ctx->n_cb_in  = 0;
    ctx->n_cb_out = 0;

    ai_inspector_config cfg = {
      .validation_mode = VALIDATION_INSPECT,
      .log_level = LOG_SUDO,
      .log_quiet = false,
      .on_report_destroy = NULL,
      .on_exec_node = &aiOnExecNode_cb,
      .cookie = NULL,
    };

    ai_handle phandle;
    ai_network_params pparams;

    if (ctx->network == AI_HANDLE_NULL)
        return -1;

    ai_mnetwork_get_private_handle(ctx->network,
            &phandle,
            &pparams);

    cfg.cookie = (ai_handle)ctx;

    if (ai_inspector_create(&ctx->inspector, &cfg) )
    {
        ctx->entry.handle = phandle;
        ctx->entry.params = pparams;
        aiInspectorBind(ctx);
    }


#endif
    return 0;
}

static int aiInspectorUnbindAndDestroy(struct ai_network_exec_ctx *ctx)
{
#ifdef HAS_INSPECTOR
    aiInspectorUnBind(ctx);
    if (ctx->inspector != AI_HANDLE_NULL)
        ai_inspector_destroy(ctx->inspector);
    ctx->inspector = AI_HANDLE_NULL;
#endif
    return 0;
}


static int aiInit(void)
{
    int res = -1;
    const char *nn_name;
    int idx;

    printf("\r\nAI platform (API %d.%d.%d - RUNTIME %d.%d.%d)\r\n",
            AI_PLATFORM_API_MAJOR,
            AI_PLATFORM_API_MINOR,
            AI_PLATFORM_API_MICRO,
            AI_PLATFORM_RUNTIME_MAJOR,
            AI_PLATFORM_RUNTIME_MINOR,
            AI_PLATFORM_RUNTIME_MICRO);

    /* Clean all network exec context */
    for (idx=0; idx < AI_MNETWORK_NUMBER; idx++) {
        net_exec_ctx[idx].network = AI_HANDLE_NULL;
#ifdef HAS_INSPECTOR
        net_exec_ctx[idx].inspector = AI_HANDLE_NULL;
        net_exec_ctx[idx].net_id = AI_INSPECTOR_NETWORK_BIND_FAILED;
#endif
    }

    /* Discover and init the embedded network */
    idx = 0;
    do {
        nn_name = ai_mnetwork_find(NULL, idx);
        if (nn_name) {
            printf("\r\nFound network \"%s\"\r\n", nn_name);
            res = aiBootstrap(nn_name, idx);
            if (res)
                nn_name = NULL;
        }
        idx++;
    } while (nn_name);

    return res;
}

static void aiDeInit(void)
{
    ai_error err;
    int idx;

    printf("Releasing the network(s)...\r\n");

    for (idx=0; idx<AI_MNETWORK_NUMBER; idx++) {
        if (net_exec_ctx[idx].network != AI_HANDLE_NULL) {
            if (ai_mnetwork_destroy(net_exec_ctx[idx].network)
                    != AI_HANDLE_NULL) {
                err = ai_mnetwork_get_error(net_exec_ctx[idx].network);
                aiLogErr(err, "ai_mnetwork_destroy");
            }
            net_exec_ctx[idx].network = AI_HANDLE_NULL;
        }
    }
}

/* -----------------------------------------------------------------------------
 * Specific APP/test functions
 * -----------------------------------------------------------------------------
 */

void aiPbCmdNNInfo(const reqMsg *req, respMsg *resp, void *param)
{
    struct ai_network_exec_ctx *ctx;

    ctx = aiExecCtx(req->name, req->param);
    if (ctx)
        aiPbMgrSendNNInfo(req, resp, EnumState_S_IDLE,
                &ctx->report);
    else
        aiPbMgrSendAck(req, resp, EnumState_S_ERROR,
                EnumError_E_INVALID_PARAM);
}

void aiPbCmdNNRun(const reqMsg *req, respMsg *resp, void *param)
{
    ai_i32 batch;
    uint32_t tend;
    bool res;
    struct ai_network_exec_ctx *ctx;
    bool inspector_mode = false;
    uint32_t ints;

    ai_buffer ai_input[1];
    ai_buffer ai_output[1];

    ctx = aiExecCtx(req->name, -1);
    if (!ctx) {
        aiPbMgrSendAck(req, resp, EnumState_S_ERROR,
                EnumError_E_INVALID_PARAM);
        return;
    }

#ifdef HAS_INSPECTOR
    ctx->no_data = false;
    if ((req->param & EnumRunParam_P_RUN_MODE_INSPECTOR) ==
            EnumRunParam_P_RUN_MODE_INSPECTOR)
        inspector_mode = true;

    if ((req->param & EnumRunParam_P_RUN_MODE_INSPECTOR_WITHOUT_DATA) ==
            EnumRunParam_P_RUN_MODE_INSPECTOR_WITHOUT_DATA) {
        inspector_mode = true;
        ctx->no_data = true;
    }

    ctx->tcom = 0ULL;
    ctx->tnodes = 0ULL;
#endif

    ai_input[0] = ctx->report.inputs;
    ai_output[0] = ctx->report.outputs;

    ai_input[0].n_batches  = 1;
    ai_input[0].data = AI_HANDLE_PTR(in_data);
    ai_output[0].n_batches = 1;
    ai_output[0].data = AI_HANDLE_PTR(out_data);

    /* 1 Send a ACK (ready to receive a buffer) */
    aiPbMgrSendAck(req, resp, EnumState_S_WAITING,
            aiPbAiBufferSize(&ai_input[0]));

    /* 2 Read buffer */
    res = aiPbMgrReceiveAiBuffer(req, resp, EnumState_S_PROCESSING,
            &ai_input[0], true);
    if (res != true)
        return;

    ints = disableInts();

    /* Update the PN context for inspector callbacks */
    aiSetPbContext(ctx, req, resp);

    if (inspector_mode)
        aiInspectorInitAndBind(ctx);

    /* Processing */
    dwtReset();

    batch = ai_mnetwork_run(ctx->network,
            &ai_input[0], &ai_output[0]);
    if (batch != 1) {
        aiLogErr(ai_mnetwork_get_error(ctx->network),
                "ai_mnetwork_run");
        aiPbMgrSendAck(req, resp, EnumState_S_ERROR,
                EnumError_E_GENERIC);
        return;
    }
    tend = dwtGetCycles();

#ifdef HAS_INSPECTOR
    tend = ctx->tcom + ctx->tnodes + tend;
#endif

    if (inspector_mode)
        aiInspectorSendReport(req, resp, EnumState_S_PROCESSING, ctx,
                dwtCyclesToFloatMs(tend));

    /* 3 Write buffer */
    aiPbMgrSendAiBuffer(req, resp, EnumState_S_DONE,
            EnumLayerType_LAYER_TYPE_OUTPUT << 16 | 0,
            0, dwtCyclesToFloatMs(tend),
            &ai_output[0]);

    if (inspector_mode)
        aiInspectorUnbindAndDestroy(ctx);

    restoreInts(ints);
}

static int aiValidationCore(void)
{
    return aiPbMgrWaitAndProcess();
}

/* -----------------------------------------------------------------------------
 * Exported/Public functions
 * -----------------------------------------------------------------------------
 */

static aiPbCmdFunc pbCmdFuncTab[] = {
#ifdef HAS_INSPECTOR
        AI_PB_CMD_SYNC((void *)EnumCapability_CAP_INSPECTOR),
#else
        AI_PB_CMD_SYNC(NULL),
#endif
        AI_PB_CMD_SYS_INFO(NULL),
        { EnumCmd_CMD_NETWORK_INFO, &aiPbCmdNNInfo, NULL },
        { EnumCmd_CMD_NETWORK_RUN, &aiPbCmdNNRun, NULL },
#if defined(AI_PB_TEST)
        AI_PB_CMD_TEST(NULL),
#endif
        AI_PB_CMD_END,
};

int aiValidationInit(void)
{
    aiPbMgrInit(pbCmdFuncTab);

    aiTestHeader(_APP_NAME_, _APP_VERSION_MAJOR_, _APP_VERSION_MINOR_);

    dwtIpInit();
    crcIpInit();
    logDeviceConf();

    return 0;
}

int aiValidationProcess(void)
{
    int r;

    r = aiInit();
    if (r) {
        printf("\r\nE:  aiInit() r=%d\r\n", r);
        HAL_Delay(2000);
        return r;
    } else {
        printf("\r\n");
        printf("-------------------------------------------\r\n");
        printf("| READY to receive a CMD from the HOST... |\r\n");
        printf("-------------------------------------------\r\n");
        printf("\r\n");
        printf("# Note: At this point, default ASCII-base terminal should be closed\r\n");
        printf("# and a stm32com-base interface should be used\r\n");
        printf("# (i.e. Python stm32com module). Protocol version = %d.%d\r\n",
                EnumVersion_P_VERSION_MAJOR,
                EnumVersion_P_VERSION_MINOR);
    }

    ioDisableWrite();

    do {
        r = aiValidationCore();
    } while (r==0);

    return r;
}

void aiValidationDeInit(void)
{
    printf("\r\n");
    aiDeInit();
    printf("bye bye ...\r\n");
}

