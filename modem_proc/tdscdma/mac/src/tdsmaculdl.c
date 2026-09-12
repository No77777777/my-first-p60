/*============================================================================================

         U P L I N K    D O W N L I N K  D A T A   H A N D L E R


GENERAL DESCRIPTION

  This module contains the functions which are used both by the UL MAC and the DL MAC.

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


*====*====*====*====*====*====*====*====*====*====*====*====*====*===*===*===*===*===*===*===*/

/*==============================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/msm5200/mac/vcs/maculdl.c_v   1.0   11 Jun 2002 18:38:26   skrishna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmaculdl.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     -----------------------------------------------------------------------------
10/24/10   guo     Initial release Upmerged WCDMA to TDS

==============================================================================================*/


/*==============================================================================================
                     INCLUDE FILES
==============================================================================================*/
#include "tdscdma_variation.h"
#include "tdsmaculdl.h"
#include "tdsmacul.h"
#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#endif


/*==============================================================================================
 FUNCTION NAME  tdsl1_post_ul_mac_config_sig

 FUNCTION   This function is called by physical layer and it posts the config signal to l2 command
            queue.
===============================================================================================*/
void tdsl1_post_ul_mac_config_sig (void)
{
  /* Set UL MAC config signal to UL L2 task */
  (void)rex_set_sigs(tds_l2_ul_get_tcb(), TDSMAC_PHY_UL_CONFIG_IND);
}

/*==============================================================================================
 FUNCTION NAME  tdsl1_post_dl_mac_config_sig

 FUNCTION   This function is called by physical layer and it posts the config signal to l2 command
            queue.
===============================================================================================*/
void tdsl1_post_dl_mac_config_sig (void)
{
  /* Set DL MAC config signal to DL L2 task */
  (void)rex_set_sigs(tds_l2_dl_get_tcb(), TDSMAC_PHY_DL_CONFIG_IND);
}


/*==============================================================================================
 FUNCTION NAME  tdsl1_post_mac_hs_config_sig

 FUNCTION   This function is called by physical layer and it posts the HS config signal to l2 command
            queue.
===============================================================================================*/
void tdsl1_post_mac_hs_config_sig (void)
{
  /* Set DL MAC config signal to DL L2 task */
  (void)rex_set_sigs(tds_l2_dl_get_tcb(), TDSMAC_PHY_HS_CONFIG_IND);
}


/*==============================================================================================
 FUNCTION NAME  tdsl1_post_dl_mac_config_sig_at_l1_failure

 FUNCTION   This function is called by physical layer and it posts the config signal to l2 command
            queue when there is failure to process TDSL1_CPHY_SETUP_REQ and MAC UL or DL sync for MAC
            config is requested
===============================================================================================*/
void tdsl1_post_dl_mac_config_sig_at_l1_failure (void)
{
  /* Set MAC config signal to DL L2 task */
  (void)rex_set_sigs(tds_l2_dl_get_tcb(), TDSMAC_PHY_DL_CONFIG_AT_L1_FAILURE_IND);
}



/*==============================================================================================
 FUNCTION NAME  tdsmaci_post_l1_config_ind

 FUNCTION   This function posts to L1 the configuration sucess message. This function can be
            called by both the UL and DL MAC to post a configuration sucess message to L1.

===============================================================================================*/
void  tdsmaci_post_l1_config_ind
(
  tdsl1_cmd_enum_type cmd_id
)
{
  /* L1 External Command Buffer Pointer */
  tdsl1_ext_cmd_type   *cmd_ptr;

  /* Get External Command Buffer Pointer */
  cmd_ptr = tdsl1_get_cmd_buf();

  /* L1 External Command Buffer Pointer is NULL */
  if (cmd_ptr == NULL){
    TDSMAC_MSG0(MSG_LEGACY_HIGH,"CPHY_MAC_CONFIG_CNF command buffer allocation failed");
  }
  else {
    /* Populate Command Header fields */
    cmd_ptr->cmd_hdr.cmd_id = cmd_id;

    /* Populate command data fields */
    cmd_ptr->cmd.mac_config_cnf.mac_config_status = TRUE;

    /* Put the command in L1 External Command Queue */
    tdsl1_put_cmd (cmd_ptr);

    TDSMAC_MSG0(MSG_LEGACY_LOW,"CPHY_MAC_CONFIG_CNF sent");

  }
}



/*-------------------------------------------------------------------
FUNCTION tdsmac_rrc_get_hfn_info

DESCRIPTION
 Function interface which will provide RRC with the information of CS
 and PS HFN numbers during the time of RRC connection release
DEPENDENCIES
  None

RETURN VALUE
  Boolean ( TRUE indicates MAC was able to populate the cs_hfn and
  ps_hfn values.FALSE indicates the data was not populated.

SIDE EFFECTS
  None
--------------------------------------------------------------------*/
boolean
tdsmac_rrc_get_hfn_info(tdsmac_hfn_info_type *hfn_info)
{
  boolean rv = TRUE;

  TDSL2_ACQ_CIPHER_PARAM_LOCK();
  hfn_info->cs_hfn = tdscur_ul_cipher_info[0].hfn;
  hfn_info->ps_hfn = tdscur_ul_cipher_info[1].hfn;
  TDSL2_ACQ_CIPHER_PARAM_UNLOCK();
  return rv;
}
