/*====*====*===*====*====*====*====*====*====*====*====*====*====*====*====*

               S Y S T E M   D E T E R M I N A T I O N
               
               D E B U G   H E A D E R   F I L E


GENERAL DESCRIPTION
  The SD is responsible for logging the below critical imformation for debugging purposes .

EXTERNALIZED FUNCTIONS

  Command Interface:

  Others:

REGIONAL FUNCTIONS
  None.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  The SD task should be started before these APIs are called.


Copyright (c) 2002 - 2013 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/sd/src/sddbg_qsh.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/21/17   KC      Initial release.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "sd_qsh_ext.h"
#include "sddbg_qsh.h"
#include "sdcmd.h"

#ifdef FEATURE_QSH_MDUMP

extern void sd_qsh_mdump_collect();
extern void prl_qsh_mdump_collect();

/*===========================================================================

FUNCTION sddbg_qsh_cb

DESCRIPTION
  Processes the QSH request .
  Currently we handle only QSH_ACTION_MDUMP .
DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

Note
use SD_API_KEEP_UNCOMPRESSED_IN_ELF ,
which keeps the API in uncompressed section in th elf
 this will be used by QSH dump collection ,
since after the crash happened uncompressed functions
compressed functions wont be accesible
===========================================================================*/
#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR
#endif
void  sddbg_qsh_cb(qsh_client_cb_params_s *cb_params_ptr)
{

  if (cb_params_ptr == NULL)
  {
    return;
  }

  #ifdef FEATURE_QSH_MDUMP
  if(cb_params_ptr->action & QSH_ACTION_MDUMP)
  {
    sddbg_qsh_mdump_collect();
  }
  #endif  //FEATURE_QSH_MDUMP  
  
}

/*===========================================================================

FUNCTION sddbg_qsh_init

DESCRIPTION
  Initializes SD QSH interface.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void  sddbg_qsh_init()
{
  qsh_client_reg_s qsh_client;

  qsh_client_reg_init(&qsh_client);

  qsh_client.client = QSH_CLT_SD;
  qsh_client.major_ver = SD_QSH_MAJOR_VER;
  qsh_client.minor_ver = SD_QSH_MINOR_VER;
  qsh_client.client_cb_ptr = sddbg_qsh_cb;
  qsh_client.cb_action_support_mask = QSH_ACTION_MDUMP;

  qsh_client_reg(&qsh_client);
  
#ifdef FEATURE_QSH_MDUMP
  qsh_add_thread_id(qsh_client.client);
#endif
}

/*===========================================================================
FUNCTION sddbg_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for SD module.

DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR
#endif
void sddbg_qsh_mdump_collect()
{
  sdcmd_qsh_mdump_collect(); 
  sdss_qsh_mdump_collect();
  sdsr_qsh_mdump_collect();
  sdprl_qsh_mdump_collect();
  sd_qsh_mdump_collect();
  prl_qsh_mdump_collect();
}


#endif
