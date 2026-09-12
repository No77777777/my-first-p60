/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

C A L L   M A N A G E R


S U P P L E M E N T A R Y   S E R V I C E   M O D U L E

GENERAL DESCRIPTION
This module contains the Call Manager Sups Object functions for WCDMA/GSM.

The Sups Object is responsible for:
1. Processing clients' call independent sups commands.
2. Processing Lower Layer replies for clients' sups commands.
3. Processing Lower Layer call independent sups related notifications.
4. Notifying the client list of sups events.


EXTERNALIZED FUNCTIONS

cmsups_init
Initializing the sups object.

cmsups_client_cmd_proc
Process call independent sups commands from the clients.

cmsups_rpt_proc
Process Lower Layer sups notifications/command replies.


INITIALIZATION AND SEQUENCING REQUIREMENTS
cmsups_init() must be called to initialize this module before any other
function declared in this module is being called.


Copyright (c) 2001 - 2015 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmwsups.c#4 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/10/13   ar      checking value of sups_info_ptr
07/03/13   dk      KW error fixed in cmwsups_cmd_copy_fields_1
06/24/13   dk      KW error fix in cmwsups_cmd_copy_fields_1()
05/24/13   ss      Sups LCS_MOLR memory fix
04/23/13   xs      USSD detect support
01/07/13   qf      FR 2564 - World mode support for DSDA
11/22/12   xs      Added PS supplemetary service support
10/30/12   xs      PS supplementary service support
05/04/12   anm     Fix to Return error for sups command with GW no subs
02/15/12   vk      Replace usage of AEE library function with corresponding
CoreBSP library
04/29/11   anm    Assigning proper AS ID to USSD phase1 req
04/21/11   sv      Correct Lint Errors
04/11/11   am      Cleaning stubs and runnning qtf SUTE
01/25/11   rm      Updating correct sups params with FEATURE_MODEM_HEAP_PHASE_2
12/23/10   sv      Integrate DSDS feature
12/17/2010 gm      Changed user_data parameter to const type in CC callback.
11/15/10   rm      Memory optimization changes
11/02/10   am      Replacing FEATURE_MOB_MMODE_CUTE
06/10/10   ak      Mainlining memory optimization changes.
06/02/10   rn      send default cc_generic_params
03/18/10   am      Fixing Compilation Warning
10/14/09   sg      ZI memory reduction. Mmode should call new Heap Manager API to
allocate memory dynamically.
08/26/09   rm      When in limited service, do not return srv_status_error
for 'Location notofication' sups command
08/07/09   rm      CMI phase-2: Removing featurisation from /api files
Removed including cause_v.h
07/26/09   rn      Centralised call control changes
05/26/09   mp      Updated SUPS event notification in SUSPEND state
04/06/09   mj      Update BOOLEAN macro w/ namespace prefix to CM_BOOLEAN
03/24/09   sv      updated to cmtaski.h
03/23/09   rm      Adding _v.h files from NAS
02/17/09   ks      Copying L3 data to sups_info ptr for more sups cnf
12/17/08   jd      Fixed Lint errors
09/05/08   rn      FDN checking option in call control
08/07/07   ic      Lint clean up
05/16/07   cl      Fix typo in include file AEEstd.h
05/02/07   rp      Adding FEATURE_CM_LCS
04/15/07   rp      Removing FEATURE_MMGPS
02/23/07   pk      Banned API lint fixes
02/08/07   rp      Copying L3 data to sups_info ptr for interrogate cnf.
01/19/07   pk      Reverting previous lint fixes
01/12/07   pk      Lint fixes
12/19/06   ka      Adding support to process UCS2 in CM_SUPS_CMD_PROCESS_USS.
11/09/06   pk      Added support for HYBR_GW and UMTS -> 1X handover cont...
09/29/06   rp/ka   Added code to initialize all the fields in sups object.
09/13/06   rp      Added code to copy Layer 3 message from MN to sups object
and sups client object and featurized under
FEATURE_SS_L3_DATA
08/21/08   ka      Fixing cmsups_ussd_phase1_req() by adding a call to
cm_mn_send_cmd().
07/04/06   ka      Changes to support call control on PS calls.
01/03/06   ic      Fixed compile error in cmsups_ussd_phase1_req()
<argument 2 to 'strncpy'>: implicit cast of pointer to
non-equal pointer
Fixed compile error in cmsups_ussd_phase1_req()
<argument 1 to 'cm_util_gsm_alphabet_to_ascii'>: implicit
cast of pointer to non-equal pointer
Correcting fields being sent for ussd phase 1 request.
01/12/06   ic      For specific calls / walks Lint warnings, replaced global
-fsp comment with -e{error#} comments where needed
01/06/06   ic      Lint cleanup
12/13/05   ic      Fixed compiler warnings
11/08/05   sk      Separated CM reports and commands.
11/03/05   ic      Added CM_ASSERT() for ptr function parameters and lint
comments for 613 Lint error
08/02/05   ic      Print value that caused error in default switch case
07/25/05   sk      Replaced (for fixing CR 67893)
cm_util_ascii_to_def_alphabet() with
cm_util_ascii_to_gsm_alphabet().
06/24/05   ic      Adding CCBS feature (merged in P4 change list 204600)
04/06/05   ka      Change #define name
02/09/05   ic      Lint cleanup
01/21/05   ka      Changing encoding scheme for CM_USSD_DCS_UNSPECIFIED to
be the same as 7bit encoding.
01/12/05   ka      Support for 7bit/8bit/unspec USSD data coding scheme.
11/15/04   ic      Lint cleanup with CM_DEBUG on
09/13/04   ka      Handling CM_CC_RESULT_NOT_ALLOWED in sups call control
call back.
08/26/04   sbs     Added externalAddress and nameString IEs to MOLR_Invoke /
LocationNotification SS messages
08/17/04   ic      Undoing P4 change list #121638
08/04/04   ic      CM Lint clean up - cmwsups.c with Lint supression verified
and free of Lint issues in all CUTE configurations.
07/27/04   ws      Add Call Control support for SUPS.
07/26/04   ic      Changed include file order to customer.h, then cm.h then
cmi.h
Added cmwsups_dummy() to get rid of ADS 1.2 compiler warning
07/07/04   ka      Fixed Lint errors.
05/27/04   ic      In cmsups_client_cmd_proc() call cmsups_client_cmd_err()
irrespective of whether error occured or not (client is
supposed to be always notified about command processing
status)
05/14/04   ka      Fixed Lint errors.
03/16/04   ka      Merged changes from 6250. Fixed Lint errors.
02/25/04   ic      Changes to make the code pass unit test cases and at the
same time make calls in Saber build
02/06/04   ws      Initial jaguar/mainline merge.
01/13/04   ka      Merged changes from MSM6200
12/19/03   RI      Added setting of USS data present field when a Phase 1
response is sent to clients in a Phase 2 structure.
11/19/03   sbs     Added LCS MOLR Completed API function
07/08/03   sbs/lt  Added support for Location Services related SS messages
06/20/03   prk     Featured out copy_fwd_to_number() to remove compiler warning.
Currently not used but may be used in the future.
06/04/03   RI      Modified the processing of forwarded to number information
in the SS confirmations.
04/28/03   AT      Lint cleanup.
04/11/03   prk     Removed references to FEATURE_SUPS.
03/13/03   RI      Updated the processing of USS indication in
cmsups_rpt_proc().
01/21/03   ws      Updated copyright information for 2003
11/14/02   vt      Merged changes from GW mainline.
[RI]Updated the processing of USSD confirmations.
05/17/02   PRK     Removed comment.
05/16/02   RI      Corrected USS problems.
05/07/02   PRK     Renamed to cmwsups.c.
04/25/02   RI      Added processing of CM_PROCESS_UNSTRUCTURED_SS_DATA_CONF.
03/11/02   RI      Modifications as per new interface definitions for
call independent ss messages.
- confirmation messages are processed based on boolean
flag 'success'
02/04/02   RI      Removed mn_cm_exp_t.h inlcude file.
- removed the IEI processing in the messages received
from the lower layer (MN) and setting of IEI fields
in the messages sent to the lower layer (MN).
01/14/02   PRK     Checked service status flag instead of service state flag
in function cmsups_client_cmd_check.
Replaced prefix cmutil_ with cm_util_.
11/28/01   RI      Commented out the password fields in activate and deactivate
SS requests.
11/21/01   RI      Fixed ADS compiler errors.
11/05/01   RI      Modified handling of IEI fields in SUPS commands
and events.
Added copy_ss_info_fields, copy_ss_error,
copy_bsg_list, copy_ff_list, copy_fwd_to_number.
10/18/01   PRK     Added changes for WCDMA/GSM mode.
Made modification due to srv_state, moving from
phone group to serving system group (FEATURE_NEWSS).
09/25/01   RI      Initial release for WCDMA/GSM.

===========================================================================*/

/*===========================================================================

INCLUDE FILES FOR MODULE

===========================================================================*/
/*lint -esym(766,customer.h) */
#include "mmcp_variation.h"
#include "customer.h"
#include "cmsups.h"

#ifdef CM_GW_SUPPORTED
#include "comdef.h"
#include <string.h>

#include <stringl/stringl.h>

#include "cmdbg.h"
#include "cmclient.h"
#include "cmph.h"
#include "cmmsc.h"
#include "cmutil.h"
#include "cmss.h"
#include "cmtaski.h"
#include "cmcall.h"
#include "mn_cnmtask.h"
#include "mn_cnmtask_v.h"
#include "cause.h"

#ifdef CM_DEBUG
#error code not present
#endif
#include "cmipappi.h"
#include "cmregprxi.h"
#include "cmregprx.h"

#include "cmefs.h"
#include <stringl/stringl.h>

#include "cmsups.h"
#include "cmpmprx.h"
#include "cmwsups.h"
#include "qmi_v.h"
#include "cmsds.h"
/*===========================================================================

LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

static boolean uss_phase1_msg = FALSE;

#define CFIS_RECORD_SIZE  16

#ifdef FEATURE_SUPS_RETRY
static void cmwsups_list_clear(
  sys_modem_as_id_e_type   asubs_id
);
#endif

/*===========================================================================

FUNCTION cmsups_nv_refresh

DESCRIPTION
Reading NV items


DEPENDENCIES
none

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
void cmsups_nv_refresh(
  uint8   refresh_subs_mask
  /* Subs mask whose NVs need to be refreshed */
)
{
  cmsups_s_type    *sups_ptr = cmsups_ptr();
  /* get the pointer to the sups object to be initialized */
  int32             read_size = 0;
  uint8             i = 0;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*                       READ NV ITEMS                                   */
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Read the supplementary service domain pref
  */
  read_size = sizeof (sys_supp_srv_domain_pref_e_type);

  for (i = 0; i < sups_ptr->sub_info_count; i++)
  {
    if (refresh_subs_mask & BM(i))
    {
      if (sups_ptr->sub_info[i] == NULL)
      {
        CM_ERR_1("cmsups_nv_refresh: sups_ptr->sub_info[i] NULL ptr for asubs_id %d", i);
      }
      else
      {
        if (cmefs_get(CMEFS_SUPP_SRV_DOMAIN_PREF,
                      (byte *)&sups_ptr->sub_info[i]->sups_svc_domain_pref,
                      read_size,
                      (sys_modem_as_id_e_type)i))
        {
          CM_MSG_MED_2("cmipsups: sub %d supplement_service_domain_pref= %d",
                       i, sups_ptr->sub_info[i]->sups_svc_domain_pref);
        }
        else
        {
          /* set default value to AUTO */
          sups_ptr->sub_info[i]->sups_svc_domain_pref = SYS_SUPP_SRV_DOMAIN_PREF_AUTO;
          CM_MSG_MED_1("cmipsups: sub %d set default PS sups srv domain pref AUTO", i);
        }
      }
      /* Read the reject Sups command interrogate OIR/ CLIR EFS settings
      */
      if (cmefs_get(CMEFS_REJECT_OIR_CLIR_INTERROGATION,
                    (byte *)&sups_ptr->sub_info[i]->sups_reject_oir_clir_interrogation,
                    sizeof(cmsups_is_reject_oir_clir_e_type),
                    (sys_modem_as_id_e_type)i)
         )
      {
        CM_MSG_HIGH_2("cmipsups: Sub %d reject_oir_clir_interrogation = %d",
                      i, sups_ptr->sub_info[i]->sups_reject_oir_clir_interrogation);
      }
      else /* NV not set */
      {
        sups_ptr->sub_info[i]->sups_reject_oir_clir_interrogation = CMSUPS_SS_REJECT_NONE;
        CM_MSG_HIGH_1("cmipsups: Sub %d read failed, Using Default CMSUPS_SS_REJECT_NONE", i);
      }

      if (cmefs_get(CMEFS_CW_EXT_CONFIG_INFO,
                    (byte *)&sups_ptr->sub_info[i]->ext_cw_config_info,
                    sizeof(cm_sups_cw_config_info_s_type),
                    (sys_modem_as_id_e_type)i)
         )
      {
        CM_MSG_HIGH_3("cmipsups: Sub %d cw_config_info cw_mode = %d cw_sups_enable_opr_mask = %d",
                      i, 
                      sups_ptr->sub_info[i]->ext_cw_config_info.cw_mode,
                      sups_ptr->sub_info[i]->ext_cw_config_info.cw_sups_enable_opr_mask);
        
        cmwsups_set_prev_ue_based_sups(cw,sups_ptr->sub_info[i]->ext_cw_config_info.cw_nw_status,i, FALSE);
      }
      else /* NV not set */
      {
        sups_ptr->sub_info[i]->ext_cw_config_info.cw_mode = CM_SUPS_PERMANENT_MODE;
        sups_ptr->sub_info[i]->ext_cw_config_info.cw_sups_enable_opr_mask = SUPS_ENABLE_OPR_MASK_NONE;
        CM_MSG_HIGH_1("cmipsups: Sub %d CW_EXT_CONFIG_INFO read failed", i);
      }

      memset(&sups_ptr->sub_info[i]->sups_enable_status_nw, 0, sizeof(cm_sups_enable_status_nw_s_type));
      if (cmefs_get(CMEFS_SUPS_ENABLE_STATUS_NW,
                    (byte *)&sups_ptr->sub_info[i]->sups_enable_status_nw,
                    sizeof(cm_sups_enable_status_nw_s_type),
                    (sys_modem_as_id_e_type)i)
         )
      {
        memscpy(&sups_ptr->sub_info[i]->prev_ue_based_sups_enabled,
               (int32)(sizeof(boolean) * CMEFS_UE_BASED_SUPS_ARRAY_USED),
               &sups_ptr->sub_info[i]->sups_enable_status_nw.sups_enable_status_nw,
               (int32)(sizeof(boolean) * CMEFS_UE_BASED_SUPS_ARRAY_USED));
        
        CM_MSG_HIGH_1("Sub %d read CMEFS_SUPS_ENABLE_STATUS_NW success ",i);
      }
      else /* NV not set */
      {
        CM_MSG_HIGH_1("Sub %d read CMEFS_SUPS_ENABLE_STATUS_NW failed ",i);
      }

     /*Update UE BASED CW NV with value stored prior to MCFG refresh if iccid is same.*/
      cmwsups_check_and_update_cw_status(i);

    }
  }
}

/*===========================================================================

FUNCTION cmsups_free_sub_info

DESCRIPTION
This functions frees cmsups sub info

DEPENDENCIES
none

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
void cmsups_free_sub_info(
  sys_modem_as_id_e_type asubs_id
)
{
  cmsups_s_type              *sups_ptr = cmsups_ptr();
  cmwsups_info_list_s_type   *node;
  cmwsups_info_list_s_type   *next;

  if (!(asubs_id >= SYS_MODEM_AS_ID_1 &&
        asubs_id < ARR_SIZE(sups_ptr->sub_info) &&
        asubs_id < SYS_MODEM_AS_ID_NO_CHANGE))
  {
    CM_MSG_HIGH_1("free cmph memory wrong sub id %d", asubs_id);
    asubs_id = SYS_MODEM_AS_ID_1;
  }

  if (sups_ptr->sub_info[asubs_id] == NULL)
  {
    return;
  }
  /* free entire sups_info_list */
  node = sups_ptr->sub_info[asubs_id]->sups_info_list_head;
  while (node)
  {
    next = node->next;
    cm_mem_free(node);
    node = next;
  }
  sups_ptr->sub_info[asubs_id]->sups_info_list = NULL;
  sups_ptr->sub_info[asubs_id]->sups_info_list_head = NULL;

  /* free sups_info */
  if (sups_ptr->sub_info[asubs_id]->sups_info != NULL)
  {
    cm_mem_free(sups_ptr->sub_info[asubs_id]->sups_info);
    sups_ptr->sub_info[asubs_id]->sups_info = NULL;
  }

  /* free uss_info */
  if (sups_ptr->sub_info[asubs_id]->uss_info != NULL)
  {
    cm_mem_free(sups_ptr->sub_info[asubs_id]->uss_info);
    sups_ptr->sub_info[asubs_id]->uss_info = NULL;
  }

  /* free sub_info */
  cm_mem_free(sups_ptr->sub_info[asubs_id]);
  sups_ptr->sub_info[asubs_id] = NULL;

}

/*===========================================================================

FUNCTION cmsups_init

DESCRIPTION
Initializing the sups object.

This function must be called before the sups object
is being used.

DEPENDENCIES
none

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
void cmsups_init(
  uint8             refresh_subs_mask,
  /* Subs mask whose NVs need to be refreshed
  */
  boolean   is_pwr_on
  /* Indicates if the function is being called during pwr on
  */
)
{
  cmsups_s_type    *sups_ptr = cmsups_ptr();
  uint8             i = 0;
  uint8 num_sims = MIN(cmpmprx_get_num_of_sims(), ARR_SIZE(sups_ptr->sub_info));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_ASSERT(sups_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Verify that the object was not already initialized.
  ** and then mark object as initialized.
  */
  if (is_pwr_on)
  {
    memset(sups_ptr, 0, sizeof(cmsups_s_type));
    CM_ASSERT(!CM_INIT_CHECK(sups_ptr->init_mask));
    CM_INIT_MARK(sups_ptr->init_mask);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* deallocate sub level data if needed */
  for (i = num_sims; i < sups_ptr->sub_info_count; i++)
  {
    if (sups_ptr->sub_info[i] != NULL)
    {
      CM_MSG_HIGH_1("cmsups_init: DEALLOC cmsups sub info for asubs_id %d", i);
      cmsups_free_sub_info((sys_modem_as_id_e_type)i);
    }
  }

  /* allocate sub level data if needed */
  for (i = 0; i < num_sims; i++)
  {
    if (sups_ptr->sub_info[i] == NULL)
    {
      CM_MSG_HIGH_1("cmsups_init: ALLOC cmsups sub info for asubs_id %d", i);
      sups_ptr->sub_info[i] = (cmsups_sub_info_s_type *)cm_mem_malloc(sizeof(cmsups_sub_info_s_type));

      if (sups_ptr->sub_info[i] != NULL)
      {
        memset(sups_ptr->sub_info[i], 0, sizeof(cmsups_sub_info_s_type));
      }
      else
      {
        sys_err_fatal_null_ptr_exception();
      }

      /* Initialize all the fields */
      sups_ptr->sub_info[i]->sups_client_ptr = CM_CLIENT_PTR_NONE;

      sups_ptr->sub_info[i]->num_pending_sups_cmds = 0;

      sups_ptr->sub_info[i]->ccbs_erase_pend_indx.is_erase_cc_conf_pend = FALSE;

      sups_ptr->sub_info[i]->sups_info = NULL;

      sups_ptr->sub_info[i]->uss_info = NULL;

      sups_ptr->sub_info[i]->mt_uss_info.is_sups_cmd_mt_ussd_active = FALSE;

#ifdef FEATURE_SUPS_RETRY
      sups_ptr->sub_info[i]->sups_info_list = NULL;
      sups_ptr->sub_info[i]->sups_info_list_head = NULL;
#endif

      sups_ptr->sub_info[i]->buffered_mn_cmd_ptr = NULL;
      sups_ptr->sub_info[i]->buffered_mn_cmd_timer = 0;
      sups_ptr->sub_info[i]->prev_ue_based_cw = FALSE;
      memset(&sups_ptr->sub_info[i]->prev_ue_based_sups_enabled,0,sizeof(boolean)*(CMEFS_UE_BASED_SUPS_ARRAY_USED+1));
      memset(&sups_ptr->sub_info[i]->sups_enable_status_nw,0,sizeof(cm_sups_enable_status_nw_s_type));
      memset(&sups_ptr->sub_info[i]->ext_cw_config_info,0,sizeof(cm_sups_cw_config_info_s_type));
    }
  }

  /* Update count */
  sups_ptr->sub_info_count = num_sims;

  /* Read EFS*/
  cmsups_nv_refresh(refresh_subs_mask);
}

/*===========================================================================

FUNCTION  cmsups_msim_resume_data

DESCRIPTION
Resume data up processing sups reports.

DEPENDENCIES
none

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
void cmsups_msim_resume_data(
  sys_modem_as_id_e_type   asubs_id
)
{
  uint8 i;
  cmss_misc_sub_prop_s_type *ss_misc_sub_ptr;
  mm_sub_stk_id_s_type  mm_id;

  /* When all the voice/emerg/sms calls have ended on this sub, if there is an existing suspended data
  ** on the other sub that is CM initiated, update the suspend state.
  */
  if(cmph_is_msim() &&
     cmcall_is_there_a_call_type_per_sub(asubs_id, CM_CALL_TYPE_VOICE, NULL) == CM_CALL_ID_INVALID &&
     cmcall_is_there_a_call_type_per_sub(asubs_id, CM_CALL_TYPE_SMS, NULL) == CM_CALL_ID_INVALID &&
     cmcall_is_there_a_call_type_per_sub(asubs_id, CM_CALL_TYPE_EMERGENCY, NULL) == CM_CALL_ID_INVALID &&
     cmcall_is_there_a_call_type_per_sub(asubs_id, CM_CALL_TYPE_VT, NULL) == CM_CALL_ID_INVALID &&
     cmcall_is_there_a_call_type_per_sub(asubs_id, CM_CALL_TYPE_VS, NULL) == CM_CALL_ID_INVALID)
  {
    for (i = 0; i<cmss_ptr()->count; i++)
    {
      ss_misc_sub_ptr = cmss_get_misc_sub_prop_ptr((sys_modem_as_id_e_type)i);
      if (ss_misc_sub_ptr == NULL)
      {
        continue;
      }
      if (cmph_is_msim() &&
          ss_misc_sub_ptr->sub_info.gen_sub_info.ue_init_ps_data_suspend == TRUE &&
          asubs_id != (sys_modem_as_id_e_type)i)
      {
        mm_id.asubs_id = (sys_modem_as_id_e_type)i;
        mm_id.stk_id = cmph_determine_stk_per_rat(mm_id.asubs_id, SYS_SYS_MODE_LTE);
        cmss_process_data_suspend_rpt(sd_is_data_suspend_ss(mm_id), mm_id);
        ss_misc_sub_ptr->sub_info.gen_sub_info.ue_init_ps_data_suspend = FALSE;
      }
    }
  }
}

/*===========================================================================

FUNCTION cmsups_update_asubs_id

DESCRIPTION
Update asubs_id of sups information

DEPENDENCIES
none

RETURN VALUE
a pointer to the sups information

SIDE EFFECTS
none

===========================================================================*/
void cmsups_update_asubs_id(
  sys_modem_as_id_e_type     as_id,
  cm_sups_info_s_type      **sups_info_pptr
)
{
  if (sups_info_pptr == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return;
  }

  *sups_info_pptr = cmsups_ensure_sups_info_ptr_asubs_id(as_id);
  if (*sups_info_pptr == NULL)
  {
    CM_ERR_1("cmsups_update_asubs_id: sups_info_ptr NULL ptr for asubs_id %d",
             as_id);
    return;
  }

  if (cmph_is_msim())
  {
    (**sups_info_pptr).asubs_id = as_id;
  }
  else
  {
    (**sups_info_pptr).asubs_id = SYS_MODEM_AS_ID_1;
  }
}

/*===========================================================================

FUNCTION  cmsups_check_if_cmd_is_MT_response

DESCRIPTION
Checks if the input command is one of the following values indicating that it
is a response to an MT initiated transaction :
1.  CM_SUPS_CMD_GET_PASSWORD_RES
2.  CM_SUPS_CMD_USS_NOTIFY_RES
3.  CM_SUPS_CMD_USS_RES
4.  CM_SUPS_CMD_LCS_LOCATION_NOTIFICATION_RES

If yes, return TRUE. Else False.

DEPENDENCIES
NONE

RETURN VALUE
TRUE, if input command is one of the above MT initiated transactions.
False, if not.

SIDE EFFECTS
None

===========================================================================*/
static boolean cmsups_check_if_cmd_is_MT_response(cm_sups_cmd_e_type  command)
{

  switch(command)
  {
    case CM_SUPS_CMD_GET_PASSWORD_RES:
    case CM_SUPS_CMD_USS_NOTIFY_RES:
    case CM_SUPS_CMD_USS_RES:
    case CM_SUPS_CMD_LCS_LOCATION_NOTIFICATION_RES:
      return TRUE;
    default:
      return FALSE;
  }
}/* cmsups_check_if_cmd_is_MT_response */

/*===========================================================================

FUNCTION  cmsups_update_cfu_indicator_for_bs_code

DESCRIPTION
Update EF-CFIS for CFWD cases when Thin-ui is enabled.
This is in accordance with 3GPP TS 31.102, section 4.2.64 - EF CFIS

DEPENDENCIES
SUPS info should have the appropriate information received from network

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
#ifdef FEATURE_CM_UPDATE_EF_CFIS
static void cmsups_update_cfu_indicator_for_bs_code(
  byte           *indicator,
  byte            bs_code,
  bsg_code_type   bs_type,
  boolean         is_active
)
{
  uint8 mask = 0;

  CM_ASSERT(indicator != NULL);

  if (bs_type == BS_CODE)
  {
    mask = (bs_code == allBearerServices) ? 0x10 : 0;
  }
  else
  {
    switch (bs_code)
    {
      case allSpeechTransmissionservices:
      case telephony:
        mask = 0x01;
        break;

      case allFacsimileTransmissionServices:
      case facsimileGroup3AndAlterSpeech:
      case automaticFacsimileGroup3:
      case facsimileGroup4:
        mask = 0x02;
        break;

      case allDataTeleservices:
        mask = 0x04;
        break;

      case allShortMessageServices:
      case shortMessageMT_PP:
      case shortMessageMO_PP:
        mask = 0x08;
        break;

      case allTeleservices:
        mask = 0x0F;
        break;

      case allTeleservices_ExeptSMS:
        mask = 0x07;
        break;

      default:
        break;
    }
  }

  *indicator = is_active ? (*indicator | mask) : (*indicator & (~mask));

  CM_MSG_HIGH_6("CFU initial bs_type=%d, Indicator=%d, mask=%d, CFU isactive=%d, Indicator=%d for bs_code=%d",
                bs_type, *indicator, mask, is_active, *indicator, bs_code);

}
#endif

/*===========================================================================

FUNCTION cmsups_check_and_update_cfis

DESCRIPTION
Update EF-CFIS for CFWD cases when Thin-ui is enabled.
This is in accordance with 3GPP TS 31.102, section 4.2.64 - EF CFIS

DEPENDENCIES
SUPS info should have the appropriate information received from network

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
#ifdef FEATURE_CM_UPDATE_EF_CFIS
static void cmsups_check_and_update_cfis(
  cm_sups_info_s_type           *sups_info_ptr,
  cm_sups_event_e_type           sups_evt
)
{
  boolean                         num_updated = FALSE, is_active = FALSE;
  mmgsdi_session_type_enum_type   session_type = MMGSDI_GW_PROV_PRI_SESSION;
  mmgsdi_file_enum_type           file_name = MMGSDI_USIM_CFIS;
  uint8                           rec_data[CFIS_RECORD_SIZE], i = 0;
  byte                            bs_code = telephony;
  bsg_code_type                   bs_type = TS_CODE;
  sys_modem_as_id_e_type          asubs_id = SYS_MODEM_AS_ID_1;
  mmgsdi_write_data_type          write_data;
  ie_forwarding_feature_list_T   *fwd_info = NULL;
  basic_service_group_list_T     *bsg_list;
  boolean                         is_EF_write_req = TRUE, is_query_or_register = FALSE;
  boolean                         is_cfu_ind_updated = TRUE;

  CM_ASSERT(sups_info_ptr != NULL);

  if (cmph_is_msim())
  {
    asubs_id = sups_info_ptr->asubs_id;
  }

  /* Check if this is one of CFU, All Call forwarding */
  if (!((sups_info_ptr->ss_code == (uint8)cfu) ||
        (sups_info_ptr->ss_code == (uint8)allForwardingSS)))
  {
    CM_MSG_HIGH_1("SS Code: %d. No need to write to EF", sups_info_ptr->ss_code);
    return;
  }

  /* Proceed further only if this has a useful conf type */
  if ((sups_info_ptr->conf_type == CC_CAUSE_INFO) ||
      (sups_info_ptr->conf_type == SS_ERROR_INFO))
  {
    /* Error conf_types. No need to write data to EF */
    CM_MSG_HIGH_1("Conf Type: %d. No need to write to EF", sups_info_ptr->conf_type);
    return;
  }

  /* Retrieve the session type */
  if (cm_mmgsdi_ssn_tbl_get_type_for_asubs_id_gw(asubs_id, &session_type) != CM_MMGSDI_SUCCESS)
  {
    CM_MSG_HIGH_1("Session Entry not found for asubs_id %d", asubs_id);
  }

  /* Set the bs_code */
  if (sups_info_ptr->basic_service.present)
  {
    if (sups_info_ptr->basic_service.code_type == TS_CODE)
    {
      bs_code = sups_info_ptr->basic_service.bs_code;
    }
    else
    {
      bs_code = allBearerServices;
      bs_type = BS_CODE;
    }
  }

  /* Set the default active flag based on the sups event */
  is_active = (sups_evt == CM_SUPS_EVENT_ACTIVATE_CONF) ? TRUE : FALSE;

  /* Check if this is a query or register request which normally doesn't modify the CFU status */
  is_query_or_register = ((sups_evt == CM_SUPS_EVENT_INTERROGATE_CONF) ||
                          (sups_evt == CM_SUPS_EVENT_REGISTER_CONF));


  /* Read the first CFIS record */
  if (!cmmmgsdi_read(session_type, file_name, 1, CFIS_RECORD_SIZE, rec_data, sizeof(rec_data), cyclic))
  {
    CM_MSG_HIGH_0("Read record failed for CFIS");
    return;
  }

  switch (sups_info_ptr->conf_type)
  {
    case FWD_INFO:
    case FWD_FEATURE_LIST_INFO:
      if (sups_info_ptr->fwd_feature_list.present)
      {
        fwd_info = &sups_info_ptr->fwd_feature_list;
      }
      else if (sups_info_ptr->forwarding_info.present)
      {
        fwd_info = &sups_info_ptr->forwarding_info.forwarding_feature_list;
      }
      else
      {
        CM_MSG_HIGH_0("FWD list info not present");
        is_cfu_ind_updated = FALSE;
      }

      if (fwd_info != NULL)
      {
        for (i = 0; i<fwd_info->forwarding_feature_list_length; i++)
        {
          if (fwd_info->forwarding_feature[i].basic_service.present)
          {
            if (fwd_info->forwarding_feature[i].basic_service.code_type == TS_CODE)
            {
              bs_code = fwd_info->forwarding_feature[i].basic_service.bs_code;
              bs_type = TS_CODE;
            }
            else
            {
              bs_code = allBearerServices;
              bs_type = BS_CODE;
            }
          }
          if (fwd_info->forwarding_feature[i].ss_status.present)
          {
            is_active = fwd_info->forwarding_feature[i].ss_status.cm_ss_active;
          }
          else
          {
            is_active = (sups_evt == CM_SUPS_EVENT_ACTIVATE_CONF) ? TRUE : FALSE;
          }
          /* Update the CFU Indicator status  for the given basic service */
          cmsups_update_cfu_indicator_for_bs_code(&rec_data[1], bs_code, bs_type, is_active);

          /* Number needs to be updated only once */
          if ((fwd_info->forwarding_feature[i].forwarded_to_number[0].length > 0) && (!num_updated))
          {
            rec_data[2] = fwd_info->forwarding_feature[i].forwarded_to_number[0].length;
            memscpy(&rec_data[3], ((ARR_SIZE(rec_data) - 3)*sizeof(rec_data[0])),
                    fwd_info->forwarding_feature[i].forwarded_to_number[0].data,
                    MIN(fwd_info->forwarding_feature[i].forwarded_to_number[0].length, 11));
            num_updated = TRUE;
          }
        }
      }
      break;

    case SS_DATA_INFO:
      if (sups_info_ptr->ss_data.present)
      {
        if (sups_info_ptr->ss_data.cm_ss_status.present)
        {
          is_active = sups_info_ptr->ss_data.cm_ss_status.cm_ss_active;
        }
        bsg_list = &sups_info_ptr->ss_data.basic_service_group_list;
        if (bsg_list->present)
        {
          for (i = 0; i < bsg_list->list_length; i++)
          {
            if (bsg_list->list[i].code_type == TS_CODE)
            {
              bs_code = bsg_list->list[i].bs_code;
              bs_type = TS_CODE;
            }
            else
            {
              bs_code = allBearerServices;
              bs_type = BS_CODE;
            }
            /* Update the CFU Indicator status  for the given basic service */
            cmsups_update_cfu_indicator_for_bs_code(&rec_data[1], bs_code, bs_type, is_active);
          }
        }
        else
        {
          is_cfu_ind_updated = FALSE;
        }
      }
      else
      {
        is_cfu_ind_updated = FALSE;
      }
      break;

    case SS_STATUS_INFO:
      if (sups_info_ptr->ss_status.present)
      {
        is_active = sups_info_ptr->ss_status.cm_ss_active;
        /* Update the CFU Indicator status  for the given basic service */
        cmsups_update_cfu_indicator_for_bs_code(&rec_data[1], bs_code, bs_type, is_active);
      }
      else
      {
        is_cfu_ind_updated = FALSE;
      }
      break;

    case NO_INFO:
    {
      if ((sups_evt == CM_SUPS_EVENT_REGISTER_CONF) && (sups_info_ptr->ss_success))
      {
        /* If network sends sups_evet registration success with conf type, NO_INFO
        ** update the EFS
        */
        cmsups_update_cfu_indicator_for_bs_code(&rec_data[1], bs_code, bs_type, TRUE);
      }
      else
      {
        is_cfu_ind_updated = FALSE;
      }
      break;
    }

    default:
      CM_MSG_HIGH_1("Conf type %d not handled here", sups_info_ptr->conf_type);
      is_EF_write_req = FALSE;
      break;
  }

  if (!num_updated && sups_info_ptr->forwarded_to_number.present &&
      (sups_info_ptr->forwarded_to_number.length > 0))
  {
    rec_data[2] = sups_info_ptr->forwarded_to_number.length;
    memscpy(&rec_data[3], ((ARR_SIZE(rec_data) - 3)*sizeof(rec_data[0])),
            sups_info_ptr->forwarded_to_number.data,
            MIN(sups_info_ptr->forwarded_to_number.length, 11));
    num_updated = TRUE;
  }

  if (num_updated)
  {
    char ascii_buf[(2 * CM_CALLED_PARTY_BCD_NO_LENGTH) + 1] = { 0 };
    (void)cm_util_bcd_num_to_ascii_num(&rec_data[2], (byte *)ascii_buf, sizeof(ascii_buf));
  }

  /* If network hasn't sent any data, try to update EF with available data as we received success */
  if (!is_cfu_ind_updated)
  {
    if (is_query_or_register)
    {
      /* Dont update anything as we don't have relevant info */
      is_EF_write_req = FALSE;
    }
    else
    {
      /* This is Activate/Deactivate/Erase case and we got success.
      Update the cfu indicator for bs_code as requested  by user */
      cmsups_update_cfu_indicator_for_bs_code(&rec_data[1], bs_code, bs_type, is_active);
    }
  }

  CM_MSG_HIGH_1("is_EF_write_req = %d", is_EF_write_req);
  if (is_EF_write_req)
  {
    /* Data read and updated. Now write it back to SIM */
    write_data.data_len = CFIS_RECORD_SIZE;
    write_data.data_ptr = rec_data;

    /* Write the data back to SIM */
    if (cmmmgsdi_write(asubs_id, file_name, CM_MMGSDI_SUBS_TYPE_GW, write_data))
    {
      CM_MSG_HIGH_1("CFIS record written successfully for asubs_id=%d", asubs_id);
    }
    else
    {
      CM_MSG_HIGH_0("Write record failed for CFIS");
    }
  }
}/* cmsups_check_and_update_cfis */
#endif

/* <EJECT> */

/*===========================================================================

FUNCTION CMSUPS_MEM_FREE_SUPS_INFO

DESCRIPTION
De allocates the memory held by sups_info_ptr

DEPENDENCIES
none

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
void cmsups_mem_free_sups_info(
    sys_modem_as_id_e_type   asubs_id
    /* sub to get sups_info for */
)
{
  cmsups_sub_info_s_type   *cmsups_sub_info_ptr = NULL;

  cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

  if (cmsups_sub_info_ptr != NULL && cmsups_sub_info_ptr->sups_info != NULL)
  {
    cm_mem_free(cmsups_sub_info_ptr->sups_info);
    cmsups_sub_info_ptr->sups_info = NULL;
  }
}

#ifdef FEATURE_SUPS_RETRY
/*===========================================================================

FUNCTION CMWSUPS_MEM_FREE_SUPS_LIST_INFO

DESCRIPTION
De allocates the memory held by sups_info_ptr

DEPENDENCIES
none

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
void cmwsups_mem_free_sups_list_info(
  cmwsups_info_list_s_type    *list,
  /* Pointer to sups obj to be freed */

  cmwsups_info_list_s_type   **list_head
)
{
  CM_ASSERT(list != NULL)
  CM_ASSERT(list_head != NULL)

  if (list->prev == NULL)
  {
    /* Set list head to next */
    *list_head = list->next;
    if (list->next != NULL)
      (list->next)->prev = NULL;
  }
  else
  {
    (list->prev)->next = list->next;
    if (list->next != NULL)
      (list->next)->prev = list->prev;
  }

  cm_mem_free(list);

  return;
}
#endif

/* <EJECT> */
/*===========================================================================

FUNCTION CMSUPS_GET_PHASE2_MSG

DESCRIPTION
Return a pointer to Phase 2 ussd request message sent.

DEPENDENCIES
none

RETURN VALUE
cm_process_unstructured_ss_req_T*

SIDE EFFECTS
none

===========================================================================*/
static cm_process_unstructured_ss_req_T *cmsups_get_phase2_msg(void)
{
  static cm_process_unstructured_ss_req_T uss_phase2_msg;
  /* Process Unstructured SS request message */

  return &uss_phase2_msg;
}

/* <EJECT> */

#ifdef FEATURE_SUPS_RETRY
/*===========================================================================

FUNCTION cmwsups_cmd_copy_fields_1

DESCRIPTION
Copy the active fields of a sups command into the sups object.

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
static void cmwsups_cmd_copy_fields_1(
  const cm_sups_cmd_info_s_type   *cmd_info_ptr
  /* Pointer to a sups command */
)
{
  cm_sups_info_s_type      *sups_info_ptr = NULL;
  cmsups_sub_info_s_type   *cmsups_sub_info_ptr;
  sys_modem_as_id_e_type    asubs_id = SYS_MODEM_AS_ID_1;

  CM_ASSERT(cmd_info_ptr != NULL)

  if (cmd_info_ptr == NULL)
  {
    CM_ERR_0("cmd_info_ptr NULL ptr");
    return;
  }

  asubs_id = cmd_info_ptr->cmd_subs_id;
  cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

  /* Null check cmsups_sub_info_ptr */
  if (cmsups_sub_info_ptr == NULL)
  {
    CM_ERR_1("cmwsups_cmd_copy_fields_1: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
             asubs_id);
    return;
  }

  sups_info_ptr = cmsups_sub_info_ptr->sups_info;

  if (sups_info_ptr == NULL)
  {
    CM_ERR_1("sups_info NULL ptr, subs_id = %d", asubs_id);
    return;
  }

  *(&(cmsups_sub_info_ptr->sups_info_list->sups_cmd_info)) = *cmd_info_ptr;

  cmsups_sub_info_ptr->sups_client_ptr = cmsups_sub_info_ptr->sups_info_list->sups_client_ptr;
  sups_info_ptr->cc_result_alpha = cmsups_sub_info_ptr->sups_info_list->cc_result_alpha;
  sups_info_ptr->mod_ss_params.call_control_result = cmsups_sub_info_ptr->sups_info_list->cc_result;

  cmsups_cmd_copy_fields_into_info(sups_info_ptr, cmd_info_ptr);
}

/*===========================================================================

FUNCTION cmwsups_info_list_find_alloc

DESCRIPTION
Allocate a sups info buffer if corresponding buffer is not found for the given ss_ref and
return the found / allocated buffer.

DEPENDENCIES
none

RETURN VALUE
Pointer to allocated sups info struct.

SIDE EFFECTS
none

===========================================================================*/
static cmwsups_info_list_s_type *cmwsups_info_list_find_alloc(
  sys_modem_as_id_e_type   asubs_id,

  uint8                    ref
  /* ss_ref or invoke_id, depending on sups_cmd_type.
  ** ss_ref for MO and Invote id for MT
  */
)
{
  cmwsups_info_list_s_type   *list_ptr, *prev_list_ptr;
  dword                       uptime = time_get_uptime_secs();
  cmsups_sub_info_s_type     *cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

  /* Null check cmsups_sub_info_ptr */
  if (cmsups_sub_info_ptr == NULL)
  {
    CM_ERR_1("cmwsups_info_list_find_alloc: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
             asubs_id);
    return NULL;
  }

  list_ptr = cmsups_sub_info_ptr->sups_info_list_head;

  /* Parse till entry found with ss_ref or end of list */
  prev_list_ptr = list_ptr;
  while (list_ptr != NULL)
  {
    if (list_ptr->sups_cmd_info.ss_ref == ref)
    {
      CM_MSG_HIGH_1("SUPS record found, ss_ref %d",
                    list_ptr->sups_cmd_info.ss_ref);

      return list_ptr;
    }
    prev_list_ptr = list_ptr;
    list_ptr = list_ptr->next;
  }

  /* No entry found with ss_ref. Allocate now */
  if ((list_ptr = (cmwsups_info_list_s_type *)cm_mem_malloc(
                    sizeof(cmwsups_info_list_s_type))) == NULL)
  {
    sys_err_fatal_null_ptr_exception();
  }
  else
  {
    list_ptr->next = NULL;
    list_ptr->prev = prev_list_ptr;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /* Initialize sups cmd info
    */
    memset((void *)&(list_ptr->sups_cmd_info), 0, sizeof(cm_sups_cmd_info_s_type));

    CM_MSG_HIGH_1("New SUPS record, ss_ref %d", ref);
    list_ptr->sups_cmd_info.ss_ref = ref;
    list_ptr->next_orig_uptime = 0;
    list_ptr->sr_start_uptime = uptime;
  }

  if (prev_list_ptr == NULL)
  {
    cmsups_sub_info_ptr->sups_info_list_head = list_ptr;
  }
  else
  {
    prev_list_ptr->next = list_ptr;
  }

  return list_ptr;
}

/*===========================================================================

FUNCTION cmwsups_info_list_find

DESCRIPTION
Return sups_info_list buffer for the given ss_ref

DEPENDENCIES
none

RETURN VALUE
Pointer to sups_info_list buffer

SIDE EFFECTS
none

===========================================================================*/
cmwsups_info_list_s_type *cmwsups_info_list_find(
  sys_modem_as_id_e_type   asubs_id,

  uint8                    ref
  /* ss_ref or invoke_id, depending on sups_cmd_type.
  ** ss_ref for MO and Invote id for MT
  */
)
{
  cmsups_sub_info_s_type     *cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);
  cmwsups_info_list_s_type   *list_ptr;

  /* Null check cmsups_sub_info_ptr */
  if (cmsups_sub_info_ptr == NULL)
  {
    CM_ERR_1("cmwsups_info_list_find: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
             asubs_id);
    return NULL;
  }

  list_ptr = cmsups_sub_info_ptr->sups_info_list_head;

  /* Parse till entry found with ss_ref or end of list */
  while (list_ptr != NULL)
  {
    if (list_ptr->sups_cmd_info.ss_ref == ref)
    {
      CM_MSG_HIGH_1("SUPS record found, ss_ref %d",
                    list_ptr->sups_cmd_info.ss_ref);

      return list_ptr;
    }

    list_ptr = list_ptr->next;
  }

  CM_MSG_HIGH_1("SUPS record NOT found for ss_ref %d", ref);
  return NULL;
}


/*===========================================================================

FUNCTION cmwsups_check_for_cs_entry_in_list

DESCRIPTION
Return TRUE if sups info list contains a node corresponding to the sups command with route as CS

DEPENDENCIES
none

RETURN VALUE
boolean

SIDE EFFECTS
none

===========================================================================*/

extern boolean cmwsups_check_for_cs_entry_in_list(
  cmwsups_info_list_s_type * sups_info_list_head
  )
{
  cmwsups_info_list_s_type   *list_ptr;
  list_ptr = sups_info_list_head;
  /* Parse till entry found with sups_cmd_route as CS or end of list */
    while (list_ptr != NULL)
    {
        if (list_ptr->sups_cmd_route == CMSUPS_CMD_ALLOW_CS)
        {
            CM_MSG_HIGH_0("SUPS record found with CS route");

            return TRUE;
        }

        list_ptr = list_ptr->next;
    }
  return FALSE;
}
#endif

/*===========================================================================

FUNCTION CMSUPS_CLIENT_CMD_ERR

DESCRIPTION
Notify clients of a specified SUPS command error.

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
static void cmsups_client_cmd_err(
  const cm_sups_cmd_s_type   *sups_cmd_ptr,
  /* Pointer to a CM command */

  cm_sups_cmd_err_e_type      cmd_err
  /* Indicate the sups command error */
)
{
  CM_ASSERT(sups_cmd_ptr != NULL);

  /* verify the command type */
  CM_ASSERT(sups_cmd_ptr->cmd_type == CM_CMD_TYPE_SUPS);

  CM_ASSERT(BETWEEN(cmd_err, CM_SUPS_CMD_ERR_NONE, CM_SUPS_CMD_ERR_MAX));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If commanding client supplied a callback function,
  notify this client of command error status.
  */
  if (sups_cmd_ptr->is_modem_cc_api == TRUE)
  {
    /* With feature defined, for these commands,
    ** only new callback is supported
    */
    if (sups_cmd_ptr->cmd_cc_cb_func != NULL)
    {
      sups_cmd_ptr->cmd_cc_cb_func(sups_cmd_ptr->data_block_ptr,
                                   sups_cmd_ptr->cmd,
                                   cmd_err,
                                   sups_cmd_ptr->cmd_info.cmd_err_cause,
                                   sups_cmd_ptr->cc_result_alpha);
    }
  }
  else
  {
    if (sups_cmd_ptr->cmd_cb_func != NULL)
    {
      sups_cmd_ptr->cmd_cb_func(sups_cmd_ptr->data_block_ptr,
                                sups_cmd_ptr->cmd,
                                cmd_err);
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If error status is other than NO ERROR,
  notify the client list of command error, as well.
  */
  if (cmd_err != CM_SUPS_CMD_ERR_NOERR)
  {
    cmclient_list_sups_cmd_err_ntfy(cmd_err, sups_cmd_ptr);
  }
}

/*===========================================================================

FUNCTION cmwsups_is_uss_cmd_already_present

DESCRIPTION
Notify clients of a specified SUPS command error.

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
SS reference of the existing USS command
0 if not found

SIDE EFFECTS
none

===========================================================================*/
static uint8 cmwsups_is_uss_cmd_already_present(
  sys_modem_as_id_e_type   asubs_id
)
{
  cmsups_s_type              *sups_ptr = cmsups_ptr();
  cmsups_sub_info_s_type     *cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);
  cmwsups_info_list_s_type   *sups_list = NULL;
  uint8                       uss_cmd = 0;
  /* Get the sups command pointer */

  /* Null check cmsups_sub_info_ptr */
  if (cmsups_sub_info_ptr == NULL)
  {
    CM_ERR_1("cmwsups_is_uss_cmd_already_present: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
             asubs_id);
    return uss_cmd;
  }

#ifdef FEATURE_SUPS_RETRY
  sups_list = cmsups_sub_info_ptr->sups_info_list_head;

  if (sups_list != NULL)
  {
    while (sups_list != NULL)
    {
      if (sups_list->cmd == CM_SUPS_CMD_PROCESS_USS)
      {
        uss_cmd = sups_list->sups_cmd_info.ss_ref;
        break;
      }
      sups_list = sups_list->next; /* Move to next record */
    }
  }
#endif

  return uss_cmd;
}

boolean cmsups_is_wlan_sups_cmd(
  cm_sups_cmd_e_type       cmd,

  sys_modem_as_id_e_type   asubs_id
)
{
#ifdef FEATURE_IP_CALL
  /*USSD go with voice RAT*/
  if (cmipsups_is_ussd_sups_cmd(cmd))
  {
    if (cmipapp_find_ussd_support_app(asubs_id) != CMIPAPP_ID_INVALID &&
        cmipapp_find_active_app_sys_mode(CM_CALL_TYPE_VOICE, asubs_id) == SYS_SYS_MODE_WLAN)
    {
      return TRUE;
    }
  }
  /*IMS do cmipapp_cap_reg for sups command over UT, not for USSD*/
  else
  {
    if (cmipapp_find_active_app(SYS_SYS_MODE_WLAN, CM_CALL_TYPE_SUPS, asubs_id) != CMIPAPP_ID_INVALID)
    {
      return TRUE;
    }
  }
#endif

  return FALSE;
}
/*===========================================================================

FUNCTION cmsups_get_reject_interrogate_clir_oir_pref

DESCRIPTION
Returns the sups_reject_oir_clir_interrogation for a given asubs_id

DEPENDENCIES
None

RETURN VALUE
cmsups_is_reject_oir_clir_e_type

SIDE EFFECTS
None

===========================================================================*/
cmsups_is_reject_oir_clir_e_type cmsups_get_reject_interrogate_clir_oir_pref(
  sys_modem_as_id_e_type sub_id
)
{
  cmsups_sub_info_s_type *cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(sub_id);

  if (cmsups_sub_info_ptr == NULL)
  {
    return CMSUPS_SS_REJECT_NONE;
  }
  CM_MSG_HIGH_2("cmsups_get_reject_interrogate_clir_oir_pref: %d, sub_id: %d",
                cmsups_sub_info_ptr->sups_reject_oir_clir_interrogation, sub_id);
  return cmsups_sub_info_ptr->sups_reject_oir_clir_interrogation;
} /* cmsups_get_reject_interrogate_clir_oir_pref () */

/*===========================================================================

FUNCTION  cmwsups_map_idx_to_code

DESCRIPTION
Maps the index to corresponding ss code

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
CM_SUPS_CMD_ERR_NONE if command is allowed in the current state
of the call/phone, specific cm_sups_cmd_err_e_type otherwise.

SIDE EFFECTS
None

===========================================================================*/
static byte  cmwsups_map_idx_to_code(
  uint8                   idx
)
{
byte    ss_code;
    switch(idx)
    {
        case colr_idx:
        {
        ss_code = colr;
        break;
        }
        case allCallRestrictionSS_idx:
        {
          ss_code = allCallRestrictionSS ;
          break;
        }
        case baoc_idx:  
        {
          ss_code = baoc;
          break;
        }
        case boic_idx:
        {
          ss_code = boic;
          break;
        }
        case boicExHC_idx:
        {
          ss_code = boicExHC;
          break;
        }
        case baic_idx:
        {
          ss_code = baic;
          break;
        }
        case bicRoam_idx:
        {
          ss_code = bicRoam;
          break;
        }
        case bain_idx:
        {
          ss_code = bain;
          break;
        }
        /*MAX entry of UE BASED SUPS ARRAY is reserved for CW */
        case CMEFS_UE_BASED_SUPS_ARRAY_USED:
        {
        ss_code = cw;
        break;
        }
        default:
        ss_code = all_ss;
        break;
    }
    return ss_code;
}

/*===========================================================================

FUNCTION  cmsups_client_cmd_check

DESCRIPTION
Check for sups command parameter errors and whether a specified command
is allowed in the current state of the call/phone.

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
CM_SUPS_CMD_ERR_NONE if command is allowed in the current state
of the call/phone, specific cm_sups_cmd_err_e_type otherwise.

SIDE EFFECTS
None

===========================================================================*/
static cm_sups_cmd_err_e_type cmsups_client_cmd_check(
  cm_sups_cmd_s_type  *sups_cmd_ptr
  /* Pointer to a CM command */
)
{
  cm_sups_cmd_err_e_type        cmd_err = CM_SUPS_CMD_ERR_NOERR;
  /* Initialize command error to NO-ERROR */

  cmph_s_type                  *ph_ptr = cmph_ptr();
  /* Pointer to the phone object */

  cmss_s_type                  *ss_ptr = cmss_ptr();
  /* Pointer to the serving system object */

  sys_modem_as_id_e_type        asubs_id = SYS_MODEM_AS_ID_1;

  cmsups_sub_info_s_type       *sups_sub_info_ptr;

  sd_ss_mode_pref_e_type        comm_mode = SD_SS_MODE_PREF_NONE;
  sd_ss_band_pref_e_type        comm_band = SD_SS_BAND_PREF_NONE;
  sd_ss_band_pref_e_type        comm_tds_band = SD_SS_BAND_PREF_NONE;
  sys_lte_band_mask_e_type      comm_lte_band = SYS_LTE_BAND_MASK_CONST_NONE;
  sys_srv_status_e_type         srv_status;
  cmregprx_state_e_type         cmreg_state;
  cmregprx_substate_e_type      cmreg_substate;
  sys_srv_status_e_type         srv_status_stk2;
  cmregprx_state_e_type         cmreg_state_stk2;
  cmregprx_substate_e_type      cmreg_substate_stk2;
  cm_ue_based_sups_config_s_type     cm_ue_based_sups_config;

  boolean                       is_ue_based_cw = FALSE;
  boolean                       is_ue_based_sups = FALSE;
    mm_sub_stk_id_s_type hdr_mm_id;
    cm_service_status_s_type stack1_srv_info;
  volte_sub_info_s_type *volte_sub_info_ptr;
  uint8 i;
   cmsups_is_reject_oir_clir_e_type clir_oir_rej_type;
  cmsups_cmd_route_e_type          clir_oir_route;
  mm_sub_stk_id_s_type mm_id;
  sd_si_info_s_type  *si_info_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_ASSERT(sups_cmd_ptr != NULL);
  CM_ASSERT(sups_cmd_ptr->cmd_type == CM_CMD_TYPE_SUPS);
  CM_ASSERT(ph_ptr != NULL);
  CM_ASSERT(ss_ptr != NULL);

  asubs_id = sups_cmd_ptr->cmd_info.cmd_subs_id;

  if (!cmutil_is_as_id_valid(asubs_id))
  {
	return FALSE;
  }

  sups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);
    hdr_mm_id.asubs_id = asubs_id;
    hdr_mm_id.stk_id = MM_STACK_1;
    cmss_get_service_status_per_stack(hdr_mm_id, &stack1_srv_info);

  memset(&mm_id, 0, sizeof(mm_sub_stk_id_s_type));
  mm_id.asubs_id = asubs_id;
  mm_id.stk_id = cmph_determine_stk_per_rat(asubs_id, SYS_SYS_MODE_LTE);

  si_info_ptr = cmss_true_stack_info_ptr(mm_id);

  if(si_info_ptr == NULL)
  {
    return FALSE;
  }

  CM_MSG_HIGH_2("cmsups_cmd_check, cmd=%d, subs_id=%d",
                sups_cmd_ptr->cmd, asubs_id);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* check cmd is already in error or not */
  if (sups_cmd_ptr->cmd_info.cmd_err != CM_SUPS_CMD_ERR_NOERR)
  {
    return sups_cmd_ptr->cmd_info.cmd_err;
  }

  /* check asubs_id is valid */
  if (asubs_id == SYS_MODEM_AS_ID_NONE)
  {
    return CM_SUPS_CMD_ERR_SUBS_ID_P;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (sups_cmd_ptr->client_ptr == CM_CLIENT_PTR_NONE ||
      sups_cmd_ptr->client_ptr->init_mask != CM_INIT_MASK)
  {
    /* Invalid Client ID */
    cmd_err = CM_SUPS_CMD_ERR_CLIENT_ID_P;
    CM_ERR_0("CM_SUPS_CMD_ERR_CLIENT_ID_P");
  }

  /* Null check sups_sub_info_ptr */
  if (sups_sub_info_ptr == NULL)
  {
    CM_ERR_1("cmsups_client_cmd_check: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
             asubs_id);
    return CM_SUPS_CMD_ERR_OTHER;
  }

  /* If ue_based_sups is configured, we don't need to do any sups cmd check */
  cm_ue_based_sups_config = cmsups_map_ss_code_to_sups_config(sups_cmd_ptr->cmd_info.ss_code,asubs_id);

  clir_oir_rej_type = cmsups_get_reject_interrogate_clir_oir_pref(asubs_id);

  CM_MSG_HIGH_3("cmsups_client_cmd_check, is_ue_based_sups=%d, ss_code=%d, clir_oir_rej_type = %d", 
                   cm_ue_based_sups_config.use_ue_based_sups, 
                   sups_cmd_ptr->cmd_info.ss_code,
                   clir_oir_rej_type
                   );
  
  if((cm_ue_based_sups_config.use_ue_based_sups == TRUE) &&
     (sups_cmd_ptr->cmd_info.ss_code == cw) &&
     cmsups_is_immediate_sups_sync_with_cs_required(asubs_id) &&
     (cmcall_is_present_cellular_ip_call_per_sub(asubs_id) == TRUE))
  {
    CM_MSG_HIGH_0("cw sups is rejected as immead sync with cs is needed during ongoing VoLTE call,");
    return CM_SUPS_CMD_ERR_REJECT_CS_ON_VOLTE;
  }

  if(cm_ue_based_sups_config.use_ue_based_sups &&
      ( (sups_cmd_ptr->cmd == CM_SUPS_CMD_ACTIVATE) ||
        (sups_cmd_ptr->cmd == CM_SUPS_CMD_DEACTIVATE) ||
        (sups_cmd_ptr->cmd == CM_SUPS_CMD_INTERROGATE) ) )
  {
    return CM_SUPS_CMD_ERR_NOERR;
  }

  /* For CLIR/OIR, CM need not perform the rest of the check's
  if it is decided to use UE based CLIR/OIR appraoch.*/
  if((sups_cmd_ptr->cmd_info.ss_code == clir) 
      && (clir_oir_rej_type != CMSUPS_SS_REJECT_NONE)
      && (sups_cmd_ptr->cmd == CM_SUPS_CMD_INTERROGATE)
    )
  {
    #if defined FEATURE_IP_CALL
    clir_oir_route = cmipsups_route_check(sups_cmd_ptr->cmd, asubs_id);
    #else
    clir_oir_route = CMSUPS_CMD_ALLOW_CS;
    #endif

    if (clir_oir_route == CMSUPS_CMD_ALLOW_IMS)
    {
      if((clir_oir_rej_type == CMSUPS_SS_REJECT_OIR) ||
         (clir_oir_rej_type == CMSUPS_SS_REJECT_OIR_CLIR))
      {
        return CM_SUPS_CMD_ERR_NOERR;
      }
    }
    else if (clir_oir_route == CMSUPS_CMD_ALLOW_CS)
    {
      if((clir_oir_rej_type == CMSUPS_SS_REJECT_CLIR) ||
         (clir_oir_rej_type == CMSUPS_SS_REJECT_OIR_CLIR))
      {
        return CM_SUPS_CMD_ERR_NOERR;
      }
    }
  }

  /*Since change password is NA when UE based call barring is active, reject the command*/
  if(sups_cmd_ptr->cmd == CM_SUPS_CMD_REG_PASSWORD)
  {
     volte_sub_info_ptr = cmph_get_volte_sub_info_ptr(asubs_id);
     for(i= allCallRestrictionSS_idx; i<= bain_idx; i++)
     {
        if(volte_sub_info_ptr->ue_based_sups[i].use_ue_based_sups == TRUE)
        {
          CM_MSG_HIGH_0("Change password not supported when UE_BASED_CALL_BARRING is active");
          return CM_SUPS_CMD_ERR_OTHER;
        }
     }
  }
  
  /* Check whether the command is possible */
  if (!cmsups_is_wlan_sups_cmd(sups_cmd_ptr->cmd, asubs_id) &&
      ph_ptr->device_prop.ph_oprt_mode.oprt_mode != SYS_OPRT_MODE_ONLINE)
  {
    /* Phone must be online to originate the command */
    return CM_SUPS_CMD_ERR_OFFLINE_S;
  }
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(cmph_is_ims_capable_on_sub(asubs_id))
  {
#if defined FEATURE_IP_CALL
    /* Check sups command route */
    sups_sub_info_ptr->sups_cmd_route = cmipsups_route_check(sups_cmd_ptr->cmd, asubs_id);
   
    /* Basically, first check whether cmd type is IMS applicable USSD code */
    if (cmipsups_is_ussd_sups_cmd(sups_cmd_ptr->cmd))
    {
      /*It means that VoLTE (Normal/Emergency) Call is up*/
      if (sups_sub_info_ptr->sups_cmd_route == CMSUPS_CMD_REJECT)
      {
        return CM_SUPS_CMD_ERR_USSD_REJECTED;
      }

      /* If CS route is calculated during ongoing VOLTE call, reject all USSD Sups request */
      if (sups_sub_info_ptr->sups_cmd_route == CMSUPS_CMD_ALLOW_CS
          && (cmcall_is_active_audio_call_per_sub_except_wlan(asubs_id) 
          ||  cmsds_is_ext_ims_call_active_per_sub(asubs_id)))
      {
        CM_MSG_HIGH_0("SUPS: During ongoing VoLTE call, sups cmd with CS route is rejected");
        return CM_SUPS_CMD_ERR_REJECT_CS_ON_VOLTE;
      }
    }
    else /* non USSD Supplimenraty Services */
    {
      /* If CS route is calculated during ongoing VOLTE call, reject all non-USSD Sups request */
      if (sups_sub_info_ptr->sups_cmd_route == CMSUPS_CMD_ALLOW_CS
          && (cmcall_is_active_audio_call_per_sub_except_wlan(asubs_id)
           ||   cmsds_is_ext_ims_call_active_per_sub(asubs_id)))
      {
        return CM_SUPS_CMD_ERR_REJECT_CS_ON_VOLTE;
      }

      if (sups_sub_info_ptr->sups_cmd_route == CMSUPS_CMD_REJECT)
      {
        cmd_err = CM_SUPS_CMD_ERR_ROUTE_FAILURE;
        CM_MSG_HIGH_1("CM_SUPS_CMD_ERR_ROUTE_FAILURE %d", sups_sub_info_ptr->sups_cmd_route);
      }
    }
#else
    sups_sub_info_ptr->sups_cmd_route = CMSUPS_CMD_ALLOW_CS;
#endif
  }
  else
  {
    sups_sub_info_ptr->sups_cmd_route = CMSUPS_CMD_ALLOW_CS;
  }
  CM_MSG_HIGH_2("sups_cmd=%d, route=%d",
                sups_cmd_ptr->cmd,
                sups_sub_info_ptr->sups_cmd_route);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* skip service status checking for sups command over IMS(Ut interface),
  ** not including USSD
  */
  srv_status = cmss_get_info_srv_status(asubs_id, MM_STACK_0);
  cmreg_state = cmregprx_get_state(cmph_map_sub_stk_to_id(asubs_id, MM_STACK_0));
  cmreg_substate = cmregprx_get_substate(cmph_map_sub_stk_to_id(asubs_id, MM_STACK_0));
  srv_status_stk2 = cmss_get_info_srv_status(asubs_id, MM_STACK_2);
  cmreg_state_stk2= cmregprx_get_state(cmph_map_sub_stk_to_id(asubs_id, MM_STACK_2));
  cmreg_substate_stk2= cmregprx_get_substate(cmph_map_sub_stk_to_id(asubs_id, MM_STACK_2));
  CM_MSG_HIGH_6("stk0: srv_status=%d, state=%d substate=%d, stk2: srv_status=%d, state=%d, sub=%d",
                srv_status, cmreg_state, cmreg_substate, srv_status_stk2, cmreg_state_stk2, asubs_id);

  if((((srv_status != SYS_SRV_STATUS_SRV) ||
      (cmreg_state != CMREGPRX_STATE_ACTIVE) ||
      (cmreg_substate == CMREGPRX_SUBSTATE_WAIT_STOP_CNF)) &&
      ((srv_status_stk2 != SYS_SRV_STATUS_SRV) ||
      (cmreg_state_stk2 != CMREGPRX_STATE_ACTIVE) ||
      (cmreg_substate_stk2 == CMREGPRX_SUBSTATE_WAIT_STOP_CNF)))&&
      /*for wlan UT interface or USSD over wlan, no service status checking*/
      !(cmsups_is_wlan_sups_cmd(sups_cmd_ptr->cmd, asubs_id) &&
        sups_sub_info_ptr->sups_cmd_route == CMSUPS_CMD_ALLOW_IMS)&&
        // If HDR is in service, app is registered for call type SUPS and the route is IMS, do not return error.
        !((cmipapp_find_active_app(SYS_SYS_MODE_HDR, CM_CALL_TYPE_SUPS,asubs_id) != CMIPAPP_ID_INVALID) &&
        (stack1_srv_info.srv_status == SYS_SRV_STATUS_SRV) &&
        (sups_sub_info_ptr->sups_cmd_route == CMSUPS_CMD_ALLOW_IMS) &&
        (asubs_id == ph_ptr->device_prop.msim_prop.default_data_subs))
    )
  {
    /* If sups command is location notification and service is limited,
    ** do not return error.
    */
    if(!(sups_cmd_ptr->cmd == CM_SUPS_CMD_LCS_LOCATION_NOTIFICATION_RES
         &&
         srv_status == SYS_SRV_STATUS_LIMITED)
      )
    {
      return CM_SUPS_CMD_ERR_SRV_STATE_S;
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* return error for processing MO supplementry services when the 3gpp mode
  ** preference is LTE only and the domain is CS_PS
  */
  /* Get ( capability & preference ) */
  cmph_get_comm_mode_band_capab_with_pm(&comm_mode,
                                        &comm_band,
                                        &comm_lte_band,
                                        &comm_tds_band,
                                        asubs_id);

  /* remove 3gpp2 from the mode as it should not be considered for this cmd*/
  comm_mode = SD_MODE_DELETE(comm_mode, SD_SS_MODE_PREF_CDMA_HDR);

  if ((sups_sub_info_ptr->sups_cmd_route == CMSUPS_CMD_ALLOW_CS) &&
      (((comm_mode & SD_SS_MODE_PREF_TDS_GSM_WCDMA) == SD_SS_MODE_PREF_NONE) ||
       cmcall_is_lte_only_mode(cmph_map_sd_mode_pref_to_cm_mode_pref(comm_mode), \
                               cmutil_map_sd_band_pref_to_cm_band_pref(comm_band), \
                               comm_lte_band, \
                               cmutil_map_sd_tds_band_pref_to_cm_tds_band_pref(comm_tds_band))))
  {
    CM_MSG_HIGH_1("sups_cmd rejected as sub %d is LTE_ONLY and route is CS", asubs_id);
    cmd_err = CM_SUPS_CMD_ERR_SRV_STATE_S;
  }


  if(cmd_err == CM_SUPS_CMD_ERR_NOERR &&
     sups_sub_info_ptr->sups_cmd_route == CMSUPS_CMD_ALLOW_CS
      &&
     !(cmregprx_is_rat_in_rat_priority_on_subs(asubs_id, SYS_SYS_MODE_GSM) ||
       cmregprx_is_rat_in_rat_priority_on_subs(asubs_id, SYS_SYS_MODE_WCDMA) ||
       cmregprx_is_rat_in_rat_priority_on_subs(asubs_id, SYS_SYS_MODE_TDS))
  )
  {
    CM_MSG_HIGH_1("sups_cmd rejected as sub %d does not have GWT in rat pri list and route is CS",
                  asubs_id);
    cmd_err = CM_SUPS_CMD_ERR_SRV_STATE_S;
  }

  /*In case route is calculated as CS and UE is camped on LTE with no CSFB support,
  reject the sups command  */

  if(sups_sub_info_ptr->sups_cmd_route == CMSUPS_CMD_ALLOW_CS && 
  	  si_info_ptr->srv_status == SYS_SRV_STATUS_SRV &&
  	  si_info_ptr->is_stable_in_svc == TRUE && 
  	  si_info_ptr->mode ==  SYS_SYS_MODE_LTE && 
  	  !cmsds_is_ppcsfb_possible(asubs_id)
  	  )
  {
    cmd_err = CM_SUPS_CMD_ERR_SRV_STATE_S;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return cmd_err;
}

/*===========================================================================

FUNCTION CMSUPS_USSD_PHASE1_REQ

DESCRIPTION
Send a phase1 USSD message

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
static void cmsups_ussd_phase1_req(
  sys_modem_as_id_e_type   asubs_id
)
{
  mn_cnm_cmd_type                  *mn_cmd_ptr;
  /* MN command pointer */

  cm_sups_info_s_type              *sups_info_ptr;
  /* Pointer to the sups information */

  byte                              ascii_string[MAX_USS_CHAR_ASCII];
  /* Storage for character string converted from 7 bit
  **
  ** CM api does not allow more than MAX_USS_CHAR_ASCII size of ussd data.
  ** CM always receives ussd in ascii and performs encoding
  ** on them. Ascii string converted out of a 7bit encoded string cannot
  ** be more than MAX_USS_CHAR_ASCII.
  */

  cm_process_unstructured_ss_req_T *uss_phase2_msg_ptr = cmsups_get_phase2_msg();
  /* Buffered phase 2 request */

  cm_process_unstructured_ss_data_T *mn_ss_data_ptr = NULL;
  /* Points to the SS data being sent to MN */

  size_t                      num_converted;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  sups_info_ptr = cmsups_get_sups_info_ptr(asubs_id);

  /* Null check sups_info_ptr */
  if (sups_info_ptr == NULL)
  {
    CM_ERR_1("cmsups_ussd_phase1_req: sups_info_ptr NULL ptr for asubs_id %d",
             asubs_id);
    return;
  }

  /* allocate a buffer */
  mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_PROCESS_UNSTRUCTURED_SS_DATA);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Copy command information fields. */
  mn_ss_data_ptr = &mn_cmd_ptr->cmd.cm_process_unstructured_ss_data;

  /* ss reference */
  mn_ss_data_ptr->cm_ss_ref = sups_info_ptr->ss_ref;
  mn_ss_data_ptr->ss_User_Data.present = uss_phase2_msg_ptr->uss_data.present;

  /*lint -save -e641 Converting enum to int */
  if (uss_phase2_msg_ptr->uss_data.present)
  {
    /* ussd phase1 request accepts only IA-5 string. This is same as ASCII.
    ** Convert 7bit encoded data into 8-bit, for all other encodings copy
    ** data as it is.
    */
    if (((uss_phase2_msg_ptr->uss_data.uss_data_coding_scheme &
          CM_USSD_ALPHABET_MASK) == CM_USSD_DCS_7_BIT)
        ||
        ((uss_phase2_msg_ptr->uss_data.uss_data_coding_scheme &
          CM_USSD_ALPHABET_MASK) == CM_USSD_DCS_UNSPECIFIED)
       )
    {
      /* Ascii buffer should be of size ((7-bit_buffer * 8/7) + 1 ) bytes
      */
      (void)cm_util_gsm7_to_iso8859(
        uss_phase2_msg_ptr->uss_data.ussData,
        uss_phase2_msg_ptr->uss_data.size,
        FALSE,
        ascii_string,
        sizeof(ascii_string),
        &num_converted
      );

      mn_ss_data_ptr->ss_User_Data.size = (byte)num_converted;

      if (mn_ss_data_ptr->ss_User_Data.size <= MAX_SS_USER_DATA_CHAR)
      {
        if (strlcpy(mn_ss_data_ptr->ss_User_Data.ss_UserData,
                    (char *)ascii_string,
                    sizeof(mn_ss_data_ptr->ss_User_Data.ss_UserData))
            >= sizeof(mn_ss_data_ptr->ss_User_Data.ss_UserData))
        {
          CM_ERR_0("Wrong ascii_string received");
        }
      }
      else
      {
        CM_ERR_1("ss_user_Data size exceeds MAX_SS_USER_DATA_CHAR %d",
                 mn_ss_data_ptr->ss_User_Data.size);
      }
    }
    else
    {
      mn_ss_data_ptr->ss_User_Data.size = uss_phase2_msg_ptr->uss_data.size;

      memscpy(mn_ss_data_ptr->ss_User_Data.ss_UserData,
              sizeof(mn_ss_data_ptr->ss_User_Data.ss_UserData),
              uss_phase2_msg_ptr->uss_data.ussData,
              MIN(sizeof(uss_phase2_msg_ptr->uss_data.ussData), uss_phase2_msg_ptr->uss_data.size));
    }
  }
  else
  {
    CM_ERR_0("uss_phase2_msg not present to send phase1 ussd");
  }
  /*lint -restore -e641 Converting enum to int */

  mn_cmd_ptr->cmd.cm_process_unstructured_ss_data.as_id = asubs_id;

  /* send the command to MN */
  cm_mn_send_cmd(mn_cmd_ptr);
}

#ifdef FEATURE_SUPS_RETRY
/*===========================================================================

FUNCTION cmsups_client_cmd_resend

DESCRIPTION
Resends a client sups commands to MN.

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
static void cmsups_client_cmd_resend(
  cmwsups_info_list_s_type   *sups_info_list_ptr
  /* Pointer to the sups object */
)
{
  mn_cnm_cmd_type          *mn_cmd_ptr;
  /* MN command pointer */

  uint8                     num_bytes = 0;
  /* Number of actual bytes required for USSD data */

  cm_process_unstructured_ss_req_T *uss_phase2_msg_ptr = cmsups_get_phase2_msg();
  /* Points to storage for phase2 msg */

  cm_sups_cmd_info_s_type  *sups_cmd_info_ptr = NULL;

  sys_modem_as_id_e_type    asubs_id = SYS_MODEM_AS_ID_1;
  cmsups_sub_info_s_type   *cmsups_sub_info_ptr;
  byte                            calculated_size = 0;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_ASSERT(sups_info_list_ptr != NULL);

  asubs_id = sups_info_list_ptr->sups_cmd_info.cmd_subs_id;
  cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

  /* Null check cmsups_sub_info_ptr */
  if (cmsups_sub_info_ptr == NULL)
  {
    CM_ERR_1("cmsups_client_cmd_resend: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
             asubs_id);
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Allocate memory for sups_info_ptr */
  if (cmsups_sub_info_ptr->sups_info == NULL)
  {
    cmsups_sub_info_ptr->sups_info = cmsups_info_alloc();
    cmsups_init_sups_info(cmsups_sub_info_ptr->sups_info);
  }
  cmsups_sub_info_ptr->sups_info->asubs_id = asubs_id;

  cmsups_sub_info_ptr->sups_info_list = sups_info_list_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  sups_cmd_info_ptr = &(sups_info_list_ptr->sups_cmd_info);

  switch (sups_info_list_ptr->cmd)
  {
    /*-------------------------*/
    /*    RegisterSS request   */
    /*-------------------------*/

    case CM_SUPS_CMD_REGISTER:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_REGISTER_SS_REQ);

      /* ss reference */
      mn_cmd_ptr->cmd.cm_register_ss_req.cm_ss_ref = sups_cmd_info_ptr->ss_ref;

      /* ss code */
      mn_cmd_ptr->cmd.cm_register_ss_req.ss_code = sups_cmd_info_ptr->ss_code;

      /* Copy command information fields. */

      mn_cmd_ptr->cmd.cm_register_ss_req.basic_service = sups_cmd_info_ptr->basic_service;

      mn_cmd_ptr->cmd.cm_register_ss_req.forwarded_to_number =
        sups_cmd_info_ptr->forwarded_to_number;

      mn_cmd_ptr->cmd.cm_register_ss_req.forwarded_to_subaddress =
        sups_cmd_info_ptr->forwarded_to_subaddress;

      mn_cmd_ptr->cmd.cm_register_ss_req.cm_no_reply_condition_timer =
        sups_cmd_info_ptr->nr_timer;

      mn_cmd_ptr->cmd.cm_register_ss_req.as_id = asubs_id;

      /* send the command to MN */
      cm_mn_send_cmd(mn_cmd_ptr);

      /* save a copy in the sups object and let the clients know */

      cmwsups_cmd_copy_fields_1(sups_cmd_info_ptr);

      cmsups_event(asubs_id, CM_SUPS_EVENT_REGISTER);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*-------------------------*/
    /*    EraseSS request      */
    /*-------------------------*/

    case CM_SUPS_CMD_ERASE:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_ERASE_SS_REQ);

      mn_cmd_ptr->cmd.cm_erase_ss_req.cm_ss_ref = sups_cmd_info_ptr->ss_ref;

      mn_cmd_ptr->cmd.cm_erase_ss_req.ss_code = sups_cmd_info_ptr->ss_code;

      mn_cmd_ptr->cmd.cm_erase_ss_req.basic_service =
        sups_cmd_info_ptr->basic_service;

      mn_cmd_ptr->cmd.cm_erase_ss_req.as_id = asubs_id;

      /* send the command to MN */
      cm_mn_send_cmd(mn_cmd_ptr);

      /* save a copy in the sups object and let the clients know */
      cmwsups_cmd_copy_fields_1(sups_cmd_info_ptr);

      cmsups_event(asubs_id, CM_SUPS_EVENT_ERASE);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*-------------------------*/
    /*    ActivateSS request   */
    /*-------------------------*/

    case CM_SUPS_CMD_ACTIVATE:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_ACTIVATE_SS_REQ);

      /* ss reference */
      mn_cmd_ptr->cmd.cm_activate_ss_req.cm_ss_ref = sups_cmd_info_ptr->ss_ref;

      /* ss code */
      mn_cmd_ptr->cmd.cm_activate_ss_req.ss_code = sups_cmd_info_ptr->ss_code;

      /* Copy command information fields. */
      mn_cmd_ptr->cmd.cm_activate_ss_req.basic_service =
        sups_cmd_info_ptr->basic_service;

      mn_cmd_ptr->cmd.cm_activate_ss_req.as_id = asubs_id;

      /* send the command to MN */
      cm_mn_send_cmd(mn_cmd_ptr);

      /* save a copy in the sups object and let the clients know */
      cmwsups_cmd_copy_fields_1(sups_cmd_info_ptr);

      cmsups_event(asubs_id, CM_SUPS_EVENT_ACTIVATE);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*-------------------------*/
    /*    DeactivateSS request */
    /*-------------------------*/

    case CM_SUPS_CMD_DEACTIVATE:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_DEACTIVATE_SS_REQ);

      /* ss reference */
      mn_cmd_ptr->cmd.cm_deactivate_ss_req.cm_ss_ref =
        sups_cmd_info_ptr->ss_ref;

      /* ss code */
      mn_cmd_ptr->cmd.cm_deactivate_ss_req.ss_code =
        sups_cmd_info_ptr->ss_code;

      /* Copy command information fields. */

      mn_cmd_ptr->cmd.cm_deactivate_ss_req.basic_service =
        sups_cmd_info_ptr->basic_service;

      mn_cmd_ptr->cmd.cm_deactivate_ss_req.as_id = asubs_id;

      /* send the command to MN */
      cm_mn_send_cmd(mn_cmd_ptr);

      /* save a copy in the sups object and let the clients know */
      cmwsups_cmd_copy_fields_1(sups_cmd_info_ptr);

      cmsups_event(asubs_id, CM_SUPS_EVENT_DEACTIVATE);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*-------------------------*/
    /*   InterrogateSS request */
    /*-------------------------*/

    case CM_SUPS_CMD_INTERROGATE:
      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_INTERROGATE_SS_REQ);

      /* Copy command information fields. */

      /* ss reference */
      mn_cmd_ptr->cmd.cm_interrogate_ss_req.cm_ss_ref =
        sups_cmd_info_ptr->ss_ref;

      /* ss code */
      mn_cmd_ptr->cmd.cm_interrogate_ss_req.ss_code =
        sups_cmd_info_ptr->ss_code;

      mn_cmd_ptr->cmd.cm_interrogate_ss_req.basic_service =
        sups_cmd_info_ptr->basic_service;

      mn_cmd_ptr->cmd.cm_interrogate_ss_req.as_id = asubs_id;

      /* send the command to MN */
      cm_mn_send_cmd(mn_cmd_ptr);

      /* save a copy in the sups object and let the clients know */
      cmwsups_cmd_copy_fields_1(sups_cmd_info_ptr);

      cmsups_event(asubs_id, CM_SUPS_EVENT_INTERROGATE);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*-----------------------------*/
    /*  Register Password request  */
    /*-----------------------------*/

    case CM_SUPS_CMD_REG_PASSWORD:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_REGISTER_PASSWORD_REQ);

      /* Copy command information fields. */

      /* ss reference */
      mn_cmd_ptr->cmd.cm_register_password_req.cm_ss_ref =
        sups_cmd_info_ptr->ss_ref;

      mn_cmd_ptr->cmd.cm_register_password_req.ss_code =
        sups_cmd_info_ptr->code;

      mn_cmd_ptr->cmd.cm_register_password_req.ss_password =
        sups_cmd_info_ptr->ss_password;

      mn_cmd_ptr->cmd.cm_register_password_req.ss_new_password =
        sups_cmd_info_ptr->ss_new_password;

      mn_cmd_ptr->cmd.cm_register_password_req.ss_new_password_again =
        sups_cmd_info_ptr->ss_new_password_again;

      mn_cmd_ptr->cmd.cm_register_password_req.as_id = asubs_id;

      /* send the command to MN */
      cm_mn_send_cmd(mn_cmd_ptr);

      /* save a copy in the sups object and let the clients know */
      cmwsups_cmd_copy_fields_1(sups_cmd_info_ptr);

      cmsups_event(asubs_id, CM_SUPS_EVENT_REG_PASSWORD);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*-----------------------------*/
    /*    Process Unstructured SS  */
    /*-----------------------------*/

    case CM_SUPS_CMD_PROCESS_USS:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_PROCESS_UNSTRUCTURED_SS_REQ);

      /* Copy command information fields. */

      /* ss reference */
      mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.cm_ss_ref =
        sups_cmd_info_ptr->ss_ref;

      /* ss operation */
      mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.ss_operation =
        sups_cmd_info_ptr->ss_operation;

      CM_MSG_HIGH_2("CM_SUPS_CMD_PROCESS_USS Size: %d bytes, DCC: %d",
                    sups_cmd_info_ptr->uss_data.size,
                    sups_cmd_info_ptr->uss_data.uss_data_coding_scheme);

      calculated_size = sups_cmd_info_ptr->uss_data.size;

      /*lint -save -e641 Converting enum to int */
      if (((sups_cmd_info_ptr->uss_data.uss_data_coding_scheme &
            CM_USSD_ALPHABET_MASK) == CM_USSD_DCS_7_BIT)
          ||
          ((sups_cmd_info_ptr->uss_data.uss_data_coding_scheme &
            CM_USSD_ALPHABET_MASK) == CM_USSD_DCS_UNSPECIFIED)
         )
      {
        /* MAX_USS_CHAR is what MN can support
        */
        if (sups_cmd_info_ptr->uss_data.size > MAX_USS_CHAR_ASCII)
        {
          sups_cmd_info_ptr->uss_data.size = MAX_USS_CHAR_ASCII;
          CM_MSG_HIGH_1("Truncating USSD msg to %d bytes", MAX_USS_CHAR);
        }

        /* convert to the default alphabet */
        num_bytes = cm_util_ascii_to_gsm_alphabet(
                      mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.uss_data.ussData,
                      sups_cmd_info_ptr->uss_data.ussData,
                      sups_cmd_info_ptr->uss_data.size);

        calculated_size = num_bytes;
      }
      /* Usage of CM_SUPS_CMD_PROCESS_USS has been to encode ussd data in CM
      ** and then send. EXCEPTION is being made to UCS2 where data being
      ** received is in encoded form. It is a TEMPORARY fix and needs to be
      ** changed with a new field that says if encoding has already been done.
      */
      else if (((sups_cmd_info_ptr->uss_data.uss_data_coding_scheme &
                 CM_USSD_ALPHABET_MASK) == CM_USSD_DCS_8_BIT)
               ||
               ((sups_cmd_info_ptr->uss_data.uss_data_coding_scheme &
                 CM_USSD_ALPHABET_MASK) == CM_USSD_DCS_UCS2)
              )

      {
        if (calculated_size > MAX_USS_BYTES)
        {
          calculated_size = MAX_USS_BYTES;
          CM_MSG_HIGH_1("Truncating USSD msg to %d bytes", MAX_USS_BYTES);
        }

        memscpy(mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.uss_data.ussData,
                sizeof(mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.uss_data.ussData),
                sups_cmd_info_ptr->uss_data.ussData,
                calculated_size);
      }
      else
      {
        CM_ERR_0("USSD Data coding scheme error");
      }
      /*lint -restore 641 Converting enum to int */

      mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.uss_data.size =
        calculated_size;

      mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.
      uss_data.uss_data_coding_scheme =
        sups_cmd_info_ptr->uss_data.uss_data_coding_scheme;

      mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.uss_data.present = TRUE;
      sups_cmd_info_ptr->uss_data.present = TRUE;

      /* also save a copy of the USSD in case there is a phase
      compatibility problem */

      memset(uss_phase2_msg_ptr, 0, sizeof(cm_process_unstructured_ss_req_T));

      /* Copy ss ref */
      uss_phase2_msg_ptr->cm_ss_ref =
        mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.cm_ss_ref;

      /* Copy ss operation */
      uss_phase2_msg_ptr->ss_operation =
        mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.ss_operation;

      /* Copy ss data */
      *(&(uss_phase2_msg_ptr->uss_data)) = *(&(mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.uss_data));

      mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.as_id = asubs_id;

      /* send the command to MN */
      cm_mn_send_cmd(mn_cmd_ptr);
      CM_MSG_HIGH_4("uss data sent to clients and NAS. present:%d coding scheme: %d size: %d uss data: %s",
                    sups_cmd_info_ptr->uss_data.present,
                    sups_cmd_info_ptr->uss_data.uss_data_coding_scheme,
                    calculated_size, sups_cmd_info_ptr->uss_data.ussData);

      /* save a copy in the sups object and let the clients know */
      cmwsups_cmd_copy_fields_1(sups_cmd_info_ptr);

      cmsups_event(asubs_id, CM_SUPS_EVENT_PROCESS_USS);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*-------------------------*/
    /*    ReleaseSS request    */
    /*-------------------------*/

    case CM_SUPS_CMD_RELEASE:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_RELEASE_SS_REQ);

      /* Copy command information fields. */

      mn_cmd_ptr->cmd.cm_release_ss_req.cm_ss_ref =
        sups_cmd_info_ptr->invoke_id;

      mn_cmd_ptr->cmd.cm_release_ss_req.ss_operation =
        sups_cmd_info_ptr->ss_operation;

      mn_cmd_ptr->cmd.cm_release_ss_req.as_id = asubs_id;

      /* send the command to MN */
      cm_mn_send_cmd(mn_cmd_ptr);

      /* save a copy in the sups object and let the clients know */
      cmwsups_cmd_copy_fields_1(sups_cmd_info_ptr);

      cmsups_event(asubs_id, CM_SUPS_EVENT_RELEASE);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*-------------------------*/
    /*   Get Password response */
    /*-------------------------*/

    case CM_SUPS_CMD_GET_PASSWORD_RES:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_GET_PASSWORD_RES);

      mn_cmd_ptr->cmd.cm_get_password_res.invoke_id =
        sups_cmd_info_ptr->invoke_id;

      mn_cmd_ptr->cmd.cm_get_password_res.ss_password =
        sups_cmd_info_ptr->ss_password;

      mn_cmd_ptr->cmd.cm_get_password_res.as_id = asubs_id;

      /* send the command to MN */
      cm_mn_send_cmd(mn_cmd_ptr);

      /* save a copy in the sups object and let the clients know */
      cmwsups_cmd_copy_fields_1(sups_cmd_info_ptr);

      cmsups_event(asubs_id, CM_SUPS_EVENT_GET_PASSWORD_RES);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*----------------------------*/
    /*  Unstructured SS response  */
    /*----------------------------*/

    case CM_SUPS_CMD_USS_RES:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_UNSTRUCTURED_SS_RES);

      /* Copy command information fields. */
      mn_cmd_ptr->cmd.cm_unstructured_ss_res.invoke_id =
        sups_cmd_info_ptr->invoke_id;

      if (sups_cmd_info_ptr->uss_res_type == CM_USS_RES_SS_ERROR)
      {
        mn_cmd_ptr->cmd.cm_unstructured_ss_res.uss_res_data.data.cm_ss_error =
          sups_cmd_info_ptr->ss_error;
        mn_cmd_ptr->cmd.cm_unstructured_ss_res.uss_res_data.data_type = SS_ERROR_INFO;
      }
      else if (sups_cmd_info_ptr->uss_res_type == CM_USS_RES_SS_DATA)
      {
        mn_cmd_ptr->cmd.cm_unstructured_ss_res.uss_res_data.data.uss_data =
          sups_cmd_info_ptr->uss_data;

        mn_cmd_ptr->cmd.cm_unstructured_ss_res.uss_res_data.data_type = USS_DATA_INFO;
      }

      mn_cmd_ptr->cmd.cm_unstructured_ss_res.as_id = asubs_id;

      /* send the command to MN */
      cm_mn_send_cmd(mn_cmd_ptr);

      /* save a copy in the sups object and let the clients know */
      cmwsups_cmd_copy_fields_1(sups_cmd_info_ptr);

      cmsups_event(asubs_id, CM_SUPS_EVENT_USS_RES);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*-----------------------------------*/
    /*  Unstructured SS Notify response  */
    /*-----------------------------------*/

    case CM_SUPS_CMD_USS_NOTIFY_RES:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_UNSTRUCTURED_SS_NOTIFY_RES);

      /* Copy command information fields. */
      mn_cmd_ptr->cmd.cm_unstructured_ss_notify_res.invoke_id =
        sups_cmd_info_ptr->invoke_id;

      if (sups_cmd_info_ptr->uss_res_type == CM_USS_RES_SS_ERROR)
      {
        mn_cmd_ptr->cmd.cm_unstructured_ss_notify_res.cm_ss_error =
          sups_cmd_info_ptr->ss_error;
      }

      mn_cmd_ptr->cmd.cm_unstructured_ss_notify_res.as_id = asubs_id;

      /* send the command to MN */
      cm_mn_send_cmd(mn_cmd_ptr);

      cmsups_reset_mt_uss_context_info(asubs_id);
      
      /* save a copy in the sups object and let the clients know */
      cmwsups_cmd_copy_fields_1(sups_cmd_info_ptr);

      cmsups_event(asubs_id, CM_SUPS_EVENT_USS_NOTIFY_RES);

      break;

#if (defined(FEATURE_CM_MMGPS) || defined(FEATURE_CM_LCS))
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*-----------------------------*/
    /*      lcs-MOLR request       */
    /*-----------------------------*/

    case CM_SUPS_CMD_LCS_MOLR:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_LCS_MOLR_REQ);

      /* Copy command information fields. */

      /* ss reference */
      mn_cmd_ptr->cmd.cm_lcs_molr_req.cm_ss_ref =
        sups_cmd_info_ptr->ss_ref;

      mn_cmd_ptr->cmd.cm_lcs_molr_req.molr_type =
        sups_cmd_info_ptr->molr_type;

      mn_cmd_ptr->cmd.cm_lcs_molr_req.location_method =
        sups_cmd_info_ptr->location_method;

      mn_cmd_ptr->cmd.cm_lcs_molr_req.lcs_qos =
        sups_cmd_info_ptr->lcs_qos;

      mn_cmd_ptr->cmd.cm_lcs_molr_req.external_id =
        sups_cmd_info_ptr->external_id;

      mn_cmd_ptr->cmd.cm_lcs_molr_req.mlc_number =
        sups_cmd_info_ptr->mlc_number;

      mn_cmd_ptr->cmd.cm_lcs_molr_req.gps_assistance_data =
        sups_cmd_info_ptr->gps_assistance_data;

      mn_cmd_ptr->cmd.cm_lcs_molr_req.supported_gad_shapes =
        sups_cmd_info_ptr->supported_gad_shapes;

      mn_cmd_ptr->cmd.cm_lcs_molr_req.as_id = asubs_id;

      /* send the command to MN */
      cm_mn_send_cmd(mn_cmd_ptr);

      /* save a copy in the sups object and let the clients know */
      cmwsups_cmd_copy_fields_1(sups_cmd_info_ptr);

      cmsups_event(asubs_id, CM_SUPS_EVENT_LCS_MOLR);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*-----------------------------*/
    /*      lcs-MOLR completed     */
    /*-----------------------------*/

    case CM_SUPS_CMD_LCS_MOLR_COMPLETED:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_LCS_MOLR_COMPLETED);

      /* Copy command information fields. */

      /* ss reference */
      mn_cmd_ptr->cmd.cm_lcs_molr_completed.cm_ss_ref =
        sups_cmd_info_ptr->ss_ref;

      mn_cmd_ptr->cmd.cm_lcs_molr_completed.as_id = asubs_id;

      /* send the command to MN */
      cm_mn_send_cmd(mn_cmd_ptr);

      /* save a copy in the sups object and let the clients know */
      cmwsups_cmd_copy_fields_1(sups_cmd_info_ptr);

      cmsups_event(asubs_id, CM_SUPS_EVENT_LCS_MOLR_COMPLETED);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*--------------------------------------*/
    /*  LCS Location Notification response  */
    /*--------------------------------------*/

    case CM_SUPS_CMD_LCS_LOCATION_NOTIFICATION_RES:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_LCS_LOCATION_NOTIFICATION_RES);

      mn_cmd_ptr->cmd.cm_lcs_location_notification_res.invoke_id =
        sups_cmd_info_ptr->invoke_id;

      mn_cmd_ptr->cmd.cm_lcs_location_notification_res.location_notification_res =
        sups_cmd_info_ptr->location_notification_res;

      mn_cmd_ptr->cmd.cm_lcs_location_notification_res.as_id = asubs_id;

      /* send the command to MN */
      cm_mn_send_cmd(mn_cmd_ptr);

      /* save a copy in the sups object and let the clients know */
      cmwsups_cmd_copy_fields_1(sups_cmd_info_ptr);

      cmsups_event(asubs_id, CM_SUPS_EVENT_LCS_LOCATION_NOTIFICATION_RES);

      break;

#endif

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*-------------------------*/
    /*    EraseCCEntry request   */
    /*-------------------------*/

    case CM_SUPS_CMD_ERASE_CC_ENTRY:

#if defined(FEATURE_CCBS)
      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_ERASE_CC_ENTRY_REQ);

      /* ss reference */
      mn_cmd_ptr->cmd.cm_erase_cc_entry_req.cm_ss_ref
        = sups_cmd_info_ptr->ss_ref;

      /* ss code */
      mn_cmd_ptr->cmd.cm_erase_cc_entry_req.ss_code = sups_cmd_info_ptr->ss_code;

      /* Deactivate one or all ccbs indx requests
      ** CM_CCBS_INDX_INVALID - maps to deactivating all requests
      ** ccbs_index is valid from 1 to 5 (22.030 Table B.1) ,
      */
      if (sups_cmd_info_ptr->ccbs_indx == CM_CCBS_INDX_INVALID)
      {
        mn_cmd_ptr->cmd.cm_erase_cc_entry_req.ccbs_index.present
          = FALSE;

        CM_MSG_HIGH_0("ERASE_CC_ENTRY for all ccbs indexes");
      }
      else
      {
        mn_cmd_ptr->cmd.cm_erase_cc_entry_req.ccbs_index.present
          = TRUE;

        mn_cmd_ptr->cmd.cm_erase_cc_entry_req.ccbs_index.ccbs_Index
          = sups_cmd_info_ptr->ccbs_indx;

        CM_MSG_HIGH_1("ERASE_CC_ENTRY for ccbs index",
                      sups_cmd_info_ptr->ccbs_indx);
      }

      CM_MSG_HIGH_1("is_erase_cc_conf_pend flag = %d",
                    cmsups_sub_info_ptr->ccbs_erase_pend_indx.is_erase_cc_conf_pend);

      /* Save the ccbs indx requested for deactivation to clear
      ** phone object ccbs indx store on confirmation
      */
      if (!cmsups_sub_info_ptr->ccbs_erase_pend_indx.is_erase_cc_conf_pend)
      {
        cmsups_sub_info_ptr->ccbs_erase_pend_indx.is_erase_cc_conf_pend = TRUE;
        cmsups_sub_info_ptr->ccbs_erase_pend_indx.cm_ss_ref = sups_cmd_info_ptr->ss_ref;
        cmsups_sub_info_ptr->ccbs_erase_pend_indx.ccbs_indx = sups_cmd_info_ptr->ccbs_indx;
      }
      else
      {
        CM_ERR_0("Simult ccbs deact requests arrived");
      }

      mn_cmd_ptr->cmd.cm_erase_cc_entry_req.as_id = asubs_id;

      /* send the command to MN */
      cm_mn_send_cmd(mn_cmd_ptr);

      /* save a copy in the sups object and let the clients know */
      cmwsups_cmd_copy_fields_1(sups_cmd_info_ptr);

      /*
      ** if ss_code is ccbs then deactivation gets converted into EraseCCEntry
      ** request (24.093 section 4.4).
      ** Since client sends a deactivation request, deactivate sups event is
      ** sent.
      */
      cmsups_event(asubs_id, CM_SUPS_EVENT_DEACTIVATE);
#endif

      break;

    default:
      CM_ERR_1("Unknown SUPS command %d for RETRY",
               sups_info_list_ptr->cmd);
      break;

  } /* end switch(sups_cmd_ptr->cmd )... */
}

#endif

/*===========================================================================

FUNCTION cmsups_create_interrogate_cnf_internal

DESCRIPTION
return interrogate cnf if UE based call waiting is used. local NV value is returned. no need to send to
lower layers

RETURN VALUE

SIDE EFFECTS
none

===========================================================================*/
void cmsups_create_interrogate_cnf_internal(
  cm_sups_cmd_info_s_type   *sups_cmd_info_ptr,
  byte                   ss_code
)
{
  cm_rpt_type                *cm_rpt_ptr = NULL;
  cm_interrogate_ss_conf_T   *interrogate_ss_conf = NULL;
  boolean                     is_ue_sups_enabled;
  sys_modem_as_id_e_type      asubs_id = sups_cmd_info_ptr->cmd_subs_id;
  cm_ue_based_sups_config_s_type     cm_ue_based_sups_config;


  cm_ue_based_sups_config = cmsups_map_ss_code_to_sups_config(sups_cmd_info_ptr->ss_code,asubs_id);
/* If ue_based_sups is configured, we don't need to do any sups cmd check */
  is_ue_sups_enabled= cm_ue_based_sups_config.is_enabled;
     
  switch(ss_code)
  {
    case cw:
    {
      cm_rpt_ptr = cm_get_cmd_buf();
      interrogate_ss_conf = &cm_rpt_ptr->cmd.interrogate_conf;
      cm_rpt_ptr->hdr.cmd = CM_INTERROGATE_SS_CONF;
      interrogate_ss_conf->as_id = asubs_id;
      interrogate_ss_conf->cm_ss_ref = sups_cmd_info_ptr->ss_ref;
      interrogate_ss_conf->success = TRUE;
      if(is_ue_sups_enabled)
      {
        interrogate_ss_conf->res_type = BSG_LIST_INFO;
        interrogate_ss_conf->InterrogateSS_Res.basic_service_group_list.present = TRUE;
        interrogate_ss_conf->InterrogateSS_Res.basic_service_group_list.list_length = 1;
        interrogate_ss_conf->InterrogateSS_Res.basic_service_group_list.list[0].bs_code = allSpeechTransmissionservices;
        interrogate_ss_conf->InterrogateSS_Res.basic_service_group_list.list[0].code_type = TS_CODE;
        interrogate_ss_conf->InterrogateSS_Res.basic_service_group_list.list[0].present = TRUE;
      }
      else
      {
        interrogate_ss_conf->res_type = SS_STATUS_INFO;
        interrogate_ss_conf->InterrogateSS_Res.cm_ss_status.present = TRUE;
        interrogate_ss_conf->InterrogateSS_Res.cm_ss_status.cm_ss_active = is_ue_sups_enabled;
      }
      CM_MSG_HIGH_4("res_type=%d, ue_sups=%d, ss_ref=%d ss_code=%d",
                interrogate_ss_conf->res_type,
                is_ue_sups_enabled,
                interrogate_ss_conf->cm_ss_ref,
                sups_cmd_info_ptr->ss_code);
      cm_put_cmd(cm_rpt_ptr);
      break;
    }
    case colr:
  case allCallRestrictionSS:
  case barringOfOutgoingCalls:
  case baoc:
  case boic:
  case boicExHC:
  case barringOfIncomingCalls:
  case baic:
  case bicRoam:
  case bain:  
    {
      cmipapp_ind_s_type          ipapp_ind;
      memset(&ipapp_ind,0,sizeof(cmipapp_ind_s_type));
      ipapp_ind.ind.interrogate_ss_conf.success = TRUE;
      ipapp_ind.ind.interrogate_ss_conf.cm_ss_ref = sups_cmd_info_ptr->ss_ref;;
      ipapp_ind.ind.interrogate_ss_conf.res_type = SS_STATUS_INFO;
	  ipapp_ind.hdr.asubs_id = asubs_id;
      if(is_ue_sups_enabled)
      {
       ipapp_ind.ind.interrogate_ss_conf.interrogate_ss_res.cm_ss_status.cm_ss_active = TRUE;
       ipapp_ind.ind.interrogate_ss_conf.interrogate_ss_res.cm_ss_status.cm_ss_provisioned = TRUE;
       ipapp_ind.ind.interrogate_ss_conf.interrogate_ss_res.cm_ss_status.cm_ss_registered = TRUE;
       ipapp_ind.ind.interrogate_ss_conf.interrogate_ss_res.cm_ss_status.present = TRUE;
       ipapp_ind.ind.interrogate_ss_conf.interrogate_ss_res.pi = CM_PRESENTATION_RESTRICTED;
       }
       else
       {
       ipapp_ind.ind.interrogate_ss_conf.interrogate_ss_res.cm_ss_status.cm_ss_active = FALSE;
       ipapp_ind.ind.interrogate_ss_conf.interrogate_ss_res.cm_ss_status.cm_ss_provisioned = TRUE;
       ipapp_ind.ind.interrogate_ss_conf.interrogate_ss_res.cm_ss_status.cm_ss_registered = FALSE;
       ipapp_ind.ind.interrogate_ss_conf.interrogate_ss_res.cm_ss_status.present = TRUE;
       ipapp_ind.ind.interrogate_ss_conf.interrogate_ss_res.pi = CM_PRESENTATION_ALLOWED;
       }
       CM_MSG_HIGH_4("ss_active=%d, ss_ref=%d, pi=%d ss_code=%d",
          ipapp_ind.ind.interrogate_ss_conf.interrogate_ss_res.cm_ss_status.cm_ss_active,
          ipapp_ind.ind.interrogate_ss_conf.cm_ss_ref,
          ipapp_ind.ind.interrogate_ss_conf.interrogate_ss_res.pi,
          sups_cmd_info_ptr->ss_code);
       cmipapp_rpt_ind (CM_IP_SUPS_INTERROGATE_SS_CONF, ipapp_ind, 0);
       break;
    }
  }
  return;
}

/*===========================================================================

FUNCTION cmsups_create_activate_cnf_internal

DESCRIPTION
activate Call waiting when UE based CW is enabled

RETURN VALUE

SIDE EFFECTS
none

===========================================================================*/
void cmsups_create_activate_cnf_internal(
  cm_sups_cmd_info_s_type   *sups_cmd_info_ptr
)
{
  cm_rpt_type             *cm_rpt_ptr = NULL;
  sys_modem_as_id_e_type   asubs_id = sups_cmd_info_ptr->cmd_subs_id;
  volte_sub_info_s_type   *volte_sub_info_ptr = cmph_get_volte_sub_info_ptr(asubs_id);

  CM_MSG_HIGH_1("cmsups_create_activate_cnf_internal, sub %d", asubs_id);

  cmwsups_update_ue_based_sups(sups_cmd_info_ptr->ss_code,TRUE, asubs_id);


  cm_rpt_ptr = cm_get_cmd_buf();
  cm_rpt_ptr->hdr.cmd = CM_ACTIVATE_SS_CONF;
  cm_rpt_ptr->cmd.activate_conf.as_id = asubs_id;
  cm_rpt_ptr->cmd.activate_conf.success = TRUE;
  cm_rpt_ptr->cmd.activate_conf.cm_ss_ref = sups_cmd_info_ptr->ss_ref;
  cm_rpt_ptr->cmd.activate_conf.ss_info.info_type = NO_INFO;

  cm_put_cmd(cm_rpt_ptr);
  return;
}

/*===========================================================================

FUNCTION cmsups_create_deactivate_cnf_internal

DESCRIPTION
deactivate Call waiting when UE based CW is enabled

RETURN VALUE

SIDE EFFECTS
none

===========================================================================*/
void cmsups_create_deactivate_cnf_internal(
  cm_sups_cmd_info_s_type   *sups_cmd_info_ptr
)
{
  cm_rpt_type             *cm_rpt_ptr = NULL;
  sys_modem_as_id_e_type   asubs_id = sups_cmd_info_ptr->cmd_subs_id;
  volte_sub_info_s_type   *volte_sub_info_ptr = cmph_get_volte_sub_info_ptr(asubs_id);

  CM_MSG_HIGH_1("cmsups_create_deactivate_cnf_internal, sub %d", asubs_id);

  cmwsups_update_ue_based_sups(sups_cmd_info_ptr->ss_code,FALSE, asubs_id);


  cm_rpt_ptr = cm_get_cmd_buf();
  cm_rpt_ptr->hdr.cmd = CM_DEACTIVATE_SS_CONF;
  cm_rpt_ptr->cmd.activate_conf.as_id = asubs_id;
  cm_rpt_ptr->cmd.activate_conf.success = TRUE;
  cm_rpt_ptr->cmd.activate_conf.cm_ss_ref = sups_cmd_info_ptr->ss_ref;
  cm_rpt_ptr->cmd.activate_conf.ss_info.info_type = NO_INFO;

  cm_put_cmd(cm_rpt_ptr);
  return;
}

/*===========================================================================

FUNCTION cmsups_reject_oir_clir_interrogate_cnf_internal

DESCRIPTION
Reject CLIR/ OIR impersonating lower layer by sending SS error Facility Not supported

RETURN VALUE
NULL;
SIDE EFFECTS
none

===========================================================================*/
static void cmsups_reject_oir_clir_interrogate_cnf_internal(cm_sups_cmd_info_s_type *sups_cmd_info_ptr)
{
  cm_rpt_type               *cm_rpt_ptr = NULL;
  cm_interrogate_ss_conf_T  *interrogate_ss_conf = NULL;
  sys_modem_as_id_e_type    asubs_id = sups_cmd_info_ptr->cmd_subs_id;

  cm_rpt_ptr = cm_get_cmd_buf();

  CM_MSG_HIGH_1("cmsups_reject_oir_clir_interrogate_cnf_internal, sub %d", asubs_id);

  interrogate_ss_conf = &cm_rpt_ptr->cmd.interrogate_conf;

  memset(interrogate_ss_conf, 0x00, sizeof(cm_interrogate_ss_conf_T));

  cm_rpt_ptr->hdr.cmd = CM_INTERROGATE_SS_CONF;
  interrogate_ss_conf->as_id = asubs_id;
  interrogate_ss_conf->cm_ss_ref = sups_cmd_info_ptr->ss_ref;
  interrogate_ss_conf->success = TRUE;

  interrogate_ss_conf->res_type = SS_ERROR_INFO;

  interrogate_ss_conf->InterrogateSS_Res.cm_ss_error.present = TRUE;
  interrogate_ss_conf->InterrogateSS_Res.cm_ss_error.error_code_tag = ERROR_CODE_TAG;
  interrogate_ss_conf->InterrogateSS_Res.cm_ss_error.error_code = facilityNotSupported;
  interrogate_ss_conf->InterrogateSS_Res.cm_ss_error.error_type = NO_ERR;

  cm_put_cmd(cm_rpt_ptr);

  return;
} /* cmsups_reject_oir_clir_interrogate_cnf_internal () */


/*===========================================================================

FUNCTION cmsups_is_reject_oir_clir_interrogate_cmd

DESCRIPTION
This function checks if Interrogate OIR/ CLIR command should be rejected
for further processing or not. If rejected there should be No signalling
to the NW

RETURN VALUE
TRUE/ FALSE based on reject value;

SIDE EFFECTS
none

===========================================================================*/
static boolean cmsups_is_reject_oir_clir_interrogate_cmd(
  sys_modem_as_id_e_type  asubs_id,
  cmsups_s_type          *sups_ptr,
  cm_sups_cmd_s_type     *sups_cmd_ptr
)
{
  boolean                  do_reject_oir_clir = FALSE;
  cm_sups_cmd_info_s_type *sups_cmd_info_ptr = SUPS_CMD_INFO_PTR(sups_cmd_ptr);
  if (cmsups_ptr()->sub_info[sups_cmd_info_ptr->cmd_subs_id] == NULL)
  {
    return TRUE;
  }

  if (sups_cmd_info_ptr->ss_code == clir && sups_cmd_ptr->cmd == CM_SUPS_CMD_INTERROGATE)
  {
    cmsups_is_reject_oir_clir_e_type is_reject_oir_clir;
    is_reject_oir_clir = cmsups_get_reject_interrogate_clir_oir_pref(sups_cmd_info_ptr->cmd_subs_id);

    switch (cmsups_ptr()->sub_info[sups_cmd_info_ptr->cmd_subs_id]->sups_cmd_route)
    {
      case CMSUPS_CMD_ALLOW_IMS:
        if (is_reject_oir_clir == CMSUPS_SS_REJECT_OIR ||
            is_reject_oir_clir == CMSUPS_SS_REJECT_OIR_CLIR)
        {
          do_reject_oir_clir = TRUE;
        }
        break;
      case CMSUPS_CMD_ALLOW_CS:
        if (is_reject_oir_clir == CMSUPS_SS_REJECT_CLIR ||
            is_reject_oir_clir == CMSUPS_SS_REJECT_OIR_CLIR)
        {
          do_reject_oir_clir = TRUE;
        }
        break;
      default:
        break;
    }
    if (do_reject_oir_clir)
    {
      cmsups_cmd_copy_fields(sups_cmd_ptr);
      cmsups_event(asubs_id, CM_SUPS_EVENT_INTERROGATE);
      cmsups_reject_oir_clir_interrogate_cnf_internal(sups_cmd_info_ptr);
    }
  }
  return do_reject_oir_clir;
} /* cmsups_is_reject_oir_clir_interrogate_cmd () */

/*===========================================================================

FUNCTION cmsups_free_sups

DESCRIPTION
Delete the recently added subs from the list

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
static void cmsups_free_sups(
  sys_modem_as_id_e_type  asubs_id
)
{
  cmsups_sub_info_s_type   *cmsups_sub_info_ptr = NULL;

  /* free the memory */
  cmsups_mem_free_sups_info(asubs_id);

#ifdef FEATURE_SUPS_RETRY
  cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

    if (cmsups_sub_info_ptr != NULL && cmsups_sub_info_ptr->sups_info_list != NULL)
    {
      cmwsups_mem_free_sups_list_info(cmsups_sub_info_ptr->sups_info_list,
                                      &(cmsups_sub_info_ptr->sups_info_list_head));
      cmsups_sub_info_ptr->sups_info_list = NULL;
    }
#endif
}

/*===========================================================================

FUNCTION cmwsups_set_prev_ue_based_sups

DESCRIPTION
To set the sups setting while camped on CS if ue_based_sups is enabled.

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
void cmwsups_set_prev_ue_based_sups(
  byte                   ss_code,
  boolean                is_enabled,
  sys_modem_as_id_e_type asubs_id,
  boolean                update_efs
)
{

  cmsups_sub_info_s_type    *cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);
  uint8 i = CMEFS_UE_BASED_SUPS_ARRAY_USED;

  if(cmsups_sub_info_ptr == NULL)
  {
    return;
  }
  CM_MSG_HIGH_4("cmwsups_set_prev_ue_based_sups, ss_code %d sub %d is_enabled = %d update_efs = %d",
                   ss_code, asubs_id, is_enabled, update_efs);
    
  i = cmsups_map_code_to_index(ss_code);
  if(i<=CMEFS_UE_BASED_SUPS_ARRAY_USED)
  {
    cmsups_sub_info_ptr->prev_ue_based_sups_enabled[i] = is_enabled;
  }

  if (update_efs == FALSE)
  {
    return;
  }
  
  if (ss_code != cw)
  {
    memscpy(&cmsups_sub_info_ptr->sups_enable_status_nw.sups_enable_status_nw,
            (sizeof(boolean) * CMEFS_UE_BASED_SUPS_ARRAY_SIZE),
            &cmsups_sub_info_ptr->prev_ue_based_sups_enabled[i],
            (sizeof(boolean) * CMEFS_UE_BASED_SUPS_ARRAY_SIZE));
    
    cmefs_fs_write(CMEFS_SUPS_ENABLE_STATUS_NW,
                   (byte *)&(cmsups_sub_info_ptr->sups_enable_status_nw),
                   (int32)sizeof(cm_sups_enable_status_nw_s_type),
                   asubs_id);
  }
  else
  {
    cmsups_sub_info_ptr->ext_cw_config_info.cw_nw_status = is_enabled;
    cmefs_fs_write(CMEFS_CW_EXT_CONFIG_INFO,
                   (byte *)&(cmsups_sub_info_ptr->ext_cw_config_info),
                   (int32)sizeof(cm_sups_cw_config_info_s_type),
                   asubs_id);
  }
  
}

/*===========================================================================

FUNCTION cmwsups_set_prev_ue_based_cw

DESCRIPTION
To store the sups setting which can be used across mcfg refresh / power cycle.

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
void cmwsups_set_prev_ue_based_cw(
  boolean                is_enabled,
  sys_modem_as_id_e_type asubs_id
)
{
  cmsups_sub_info_s_type    *cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

  if(cmsups_sub_info_ptr == NULL)
  {
    return;
  }
    CM_MSG_HIGH_2("cmwsups_set_prev_ue_based_cw, sub %d is_enabled = %d",
                    asubs_id, is_enabled);

    if(cmsups_sub_info_ptr->prev_ue_based_cw != is_enabled)
    {
      cmsups_sub_info_ptr->prev_ue_based_cw = is_enabled;
      cmefs_fs_write(CMEFS_PREV_UE_BASED_CW,
                   (byte *)&(cmsups_sub_info_ptr->prev_ue_based_cw),
                   (int32)sizeof(boolean),
                   asubs_id);
    }
}

/*===========================================================================

FUNCTION cmsups_client_cmd_forward_ll

DESCRIPTION
Forward a client sups commands to MN

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
static void cmsups_client_cmd_forward_ll(
  cmsups_s_type  *sups_ptr,
  /* Pointer to the sups object */

  cm_sups_cmd_s_type    *sups_cmd_ptr
  /* Pointer to a CM command */
)
{
  cm_sups_cmd_info_s_type   *sups_cmd_info_ptr;


  sys_modem_as_id_e_type     asubs_id = sups_cmd_ptr->cmd_info.cmd_subs_id;
  multimode_stack_e_type     stack_id = MM_STACK_0;

  cmsups_sub_info_s_type    *cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

  cm_sups_event_e_type       sups_event_type = CM_SUPS_EVENT_NONE;
  volte_sub_info_s_type     *volte_sub_info_ptr = cmph_get_volte_sub_info_ptr(asubs_id);
  cm_ue_based_sups_config_s_type     cm_ue_based_sups_config;

#ifdef FEATURE_MMODE_DUAL_SIM
  sys_modem_as_id_e_type     other_as_id = SYS_MODEM_AS_ID_NONE;
  cmph_s_type               *ph_ptr = cmph_ptr();
#endif

  boolean                    is_cmd_sent_to_ims = TRUE;
  boolean                       is_ue_based_sups = FALSE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_ASSERT(sups_cmd_ptr != NULL);
  CM_ASSERT(sups_ptr != NULL);
  CM_ASSERT(sups_cmd_ptr->cmd_type == CM_CMD_TYPE_SUPS);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* sups command information pointer */
  sups_cmd_info_ptr = SUPS_CMD_INFO_PTR(sups_cmd_ptr);

  /* If use UE based call waiting, it needs to be returned stored value.
  ** there is no signaling to the network
  */

  /* Null check cmsups_sub_info_ptr */
  if (cmsups_sub_info_ptr == NULL)
  {
    CM_ERR_1("cmsups_client_cmd_forward_ll: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
             asubs_id);
    return;
  }
  cm_ue_based_sups_config = cmsups_map_ss_code_to_sups_config(sups_cmd_ptr->cmd_info.ss_code,asubs_id);
  is_ue_based_sups = cm_ue_based_sups_config.use_ue_based_sups;
  /* If ue_based_sups is configured, we don't need to do any sups cmd check */
  CM_MSG_HIGH_4("SUPS_FW_LL: is_ue_based_sups=%d, enabled=%d, ss_code=%d cw_sups_enable_opr_mask %d",
                is_ue_based_sups,
                cm_ue_based_sups_config.is_enabled,
                sups_cmd_info_ptr->ss_code,
                cmsups_sub_info_ptr->ext_cw_config_info.cw_sups_enable_opr_mask);


  if(is_ue_based_sups == TRUE)
  {
    /* If sync ue_based_cw with cs is enabled and UE is camped on CS
    ** 1. If user activates/deactivates CW , send OTA req to NAS and update CMEFS_ID_UE_BASED_CW
    ** 2. If user queries for CW setting, return value directly from modem
    */
    if(cm_ue_based_sups_config.sync_ue_based_sups_with_cs == TRUE &&
       ((cmwsups_is_ue_camped_in_cs(asubs_id) == TRUE) 
        || (cmsups_is_immediate_sups_sync_with_cs_required(asubs_id) == TRUE))
       )
    {
      switch (sups_cmd_ptr->cmd)
      {
        case CM_SUPS_CMD_ACTIVATE:
          cmsups_cmd_copy_fields(sups_cmd_ptr);
          cmwsups_setting_forward_gw(CM_ACTIVATE_SS_REQ, sups_cmd_info_ptr->ss_ref,sups_cmd_info_ptr->ss_code, asubs_id);
          cmsups_event(asubs_id, CM_SUPS_EVENT_ACTIVATE);
          cmwsups_update_ue_based_sups(sups_cmd_ptr->cmd_info.ss_code,TRUE, asubs_id);
          cmwsups_set_prev_ue_based_sups(sups_cmd_ptr->cmd_info.ss_code,TRUE, asubs_id, TRUE);
          break;
        case CM_SUPS_CMD_DEACTIVATE:
          cmsups_cmd_copy_fields(sups_cmd_ptr);
          cmwsups_setting_forward_gw(CM_DEACTIVATE_SS_REQ, sups_cmd_info_ptr->ss_ref,sups_cmd_info_ptr->ss_code, asubs_id);
          cmsups_event(asubs_id, CM_SUPS_EVENT_DEACTIVATE);
          cmwsups_update_ue_based_sups(sups_cmd_ptr->cmd_info.ss_code,FALSE, asubs_id);
          cmwsups_set_prev_ue_based_sups(sups_cmd_ptr->cmd_info.ss_code,FALSE, asubs_id, TRUE);
          break;
        case CM_SUPS_CMD_INTERROGATE:
          cmsups_cmd_copy_fields(sups_cmd_ptr);
          cmsups_event(asubs_id, CM_SUPS_EVENT_INTERROGATE);
          cmsups_create_interrogate_cnf_internal(sups_cmd_info_ptr,sups_cmd_ptr->cmd_info.ss_code);
          break;
        default:
          break;
      }
    }
    else
    {
    switch (sups_cmd_ptr->cmd)
    {
      case CM_SUPS_CMD_ACTIVATE:
        cmsups_cmd_copy_fields(sups_cmd_ptr);
        cmsups_event(asubs_id, CM_SUPS_EVENT_ACTIVATE);
        cmsups_create_activate_cnf_internal(sups_cmd_info_ptr);
        break;
      case CM_SUPS_CMD_DEACTIVATE:
        cmsups_cmd_copy_fields(sups_cmd_ptr);
        cmsups_event(asubs_id, CM_SUPS_EVENT_DEACTIVATE);
        cmsups_create_deactivate_cnf_internal(sups_cmd_info_ptr);
        break;
      case CM_SUPS_CMD_INTERROGATE:
        cmsups_cmd_copy_fields(sups_cmd_ptr);
        cmsups_event(asubs_id, CM_SUPS_EVENT_INTERROGATE);
          cmsups_create_interrogate_cnf_internal(sups_cmd_info_ptr,sups_cmd_ptr->cmd_info.ss_code);
        break;
      default:
        break;
    }
  }
    return;
}
  
  /* If OIR CLIR Interrogation reject NV is set to disallow Signaling then
  ** simulate NW reject from here and no need to forward comand to Lower layers
  */
  if (cmsups_is_reject_oir_clir_interrogate_cmd(asubs_id, sups_ptr, sups_cmd_ptr) == TRUE)
  {
    CM_MSG_HIGH_1("Reject CLIR/ OIR Interrogate command sent on sub %d", asubs_id);
    return;
  }

#ifdef FEATURE_MMODE_DUAL_SIM

  if ((cmph_is_dsds() || cmph_is_tsts()) &&
      ph_ptr->device_prop.msim_prop.internal_standby_pref != SYS_MODEM_DS_PREF_SINGLE_STANDBY)
  {
    cmss_misc_sub_prop_s_type *ss_misc_sub_ptr;
    other_as_id = cmph_get_other_sub(asubs_id);
    ss_misc_sub_ptr = cmss_get_misc_sub_prop_ptr(other_as_id);
    if (ss_misc_sub_ptr == NULL || !cmutil_is_as_id_valid(other_as_id))
    {
      return;
    }

    /* Check if the other sub should be data suspended
    */
    if (!cmph_is_dual_lte() &&
        ph_ptr->device_prop.msim_prop.default_data_subs == asubs_id)
    {
      CM_MSG_HIGH_1("SUPS_FW_LL: Not needed to suspend data on non-DDS sub %d in non-dual-LTE", other_as_id);
    }
    else if ((cmcall_is_there_a_call_type_per_sub(other_as_id, CM_CALL_TYPE_VOICE, NULL) != CM_CALL_ID_INVALID) ||
             (cmcall_is_there_a_call_type_per_sub(other_as_id, CM_CALL_TYPE_EMERGENCY, NULL) != CM_CALL_ID_INVALID) ||
             (cmcall_is_there_a_call_type_per_sub(other_as_id, CM_CALL_TYPE_CS_DATA, NULL) != CM_CALL_ID_INVALID))
    {
      CM_MSG_HIGH_1("SUPS_FW_LL: Voice/Emerg/CS data call active on other sub %d", other_as_id);
    }
    else if (sups_cmd_ptr->cmd == CM_SUPS_CMD_GET_PASSWORD_RES ||
             sups_cmd_ptr->cmd == CM_SUPS_CMD_USS_NOTIFY_RES ||
             sups_cmd_ptr->cmd == CM_SUPS_CMD_USS_RES ||
             sups_cmd_ptr->cmd == CM_SUPS_CMD_LCS_LOCATION_NOTIFICATION_RES ||
             sups_cmd_ptr->cmd == CM_SUPS_CMD_LCS_MOLR_COMPLETED)
    {
      CM_MSG_HIGH_2("SUPS_FW_LL: Not needed to suspend data on other sub %d for sups cmd %d", other_as_id, sups_cmd_ptr->cmd);
    }
    else
    {
      if (cmcall_is_there_a_call_type_per_sub(other_as_id, CM_CALL_TYPE_PS_DATA, NULL) != CM_CALL_ID_INVALID)
      {
        #if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
        if (!cmcall_end_active_1x_datacalls(other_as_id, TRUE))
        #endif
        {
          #ifdef FEATURE_UE_INITIATED_DORMANCY
          sm_cmd_type *sm_ptr = cm_sm_get_buf_else_err_fatal(SMREG_PS_SIGNALING_REL_REQ);
          sm_ptr->cmd.ps_signalling_rel_req.as_id = other_as_id;
          sm_ptr->cmd.ps_signalling_rel_req.cause = SMREG_DUAL_STANDBY_CS_MO_CALL;
          cm_sm_send_cmd(sm_ptr);
          CM_MSG_HIGH_1("SUPS_FW_LL: SMREG_PS_SIGNALING_REL_REQ sent on SubsId %d", other_as_id);
          #endif
        }
      }

      ss_misc_sub_ptr->sub_info.gen_sub_info.ue_init_ps_data_suspend = TRUE;
      cmss_process_data_suspend_rpt(
        TRUE, 
        cmph_map_sub_stk_to_id(
          other_as_id, 
          cmph_determine_stk_per_rat(other_as_id, SYS_SYS_MODE_LTE)));
    }
  }
#endif /* FEATURE_MMODE_DUAL_SIM */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef CM_DEBUG
  #error code not present
#endif
  /* If PS supp domain preference is PS_PREF and ICS interface is available, route SS
  ** command to IMS
  */
  if (cmsups_sub_info_ptr->sups_cmd_route == CMSUPS_CMD_ALLOW_IMS)
  {
#ifdef FEATURE_IP_CALL
    is_cmd_sent_to_ims =    cmipsups_client_cmd_forward_ip(sups_ptr, sups_cmd_ptr);
#endif
  }
  /* Check if cmd couldnt be sent to IMS, either APP not found or cmd not recognised,
  ** determine route again to check if it could be routed on CS
  */
  if(is_cmd_sent_to_ims == FALSE)
  {
#ifdef FEATURE_IP_CALL
    cmsups_sub_info_ptr->sups_cmd_route = cmipsups_route_check(sups_cmd_ptr->cmd, asubs_id);
#endif

    CM_MSG_HIGH_1("Sups cmd not sent to IMS, route calculates %d",cmsups_sub_info_ptr->sups_cmd_route);

    /* IF CS is not allowed per current config, reject sups cmd
    */
    if(cmsups_sub_info_ptr->sups_cmd_route == CMSUPS_CMD_REJECT)
    {
      CM_MSG_HIGH_0("CM_SUPS_CMD_ERR_ROUTE_FAILURE ");
      cmsups_client_cmd_err( sups_cmd_ptr, CM_SUPS_CMD_ERR_ROUTE_FAILURE );
      cmsups_free_sups(asubs_id);
      cmsups_free_uss_info_ptr(asubs_id);
      return;
    }
  }
  /* Either route could be initially determined as CS or
  ** CS as fallback option since cmd couldnt be routed to IMS
  */
  if (cmsups_sub_info_ptr->sups_cmd_route == CMSUPS_CMD_ALLOW_CS)
  {
    /* save a copy in the sups object and send the command to MN */
    cmsups_cmd_copy_fields(sups_cmd_ptr);
    /* Route to CS domain */
    switch (sups_cmd_ptr->cmd)
    {
      case CM_SUPS_CMD_LCS_MOLR:
      case CM_SUPS_CMD_LCS_MOLR_COMPLETED:
      case CM_SUPS_CMD_LCS_LOCATION_NOTIFICATION_RES:
        cmwsups_client_lcs_cmd_forward_gw(sups_cmd_info_ptr, sups_cmd_ptr->cmd);
        break;
      default:
        cmwsups_client_cmd_forward_gw(asubs_id, sups_cmd_ptr->cmd, FALSE);
        break;
    }

    sups_event_type = cmsups_cmd_to_event_map(sups_cmd_ptr->cmd);
    if (sups_event_type != CM_SUPS_EVENT_NONE)
    {
      cmsups_event(asubs_id, sups_event_type);
    }
  }

  return;
}

/*===========================================================================

FUNCTION cmwsups_send_release_ss_req

DESCRIPTION
Forward a SS release request to NAS with release cause

DEPENDENCIES
NONE

RETURN VALUE
NONE

SIDE EFFECTS
none

===========================================================================*/
static void cmwsups_send_release_ss_req(
  byte                     invoke_id,
  byte                     ss_operation,
  ie_cm_ss_error_T         cm_ss_error,

  sys_modem_as_id_e_type   asubs_id
)
{
  mn_cnm_cmd_type   *mn_cmd_ptr;
  /* allocate a buffer */
  mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_RELEASE_SS_REQ);

  /* Copy command information fields. */
  mn_cmd_ptr->cmd.cm_release_ss_req.cm_ss_ref = invoke_id;
  mn_cmd_ptr->cmd.cm_release_ss_req.ss_operation = ss_operation;
  mn_cmd_ptr->cmd.cm_release_ss_req.as_id = asubs_id;
  mn_cmd_ptr->cmd.cm_release_ss_req.cm_ss_error = cm_ss_error;

  /* send the command to MN */
  cm_mn_send_cmd(mn_cmd_ptr);
} /* cmwsups_send_release_ss_req () */

/*===========================================================================

FUNCTION cmwsups_client_lcs_cmd_forward_gw

DESCRIPTION
Forward a sups commands to MN.
This function will be called when route is calculated as CS
This Function will also be called when IMS sends fallback to CS.

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
NONE

SIDE EFFECTS
none

===========================================================================*/
void cmwsups_client_lcs_cmd_forward_gw(
  cm_sups_cmd_info_s_type   *sups_cmd_info_ptr,
  cm_sups_cmd_e_type              sups_cmd
)
{

  mn_cnm_cmd_type           *mn_cmd_ptr; /* MN command pointer */

  switch (sups_cmd)
  {
#if (defined(FEATURE_CM_MMGPS) || defined(FEATURE_CM_LCS))
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*-----------------------------*/
    /*      lcs-MOLR request       */
    /*-----------------------------*/

    case CM_SUPS_CMD_LCS_MOLR:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_LCS_MOLR_REQ);

      /* Copy command information fields. */

      /* ss reference */
      mn_cmd_ptr->cmd.cm_lcs_molr_req.cm_ss_ref =
        sups_cmd_info_ptr->ss_ref;

      mn_cmd_ptr->cmd.cm_lcs_molr_req.molr_type =
        sups_cmd_info_ptr->molr_type;

      mn_cmd_ptr->cmd.cm_lcs_molr_req.location_method =
        sups_cmd_info_ptr->location_method;

      mn_cmd_ptr->cmd.cm_lcs_molr_req.lcs_qos =
        sups_cmd_info_ptr->lcs_qos;

      mn_cmd_ptr->cmd.cm_lcs_molr_req.external_id =
        sups_cmd_info_ptr->external_id;

      mn_cmd_ptr->cmd.cm_lcs_molr_req.mlc_number =
        sups_cmd_info_ptr->mlc_number;

      mn_cmd_ptr->cmd.cm_lcs_molr_req.gps_assistance_data =
        sups_cmd_info_ptr->gps_assistance_data;

      mn_cmd_ptr->cmd.cm_lcs_molr_req.supported_gad_shapes =
        sups_cmd_info_ptr->supported_gad_shapes;

      mn_cmd_ptr->cmd.cm_lcs_molr_req.as_id = sups_cmd_info_ptr->cmd_subs_id;

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*-----------------------------*/
    /*      lcs-MOLR completed     */
    /*-----------------------------*/

    case CM_SUPS_CMD_LCS_MOLR_COMPLETED:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_LCS_MOLR_COMPLETED);

      /* Copy command information fields. */

      /* ss reference */
      mn_cmd_ptr->cmd.cm_lcs_molr_completed.cm_ss_ref =
        sups_cmd_info_ptr->ss_ref;

      mn_cmd_ptr->cmd.cm_lcs_molr_completed.as_id = sups_cmd_info_ptr->cmd_subs_id;

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*--------------------------------------*/
    /*  LCS Location Notification response  */
    /*--------------------------------------*/

    case CM_SUPS_CMD_LCS_LOCATION_NOTIFICATION_RES:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_LCS_LOCATION_NOTIFICATION_RES);

      mn_cmd_ptr->cmd.cm_lcs_location_notification_res.invoke_id =
        sups_cmd_info_ptr->invoke_id;

      mn_cmd_ptr->cmd.cm_lcs_location_notification_res.location_notification_res =
        sups_cmd_info_ptr->location_notification_res;

      mn_cmd_ptr->cmd.cm_lcs_location_notification_res.as_id = sups_cmd_info_ptr->cmd_subs_id;

      break;
#endif
    default:
      CM_MSG_HIGH_1("Unknown SUPS command %d received from client", sups_cmd);
      return;

  }

  /* send the command to MN */
  cm_mn_send_cmd(mn_cmd_ptr);

}

/*===========================================================================

FUNCTION cmwsups_client_cmd_forward_gw

DESCRIPTION
Forward a sups commands to MN.
This function will be called when route is calculated as CS
This Function will also be called when IMS sends fallback to CS.

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
NONE

SIDE EFFECTS
none

===========================================================================*/
extern void cmwsups_client_cmd_forward_gw(
  sys_modem_as_id_e_type  asubs_id,
  /* Subscription ID */
  cm_sups_cmd_e_type      cmd,
  /* Pointer to a CM command type */
  boolean                 is_fallback
  /* Indicate if this function is called due to IMS fallback to CS */
)
{
  mn_cnm_cmd_type   *mn_cmd_ptr;
  /* MN command pointer */
  uint8                      num_bytes = 0;
  /* Number of actual bytes required for USSD data */
  cm_process_unstructured_ss_req_T *uss_phase2_msg_ptr = cmsups_get_phase2_msg();
  cmsups_sub_info_s_type    *cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);
  cm_sups_info_s_type          *sups_info_ptr = cmsups_get_sups_info_ptr(asubs_id);
  cm_sups_uss_data_info_s_type *uss_info_ptr = cmsups_get_uss_info_ptr(asubs_id);
  cmwsups_info_list_s_type   *sups_info_list;
  /* Points to storage for phase2 msg */
  byte                            calculated_size = 0;

  if (sups_info_ptr == NULL || uss_info_ptr == NULL)
  {
    CM_MSG_HIGH_1("sups_info_ptr %x or uss_info_ptr is NULL ", sups_info_ptr);
    return;
  }

  switch (cmd)
  {
    case CM_SUPS_CMD_REGISTER:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_REGISTER_SS_REQ);

      /* ss reference */
      mn_cmd_ptr->cmd.cm_register_ss_req.cm_ss_ref = sups_info_ptr->ss_ref;

      /* ss code */
      mn_cmd_ptr->cmd.cm_register_ss_req.ss_code = sups_info_ptr->ss_code;

      /* Copy command information fields. */

      mn_cmd_ptr->cmd.cm_register_ss_req.basic_service = sups_info_ptr->basic_service;

      mn_cmd_ptr->cmd.cm_register_ss_req.forwarded_to_number =
        sups_info_ptr->forwarded_to_number;

      mn_cmd_ptr->cmd.cm_register_ss_req.forwarded_to_subaddress =
        sups_info_ptr->forwarded_to_subaddress;

      mn_cmd_ptr->cmd.cm_register_ss_req.cm_no_reply_condition_timer =
        sups_info_ptr->nr_timer;

      mn_cmd_ptr->cmd.cm_register_ss_req.as_id = sups_info_ptr->asubs_id;

      CM_MSG_HIGH_5("CM_SUPS_CMD_REGISTER: ss_code=%d, ss_ref=%d basic_service present=%d,code_type=%d,bs_code=%d",
                    sups_info_ptr->ss_code, \
                    sups_info_ptr->ss_ref, \
                    mn_cmd_ptr->cmd.cm_register_ss_req.basic_service.present, \
                    mn_cmd_ptr->cmd.cm_register_ss_req.basic_service.code_type, \
                    mn_cmd_ptr->cmd.cm_register_ss_req.basic_service.bs_code);
      CM_MSG_HIGH_3("fw_tonum present=%d,len=%d,data=%d", \
                    mn_cmd_ptr->cmd.cm_register_ss_req.forwarded_to_number.present, \
                    mn_cmd_ptr->cmd.cm_register_ss_req.forwarded_to_number.length, \
                    mn_cmd_ptr->cmd.cm_register_ss_req.forwarded_to_number.data);
      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*-------------------------*/
    /*    EraseSS request      */
    /*-------------------------*/

    case CM_SUPS_CMD_ERASE:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_ERASE_SS_REQ);

      mn_cmd_ptr->cmd.cm_erase_ss_req.cm_ss_ref = sups_info_ptr->ss_ref;

      mn_cmd_ptr->cmd.cm_erase_ss_req.ss_code = sups_info_ptr->ss_code;

      mn_cmd_ptr->cmd.cm_erase_ss_req.basic_service =
        sups_info_ptr->basic_service;

      mn_cmd_ptr->cmd.cm_erase_ss_req.as_id = sups_info_ptr->asubs_id;

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*-------------------------*/
    /*    ActivateSS request   */
    /*-------------------------*/

    case CM_SUPS_CMD_ACTIVATE:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_ACTIVATE_SS_REQ);

      /* ss reference */
      mn_cmd_ptr->cmd.cm_activate_ss_req.cm_ss_ref = sups_info_ptr->ss_ref;

      /* ss code */
      mn_cmd_ptr->cmd.cm_activate_ss_req.ss_code = sups_info_ptr->ss_code;

      /* Copy command information fields. */
      mn_cmd_ptr->cmd.cm_activate_ss_req.basic_service =
        sups_info_ptr->basic_service;

      mn_cmd_ptr->cmd.cm_activate_ss_req.as_id = sups_info_ptr->asubs_id;

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*-------------------------*/
    /*    DeactivateSS request */
    /*-------------------------*/

    case CM_SUPS_CMD_DEACTIVATE:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_DEACTIVATE_SS_REQ);

      /* ss reference */
      mn_cmd_ptr->cmd.cm_deactivate_ss_req.cm_ss_ref =
        sups_info_ptr->ss_ref;

      /* ss code */
      mn_cmd_ptr->cmd.cm_deactivate_ss_req.ss_code =
        sups_info_ptr->ss_code;

      /* Copy command information fields. */

      mn_cmd_ptr->cmd.cm_deactivate_ss_req.basic_service =
        sups_info_ptr->basic_service;

      mn_cmd_ptr->cmd.cm_deactivate_ss_req.as_id = sups_info_ptr->asubs_id;

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*-------------------------*/
    /*   InterrogateSS request */
    /*-------------------------*/

    case CM_SUPS_CMD_INTERROGATE:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_INTERROGATE_SS_REQ);

      /* Copy command information fields. */

      /* ss reference */
      mn_cmd_ptr->cmd.cm_interrogate_ss_req.cm_ss_ref =
        sups_info_ptr->ss_ref;

      /* ss code */
      mn_cmd_ptr->cmd.cm_interrogate_ss_req.ss_code =
        sups_info_ptr->ss_code;

      mn_cmd_ptr->cmd.cm_interrogate_ss_req.basic_service =
        sups_info_ptr->basic_service;

      mn_cmd_ptr->cmd.cm_interrogate_ss_req.as_id = sups_info_ptr->asubs_id;

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*-----------------------------*/
    /*  Register Password request  */
    /*-----------------------------*/

    case CM_SUPS_CMD_REG_PASSWORD:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_REGISTER_PASSWORD_REQ);

      /* Copy command information fields. */

      /* ss reference */
      mn_cmd_ptr->cmd.cm_register_password_req.cm_ss_ref =
        sups_info_ptr->ss_ref;

      mn_cmd_ptr->cmd.cm_register_password_req.ss_code =
        sups_info_ptr->code;

      mn_cmd_ptr->cmd.cm_register_password_req.ss_password =
        sups_info_ptr->ss_password;

      mn_cmd_ptr->cmd.cm_register_password_req.ss_new_password =
        sups_info_ptr->ss_new_password;

      mn_cmd_ptr->cmd.cm_register_password_req.ss_new_password_again =
        sups_info_ptr->ss_new_password_again;

      mn_cmd_ptr->cmd.cm_register_password_req.as_id = sups_info_ptr->asubs_id;

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*-----------------------------*/
    /*    Process Unstructured SS  */
    /*-----------------------------*/

    case CM_SUPS_CMD_PROCESS_USS:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_PROCESS_UNSTRUCTURED_SS_REQ);

      /* Copy command information fields. */

      /* ss reference */
      mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.cm_ss_ref =
        uss_info_ptr->ss_ref;

      /* ss operation */
      mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.ss_operation =
        uss_info_ptr->ss_operation;

      CM_MSG_HIGH_2("CM_SUPS_CMD_PROCESS_USS Size: %d bytes, DCC: %d",
                    uss_info_ptr->uss_data.size,
                    uss_info_ptr->uss_data.uss_data_coding_scheme);

      calculated_size = uss_info_ptr->uss_data.size;

      /*lint -save -e641 Converting enum to int */
      if (((uss_info_ptr->uss_data.uss_data_coding_scheme &
            CM_USSD_ALPHABET_MASK) == CM_USSD_DCS_7_BIT)
          ||
          ((uss_info_ptr->uss_data.uss_data_coding_scheme &
            CM_USSD_ALPHABET_MASK) == CM_USSD_DCS_UNSPECIFIED)
         )
      {
        /* MAX_USS_CHAR is what MN can support
        */
        if (uss_info_ptr->uss_data.size > MAX_USS_CHAR_ASCII)
        {
          uss_info_ptr->uss_data.size = MAX_USS_CHAR_ASCII;
          CM_MSG_HIGH_1("Truncating USSD msg to %d bytes", MAX_USS_CHAR);
        }

        /* convert to the default alphabet */
        num_bytes = cm_util_ascii_to_gsm_alphabet(
                      mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.uss_data.ussData,
                      uss_info_ptr->uss_data.ussData,
                      uss_info_ptr->uss_data.size);

        calculated_size = num_bytes;

      }
      /* Usage of CM_SUPS_CMD_PROCESS_USS has been to encode ussd data in CM
      ** and then send. EXCEPTION is being made to UCS2 where data being
      ** received is in encoded form. It is a TEMPORARY fix and needs to be
      ** changed with a new field that says if encoding has already been done.
      */
      else if (((uss_info_ptr->uss_data.uss_data_coding_scheme &
                 CM_USSD_ALPHABET_MASK) == CM_USSD_DCS_8_BIT)
               ||
               ((uss_info_ptr->uss_data.uss_data_coding_scheme &
                 CM_USSD_ALPHABET_MASK) == CM_USSD_DCS_UCS2)
              )

      {
        if (calculated_size > MAX_USS_BYTES)
        {
          calculated_size = MAX_USS_BYTES;
          CM_MSG_HIGH_1("Truncating USSD msg to %d bytes", MAX_USS_BYTES);
        }

        memscpy(mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.
                uss_data.ussData,
                sizeof(mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.
                       uss_data.ussData),
                uss_info_ptr->uss_data.ussData,
                calculated_size);
      }
      else
      {
        CM_ERR_0("USSD Data coding scheme error");
      }
      /*lint -restore 641 Converting enum to int */

      mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.uss_data.size =
        calculated_size;

      mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.
      uss_data.uss_data_coding_scheme =
        uss_info_ptr->uss_data.uss_data_coding_scheme;

      mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.uss_data.present = TRUE;
      uss_info_ptr->uss_data.present = TRUE;

      /* also save a copy of the USSD in case there is a phase
      compatibility problem */

      memset(uss_phase2_msg_ptr, 0, sizeof(cm_process_unstructured_ss_req_T));

      /* Copy ss ref */
      uss_phase2_msg_ptr->cm_ss_ref =
        mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.cm_ss_ref;

      /* Copy ss operation */
      uss_phase2_msg_ptr->ss_operation =
        mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.ss_operation;

      /* Copy ss data */
      memscpy(&uss_phase2_msg_ptr->uss_data,
              sizeof(uss_phase2_msg_ptr->uss_data),
              &mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.uss_data,
              sizeof(uss_data_T));

      mn_cmd_ptr->cmd.cm_process_unstructured_ss_req.as_id = uss_info_ptr->asubs_id;

      CM_MSG_HIGH_4("uss data sent to clients and NAS. present:%d coding scheme: %d size: %d uss data: %s",
                    uss_info_ptr->uss_data.present,
                    uss_info_ptr->uss_data.uss_data_coding_scheme,
                    calculated_size,
                    uss_info_ptr->uss_data.ussData);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*-------------------------*/
    /*    ReleaseSS request    */
    /*-------------------------*/

    case CM_SUPS_CMD_RELEASE:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_RELEASE_SS_REQ);

      /* Copy command information fields. */

      mn_cmd_ptr->cmd.cm_release_ss_req.cm_ss_ref =
        uss_info_ptr->invoke_id;

      mn_cmd_ptr->cmd.cm_release_ss_req.ss_operation =
        uss_info_ptr->ss_operation;

      mn_cmd_ptr->cmd.cm_release_ss_req.as_id = uss_info_ptr->asubs_id;

      cmsups_reset_mt_uss_context_info(uss_info_ptr->asubs_id);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*-------------------------*/
    /*   Get Password response */
    /*-------------------------*/

    case CM_SUPS_CMD_GET_PASSWORD_RES:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_GET_PASSWORD_RES);

      mn_cmd_ptr->cmd.cm_get_password_res.invoke_id = sups_info_ptr->invoke_id;

      mn_cmd_ptr->cmd.cm_get_password_res.ss_password = sups_info_ptr->ss_password;

      mn_cmd_ptr->cmd.cm_get_password_res.as_id = sups_info_ptr->asubs_id;

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*----------------------------*/
    /*  Unstructured SS response  */
    /*----------------------------*/

    case CM_SUPS_CMD_USS_RES:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_UNSTRUCTURED_SS_RES);

      /* Copy command information fields. */
      mn_cmd_ptr->cmd.cm_unstructured_ss_res.invoke_id =
        uss_info_ptr->invoke_id;

      if (uss_info_ptr->uss_res_type == CM_USS_RES_SS_ERROR)
      {
        mn_cmd_ptr->cmd.cm_unstructured_ss_res.uss_res_data.data.cm_ss_error =
          uss_info_ptr->ss_error;
        mn_cmd_ptr->cmd.cm_unstructured_ss_res.uss_res_data.data_type = SS_ERROR_INFO;
      }
      else if (uss_info_ptr->uss_res_type == CM_USS_RES_SS_DATA)
      {
        mn_cmd_ptr->cmd.cm_unstructured_ss_res.uss_res_data.data.uss_data =
          uss_info_ptr->uss_data;

        mn_cmd_ptr->cmd.cm_unstructured_ss_res.uss_res_data.data_type = USS_DATA_INFO;
      }

      mn_cmd_ptr->cmd.cm_unstructured_ss_res.as_id = uss_info_ptr->asubs_id;

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*-----------------------------------*/
    /*  Unstructured SS Notify response  */
    /*-----------------------------------*/

    case CM_SUPS_CMD_USS_NOTIFY_RES:

      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_UNSTRUCTURED_SS_NOTIFY_RES);

      /* Copy command information fields. */
      mn_cmd_ptr->cmd.cm_unstructured_ss_notify_res.invoke_id =
        uss_info_ptr->invoke_id;

      if (uss_info_ptr->uss_res_type == CM_USS_RES_SS_ERROR)
      {
        mn_cmd_ptr->cmd.cm_unstructured_ss_notify_res.cm_ss_error =
          uss_info_ptr->ss_error;
      }

      mn_cmd_ptr->cmd.cm_unstructured_ss_notify_res.as_id = uss_info_ptr->asubs_id;

      cmsups_reset_mt_uss_context_info(uss_info_ptr->asubs_id);
     
      break;
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*-------------------------*/
    /*    EraseCCEntry request   */
    /*-------------------------*/

    case CM_SUPS_CMD_ERASE_CC_ENTRY:

#if defined(FEATURE_CCBS)
      /* allocate a buffer */
      mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(CM_ERASE_CC_ENTRY_REQ);

      /* ss reference */
      mn_cmd_ptr->cmd.cm_erase_cc_entry_req.cm_ss_ref
        = sups_info_ptr->ss_ref;

      /* ss code */
      mn_cmd_ptr->cmd.cm_erase_cc_entry_req.ss_code = sups_info_ptr->ss_code;

      /* Deactivate one or all ccbs indx requests
      ** CM_CCBS_INDX_INVALID - maps to deactivating all requests
      ** ccbs_index is valid from 1 to 5 (22.030 Table B.1) ,
      */
      if (sups_info_ptr->ccbs_indx == CM_CCBS_INDX_INVALID)
      {
        mn_cmd_ptr->cmd.cm_erase_cc_entry_req.ccbs_index.present
          = FALSE;

        CM_MSG_HIGH_0("ERASE_CC_ENTRY for all ccbs indexes");
      }
      else
      {
        mn_cmd_ptr->cmd.cm_erase_cc_entry_req.ccbs_index.present
          = TRUE;

        mn_cmd_ptr->cmd.cm_erase_cc_entry_req.ccbs_index.ccbs_Index
          = sups_info_ptr->ccbs_indx;

        CM_MSG_HIGH_1("ERASE_CC_ENTRY for ccbs index %d",
                      sups_info_ptr->ccbs_indx);
      }

      CM_MSG_HIGH_1("is_erase_cc_conf_pend flag = %d",
                    cmsups_sub_info_ptr->ccbs_erase_pend_indx.is_erase_cc_conf_pend);

      /* Save the ccbs indx requested for deactivation to clear
      ** phone object ccbs indx store on confirmation
      */
      if (!cmsups_sub_info_ptr->ccbs_erase_pend_indx.is_erase_cc_conf_pend)
      {
        cmsups_sub_info_ptr->ccbs_erase_pend_indx.is_erase_cc_conf_pend = TRUE;
        cmsups_sub_info_ptr->ccbs_erase_pend_indx.cm_ss_ref = sups_info_ptr->ss_ref;
        cmsups_sub_info_ptr->ccbs_erase_pend_indx.ccbs_indx = sups_info_ptr->ccbs_indx;
      }
      else
      {
        CM_MSG_HIGH_0("Simult ccbs deact requests arrived");
      }

      mn_cmd_ptr->cmd.cm_erase_cc_entry_req.as_id = sups_info_ptr->asubs_id;

#endif

      break;

    default:
      CM_MSG_HIGH_1("Unknown SUPS command %d received from client", cmd);
      return;

  }

  /* For dual LTE, if a sups cmd is falling back from IMS to CS on the non DDS sub, and that
  ** sub is waiting for a service CNF, buffer the sups cmd for 1s or until a service CNF is
  ** received
  */
  if(is_fallback && cmwsups_is_buffer_cs_sups(asubs_id))
  {
    CM_MSG_HIGH_1("SUPS: buffer the sups cmd %d for 1s or until service CNF", cmd);
    cmsups_sub_info_ptr->buffered_mn_cmd_ptr = mn_cmd_ptr;
    cmsups_sub_info_ptr->buffered_mn_cmd_timer = time_get_uptime_secs() + SUPS_FALLBACK_BUF_DURATION;
    return;
  }

  sups_info_list = cmwsups_info_list_find(sups_info_ptr->asubs_id, sups_info_ptr->ss_ref);
  if(sups_info_list != NULL)
    sups_info_list->sups_cmd_route = CMSUPS_CMD_ALLOW_CS;
  /* send the command to MN */
  cm_mn_send_cmd(mn_cmd_ptr);
}

/*===========================================================================

FUNCTION CMSUPS_CONF_COPY

DESCRIPTION
Copy the confirmation from MN to the SUPS object.

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
static void cmsups_conf_copy(
  sys_modem_as_id_e_type   asubs_id,
  const ss_info_T         *ss_info
)
{
  cm_sups_info_s_type  *sups_conf_ptr;

  int                   i;
  int                   j;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  CM_ASSERT(ss_info != NULL);

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  sups_conf_ptr = cmsups_get_sups_info_ptr(asubs_id);

  /* Null check sups_conf_ptr */
  if (sups_conf_ptr == NULL)
  {
    CM_ERR_1("cmsups_conf_copy: sups_conf_ptr NULL ptr for asubs_id %d",
             asubs_id);
    return;
  }

  /* save the confirmation type */
  sups_conf_ptr->conf_type = ss_info->info_type;

  if (sups_conf_ptr->conf_type == NO_INFO)
  {
    /* confirmation with no parameter information, return now */
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch (ss_info->info_type)
  {
    case FWD_INFO:
      sups_conf_ptr->forwarding_info = ss_info->info.forwarding_info;
      /* need to convert the BCD number to ASCII number */
      if (sups_conf_ptr->forwarding_info.forwarding_feature_list.
          forwarding_feature_list_length != 0)

      {
        for (i = 0;
             i < sups_conf_ptr->forwarding_info.forwarding_feature_list.
             forwarding_feature_list_length;
             i++)
        {
          for (j = 0; j < MAXNO_FORWARD_TO_NUMBER; j++)
          {
            if (sups_conf_ptr->forwarding_info.forwarding_feature_list.forwarding_feature[i].forwarded_to_number[j].present)
            {
              memscpy(
                &sups_conf_ptr->forwarding_info.forwarding_feature_list.forwarding_feature[i].forwarded_to_number[j],
                sizeof(sups_conf_ptr->forwarding_info.forwarding_feature_list.forwarding_feature[i].forwarded_to_number[j]),
                &ss_info->info.forwarding_info.forwarding_feature_list.forwarding_feature[i].forwarded_to_number[j],
                sizeof(ss_info->info.forwarding_info.forwarding_feature_list.forwarding_feature[i].forwarded_to_number[j]));
            }
          }
        }
      }
      break;

    case CALL_BARRING_INFO:
      sups_conf_ptr->call_barring_info = ss_info->info.call_barring_info;
      break;

    case CUG_INFO:
      sups_conf_ptr->cug_info = ss_info->info.cug_info;
      break;

    case SS_DATA_INFO:
      sups_conf_ptr->ss_data = ss_info->info.ss_data;
      break;

    case SS_ERROR_INFO:
      sups_conf_ptr->ss_error = ss_info->info.cm_ss_error;
      break;

    case CC_CAUSE_INFO:
      sups_conf_ptr->cc_cause = ss_info->info.cm_cc_cause;
      break;

#if defined(FEATURE_CCBS)
    case CCBS_INFO:
      CM_MSG_HIGH_0("cmsups_conf_copy(): not expected for CCBS_INFO");
      break;
#endif /* FEATURE_CCBS */

    default:
      CM_MSG_HIGH_1("ERR: Unknown Confirmation type %d received", ss_info->info_type);
      sups_conf_ptr->conf_type = UNKNOWN_CONF_INFO;
      break;
  }
}

#ifdef FEATURE_SS_L3_DATA
/*===========================================================================

FUNCTION cmwsups_copy_suppsvc_to_sups_info

DESCRIPTION
Copies unparsed L3 message from suppsvc_result (in cmd_ptr) to sups_info_ptr

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
sups_unparsed_l3_data field in SUPS Object will get populated if data is
present.

===========================================================================*/
static void cmwsups_copy_suppsvc_to_sups_info(
  sys_modem_as_id_e_type              asubs_id,
  const unparsed_suppsvc_l3_data_T   *suppsvc_ptr
)
{
  uint16                 l3_data_size = 0;

  cm_sups_info_s_type   *sups_info_ptr = NULL;
  /* Pointer to the sups information */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_ASSERT(suppsvc_ptr != NULL);

  sups_info_ptr = cmsups_get_sups_info_ptr(asubs_id);

  /* Null check sups_info_ptr */
  if (sups_info_ptr == NULL)
  {
    CM_ERR_1("cmwsups_copy_suppsvc_to_sups_info: sups_info_ptr NULL ptr for asubs_id %d",
             asubs_id);
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Sanity check */

  if (!suppsvc_ptr->present)
  {
    CM_MSG_HIGH_0("Layer 3 data not present");
    sups_info_ptr->sups_unparsed_l3_data.present = FALSE;
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Range checks */

  l3_data_size = suppsvc_ptr->size;

  if (l3_data_size == 0)
  {
    CM_ERR_0("Layer 3 data size is 0. No copy required");
    return;
  }

  if (l3_data_size > ARR_SIZE(suppsvc_ptr->data))
  {
    CM_ERR_0("Value of suppsvc_ptr->size greater than array size");
    l3_data_size = ARR_SIZE(suppsvc_ptr->data);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Copy data */

  CM_MSG_HIGH_0("Copying layer 3 data");

  sups_info_ptr->sups_unparsed_l3_data.present =
    suppsvc_ptr->present;

  sups_info_ptr->sups_unparsed_l3_data.size = l3_data_size;

  memscpy(sups_info_ptr->sups_unparsed_l3_data.data,
          sizeof(sups_info_ptr->sups_unparsed_l3_data.data),
          suppsvc_ptr->data,
          l3_data_size);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Print debug messages */

  CM_MSG_HIGH_2("Present: %d, Size: %d",
                sups_info_ptr->sups_unparsed_l3_data.present,
                sups_info_ptr->sups_unparsed_l3_data.size);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return;
} /* cmwsups_copy_suppsvc_to_sups_info */
#endif /*   #ifdef FEATURE_SS_L3_DATA */

#ifdef FEATURE_SUPS_RETRY

/*===========================================================================

FUNCTION cmwsups_send_sups_fail_event

DESCRIPTION
Process timer events

DEPENDENCIES
Sups object must have already been initialized with
cmcall_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
static void cmwsups_send_sups_fail_event(
  sys_modem_as_id_e_type        asubs_id,
  cmwsups_info_list_s_type     *sups_list
)
{
  cm_sups_info_s_type   *sups_info_ptr = NULL;
  cm_sups_event_e_type   sups_event = CM_SUPS_EVENT_NONE;

  /* If SUPS info ptr for given SS is NULL, alloc again now
  */
  sups_info_ptr = cmsups_ensure_sups_info_ptr_asubs_id(asubs_id);
  if (sups_info_ptr == NULL)
  {
    CM_ERR_1("cmwsups_send_sups_fail_event: sups_info_ptr NULL ptr for asubs_id %d",
             asubs_id);
    return;
  }

  /* Update sups info with the cmd info if ss_ref does not match */
  if (sups_info_ptr->ss_ref != sups_list->sups_cmd_info.ss_ref)
  {
    cmsups_cmd_copy_fields_into_info(sups_info_ptr, &(sups_list->sups_cmd_info));
  }

  /* Set success as FALSE */
  sups_info_ptr->ss_success = FALSE;
  sups_info_ptr->uss_data.present = FALSE;
  sups_info_ptr->uss_data.size = 0;

  /* Send SUPS event now */
  switch (sups_list->cmd)
  {
    case CM_SUPS_CMD_REGISTER:
      sups_event = CM_SUPS_EVENT_REGISTER_CONF;
      break;

    case CM_SUPS_CMD_ERASE:
      sups_event = CM_SUPS_EVENT_ERASE_CONF;
      break;

    case CM_SUPS_CMD_ACTIVATE:
      sups_event = CM_SUPS_EVENT_ACTIVATE_CONF;
      break;

    case CM_SUPS_CMD_DEACTIVATE:
      sups_event = CM_SUPS_EVENT_DEACTIVATE_CONF;
      break;

    case CM_SUPS_CMD_INTERROGATE:
      sups_event = CM_SUPS_EVENT_INTERROGATE_CONF;
      break;

    case CM_SUPS_CMD_REG_PASSWORD:
      sups_event = CM_SUPS_EVENT_REG_PASSWORD_CONF;
      break;

    case CM_SUPS_CMD_PROCESS_USS:
      sups_event = CM_SUPS_EVENT_PROCESS_USS_CONF;
      break;

    case CM_SUPS_CMD_LCS_MOLR:
      sups_event = CM_SUPS_EVENT_LCS_MOLR_CONF;
      break;

    case CM_SUPS_CMD_ERASE_CC_ENTRY:
      sups_event = CM_SUPS_EVENT_DEACTIVATE_CONF;
      break;

    default:
      sups_event = CM_SUPS_EVENT_NONE;
  }

  if (sups_event != CM_SUPS_EVENT_NONE)
  {
    cmsups_event(sups_list->sups_cmd_info.cmd_subs_id, sups_event);
  }

  return;
}

/*===========================================================================

FUNCTION cmwsups_check_and_reject_uss_in_silent_redial

DESCRIPTION
Check for any MO USSD in silent redial and send fail
event for same.

DEPENDENCIES

RETURN VALUE
TRUE / FALSE

SIDE EFFECTS
none

===========================================================================*/
static void cmwsups_check_and_reject_uss_in_silent_redial(
  sys_modem_as_id_e_type asubs_id
)
{

  dword                     uptime = time_get_uptime_secs();
  /* Pointer to the sups object */
  cmsups_sub_info_s_type     *cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);
  cmwsups_info_list_s_type *sups_list_ptr = NULL;
  cmwsups_info_list_s_type *temp_list = NULL;

  /* Null check cmsups_sub_info_ptr */
  if (cmsups_sub_info_ptr == NULL)
  {
    CM_ERR_1("cmsups_rpt_proc: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
             asubs_id);
    return;
  }

  sups_list_ptr = cmsups_sub_info_ptr->sups_info_list_head;

  while (sups_list_ptr != NULL)
  {
    temp_list = sups_list_ptr->next;


    /* Need to look for MO USSD for which next_orig_uptime is not is non-zero and
    ** smaller than current uptime --> waiting for next redial
    ** next_orig_uptime == 0 is not possible . If NAS already had sups request , its NAS
    ** responsibility to not sent MT SUPS IND
    */

    CM_MSG_HIGH_4("SUPS MAX timer expired freeing the memory ss ref:%d,next_orig:%d,strt_time:%d cmd : %d",
                  sups_list_ptr->sups_cmd_info.ss_ref,
                  sups_list_ptr->next_orig_uptime,
                  sups_list_ptr->sr_start_uptime,
                  sups_list_ptr->cmd);

    /* Just checking next_orig_uptime is not zero should be enough .
    ** WRT current uptime there can be two cases:
    ** 1. sups_list_ptr->next_orig_uptime >= uptime -->
    **                          MO USSD in queue for redial
    ** 2.sups_list_ptr->next_orig_uptime <  uptime -->
    **                 Only possibility is that MT USSD NOTIFY IND has collided with
    **                 SUPS REDIAL timer expiry when this fucntion was called.
    **                 Basically, its not possible we had sent SUPS CMD REQ to NAS, else we
    **                 wouldnt have received MT USSD NOTIFY IND
    */
    if (sups_list_ptr->next_orig_uptime != 0 &&
        sups_list_ptr->cmd == CM_SUPS_CMD_PROCESS_USS)
    {
      if (sups_list_ptr == cmsups_sub_info_ptr->sups_info_list)
      {
        cmsups_sub_info_ptr->sups_info_list = NULL;
      }

      /* Send failure event to clients */
      cmwsups_send_sups_fail_event(asubs_id, sups_list_ptr);
      cmwsups_mem_free_sups_list_info(sups_list_ptr, &(cmsups_sub_info_ptr->sups_info_list_head));
    }

    sups_list_ptr = temp_list; /* Move to next record */
  }


}

#endif

/*===========================================================================

FUNCTION cmwsups_check_redial_cc_cause

DESCRIPTION
Check if given CC cause needs sups retry

DEPENDENCIES

RETURN VALUE
TRUE / FALSE

SIDE EFFECTS
none

===========================================================================*/
boolean cmwsups_check_redial_cc_cause(
  byte   cause_value
)
{
  switch (cause_value)
  {
    case AS_REJ_RRC_OPEN_SESSION_FAILURE:
    case AS_REJ_LOW_LEVEL_FAIL:
    case AS_REJ_RRC_REL_IND:
    case AS_REJ_RR_REL_IND:
    case AS_REJ_ABORT_RADIO_UNAVAILABLE:
    case AS_REJ_LOW_LEVEL_IMMED_RETRY:
    case AS_REJ_RR_RANDOM_ACCESS_FAILURE:
    case AS_REJ_LRRC_CONN_EST_FAILURE_ABORTED:
    case AS_REJ_LRRC_UL_DATA_CNF_FAILURE_CONN_REL:
    case MM_REJ_NO_SERVICE:
    case MM_REJ_NO_CELL_AVAILABLE:
    case MM_REJ_WRONG_STATE:
    case MM_REJ_TIMER_T3230_EXP:
      return TRUE;

    default:
      return FALSE;
  }
}

/*===========================================================================

FUNCTION cmwsups_check_and_update_cw_status

DESCRIPTION
Check and update NV for CW status with value stored prior to MCFG refresh if iccid is same.

DEPENDENCIES

RETURN VALUE


SIDE EFFECTS
none

===========================================================================*/
static void cmwsups_check_and_update_cw_status(
  sys_modem_as_id_e_type asubs_id
)
{
#if defined(CM_GW_SUPPORTED) || defined(FEATURE_IP_CALL)
  cmph_sub_info_s_type *ph_sub_ptr = cmph_get_sub_ptr(asubs_id);
  boolean is_sim_changed = FALSE;
  cmsups_s_type    *sups_ptr = cmsups_ptr();
  cmsups_sub_info_s_type    *cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);
  volte_sub_info_s_type	*volte_sub_info_ptr = cmph_get_volte_sub_info_ptr(asubs_id);
  cmph_s_type    *ph_ptr  =  cmph_ptr();
  boolean    prev_ue_based_cw = FALSE;


  if(cmsups_sub_info_ptr == NULL || ph_sub_ptr == NULL || ph_ptr == NULL)
  {
    return;
  }

  if(volte_sub_info_ptr->ue_based_cw.use_ue_based_cw == TRUE)
  {
    if(ph_ptr->device_prop.device_dbg_info.mcfg_refresh == TRUE && 
       mmgsdi_rpm_has_iccid_changed(ph_sub_ptr->slot_id, &is_sim_changed) == MMGSDI_SUCCESS)
    {
      if( is_sim_changed == FALSE)
      {
        prev_ue_based_cw = cmwsups_get_prev_ue_based_cw( asubs_id);
        if(cmph_get_volte_sub_info_ptr(asubs_id)->ue_based_cw.is_enabled != prev_ue_based_cw)
        {
          CM_MSG_HIGH_0("cmwsups_check_and_update_cw_status overwrite UE BASED CW NV");
          volte_sub_info_ptr->ue_based_cw.is_enabled = cmsups_sub_info_ptr->prev_ue_based_cw;
          cmefs_fs_write(CMEFS_UE_BASED_CW,
                       (byte *)&(volte_sub_info_ptr->ue_based_cw),
                       sizeof(cm_ue_based_cw_s_type),
                       asubs_id);
        }
      }
      else
      {
        CM_MSG_HIGH_0("cmwsups_check_and_update_cw_status sim got changed. Reset prev ue based cw");
        cmwsups_set_prev_ue_based_cw(volte_sub_info_ptr->ue_based_cw.is_enabled, asubs_id);
      }
    }
  }
#endif
}

/*===========================================================================

FUNCTION cmwsups_is_buffer_cs_sups

DESCRIPTION
Check if CS sups is to be buffered till SRV CNF

DEPENDENCIES

RETURN VALUE
TRUE / FALSE

SIDE EFFECTS
none

===========================================================================*/
boolean cmwsups_is_buffer_cs_sups(
  sys_modem_as_id_e_type asubs_id
)
{

  cmsups_sub_info_s_type    *cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

  if(cmph_is_dual_lte() &&
      asubs_id != cmph_ptr()->device_prop.msim_prop.default_data_subs &&
      cmregprx_get_substate(cmph_map_sub_stk_to_id(asubs_id, MM_STACK_0)) == CMREGPRX_SUBSTATE_WAIT_SRV_CNF &&
      cmsups_sub_info_ptr != NULL &&
      cmsups_sub_info_ptr->buffered_mn_cmd_ptr == NULL
     )
  {
    return TRUE;
  }

  return FALSE;
}


/*===========================================================================


FUNCTION cmwsups_setting_forward_gw

DESCRIPTION
Forwards call waiting setting to NAS


DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
    void cmwsups_setting_forward_gw(
  byte                   cmd,
  byte                   ss_ref,
      byte                   ss_code,
  sys_modem_as_id_e_type asubs_id)
{
  
  mn_cnm_cmd_type          *mn_cmd_ptr; /* MN command pointer */

  mn_cmd_ptr = cm_mn_get_buf_else_err_fatal(cmd);

  switch(cmd)
  {
    case CM_ACTIVATE_SS_REQ:
    {
      mn_cmd_ptr->cmd.cm_activate_ss_req.ss_code = ss_code;
      mn_cmd_ptr->cmd.cm_activate_ss_req.cm_ss_ref = ss_ref;
      mn_cmd_ptr->cmd.cm_activate_ss_req.as_id = asubs_id;
      break;
     }

    case CM_DEACTIVATE_SS_REQ:
    {
      mn_cmd_ptr->cmd.cm_deactivate_ss_req.ss_code = ss_code;
      mn_cmd_ptr->cmd.cm_activate_ss_req.cm_ss_ref = ss_ref;
      mn_cmd_ptr->cmd.cm_deactivate_ss_req.as_id = asubs_id;
      break;
    }
  }
  CM_MSG_HIGH_4("sending sups cmd %d ss_ref %d ss_code %d asub_id %d",cmd,ss_ref,ss_code,asubs_id);
  /* send the command to MN */
  cm_mn_send_cmd(mn_cmd_ptr);
}

/*===========================================================================
FUNCTION cmwsups_update_ue_based_sups

DESCRIPTION
Update CMEFS_UE_BASED_CW if CW setting changed while in CS

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
void cmwsups_update_ue_based_sups(
  byte                   ss_code,
  boolean                is_enabled,
  sys_modem_as_id_e_type asubs_id
)
{
  volte_sub_info_s_type   *volte_sub_info_ptr = cmph_get_volte_sub_info_ptr(asubs_id);
  uint8 i = CMEFS_UE_BASED_SUPS_ARRAY_USED;
  cm_ue_based_sups_s_type ue_based_sups;
  boolean update_ue_based_sups_flag = FALSE;
  cmsups_sub_info_s_type     *cmsups_sub_info_ptr = NULL;
  
  cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);
  
  if(volte_sub_info_ptr != NULL)
  {
    CM_MSG_HIGH_2("cmwsups_update_ue_based_sups, sub %d is ue_based_sups enabled = %d", asubs_id,is_enabled);

    switch(ss_code)
    {
        case cw:
        {
            volte_sub_info_ptr->ue_based_cw.is_enabled = is_enabled;
            cmefs_fs_write(CMEFS_UE_BASED_CW,
                           (byte *)&(volte_sub_info_ptr->ue_based_cw),
                           sizeof(cm_ue_based_cw_s_type),
                           asubs_id);
         if(volte_sub_info_ptr->ue_based_cw.use_ue_based_cw == TRUE)
         {
           cmwsups_set_prev_ue_based_cw(is_enabled, asubs_id);
         }
         break;
         }
       /*Deactivating all_call_barring settings - allCallRestrictionSS comes with deactivate, deactivate all call_barring settings*/
       case allCallRestrictionSS:
        {
        if(is_enabled == 0) /*Curently UE supports only deactivate all, that is why we checked for is_enabled=0*/
        {
         for(i = allCallRestrictionSS_idx; i <= bain_idx; i++)
          {
           volte_sub_info_ptr->ue_based_sups[i].is_enabled = is_enabled;
          }
        }
        update_ue_based_sups_flag = TRUE;
        break;
       }
       case colr:
       case barringOfOutgoingCalls:
       case baoc:
       case boic:
       case boicExHC:
       case barringOfIncomingCalls:
       case baic:
       case bicRoam:
       case bain:   
        {
         i = cmsups_map_code_to_index(ss_code);
         if(i < CMEFS_UE_BASED_SUPS_ARRAY_USED)
            {
             volte_sub_info_ptr->ue_based_sups[i].is_enabled = is_enabled;
            }
         
         update_ue_based_sups_flag = TRUE;
         break;
        } 
      default:
        break;
    }

    if(update_ue_based_sups_flag == TRUE)
    {
             ue_based_sups.version = 0;
             memscpy(ue_based_sups.config, sizeof(ue_based_sups.config), volte_sub_info_ptr->ue_based_sups, sizeof(volte_sub_info_ptr->ue_based_sups));

             cmefs_fs_write(CMEFS_UE_BASED_SUPS,
                         (byte *)&(ue_based_sups),
                         sizeof(cm_ue_based_sups_s_type),
                         asubs_id);
     } 
  return;
    }
}
/*===========================================================================

FUNCTION cmwsups_get_prev_ue_based_sups

DESCRIPTION
To fetch the value stored in prev_ue_based_sups_enabled.

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
static boolean cmwsups_get_prev_ue_based_sups(
  byte                   ss_code,
  sys_modem_as_id_e_type asubs_id
)
{

  cmsups_sub_info_s_type    *cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);
  uint8 idx = CMEFS_UE_BASED_SUPS_ARRAY_USED;

  if(cmsups_sub_info_ptr != NULL)
  {
     idx = cmsups_map_code_to_index(ss_code);
     if(idx<(CMEFS_UE_BASED_SUPS_ARRAY_USED + 1))
        {
          return cmsups_sub_info_ptr->prev_ue_based_sups_enabled[idx];
        }
  }
return FALSE;
}

/*===========================================================================

FUNCTION cmwsups_get_prev_ue_based_cw

DESCRIPTION
To fetch the value stored in prev_ue_based_cw.

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
static boolean cmwsups_get_prev_ue_based_cw(
  sys_modem_as_id_e_type asubs_id
)
{
  cmsups_sub_info_s_type    *cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

  if(cmsups_sub_info_ptr != NULL)
  {
    if (cmefs_get(CMEFS_PREV_UE_BASED_CW,
                  (byte *)&cmsups_sub_info_ptr->prev_ue_based_cw,
                   sizeof(boolean),
                   asubs_id)
        )
    {
      CM_MSG_HIGH_2("Sub %d read CMEFS_PREV_UE_BASED_CW success prev_ue_based_cw %d ",asubs_id,cmsups_sub_info_ptr->prev_ue_based_cw);
      return cmsups_sub_info_ptr->prev_ue_based_cw;
    }
    else /* NV not set */
    {
      CM_MSG_HIGH_1("Sub %d CMEFS_PREV_UE_BASED_CW read failed", asubs_id);
      return cmph_get_volte_sub_info_ptr(asubs_id)->ue_based_cw.is_enabled;
    }
  }
  return FALSE;
}

/*===========================================================================

FUNCTION cmwsups_is_ue_camped_in_cs

DESCRIPTION
Check if the active RAT is CS


DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
boolean cmwsups_is_ue_camped_in_cs(
  sys_modem_as_id_e_type asubs_id
)
{
  cm_mm_ss_info_sub_prop_s_type *ss_sub_ptr = cmss_get_info_sub_prop(asubs_id);
  cm_mm_ss_info_gen_stack_info_s_type *ss_stack_ptr;
  if(ss_sub_ptr == NULL)
  {
    return FALSE;
  }
  ss_stack_ptr = ss_sub_ptr->stack_info[MM_STACK_0].gen_info;
  if(ss_stack_ptr == NULL)
  {
    return FALSE;
  }

  /* Return true if UE is camped in one of the CS RATs GWT.
  */
  if(sd_misc_is_mode_pref(BM(ss_stack_ptr->sys_mode),SD_SS_MODE_PREF_TDS_GSM_WCDMA) &&
     ss_stack_ptr->srv_status == SYS_SRV_STATUS_SRV)
  {
    CM_MSG_HIGH_1("UE is camped on CS: sys_mode = %d",ss_stack_ptr->sys_mode);
    return TRUE;
  }
  return FALSE;
}

/*===========================================================================

FUNCTION cmwsups_check_and_perform_cs_sups_sync

DESCRIPTION
check and perform the CS sups sync.
DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
void cmwsups_check_and_perform_immed_cs_sups_sync(
    sys_modem_as_id_e_type asubs_id
)
{
  cmsups_sub_info_s_type     *cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);
  dword                      uptime               = time_get_uptime_secs();
  byte                        ss_ref;
  cm_ue_based_sups_config_s_type     cm_ue_based_sups_config;
  boolean    prev_ue_based_sups_check = FALSE;
  byte     ss_code;
  uint8     i=0;

  if ((cmsups_sub_info_ptr == NULL) ||
      (cmsups_is_immediate_sups_sync_with_cs_required(asubs_id) == FALSE))
  {
    return;
  }
  
  /* Send OTA sups setting cahnge when UE_BASED_SUPS is enabled and
        ** UE based CW setting changed from the last time UE was camped on CS
        */
  for(i=0;i<=CMEFS_UE_BASED_SUPS_ARRAY_USED ; i++)
  {
    ss_code = cmwsups_map_idx_to_code(i);
    cm_ue_based_sups_config = cmsups_map_ss_code_to_sups_config(ss_code,asubs_id);
  
    if(cm_ue_based_sups_config.sync_ue_based_sups_with_cs == TRUE)
    {
      CM_MSG_HIGH_5_ASID("UE_BASED_SUPS in CS: is_enabled = %d, use_ue_based_sups = %d, prev_is_ue_based_sups_enabled = %d, immediate_sups_sync_with_cs_req = %d",
                           asubs_id,
                           cm_ue_based_sups_config.is_enabled,
                           cm_ue_based_sups_config.use_ue_based_sups,
                           prev_ue_based_sups_check,
                           cmsups_is_immediate_sups_sync_with_cs_required(asubs_id));
  
      /* If ue_based_sups is configured, we don't need to do any sups cmd check */
      prev_ue_based_sups_check = cmwsups_get_prev_ue_based_sups(ss_code, asubs_id);
      ss_ref = qmi_voice_cm_sups_get_ss_ref();
      if(cm_ue_based_sups_config.use_ue_based_sups == TRUE &&
         (prev_ue_based_sups_check != cm_ue_based_sups_config.is_enabled))
      {
        cmsups_sub_info_ptr->sups_info_list = cmwsups_info_list_find_alloc(asubs_id, ss_ref);
        if (cmsups_sub_info_ptr->sups_info_list != NULL)
        {
          cmsups_sub_info_ptr->sups_info_list->sr_start_uptime = uptime;
          cmsups_sub_info_ptr->sups_info_list->sups_cmd_info.ss_code = ss_code;
          cmsups_sub_info_ptr->sups_info_list->sups_cmd_info.cmd_subs_id = asubs_id;
          if(cm_ue_based_sups_config.is_enabled)
          {
            cmsups_sub_info_ptr->sups_info_list->cmd = CM_SUPS_CMD_ACTIVATE;
          }
          else
          {
            cmsups_sub_info_ptr->sups_info_list->cmd = CM_SUPS_CMD_DEACTIVATE;
          }
        }
        if(cm_ue_based_sups_config.is_enabled)
        {
          cmwsups_setting_forward_gw(CM_ACTIVATE_SS_REQ, ss_ref,ss_code, asubs_id);
        }
        else
        {
          cmwsups_setting_forward_gw(CM_DEACTIVATE_SS_REQ, ss_ref,ss_code, asubs_id);
        }
        /* Increase the pending sups count */
        cmsups_sub_info_ptr->num_pending_sups_cmds++;
  
        cmwsups_set_prev_ue_based_sups(ss_code,cm_ue_based_sups_config.is_enabled, asubs_id, TRUE);
      }
  
    }
  }


}


/*===========================================================================

FUNCTION cmwsups_enable_cw_on_sim_chg

DESCRIPTION
Process SD reports 

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
void cmwsups_enable_cw_on_sim_chg(
    sys_modem_as_id_e_type asubs_id

)
{
  dword                       uptime                = time_get_uptime_secs();
  cmsups_sub_info_s_type     *cmsups_sub_info_ptr   = cmsups_get_sub_info_ptr(asubs_id);
  byte                        ss_ref;
  byte                        ss_code;


  if(cmsups_sub_info_ptr == NULL)
  {
    return;
  }

  /* if IMS CHG CW enable cofig is not set, then return immeadiately*/
  if (((cmsups_sub_info_ptr->ext_cw_config_info.cw_sups_enable_opr_mask & 
        SUPS_ENABLE_OPR_MASK_SIM_CHG) == 0) ||
       (cmsups_sub_info_ptr->is_sim_chg_cw_enable == FALSE))
  {
    return;
  }

  CM_MSG_HIGH_0("Enable CW as sim card change is detected and no IMS REG ongoing");
  ss_ref = qmi_voice_cm_sups_get_ss_ref();
  ss_code = cw;
  /* Create an entry for sups retry */
  cmsups_sub_info_ptr->sups_info_list = cmwsups_info_list_find_alloc(asubs_id, ss_ref);
  if (cmsups_sub_info_ptr->sups_info_list != NULL)
  {
    cmsups_sub_info_ptr->sups_info_list->sr_start_uptime = uptime;
    cmsups_sub_info_ptr->sups_info_list->sups_cmd_info.ss_code = ss_code;
    cmsups_sub_info_ptr->sups_info_list->sups_cmd_info.cmd_subs_id = asubs_id;
    cmsups_sub_info_ptr->sups_info_list->cmd = CM_SUPS_CMD_ACTIVATE;
  }
  
  cmwsups_setting_forward_gw(CM_ACTIVATE_SS_REQ, ss_ref, ss_code, asubs_id);
  cmwsups_update_ue_based_sups(ss_code,TRUE, asubs_id);
  cmwsups_set_prev_ue_based_sups(ss_code,TRUE, asubs_id, TRUE);
  cmsups_sub_info_ptr->is_sim_chg_cw_enable = FALSE;

}
/*===========================================================================

FUNCTION cmwsups_sd_rpt_proc

DESCRIPTION
Process SD reports 

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
void cmwsups_sd_rpt_proc(
  const cm_hdr_type   *rpt_ptr
  /* Pointer to SD report */
)
/*lint -esym(529, sups_ptr) not referenced */
{
  const cm_sd_rpt_u_type                *sd_rpt_ptr = (cm_sd_rpt_u_type *)rpt_ptr;
  /* Pointer to LL report */

  cmsups_s_type              *sups_ptr            = cmsups_ptr();
  cmsups_sub_info_s_type     *cmsups_sub_info_ptr = NULL ;
  dword                      uptime = time_get_uptime_secs();
  sys_modem_as_id_e_type      asubs_id            = sd_rpt_ptr->hdr.asubs_id;
  cmph_sub_info_s_type       *ph_sub_ptr = cmph_get_sub_ptr(asubs_id);
  cmregprx_sub_info_s_type   *cmreg_sub_ptr;
  byte                        ss_ref;
  cm_ue_based_sups_config_s_type     cm_ue_based_sups_config;
  cm_mm_ss_info_gen_stack_info_s_type *ss_stack_ptr;
  cm_mm_ss_info_sub_prop_s_type *ss_sub_ptr ;
  boolean    prev_ue_based_sups_check = FALSE;
  byte     ss_code;
  uint8     i=0;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  CM_ASSERT( sups_ptr != NULL );
  CM_ASSERT( sd_rpt_ptr != NULL );
  if( ph_sub_ptr == NULL )
  {
    return;
  }

  cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cmreg_sub_ptr = cmregprx_get_sub_ptr(asubs_id);

  if (cmsups_sub_info_ptr == NULL || cmreg_sub_ptr == NULL)
  {
    CM_MSG_HIGH_1("cmwsups_sd_rpt_proc: sub_info_ptr/cmreg_sub_ptr is NULL for asubs_id %d",
             asubs_id);
    return;
  }

  ss_sub_ptr = cmss_get_info_sub_prop(asubs_id);
  
  if(ss_sub_ptr == NULL)
  {
    return;
  }
  ss_stack_ptr = ss_sub_ptr->stack_info[MM_STACK_0].gen_info;
  if(ss_stack_ptr == NULL)
  {
    return;
  }
  
  switch( sd_rpt_ptr->hdr.cmd )
  {
    case CM_SRV_IND_INFO_F:
    {
      volte_sub_info_s_type *volte_ptr = cmph_get_volte_sub_info_ptr(asubs_id);
      
      if (sd_rpt_ptr->srv_ind_info.stack_id != MM_STACK_0 || volte_ptr == NULL)
      {
        return;
      }

      /* Enable CW sups upon SIM change.
             1. If UE is camped on stable G/W/T service or
             2. If UE is camped on stable LTE with CSFB support and 
                    a) UE is IMS registered over WLAN or
                    b) UE is not IMS registered over WLAN and UE is not performing IMS REG over LTE
             For 2b) CM will enable CW after ims REG is success or failed in IMS.
           */
      if(sd_rpt_ptr->srv_ind_info.si_info.srv_status == SYS_SRV_STATUS_SRV &&
         sd_rpt_ptr->srv_ind_info.si_info.is_stable_in_svc == TRUE &&
         (sd_misc_is_mode_pref(BM(sd_rpt_ptr->srv_ind_info.si_info.mode),SD_SS_MODE_PREF_TDS_GSM_WCDMA) ||
           ((cmsds_is_ppcsfb_possible(asubs_id) == TRUE) &&
                ((cmsds_get_ims_voice_reg_status(asubs_id, SYS_SYS_MODE_WLAN) == SYS_IMS_REG_STATE_REGISTERED) ||
                 ((cmsds_get_ims_voice_reg_status(asubs_id, SYS_SYS_MODE_WLAN) != SYS_IMS_REG_STATE_REGISTERED) &&
                  (cmsds_get_ims_voice_reg_status(asubs_id, SYS_SYS_MODE_LTE) != SYS_IMS_REG_STATE_NEEDS_REG) )
                 )
             )
           )
        )
      {
        /* If UE acquires stable CS/CSFB service, then enable the CW setting based on NV config*/
        if ((cmsups_sub_info_ptr->ext_cw_config_info.cw_sups_enable_opr_mask & 
               SUPS_ENABLE_OPR_MASK_SIM_CHG) &&
            (cmsups_sub_info_ptr->is_sim_chg_cw_enable == TRUE)
           )
        {
           cmwsups_enable_cw_on_sim_chg(asubs_id);
          return;
        }
      }

      /* Perform the sups sync, if CW status of NW and UE are not in sync and
             1. UE is camped on LTE with CSFB support and immeadiate sups sync is enabled. This shouldn't collide with IMS REG on LTE.
             2. UE is camped on CS RAT like G/w/T.
             */
      if(sd_rpt_ptr->srv_ind_info.si_info.srv_status == SYS_SRV_STATUS_SRV &&
          sd_rpt_ptr->srv_ind_info.si_info.is_stable_in_svc == TRUE &&
         (sd_misc_is_mode_pref(BM(sd_rpt_ptr->srv_ind_info.si_info.mode),SD_SS_MODE_PREF_TDS_GSM_WCDMA) ||
            ((cmsups_is_immediate_sups_sync_with_cs_required(asubs_id) == TRUE) &&
              (cmsds_is_ppcsfb_possible(asubs_id) == TRUE) &&
              ((cmsds_get_ims_voice_reg_status(asubs_id, SYS_SYS_MODE_WLAN) == SYS_IMS_REG_STATE_REGISTERED) ||
                 ((cmsds_get_ims_voice_reg_status(asubs_id, SYS_SYS_MODE_WLAN) != SYS_IMS_REG_STATE_REGISTERED) &&
                  (cmsds_get_ims_voice_reg_status(asubs_id, SYS_SYS_MODE_LTE) != SYS_IMS_REG_STATE_NEEDS_REG) )
               )
             )
          )
         )
       {
        /* Send OTA sups setting cahnge when UE_BASED_SUPS is enabled and
              ** UE based CW setting changed from the last time UE was camped on CS
        */
      for(i=0;i<=CMEFS_UE_BASED_SUPS_ARRAY_USED ; i++)
      {
        ss_code = cmwsups_map_idx_to_code(i);
        cm_ue_based_sups_config = cmsups_map_ss_code_to_sups_config(ss_code,asubs_id);
       
        if(cm_ue_based_sups_config.sync_ue_based_sups_with_cs == TRUE)
        {
          CM_MSG_HIGH_5_ASID("UE_BASED_SUPS in CS: is_enabled = %d, use_ue_based_sups = %d, prev_is_ue_based_sups_enabled = %d, immediate_sups_sync_with_cs_req = %d",
                        asubs_id,
                        cm_ue_based_sups_config.is_enabled,
                        cm_ue_based_sups_config.use_ue_based_sups,
                        prev_ue_based_sups_check,
                        cmsups_is_immediate_sups_sync_with_cs_required(asubs_id));
 
          /* If ue_based_sups is configured, we don't need to do any sups cmd check */
          prev_ue_based_sups_check = cmwsups_get_prev_ue_based_sups(ss_code, asubs_id);
          ss_ref = qmi_voice_cm_sups_get_ss_ref();
          if(cm_ue_based_sups_config.use_ue_based_sups == TRUE &&
                  (prev_ue_based_sups_check != cm_ue_based_sups_config.is_enabled))
          {
            cmsups_sub_info_ptr->sups_info_list = cmwsups_info_list_find_alloc(asubs_id, ss_ref);
            if (cmsups_sub_info_ptr->sups_info_list != NULL)
            {
              cmsups_sub_info_ptr->sups_info_list->sr_start_uptime = uptime;
              cmsups_sub_info_ptr->sups_info_list->sups_cmd_info.ss_code = ss_code;
              cmsups_sub_info_ptr->sups_info_list->sups_cmd_info.cmd_subs_id = asubs_id;
              if(cm_ue_based_sups_config.is_enabled)
              {
                cmsups_sub_info_ptr->sups_info_list->cmd = CM_SUPS_CMD_ACTIVATE;
              }
              else
              {
                cmsups_sub_info_ptr->sups_info_list->cmd = CM_SUPS_CMD_DEACTIVATE;
              }
            }
            if(cm_ue_based_sups_config.is_enabled)
            {
              cmwsups_setting_forward_gw(CM_ACTIVATE_SS_REQ, ss_ref,ss_code, asubs_id);
            }
            else
            {
              cmwsups_setting_forward_gw(CM_DEACTIVATE_SS_REQ, ss_ref,ss_code, asubs_id);
            }
            /* Increase the pending sups count */
            cmsups_sub_info_ptr->num_pending_sups_cmds++;
            
            cmwsups_set_prev_ue_based_sups(ss_code,cm_ue_based_sups_config.is_enabled, asubs_id, TRUE);
          }
         }
       }
     }
     break;
   }
   default:
    break;
  }
}
/*===========================================================================
FUNCTION cmsups_rpt_proc

DESCRIPTION
Process lower layer reports (i.e. notifications of sups events from MN)

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
void cmsups_rpt_proc(
  const cm_hdr_type   *rpt_ptr
  /* Pointer to LL report */
)
/*lint -esym(529, sups_ptr) not referenced */
{
  cm_rpt_type                *cm_rpt_ptr = (cm_rpt_type *)rpt_ptr;
  /* Pointer to LL report */

  cmsups_s_type              *sups_ptr = cmsups_ptr();
  /* Pointer to the sups object */
  cmsups_sub_info_s_type     *cmsups_sub_info_ptr;
  sys_modem_as_id_e_type      asubs_id = SYS_MODEM_AS_ID_1;

  boolean                     is_mem_free_req = TRUE;
  boolean                     is_retry = FALSE;

  cm_sups_info_s_type        *sups_info_ptr = NULL;
  /* Pointer to the sups information */

  InterrogateSS_Res_T        *interr_conf_ptr;
  /* Pointer to the interrogateSS response */

#if defined(FEATURE_CCBS)
  ie_ccbs_indicator_T        *ccbs_indicator_ptr;
  /* Pointer to ccbs indicator */

  int                         indx_count = 0;
  /* Loops through ccbs indx list */
#endif

  int                         i;
  int                         j;

  cm_sups_event_e_type        sups_event_type = CM_SUPS_EVENT_NONE;

  boolean                     is_break_in_mid = FALSE;

#ifdef FEATURE_SUPS_RETRY
  dword                       uptime = time_get_uptime_secs();
  cmwsups_info_list_s_type   *sups_list_info_ptr = NULL;
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_ASSERT(cm_rpt_ptr != NULL);
  CM_ASSERT(sups_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Do command specific processing as follows:
  **   Update appropriate sups object fields.
  **   Notifying clients of event if necessary.
  */
  switch (cm_rpt_ptr->hdr.cmd)
  {
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*                              **
    **  Register SS Confirmation    **
    **                              */
    case CM_REGISTER_SS_CONF:

      CM_MSG_HIGH_0("RXD: CM_REGISTER_SS_CONF");

      asubs_id = cm_rpt_ptr->cmd.register_conf.as_id;
      cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

      /* Null check cmsups_sub_info_ptr */
      if (cmsups_sub_info_ptr == NULL)
      {
        CM_ERR_1("cmsups_rpt_proc: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
                 asubs_id);
        return;
      }

      /* Update the number of pending sups requests */
      if (cmsups_sub_info_ptr->num_pending_sups_cmds != 0)
        cmsups_sub_info_ptr->num_pending_sups_cmds--;

      cmsups_update_asubs_id(cm_rpt_ptr->cmd.register_conf.as_id, &sups_info_ptr);

#ifdef FEATURE_SUPS_RETRY
      sups_list_info_ptr = cmwsups_info_list_find(asubs_id, cm_rpt_ptr->cmd.register_conf.cm_ss_ref);
#endif

#ifdef FEATURE_SS_L3_DATA
      cmwsups_copy_suppsvc_to_sups_info(asubs_id,
                                        &cm_rpt_ptr->cmd.register_conf.suppsvc_result);
#endif

      sups_info_ptr->ss_ref = cm_rpt_ptr->cmd.register_conf.cm_ss_ref;

      if (cm_rpt_ptr->cmd.register_conf.success)
      {
        cmsups_conf_copy(asubs_id, &cm_rpt_ptr->cmd.register_conf.ss_info);
        sups_info_ptr->ss_success = TRUE;
      }
      else
      {
        /* error in the message received from the network */
        sups_info_ptr->ss_success = FALSE;
      }

      /* Multi-sim: Resume ps data */
      cmsups_msim_resume_data(asubs_id);

#if defined(FEATURE_SUPS_RETRY)
      if (((cm_rpt_ptr->cmd.register_conf.ss_info.info_type == CC_CAUSE_INFO &&
            (cmwsups_check_redial_cc_cause(cm_rpt_ptr->cmd.register_conf.ss_info.info.cm_cc_cause.cause_value) == TRUE)) ||
           (cm_rpt_ptr->cmd.register_conf.ss_info.info_type == SS_ERROR_INFO &&
            cm_rpt_ptr->cmd.register_conf.ss_info.info.cm_ss_error.error_code == AS_REJ_ABORT_RADIO_UNAVAILABLE)) &&
          sups_list_info_ptr != NULL)
      {
        /* If retry timer will expire before next retry...send the event now */
        if (sups_list_info_ptr->sr_start_uptime  > (uptime + SUPS_NEXT_RETRY_DURATION - SUPS_RETRY_MAX_DURATION))
        {
          /* Radio Unavailable....initiate retry */
          sups_list_info_ptr->next_orig_uptime = uptime + SUPS_NEXT_RETRY_DURATION;

          CM_MSG_HIGH_3("SUPS failed, Initiate Retry. ss_ref %d cause %d next uptime %d",
                        sups_info_ptr->ss_ref,
                        cm_rpt_ptr->cmd.register_conf.ss_info.info.cm_cc_cause.cause_value,
                        sups_list_info_ptr->next_orig_uptime);

          /* Set flag to not free the corresponding memory */
          is_retry = TRUE;
        }
      }
#endif

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*                           **
    **  Erase SS Confirmation    **
    **                           */

    case CM_ERASE_SS_CONF:

      CM_MSG_HIGH_0("RXD: CM_ERASE_SS_CONF");

      asubs_id = cm_rpt_ptr->cmd.erase_conf.as_id;
      cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

      /* Null check cmsups_sub_info_ptr */
      if (cmsups_sub_info_ptr == NULL)
      {
        CM_ERR_1("cmsups_rpt_proc: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
                 asubs_id);
        return;
      }

      /* Update the number of pending sups requests */
      if (cmsups_sub_info_ptr->num_pending_sups_cmds != 0)
        cmsups_sub_info_ptr->num_pending_sups_cmds--;

      cmsups_update_asubs_id(cm_rpt_ptr->cmd.erase_conf.as_id, &sups_info_ptr);

#ifdef FEATURE_SUPS_RETRY
      sups_list_info_ptr = cmwsups_info_list_find(asubs_id, cm_rpt_ptr->cmd.erase_conf.cm_ss_ref);
#endif

#ifdef FEATURE_SS_L3_DATA
      cmwsups_copy_suppsvc_to_sups_info(asubs_id,
                                        &cm_rpt_ptr->cmd.erase_conf.suppsvc_result);
#endif

      sups_info_ptr->ss_ref = cm_rpt_ptr->cmd.erase_conf.cm_ss_ref;

      if (cm_rpt_ptr->cmd.erase_conf.success)
      {
        cmsups_conf_copy(asubs_id, &cm_rpt_ptr->cmd.erase_conf.ss_info);
        sups_info_ptr->ss_success = TRUE;
      }
      else
      {
        /* error in the message received from the network */
        sups_info_ptr->ss_success = FALSE;
      }

      /* Multi-sim: Resume ps data */
      cmsups_msim_resume_data(asubs_id);

#if defined(FEATURE_SUPS_RETRY)
      if (((cm_rpt_ptr->cmd.erase_conf.ss_info.info_type == CC_CAUSE_INFO &&
            (cmwsups_check_redial_cc_cause(cm_rpt_ptr->cmd.erase_conf.ss_info.info.cm_cc_cause.cause_value) == TRUE)) ||
           (cm_rpt_ptr->cmd.erase_conf.ss_info.info_type == SS_ERROR_INFO&&
            cm_rpt_ptr->cmd.erase_conf.ss_info.info.cm_ss_error.error_code == AS_REJ_ABORT_RADIO_UNAVAILABLE)) &&
          sups_list_info_ptr != NULL)
      {
        /* If retry timer will expire before next retry...send the event now */
        if (sups_list_info_ptr->sr_start_uptime  > (uptime + SUPS_NEXT_RETRY_DURATION - SUPS_RETRY_MAX_DURATION))
        {
          /* Radio Unavailable....initiate retry */
          sups_list_info_ptr->next_orig_uptime = uptime + SUPS_NEXT_RETRY_DURATION;

          CM_MSG_HIGH_3("SUPS failed, Initiate Retry. ss_ref %d cause %d next uptime %d",
                        sups_info_ptr->ss_ref,
                        cm_rpt_ptr->cmd.erase_conf.ss_info.info.cm_cc_cause.cause_value,
                        sups_list_info_ptr->next_orig_uptime);

          /* Set flag to not free the corresponding memory */
          is_retry = TRUE;
        }
      }
#endif

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*                           **
    **  Activate SS Confirmation **
    **                           */

    case CM_ACTIVATE_SS_CONF:

      CM_MSG_HIGH_0("RXD: CM_ACTIVATE_SS_CONF");

      asubs_id = cm_rpt_ptr->cmd.activate_conf.as_id;
      cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

      /* Null check cmsups_sub_info_ptr */
      if (cmsups_sub_info_ptr == NULL)
      {
        CM_ERR_1("cmsups_rpt_proc: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
                 asubs_id);
        return;
      }

      /* Update the number of pending sups requests */
      if (cmsups_sub_info_ptr->num_pending_sups_cmds != 0)
        cmsups_sub_info_ptr->num_pending_sups_cmds--;

      cmsups_update_asubs_id(cm_rpt_ptr->cmd.activate_conf.as_id, &sups_info_ptr);

#ifdef FEATURE_SUPS_RETRY
      sups_list_info_ptr = cmwsups_info_list_find(asubs_id, cm_rpt_ptr->cmd.activate_conf.cm_ss_ref);
#endif

#ifdef FEATURE_SS_L3_DATA
      cmwsups_copy_suppsvc_to_sups_info(asubs_id,
                                        &cm_rpt_ptr->cmd.activate_conf.suppsvc_result);
#endif

      sups_info_ptr->ss_ref = cm_rpt_ptr->cmd.activate_conf.cm_ss_ref;

      if (cm_rpt_ptr->cmd.activate_conf.success)
      {
        cmsups_conf_copy(asubs_id, &cm_rpt_ptr->cmd.activate_conf.ss_info);
        sups_info_ptr->ss_success = TRUE;
      }
      else
      {
        /* error in the message received from the network */
        sups_info_ptr->ss_success = FALSE;
      }

      /* Multi-sim: Resume ps data */
      cmsups_msim_resume_data(asubs_id);

#if defined(FEATURE_SUPS_RETRY)
      if (((cm_rpt_ptr->cmd.activate_conf.ss_info.info_type == CC_CAUSE_INFO &&
            (cmwsups_check_redial_cc_cause(cm_rpt_ptr->cmd.activate_conf.ss_info.info.cm_cc_cause.cause_value) == TRUE)) ||
           (cm_rpt_ptr->cmd.activate_conf.ss_info.info_type == SS_ERROR_INFO&&
            cm_rpt_ptr->cmd.activate_conf.ss_info.info.cm_ss_error.error_code == AS_REJ_ABORT_RADIO_UNAVAILABLE)) &&
          sups_list_info_ptr != NULL)
      {
        /* If retry timer will expire before next retry...send the event now */
        if (sups_list_info_ptr->sr_start_uptime  > (uptime + SUPS_NEXT_RETRY_DURATION - SUPS_RETRY_MAX_DURATION))
        {
          /* Radio Unavailable....initiate retry */
          sups_list_info_ptr->next_orig_uptime = uptime + SUPS_NEXT_RETRY_DURATION;

          CM_MSG_HIGH_3("SUPS failed, Initiate Retry. ss_ref %d cause %d next uptime %d",
                        sups_info_ptr->ss_ref,
                        cm_rpt_ptr->cmd.activate_conf.ss_info.info.cm_cc_cause.cause_value,
                        sups_list_info_ptr->next_orig_uptime);

          /* Set flag to not free the corresponding memory */
          is_retry = TRUE;
        }
        /*  Revert the sups setting if there is a failure after sending sups req to NAS
        ** and no more retries are possible */
        else
        {
          cmwsups_update_ue_based_sups(sups_list_info_ptr->sups_cmd_info.ss_code,FALSE, asubs_id);
          cmwsups_set_prev_ue_based_sups(sups_list_info_ptr->sups_cmd_info.ss_code,FALSE, asubs_id, TRUE);
        }
      }
      else if(((cm_rpt_ptr->cmd.activate_conf.ss_info.info_type == CC_CAUSE_INFO) ||
        (cm_rpt_ptr->cmd.activate_conf.ss_info.info_type == SS_ERROR_INFO )) && sups_list_info_ptr != NULL)
      {
         cmwsups_update_ue_based_sups(sups_list_info_ptr->sups_cmd_info.ss_code,FALSE, asubs_id);
         cmwsups_set_prev_ue_based_sups(sups_list_info_ptr->sups_cmd_info.ss_code,FALSE, asubs_id, TRUE);
      }
#endif

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*                            **
    ** Deactivate SS Confirmation **
    **                            */

    case CM_DEACTIVATE_SS_CONF:

      CM_MSG_HIGH_0("RXD: CM_DEACTIVATE_SS_CONF");

      asubs_id = cm_rpt_ptr->cmd.deactivate_conf.as_id;
      cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

      /* Null check cmsups_sub_info_ptr */
      if (cmsups_sub_info_ptr == NULL)
      {
        CM_ERR_1("cmsups_rpt_proc: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
                 asubs_id);
        return;
      }

      /* Update the number of pending sups requests */
      if (cmsups_sub_info_ptr->num_pending_sups_cmds != 0)
        cmsups_sub_info_ptr->num_pending_sups_cmds--;

      cmsups_update_asubs_id(cm_rpt_ptr->cmd.deactivate_conf.as_id, &sups_info_ptr);

#ifdef FEATURE_SUPS_RETRY
      sups_list_info_ptr = cmwsups_info_list_find(asubs_id, cm_rpt_ptr->cmd.deactivate_conf.cm_ss_ref);
#endif

#ifdef FEATURE_SS_L3_DATA
      cmwsups_copy_suppsvc_to_sups_info(asubs_id,
                                        &cm_rpt_ptr->cmd.deactivate_conf.suppsvc_result);
#endif

      sups_info_ptr->ss_ref = cm_rpt_ptr->cmd.deactivate_conf.cm_ss_ref;
      if (cm_rpt_ptr->cmd.deactivate_conf.success)
      {
        cmsups_conf_copy(asubs_id, &cm_rpt_ptr->cmd.deactivate_conf.ss_info);
        sups_info_ptr->ss_success = TRUE;
      }
      else
      {
        /* error in the message received from the network */
        sups_info_ptr->ss_success = FALSE;
      }

      /* Multi-sim: Resume ps data */
      cmsups_msim_resume_data(asubs_id);

#if defined(FEATURE_SUPS_RETRY)
      if (((cm_rpt_ptr->cmd.deactivate_conf.ss_info.info_type == CC_CAUSE_INFO &&
            (cmwsups_check_redial_cc_cause(cm_rpt_ptr->cmd.deactivate_conf.ss_info.info.cm_cc_cause.cause_value) == TRUE)) ||
           (cm_rpt_ptr->cmd.deactivate_conf.ss_info.info_type == SS_ERROR_INFO &&
            cm_rpt_ptr->cmd.deactivate_conf.ss_info.info.cm_ss_error.error_code == AS_REJ_ABORT_RADIO_UNAVAILABLE)) &&
          sups_list_info_ptr != NULL)
      {
        /* If retry timer will expire before next retry...send the event now */
        if (sups_list_info_ptr->sr_start_uptime  > (uptime + SUPS_NEXT_RETRY_DURATION - SUPS_RETRY_MAX_DURATION))
        {
          /* Radio Unavailable....initiate retry */
          sups_list_info_ptr->next_orig_uptime = uptime + SUPS_NEXT_RETRY_DURATION;

          CM_MSG_HIGH_3("SUPS failed, Initiate Retry. ss_ref %d cause %d next uptime %d",
                        sups_info_ptr->ss_ref,
                        cm_rpt_ptr->cmd.deactivate_conf.ss_info.info.cm_cc_cause.cause_value,
                        sups_list_info_ptr->next_orig_uptime);

          /* Set flag to not free the corresponding memory */
          is_retry = TRUE;
        }
        /*  Revert the sups setting if there is a failure after sending sups req to NAS
         ** and no more retries are possible */
        else
        {
          cmwsups_update_ue_based_sups(sups_list_info_ptr->sups_cmd_info.ss_code,TRUE, asubs_id);
          cmwsups_set_prev_ue_based_sups(sups_list_info_ptr->sups_cmd_info.ss_code,TRUE, asubs_id, TRUE);
        }
      }
      else if(((cm_rpt_ptr->cmd.activate_conf.ss_info.info_type == CC_CAUSE_INFO) ||
        (cm_rpt_ptr->cmd.activate_conf.ss_info.info_type == SS_ERROR_INFO )) && sups_list_info_ptr != NULL)
      {
         cmwsups_update_ue_based_sups(sups_list_info_ptr->sups_cmd_info.ss_code,TRUE, asubs_id); 
         cmwsups_set_prev_ue_based_sups(sups_list_info_ptr->sups_cmd_info.ss_code,TRUE, asubs_id, TRUE);
      }

#endif

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*                             **
    ** Interrogate SS Confirmation **
    **                             */

    case CM_INTERROGATE_SS_CONF:

      CM_MSG_HIGH_0("RXD: CM_INTERROGATE_SS_CONF");

      asubs_id = cm_rpt_ptr->cmd.interrogate_conf.as_id;
      cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

      /* Null check cmsups_sub_info_ptr */
      if (cmsups_sub_info_ptr == NULL)
      {
        CM_ERR_1("cmsups_rpt_proc: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
                 asubs_id);
        return;
      }

      /* Update the number of pending sups requests */
      if (cmsups_sub_info_ptr->num_pending_sups_cmds != 0)
        cmsups_sub_info_ptr->num_pending_sups_cmds--;

      cmsups_update_asubs_id(cm_rpt_ptr->cmd.interrogate_conf.as_id, &sups_info_ptr);

#ifdef FEATURE_SUPS_RETRY
      sups_list_info_ptr = cmwsups_info_list_find(asubs_id, cm_rpt_ptr->cmd.interrogate_conf.cm_ss_ref);
#endif

#ifdef FEATURE_SS_L3_DATA
      cmwsups_copy_suppsvc_to_sups_info(asubs_id,
                                        &cm_rpt_ptr->cmd.interrogate_conf.suppsvc_result);
#endif

      sups_info_ptr->ss_ref = cm_rpt_ptr->cmd.interrogate_conf.cm_ss_ref;

      if (cm_rpt_ptr->cmd.interrogate_conf.success)
      {
        sups_info_ptr->ss_success = TRUE;

        interr_conf_ptr = &cm_rpt_ptr->cmd.interrogate_conf.InterrogateSS_Res;

        /* confirmation type */
        sups_info_ptr->conf_type = cm_rpt_ptr->cmd.interrogate_conf.res_type;

        /* save the interrogate SS response */
        switch (sups_info_ptr->conf_type)
        {
          /* no information */
          case NO_INFO:
            break;
          /* ss status */
          case SS_STATUS_INFO:
            sups_info_ptr->ss_status = interr_conf_ptr->cm_ss_status;
            break;

          /* cause */
          case CC_CAUSE_INFO:
            sups_info_ptr->cc_cause = interr_conf_ptr->cm_cc_cause;
            break;

          /* ss error */
          case SS_ERROR_INFO:
            sups_info_ptr->ss_error = interr_conf_ptr->cm_ss_error;
            break;

          /* CLI restriction */
          case CLI_RESTRICTION_INFO:
            sups_info_ptr->cli_restriction = interr_conf_ptr->cli_restriction_info;
            break;

          /* Forwarding Feature List */
          case FWD_FEATURE_LIST_INFO:
            sups_info_ptr->fwd_feature_list =
              interr_conf_ptr->forwarding_feature_list;
            /* if there is a forwarded to number, convert the number
            from BCD to ASCII */
            if (sups_info_ptr->fwd_feature_list.forwarding_feature_list_length != 0)
            {
              for (i = 0; i < sups_info_ptr->fwd_feature_list.forwarding_feature_list_length; i++)
              {
                for (j = 0; j < MAXNO_FORWARD_TO_NUMBER; j++)
                {
                  if (sups_info_ptr->fwd_feature_list.forwarding_feature[i].forwarded_to_number[j].present)
                  {
                    memscpy(
                      &sups_info_ptr->fwd_feature_list.forwarding_feature[i].forwarded_to_number[j],
                      sizeof(sups_info_ptr->fwd_feature_list.forwarding_feature[i].forwarded_to_number[j]),
                      &interr_conf_ptr->forwarding_feature_list.forwarding_feature[i].forwarded_to_number[j],
                      sizeof(interr_conf_ptr->forwarding_feature_list.forwarding_feature[i].forwarded_to_number[j]));
                  }
                }
              }
            }
            break;

#if defined(FEATURE_CCBS)

          /* CCBS information */
          case CCBS_INFO:
            /* Returns list of ccbs indexes stored at network
            */
            sups_info_ptr->ccbs_info = interr_conf_ptr->ccbs_info;

            if (!sups_info_ptr->ccbs_info.present)
            {
              break;
            }

            /* clear all ccbs index entries from Ph object
            */
            cmph_clear_ccbs_entries(asubs_id);

            /* ccbs index store in ph object is replaced with list returned
            ** from interrogate conf
            */
            for (indx_count = 0;
                 indx_count < (int)ARR_SIZE(interr_conf_ptr->ccbs_info.ccbs_feature_list);
                 ++indx_count)
            {
              ccbs_indicator_ptr = &interr_conf_ptr->ccbs_info.ccbs_feature_list[indx_count];

              if ((ccbs_indicator_ptr->present == TRUE) &&
                  (ccbs_indicator_ptr->ccbsIndex.present == TRUE)
                 )
              {
                if (!cmph_add_ccbs_entry(asubs_id,
                                         ccbs_indicator_ptr->ccbsIndex.ccbs_Index,
                                         ccbs_indicator_ptr->subscriberNumber[0])
                   )
                {
                  CM_ERR_0("Error in adding ccbs entry");
                }
              }
            }
            break;
#endif /* FEATURE_CCBS */

          /* Basic Service Group List */
          case BSG_LIST_INFO:
            sups_info_ptr->bsg_list = interr_conf_ptr->basic_service_group_list;
            break;

          /* Forwarded to Number */
          case FWD_TO_NUM_INFO:
          case CALLED_PARTY_NUM_INFO:
            break;

          default:
            CM_MSG_HIGH_1("ERR: Unknown Confirmation type %d received", sups_info_ptr->conf_type);
            /* required if lower layer does not set the unknown info type */
            sups_info_ptr->conf_type = UNKNOWN_CONF_INFO;
            break;
        }
      }
      else
      {
        /* error in the message received from the network */
        sups_info_ptr->ss_success = FALSE;

      }

      /* Multi-sim: Resume ps data */
      cmsups_msim_resume_data(asubs_id);

#if defined(FEATURE_SUPS_RETRY)
      if (((cm_rpt_ptr->cmd.interrogate_conf.res_type == CC_CAUSE_INFO &&
            (cmwsups_check_redial_cc_cause(cm_rpt_ptr->cmd.interrogate_conf.InterrogateSS_Res.cm_cc_cause.cause_value) == TRUE)) ||
           (cm_rpt_ptr->cmd.interrogate_conf.res_type == SS_ERROR_INFO &&
            cm_rpt_ptr->cmd.interrogate_conf.InterrogateSS_Res.cm_ss_error.error_code == AS_REJ_ABORT_RADIO_UNAVAILABLE)) &&
          sups_list_info_ptr != NULL)
      {
        /* If retry timer will expire before next retry...send the event now */
        if (sups_list_info_ptr->sr_start_uptime > (uptime + SUPS_NEXT_RETRY_DURATION - SUPS_RETRY_MAX_DURATION))
        {
          /* Radio Unavailable....initiate retry */
          sups_list_info_ptr->next_orig_uptime = uptime + SUPS_NEXT_RETRY_DURATION;

          CM_MSG_HIGH_3("SUPS failed, Initiate Retry. ss_ref %d cause %d next uptime %d",
                        sups_info_ptr->ss_ref,
                        cm_rpt_ptr->cmd.interrogate_conf.InterrogateSS_Res.cm_cc_cause.cause_value,
                        sups_list_info_ptr->next_orig_uptime);

          /* Set flag to not free the corresponding memory */
          is_retry = TRUE;
        }
      }
#endif

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*                                **
    ** Register Password Confirmation **
    **                                */

    case CM_REGISTER_PASSWORD_CONF:

      CM_MSG_HIGH_0("RXD: CM_REGISTER_PASSWORD_CONF");

      asubs_id = cm_rpt_ptr->cmd.reg_password_conf.as_id;
      cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

      /* Null check cmsups_sub_info_ptr */
      if (cmsups_sub_info_ptr == NULL)
      {
        CM_ERR_1("cmsups_rpt_proc: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
                 asubs_id);
        return;
      }

      /* Update the number of pending sups requests */
      if (cmsups_sub_info_ptr->num_pending_sups_cmds != 0)
        cmsups_sub_info_ptr->num_pending_sups_cmds--;

      cmsups_update_asubs_id(cm_rpt_ptr->cmd.reg_password_conf.as_id, &sups_info_ptr);

#ifdef FEATURE_SUPS_RETRY
      sups_list_info_ptr = cmwsups_info_list_find(asubs_id, cm_rpt_ptr->cmd.reg_password_conf.cm_ss_ref);
#endif

#ifdef FEATURE_SS_L3_DATA
      cmwsups_copy_suppsvc_to_sups_info(asubs_id,
                                        &cm_rpt_ptr->cmd.reg_password_conf.suppsvc_result);
#endif

      sups_info_ptr->ss_ref = cm_rpt_ptr->cmd.reg_password_conf.cm_ss_ref;

      if (cm_rpt_ptr->cmd.reg_password_conf.success)
      {
        sups_info_ptr->ss_success = TRUE;

        /* confirmation type */
        sups_info_ptr->conf_type = cm_rpt_ptr->cmd.reg_password_conf.info.data_type;

        switch (sups_info_ptr->conf_type)
        {
          /* no information */
          case NO_INFO:
            break;

          /* SS error */
          case SS_ERROR_INFO:
            sups_info_ptr->ss_error =
              cm_rpt_ptr->cmd.reg_password_conf.info.data.cm_ss_error;
            break;

          /* New password */
          case NEW_PWD_INFO:
            sups_info_ptr->ss_password =
              cm_rpt_ptr->cmd.reg_password_conf.info.data.ss_new_password;
            break;

          /* Cause */
          case CC_CAUSE_INFO:
            sups_info_ptr->cc_cause =
              cm_rpt_ptr->cmd.reg_password_conf.info.data.cm_cc_cause;
            break;

          default:
            CM_MSG_HIGH_1("ERROR: Unknown Confirmation type %d received",
                          sups_info_ptr->conf_type);
            /* required if lower layer does not set the unknown info type */
            sups_info_ptr->conf_type = UNKNOWN_CONF_INFO;
            break;
        }
      }
      else
      {
        sups_info_ptr->ss_success = FALSE;
      }

      /* Multi-sim: Resume ps data */
      cmsups_msim_resume_data(asubs_id);

#if defined(FEATURE_SUPS_RETRY)
      if (((cm_rpt_ptr->cmd.reg_password_conf.info.data_type == CC_CAUSE_INFO &&
            (cmwsups_check_redial_cc_cause(cm_rpt_ptr->cmd.reg_password_conf.info.data.cm_cc_cause.cause_value) == TRUE)) ||
           (cm_rpt_ptr->cmd.reg_password_conf.info.data_type == SS_ERROR_INFO &&
            cm_rpt_ptr->cmd.reg_password_conf.info.data.cm_ss_error.error_code == AS_REJ_ABORT_RADIO_UNAVAILABLE)) &&
          sups_list_info_ptr != NULL)
      {
        /* If retry timer will expire before next retry...send the event now */
        if (sups_list_info_ptr->sr_start_uptime  > (uptime + SUPS_NEXT_RETRY_DURATION - SUPS_RETRY_MAX_DURATION))
        {
          /* Radio Unavailable....initiate retry */
          sups_list_info_ptr->next_orig_uptime = uptime + SUPS_NEXT_RETRY_DURATION;

          CM_MSG_HIGH_3("SUPS failed, Initiate Retry. ss_ref %d cause %d next uptime %d",
                        sups_info_ptr->ss_ref,
                        cm_rpt_ptr->cmd.reg_password_conf.info.data.cm_cc_cause.cause_value,
                        sups_list_info_ptr->next_orig_uptime);

          /* Set flag to not free the corresponding memory */
          is_retry = TRUE;
        }
      }
#endif

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*                             **
    ** Process USS Confirmation    **
    **                             */
    case CM_PROCESS_UNSTRUCTURED_SS_CONF:

      CM_MSG_HIGH_0("RXD: CM_PROCESS_UNSTRUCTURED_SS_CONF");

      asubs_id = cm_rpt_ptr->cmd.process_uss_conf.as_id;
      cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

      /* Null check cmsups_sub_info_ptr */
      if (cmsups_sub_info_ptr == NULL)
      {
        CM_ERR_1("cmsups_rpt_proc: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
                 asubs_id);
        return;
      }

      /* Update the number of pending sups requests */
      if (cmsups_sub_info_ptr->num_pending_sups_cmds != 0)
        cmsups_sub_info_ptr->num_pending_sups_cmds--;

      cmsups_update_asubs_id(cm_rpt_ptr->cmd.process_uss_conf.as_id, &sups_info_ptr);

#ifdef FEATURE_SUPS_RETRY
      sups_list_info_ptr = cmwsups_info_list_find(asubs_id, cm_rpt_ptr->cmd.process_uss_conf.cm_ss_ref);
#endif

#ifdef FEATURE_SS_L3_DATA
      cmwsups_copy_suppsvc_to_sups_info(asubs_id,
                                        &cm_rpt_ptr->cmd.process_uss_conf.suppsvc_result);
#endif

      sups_info_ptr->ss_ref = cm_rpt_ptr->cmd.process_uss_conf.cm_ss_ref;

      if (cm_rpt_ptr->cmd.process_uss_conf.success)
      {
        sups_info_ptr->ss_success = TRUE;

        /* confirmation type */
        sups_info_ptr->conf_type = cm_rpt_ptr->cmd.process_uss_conf.ss_data_info.data_type;

        if (((cm_rpt_ptr->cmd.process_uss_conf.ss_data_info.data_type == SS_ERROR_INFO) &&
             (cm_rpt_ptr->cmd.process_uss_conf.ss_data_info.data.cm_ss_error.error_code_tag ==
              INVOKE_PROBLEM) &&
             (cm_rpt_ptr->cmd.process_uss_conf.ss_data_info.data.cm_ss_error.error_code ==
              UNRECOGNISED_OPERATION))
            || ((cm_rpt_ptr->cmd.process_uss_conf.ss_data_info.data_type == CC_CAUSE_INFO) &&
                (cm_rpt_ptr->cmd.process_uss_conf.ss_data_info.data.cm_cc_cause.cause_value
                 == FACILITY_REJECTED)
               )
           )
        {
          /*
          ** the ussd request was not understood by the network,
          ** resend a phase1 message.
          */
          CM_MSG_HIGH_0("Send PHASE 1 USS MESSAGE");

          uss_phase1_msg = TRUE;

          cmsups_ussd_phase1_req(asubs_id);

          /* Needs to maintain a new local instead of uss_phase1_msg
          because, real MS USSD events are atomic, but UT USSD
          events are triggered, one after one before getting the
          previous transaction completed, we will just break in
          mid when we have received incorrect USS_CONF  */

          is_break_in_mid = TRUE;

          break;
        }

        sups_info_ptr->uss_data_type = CM_USS_PHASE2_TYPE;

        switch (sups_info_ptr->conf_type)
        {
          /* no information */
          case NO_INFO:
            break;

          /* USS data */
          case USS_DATA_INFO:
            sups_info_ptr->uss_data =
              cm_rpt_ptr->cmd.process_uss_conf.ss_data_info.data.uss_data;
            break;

          /* SS error */
          case SS_ERROR_INFO:
            sups_info_ptr->ss_error =
              cm_rpt_ptr->cmd.process_uss_conf.ss_data_info.data.cm_ss_error;
            break;

          /* Cause */
          case CC_CAUSE_INFO:
            sups_info_ptr->cc_cause =
              cm_rpt_ptr->cmd.process_uss_conf.ss_data_info.data.cm_cc_cause;
            break;

          default:
            CM_MSG_HIGH_1("ERR: Unknown Confirmation type %d received",
                          sups_info_ptr->conf_type);
            /* required if lower layer does not set the unknown info type */
            sups_info_ptr->conf_type = UNKNOWN_CONF_INFO;
            break;
        }

        /* set the USS data type for the client processing */
        sups_info_ptr->uss_data_type = CM_USS_PHASE2_TYPE;

      }
      else
      {
        sups_info_ptr->ss_success = FALSE;
      }

      /* Multi-sim: Resume ps data */
      cmsups_msim_resume_data(asubs_id);

#if defined(FEATURE_SUPS_RETRY)
      if (((cm_rpt_ptr->cmd.process_uss_conf.ss_data_info.data_type == CC_CAUSE_INFO &&
            (cmwsups_check_redial_cc_cause(cm_rpt_ptr->cmd.process_uss_conf.ss_data_info.data.cm_cc_cause.cause_value) == TRUE)) ||
           (cm_rpt_ptr->cmd.process_uss_conf.ss_data_info.data_type == SS_ERROR_INFO &&
            cm_rpt_ptr->cmd.process_uss_conf.ss_data_info.data.cm_ss_error.error_code == AS_REJ_ABORT_RADIO_UNAVAILABLE)) &&
          sups_list_info_ptr != NULL)
      {
        /* If retry timer will expire before next retry...send the event now */
        if (sups_list_info_ptr->sr_start_uptime  > (uptime + SUPS_NEXT_RETRY_DURATION - SUPS_RETRY_MAX_DURATION))
        {
          /* Radio Unavailable....initiate retry */
          sups_list_info_ptr->next_orig_uptime = uptime + SUPS_NEXT_RETRY_DURATION;

          CM_MSG_HIGH_3("SUPS failed, Initiate Retry. ss_ref %d cause %d next uptime %d",
                        sups_info_ptr->ss_ref,
                        cm_rpt_ptr->cmd.process_uss_conf.ss_data_info.data.cm_cc_cause.cause_value,
                        sups_list_info_ptr->next_orig_uptime);

          /* Set flag to not free the corresponding memory */
          is_retry = TRUE;
        }
      }
#endif

      break;


    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*                             **
    ** Process USSD Confirmation   **
    **                             */
    case CM_PROCESS_UNSTRUCTURED_SS_DATA_CONF:

      CM_MSG_HIGH_0("RXD: CM_PROCESS_UNSTRUCTURED_SS_DATA_CONF");

      asubs_id = cm_rpt_ptr->cmd.process_uss_data_conf.as_id;
      cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

      /* Null check cmsups_sub_info_ptr */
      if (cmsups_sub_info_ptr == NULL)
      {
        CM_ERR_1("cmsups_rpt_proc: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
                 asubs_id);
        return;
      }

      /* Update the number of pending sups requests */
      if (cmsups_sub_info_ptr->num_pending_sups_cmds != 0)
        cmsups_sub_info_ptr->num_pending_sups_cmds--;

      cmsups_update_asubs_id(cm_rpt_ptr->cmd.process_uss_data_conf.as_id, &sups_info_ptr);

#ifdef FEATURE_SUPS_RETRY
      sups_list_info_ptr = cmwsups_info_list_find(asubs_id, cm_rpt_ptr->cmd.process_uss_data_conf.cm_ss_ref);
#endif

#ifdef FEATURE_SS_L3_DATA
      cmwsups_copy_suppsvc_to_sups_info(asubs_id,
                                        &cm_rpt_ptr->cmd.process_uss_data_conf.suppsvc_result);
#endif

      if ((uss_phase1_msg) &&
          (cm_rpt_ptr->cmd.process_uss_data_conf.success))
      {
        uss_phase1_msg = FALSE;

        sups_info_ptr->ss_success = TRUE;

        sups_info_ptr->uss_data_type = CM_USS_PHASE1_TYPE;

        sups_info_ptr->ss_ref =
          cm_rpt_ptr->cmd.process_uss_data_conf.cm_ss_ref;

        sups_info_ptr->conf_type =
          cm_rpt_ptr->cmd.process_uss_data_conf.ss_UserData_info.data_type;
        /*
        1. copy only USS_SIZE (160 not 200)
        2. Indicate to UI that there is no need to convert to ASCII
        since this is a phase1 message
        */
        switch (sups_info_ptr->conf_type)
        {
          case NO_INFO:
            break;

          /* USS data */
          case USS_DATA_INFO:
            memscpy(sups_info_ptr->uss_data.ussData,
                    sizeof(sups_info_ptr->uss_data.ussData),
                    cm_rpt_ptr->cmd.process_uss_data_conf.ss_UserData_info.data.ss_User_Data.ss_UserData,
                    MAX_USS_BYTES);
            /* use MAX_USS_BYTES here since, from n/w side string would always come in gsm-7
            and, max bytes supported for gsm-7 is 160 */

            sups_info_ptr->uss_data.size = cm_rpt_ptr->cmd.process_uss_data_conf.ss_UserData_info.data.ss_User_Data.size;
            sups_info_ptr->uss_data.present = cm_rpt_ptr->cmd.process_uss_data_conf.ss_UserData_info.data.ss_User_Data.present;
            break;

          /* SS error */
          case SS_ERROR_INFO:
            sups_info_ptr->ss_error =
              cm_rpt_ptr->cmd.process_uss_data_conf.ss_UserData_info.data.cm_ss_error;
            break;

          /* Cause */
          case CC_CAUSE_INFO:
            sups_info_ptr->cc_cause =
              cm_rpt_ptr->cmd.process_uss_data_conf.ss_UserData_info.data.cm_cc_cause;
            break;

          default:
            CM_MSG_HIGH_1("ERR: Unknown Confirmation type %d received",
                          sups_info_ptr->conf_type);
            /* required if lower layer does not set the unknown info type */
            sups_info_ptr->conf_type = UNKNOWN_CONF_INFO;
            break;

        }
      }
      else
      {
        /* USSD phase 1 procedure has completed */
        CM_MSG_HIGH_0("resetting uss_phase1_msg");
        uss_phase1_msg = FALSE;
        sups_info_ptr->ss_success = FALSE;
        sups_info_ptr->ss_ref =
          cm_rpt_ptr->cmd.process_uss_data_conf.cm_ss_ref;
      }

      /* Multi-sim: Resume ps data */
      cmsups_msim_resume_data(asubs_id);

#if defined(FEATURE_SUPS_RETRY)
      if (((cm_rpt_ptr->cmd.process_uss_data_conf.ss_UserData_info.data_type == CC_CAUSE_INFO &&
            (cmwsups_check_redial_cc_cause(cm_rpt_ptr->cmd.process_uss_data_conf.ss_UserData_info.data.cm_cc_cause.cause_value) == TRUE)) ||
           (cm_rpt_ptr->cmd.process_uss_data_conf.ss_UserData_info.data_type == SS_ERROR_INFO &&
            cm_rpt_ptr->cmd.process_uss_data_conf.ss_UserData_info.data.cm_ss_error.error_code == AS_REJ_ABORT_RADIO_UNAVAILABLE)) &&
          sups_list_info_ptr != NULL)
      {
        /* If retry timer will expire before next retry...send the event now */
        if (sups_list_info_ptr->sr_start_uptime  > (uptime + SUPS_NEXT_RETRY_DURATION - SUPS_RETRY_MAX_DURATION))
        {
          /* Radio Unavailable....initiate retry */
          sups_list_info_ptr->next_orig_uptime = uptime + SUPS_NEXT_RETRY_DURATION;

          CM_MSG_HIGH_3("SUPS failed, Initiate Retry. ss_ref %d cause %d next uptime %d",
                        sups_info_ptr->ss_ref,
                        cm_rpt_ptr->cmd.process_uss_data_conf.ss_UserData_info.data.cm_cc_cause.cause_value,
                        sups_list_info_ptr->next_orig_uptime);

          /* Set flag to not free the corresponding memory */
          is_retry = TRUE;
        }
      }
#endif

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*                             **
    ** Forward Check SS Indication **
    **                             */

    case CM_FORWARD_CHECK_SS_INDICATION_IND:

      CM_MSG_HIGH_0("RXD: CM_FWD_CHECK_SS_IND");

      asubs_id = cm_rpt_ptr->cmd.fwd_check_ind.as_id;
      cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

      /* Null check cmsups_sub_info_ptr */
      if (cmsups_sub_info_ptr == NULL)
      {
        CM_ERR_1("cmsups_rpt_proc: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
                 asubs_id);
        return;
      }

      /* Update the number of pending sups requests */
      if (cmsups_sub_info_ptr->num_pending_sups_cmds != 0)
        cmsups_sub_info_ptr->num_pending_sups_cmds--;

      /* Multi-sim: Resume ps data */
      cmsups_msim_resume_data(asubs_id);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*                             **
    ** Unstructured SS Indication  **
    **                             */
    case CM_UNSTRUCTURED_SS_IND:
    {

      cmsups_cmd_route_e_type sups_route;
      if (cmsups_get_mt_uss_routing_context_info(cm_rpt_ptr->cmd.uss_notify_ind.as_id, &sups_route) == TRUE &&
          sups_route == CMSUPS_CMD_ALLOW_IMS)
      {
        ie_cm_ss_error_T         cm_ss_error;
        CM_MSG_HIGH_0("Reject cmd, MT USS on IMS domain is ongoing");
        cm_ss_error.present = TRUE;
        cm_ss_error.error_code_tag = 0x02;
        cm_ss_error.error_code = ussd_Busy;
        cmwsups_send_release_ss_req(cm_rpt_ptr->cmd.uss_ind.invoke_id,
                                    unstructuredSS_Request,
                                    cm_ss_error,
                                    cm_rpt_ptr->cmd.uss_ind.as_id);
        return;
      }

      CM_MSG_HIGH_0("RXD: CM_UNSTRUCTURED_SS_IND");
      cmsups_set_mt_uss_context_info(cm_rpt_ptr->cmd.uss_ind.as_id, CMSUPS_CMD_ALLOW_CS);

      asubs_id = cm_rpt_ptr->cmd.uss_ind.as_id;
      cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

      /* Null check cmsups_sub_info_ptr */
      if (cmsups_sub_info_ptr == NULL)
      {
        CM_ERR_1("cmsups_rpt_proc: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
                 asubs_id);
        return;
      }

      /* Update the number of pending sups requests */
      if (cmsups_sub_info_ptr->num_pending_sups_cmds != 0)
        cmsups_sub_info_ptr->num_pending_sups_cmds--;

      cmsups_update_asubs_id(cm_rpt_ptr->cmd.uss_ind.as_id, &sups_info_ptr);

      /* USS invoke id */
      sups_info_ptr->invoke_id = cm_rpt_ptr->cmd.uss_ind.invoke_id;

      /* USS data */
      sups_info_ptr->uss_data = cm_rpt_ptr->cmd.uss_ind.uss_data;

#ifdef FEATURE_SUPS_RETRY
      {
        cmwsups_info_list_s_type *sups_list = NULL;
        sups_list = cmsups_sub_info_ptr->sups_info_list_head;

        while (sups_list != NULL)
        {
          if (sups_list->cmd == CM_SUPS_CMD_PROCESS_USS)
          {
            CM_MSG_HIGH_0("Free USS Sups list object due to MT USS from network");

            if (sups_list == cmsups_sub_info_ptr->sups_info_list)
            {
              CM_MSG_HIGH_1("sups_ptr->sups_info_list: %d", cmsups_sub_info_ptr->sups_info_list);
              cmsups_sub_info_ptr->sups_info_list = NULL;
            }
            cmwsups_mem_free_sups_list_info(sups_list, &(cmsups_sub_info_ptr->sups_info_list_head));
            CM_MSG_HIGH_1("sups_ptr->sups_info_list_head: %d", cmsups_sub_info_ptr->sups_info_list_head);
            sups_list = NULL;

            break;
          }
          sups_list = sups_list->next; /* Move to next record */
        }
      }
#endif

      /* Multi-sim: Resume ps data */
      cmsups_msim_resume_data(asubs_id);
    }
    break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*                                   **
    ** Unstructured SS Notify Indication **
    **                                   */
    case CM_UNSTRUCTURED_SS_NOTIFY_IND:
    {
      cmsups_cmd_route_e_type sups_route;
      if (cmsups_get_mt_uss_routing_context_info(cm_rpt_ptr->cmd.uss_notify_ind.as_id, &sups_route) == TRUE &&
          sups_route == CMSUPS_CMD_ALLOW_IMS)
      {
        ie_cm_ss_error_T         cm_ss_error;
        CM_MSG_HIGH_0("Reject cmd, MT USS on IMS domain is ongoing");
        cm_ss_error.present = TRUE;
        cm_ss_error.error_code_tag = 0x02;
        cm_ss_error.error_code = ussd_Busy;
        cmwsups_send_release_ss_req(cm_rpt_ptr->cmd.uss_ind.invoke_id,
                                    unstructuredSS_Notify,
                                    cm_ss_error,
                                    cm_rpt_ptr->cmd.uss_ind.as_id);

        return;
      }

      CM_MSG_HIGH_0("RXD: CM_UNSTRUCTURED_SS_NOTIFY_IND");

      cmsups_set_mt_uss_context_info(cm_rpt_ptr->cmd.uss_notify_ind.as_id, CMSUPS_CMD_ALLOW_CS);

      asubs_id = cm_rpt_ptr->cmd.uss_notify_ind.as_id;
      cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

      /* Null check cmsups_sub_info_ptr */
      if (cmsups_sub_info_ptr == NULL)
      {
        CM_ERR_1("cmsups_rpt_proc: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
                 asubs_id);
        return;
      }

      /*When CM received MT USSD IND, it should end and reject any MO USSD that may be in silent redial, only
      ** one such MO USSD is possible to be in silent redial phase  */
#ifdef FEATURE_SUPS_RETRY
      cmwsups_check_and_reject_uss_in_silent_redial(asubs_id);
#endif

      /* Update the number of pending sups requests */
      if (cmsups_sub_info_ptr->num_pending_sups_cmds != 0)
        cmsups_sub_info_ptr->num_pending_sups_cmds--;

      cmsups_update_asubs_id(cm_rpt_ptr->cmd.uss_notify_ind.as_id, &sups_info_ptr);

      /* USS invoke id */
      sups_info_ptr->invoke_id = cm_rpt_ptr->cmd.uss_notify_ind.invoke_id;

      /* USS data */
      sups_info_ptr->uss_data = cm_rpt_ptr->cmd.uss_notify_ind.uss_data;
      if (sups_info_ptr->uss_data.size > MAX_USS_BYTES)
      {
        sups_info_ptr->uss_data.size = MAX_USS_BYTES;
        /* max length can be 160 only in case of GSM-7 bit encoding */
        memset(sups_info_ptr->uss_data.ussData + MAX_USS_BYTES, 0, ((MAX_USS_CHAR_ASCII - MAX_USS_BYTES) + 1));
        CM_MSG_HIGH_1("Truncating USSD msg from n/w to %d bytes", MAX_USS_BYTES);
      }

      /* Multi-sim: Resume ps data */
      cmsups_msim_resume_data(asubs_id);
    }
    break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*                             **
    ** Get Password Indication     **
    **                             */
    case CM_GET_PASSWORD_IND:

      CM_MSG_HIGH_0("RXD: CM_GET_PASSWORD_IND");

      asubs_id = cm_rpt_ptr->cmd.get_password_ind.as_id;
      cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

      /* Null check cmsups_sub_info_ptr */
      if (cmsups_sub_info_ptr == NULL)
      {
        CM_ERR_1("cmsups_rpt_proc: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
                 asubs_id);
        return;
      }

      /* Update the number of pending sups requests */
      if (cmsups_sub_info_ptr->num_pending_sups_cmds != 0)
        cmsups_sub_info_ptr->num_pending_sups_cmds--;

      cmsups_update_asubs_id(cm_rpt_ptr->cmd.get_password_ind.as_id, &sups_info_ptr);

      /* Invoke ID */
      sups_info_ptr->invoke_id = cm_rpt_ptr->cmd.get_password_ind.invoke_id;

      /* Guidance Info */
      sups_info_ptr->guidance_info = (ss_password_value) \
                                     cm_rpt_ptr->cmd.get_password_ind.guidance_info;

      /* Multi-sim: Resume ps data */
      cmsups_msim_resume_data(asubs_id);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*                             **
    ** Release USS Indication      **
    **                             */

    case CM_RELEASE_UNSTRUCTURED_SS_IND:

      CM_MSG_HIGH_0("RXD: CM_RELEASE_USS_IND");

      asubs_id = cm_rpt_ptr->cmd.rel_uss_ind.as_id;
      cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

      /* Null check cmsups_sub_info_ptr */
      if (cmsups_sub_info_ptr == NULL)
      {
        CM_ERR_1("cmsups_rpt_proc: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
                 asubs_id);
        return;
      }

      /* Update the number of pending sups requests */
      if (cmsups_sub_info_ptr->num_pending_sups_cmds != 0)
        cmsups_sub_info_ptr->num_pending_sups_cmds--;

      /* Note: If a cross phase compatibility is required, do not send the
      release uss indication event to the clients till the final
      release complete is received.
      */
      if (!uss_phase1_msg)
      {
        cmsups_update_asubs_id(cm_rpt_ptr->cmd.rel_uss_ind.as_id, &sups_info_ptr);

        /* clear the invocation ID */
        sups_info_ptr->invoke_id = 0;
        cmsups_reset_mt_uss_context_info(cm_rpt_ptr->cmd.rel_uss_ind.as_id);

      }
      else
      {
        /* Free the memory alloacted to sups_info_ptr, after sending the event */
        if (is_mem_free_req == TRUE && cmsups_sub_info_ptr->num_pending_sups_cmds == 0)
        {
          cmsups_mem_free_sups_info(asubs_id);
          cmsups_free_uss_info_ptr(asubs_id);
        }

        return;
      }

      /* Multi-sim: Resume ps data */
      cmsups_msim_resume_data(asubs_id);

#ifdef FEATURE_SUPS_RETRY
      {
        cmwsups_info_list_s_type *sups_list = NULL;
        sups_list = cmsups_sub_info_ptr->sups_info_list_head;

        if (sups_list != NULL)
        {
          while (sups_list != NULL)
          {
            if (sups_list->cmd == CM_SUPS_CMD_PROCESS_USS ||
                sups_list->cmd == CM_SUPS_CMD_RELEASE)
            {
              CM_MSG_HIGH_0("Free USS Sups list object due to rel ind recvd from LL");

              if (sups_list == cmsups_sub_info_ptr->sups_info_list)
              {
                CM_MSG_HIGH_1("sups_ptr->sups_info_list: %d", cmsups_sub_info_ptr->sups_info_list);
                cmsups_sub_info_ptr->sups_info_list = NULL;
              }
              cmwsups_mem_free_sups_list_info(sups_list, &(cmsups_sub_info_ptr->sups_info_list_head));
              CM_MSG_HIGH_1("sups_ptr->sups_info_list_head: %d", cmsups_sub_info_ptr->sups_info_list_head);
              sups_list = NULL;

              break;
            }
            sups_list = sups_list->next; /* Move to next record */
          }
        }
      }
#endif

      break;

#if (defined(FEATURE_CM_MMGPS) || defined(FEATURE_CM_LCS))
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*                                      **
    ** LCS Location Notification Indication **
    **                                      */
    case CM_LCS_LOCATION_NOTIFICATION_IND:

      CM_MSG_HIGH_0("RXD: CM_LCS_LOCATION_NOTIFICATION_IND");

      asubs_id = cm_rpt_ptr->cmd.lcs_location_notification_ind.as_id;
      cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

      /* Null check cmsups_sub_info_ptr */
      if (cmsups_sub_info_ptr == NULL)
      {
        CM_ERR_1("cmsups_rpt_proc: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
                 asubs_id);
        return;
      }

      /* Update the number of pending sups requests */
      if (cmsups_sub_info_ptr->num_pending_sups_cmds != 0)
        cmsups_sub_info_ptr->num_pending_sups_cmds--;

      cmsups_update_asubs_id(cm_rpt_ptr->cmd.lcs_location_notification_ind.as_id, &sups_info_ptr);

      /* LCS Location Notification invoke id */
      sups_info_ptr->invoke_id =
        cm_rpt_ptr->cmd.lcs_location_notification_ind.invoke_id;

      /* Location Notification Args */

      sups_info_ptr->location_notification =
        cm_rpt_ptr->cmd.lcs_location_notification_ind.location_notification;

      /* Multi-sim: Resume ps data */
      cmsups_msim_resume_data(asubs_id);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*                             **
    ** LCS MOLR Confirmation       **
    **                             */

    case CM_LCS_MOLR_CONF:

      CM_MSG_HIGH_0("RXD: CM_LCS_MOLR_CONF");

      asubs_id = cm_rpt_ptr->cmd.lcs_molr_conf.as_id;
      cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

      /* Null check cmsups_sub_info_ptr */
      if (cmsups_sub_info_ptr == NULL)
      {
        CM_ERR_1("cmsups_rpt_proc: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
                 asubs_id);
        return;
      }

      /* Update the number of pending sups requests */
      if (cmsups_sub_info_ptr->num_pending_sups_cmds != 0)
        cmsups_sub_info_ptr->num_pending_sups_cmds--;

      cmsups_update_asubs_id(cm_rpt_ptr->cmd.lcs_molr_conf.as_id, &sups_info_ptr);

#ifdef FEATURE_SUPS_RETRY
      sups_list_info_ptr = cmwsups_info_list_find(asubs_id, cm_rpt_ptr->cmd.lcs_molr_conf.cm_ss_ref);
#endif

#ifdef FEATURE_SS_L3_DATA
      cmwsups_copy_suppsvc_to_sups_info(asubs_id,
                                        &cm_rpt_ptr->cmd.lcs_molr_conf.suppsvc_result);
#endif

      sups_info_ptr->ss_ref = cm_rpt_ptr->cmd.lcs_molr_conf.cm_ss_ref;
      sups_info_ptr->ss_success = cm_rpt_ptr->cmd.lcs_molr_conf.success;
      sups_info_ptr->lcs_molr_res = cm_rpt_ptr->cmd.lcs_molr_conf.lcs_molr_res;

      /* Multi-sim: Resume ps data */
      cmsups_msim_resume_data(asubs_id);

#if defined(FEATURE_SUPS_RETRY)
      if (((cm_rpt_ptr->cmd.lcs_molr_conf.lcs_molr_res.data_type == CC_CAUSE_INFO &&
            (cmwsups_check_redial_cc_cause(cm_rpt_ptr->cmd.lcs_molr_conf.lcs_molr_res.data.cm_cc_cause.cause_value) == TRUE)) ||
           (cm_rpt_ptr->cmd.lcs_molr_conf.lcs_molr_res.data_type == SS_ERROR_INFO &&
            cm_rpt_ptr->cmd.lcs_molr_conf.lcs_molr_res.data.cm_ss_error.error_code == AS_REJ_ABORT_RADIO_UNAVAILABLE)) &&
          sups_list_info_ptr != NULL)
      {
        /* If retry timer will expire before next retry...send the event now */
        if (sups_list_info_ptr->sr_start_uptime  > (uptime + SUPS_NEXT_RETRY_DURATION - SUPS_RETRY_MAX_DURATION))
        {
          /* Radio Unavailable....initiate retry */
          sups_list_info_ptr->next_orig_uptime = uptime + SUPS_NEXT_RETRY_DURATION;

          CM_MSG_HIGH_3("SUPS failed, Initiate Retry. ss_ref %d cause %d next uptime %d",
                        sups_info_ptr->ss_ref,
                        cm_rpt_ptr->cmd.lcs_molr_conf.lcs_molr_res.data.cm_cc_cause.cause_value,
                        sups_list_info_ptr->next_orig_uptime);

          /* Set flag to not free the corresponding memory */
          is_retry = TRUE;
        }
      }
#endif

      break;

#endif

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*                             **
    ** Erase CC ENTRY Confirmation **
    **                             */

    case CM_ERASE_CC_ENTRY_CONF:

      asubs_id = cm_rpt_ptr->cmd.erase_cc_entry_conf.as_id;
      cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

      /* Null check cmsups_sub_info_ptr */
      if (cmsups_sub_info_ptr == NULL)
      {
        CM_ERR_1("cmsups_rpt_proc: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
                 asubs_id);
        return;
      }

#if defined(FEATURE_CCBS)

      CM_MSG_HIGH_0("RXD: CM_ERASE_CC_ENTRY_CONF");

      /* Update the number of pending sups requests */
      if (cmsups_sub_info_ptr->num_pending_sups_cmds != 0)
        cmsups_sub_info_ptr->num_pending_sups_cmds--;

      cmsups_update_asubs_id(cm_rpt_ptr->cmd.erase_cc_entry_conf.as_id, &sups_info_ptr);

#ifdef FEATURE_SUPS_RETRY
      sups_list_info_ptr = cmwsups_info_list_find(asubs_id, cm_rpt_ptr->cmd.erase_cc_entry_conf.cm_ss_ref);
#endif

      sups_info_ptr->ss_ref = cm_rpt_ptr->cmd.erase_cc_entry_conf.cm_ss_ref;

#if defined(FEATURE_SUPS_RETRY)
      if (((cm_rpt_ptr->cmd.erase_cc_entry_conf.ss_info.info_type == CC_CAUSE_INFO &&
            (cmwsups_check_redial_cc_cause(cm_rpt_ptr->cmd.erase_cc_entry_conf.ss_info.info.cm_cc_cause.cause_value) == TRUE)) ||
           (cm_rpt_ptr->cmd.erase_cc_entry_conf.ss_info.info_type == SS_ERROR_INFO &&
            cm_rpt_ptr->cmd.erase_cc_entry_conf.ss_info.info.cm_ss_error.error_code == AS_REJ_ABORT_RADIO_UNAVAILABLE)) &&
          sups_list_info_ptr != NULL)
      {
        /* If retry timer will expire before next retry...send the event now */
        if (sups_list_info_ptr->sr_start_uptime  > (uptime + SUPS_NEXT_RETRY_DURATION - SUPS_RETRY_MAX_DURATION))
        {
          /* Radio Unavailable....initiate retry */
          sups_list_info_ptr->next_orig_uptime = uptime + SUPS_NEXT_RETRY_DURATION;

          CM_MSG_HIGH_3("SUPS failed, Initiate Retry. ss_ref %d cause %d next uptime %d",
                        sups_info_ptr->ss_ref,
                        cm_rpt_ptr->cmd.erase_cc_entry_conf.ss_info.info.cm_cc_cause.cause_value,
                        sups_list_info_ptr->next_orig_uptime);

          /* Set flag to not free the corresponding memory */
          is_retry = TRUE;
        }
      }
#endif

      if (!cm_rpt_ptr->cmd.erase_cc_entry_conf.success)
      {
        sups_info_ptr->ss_success = FALSE;
        break;
      }

      sups_info_ptr->ss_success = TRUE;

      cmsups_conf_copy(asubs_id, &cm_rpt_ptr->cmd.erase_cc_entry_conf.ss_info);

      /* info_type is NO_INFO when CCBS deactivation is successfull
      ** info_type is SS_ERROR_INFO for all other cases
      */
      if (cm_rpt_ptr->cmd.erase_cc_entry_conf.ss_info.info_type == NO_INFO)
      {
        /* Get ccbs index from SS object
        */
        if (!((cmsups_sub_info_ptr->ccbs_erase_pend_indx.is_erase_cc_conf_pend) &&

              (cmsups_sub_info_ptr->ccbs_erase_pend_indx.cm_ss_ref ==
               cm_rpt_ptr->cmd.erase_cc_entry_conf.cm_ss_ref)
             )
           )
        {
          CM_ERR_0("Erase CC Conf is not matching our request");
        }

        if (cmsups_sub_info_ptr->ccbs_erase_pend_indx.ccbs_indx != CM_CCBS_INDX_INVALID)
        {
          if (!cmph_remove_ccbs_entry(asubs_id,
                                      cmsups_sub_info_ptr->ccbs_erase_pend_indx.ccbs_indx))
          {
            CM_ERR_0("ccbs entry could not be removed");
          }
        }
        else
        {
          /* clear all ccbs index entries from Ph object
          */
          cmph_clear_ccbs_entries(asubs_id);
        }

        cmsups_sub_info_ptr->ccbs_erase_pend_indx.is_erase_cc_conf_pend = FALSE;
      }

      /*
      ** if ss_code is ccbs then deactivation gets converted into EraseCCEntry
      ** request (24.093 section 4.4).
      ** Since client sends a deactivation request, they expect a deactivate
      ** sups event
      */

      /* Multi-sim: Resume ps data */
      cmsups_msim_resume_data(asubs_id);

      break;

#else
      /* Free the memory alloacted to sups_info_ptr, after sending the event */
      if (is_mem_free_req == TRUE && cmsups_sub_info_ptr->num_pending_sups_cmds == 0)
      {
        cmsups_mem_free_sups_info(asubs_id);
        cmsups_free_uss_info_ptr(asubs_id);
      }
      return;
#endif

    default:
      is_mem_free_req = FALSE;

      /* Abort_cc_req may be sent due to subs chg on other sub due to which stack needs to be shut down
      If we have received the abort_cc_cnf for any asid which means that stack will be shutting down
      so we need to clear the sups context and send the failure event to the clients to avoid them
      from waiting forever
      */

#ifdef FEATURE_SUPS_RETRY
      if (cm_rpt_ptr->hdr.cmd == CM_ABORT_CC_CONF)
      {
        /* Clear the pending Sups command since the stack will be shutting down after stop mode.
        */
        /* NAS sends ASID in case of DUAL SIM , Get SS for given asubs_id */
#ifdef FEATURE_MMODE_DUAL_SIM
        asubs_id = cm_rpt_ptr->cmd.cm_abort_cc_cnf.as_id;
#else
        asubs_id = SYS_MODEM_AS_ID_1;
#endif
        CM_MSG_HIGH_1_EXT("Recvd abort_cc_cnf on ASID = %d",
                          cm_rpt_ptr->cmd.cm_abort_cc_cnf.as_id,
                          (asubs_id+1));
        cmwsups_list_clear(asubs_id);
        cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);
        if(cmsups_sub_info_ptr != NULL && cmsups_sub_info_ptr->buffered_mn_cmd_ptr != NULL)
        {
          cm_mem_free(cmsups_sub_info_ptr->buffered_mn_cmd_ptr);
          cmsups_sub_info_ptr->buffered_mn_cmd_ptr = NULL;
          cmsups_sub_info_ptr->buffered_mn_cmd_timer = 0;
        }
      }
#endif
      return;
  }

  sups_event_type = cmsups_conf_to_event_map(cm_rpt_ptr->hdr.cmd);

  if ((is_break_in_mid == FALSE) && (sups_event_type != CM_SUPS_EVENT_NONE))
  {
#ifdef FEATURE_SUPS_RETRY
    if (is_retry == FALSE)
#endif
      cmsups_event(asubs_id, sups_event_type);

#ifdef FEATURE_CM_UPDATE_EF_CFIS
    if ((sups_info_ptr != NULL) && sups_info_ptr->ss_success)
    {
      cmsups_check_and_update_cfis(sups_info_ptr, sups_event_type);
    }
#else
    CM_MSG_HIGH_0("Feature Disabled. No need to update EF-CFIS");
#endif
  }

  /* Free the memory alloacted to sups_info_ptr, after sending the event */
  if (is_mem_free_req == TRUE && cmsups_sub_info_ptr->num_pending_sups_cmds == 0)
  {
    cmsups_mem_free_sups_info(asubs_id);
    cmsups_free_uss_info_ptr(asubs_id);
  }

#ifdef FEATURE_SUPS_RETRY
  if (is_retry == FALSE && sups_list_info_ptr != NULL)
  {
    cmsups_sub_info_ptr->sups_info_list = NULL;
    cmwsups_mem_free_sups_list_info(sups_list_info_ptr, &(cmsups_sub_info_ptr->sups_info_list_head));
  }
#endif
} /* cmsups_rpt_proc */
/*lint +esym(529, sups_ptr) not referenced */

/*===========================================================================

FUNCTION cmsups_call_control_complete_cb

DESCRIPTION
Callback for Number Classification to call when call control is complete.

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
void cmsups_call_control_complete_cb(
  cm_call_id_type                         call_id,
  /* call id */

  cm_cc_status_e_type                     status,
  /* status of command */

  cm_call_type_e_type                     call_type,
  /* call type */

  const cm_orig_call_control_s_type      *cdma_call_control_info_ptr,
  /* pointer to an info struct - NULL if not allowed on cdma */

  const cm_orig_call_control_s_type      *gw_call_control_info_ptr,
  /* pointer to an info struct - NULL if not allowed on gw */

  const void                             *user_data
  /* data passed from original call */
)
/*lint -esym(715, cdma_call_control_info_ptr)*/
/*lint -esym(715, status)*/
/*lint -esym(715, call_type)*/
/*lint -esym(715, call_id)*/
/*lint -esym(529, return_status) not referenced */
/*lint -esym(765, cmsups_call_control_complete_cb)
** Can't be static - callback
*/
{
  cm_sups_cmd_s_type               *sups_cmd_ptr;
  boolean                           return_status = FALSE;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  CM_ASSERT(user_data != NULL);

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  sups_cmd_ptr = (cm_sups_cmd_s_type *)user_data;

  /* Call control processing is complete
  */
  sups_cmd_ptr->is_call_control_processing_complete = TRUE;

  if (gw_call_control_info_ptr == NULL)
  {
    /* SS command is not allowed by CC (Reason could be GW subscription not available)
    ** Requeue the command to CM.
    */
    sups_cmd_ptr->cmd_info.cmd_err = CM_SUPS_CMD_ERR_CALL_CONTROL_REJECTED;

    CM_MSG_HIGH_0("SS CC -- Not Allowed (GW Subsc may not be avail)");
    cm_cmd_queue((cm_cmd_type *)sups_cmd_ptr);

    return;
  }

  sups_cmd_ptr->is_sim_cc_performed = gw_call_control_info_ptr->is_sim_cc_performed;

  switch (gw_call_control_info_ptr->call_control_result)
  {
    case CM_CC_RESULT_ALLOWED_NO_MODIFICATIONS:
      /* No modifications to the SS command.  Requeue the command to CM.
      */
      CM_MSG_HIGH_0("SS Call Control -- Allowed No Modifications");
      sups_cmd_ptr->is_parsing_required = FALSE;
      sups_cmd_ptr->cc_result_alpha = gw_call_control_info_ptr->alpha;
      cm_cmd_queue((cm_cmd_type *)sups_cmd_ptr);
      break;

    case CM_CC_RESULT_ALLOWED_BUT_MODIFIED:
    case CM_CC_RESULT_ALLOWED_BUT_MODIFIED_TO_SS:
    case CM_CC_RESULT_ALLOWED_BUT_MODIFIED_TO_USSD:
      if (sups_cmd_ptr->is_modem_cc_api == FALSE)
      {
        /* Modified to another SS/USS type.
        */
        CM_MSG_HIGH_1("SS Call Control -- Allowed/Modified to %d", \
                      gw_call_control_info_ptr->sups_cmd);
        switch (gw_call_control_info_ptr->sups_cmd)
        {
          case CM_SUPS_CMD_ERASE:
            return_status = cm_sups_cmd_erase_cc(sups_cmd_ptr->cmd_cb_func,
                                                 sups_cmd_ptr->data_block_ptr,
                                                 sups_cmd_ptr->client_id,
                                                 &gw_call_control_info_ptr->sups_params.erase_ss_params,
                                                 NULL,
                                                 TRUE);
            break;
          case CM_SUPS_CMD_REGISTER:
            return_status = cm_sups_cmd_register_cc(sups_cmd_ptr->cmd_cb_func,
                                                    sups_cmd_ptr->data_block_ptr,
                                                    sups_cmd_ptr->client_id,
                                                    &gw_call_control_info_ptr->sups_params.reg_ss_params,
                                                    NULL,
                                                    TRUE);
            break;
          case CM_SUPS_CMD_ACTIVATE:
            return_status = cm_sups_cmd_activate_cc(sups_cmd_ptr->cmd_cb_func,
                                                    sups_cmd_ptr->data_block_ptr,
                                                    sups_cmd_ptr->client_id,
                                                    &gw_call_control_info_ptr->sups_params.activate_ss_params,
                                                    NULL,
                                                    TRUE);
            break;
          case CM_SUPS_CMD_DEACTIVATE:
            return_status = cm_sups_cmd_deactivate_cc(sups_cmd_ptr->cmd_cb_func,
                            sups_cmd_ptr->data_block_ptr,
                            sups_cmd_ptr->client_id,
                            &gw_call_control_info_ptr->sups_params.deactivate_ss_params,
                            NULL,
                            TRUE);
            break;
          case CM_SUPS_CMD_INTERROGATE:
            return_status = cm_sups_cmd_interrogate_cc(sups_cmd_ptr->cmd_cb_func,
                            sups_cmd_ptr->data_block_ptr,
                            sups_cmd_ptr->client_id,
                            &gw_call_control_info_ptr->sups_params.interrogate_ss_params,
                            NULL,
                            TRUE);
            break;
          case CM_SUPS_CMD_PROCESS_USS:
            return_status = cm_sups_cmd_process_uss_cc(sups_cmd_ptr->cmd_cb_func,
                            sups_cmd_ptr->data_block_ptr,
                            sups_cmd_ptr->client_id,
                            &gw_call_control_info_ptr->sups_params.uss_params,
                            NULL,
                            TRUE);
            break;
          case CM_SUPS_CMD_REG_PASSWORD:
            return_status = cm_sups_cmd_reg_password_cc(sups_cmd_ptr->cmd_cb_func,
                            sups_cmd_ptr->data_block_ptr,
                            sups_cmd_ptr->client_id,
                            &gw_call_control_info_ptr->sups_params.reg_pwd_params,
                            NULL,
                            TRUE);
            break;
          case CM_SUPS_CMD_RELEASE:
            return_status = cm_sups_cmd_release_cc(sups_cmd_ptr->cmd_cb_func,
                                                   sups_cmd_ptr->data_block_ptr,
                                                   sups_cmd_ptr->client_id,
                                                   &gw_call_control_info_ptr->sups_params.release_ss_params,
                                                   NULL,
                                                   TRUE);
            break;
          default:
            CM_ERR_1("unsupported mod to SS=%d", gw_call_control_info_ptr->sups_cmd);
        }
        if (!return_status)
        {
          CM_ERR_0("Unable to start new SUPS operation");
        }
      }
      else
      {
        /* Modified to another SS type.
        */
        CM_MSG_HIGH_0("SS Call Control -- Allowed,Modified");
        if (gw_call_control_info_ptr->call_control_result ==
            CM_CC_RESULT_ALLOWED_BUT_MODIFIED_TO_USSD)
        {
          /* Modified to USSD type.
          */
          sups_cmd_ptr->is_parsing_required = FALSE;
          sups_cmd_ptr->cc_result_alpha =
            gw_call_control_info_ptr->alpha;
          sups_cmd_ptr->dialed_digits = gw_call_control_info_ptr->num;
          sups_cmd_ptr->cmd_info.uss_data = gw_call_control_info_ptr->sups_params.uss_params.uss_data;
          sups_cmd_ptr->cc_result = CM_CC_RESULT_ALLOWED_BUT_MODIFIED_TO_USSD;
          sups_cmd_ptr->cmd = CM_SUPS_CMD_PROCESS_USS;
          cm_cmd_queue((cm_cmd_type *)sups_cmd_ptr);
          CM_MSG_HIGH_1("S CC -- Allowed,Modified to USSD. Posted CM_SUPS_CMD_PROCESS_USS. with cc result %d",
                        sups_cmd_ptr->cc_result);
        }
        else
        {
          /* Mark that string needs to be parsed and then
          ** corresponding processing of Sups cmd needs to be done
          */
          sups_cmd_ptr->is_parsing_required = TRUE;
          sups_cmd_ptr->cc_result_alpha =
            gw_call_control_info_ptr->alpha;
          sups_cmd_ptr->dialed_digits = gw_call_control_info_ptr->num;
          sups_cmd_ptr->cc_result = CM_CC_RESULT_ALLOWED_BUT_MODIFIED_TO_SS;
          cm_cmd_queue((cm_cmd_type *)sups_cmd_ptr);

          CM_MSG_HIGH_1("SS CC -- Allowed, Modified to SS. Posted sups cmd. with cc result %d",
                        sups_cmd_ptr->cc_result);
        }
      }
      break;

    case CM_CC_RESULT_ALLOWED_BUT_MODIFIED_TO_VOICE:
    #ifdef FEATURE_DISABLED_HABANERO
    case CM_CC_RESULT_ALLOWED_BUT_MODIFIED_TO_VT:
    #endif
      if (sups_cmd_ptr->cmd_cb_func != NULL)
      {
        sups_cmd_ptr->cmd_cb_func(sups_cmd_ptr->data_block_ptr,
                                  sups_cmd_ptr->cmd,
                                  CM_SUPS_CMD_ERR_NOERR);
      }
      #ifdef FEATURE_DISABLED_HABANERO
      if(gw_call_control_info_ptr->call_control_result == CM_CC_RESULT_ALLOWED_BUT_MODIFIED_TO_VOICE)
      #endif
      {
        sups_cmd_ptr->cc_result = CM_CC_RESULT_ALLOWED_BUT_MODIFIED_TO_VOICE;
      }
      #ifdef FEATURE_DISABLED_HABANERO
      else if(gw_call_control_info_ptr->call_control_result == CM_CC_RESULT_ALLOWED_BUT_MODIFIED_TO_VT)
      {
        sups_cmd_ptr->cc_result = CM_CC_RESULT_ALLOWED_BUT_MODIFIED_TO_VT;
      }
      #endif
      sups_cmd_ptr->cc_result_alpha = gw_call_control_info_ptr->alpha;
      sups_cmd_ptr->dialed_digits = gw_call_control_info_ptr->num;

      cm_cmd_queue((cm_cmd_type *)sups_cmd_ptr);

      CM_MSG_HIGH_1("updated voice call num. SS Call Control -- Allowed/Voice. Posted sups cmd. with cc result %d",
                    sups_cmd_ptr->cc_result);
      break;

    case CM_CC_RESULT_NOT_ALLOWED:
      /* SS command is not allowed by CC.  Requeue the command to CM.
      */
      sups_cmd_ptr->cmd_info.cmd_err = CM_SUPS_CMD_ERR_CALL_CONTROL_REJECTED;
      sups_cmd_ptr->cmd_info.cmd_err_cause =
        (cm_sups_cmd_err_cause_e_type)gw_call_control_info_ptr->error_cause;
      sups_cmd_ptr->cc_result_alpha = gw_call_control_info_ptr->alpha;
      sups_cmd_ptr->cc_result = CM_CC_RESULT_NOT_ALLOWED;

      CM_MSG_HIGH_0("SS Call Control -- Not Allowed");

      cm_cmd_queue((cm_cmd_type *)sups_cmd_ptr);
      break;

    default:
      CM_ERR_1("Unexpected CC Result %d",
               (int)gw_call_control_info_ptr->call_control_result);
  }
}  /* cmsups_call_control_complete_cb */
/*lint +esym(715, cdma_call_control_info_ptr)*/
/*lint +esym(715, status)*/
/*lint +esym(715, call_type)*/
/*lint +esym(715, call_id)*/
/*lint +esym(529, return_status) not referenced */

/*===========================================================================

FUNCTION cmwsups_process_sups_to_voice

DESCRIPTION
Handle to sups to voice processing

DEPENDENCIES

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
static void cmwsups_process_sups_to_voice(
  const cm_sups_cmd_s_type   *sups_cmd_ptr,
  /* command pointer info */
  cm_call_type_e_type        modify_call_type
)
{
  cm_gw_cs_orig_params_s_type   *gw_orig_params_ptr;
  cm_orig_alpha_s_type           cm_alpha;
  cm_call_id_type                call_id;
  cm_sups_info_s_type           *sups_info_ptr = NULL;
  cmsups_s_type                 *sups_ptr = cmsups_ptr();
  sys_modem_as_id_e_type         asubs_id = SYS_MODEM_AS_ID_1;
  cmsups_sub_info_s_type        *cmsups_sub_info_ptr;

  /* Since cm_gw_cs_orig_params_s_type is so big, let's allocate memory
  ** dynamically and deallocate when we are done sending it to the origination.
  */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  CM_ASSERT(sups_cmd_ptr != NULL);
  CM_ASSERT(sups_cmd_ptr->cmd_type == CM_CMD_TYPE_SUPS);

  asubs_id = sups_cmd_ptr->cmd_info.cmd_subs_id;
  cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

  /* Null check cmsups_sub_info_ptr */
  if (cmsups_sub_info_ptr == NULL)
  {
    CM_ERR_1("cmwsups_process_sups_to_voice: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
             asubs_id);
    return;
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  gw_orig_params_ptr = (cm_gw_cs_orig_params_s_type *)cm_mem_malloc
                       (sizeof (cm_gw_cs_orig_params_s_type));

  sups_info_ptr = cmsups_ensure_sups_info_ptr_asubs_id(asubs_id);
  if (sups_info_ptr == NULL)
  {
    CM_ERR_1("cmwsups_process_sups_to_voice: sups_info_ptr NULL ptr for asubs_id %d",
             asubs_id);
    return;
  }

  cm_util_default_gw_orig_params_per_sub(gw_orig_params_ptr, asubs_id);
  gw_orig_params_ptr->call_control_completed = TRUE;

  cm_alpha.buf[0] = '\0';
  cm_alpha.len = 0;

  if (!cm_mm_call_cmd_orig_per_subs(NULL,
                                    sups_cmd_ptr->data_block_ptr,
                                    sups_cmd_ptr->client_id,
                                    modify_call_type,
                                    CM_SRV_TYPE_AUTOMATIC,
                                    sups_cmd_ptr->cmd_info.cmd_subs_id,
                                    NULL,                           /* Calling Number */
                                    &sups_cmd_ptr->dialed_digits,   /* Called Number */
                                    &cm_alpha,                      /* alpha_buf */
                                    NULL,                           /* cdma_orig_params_ptr */
                                    gw_orig_params_ptr,             /* gw_cs_orig_params_ptr */
                                    NULL,                           /* gw_ps_orig_params_ptr */
                                    &call_id                        /* return call id ptr */
                                   ))
  {
    CM_ERR_0("Unable to start Orig for SUPS call control!");
  }
  else
  {
    CM_MSG_HIGH_1("call_cmd_orig called...modify_type = %d", modify_call_type);
    sups_info_ptr->mod_ss_params.call_id = call_id;
    #ifdef FEATURE_DISABLED_HABANERO
    if(modify_call_type == CM_CALL_TYPE_VOICE)
    #endif
    {
      sups_info_ptr->mod_ss_params.call_control_result =
        CM_CC_RESULT_ALLOWED_BUT_MODIFIED_TO_VOICE;
    }
    #ifdef FEATURE_DISABLED_HABANERO
    else if(modify_call_type == CM_CALL_TYPE_VT)
    {
      sups_info_ptr->mod_ss_params.call_control_result =
        CM_CC_RESULT_ALLOWED_BUT_MODIFIED_TO_VT;
    }
    #endif
    sups_info_ptr->ss_ref = sups_cmd_ptr->cmd_info.ss_ref;
    CM_MSG_HIGH_2("CM_SUPS_EVENT_SS_MOD_TO_ORIG: call_id: %d, ss_ref: %d",
                  call_id, sups_info_ptr->ss_ref);

    cmsups_event(asubs_id, CM_SUPS_EVENT_SS_MOD_TO_ORIG);
  }
  cm_mem_free(gw_orig_params_ptr);
  if (cmsups_sub_info_ptr->num_pending_sups_cmds == 0)
  {
    /* Free sups info ptr as CM is not waiting for any CNF from NAS*/
    cmsups_mem_free_sups_info(asubs_id);
    cmsups_free_uss_info_ptr(asubs_id);
  }
}

/*===========================================================================

FUNCTION cmsups_client_cmd_proc

DESCRIPTION
Process clients' sups commands

DEPENDENCIES
SUPS object must have already been initialized with
cmsups_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
void cmsups_client_cmd_proc(
  cm_sups_cmd_s_type   *sups_cmd_ptr
  /* pointer to a CM command */
)
/*lint -esym(550, parse_result) */
{
  cmsups_s_type             *sups_ptr = cmsups_ptr();
  /* Point at sups object */

  cm_sups_cmd_err_e_type     sups_cmd_err = CM_SUPS_CMD_ERR_NOERR;

  boolean                    is_starting_cc_proc = FALSE;

  dword                      uptime = time_get_uptime_secs();

  sys_modem_as_id_e_type     asubs_id = SYS_MODEM_AS_ID_1;

  cmsups_sub_info_s_type    *cmsups_sub_info_ptr;

  boolean                    is_subs_not_avail = FALSE;

#ifdef FEATURE_IP_CALL
  cmsups_cmd_route_e_type    sups_cmd_route;
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_ASSERT(sups_cmd_ptr != NULL);
  CM_ASSERT(sups_ptr != NULL);
  CM_ASSERT(sups_cmd_ptr->cmd_type == CM_CMD_TYPE_SUPS);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_MSG_HIGH_2(" cmsups_client_cmd_proc(), cmutil_num_of_subs_avail %d oprt_mode_send_time %d",
                cmutil_num_of_subs_avail(), cmph_ptr()->device_prop.ph_oprt_mode.oprt_mode_send_time);
  if (cmutil_num_of_subs_avail() && cmph_ptr()->device_prop.ph_oprt_mode.oprt_mode_send_time != CMPH_MAX_UPTIME)
  {
    cmph_send_oprt_mode_online_to_mmoc();
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef FEATURE_MMODE_DUAL_SIM
  if (cmph_is_msim()
      &&
      ((sups_cmd_ptr->cmd == CM_SUPS_CMD_LCS_LOCATION_NOTIFICATION_RES)
       || (sups_cmd_ptr->cmd == CM_SUPS_CMD_LCS_MOLR_COMPLETED)
       || (sups_cmd_ptr->cmd == CM_SUPS_CMD_LCS_MOLR))
     )
  {
    if (cmph_ptr()->device_prop.msim_prop.internal_standby_pref != SYS_MODEM_DS_PREF_DUAL_STANDBY
#if defined FEATURE_MMODE_TRIPLE_SIM
        && cmph_ptr()->device_prop.msim_prop.internal_standby_pref != SYS_MODEM_DS_PREF_TRIPLE_STANDBY
#endif
       )
    {
      sys_modem_as_id_e_type sub;
      for (sub = SYS_MODEM_AS_ID_1; cmutil_is_as_id_valid(sub); sub++)
      {
        if (cmph_ptr()->device_prop.msim_prop.active_subs & BM(sub))
        {
          sups_cmd_ptr->cmd_info.cmd_subs_id = sub;
        }
      }
    }
    else
    {
      sups_cmd_ptr->cmd_info.cmd_subs_id = cmph_ptr()->device_prop.msim_prop.default_data_subs;
    }
  }
  CM_MSG_HIGH_1("cmd_subs_id updated: %d", sups_cmd_ptr->cmd_info.cmd_subs_id);
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (sups_cmd_ptr->cmd_info.cmd_subs_id > SYS_MODEM_AS_ID_NONE &&
      sups_cmd_ptr->cmd_info.cmd_subs_id < ARR_SIZE(cmph_ptr()->sub_info) &&
      !cmph_ptr()->sub_info[sups_cmd_ptr->cmd_info.cmd_subs_id]->is_3gpp_subs_avail &&
      !cmph_ptr()->sub_info[sups_cmd_ptr->cmd_info.cmd_subs_id]->is_3gpp2_subs_avail)
  {
    is_subs_not_avail = TRUE;
  }
  else
  {
    is_subs_not_avail = FALSE;
  }

  if (is_subs_not_avail)
  {
    CM_MSG_HIGH_0("NO Subscription available...rejecting sups command");
    cmsups_client_cmd_err(sups_cmd_ptr, CM_SUPS_CMD_ERR_SUBS_ID_P);
    if(cmsups_check_if_cmd_is_MT_response(sups_cmd_ptr->cmd))
    {
      /* If subs is not available and sups command is a response
      ** to an MT transaction, send a release request to NAS
      ** to release the resources.
      */
      ie_cm_ss_error_T         cm_ss_error;
      cm_ss_error.present = TRUE;
      cm_ss_error.error_code_tag = ERROR_CODE_TAG;
      cm_ss_error.error_code = ussd_Busy;
      cmwsups_send_release_ss_req(sups_cmd_ptr->cmd_info.invoke_id,
                                  ss_NotAvailable,
                                  cm_ss_error,
                                  sups_cmd_ptr->cmd_info.cmd_subs_id);
    }
    return;
  }

#ifdef FEATURE_MMODE_DUAL_SIM
  if (sups_cmd_ptr->cmd_info.cmd_subs_id == SYS_MODEM_AS_ID_NO_CHANGE &&
      cmph_is_msim())
  {
    if ((cmph_ptr()->device_prop.msim_prop.internal_standby_pref != SYS_MODEM_DS_PREF_DUAL_STANDBY
#ifdef FEATURE_MMODE_TRIPLE_SIM
         && cmph_ptr()->device_prop.msim_prop.internal_standby_pref != SYS_MODEM_DS_PREF_TRIPLE_STANDBY
#endif
        )
        && (cmph_ptr()->device_prop.msim_prop.active_subs == SYS_MODEM_AS_ID_2_MASK))
    {
      sups_cmd_ptr->cmd_info.cmd_subs_id = SYS_MODEM_AS_ID_2;
    }
#ifdef FEATURE_MMODE_TRIPLE_SIM
    else if ((cmph_ptr()->device_prop.msim_prop.internal_standby_pref != SYS_MODEM_DS_PREF_DUAL_STANDBY
              && (cmph_ptr()->device_prop.msim_prop.internal_standby_pref != SYS_MODEM_DS_PREF_TRIPLE_STANDBY)
             )
             && (cmph_ptr()->device_prop.msim_prop.active_subs == SYS_MODEM_AS_ID_3_MASK))
    {
      sups_cmd_ptr->cmd_info.cmd_subs_id = SYS_MODEM_AS_ID_3;
    }
#endif
    else
    {
      /* default it to MAIN if no as_id is passed for dual standby  */
      sups_cmd_ptr->cmd_info.cmd_subs_id = SYS_MODEM_AS_ID_1;
    }
  }
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (sups_cmd_ptr->cmd_info.cmd_subs_id == SYS_MODEM_AS_ID_NO_CHANGE)
  {
    sups_cmd_ptr->cmd_info.cmd_subs_id = SYS_MODEM_AS_ID_1;
  }
  asubs_id = sups_cmd_ptr->cmd_info.cmd_subs_id;
  cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

  /* Null check cmsups_sub_info_ptr */
  if (cmsups_sub_info_ptr == NULL)
  {
    CM_ERR_1("cmsups_client_cmd_proc: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
             asubs_id);
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Clear info part of sups object
  */

  /* Allocate memory for sups_info_ptr */
  if (cmsups_sub_info_ptr->sups_info == NULL)
  {
    cmsups_sub_info_ptr->sups_info = cmsups_info_alloc();
    cmsups_init_sups_info(cmsups_sub_info_ptr->sups_info);
    CM_MSG_HIGH_0("Memory allocated for sups_info");
  }
  else
  {
    cmsups_init_sups_info(cmsups_sub_info_ptr->sups_info);
  }

  if (cmsups_sub_info_ptr->uss_info == NULL)
  {
    (void)cmsups_get_uss_info_ptr(asubs_id);
  }
  cmsups_sub_info_ptr->sups_info->asubs_id = asubs_id;

#ifdef FEATURE_SUPS_RETRY  /* SUPS RETRY */
  /*
  ** IP SUPS command will be retried on CS when IP SUPS command fallback from IMS to CS. there is no retry of SUPS command on IMS.
  */
#ifdef FEATURE_IP_CALL
  /*Update sups cmd route here as its used in call control
  ** It will be re-updated after call control in client_cmd_check
  */
  cmsups_sub_info_ptr->sups_cmd_route = sups_cmd_route = \
  cmipsups_route_check(sups_cmd_ptr->cmd, asubs_id);

  if (!(cmipsups_is_ussd_sups_cmd(sups_cmd_ptr->cmd) && (sups_cmd_route == CMSUPS_CMD_REJECT)))
#endif
  {
    if (sups_cmd_ptr->cmd == CM_SUPS_CMD_PROCESS_USS &&
        (cmwsups_is_uss_cmd_already_present(asubs_id) != sups_cmd_ptr->cmd_info.ss_ref) &&
        (cmwsups_is_uss_cmd_already_present(asubs_id) != 0))
    {
      CM_MSG_HIGH_0("One sups cmd already pending...rejecting next one");
      cmsups_client_cmd_err(sups_cmd_ptr, CM_SUPS_CMD_ERR_OTHER);
      return;
    }
    else
    {
      /* Allocate memory for sups_info_ptr */
      if (!((sups_cmd_ptr->cmd == CM_SUPS_CMD_GET_PASSWORD_RES) ||
            (sups_cmd_ptr->cmd == CM_SUPS_CMD_USS_NOTIFY_RES) ||
            (sups_cmd_ptr->cmd == CM_SUPS_CMD_USS_RES) ||
            (sups_cmd_ptr->cmd == CM_SUPS_CMD_LCS_LOCATION_NOTIFICATION_RES)) &&
            cmph_ptr()->device_prop.ph_oprt_mode.oprt_mode == SYS_OPRT_MODE_ONLINE)
      {
        cmsups_sub_info_ptr->sups_info_list = cmwsups_info_list_find_alloc(asubs_id, sups_cmd_ptr->cmd_info.ss_ref);
        if (cmsups_sub_info_ptr->sups_info_list != NULL)
        {
          cmsups_sub_info_ptr->sups_info_list->sr_start_uptime = uptime;
        }
      }
    }
  }
#endif

  /* If call control is needed, pass it along to the number classification routines
  */
  CM_MSG_HIGH_3("cc complete: %d dial digit len: %d cc_res: %d",
                sups_cmd_ptr->is_call_control_processing_complete,
                sups_cmd_ptr->dialed_digits.len,
                sups_cmd_ptr->cc_result);

  /* Send call control result to clients */
  if (sups_cmd_ptr->is_call_control_processing_complete &&
      sups_cmd_ptr->is_sim_cc_performed)
  {
    /* save a copy in the sups object and let the clients know of call control result and alpha */
    cmsups_cmd_copy_fields(sups_cmd_ptr);
    cmsups_event(asubs_id, CM_SUPS_EVENT_CC_RESULT_INFO);
  }

  if (sups_cmd_ptr->is_call_control_processing_complete == FALSE &&
      sups_cmd_ptr->dialed_digits.len != 0
     )
  {
    cm_cc_generic_params_s_type   cc_generic_params;
    cc_generic_params.is_fdn_to_check = TRUE;

    is_starting_cc_proc = cm_nc_call_control_processing
                          (
                            CM_CALL_ID_UNASSIGNED,
                            CM_CC_SUBSCRIPTION_SOURCE_NONE, /* SUPS for GW only */
                            cmph_get_gw_subscription_source(asubs_id),
                            CM_CALL_TYPE_SUPS,
                            &sups_cmd_ptr->dialed_digits,
                            (void *)sups_cmd_ptr,
                            cmsups_call_control_complete_cb,
                            NULL,
                            &cc_generic_params
                          );

    if (is_starting_cc_proc)
    {
      CM_MSG_HIGH_0("SUPS: Starting Call Control Processing");
    }
    else
    {
      CM_MSG_HIGH_0("cm_nc_call_control_processing returned FALSE");
      /*lint -save -e527 Unreachable
      **     When CM_DEBUG is on, CM_ERR does exit(1)
      */
      sups_cmd_err = CM_SUPS_CMD_ERR_CALL_CONTROL_FAILURE;
      /* Since Number classification returned false, it's upto CM to free
      ** the command buffer that was sent to it.
      */
      sups_cmd_ptr->is_reused = FALSE;
      /*lint -restore */
    }

    /* If there was a call control error, the command is done so notify the client.
    ** Otherwise, don't notify the client.
    */
    if (sups_cmd_err != CM_SUPS_CMD_ERR_NOERR)
    {
      cmsups_client_cmd_err(sups_cmd_ptr, sups_cmd_err);
      cmsups_free_sups(asubs_id);
      cmsups_free_uss_info_ptr(asubs_id);
    }
    return;
  }
  else if (sups_cmd_ptr->is_call_control_processing_complete == TRUE &&
           (sups_cmd_ptr->cc_result == CM_CC_RESULT_ALLOWED_BUT_MODIFIED_TO_VOICE 
            #ifdef FEATURE_DISABLED_HABANERO
            || sups_cmd_ptr->cc_result == CM_CC_RESULT_ALLOWED_BUT_MODIFIED_TO_VT
            #endif
           ))
  {
    /* Check if CC modified sups call to voice */
    /*
    ** Notify clients of command processing status.
    ** If error found, return.
    */
    cmsups_client_cmd_err(sups_cmd_ptr, sups_cmd_err);

    CM_MSG_HIGH_0("sups to voice processing");
    #ifdef FEATURE_DISABLED_HABANERO
    if(sups_cmd_ptr->cc_result == CM_CC_RESULT_ALLOWED_BUT_MODIFIED_TO_VOICE)
    #endif
    {
      cmwsups_process_sups_to_voice(sups_cmd_ptr, CM_CALL_TYPE_VOICE);
    }
    #ifdef FEATURE_DISABLED_HABANERO
    else if(sups_cmd_ptr->cc_result == CM_CC_RESULT_ALLOWED_BUT_MODIFIED_TO_VT)
    {
      cmwsups_process_sups_to_voice(sups_cmd_ptr, CM_CALL_TYPE_VT);
    }
    #endif
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Start by checking whether this is a valid command
  ** relative to the current state of the call/phone.
  */
  sups_cmd_err = cmsups_client_cmd_check(sups_cmd_ptr);

  /* Notify clients of command processing status.
  ** If error found, return.
  */
  cmsups_client_cmd_err(sups_cmd_ptr, sups_cmd_err);
  if (sups_cmd_err != CM_SUPS_CMD_ERR_NOERR)
  {
    cmsups_free_sups(asubs_id);
    cmsups_free_uss_info_ptr(asubs_id);
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If we got here, no errors were found,
  ** parse string, if specified, and then
  ** forward the command to Lower Layer.
  */
  if (sups_cmd_ptr->is_parsing_required == TRUE)
  {
    /* If we reach here, sups cmd params are not to be
    ** considered, but dialled string needs to be parsed
    ** to find out the sups cmd and its parameters
    */
    cmutil_parse_sups_result_e_type parse_result = CMUTIL_PARSE_SUPS_RESULT_NONE;

    cmutil_sups_params_s_type sups_cmd_params;

    CM_MSG_HIGH_0("Invoking parse utility");
    /* invoke parsing utility */
    if (sups_cmd_ptr->dialed_digits.len > CM_MAX_NUMBER_CHARS)
    {
      /* Error in parsing length */
      CM_ERR_1("Parsing length is out of boundary %d",
               sups_cmd_ptr->dialed_digits.len);
      return;
    }

    parse_result = cmutil_parse_sups_string(
                     sups_cmd_ptr->dialed_digits,
                     sups_cmd_ptr->dialed_digits.len,
                     &sups_cmd_params);
    if (parse_result != CMUTIL_PARSE_SUPS_RESULT_SUCCESS)
    {
      /* Error in parsing */
      CM_ERR_1("Parsing error %d, sending client originated Supplementary service request",
               parse_result);
      /* Send the original Sups command to LL */
    }
    else
    {
      sups_cmd_ptr->cmd = sups_cmd_params.sups_cmd;
#ifdef FEATURE_CM_DEBUG_BUFFER
      cmdbg_add_to_dbg_buffer(CM_BUFF_SUPS_CMD_TYPE, sups_cmd_ptr->cmd, (void *)sups_cmd_ptr);
#endif
      switch (sups_cmd_ptr->cmd)
      {
        case CM_SUPS_CMD_ACTIVATE:
          sups_cmd_ptr->cmd_info.ss_code =
            sups_cmd_params.sups_params.activate_ss_params.ss_code;

          sups_cmd_ptr->cmd_info.basic_service =
            sups_cmd_params.sups_params.activate_ss_params.basic_service;

          sups_cmd_ptr->cmd_info.ss_password =
            sups_cmd_params.sups_params.activate_ss_params.ss_pwd;

          sups_cmd_ptr->cmd_info.ss_operation = activateSS;

          break;

        case CM_SUPS_CMD_DEACTIVATE:
          sups_cmd_ptr->cmd_info.ss_code =
            sups_cmd_params.sups_params.deactivate_ss_params.ss_code;

          sups_cmd_ptr->cmd_info.basic_service =
            sups_cmd_params.sups_params.deactivate_ss_params.basic_service;

          sups_cmd_ptr->cmd_info.ss_password =
            sups_cmd_params.sups_params.deactivate_ss_params.ss_pwd;

          if (sups_cmd_ptr->cmd_info.ss_code == (byte)ccbs)
          {
            sups_cmd_ptr->cmd = CM_SUPS_CMD_ERASE_CC_ENTRY;
            sups_cmd_ptr->cmd_info.ccbs_indx =
              sups_cmd_params.sups_params.deactivate_ss_params.ccbs_indx;
          }
          sups_cmd_ptr->cmd_info.ss_operation = deactivateSS;
          break;

        case CM_SUPS_CMD_INTERROGATE:
          sups_cmd_ptr->cmd_info.ss_code =
            sups_cmd_params.sups_params.interrogate_ss_params.ss_code;

          sups_cmd_ptr->cmd_info.basic_service =
            sups_cmd_params.sups_params.interrogate_ss_params.basic_service;

          sups_cmd_ptr->cmd_info.ss_operation = interrogateSS;
          break;

        case CM_SUPS_CMD_ERASE:
          sups_cmd_ptr->cmd_info.ss_code =
            sups_cmd_params.sups_params.erase_ss_params.ss_code;

          sups_cmd_ptr->cmd_info.basic_service =
            sups_cmd_params.sups_params.erase_ss_params.basic_service;

          sups_cmd_ptr->cmd_info.ss_operation = eraseSS;
          break;

        case CM_SUPS_CMD_REGISTER:
          sups_cmd_ptr->cmd_info.ss_code =
            sups_cmd_params.sups_params.reg_ss_params.ss_code;

          sups_cmd_ptr->cmd_info.basic_service =
            sups_cmd_params.sups_params.reg_ss_params.basic_service;
            sups_cmd_ptr->cmd_info.fwd_type =
            sups_cmd_params.sups_params.reg_ss_params.fwd_type;
          
          sups_cmd_ptr->cmd_info.forwarded_to_number =
            sups_cmd_params.sups_params.reg_ss_params.fwd_to_num;

          sups_cmd_ptr->cmd_info.forwarded_to_subaddress =
            sups_cmd_params.sups_params.reg_ss_params.fwd_to_subaddr;

          sups_cmd_ptr->cmd_info.nr_timer =
            sups_cmd_params.sups_params.reg_ss_params.nr_timer;

          sups_cmd_ptr->cmd_info.ss_operation = registerSS;
          break;

        case CM_SUPS_CMD_REG_PASSWORD:
          sups_cmd_ptr->cmd_info.code =
            sups_cmd_params.sups_params.reg_pwd_params.ss_code;

          sups_cmd_ptr->cmd_info.ss_password =
            sups_cmd_params.sups_params.reg_pwd_params.ss_old_pwd;

          sups_cmd_ptr->cmd_info.ss_new_password =
            sups_cmd_params.sups_params.reg_pwd_params.ss_new_pwd;

          sups_cmd_ptr->cmd_info.ss_new_password_again =
            sups_cmd_params.sups_params.reg_pwd_params.ss_new_pwd_again;

          sups_cmd_ptr->cmd_info.ss_operation = registerPassword;
          break;

        case CM_SUPS_CMD_PROCESS_USS:
          sups_cmd_ptr->cmd_info.ss_operation =
            processUnstructuredSS_Request;

          *(&(sups_cmd_ptr->cmd_info.uss_data)) = *(&(sups_cmd_params.sups_params.uss_params.uss_data));
          break;

        default:
          /* Send the original Sups command to LL */
          break;
      }
    }

    /* Just to be safe, mark this field FALSE */
    sups_cmd_ptr->is_parsing_required = FALSE;
  }
  else /* Parsing is not required */
  {
    CM_MSG_HIGH_0("USS. Parsing not req. ss_op processUnstructuredSS_Request");
    if (sups_cmd_ptr->cmd == CM_SUPS_CMD_PROCESS_USS)
    {
      sups_cmd_ptr->cmd_info.ss_operation = processUnstructuredSS_Request;
      /* Reset ss_code to 0 in case of USS */
      sups_cmd_ptr->cmd_info.ss_code = 0;
      CM_MSG_HIGH_0("ss_op: processUnstructuredSS_Request ss code: 0");
    }
  }

  /* forward to lower layer */
  cmsups_client_cmd_forward_ll(sups_ptr, sups_cmd_ptr);

  /* Increase the pending sups count */
  cmsups_sub_info_ptr->num_pending_sups_cmds++;

  /* Not freeing memory of sups_ptr here is intentional.
  ** It will be freed once CM gets conf report from NAS.
  */
  /* For cases where we dont get conf from NAS we need to free the memory here
  */
  if(sups_cmd_ptr->cmd == CM_SUPS_CMD_GET_PASSWORD_RES ||
     sups_cmd_ptr->cmd == CM_SUPS_CMD_USS_NOTIFY_RES ||
     sups_cmd_ptr->cmd == CM_SUPS_CMD_LCS_LOCATION_NOTIFICATION_RES
    )
  {
    cmsups_sub_info_ptr->num_pending_sups_cmds--;
    cmsups_mem_free_sups_info(asubs_id);
    cmsups_free_uss_info_ptr(asubs_id);
  }
}
/*lint +esym(550, parse_result) */

/*===========================================================================

FUNCTION cmwsups_send_buffered_cmd_gw

DESCRIPTION
  Send the buffered SUPS command to NAS

DEPENDENCIES
  Sups object must have already been initialized with
  cmcall_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmwsups_send_buffered_cmd_gw(
  sys_modem_as_id_e_type  asubs_id
)
{
  cmsups_sub_info_s_type  *cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

  if(cmsups_sub_info_ptr != NULL &&
     cmsups_sub_info_ptr->buffered_mn_cmd_ptr != NULL)
  {
    CM_MSG_HIGH_2("SUPS: send buffered cmd %d to MN on sub %d",
                  cmsups_sub_info_ptr->buffered_mn_cmd_ptr->cmd.hdr.message_id,
                  asubs_id);
    cm_mn_send_cmd(cmsups_sub_info_ptr->buffered_mn_cmd_ptr);
    cmsups_sub_info_ptr->buffered_mn_cmd_ptr = NULL;
    cmsups_sub_info_ptr->buffered_mn_cmd_timer = 0;
  }
}

#ifdef FEATURE_SUPS_RETRY

/*===========================================================================

FUNCTION cmwsups_list_clear

DESCRIPTION
Clear all SUPS list for given asubs_id

DEPENDENCIES
none

RETURN VALUE
Pointer to allocated sups info struct.

SIDE EFFECTS
none

===========================================================================*/
static void cmwsups_list_clear(
  sys_modem_as_id_e_type   asubs_id
)
{
  cmwsups_info_list_s_type   *list_ptr = NULL;
  cmwsups_info_list_s_type   *temp_ptr = NULL;
  cmsups_sub_info_s_type     *cmsups_sub_info_ptr = cmsups_get_sub_info_ptr(asubs_id);

  /* Null check cmsups_sub_info_ptr */
  if (cmsups_sub_info_ptr == NULL)
  {
    CM_ERR_1("cmwsups_list_clear: sups_ptr->sub_info[i] NULL ptr for asubs_id %d",
             asubs_id);
    return;
  }

  list_ptr = cmsups_sub_info_ptr->sups_info_list_head;

  while (list_ptr != NULL)
  {
    temp_ptr = list_ptr->next;

    /* Send failure event to clients */
    cmwsups_send_sups_fail_event(asubs_id, list_ptr);
    CM_MSG_HIGH_1("Sending sups fail event for cmd = %d", list_ptr->cmd);
    cm_mem_free(list_ptr);
    list_ptr = temp_ptr;
  }

  cmsups_sub_info_ptr->sups_info_list_head = NULL;
  cmsups_sub_info_ptr->sups_info_list = NULL;

  return;
}
#endif

/*===========================================================================

FUNCTION cmwsups_timer_proc

DESCRIPTION
Process timer events

DEPENDENCIES
Sups object must have already been initialized with
cmcall_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
void cmwsups_timer_proc(
  cm_timer_event_type   timer_event
  /* Indicate specific timer event
  ** Note that currently timer event is not being used
  */
)
{
  dword                          uptime = time_get_uptime_secs();
  cmsups_s_type                 *sups_ptr = cmsups_ptr();
  cmsups_sub_info_s_type        *cmsups_sub_info_ptr;
  uint8                          i = 0;

#ifdef FEATURE_SUPS_RETRY
  cmwsups_info_list_s_type      *sups_list = NULL;
  cmwsups_info_list_s_type      *temp_list = NULL;
  /* Pointer to the phone object */

  sd_ss_mode_pref_e_type         comm_mode = CM_MODE_PREF_NONE;
  /* Mode capability supported by the current target */

  sd_ss_band_pref_e_type         comm_band = CM_BAND_PREF_NONE;
  /* Band capability supported by the current target */

#ifdef FEATURE_CM_LTE
  sys_lte_band_mask_e_type       comm_lte_band = SYS_LTE_BAND_MASK_CONST_NONE;
  /* LTE band capability supported by the current target */
#endif
  sd_ss_band_pref_e_type         comm_tds_band = CM_BAND_PREF_NONE;
  /* TD-SCDMA band capability supported by the current target */

  boolean                        no_retry = TRUE;
#endif


  /* If the buffered sups cmd timer has expired, send the buffered cmd to NAS
  */
  for (i = 0; i < sups_ptr->sub_info_count; i++)
  {
    cmsups_sub_info_ptr = cmsups_get_sub_info_ptr((sys_modem_as_id_e_type)i);

    if(cmsups_sub_info_ptr != NULL && cmsups_sub_info_ptr->buffered_mn_cmd_ptr != NULL &&
       cmsups_sub_info_ptr->buffered_mn_cmd_timer != 0 &&
       cmsups_sub_info_ptr->buffered_mn_cmd_timer <= uptime)
    {
      CM_MSG_HIGH_2("SUPS timer proc: timer expired, send buffered cmd %d on sub %d",
                    cmsups_sub_info_ptr->buffered_mn_cmd_ptr->cmd.hdr.message_id, i);
      cmwsups_send_buffered_cmd_gw((sys_modem_as_id_e_type)i);
    }
  }

#ifdef FEATURE_SUPS_RETRY
  /* if all info_lists are NULL, return now
  */
  for (i = 0; i < sups_ptr->sub_info_count; i++)
  {
    cmsups_sub_info_ptr = cmsups_get_sub_info_ptr((sys_modem_as_id_e_type)i);

    if (cmsups_sub_info_ptr != NULL && cmsups_sub_info_ptr->sups_info_list_head != NULL)
    {
      no_retry = FALSE;
    }
  }
  if (no_retry)
  {
    return;
  }

  /* free the sups reference memeory when the mode preference is LTE only and the domain is CS_PS
  */
#ifdef FEATURE_CM_LTE
  for (i = 0; i < sups_ptr->sub_info_count; i++)
  {
    cmsups_sub_info_ptr = cmsups_get_sub_info_ptr((sys_modem_as_id_e_type)i);

    if (cmsups_sub_info_ptr != NULL && cmsups_sub_info_ptr->sups_info_list_head != NULL)
    {
      cmph_get_comm_mode_band_capab_with_pm(&comm_mode, &comm_band, &comm_lte_band, &comm_tds_band, (sys_modem_as_id_e_type)i);
      if ((((comm_mode & SD_SS_MODE_PREF_TDS_GSM_WCDMA) == SD_SS_MODE_PREF_NONE) ||
          cmcall_is_lte_only_mode(cmph_map_sd_mode_pref_to_cm_mode_pref(comm_mode),
                                  cmutil_map_sd_band_pref_to_cm_band_pref(comm_band),
                                  comm_lte_band,
             cmutil_map_sd_tds_band_pref_to_cm_tds_band_pref(comm_tds_band))) &&
             cmwsups_check_for_cs_entry_in_list(cmsups_sub_info_ptr->sups_info_list_head)
         )
      {
        CM_MSG_HIGH_1("cmwsups_timer_proc: clear sub %d list", i);
        cmwsups_list_clear((sys_modem_as_id_e_type)i);
        return;
      }
    }
  }
#endif

  /* process retry
  */
  for (i = 0; i < sups_ptr->sub_info_count; i++)
  {
    cmsups_sub_info_ptr = cmsups_get_sub_info_ptr((sys_modem_as_id_e_type)i);
    if (cmsups_sub_info_ptr != NULL)
    {
      sups_list = cmsups_sub_info_ptr->sups_info_list_head;
      if (sups_list != NULL)
      {
        while (sups_list != NULL)
        {
          temp_list = sups_list->next;

          /* If next orig uptime is 0 means origination is already sent.
          ** No need to send again.
          ** If next orig uptime is non-zero and smaller than current uptime
          ** then time to send next retry for this sups command
          */
          if (sups_list->next_orig_uptime < uptime && sups_list->next_orig_uptime != 0)
          {
            sups_list->next_orig_uptime = 0; /* Set to 0 as uptime is alreayd sent */

            /*Since IP call is in progress, end pending CS ROUTE SUPS command*/
            if(cmcall_is_present_cellular_ip_call_per_sub((sys_modem_as_id_e_type)i) &&
               (cmsups_sub_info_ptr->sups_cmd_route == CMSUPS_CMD_ALLOW_CS)
              )
            {
              if (sups_list == cmsups_sub_info_ptr->sups_info_list)
              {
                cmsups_sub_info_ptr->sups_info_list = NULL;
              }
              /* Send failure event to clients */
              cmwsups_send_sups_fail_event((sys_modem_as_id_e_type)i, sups_list);
              cmwsups_mem_free_sups_list_info(sups_list, &(cmsups_sub_info_ptr->sups_info_list_head));
              CM_MSG_HIGH_3("Timer expired but call in progress.freeing the memory ss ref:%d,next_orig:%d,strt_time:%d",
                            sups_list->sups_cmd_info.ss_ref,
                            sups_list->next_orig_uptime,
                            sups_list->sr_start_uptime);
            }
            else
            {
            cmsups_client_cmd_resend(sups_list); /* If sups send fails, corresponding sups record is freed */

            CM_MSG_HIGH_3("Resending the Sups cmd: as_id %d ss_ref %d next uptime %d",
                          sups_list->sups_cmd_info.cmd_subs_id,
                          sups_list->sups_cmd_info.ss_ref,
                          sups_list->next_orig_uptime);
          }
          }
          else if (sups_list->sr_start_uptime + SUPS_RETRY_MAX_DURATION < uptime)
          {
            /* If a sups record exceeded max duration of attempts, free the memory
            ** If next orig time is 0 mans we have sent the request and we are waiting for response from NAS
            ** so we should not clear the sups, it will be cleared once we get the response from LL
            ** if next orig time is non zero it means we have got the response, so clear now
            */
            if (sups_list->next_orig_uptime != 0)
            {
              if (sups_list == cmsups_sub_info_ptr->sups_info_list)
              {
                cmsups_sub_info_ptr->sups_info_list = NULL;
              }
              /* Send failure event to clients */
              cmwsups_send_sups_fail_event((sys_modem_as_id_e_type)i, sups_list);
              cmwsups_mem_free_sups_list_info(sups_list, &(cmsups_sub_info_ptr->sups_info_list_head));
              CM_MSG_HIGH_3("SUPS MAX timer expired freeing the memory ss ref:%d,next_orig:%d,strt_time:%d",
                            sups_list->sups_cmd_info.ss_ref,
                            sups_list->next_orig_uptime,
                            sups_list->sr_start_uptime);
            }
            else
            {
              CM_MSG_HIGH_3("SUPS MAX timer expired but not free'g the memory, wait for resp from NAS ss ref:%d,next_orig:%d,strt_time:%d",
                            sups_list->sups_cmd_info.ss_ref,
                            sups_list->next_orig_uptime,
                            sups_list->sr_start_uptime);
            }
          }
          else
          {
            CM_MSG_HIGH_3("SUPS hold for retry, as_id %d ss_ref %d next uptime %d",
                          sups_list->sups_cmd_info.cmd_subs_id,
                          sups_list->sups_cmd_info.ss_ref,
                          sups_list->next_orig_uptime);
          }
          sups_list = temp_list;
        }
      }
    }
  }
#endif
}

/*===========================================================================
FUNCTION cmsups_dem_is_apps_wakeup_from_suspend_event

DESCRIPTION
Determines if the event should wakeup the processor from the suspend state

DEPENDENCIES
None.

RETURN VALUE
True : If the apps processor needs to be woken up
False: Otherwise

SIDE EFFECTS
none

===========================================================================*/
boolean cmsups_dem_is_apps_wakeup_from_suspend_event(
  cm_sups_event_e_type   sups_event
  /* SUPS event */
)
/*lint -esym(715, sups_event) */
{
#ifndef FEATURE_APPS_POWER_COLLAPSE
  /* If Apps power collapse is not defined, return ture
  */
  return TRUE;
#else

  switch (sups_event)
  {
    case CM_SUPS_EVENT_USS_NOTIFY_IND:
    case CM_SUPS_EVENT_USS_IND:
    case CM_SUPS_EVENT_LCS_LOCATION_NOTIFICATION_IND:
      return TRUE;

    default:
      return FALSE;
  }

#endif
} /* cmsups_dem_is_apps_wakeup_from_suspend_event */
/*lint +esym(715, sups_event) */

#else /* FEATURE_WCDMA || FEATURE_GSM */

/*===========================================================================
FUNCTION cmwsups_dummy

DESCRIPTION

Dummy function, defined to get rid of
'Warning: C3040E: no external declaration in translation unit'
ADS 1.2 compiler warning

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None

===========================================================================*/



void cmwsups_dummy(void)
/*lint -esym(714,cmwsups_dummy) */
/*lint -esym(765,cmwsups_dummy)
** Can't be static as it would result in compiler warnings
*/
{
}

#endif /* FEATURE_GSM || FEATURE_WCDMA */
