/******************************************************************************
  @file    qmi_cci_target.c
  @brief   OS Specific routines internal to QCCI.

  DESCRIPTION
  This source provides an OS abstraction to QCCI.
  
  The current design is to use QuRT abstraction for all interfaces except
  when the user is involved. If the user provides NULL as the rex TCB type,
  the QuRT abstraction is automatically used.

  ---------------------------------------------------------------------------
  Copyright (c) 2015-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
  ---------------------------------------------------------------------------
*******************************************************************************/
/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header: //components/rel/core.mpss/10.0/mproc/qmi/qcsi/qmi_csi_target.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
===========================================================================*/

/*===========================================================================
                      INCLUDES
===========================================================================*/

#ifdef FEATURE_QMI_ULOG
#include "ULogFront.h"
#include "DALStdErr.h"
#endif

#include "qmi_csi_target.h"
#include "qmi_common.h"
#include "qmi_idl_lib_internal.h"

#ifdef FEATURE_QMI_SMEM_LOG
#include "smem_log.h"

#define QMI_CSI_LOG_EVENT_TX            (SMEM_LOG_QMI_CSI_EVENT_BASE + 0x00)
#define QMI_CSI_LOG_EVENT_TX_EXT        (SMEM_LOG_QMI_CSI_EVENT_BASE + 0x04)
#define QMI_CSI_LOG_EVENT_RX            (SMEM_LOG_QMI_CSI_EVENT_BASE + 0x01)
#define QMI_CSI_LOG_EVENT_RX_EXT        (SMEM_LOG_QMI_CSI_EVENT_BASE + 0x05)
#define QMI_CSI_LOG_EVENT_ERROR         (SMEM_LOG_QMI_CSI_EVENT_BASE + 0x03)
#endif

size_t strlcpy(char *dst, const char *src, size_t siz);

#ifdef FEATURE_QMI_ULOG

#define REQ_STR  "REQ"
#define RESP_STR "RESP"
#define IND_STR  "IND"
#define INVALID_STR  "INVALID"

/* These are defined in scons */
#ifndef QMI_OS_LOG_BUFFER_SIZE
#define QMI_OS_LOG_BUFFER_SIZE       ( 0x2000 )
#endif

#ifndef QMI_OS_ERROR_LOG_BUFFER_SIZE
#define QMI_OS_ERROR_LOG_BUFFER_SIZE      ( 0x400 )
#endif


/* ULOG handle */
static ULogHandle qcsi_ulog_handle = NULL;
static ULogHandle qcsi_error_ulog_handle = NULL;
#endif


void qmi_csi_os_signal_set(qmi_csi_os_params *sig)
{
#ifdef FEATURE_QMI_CSI_REX_SUPPORTED
  if(sig->tcb)
  {
    rex_set_sigs(sig->tcb, (rex_sigs_type)sig->sig);
    return;
  }
#endif
  ASSERT(sig->signal);
  if(sig->signal)
  {
    qurt_anysignal_set(sig->signal, sig->sig);
  }
}

void qmi_csi_os_log_tx
(
  qmi_idl_service_object_type svc_obj,
  uint32_t cntl_flag,
  uint32_t txn_id,
  uint32_t msg_id,
  uint32_t msg_len,
  void *addr,
  size_t addr_len
)
{
  uint32_t *int_addr = (uint32_t *)(addr);
#ifdef FEATURE_QMI_SMEM_LOG
  SMEM_LOG_EVENT6(QMI_CSI_LOG_EVENT_TX_EXT, (cntl_flag << 16) | txn_id, 
                  (msg_id << 16) | msg_len,    svc_obj->service_id, 
                  int_addr[0], int_addr[1], int_addr[2]);
#endif
#ifdef FEATURE_QMI_ULOG
  if( qcsi_ulog_handle ) \
  {
    char *str;
    switch(cntl_flag)
    {
      case QMI_REQUEST_CONTROL_FLAG:
         str = REQ_STR;
         break;
      case QMI_RESPONSE_CONTROL_FLAG:
        str = RESP_STR;
        break;
      case QMI_INDICATION_CONTROL_FLAG:
        str = IND_STR;
        break;
      default:
        str = INVALID_STR;
    }

    ULogFront_RealTimePrintf( qcsi_ulog_handle, 8, "QCSI TX %s: Svc_id:0x%x "
                                 "clnt_addr:0x%x:0x%x:0x%x Txn:0x%x Msg:0x%x Len:0x%x",
                                 str, svc_obj->service_id, int_addr[0], int_addr[1], 
                                 int_addr[2], txn_id, msg_id, msg_len);
  }
#endif
}

void qmi_csi_os_log_rx
(
  qmi_idl_service_object_type svc_obj,
  uint32_t cntl_flag,
  uint32_t txn_id,
  uint32_t msg_id,
  uint32_t msg_len,
  void *addr,
  size_t addr_len
)
{
  uint32_t *int_addr = (uint32_t *)(addr);
#ifdef FEATURE_QMI_SMEM_LOG
  SMEM_LOG_EVENT6(QMI_CSI_LOG_EVENT_RX_EXT, (cntl_flag << 16) | txn_id, 
                    (msg_id << 16) | msg_len, svc_obj->service_id,
                    int_addr[0], int_addr[1], int_addr[2]);
#endif
#ifdef FEATURE_QMI_ULOG
  if( qcsi_ulog_handle )
  {
    char *str;
    switch(cntl_flag)
    {
      case QMI_REQUEST_CONTROL_FLAG:
         str = REQ_STR;
         break;
      case QMI_RESPONSE_CONTROL_FLAG:
        str = RESP_STR;
        break;
      case QMI_INDICATION_CONTROL_FLAG:
        str = IND_STR;
        break;
      default:
        str = INVALID_STR;
    }
    
    ULogFront_RealTimePrintf( qcsi_ulog_handle, 8, "QCSI RX %s: Svc_id:0x%x "
                              "clnt_addr:0x%x:0x%x:0x%x Txn:0x%x Msg:0x%x Len:0x%x", 
                              str, svc_obj->service_id, int_addr[0], int_addr[1], 
                              int_addr[2], txn_id, msg_id, msg_len);
  }
#endif
}

void qmi_csi_os_log_error(char *filename, unsigned int line)
{
#ifdef FEATURE_QMI_SMEM_LOG
  uint32 name[5];
  strlcpy((char *)name, filename, sizeof(name));
  SMEM_LOG_EVENT6(QMI_CSI_LOG_EVENT_ERROR, name[0], name[1], name[2], name[3], 
      name[4], line);
#endif
#ifdef FEATURE_QMI_MSG
  MSG_2(MSG_SSID_ONCRPC, MSG_LEGACY_ERROR,
        "Runtime error. File 0x%s, Line: %d", filename, line);
#endif

#ifdef FEATURE_QMI_ULOG
  if( qcsi_error_ulog_handle )
  {
    ULogFront_RealTimePrintf( qcsi_error_ulog_handle, 3, "QCSI Error at %s:%d", filename, line );
  }
#endif
}

/*===========================================================================
  FUNCTION  qmi_csi_os_init
===========================================================================*/
/*!
@brief

  QCSI OS INIT function

@return
  None

@note
  None
*/
/*=========================================================================*/
void qmi_csi_os_init(void)
{
#ifdef FEATURE_QMI_ULOG
  ULogResult res;

  if( NULL == qcsi_ulog_handle )
  {
    /* Initialize uLOG buffer */
    res = ULogFront_RealTimeInit( &qcsi_ulog_handle,
                            "QMI_TXN_LOG",
                            QMI_OS_LOG_BUFFER_SIZE,
                            ULOG_MEMORY_LOCAL,
                            ULOG_LOCK_OS );
                            
    if(res != DAL_SUCCESS && res != ULOG_ERR_ALREADYCREATED)
    {
      qcsi_ulog_handle = 0;
    }
  }
  
  if( NULL == qcsi_error_ulog_handle )
  {
    /* Initialize Err uLOG buffer */
    res = ULogFront_RealTimeInit( &qcsi_error_ulog_handle,
                            "QMI_ERR_LOG",
                            QMI_OS_ERROR_LOG_BUFFER_SIZE,
                            ULOG_MEMORY_LOCAL,
                            ULOG_LOCK_OS );
    
    if(res != DAL_SUCCESS && res != ULOG_ERR_ALREADYCREATED)
    {
      qcsi_error_ulog_handle = 0;
    }
  }
  
#endif
} /* qmi_csi_os_init */

void qmi_csi_os_deinit(void)
{
  return;
} /* qmi_csi_os_deinit */

