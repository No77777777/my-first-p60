/*============================================================================
 @file gera_api.h

 GERA common module API interface

 GENERAL DESCRIPTION

 This file defines interface for the GNSS Engine Resource Arbitrator(GERA)

 EXTERNALIZED FUNCTIONS


 INITIALIZATION AND SEQUENCING REQUIREMENTS

 Copyright (c) 2015 Qualcomm Atheros, Inc.
 Qualcomm Atheros Confidential and Proprietary. All Rights Reserved.

 Export of this technology or software is regulated by the U.S. Government.
 Diversion contrary to U.S. law prohibited.

 Copyright (c) 2016 Qualcomm Technologies, Inc.
 All Rights Reserved.
 Confidential and Proprietary - Qualcomm Technologies, Inc.

 =============================================================================*/

/*============================================================================

 EDIT HISTORY FOR FILE

 This section contains comments describing changes made to the module.

 $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/gera/api/gera_api.h#1 $
 $DateTime: 2019/10/24 01:30:36 $
 $Author: pwbldsvc $

 when       who     what, where, why
 --------   ---     ----------------------------------------------------------
 04/04/16   ssu     Initial version

 =============================================================================*/

#ifndef _GERA_API_H_
#define _GERA_API_H_

#include "comdef.h"
#include "gps_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* sets the specified bit in a u32 bit mask. bit counts range: 0-31 */
#define GERA_SET_U32BITMASK_BIT(n) (((uint32)0x1) << (n))

/* The mask of different clients supported by GERA */
#define GERA_CLIENT_E911_MASK           GERA_SET_U32BITMASK_BIT(0) /* 0x1 */
#define GERA_CLIENT_APMP_TTR_MASK       GERA_SET_U32BITMASK_BIT(1) /* 0x2 */
#define GERA_CLIENT_DPO_MASK            GERA_SET_U32BITMASK_BIT(2) /* 0x4 */
#define GERA_CLIENT_STANDALONE_MASK     GERA_SET_U32BITMASK_BIT(3) /* 0x8 */
#define GERA_CLIENT_MSA_MASK            GERA_SET_U32BITMASK_BIT(4) /* 0x10 */
#define GERA_CLIENT_MSB_MASK            GERA_SET_U32BITMASK_BIT(5) /* 0x20 */

/* Gera client IDs for different clients */
typedef enum
{
  GERA_CLIENT_ID_FIRST = 0,
  /* Client Id for E911 */
  GERA_CLIENT_ID_E911 =  GERA_CLIENT_ID_FIRST,
  /* Client Id for AP MP time transfer */
  GERA_CLIENT_ID_AP_MP_TTR = 1,
  /* Client Id for DPO */
  GERA_CLIENT_ID_DPO = 2,
  /* Client Id for Standalone */
  GERA_CLIENT_ID_STANDALONE = 3,
  /* Client Id for MSA */
  GERA_CLIENT_ID_MSA = 4,
  /* Client Id for MSB */
  GERA_CLIENT_ID_MSB = 5,
  GERA_CLIENT_ID_LAST = GERA_CLIENT_ID_MSB,
  GERA_CLIENT_ID_MAX = 0xFFFFFFF
} gera_client_id_e_type;

/* GERA operation status */
typedef enum
{
  /* GERA operation status FAILURE for an unknown reason.  */
  GERA_OP_STATUS_ERROR_UNKNOWN = 0,
  /* GERA operation status FAILURE if client is unregistered. */
  GERA_OP_STATUS_ERROR_CLIENT_UNREGISTERED = 1,
  /* GERA operation status FAILURE if client is already registered. */
  GERA_OP_STATUS_ERROR_CLIENT_ALREADY_REGISTERED = 2,
  /* GERA operation status FAILURE if client is unregistered. */
  GERA_OP_STATUS_ERROR_BAD_PARAMS = 3,
  /* GERA operation status FAILURE for memory allocation failure  */
  GERA_OP_STATUS_ERROR_MEM_ALLOC_FAILURE = 4,
  /* GERA was able to provide the ACCESS grant to the client*/
  GERA_OP_STATUS_ACCESS_GRANT = 5,
  /* GERA was able to provide ACCESS DENIED to the client*/
  GERA_OP_STATUS_ACCESS_DENIED = 6,
  /* GERA operation completed succesfully. */
  GERA_OP_STATUS_REQUEST_SUCCESS = 7,
  /* GERA operation resulted in a failure */
  GERA_OP_STATUS_REQUEST_FAILURE = 8,
  GERA_OP_STATUS_ERROR_MAX = 0xFFFFFFFF
} gera_op_status_e_type;

/* GERA client states */
typedef enum
{
  /* Unregistered: The client is not registered and thus the client cannot be controlled by any controller.  */
  GERA_CLIENT_STATE_UNREGISTERED = 0,
  /* CLIENT_STATE_DISABLED: The client is registered, but it does not have the access grant . The client is INACTIVE at this point. */
  GERA_CLIENT_STATE_DISABLED = 1,
  /* CLIENT_STATE_ENABLING: GERA has received the new client access request and is in the process of removing the access to any lower priority clients. */
  GERA_CLIENT_STATE_ENABLING = 2,
  /* CLIENT_STATE_ENABLED: The client has access grant. The client is ACTIVE at this point. */
  GERA_CLIENT_STATE_ENABLED = 3,
  /* CLIENT_STATE_DISABLING: GERA is in the process of removing the access from the client. This involves revoking the access of dependent clients */
  GERA_CLIENT_STATE_DISABLING = 4,
  GERA_CLIENT_STATE_MAX = 0xFFFFFFFF
} gera_client_state_e_type;


/* GERA client callbacks */
typedef enum
{
  GERA_CB_TYPE_ACCESS_GRANT = 0,
  GERA_CB_TYPE_ACCESS_DENIED = 1,
  GERA_CB_TYPE_ACCESS_REVOKE = 2,
  GERA_CB_TYPE_MAX = 0xFFFFFFFF
}gera_client_cb_e_type;


/* Data type having the information for the GERA client callbacks. */
typedef struct
{
  /* Type of callback */
  gera_client_cb_e_type e_cb_type;

  /* The client handler identifying the client type */
  gera_client_id_e_type e_client_id;

  /* Information to indicate if the callback was due to a GERA reset */
  boolean v_is_gera_reset;

  /* The client which caused the revoking of access. Only applicable when callback type is Revoke */
  gera_client_id_e_type e_client_causing_revoke;

  /* Return code from the GERA operation */
  gera_op_status_e_type e_return_code;
}gera_client_cb_data_s_type;

/* -----------------------------------------------------------------------*//**
@brief
  Function callback prototype for the GERA client.
@param[in]   p_client_cb_data    The additional information about the callbacks.

@retval    NONE

*//* ------------------------------------------------------------------------*/
typedef void (gera_client_cb_f_type)
(
  const gera_client_cb_data_s_type *p_client_cb_data
);


typedef struct
{
  gera_client_cb_f_type *p_cb_func;
}gera_client_cb_s_type;

/* -----------------------------------------------------------------------*//**
@brief
  This function is called to initialize GERA module.

@retval      gera_op_status_e_type   The operation status of the GERA.
*//* ------------------------------------------------------------------------*/
gera_op_status_e_type gera_init(void);

/* -----------------------------------------------------------------------*//**
@brief
  Function to register the clients with GERA common module.

@param[in]   p_cb                            The client callback needed to regsiter with GERA.
@param[in]   e_client_id                     The client ID which identifies the type of client.
@retval    gera_op_status_e_type             The operation status of the GERA.

*//* ------------------------------------------------------------------------*/
gera_op_status_e_type gera_client_register(const gera_client_cb_s_type *p_cb,
                                           const gera_client_id_e_type e_client_id);


/* -----------------------------------------------------------------------*//**
@brief
  Function to de-register the clients with GERA common module.

@param[in]   e_client_id           The client ID which identifies the type of client.
@retval    gera_op_status_e_type   The operation status of the GERA.

*//* ------------------------------------------------------------------------*/
gera_op_status_e_type gera_client_deregister(const gera_client_id_e_type e_client_id);


/* -----------------------------------------------------------------------*//**
@brief
  This function is called when client controller decides to enable the client.
  GERA uses this function to consider the client's request and synchronously responds to the client with a decision
  to enable the client or not.
  GERA first enables the client which is specified in the e_client_id . If this client was succesfully enabled, then
  it tries to enable the additional dependent clients in the mask. 


@param[in]   e_client_id           The client ID which identifies the type of client. This field can have the client ID of
                                   dependent or independent client.
                                   If this field has a dependent client ID , then the q_dep_client_id_mask has to be 0 
                                   otherwise an error is returned. 
                                   If this field has an independent client ID , then the q_dep_client_id_mask can have the masks 
                                   for dependent clients. If the mask has an independent client then its an error. 
@param[in]   q_dep_client_id_mask  The dependent client ID mask which is used to enable additional dependent clients.
                                   this field may or maynot be 0 . This field needs to have the mask of dependent clients ONLY.                                                                        
@retval    gera_op_status_e_type   The operation status of the GERA. Th return value of this API indicates the enable request 
                                   status of the e_client_id. The enable request status of the dependent clients are conveyed by GERA
                                   through callbacks to the clients in the mask.
*//* ------------------------------------------------------------------------*/
gera_op_status_e_type gera_client_enable_request(const gera_client_id_e_type e_client_id, 
                                                 const uint32 q_dep_client_id_mask);


/* -----------------------------------------------------------------------*//**
@brief
  This function is called when client controller decides to disable the client.
  GERA uses this function to consider the client's request and synchronously responds to the client with a decision
  to disable the client or not.
  If this API returns a SUCCESS then the client is expected to clean up its state and synchronously respond to GERA 
  that it has disabled using the gera_client_disabled API

@param[in]   e_client_id           The client ID which identifies the type of client.

@retval    gera_op_status_e_type   The operation status of the GERA.
*//* ------------------------------------------------------------------------*/
gera_op_status_e_type gera_client_disable_request(const gera_client_id_e_type e_client_id);


/* -----------------------------------------------------------------------*//**
@brief
  This function is called by the client controller to notify GERA that it has 
  successfully disabled the client has disabled the client.

@param[in]   e_client_id             The client ID which identifies the type of client.
@retval      gera_op_status_e_type   The operation status of the GERA.

*//* ------------------------------------------------------------------------*/
gera_op_status_e_type gera_client_disabled(const gera_client_id_e_type e_client_id);


/* -----------------------------------------------------------------------*//**
@brief
  This function is called when client controller needs to reset the GERA information.
  This function resets the state of all the registered clients of GERA to DISABLED.
  However GERA does not reset the registration information of the client. So clients 
  need not register with GERA again after the reset operation.

@retval      gera_op_status_e_type   The operation status of the GERA.
*//* ------------------------------------------------------------------------*/
gera_op_status_e_type gera_reset(void);


/* -----------------------------------------------------------------------*//**
@brief
  This function is used by the client controller to query the state of the GERA clients in the system 

@retval  A mask of ENABLED/DISABLED status for each GERA client in the system.
Each bit in the mask corresponds to a GERA client . The bit mask for each client is already defined 
in the API header file .
For each client an ENABLED shall be returned only if the client has been granted ACCESS by GERA.
In all other cases GERA shall return DISABLED for the client. 

*//* ------------------------------------------------------------------------*/
uint32 gera_client_query_state(void);

/* -----------------------------------------------------------------------*//**
@brief
  This function is used to enable a dependent client from a set of dependent clients using the GERA rules.
  GERA uses this function to consider the client's request and synchronously responds to the client with a decision
  to enable the client or not .
  If there is no independent client enabled in the system , then GERA responds with an error for this request.
  Only one independent client can be active at a time in the system . 
  If a dependent client is selected to be enabled then it is notified via callbacks. Only one dependent client 
  is enabled from the mask of dependent clients. 
  
@param[in]   q_dep_client_id_mask  The dependent client ID mask which is used to enable additional dependent clients.
                                   this field may or maynot be 0 . This field needs to have the mask of dependent clients ONLY.                                                                        
@retval    gera_op_status_e_type   The operation status of the GERA. Th return value of this API indicates the enable request 
                                   status of the e_client_id. The enable request status of the dependent clients are conveyed by GERA
                                   through callbacks to the clients in the mask.
*//* ------------------------------------------------------------------------*/
gera_op_status_e_type gera_enable_dependent_clients_request(const uint32 q_dep_client_id_mask);

#ifdef __cplusplus
}
#endif

#endif  //_GERA_API_H_

