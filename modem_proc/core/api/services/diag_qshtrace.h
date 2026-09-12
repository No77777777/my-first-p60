
/*!
@ingroup diag_qshtrace
  @file Diag_QSHTRACE.h 
  @brief
  Diagnostic Services API definitions for Qshtrace

  This file contains APIs which can be called by clients to be used with QShtrace in order to write data over STM od DIAGBUF.
  
Copyright (c)  2016 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*/

/*===========================================================================

                        EDIT HISTORY FOR MODE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/api/services/diag_qshtrace.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
05/12/16   sa      File Creation

===========================================================================*/


#include "comdef.h"



/*!
@ingroup diag_qshtrace
@brief
   Clients can call this API to submit QSHTRACE packets to diag. 
   If QSHTRACE is enabled using the "diag_qshtrace_enabled" API and STM is also enabled, the packet will be written immediately to STM.
   If QSHTRACE is not enabled, the packet will be dropped.
   If QSHTRACE is enabled but STM is disabled,the data will be buffered and drained later by diag task.
   
   @param[in]
      hdr_ptr - Pointer to the header to be written
      hdr_len - Length of the header (in bytes) to be written
      data_ptr - Void pointer to the data to be written
      data_len - Length (in bytes) of data to be written
   
   @dependencies
      "DIAG_QSHTRACE_ENABLE" API should be called to set qshtrace enable for diag.

   @return
      None

   @sideeffects
      None

   @sa
      diag_qshtrace_enable
*/


void diag_qshtrace_write(void *hdr_ptr, uint32 hdr_len, void *data_ptr, uint32 data_len);

/*!
@ingroup diag_qshtrace
@brief
   Clients can call this API to submit QSHTRACE packets to diag using a fixed 4 byte header.
   If QSHTRACE is enabled using the "diag_qshtrace_enabled" API and STM is also enabled, the packet will be written immediately to STM.
   If QSHTRACE is not enabled, the packet will be dropped.
   If QSHTRACE is enabled but STM is disabled,the data will be buffered and drained later by diag task.
   
   @param[in]
      header - 4 byte header value. Handled as a byte array.
      data_ptr - Void pointer to the data to be written
      data_len - Length (in bytes) of data to be written
   
   @dependencies
      "DIAG_QSHTRACE_ENABLE" API should be called to set qshtrace enable for diag.

   @return
      None

   @sideeffects
      None

   @sa
      diag_qshtrace_enable
*/


void diag_qshtrace_header32_write(uint32 header, void *data_ptr, uint32 data_len);


/*!
@ingroup diag_qshtrace
@brief
   Clients should call this API to indicate to diag that qshtrace should be enabled.
   Once this API is called to enable qshtrace, any data written to diag using diah_qshtrace_write will be send over QDSS.
   @param[in] 
      TRUE - Set qshtrace to enable
      FALSE - Set qshtrace to disable
   
   @dependencies
      None

   @return
      None

   @sideeffects
      None

   @sa
      None
*/

void diag_qshtrace_enable(boolean mode);


/*!
@ingroup diag_qshtrace
@brief
   Clients can call this API to get the DiagID assigned to this image.
   The id is hardcoded as of now. Will be changed later and assigned dynamically by APPS.

   @param[in] 
    None
   
   @dependencies
    None

   @return
    ClientID

   @sideeffects
   None

   @sa
   
*/


//PUBLIC API: Return the diagID
uint8 diag_get_diagID();

