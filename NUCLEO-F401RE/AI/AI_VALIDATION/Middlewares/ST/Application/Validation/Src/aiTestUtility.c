/**
  ******************************************************************************
  * @file    aiTestUtility.c
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

 /*
 * Description:
 *
 * Helper & Low-Level IO functions for AI test applications
 *
 * History:
 *  - v1.0 - Initial version
 *  - v1.1 - Add IO low level nanopb functions
 *  - v1.2 - Adding STM32H7/F3 MCU support
 *  - v1.3 - Adding STM32F3 str description + IO PB packet mode
 */

/* System headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#if defined(__GNUC__)
#include <errno.h>
#include <sys/unistd.h> /* STDOUT_FILENO, STDERR_FILENO */
#elif defined (__ICCARM__)
#if (__IAR_SYSTEMS_ICC__ <= 8)
/* Temporary workaround - LowLevelIOInterface.h seems not available
   with IAR 7.80.4 */
#define _LLIO_STDIN  0
#define _LLIO_STDOUT 1
#define _LLIO_STDERR 2
#define _LLIO_ERROR ((size_t)-1) /* For __read and __write. */
#else
#include <LowLevelIOInterface.h> /* _LLIO_STDOUT, _LLIO_STDERR */
#endif

#elif defined (__CC_ARM)

#endif

#include <aiTestUtility.h>
#include <bsp_ai.h>

#define _IO_OUT_PACKET_MODE


extern UART_HandleTypeDef UartHandle;

/* -----------------------------------------------------------------------------
 * Device-related functions
 * -----------------------------------------------------------------------------
 */

#if !defined(STM32F7) && !defined(STM32L4) && !defined(STM32F4) && !defined(STM32H7) && !defined(STM32F3)
#error Only STM32H7, STM32F7, STM32F4, STM32L4 or STM32F3 device are supported
#endif

void crcIpInit(void)
{
#if defined(STM32H7)
    /* By default the CRC IP clock is enabled */
    __HAL_RCC_CRC_CLK_ENABLE();
#else
    if (!__HAL_RCC_CRC_IS_CLK_ENABLED())
        printf("W: CRC IP clock is NOT enabled\r\n");

    /* By default the CRC IP clock is enabled */
    __HAL_RCC_CRC_CLK_ENABLE();
#endif
}

void dwtIpInit(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

#ifdef STM32F7
    DWT->LAR = 0xC5ACCE55;
#endif

    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk | DWT_CTRL_CPIEVTENA_Msk;
}

void dwtReset(void)
{
    DWT->CYCCNT = 0; /* Clear DWT cycle counter */
}

uint32_t dwtGetCycles(void)
{
    return DWT->CYCCNT;
}

uint32_t systemCoreClock(void)
{
#if !defined(STM32H7)
    return HAL_RCC_GetHCLKFreq();
#else
    return HAL_RCC_GetSysClockFreq();
#endif
}

int dwtCyclesToTime(uint64_t clks, struct dwtTime *t)
{
    if (!t)
        return -1;
    uint32_t fcpu = systemCoreClock();
    uint64_t s  = clks / fcpu;
    uint64_t ms = (clks * 1000) / fcpu;
    uint64_t us = (clks * 1000 * 1000) / fcpu;
    ms -= (s * 1000);
    us -= (ms * 1000 + s * 1000000);
    t->fcpu = fcpu;
    t->s = s;
    t->ms = ms;
    t->us = us;
    return 0;
}


float dwtCyclesToFloatMs(uint64_t clks)
{
    float res;
    float fcpu = (float)systemCoreClock();
    res = ((float)clks * (float)1000.0) / fcpu;
    return res;
}


__STATIC_INLINE const char *devIdToStr(uint16_t dev_id)
{
    const char *str;
    switch (dev_id) {
    case 0x422: str = "STM32F303xB/C"; break;
    case 0x438: str = "STM32F303x6/8"; break;
    case 0x446: str = "STM32F303xD/E"; break;
    case 0x431: str = "STM32F411xC/E"; break;
    case 0x435: str = "STM32L43xxx"; break;
    case 0x462: str = "STM32L45xxx"; break;
    case 0x415: str = "STM32L4x6xx"; break;
    case 0x470: str = "STM32L4Rxxx"; break;
    case 0x449: str = "STM32F74xxx"; break;
    case 0x450: str = "STM32H743/753 and STM32H750"; break;
    default:    str = "UNKNOWN";
    }
    return str;
}

#if !defined(STM32F3)
__STATIC_INLINE const char* bitToStr(uint32_t val)
{
    if (val)
        return "True";
    else
        return "False";
}
#endif


static uint32_t mconf;

uint32_t getFlashCacheConf(void)
{
    return mconf;
}

void logDeviceConf(void)
{
    uint32_t acr = FLASH->ACR ;
    uint32_t val;

    printf("STM32 Runtime configuration...\r\n");

    printf(" Device       : DevID:0x%08x (%s) RevID:0x%08x\r\n",
            (int)HAL_GetDEVID(),
            devIdToStr(HAL_GetDEVID()),
            (int)HAL_GetREVID()
    );

    printf(" Core Arch.   : M%d - %s %s\r\n",
            __CORTEX_M,
#if (__FPU_PRESENT == 1)
            "FPU PRESENT",
            __FPU_USED ? "and used" : "and not used!"
#else
            "!FPU NOT PRESENT",
            ""
#endif
    );

#if (__FPU_PRESENT == 1) && __FPU_USED
    mconf = (1 << 16);  /* FPU present and used */
#endif

    printf(" HAL version  : 0x%08x\r\n", (int)HAL_GetHalVersion());

    val = systemCoreClock()/1000000;

#if !defined(STM32H7)
    printf(" system clock : %u MHz\r\n", (int)val);
#else
    printf(" SYSCLK clock : %u MHz\r\n", (int)val);
    printf(" HCLK clock   : %u MHz\r\n", (int)HAL_RCC_GetHCLKFreq()/1000000);
#endif

#if defined(STM32F7) || defined(STM32H7)
    val = SCB->CCR;
#if !defined(STM32H7)
    mconf |= (2 << 24);  /* F7 conf. */
    mconf |= ((acr & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos);
    if ((acr & FLASH_ACR_PRFTEN_Msk) >> FLASH_ACR_PRFTEN_Pos)
        mconf |= (1 << 8);
    if ((acr & FLASH_ACR_ARTEN_Msk) >> FLASH_ACR_ARTEN_Pos)
        mconf |= (1 << 9);
    printf(" FLASH conf.  : ACR=0x%08x - Prefetch=%s ART=%s latency=%d\r\n",
            (int)acr,
            bitToStr((acr & FLASH_ACR_PRFTEN_Msk) >> FLASH_ACR_PRFTEN_Pos),
            bitToStr((acr & FLASH_ACR_ARTEN_Msk) >> FLASH_ACR_ARTEN_Pos),
            (int)((acr & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos));
#else
    mconf |= (3 << 24);  /* H7 conf. */
    mconf |= (acr & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos;
    printf(" FLASH conf.  : ACR=0x%08x - latency=%d\r\n",
            (int)acr,
            (int)((acr & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos));
#endif
    if (val & SCB_CCR_IC_Msk)
        mconf |= (1 << 10);
    if (val & SCB_CCR_DC_Msk)
        mconf |= (1 << 11);
    printf(" CACHE conf.  : $I/$D=(%s,%s)\r\n",
            bitToStr(val & SCB_CCR_IC_Msk),
            bitToStr(val & SCB_CCR_DC_Msk));
#else
#if !defined(STM32F3)
    mconf |= (1 << 24);  /* F4/L4 conf. */
    mconf |= ((acr & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos);
    if ((acr & FLASH_ACR_PRFTEN_Msk) >> FLASH_ACR_PRFTEN_Pos)
        mconf |= (1 << 8);
    if ((acr & FLASH_ACR_ICEN_Msk) >> FLASH_ACR_ICEN_Pos)
        mconf |= (1 << 9);
    if ((acr & FLASH_ACR_DCEN_Msk) >> FLASH_ACR_DCEN_Pos)
        mconf |= (1 << 10);
    printf(" FLASH conf.  : ACR=0x%08x - Prefetch=%s $I/$D=(%s,%s) latency=%d\r\n",
            (int)acr,
            bitToStr((acr & FLASH_ACR_PRFTEN_Msk) >> FLASH_ACR_PRFTEN_Pos),
            bitToStr((acr & FLASH_ACR_ICEN_Msk) >> FLASH_ACR_ICEN_Pos),
            bitToStr((acr & FLASH_ACR_DCEN_Msk) >> FLASH_ACR_DCEN_Pos),
            (int)((acr & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos));
#endif
#endif
}

uint32_t disableInts(void)
{
    uint32_t state;

    state = __get_PRIMASK();
    __disable_irq();

    return state;
}

void restoreInts(uint32_t state)
{
   __set_PRIMASK(state);
}


/* -----------------------------------------------------------------------------
 * low-level I/O functions
 * -----------------------------------------------------------------------------
 */

bool _ioWriteAllowed = true;

void ioDisableWrite(void)
{
    _ioWriteAllowed = false;
}

#ifndef _IO_OUT_PACKET_MODE
static int ioGetUint8(uint8_t *buff, int count, uint32_t timeout)
{
    HAL_StatusTypeDef status;

    if ((!buff) || (count <= 0))
        return -1;

    status = HAL_UART_Receive(&UartHandle, (uint8_t *)buff, count,
            timeout);

    if (status == HAL_TIMEOUT)
        return -1;

    return (status == HAL_OK ? count : 0);
}
#endif

#if defined(__GNUC__)

int _write(int fd, const void *buff, int count);

int _write(int fd, const void *buff, int count)
{
    HAL_StatusTypeDef status;

    if ((count < 0) && (fd != STDOUT_FILENO) && (fd != STDERR_FILENO)) {
        errno = EBADF;
        return -1;
    }

    if (_ioWriteAllowed)
        status = HAL_UART_Transmit(&UartHandle, (uint8_t *)buff, count,
                HAL_MAX_DELAY);
    else
        status = HAL_OK;

    return (status == HAL_OK ? count : 0);
}

#elif defined (__ICCARM__)

__ATTRIBUTES  size_t __write(int handle, const unsigned char *buffer,
                             size_t size);

__ATTRIBUTES  size_t __write(int handle, const unsigned char *buffer,
                             size_t size)
{
    HAL_StatusTypeDef status;

    /*
     * This means that we should flush internal buffers.  Since we
     * don't we just return.  (Remember, "handle" == -1 means that all
     * handles should be flushed.)
     */
    if (buffer == 0)
      return 0;

    /* This template only writes to "standard out" and "standard err",
     * for all other file handles it returns failure.
     */
    if ((handle != _LLIO_STDOUT) && (handle != _LLIO_STDERR))
        return _LLIO_ERROR;

    if (_ioWriteAllowed)
        status = HAL_UART_Transmit(&UartHandle, (uint8_t *)buffer, size,
                HAL_MAX_DELAY);
    else
        status = HAL_OK;

    return (status == HAL_OK ? size : _LLIO_ERROR);
}

#elif defined (__CC_ARM)

int fputc(int ch, FILE *f)
{
    if (_ioWriteAllowed)
      HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1,
            HAL_MAX_DELAY);

    return ch;
}

#else
#error ARM MCU tool-chain is not supported.
#endif

#if defined(__GNUC__)

void* __real_malloc(size_t bytes);
void __real_free(void *ptr);

#define MALLOC_TRACK_DEPTH_SIZE (16)
struct _malloc_track {
    void *aptr[MALLOC_TRACK_DEPTH_SIZE];
    size_t a[MALLOC_TRACK_DEPTH_SIZE];
    int aidx;
    uint32_t n_a;
    void *fptr[MALLOC_TRACK_DEPTH_SIZE];
    int fidx;
    uint32_t n_f;
    int n_af;
} malloc_track;


void* __wrap_malloc(size_t bytes)
{
    uint8_t *ptr;
    ptr = (uint8_t*)__real_malloc(bytes);

    if (ptr) {
        malloc_track.n_af++;
        malloc_track.n_a++;
        malloc_track.aptr[malloc_track.aidx] = ptr;
        malloc_track.a[malloc_track.aidx] = bytes;
        malloc_track.aidx++;

        if (malloc_track.aidx >= MALLOC_TRACK_DEPTH_SIZE)
            malloc_track.aidx = 0;
    }

    return ptr;
}

void __wrap_free(void *ptr)
{
    if (ptr) {
        malloc_track.n_af--;
        malloc_track.fptr[malloc_track.fidx] = ptr;
        malloc_track.fidx++;
        malloc_track.n_f++;
        if (malloc_track.fidx >= MALLOC_TRACK_DEPTH_SIZE)
            malloc_track.fidx = 0;
    }

    __real_free(ptr);
}

#endif

/* -----------------------------------------------------------------------------
 * AI-related functions
 * -----------------------------------------------------------------------------
 */

void aiLogErr(const ai_error err, const char *fct)
{
    if (fct)
        printf("E: AI error (%s) - type=%d code=%d\r\n", fct,
                err.type, err.code);
    else
        printf("E: AI error - type=%d code=%d\r\n", err.type, err.code);
}

__STATIC_INLINE const char* aiBufferFormatToStr(uint32_t val)
{
    if (val == AI_BUFFER_FORMAT_NONE)
        return "AI_BUFFER_FORMAT_NONE";
    else if (val == AI_BUFFER_FORMAT_FLOAT)
        return "AI_BUFFER_FORMAT_FLOAT";
    else if (val == AI_BUFFER_FORMAT_U8)
        return "AI_BUFFER_FORMAT_U8";
    else if (val == AI_BUFFER_FORMAT_Q15)
        return "AI_BUFFER_FORMAT_Q15";
    else if (val == AI_BUFFER_FORMAT_Q7)
        return "AI_BUFFER_FORMAT_Q7";
    else
        return "UNKNOWN";
}

ai_u32 aiBufferSize(const ai_buffer* buffer)
{
    return buffer->height * buffer->width * buffer->channels;
}

__STATIC_INLINE void aiPrintLayoutBuffer(const char *msg,
        const ai_buffer* buffer)
{
    printf("%s HWC layout:%d,%d,%ld (s:%ld f:%s)\r\n",
      msg, buffer->height, buffer->width, buffer->channels,
      aiBufferSize(buffer),
      aiBufferFormatToStr(buffer->format));
}

void aiPrintNetworkInfo(const ai_network_report* report)
{
  printf("Network configuration...\r\n");
  printf(" Model name         : %s\r\n", report->model_name);
  printf(" Model signature    : %s\r\n", report->model_signature);
  printf(" Model datetime     : %s\r\n", report->model_datetime);
  printf(" Compile datetime   : %s\r\n", report->compile_datetime);
  printf(" Runtime revision   : %s (%d.%d.%d)\r\n", report->runtime_revision,
    report->runtime_version.major,
    report->runtime_version.minor,
    report->runtime_version.micro);
  printf(" Tool revision      : %s (%d.%d.%d)\r\n", report->tool_revision,
    report->tool_version.major,
    report->tool_version.minor,
    report->tool_version.micro);
  printf("Network info...\r\n");
  /* printf("  signature         : 0x%lx\r\n", report->signature); */
  printf("  nodes             : %ld\r\n", report->n_nodes);
  printf("  complexity        : %ld MACC\r\n", report->n_macc);
  printf("  activation        : %ld bytes\r\n", aiBufferSize(&report->activations));
  printf("  weights           : %ld bytes\r\n", aiBufferSize(&report->weights));
  printf("  inputs/outputs    : %u/%u\r\n", report->n_inputs, report->n_outputs);
  aiPrintLayoutBuffer("  IN tensor format  :", &report->inputs);
  aiPrintLayoutBuffer("  OUT tensor format :", &report->outputs);
}


/* -----------------------------------------------------------------------------
 * Test-related functions
 * -----------------------------------------------------------------------------
 */

void aiTestHeader(const char* tname, int major, int minor)
{
    printf("\r\n#\r\n");
    printf("# %s %d.%d\r\n", tname , major, minor);
    printf("#\r\n");

#if defined(__GNUC__)
    printf("Compiled with GCC %d.%d.%d\r\n", __GNUC__, __GNUC_MINOR__,
            __GNUC_PATCHLEVEL__);
#elif defined(__ICCARM__)
    printf("Compiled with IAR %d (build %d)\r\n", __IAR_SYSTEMS_ICC__,
            __BUILD_NUMBER__
    );
#elif defined (__CC_ARM)
    printf("Compiled with MDK-ARM Keil %d\r\n", __ARMCC_VERSION);
#endif
}


/* -----------------------------------------------------------------------------
 * low-level nanopb functions
 * -----------------------------------------------------------------------------
 */

#include <pb.h>
#include <pb_encode.h>
#include <pb_decode.h>


#ifdef _IO_OUT_PACKET_MODE

#include <stm32msg.pb.h>


#define _PACKET_PAYLOAD_IN_SIZE (EnumLowLevelIO_IO_IN_PACKET_SIZE)

static struct o_packet {
    uint8_t pw;
    uint8_t payload[_PACKET_PAYLOAD_IN_SIZE];
} o_packet;

static bool write_packet(void) {
    HAL_StatusTypeDef status;

    status = HAL_UART_Transmit(&UartHandle, (uint8_t *)&o_packet,
            _PACKET_PAYLOAD_IN_SIZE + 1,
            HAL_MAX_DELAY);

    return (status == HAL_OK ? (_PACKET_PAYLOAD_IN_SIZE + 1) : 0);
}

void ioFlushWrite(void)
{
    o_packet.pw |= (1 << 7); /* Indicate last packet */
    write_packet();
    o_packet.pw = 0;
}

static bool write_callback(pb_ostream_t *stream, const uint8_t *buf,
        size_t count)
{
    bool res = true;
    uint8_t *pr = (uint8_t *)buf;
    while (count) {
        for (; o_packet.pw < _PACKET_PAYLOAD_IN_SIZE && count; o_packet.pw++) {
            o_packet.payload[o_packet.pw] = *pr;
            pr++;
            count--;
        }
        if (o_packet.pw == _PACKET_PAYLOAD_IN_SIZE) {
                res = write_packet();
                o_packet.pw = 0;
        }
    }
    return res;
}


#define _PACKET_PAYLOAD_OUT_SIZE (EnumLowLevelIO_IO_OUT_PACKET_SIZE)

static struct i_packet {
    uint8_t pr;
    uint8_t payload[_PACKET_PAYLOAD_OUT_SIZE];
} i_packet;

static int i_ridx = 0;

static bool read_packet(void) {
    HAL_StatusTypeDef status;

    status = HAL_UART_Receive(&UartHandle, (uint8_t *)&i_packet,
            _PACKET_PAYLOAD_OUT_SIZE + 1,
            HAL_MAX_DELAY);

    i_ridx = 0;

    return (status == HAL_OK ? (_PACKET_PAYLOAD_OUT_SIZE + 1) : 0);
}

void ioFlushRead(void)
{
    i_packet.pr = 0xFF;
    i_ridx = 0;
}

static bool read_callback(pb_istream_t *stream, uint8_t *buf, size_t count)
{
    bool res = true;
    uint8_t *pw = (uint8_t *)buf;

    if (count == 0)
        return true;

    if (i_packet.pr == 0xFF)
        res = read_packet();

    if (res == false)
        return res;

    while (count) {
        for (; i_packet.pr > 0 && count; i_packet.pr--) {
            *pw = i_packet.payload[i_ridx];
            pw++;
            count--;
            i_ridx++;
        }
        if (count && i_packet.pr == 0) {
            uint8_t sync = 0xAA;
            HAL_UART_Transmit(&UartHandle, &sync, 1,
                        HAL_MAX_DELAY);
            read_packet();
        }
    }

    return res;
}

#else

void ioFlushWrite(void)
{

}

static bool write_callback(pb_ostream_t *stream, const uint8_t *buf,
        size_t count)
{
    HAL_StatusTypeDef status;

    status = HAL_UART_Transmit(&UartHandle, (uint8_t *)buf, count,
            HAL_MAX_DELAY);

    return (status == HAL_OK ? count : 0);
}

static bool read_callback(pb_istream_t *stream, uint8_t *buf, size_t count)
{
    int result = 0;

    if (count == 0)
        return true;

    if (buf == NULL) {
        uint8_t tmp;
        do {
            ioGetUint8(&tmp, 1, HAL_MAX_DELAY);
        } while (count--);
        return true;
    }

    result = ioGetUint8(buf, count, HAL_MAX_DELAY);

    return result == count;
}
#endif


pb_ostream_t pb_ostream_to_uart(int fd)
{
    pb_ostream_t stream = {&write_callback, (void*)(intptr_t)fd, SIZE_MAX, 0};
    return stream;
}

pb_istream_t pb_istream_from_uart(int fd)
{
    pb_istream_t stream = {&read_callback, (void*)(intptr_t)fd, SIZE_MAX};
    return stream;
}
