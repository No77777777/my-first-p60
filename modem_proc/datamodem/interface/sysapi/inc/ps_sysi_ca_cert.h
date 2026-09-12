/******************************************************************************
  @file    ps_sysi_ca_cert.h
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

  $Header: //components/rel/data.mpss/3.5..2.1/interface/sysapi/inc/ps_sysi_ca_cert.h#1 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $
 
when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/17/18   shannonl     Created functions to support CA certificate feature
===========================================================================*/

/*---------------------------------------------------------------------------
                           INCLUDE FILES
---------------------------------------------------------------------------*/
#include "ds_sys.h"
#include "ps_sys.h"

typedef enum
{
  PS_SYS_CA_CERT_ERR_INTERNAL      = 1,
  PS_SYS_CA_CERT_NO_MEMORY         = 2,
  PS_SYS_CA_CERT_INVALID_CERT_DN   = 3,
  PS_SYS_CA_CERT_OPERATION_FAILED  = 4,
  PS_SYS_CA_CERT_NO_MATCH_REQUEST  = 5,
  PS_SYS_CA_CERT_INVALID_TX_ID     = 6,
  PS_SYS_CA_CERT_INVALID_CB        = 7
}ps_sysi_ca_cert_error_enum_type;

typedef struct
{
  q_link_type                           link;
  uint32                                tx_id;
  uint8                                 cert_dn[DS_SYS_CERT_DN_MAX];
  uint8                                 cert_dn_length;
  ds_sys_ca_cert_cback_f_ptr_type       client_cb_ptr;
  void                                 *user_data_ptr;
}ps_sys_ca_cert_q_type;

typedef struct
{
  q_type                          ca_cert_client_info; /* q for the complete CA cert client info */
  uint32                          ca_cert_count;       /* translate to tx_id */
}ps_sys_ca_cert_client_state;

typedef struct
{
  uint8                                 cert_dn[DS_SYS_CERT_DN_MAX];
  uint8                                 cert_dn_length;
  ds_sys_ca_cert_cback_f_ptr_type       client_cb_ptr;
}ps_sysi_ca_cert_compare_val_type;

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
void ps_sysi_ca_cert_init();


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
);

/*===========================================================================
FUNCTION PS_SYS_MODIFY_CA_CERT_CLIENT()

DESCRIPTION
  This function modifies the CA certificate client info queue. It can add a 
  new client (node) or delete an existing client (node) by searching through
  the tx_id and cert_dn. Either tx_id or cert_dn has to be present.

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
);

/*===========================================================================
FUNCTION PS_SYS_MODIFY_CA_CERT_CLIENT()

DESCRIPTION
  Send CA certificate to client via client registered callback.\

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void ps_sys_send_ca_cert
(
  void                                  * send_ca_cert_ptr,
  ps_sys_subscription_enum_type           subscription_id,
  int16                                 * ps_errno
);

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
);

/*=======================================================================
FUNCTION PS_SYS_CA_CERT_CANCEL()

DESCRIPTION
  Process cancel request from CA certificate clients. cancel CA certificate
  retrieval requests 

PARAMETERS
  uint8                                 ca_cert_dn[DS_SYS_CERT_DN_MAX],
  uint8                                 ca_cert_dn_len,
  ds_sys_ca_cert_cback_f_ptr_type       client_cb_ptr,
  ps_sys_subscription_enum_type         subscription_id,
  int16                                *ps_errno

RETURN VALUE
  None

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
);



