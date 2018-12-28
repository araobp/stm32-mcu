/**
  ******************************************************************************
  * @file    aiPbMgr.c
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

#include <aiTestUtility.h>
#include <bsp_ai.h>

#include "aiPbMgr.h"
#include <pb_encode.h>
#include <pb_decode.h>


/*---------------------------------------------------------------------------*/

static struct pbContextMgr {
    pb_istream_t input;
    pb_ostream_t output;
    const aiPbCmdFunc *funcs;
    uint32_t  n_func;
} pbContextMgr;

void aiPbMgrInit(const aiPbCmdFunc *funcs)
{
    const aiPbCmdFunc *cfunc;
    pbContextMgr.input = pb_istream_from_uart(0);
    pbContextMgr.output = pb_ostream_to_uart(0);

    pbContextMgr.n_func = 0;
    pbContextMgr.funcs = NULL;

    if (funcs) {
        cfunc = funcs;
        while (cfunc->process) {
            pbContextMgr.n_func++;
            cfunc++;
        }
        pbContextMgr.funcs = funcs;
    }
}

int aiPbMgrWaitAndProcess(void)
{
    uint32_t idx;
    static reqMsg  req = reqMsg_init_zero;
    static respMsg resp = respMsg_init_default;
    const aiPbCmdFunc *cfunc;

    ioFlushRead();
    if (pb_decode_delimited(&pbContextMgr.input, reqMsg_fields, &req)) {
        ioFlushRead();
        for (idx = 0; idx < pbContextMgr.n_func; idx++) {
            cfunc = &pbContextMgr.funcs[idx];
            if (cfunc->cmd == req.cmd) {
                cfunc->process(&req, &resp, cfunc->param);
                break;
            }
        }
        if (idx == pbContextMgr.n_func) {
            aiPbMgrSendAck(&req, &resp, EnumState_S_ERROR,
                    EnumError_E_INVALID_PARAM);
        }
    }

    ioFlushRead();

    return 0;
}


/*---------------------------------------------------------------------------*/

void aiPbMgrSendResp(const reqMsg *req, respMsg *resp,
        EnumState state)
{
    resp->reqid = req->reqid;
    resp->state = state;
    pb_encode(&pbContextMgr.output, respMsg_fields, resp);
    ioFlushWrite();
}

void aiPbMgrSendAck(const reqMsg *req, respMsg *resp,
        EnumState state, uint32_t param)
{
    resp->which_payload = respMsg_ack_tag;
    resp->payload.ack.param = param;
    aiPbMgrSendResp(req, resp, state);
}

bool aiPbMgrWaitAck(void)
{
    bool res;
    ackMsg ack = ackMsg_init_default;
    res = pb_decode_delimited(&pbContextMgr.input, ackMsg_fields, &ack);
    ioFlushRead();
    return res;
}

bool aiPbMgrSendLog(const reqMsg *req, respMsg *resp,
        EnumState state, uint32_t lvl, const char *str)
{
    bool res;
    ackMsg ack = ackMsg_init_default;

    int len = strlen(str);

    resp->which_payload = respMsg_log_tag;
    resp->payload.log.level = lvl;
    if (len >= sizeof(resp->payload.log.str))
        len = sizeof(resp->payload.log.str) - 1;

    memcpy(&resp->payload.log.str[0], str, len+1);

    aiPbMgrSendResp(req, resp, state);

    res = pb_decode_delimited(&pbContextMgr.input, ackMsg_fields, &ack);
    ioFlushRead();
    return res;
}

struct aiPbMgrBuffer {
    ai_float *buffer;
    uint32_t n_max;
    uint32_t n_ops;
    aiBufferMsg *msg;
};

uint32_t aiPbAiBufferSize(ai_buffer *buffer)
{
    if (!buffer)
        return 0;
    else
        return buffer->channels * buffer->height
                * buffer->width * buffer->n_batches;
}

__STATIC_INLINE uint32_t aiPbAiBufferMsgSize(aiBufferMsg *buffer)
{
    if (!buffer)
        return 0;
    else
        return buffer->channels * buffer->height
                * buffer->width * buffer->n_batches;
}

static bool aiPbBuffer_read_cb(pb_istream_t *stream, const pb_field_t *field,
        void **arg)
{
    struct aiPbMgrBuffer *buffer = (struct aiPbMgrBuffer *)*arg;

    int maxr = 0;
    ai_float *pw = NULL;

    if (buffer) {
        buffer->n_ops = 0;
        if (buffer->n_max && buffer->buffer) {
            maxr = buffer->n_max;
            pw = buffer->buffer;
        }
    }

    while (stream->bytes_left)
    {
        float value;
        if (!pb_decode_fixed32(stream, &value))
            return false;
        if (maxr > 0) {
            *pw++ = (ai_float)value;
            maxr--;
            buffer->n_ops++;
        }
    }
    return true;
}

static bool aiPbBuffer_write_cb(pb_ostream_t *stream, const pb_field_t *field,
        void * const *arg)
{
    float value;

    struct aiPbMgrBuffer *buffer = (struct aiPbMgrBuffer *)*arg;

    int maxw = 0;
    ai_float *pr = NULL;

    if (buffer) {
        if (buffer->n_max && buffer->buffer) {
            if (buffer->n_ops)
                maxw = (buffer->n_ops < buffer->n_max)?buffer->n_ops:buffer->n_max;
            else
                maxw = buffer->n_max;
            pr = buffer->buffer;
            buffer->n_ops = 0;
        }
    }

    for (int i = 0; i < maxw; i++) {

        if (!pb_encode_tag_for_field(stream, field))
            return false;

        // value = 2.0f * (ai_float) rand() / (ai_float) RAND_MAX - 1.0f;
        value = *pr++;

        if (!pb_encode_fixed32(stream, &value))
            return false;

        buffer->n_ops++;
    }

    return true;
}

bool aiPbMgrReceiveAiBuffer(const reqMsg *req, respMsg *resp,
        EnumState state, ai_buffer *buffer, bool check)
{
    aiBufferMsg aibuffer;
    struct aiPbMgrBuffer hdlb;
    bool res = true;

    hdlb.n_ops = 0;
    if (!buffer || buffer->format != AI_BUFFER_FORMAT_FLOAT ||
            !buffer->data) {
        /* all received data are skipped */
        hdlb.n_max  = 0;
        hdlb.buffer = NULL;
    } else {
        hdlb.buffer = (ai_float *)buffer->data;
        hdlb.n_max = aiPbAiBufferSize(buffer);
    }
    hdlb.msg = &aibuffer;

    aibuffer.datas.funcs.decode = &aiPbBuffer_read_cb;
    aibuffer.datas.arg = &hdlb;

    /* Waiting buffer message */
    pb_decode_delimited(&pbContextMgr.input, aiBufferMsg_fields, &aibuffer);
    ioFlushRead();

    if (!buffer) {
        aiPbMgrSendAck(req, resp, state, hdlb.n_ops);
        aiPbMgrWaitAck();
        res = true;
    } else {
        if (check == true) {
            if ((aibuffer.channels != buffer->channels) ||
                    (aibuffer.height != buffer->height) ||
                    (aibuffer.width != buffer->width) ||
                    (aibuffer.n_batches != buffer->n_batches) ||
                    (hdlb.n_ops != hdlb.n_max)) {
                aiPbMgrSendAck(req, resp, EnumState_S_ERROR,
                        EnumError_E_INVALID_SIZE);
                res = false;
            } else {
                aiPbMgrSendAck(req, resp, state, hdlb.n_ops);
                aiPbMgrWaitAck();
                res = true;
            }
        } else {
            if ((hdlb.n_ops >= aiPbAiBufferMsgSize(&aibuffer))) {
                buffer->channels = aibuffer.channels;
                buffer->height = aibuffer.height;
                buffer->width = aibuffer.width;
                buffer->n_batches = aibuffer.n_batches;
            }
            aiPbMgrSendAck(req, resp, state, hdlb.n_ops);
            aiPbMgrWaitAck();
            res = true;
        }
    }

    return res;
}

bool aiPbMgrSendAiBuffer(const reqMsg *req, respMsg *resp, EnumState state,
        uint32_t type, uint32_t id, ai_float dur_ms, const ai_buffer *buffer)
{
    struct aiPbMgrBuffer hdlb;

    hdlb.n_ops = 0;
    if (!buffer || buffer->format != AI_BUFFER_FORMAT_FLOAT ||
            !buffer->data) {
        hdlb.n_max  = 0;
        hdlb.buffer = NULL;
    } else {
        hdlb.buffer = (ai_float *)buffer->data;
        hdlb.n_max = buffer->channels * buffer->height
                * buffer->width * buffer->n_batches;
    }

    if (type & PB_BUFFER_TYPE_SEND_WITHOUT_DATA) {
        hdlb.n_max  = 0;
        type &= (~PB_BUFFER_TYPE_SEND_WITHOUT_DATA);
    }

    resp->which_payload = respMsg_layer_tag;
    resp->payload.layer.buffer.datas.funcs.encode = &aiPbBuffer_write_cb;
    resp->payload.layer.buffer.datas.arg = &hdlb;

    resp->payload.layer.type = type;
    resp->payload.layer.id = id;
    resp->payload.layer.duration = dur_ms;
    if (buffer) {
        resp->payload.layer.buffer.channels = buffer->channels;
        resp->payload.layer.buffer.format = EnumFormat_F_FLOAT;
        resp->payload.layer.buffer.height = buffer->height;
        resp->payload.layer.buffer.width = buffer->width;
        resp->payload.layer.buffer.n_batches = buffer->n_batches;
    }

    /* Send buffer */
    aiPbMgrSendResp(req, resp, state);

    /* Waiting ACK */
    if (state == EnumState_S_PROCESSING)
        return aiPbMgrWaitAck();
    else
        return true;
}

/*---------------------------------------------------------------------------*/

void aiPbCmdSync(const reqMsg *req, respMsg *resp, void *param)
{
    resp->which_payload = respMsg_sync_tag;
    resp->payload.sync.version =
            EnumVersion_P_VERSION_MAJOR << 8 |
            EnumVersion_P_VERSION_MINOR;

#if defined(AI_PB_TEST) && (AI_PB_TEST == 1)
    resp->payload.sync.capability |= EnumCapability_CAP_SELF_TEST;
#endif

    if (param)
        resp->payload.sync.capability |= (uint32_t)param;

    aiPbMgrSendResp(req, resp, EnumState_S_IDLE);
}

void aiPbCmdSysInfo(const reqMsg *req, respMsg *resp, void *param)
{
    resp->which_payload = respMsg_sinfo_tag;
    resp->payload.sinfo.devid = HAL_GetDEVID();
    resp->payload.sinfo.sclock = HAL_RCC_GetSysClockFreq();
    resp->payload.sinfo.hclock = HAL_RCC_GetHCLKFreq();
    resp->payload.sinfo.cache = getFlashCacheConf();

    aiPbMgrSendResp(req, resp, EnumState_S_IDLE);
}


static void init_aibuffer_msg(const ai_buffer *aibuffer, aiBufferMsg *msg)
{
    if ((!aibuffer) || (!msg))
        return;

    msg->format = (EnumFormat)aibuffer->format;
    msg->channels = aibuffer->channels;
    msg->height = aibuffer->height;
    msg->width = aibuffer->width;
    msg->n_batches = aibuffer->n_batches;
    msg->datas.funcs.encode = NULL;
}

static bool nn_shape_w_cb(pb_ostream_t *stream, const pb_field_t *field,
        const ai_buffer *aibuffer, int maxw)
{
    aiBufferMsg msg;

    for (int i = 0; i < maxw; i++) {
        if (!pb_encode_tag_for_field(stream, field))
            return false;

        init_aibuffer_msg(&aibuffer[i], &msg);

        if (!pb_encode_submessage(stream, aiBufferMsg_fields, &msg))
            return false;
    }
    return true;
}

static bool nn_inputs_w_cb(pb_ostream_t *stream, const pb_field_t *field,
        void * const *arg)
{
    ai_network_report *report = (ai_network_report *)*arg;

    if (!report)
        return true;

    return nn_shape_w_cb(stream, field, &report->inputs, 1);
}

static bool nn_outputs_w_cb(pb_ostream_t *stream, const pb_field_t *field,
        void * const *arg)
{
    ai_network_report *report = (ai_network_report *)*arg;

    if (!report)
        return true;

    return nn_shape_w_cb(stream, field, &report->outputs, 1);
}

void aiPbStrCopy(const char *src, char *dst, uint32_t max)
{
    const char undef[] = "UNDEFINED";
    int l = strlen(src);

    if (l > max)
        l = max-1;

    if (!dst)
        return;

    if (src && l)
        memcpy(dst, src, l+1);
    else
        memcpy(dst, undef, strlen(undef)+1);
}

uint32_t aiPbVersionToUint32(const ai_platform_version *ver)
{
    if (!ver)
        return 0;

    return ver->major << 24 | ver->minor << 16
            | ver->micro << 8 | ver->reserved;
}

void aiPbMgrSendNNInfo(const reqMsg *req, respMsg *resp,
        EnumState state, const ai_network_report *nn)
{
    resp->which_payload = respMsg_ninfo_tag;

    aiPbStrCopy(nn->model_name,
            &resp->payload.ninfo.model_name[0],
            sizeof(resp->payload.ninfo.model_name));
    aiPbStrCopy(nn->model_signature,
            &resp->payload.ninfo.model_signature[0],
            sizeof(resp->payload.ninfo.model_signature));
    aiPbStrCopy(nn->model_datetime,
            &resp->payload.ninfo.model_datetime[0],
            sizeof(resp->payload.ninfo.model_datetime));
    aiPbStrCopy(nn->compile_datetime,
            &resp->payload.ninfo.compile_datetime[0],
            sizeof(resp->payload.ninfo.compile_datetime));
    aiPbStrCopy(nn->runtime_revision,
            &resp->payload.ninfo.runtime_revision[0],
            sizeof(resp->payload.ninfo.runtime_revision));
    aiPbStrCopy(nn->tool_revision,
            &resp->payload.ninfo.tool_revision[0],
            sizeof(resp->payload.ninfo.tool_revision));

    resp->payload.ninfo.n_inputs = nn->n_inputs;
    resp->payload.ninfo.n_outputs = nn->n_outputs;
    resp->payload.ninfo.n_nodes = nn->n_nodes;
    resp->payload.ninfo.n_macc = nn->n_macc;

    resp->payload.ninfo.signature = nn->signature;
    resp->payload.ninfo.api_version =
            aiPbVersionToUint32(&nn->api_version);
    resp->payload.ninfo.interface_api_version =
            aiPbVersionToUint32(&nn->interface_api_version);
    resp->payload.ninfo.runtime_version =
            aiPbVersionToUint32(&nn->runtime_version);
    resp->payload.ninfo.tool_version =
            aiPbVersionToUint32(&nn->tool_version);
    resp->payload.ninfo.tool_api_version =
            aiPbVersionToUint32(&nn->tool_api_version);

    init_aibuffer_msg(&nn->activations, &resp->payload.ninfo.activations);
    init_aibuffer_msg(&nn->weights, &resp->payload.ninfo.weights);

    resp->payload.ninfo.inputs.funcs.encode = nn_inputs_w_cb;
    resp->payload.ninfo.inputs.arg = (void *)nn;

    resp->payload.ninfo.outputs.funcs.encode = nn_outputs_w_cb;
    resp->payload.ninfo.outputs.arg = (void *)nn;

    aiPbMgrSendResp(req, resp, state);
}


/*---------------------------------------------------------------------------*/

#if defined(AI_PB_TEST) && (AI_PB_TEST == 1)

#define _MAX_BUFF_SIZE_ (1000)
static ai_float buffer_test[_MAX_BUFF_SIZE_];
static ai_buffer ai_buffer_test;

void aiPbMgrRstAiBuffer(ai_buffer *buffer)
{
    buffer->channels = _MAX_BUFF_SIZE_;
    buffer->height = buffer->width = buffer->n_batches = 1;
    buffer->data = (ai_handle)buffer_test;
    buffer->format = AI_BUFFER_FORMAT_FLOAT;
}

/* TEST - Receive Simple buffer */
static void aiPbCmdRecSimpleBuffer(const reqMsg *req, respMsg *resp,
        void *param)
{
    aiPbMgrRstAiBuffer(&ai_buffer_test);

    /* 1 Send a ACK/WAITING (ready to receive a buffer) */
    aiPbMgrSendAck(req, resp, EnumState_S_WAITING,
            aiPbAiBufferSize(&ai_buffer_test));

    /* 2 Read buffer */
    aiPbMgrReceiveAiBuffer(req, resp, EnumState_S_PROCESSING,
            &ai_buffer_test, false);

    /* Emulate processing */
    HAL_Delay(300);  /* 300ms */

    /* 4 Send ACK/DONE (operation DONE) */
    aiPbMgrSendAck(req, resp, EnumState_S_DONE, 0);
}

/* TEST - Receive/Send Simple buffer */
static void aiPbCmdRecSendSimpleBuffer(const reqMsg *req, respMsg *resp,
        void *param)
{
    aiPbMgrRstAiBuffer(&ai_buffer_test);

    /* 1 Send a ACK/WAITING (ready to receive a buffer) */
    aiPbMgrSendAck(req, resp, EnumState_S_WAITING,
            aiPbAiBufferSize(&ai_buffer_test));

    /* 2 Read buffer */
    aiPbMgrReceiveAiBuffer(req, resp, EnumState_S_PROCESSING,
            &ai_buffer_test, false);

    /* Emulate processing */
    if (req->param == 200) {
        HAL_Delay(100);
    }


    if (req->param == 200) {
        /* 3 Write result/buffer */
        aiPbMgrSendAiBuffer(req, resp, EnumState_S_PROCESSING,
                0, 0, 100000, &ai_buffer_test);

        /* 4 - Send ACK/DONE */
        aiPbMgrSendAck(req, resp, EnumState_S_DONE, 0);
    } else {
        /* 3 Write result/buffer */
        aiPbMgrSendAiBuffer(req, resp, EnumState_S_DONE,
                0, 0, 0, &ai_buffer_test);
    }
}

/* TEST CMD */
void aiPbCmdTest(const reqMsg *req, respMsg *resp, void *param)
{
    if (req->param == 0) {
        aiPbMgrSendAck(req, resp, EnumState_S_DONE, 0);
    } else if (req->param == 1) {
        resp->which_payload = respMsg_ack_tag;
        if (!req->name[0]) {
            aiPbMgrSendAck(req, resp, EnumState_S_ERROR, 1);
        } else {
            aiPbMgrSendAck(req, resp, EnumState_S_DONE,
                    strlen(req->name));
        }
    } else if (req->param == 2) {
        const char *str = "Hello..";
        aiPbMgrSendLog(req, resp, EnumState_S_DONE, 1, str);
    } else if (req->param == 3) {
        /* Time out test */
        HAL_Delay(500);
    } else if (req->param == 4) {
        char str[20] = "Bye Bye..";
        for (int i=0; i<5;i++) {
            str[0] = 'A' + i;
            aiPbMgrSendLog(req, resp, EnumState_S_PROCESSING, i, str);
        }
        aiPbMgrSendAck(req, resp, EnumState_S_DONE, 0);
    } else if ((req->param >= 100) && (req->param < 200)) {
        aiPbCmdRecSimpleBuffer(req, resp, param);
    } else if ((req->param >= 200) && (req->param < 300)) {
        aiPbCmdRecSendSimpleBuffer(req, resp, param);
    } else {
        aiPbMgrSendAck(req, resp, EnumState_S_DONE, req->param + 1);
    }
}

#endif
