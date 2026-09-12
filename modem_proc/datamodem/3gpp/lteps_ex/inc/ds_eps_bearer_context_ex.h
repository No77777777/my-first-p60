
/*!
  @file
  ds_eps_bearer_context_ex.h

  @brief
  Lower layer/Bearer Context interface

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2008 - 2014 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/lteps_ex/inc/ds_eps_bearer_context_ex.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/27/18   vj      Added function to return so mask based on the number of 
                   configured scells and mimo/modulation information
===========================================================================*/

#ifndef DSEPSBEARERCONTEXTEX_H
#define DSEPSBEARERCONTEXTEX_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "datamodem_variation.h"
#ifdef FEATURE_DATA_LTE

#include "ds_3gpp_bearer_context.h"

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/
/*------------------------------------------------------------------------
  Define the max index for bit rate
------------------------------------------------------------------------*/
#define DS_EPS_BIT_RATE_INDEX_MAX   63
/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_EX_GET_SO_MASK

DESCRIPTION
  This function returns the SO MASK corresponding to the LTE carrier 
  aggregation.and mimo/modulation information

PARAMETERS
  ds3g_subs_id: Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  lte so mask 

SIDE EFFECTS
  None.
===========================================================================*/
uint64 ds_eps_bearer_cntxt_ex_get_so_mask
(
  ds3gsubsmgr_subs_id_e_type  ds3g_subs_id
);

/*===========================================================================
FUNCTION  DS_EPS_BEARER_CNTXT_REC_BIT_RATE_IND_HDLR

DESCRIPTION
  This function is used to handle the message from MAC. The msg is the recommended
  bit rate for the bearer and would be passed to IMS.
  
PARAMETERS
  msgrtype  - UMID of the message sent
  subs_id   - subscription ID
  dsmsg_ptr - Pointer to the payload

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_eps_bearer_cntxt_ex_rec_bit_rate_ind_hdlr
(
  msgr_umid_type             msgrtype,
  sys_modem_as_id_e_type     subs_id,
  const msgr_hdr_struct_type *dsmsg_ptr
);

/*===========================================================================
FUNCTION DS_EPS_BEARER_CNTXT_EX_RAN_ASST_MEDIA_ADAPT_UPDATE

DESCRIPTION
  This function process the query bit rate info from PS. It would send the
  query bit rate to MAC if the configuration is supported

PARAMETERS
  phys_link_ptr_i   physical link pointer
  argval_ptr        the payload of query bit rate ioctl
  subs_id:          Subscription Id
  ps_errno          error number

DEPENDENCIES
  None.

RETURN VALUE
  lte so mask 

SIDE EFFECTS
  None.
===========================================================================*/
int ds_eps_bearer_cntxt_ex_ran_asst_media_adapt_update
(
  ps_phys_link_type        *phys_link_ptr_i,
  void                     *argval_ptr,
  sys_modem_as_id_e_type    subs_id,
  int16                    *ps_errno
);

#endif /* FEATURE_DATA_LTE */
#endif /* DSEPSBEARERCONTEXTEX_H */

