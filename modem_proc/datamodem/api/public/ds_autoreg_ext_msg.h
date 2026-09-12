#ifndef DS_AUTOREG_EXT_MSG_H
#define DS_AUTOREG_EXT_MSG_H
/*===========================================================================
 
                         DS_AUTOREG_EXT_MSG.H

DESCRIPTION
  This file defines the DS auto registration related UMIDs and structures
 
Copyright (c) 2018 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/api/public/ds_autoreg_ext_msg.h#1 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/01/18   hr      Initial version
===========================================================================*/

/*===========================================================================
                         INCLUDE FILES FOR MODULE
===========================================================================*/
#include "msgr.h"
#include "comdef.h"
#include "msgr_ds.h"

/*===========================================================================
                         MACRO AND TYPE DEFINITIONS
===========================================================================*/
#define DS_AUTOREG_VOLTE_SWITCH_STATUS_IND_ID 0x11

typedef enum
{
  DS_AUTOREG_VOLTE_STATUS_MIN    = 0,
  DS_AUTOREG_VOLTE_SWITCH_ON     = 1,
  DS_AUTOREG_VOLTE_SWITCH_OFF    = 2,
  DS_AUTOREG_VOLTE_NOT_SUPPORTED = 3,
  DS_AUTOREG_VOLTE_STATUS_MAX    = 4
}ds_autoreg_volte_status_e_type;

typedef struct
{
  msgr_hdr_s                     msg_hdr;      /* Message router header */
  ds_autoreg_volte_status_e_type volte_status; /* VoLTE switch status */
}ds_autoreg_volte_switch_status_ind_s;

enum
{
  MSGR_DEFINE_UMID(DS, APPSRV, IND, AUTOREG_VOLTE_SWITCH_STATUS,
                   DS_AUTOREG_VOLTE_SWITCH_STATUS_IND_ID,
                   ds_autoreg_volte_switch_status_ind_s),
};

#endif /* DS_AUTOREG_EXT_MSG_H */
