/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            S I M L O C K   S W I T C H   S F S   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the simlock functions to switch usage of 
  local to remote SFS or vice-versa


EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS 


                        COPYRIGHT INFORMATION

Copyright (c) 2014, 2016 - 2017, 2019 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/simlock/src/simlock_switch_sfs.c#2 $$ $DateTime: 2019/10/30 06:56:05 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
09/10/19   vdc     SimLock solution for activation at Point Of Sale
07/24/17   nr      Reduce stack consumption
09/16/16   nr      Support remote and local configurations together
09/29/14   vv      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "customer.h"

#ifdef FEATURE_SIMLOCK

#include "simlock_switch_sfs.h"
#include "simlock.h"
#include "simlock_modem_p.h"
#include "simlock_util.h"
#include "simlock_file.h"
#include "simlock_config.h"


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*===========================================================================
FUNCTION SIMLOCK_SET_REMOTE_SFS_REQUEST

DESCRIPTION
  This function processes the request to enable or disable the remote SFS
  storage

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_set_remote_sfs_request
(
  const simlock_set_remote_sfs_msg_req_type * msg_request_ptr
)
{
  simlock_result_enum_type  simlock_status       = SIMLOCK_SUCCESS;
  simlock_sfs_enum_type     write_sfs            = SIMLOCK_SFS_LOCAL;
  simlock_config_data_type *config_file_data_ptr = NULL;

  if(msg_request_ptr == NULL)
  {
    return SIMLOCK_INCORRECT_PARAMS;
  }

  SIMLOCK_MSG_MED_1("simlock_set_remote_sfs_request, req_type: 0x%x",
                    msg_request_ptr->request_type);

  switch(msg_request_ptr->request_type)
  {
    case SIMLOCK_REMOTE_SFS_ENABLE:
      write_sfs = SIMLOCK_SFS_REMOTE;
      break;

    case SIMLOCK_REMOTE_SFS_DISABLE:
      write_sfs = SIMLOCK_SFS_LOCAL;
      break;

    default:
      return SIMLOCK_INCORRECT_PARAMS;
  }

  /* read config file data */
  config_file_data_ptr = simlock_config_read_config_data(SIMLOCK_CONFIG_ID_DEFAULT);
  if(config_file_data_ptr == NULL)
  {
    SIMLOCK_MSG_ERR_0("read config file failed");
    return SIMLOCK_GENERIC_ERROR;
  }

  if(((config_file_data_ptr->sfs_type == SIMLOCK_SFS_REMOTE) &&
      (msg_request_ptr->request_type == SIMLOCK_REMOTE_SFS_ENABLE))
                  ||
     ((config_file_data_ptr->sfs_type == SIMLOCK_SFS_LOCAL) &&
      (msg_request_ptr->request_type == SIMLOCK_REMOTE_SFS_DISABLE)))
  {
    SIMLOCK_MSG_ERR_1("remote sfs request invalid sfs_type: 0x%x", config_file_data_ptr->sfs_type);
    simlock_status = SIMLOCK_INVALID_STATE;
  }
  else
  {
    simlock_status = simlock_config_update_sfs_type(write_sfs, config_file_data_ptr);
    if(simlock_status == SIMLOCK_SUCCESS)
    {
      simlock_status = simlock_config_write_data(config_file_data_ptr, SIMLOCK_CONFIG_ID_DEFAULT);
    }
  }

  simlock_config_free_config_data(config_file_data_ptr);

  return simlock_status;
} /* simlock_set_remote_sfs_request */

#endif /* FEATURE_SIMLOCK */

