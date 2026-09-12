/*==========================================================================

  DIAG DRAIN PARAMETERS MACRO VALUES

DESCRIPTION
  This file has the macro values that are used to configure diag drain parameters
  through diag commands


Copyright (c) 2016 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/services/diag/DCM/common/src/diagdrain_param.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
09/19/16   nk      Changes to configure Diag drain parameters through Diag Commands

===========================================================================*/


/* Maximum and Minimum Values of diag drain parameters in Real Time Mode*/
#define   DIAGBUF_COMMIT_THRESHOLD_MIN 0x1000 /* (1024*4) */
#define   DIAGBUF_COMMIT_THRESHOLD_MAX 0x3000 /* (1024*12) */
#define   DIAG_DRAIN_TIMER_LEN_MIN  0x32 /* 50 */ 
#define   DIAG_DRAIN_TIMER_LEN_MAX  0x190 /* 400 */
#define   EVENT_TIMER_LEN_MIN 0x1F4 /* 500 */
#define   EVENT_TIMER_LEN_MAX 0xBB8 /* 3000 */
#define   EVENT_RPT_PKT_LEN_SIZE_MIN 0x100
#define   EVENT_RPT_PKT_LEN_SIZE_MAX 0x300
#define   MAX_EVENT_SEND_MIN 0x19 /* 25 */
#define   MAX_EVENT_SEND_MAX 0x64 /* 100 */
#define   EVENT_RPT_PKT_SIZE_MIN 0x40 /* 64 */
#define   EVENT_RPT_PKT_SIZE_MAX 0x100 /* 256 */


/* Maximum and Minimum Values of diag drain parameters in Non-Real Time Mode*/
#define   DIAGBUF_COMMIT_THRESHOLD_NRT_MIN 0x8000  /* (1024*32)*/
#define   DIAGBUF_COMMIT_THRESHOLD_NRT_MAX 0x12000 /* (1024*72)*/
#define   DIAG_DRAIN_TIMER_LEN_NRT_MIN  0
#define   DIAG_DRAIN_TIMER_LEN_NRT_MAX  0
#define   EVENT_TIMER_LEN_NRT_MIN 0
#define   EVENT_TIMER_LEN_NRT_MAX 0
#define   EVENT_RPT_PKT_LEN_SIZE_NRT_MIN 0X800  /* (1024*2)*/
#define   EVENT_RPT_PKT_LEN_SIZE_NRT_MAX 0X1000 /* (1024*4)*/
#define   MAX_EVENT_SEND_NRT_MIN 0x400 /* 512 */
#define   MAX_EVENT_SEND_NRT_MAX 0x800 /* (1024*2) */
#define   EVENT_RPT_PKT_SIZE_NRT_MIN 0x1F4 /* 500 */
#define   EVENT_RPT_PKT_SIZE_NRT_MAX 0x7D0 /* 2000 */


/* Bit mask values for diag drain parameters that can be set through diag commands  */
#define   BIT_DIAGBUF_COMMIT_THRESHOLD  0x0001
#define   BIT_DIAG_DRAIN_TIMER_LEN      0x0002
#define   BIT_EVENT_STALE_TIMER_LEN     0x0004
#define   BIT_EVENT_REPORT_PKT_LEN_SIZE 0x0008
#define   BIT_EVENT_SEND_MAX_NUM        0x0010
#define   BIT_EVENT_REPORT_PKT_SIZE     0x0020


/* To indicate the version of diag config drain parameters command  */
#define  DRAIN_PARAM_CMD_VERSION_1  1

/* To indicate the status of diag config drain parameters command  */
#define  DRAIN_PARAM_CMD_STATUS_SUCCESS 1
#define  DRAIN_PARAM_CMD_STATUS_FAILURE 0

/* To indicate the mode(RT/NRT) of diag   */
#define  DRAIN_PARAM_CMD_RT_MODE 1  /* Real Time Mode */
#define  DRAIN_PARAM_CMD_NRT_MODE 0 /* Non-Real Time Mode */






