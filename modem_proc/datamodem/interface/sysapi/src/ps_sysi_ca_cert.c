
/******************************************************************************
  @file    ps_sysi_ca_cert.c
  @brief   

  DESCRIPTION
  This file defines function, variables and data structures for CA certificate
  feature

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  N/A

  ---------------------------------------------------------------------------
  Copyright (C) 2011-2018 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
******************************************************************************/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/sysapi/src/ps_sysi_ca_cert.c#2 $ $DateTime: 2021/02/09 01:09:33 $ $Author: pwbldsvc $
 
when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/17/18   shannonl     Created functions to support CA certificate feature
===========================================================================*/

/*---------------------------------------------------------------------------
                           INCLUDE FILES
---------------------------------------------------------------------------*/
#include "ps_sysi_ca_cert.h"
#include "ps_system_heap.h"
#include "ps_sys_ioctl.h"
#include "ps_sys_event.h"
#include "ds_Utils_DebugMsg.h"

/*Cache for all CA certificate information */
static ps_sys_ca_cert_client_state ps_sys_ca_cert_state;

/*=======================================================================
FUNCTION PS_SYSI_CA_CERT_INIT()

DESCRIPTION
  Initialize CA certificate client queue

PARAMETERS
  None

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_sysi_ca_cert_init( void )
{
  /*-----------------------------------------------------------------------
  Initialize the queue which will hold all info of CA certificate clients 
  ------------------------------------------------------------------------*/
  memset(&ps_sys_ca_cert_state, 0 , sizeof(ps_sys_ca_cert_state));
  (void)q_init(&ps_sys_ca_cert_state.ca_cert_client_info);
  ps_sys_ca_cert_state.ca_cert_count = 0;
}/* ps_sysi_ca_cert_init */

/*===========================================================================
FUNCTION PS_SYS_GET_CA_CERT_COUNT()

DESCRIPTION
  This function gets the CA certificate counts, increment the current count and
  returns the new count

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
uint32 ps_sys_get_ca_cert_count
(
  void
)
{
  return ++ps_sys_ca_cert_state.ca_cert_count;
}

/*===========================================================================
FUNCTION PS_SYSI_COMPARE_CA_INFO()

DESCRIPTION
  This is a comparison function for Cert_dn and cb_ptr.

DEPENDENCIES
  None.

RETURN VALUE
  1: Fpund a match
  0:.No match found

SIDE EFFECTS
  None.
===========================================================================*/

static int ps_sysi_compare_ca_info
( 
  void *item_ptr,
  void *compare_val 
)
{
  ps_sys_ca_cert_q_type *clnt_info = (ps_sys_ca_cert_q_type*)item_ptr;
  ps_sysi_ca_cert_compare_val_type *compared_val = (ps_sysi_ca_cert_compare_val_type*)compare_val;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 != memcmp(&clnt_info->cert_dn, &compared_val->cert_dn, clnt_info->cert_dn_length))
  {
    return 0;
  }

  if(clnt_info->client_cb_ptr != compared_val->client_cb_ptr)
  {
    return 0;
  }

  return 1;
}

/*===========================================================================
FUNCTION PS_SYSI_COMPARE_CA_TX_INFO()

DESCRIPTION
  This is a comparison function for tx_id.

DEPENDENCIES
  None.

RETURN VALUE
  1: Fpund a match
  0:.No match found

SIDE EFFECTS
  None.
===========================================================================*/
static int ps_sysi_compare_ca_tx_info
( 
  void *item_ptr,
  void *compare_val 
)
{
  ps_sys_ca_cert_q_type *clnt_info = (ps_sys_ca_cert_q_type*)item_ptr;
  uint32                 compared_val = *(uint32 *)compare_val;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return ((clnt_info->tx_id == compared_val )? 1:0);
}


/*===========================================================================
FUNCTION PS_SYS_MODIFY_CA_CERT_CLIENT()

DESCRIPTION
  This function modifies the CA certificate client info queue. It can add a 
  new request (node).

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: Operation success
  False:Operation Failure.

SIDE EFFECTS
  None.
===========================================================================*/
boolean ps_sys_modify_ca_cert_client
(
  uint8                                 ca_cert_dn[DS_SYS_CERT_DN_MAX],
  uint8                                 ca_cert_dn_len,
  uint32                                *tx_id,
  ds_sys_ca_cert_cback_f_ptr_type       client_cb_ptr,
  void                                 *user_data_ptr,
  int16                                *ps_errno
)
{
  ps_sys_ca_cert_q_type           * ca_cert_q_node = NULL;
  uint8                             error_reason = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(ca_cert_dn == NULL)
  {
    error_reason = PS_SYS_CA_CERT_ERR_INTERNAL;
    goto print_result;
  }
  
  if ((int32)ca_cert_dn_len > ((DS_SYS_CERT_DN_MAX)))
  {
    error_reason = PS_SYS_CA_CERT_INVALID_CERT_DN;
    goto print_result;
  }
  /*-------------------------------------------------------------------------
    Allocate a new event callback buffer.
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(ca_cert_q_node, sizeof(ps_sys_ca_cert_q_type), 
                           ps_sys_ca_cert_q_type*);
  if (ca_cert_q_node == NULL)
  {
    error_reason = PS_SYS_CA_CERT_NO_MEMORY;
    goto print_result;
  }

  *tx_id =  ps_sys_get_ca_cert_count();
  if(*tx_id == 0)
  {
    error_reason = PS_SYS_CA_CERT_INVALID_TX_ID;
    goto print_result;
  }

  memset(ca_cert_q_node, 0, sizeof(ps_sys_ca_cert_q_type));
  /*-------------------------------------------------------------------------
    Initialize the CA cert buffer
  -------------------------------------------------------------------------*/
  (void) q_link(ca_cert_q_node, &(ca_cert_q_node->link));
  ca_cert_q_node->tx_id          = *tx_id;
  ca_cert_q_node->cert_dn_length = ca_cert_dn_len;
  memscpy(&ca_cert_q_node->cert_dn, DS_SYS_CERT_DN_MAX,
          ca_cert_dn, ca_cert_dn_len);
  ca_cert_q_node->client_cb_ptr = client_cb_ptr;
  ca_cert_q_node->user_data_ptr = user_data_ptr;
  q_put(&(ps_sys_ca_cert_state.ca_cert_client_info), &(ca_cert_q_node->link));
  
  return TRUE;

  print_result:
  LOG_MSG_ERROR_1("ps_sys_modify_ca_cert_client() error %d", error_reason);
  return FALSE;
}/*  ps_sys_modify_ca_cert_client */


void ps_sys_send_ca_cert
(
  void                                  * send_ca_cert_ptr,
  ps_sys_subscription_enum_type           subscription_id,
  int16                                 * ps_errno
)
{
  ps_sys_ca_cert_q_type               * ca_cert_q_node = NULL;
  uint8                                 error_reason = 0;
  ps_sys_ioctl_send_ca_cert_info_type  *ca_cert_ptr = NULL; 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  ca_cert_ptr = (ps_sys_ioctl_send_ca_cert_info_type*)send_ca_cert_ptr;

  /* 1. traverse through the queue, find the matching tx_id node */
  ca_cert_q_node = q_linear_search(&ps_sys_ca_cert_state.ca_cert_client_info,
                                    ps_sysi_compare_ca_tx_info,
                                    &(ca_cert_ptr->tx_id));
  
  /* 2. if not found, abort the send CA operation */
  if (NULL == ca_cert_q_node) 
  {
    /* Cannot find the tx_id */
     error_reason = PS_SYS_CA_CERT_INVALID_TX_ID;
    *ps_errno = DS_ENOROUTE;
    goto send_result;
  }
  
  if(ca_cert_q_node->client_cb_ptr == NULL)
  {  
    error_reason = PS_SYS_CA_CERT_INVALID_CB;
    *ps_errno = DS_EHOSTNOTFOUND;
    goto send_result;
  }

  
  /* 3. If found, fill in the payload and call the cb_ptr */
  ca_cert_q_node->client_cb_ptr(ca_cert_q_node->cert_dn,
                                ca_cert_q_node->cert_dn_length,
                                ca_cert_ptr->failure_info,
                                ca_cert_ptr->ca_cert_q_ptr,
                                ca_cert_q_node->user_data_ptr);

  /* 4. Delete the node */
  if (TRUE != q_delete_ext(&ps_sys_ca_cert_state.ca_cert_client_info,
                           &ca_cert_q_node->link)) 
  {
    error_reason = PS_SYS_CA_CERT_OPERATION_FAILED;
    PS_SYSTEM_HEAP_MEM_FREE(ca_cert_q_node);
    goto send_result;
  }

  PS_SYSTEM_HEAP_MEM_FREE(ca_cert_q_node);

  send_result:
  LOG_MSG_ERROR_1("ps_sys_send_ca_cert() error %d", error_reason);
  return;
}/* ps_sys_send_ca_cert */
/*=======================================================================
FUNCTION PS_SYS_CA_CERT_RETRIEVAL()

DESCRIPTION
  Create a transaction ID for the CA retrieval request and post an internal 
  event PS_SYS_EVENT_GET_CA_CERTIFICATE

PARAMETERS
  void                            *ca_info_ptr
  ps_sys_subscription_enum_type   subscription_id
  int16                         * ps_errno

RETURN VALUE
  ret_val

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
uint16 ps_sys_ca_cert_retrieval
(
  void                            *info_ptr,
  ps_sys_subscription_enum_type   subscription_id,
  int16                         * ps_errno
)
{
  int16                                error;
  ps_sys_event_get_ca_certificate      ca_cert_event;
  int                                  ret_val = -1;
  ps_sys_ioctl_get_ca_cert_info_type *ca_info_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*--------------------------------------------------------------
Steps:
1. Fill in ps sys event payload
2. Create new q for this transaction
3. Post the event to QMI framework
---------------------------------------------------------------*/
  if( info_ptr == NULL)
  {
    error = PS_SYS_CA_CERT_ERR_INTERNAL;
    goto send_result;
  }
  ca_info_ptr = (ps_sys_ioctl_get_ca_cert_info_type*)info_ptr;

  memset(&ca_cert_event, 0, sizeof(ps_sys_event_get_ca_certificate));
  /* 1. Fill in ps sys event payload */

  ca_cert_event.cert_dn_length = ca_info_ptr->cert_dn_length;
  memscpy(&ca_cert_event.cert_dn, DS_SYS_CERT_DN_MAX,
          &ca_info_ptr->cert_dn ,ca_info_ptr->cert_dn_length);

  /* 2. Create new q for this transaction */
  if(ps_sys_modify_ca_cert_client(ca_cert_event.cert_dn, 
                                  ca_cert_event.cert_dn_length,
                                  &ca_cert_event.tx_id,
                                  ca_info_ptr->client_cb_ptr,
                                  ca_info_ptr->user_data_ptr,
                                  ps_errno))
  {
    /* 3. Post the event to QMI framework */
    ret_val = ps_sys_event_ind_ex(PS_SYS_TECH_ALL,
                                  PS_SYS_EVENT_GET_CA_CERTIFICATE,
                                  subscription_id,
                                  &ca_cert_event,
                                  ps_errno);
    if( 0 != ret_val )
    {
      error = PS_SYS_CA_CERT_ERR_INTERNAL;
      goto send_result;
    }
  }
  return 0;
  
send_result:
  LOG_MSG_ERROR_3("ps_sys_ca_cert_retrieval "
                  "ret_val = %d, ps_error = %d error %d", ret_val, ps_errno, error);
  return -1;
}

/*=======================================================================
FUNCTION PS_SYS_CA_CERT_CANCEL()

DESCRIPTION
  Cancel a transaction ID for the CA retrieval request.

PARAMETERS
  void                            *ca_info_ptr
  ps_sys_subscription_enum_type   subscription_id
  int16                         * ps_errno

RETURN VALUE
  ret_val

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_sys_ca_cert_cancel
(
  uint8                                 ca_cert_dn[DS_SYS_CERT_DN_MAX],
  uint8                                 ca_cert_dn_len,
  ds_sys_ca_cert_cback_f_ptr_type       client_cb_ptr,
  ps_sys_subscription_enum_type         subscription_id,
  int16                                *ps_errno
)
{
  ps_sysi_ca_cert_compare_val_type     compare_val;
  uint16                               error_reason = 0;
  ps_sys_ca_cert_q_type              * ca_cert_q_node = NULL;
/*-------------------------------------------------------------*/
  /*-------------------------------------------------------------------------
    For Cancel transaction, need to
    1. traverse through the queue, find the matching cert_dn and cb_ptr node
    2. if found, delete the node
    3. if not found, return DS_ENOROUTE error
  -------------------------------------------------------------------------*/
  do 
  {
    /* 1. traverse through the queue, find the matching cert_dn and cb_ptr node */
    memset(&compare_val, 0, sizeof(ps_sysi_ca_cert_compare_val_type));
    memscpy(&compare_val.cert_dn, DS_SYS_CERT_DN_MAX,
            ca_cert_dn, ca_cert_dn_len);
    compare_val.cert_dn_length =  ca_cert_dn_len;
    compare_val.client_cb_ptr = client_cb_ptr;
    ca_cert_q_node = q_linear_search(&ps_sys_ca_cert_state.ca_cert_client_info,
                                      ps_sysi_compare_ca_info,
                                      &compare_val);
    
    /* 3. if not found, return DS_ENOROUTE error */
    if (NULL == ca_cert_q_node) 
    {
      error_reason = PS_SYS_CA_CERT_NO_MATCH_REQUEST;
      *ps_errno = DS_ENOROUTE;
      goto failure_result;
    }
  
    if (TRUE != q_delete_ext(&ps_sys_ca_cert_state.ca_cert_client_info,
                             &ca_cert_q_node->link)) 
    {
      error_reason = PS_SYS_CA_CERT_OPERATION_FAILED;
      PS_SYSTEM_HEAP_MEM_FREE(ca_cert_q_node);
      goto failure_result;
    }
    PS_SYSTEM_HEAP_MEM_FREE(ca_cert_q_node);
  } while (0);
  return;
failure_result:
  LOG_MSG_ERROR_1("ps_sys_ca_cert_cancel error reason %d",error_reason);
  return;
}

