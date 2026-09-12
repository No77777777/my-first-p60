/*!
  @file
  qsh_bc.h

  @brief
  Public interface for QSH Binary collect.
*/

/*==============================================================================

  Copyright (c) 2019 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/utils.mpss/5.4/api/qsh/qsh_mdb.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------

01/31/19   am      Initial check-in
==============================================================================*/
#ifndef QSH_MDB_H
#define QSH_MDB_H

#include <comdef.h>
#include <sys.h>
#include <qsh_types.h>


typedef uint32 * qsh_mdb_context_id_t;

typedef uint32  qsh_mdb_data_id_t;

/*!
  @brief
  Error codes for BINARY COLLECT requests to be reported to external client 
*/
typedef enum {
  /*! SUCCESS */
  QSH_MDB_SUCCESS,

  /*! Duplicate request */
  QSH_MDB_DUP_REQ,

  /*! Feature not enabled */
  QSH_MDB_NOT_ENABLED,

  /*! Invalid data ID */
  QSH_MDB_INVALID_DATA_ID,

  /*! issue in collecting data from internal client */
  QSH_MDB_RSP_FAIL,

  /*! Invalid request, input arguments are not correct */
  QSH_MDB_INVALID_REQ,
} qsh_mdb_err_e;


/*!
  @brief
  Represent binary data collect external client callback 
  argument structure.
*/
typedef struct {
  /*! Address of Bin data*/
  void*                        address;

  /*! Size of binary data */
  uint32                       size;

  /*! Data ID of collected binary data */
  qsh_clt_e                    client_id;
  
  /*! Data ID requested */
  const char*                  data_id_str;

  /*! SUB ID of data */
  sys_modem_as_id_e_type       sub_id;

  /*! Binary data collect status after response from internal client */
  qsh_mdb_err_e          status;
} qsh_mdb_ext_s;

/*! 
  @brief
  External client callback function type 
*/
typedef void (*qsh_mdb_ext_cb_t)(qsh_mdb_ext_s * bin_data);

/*==============================================================================

  FUNCTION: qsh_client_mdb_ext

==============================================================================*/
/*!
  @brief
  To be called by external clients like BSI to save binary data.

  This function will start the bin data collect procedure with internal clients
*/
/*============================================================================*/ 
qsh_mdb_err_e qsh_client_mdb_ext
(
  /*! Client ID from which to collect binary data*/
  qsh_clt_e                    client, 

  /*! Binary Data ID, trasnparent to QSH */
  uint8*                       data_id_str, 

  /*! external client callback function */
  qsh_mdb_ext_cb_t             client_cb,

  sys_modem_as_id_e_type       sub_id
);

/*==============================================================================

  FUNCTION: qsh_client_mdb

==============================================================================*/
/*!
  @brief
  To be called by internal clients to save binary data. This function will save 
  data to QSH internal memory and send a message to QSH task. 
*/
/*============================================================================*/ 
boolean qsh_client_mdb 
(
  /*! Context id */
  qsh_mdb_context_id_t         context_id,

  /*! Source address of binary data */
  void *                       address,

  /*! Size of binary data */
  uint32                       size,

  /*! Binary Data ID, transparent to QSH */
  qsh_mdb_data_id_t            data_id,

  /*! Sub ID */
  sys_modem_as_id_e_type       sub_id
);

#endif
