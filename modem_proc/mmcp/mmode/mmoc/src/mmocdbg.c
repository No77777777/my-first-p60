/*====*====*===*====*====*====*====*====*====*====*====*====*====*====*====*
               M U L T I M O D E   C O N T R O L L E R ( MMoC )

                     D E B U G      M O D U L E

GENERAL DESCRIPTION
   This module contains the debug related functionality of MMoC.

EXTERNALIZED FUNCTIONS
  mmocdbg_print_message ()


INITIALIZATION AND SEQUENCING REQUIREMENTS
  The MMoC task should be started before DEBUG APIs are called.


Copyright (c) 2014 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/05/13   jvo     Added 5 mmocdbg_print_x functions to functionalize usage
                   of MMOC_PRINT_MSG_HIGH under MMOC_STRING_MAPS_ACTIVE
08/28/13   jvo     Cleanup remaining Fusion pieces
09/27/12   vm      Removing DIME featurization and cleanup
03/02/12   rk      Adding support for deativating dormant protocol through
                   MMOC
02/15/12   vk      Replace usage of AEE library function with corresponding
                   CoreBSP library
01/17/12   aj/cl   SC SVLTE SO 33 changes
01/11/12   xs      Remove MMOC_TRANS_PWR_SAVE_EXIT transaction state
09/14/11   vk      New MMOC Transaction "MMOC_TRANS_DEACT_1XCSFB_CMD" related
                   changes.
07/22/11   xs      MMOC SC_SVLTE 1.0 merging to Mainline
07/13/11   nk      Updated stubs in UT environment to be in sync with QTF.
05/20/11   ak      Adding new prot state for blocked RAT on SVLTEII.
05/16/11   xs      Added another stop mode rquest to deactivate NAS in suspend
                   state
05/06/11   rk      Correcting the order in mmocdbg_trans_e_type_string_map
03/09/11   cl      Add support for SVLTE2 MMSS arbitration
02/28/11   aj      Add wait for activation_cnf state in DO to LTE reselection
12/10/10   rk      MO/MT 1XCSFB call successful scenarios support
12/09/10   rk      Updating debug string arrays
11/10/10   am      Changes for MUTE to MOB migration
11/05/10   gm      Changes to wait for session open confirmation
10/27/10   rk      Reverting support for deativating dormant protocol
10/28/10   gm      Fix to compiler warning.
10/27/10   rk      Adding support for deativating dormant protocol through
                   MMOC
05/24/10   mp      Added support for session close and
                   MMOC_TRANS_MMGSDI_INFO_IND transaction to avoid nested
                   callbacks
01/28/10   mp      EF_EPRL Implementation
11/21/09   rn      LTE-eHRPD Redirection support
10/16/08   sv      Correcting Featurization for Memory Reduction
09/19/08   jd      Memory reduction changes
04/28/08   fj      Lint cleanup, CR 144389
03/12/08   aj      Edit header to reflect p4 path and revision
03/19/07   ka      Replacing banned apis
01/31/07   ka      Adding new state in MMOC to wait for TMC to ack operating
                   mode change.
08/02/06   ka      Adding additional entry in prot_state string map.
04/03/06   ka      Changing copyright.
12/20/05   ka      Initial revision

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "mmcp_variation.h"
#include "mmocdbg.h" /* Includes mmoc debug related info */
#include <string.h>
#include <stringl/stringl.h>

#ifdef FEATURE_MMOC_UNIT_TEST
#error code not present
#endif

/*===========================================================================
=============================================================================
=============================================================================
==================== MMoC DEBUG STRING MAPS =================================
=============================================================================
=============================================================================
===========================================================================*/

#ifndef FEATURE_MMOC_LOW_MEM_TARGET

#ifdef MMOC_STRING_MAPS_ACTIVE

/* Provide strings for  values mmoc_cmd_e_type */
const char *mmocdbg_cmd_e_type_string_map [ MMOC_CMD_MAX ] =
{
  "SUBSCRIPTION_CHGD",
  "PROT_GEN_CMD",
  "OPRT_MODE_CHGD",
  "WKUP_FROM_PWRSAVE",
  "DUAL_STANDBY_CHGD",
  "PROT_REDIR_IND",
  "PROT_HO_IND",
  "MMGSDI_INFO_IND",
  "DEACT_1XCSFB_PROT",
  "SUSPEND_SS",
  "DEACT_FROM_DORMANT",
  "MCFG_REFRESH",
  "SUSPEND_RESUME_IND"
};


/* Provide strings for  values mmoc_rpt_e_type */
const char *mmocdbg_rpt_e_type_string_map [ MMOC_RPT_MAX ] =
{
  "PROT_DEACTD_CNF",
  "PROT_AUTO_DEACTD_IND",
  "PH_STAT_CHGD_CNF",
  "PROT_GEN_CMD_CNF",
  "PROT_AUTO_ACTD_IND",
  "MMGSDI_CNF",
  "DS_STAT_CHGD_CNF",
  "ACTIVATION_CNF",
  "PS_DETACH_CNF",
  "1XCSFB_PROT_DEACTD_CNF",
  "IRAT_HOLD_USER_ACT_CNF",
  "UE_MODE_SWITCH_CNF",
  "SUBS_CAP_CHGD_CNF",
  "SD_NAM_SEL_CNF",
  "RPT_UE_MODE_SWITCH_CNF_1X",
  "RPT_UE_MODE_SWITCH_CNF_HDR"
};


/* Provide strings for  values mmoc_trans_state_e_type */
const char *mmocdbg_trans_state_e_type_string_map [ MMOC_TRANS_STATE_MAX ] =
{
  "NULL",
  "PROT_DEACT_ENTER",
  "WAIT_DEACTD_CNF",
  "MMGSDI_READ_ENTER",
  "MMGSDI_READ_CNF",
  "PROT_PH_STAT_ENTER",
  "WAIT_PH_STAT_CNF",
  "GEN_CMD_ENTER",
  "WAIT_GEN_CMD_CNF",
  "WAIT_AUTO_DEACTD_IND",
  "WAIT_AUTO_ACTD_IND",
  "HDR_DEACT_ENTER",
  "WAIT_HDR_DEACTD_CNF",
  "PROT_REDIR_ENTER",
  "WAIT_SESSION_OPEN_CNF",
  "PROT_HO_ENTER",
  "DS_STAT_CHGD_ENTER",
  "WAIT_DS_STAT_CHGD_CNF",
  "GEN_CMD_ACTIVATION_ENTER",
  "WAIT_ACTIVATION_CNF",
  "WAIT_PS_DETACH_CNF",
  "HYBR2_DEACT_ENTER",
  "WAIT_HYBR2_DEACTD_CNF",
  "WAIT_DEACTD_CNF_GWL",
  "WAIT_1XCSFB_DEACT_CNF",
  "SUSPEND_STACK_ENTER",
  "RESUME_STACK_ENTER",
  "WAIT_HOLD_USER_ACT_CNF",
  "WAIT_UE_MODE_SWITCH",
  "WAIT_SUBS_CAP_CHGD_ENTER",
  "WAIT_SUBS_CAP_CHGD_CNF",
  "SUSPEND_RESUME_ENTER",
  "WAIT_SD_NAM_SEL_CNF",
  "WAIT_UE_MODE_SWITCH_CNF_1X",
  "WAIT_UE_MODE_SWITCH_CNF_HDR"
};

/* Provide strings for  values mmoc_trans_e_type */
const char *mmocdbg_trans_e_type_string_map [ MMOC_TRANS_MAX ] =
{
  "NULL",
  "SUBSC_CHGD",
  "ONLINE",
  "OFFLINE",
  "PROT_GEN_CMD",
  "PWR_DOWN",
  "PWR_SAVE_ENTER",
  "PROT_AUTO_DEACTD_IND",
  "DUAL_STANDBY_CHGD",
  "PROT_REDIR_IND",
  "PROT_HO_IND",
  "MMGSDI_INFO_IND",
  "DEACT_1XCSFB_CMD",
  "SUSPEND_SS",
  "DEACT_FROM_DORMANT",
  "MCFG_REFRESH",
  "SUSPEND_RESUME"
};

/* Provide strings for  values prot_state_e_type */
const char *mmocdbg_prot_state_e_type_string_map [ PROT_STATE_MAX ] =
{
  "NULL",
  "ONLINE_CDMA",
  "OFFLINE_CDMA",
  "ONLINE_AMPS",
  "OFFLINE_AMPS",
  "ONLINE_GWL",
  "ONLINE_HDR",
  "OFFLINE",
  "FTM",
  "PWR_SAVE",
  "ONLINE_DED_MEAS",
  "RESET",
  "LPM",
  "PWR_DOWN",
  "GPSONE_MSBASED",
  "DORMANT_GWL",
  "BLOCKED"
};

#endif /* MMOC_STRING_MAPS_ACTIVE */

mmoc_trans_e_type                    dbg_current_trans = MMOC_TRANS_MAX;
mmoc_trans_state_e_type              dbg_current_trans_state = MMOC_TRANS_STATE_MAX;
dword dbg_curr_uptime  = 0;


/* <EJECT> */
/*===========================================================================
=============================================================================
=============================================================================
==================== MMoC DEBUG RELATED FUNCTIONS ===========================
=============================================================================
=============================================================================
===========================================================================*/

/* <EJECT> */

/*===========================================================================
FUNCTION  mmocdbg_sys_as_id_to_uint32

DESCRIPTION
   This function increments the subscription ID by 1 and converts it from
   sys_modem_as_id_e_type to uint32.
   Example:
   SYS_MODEM_AS_ID_NONE -> 0
   SYS_MODEM_AS_ID_1 -> 1
   SYS_MODEM_AS_ID_2 -> 2

RETURN VALUE
  uint32

SIDE EFFECTS
  None

===========================================================================*/
static uint32 mmocdbg_sys_as_id_to_uint32(
  sys_modem_as_id_e_type         asubs_id
  /* Subscription ID */
)
{
  /* if asubs_id is SYS_MODEM_AS_ID_NONE, converted_subid is 0 */
  uint32 converted_subid = 0; 

  if(asubs_id >= SYS_MODEM_AS_ID_1 &&
     asubs_id < MAX_SIMS)
  {
    /* if asubs_id is in range, add 1 */
    converted_subid = (uint32)(asubs_id + 1);
  }
  return converted_subid;
}

/*===========================================================================

FUNCTION MMOCDBG_PRINT_MESSAGE

DESCRIPTION

 Behaves as printf() - takes the very same format string and any number of
 parameters and allows to print <stuff> to diagnostic monitor output
 screen.

 The problem is that %s in the format string does not work for
 MSG_HIGH (and other MSG_ macros) plus those macros support only
 maximum of 3 arguments, so this function was provided to remedy both problems.

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmocdbg_print_message
(

  const char *string,
  /* Format in which string needs to be printed */
  ...
)
{
  va_list   arg_ptr = NULL;
  char        msg_str[ MMOC_MAX_STRING_LENGTH];

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Clear the string buffer
  */
  memset (msg_str, 0, sizeof(msg_str));

  /*lint -save -e64 -e718 -e746 -e63 -e530 */
  va_start( arg_ptr, string );
  /*lint -restore */

  /*  Writing max of MMOC_MAX_STRING_LENGTH-1 characters only,
  **  so the end null character stays untouched.
  */
  (void)vsnprintf (msg_str, (MMOC_MAX_STRING_LENGTH - 1), string, arg_ptr);
  va_end( arg_ptr );

  MSG_SPRINTF_1(MSG_SSID_DFLT, MSG_LVL_HIGH, "=MMOC= %s", msg_str);

} /* mmocdbg_print_message() */


/*===========================================================================

FUNCTION mmocdbg_print_protocol_state

DESCRIPTION
  Prints out the current protocol state information

DEPENDENCIES
  mmocdbg_print_message()

===========================================================================*/
void mmocdbg_print_protocol_state
(
  mmoc_state_info_s_type *mmoc_info_ptr
  /* Pointer to MMoC's state information */
)
{
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  multimode_stack_e_type stk = MM_STACK_NONE;
  uint32 diag_sub_pri = mmocdbg_sys_as_id_to_uint32(SYS_MODEM_AS_ID_1);
  uint32 diag_sub_sec = mmocdbg_sys_as_id_to_uint32(SYS_MODEM_AS_ID_2);

  /* Validate input parameter(s) */
  if ( mmoc_info_ptr == NULL )
  {
    MMOC_ERR_0( "NULL mmoc_info_ptr");
    MMOC_ASSERT(FALSE);
    return;
  }

  /* Validate indices into string array */
  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    for(stk = MM_STACK_0; stk < mmoc_info_ptr->sub_prop[sub]->nStacks && stk < MAX_STACKS ; stk++)
    {
      if(mmoc_info_ptr->sub_prop[sub]->stack_prop[stk]->prot_state < 0 ||
          mmoc_info_ptr->sub_prop[sub]->stack_prop[stk]->prot_state >= PROT_STATE_MAX)
      {
        MMOC_ERR_3("Bad prot_state %d sub %d stk %d",
                   mmoc_info_ptr->sub_prop[sub]->stack_prop[stk]->prot_state,
                   sub,
                   stk);

        MMOC_ASSERT(FALSE);
        return;
      }
    }
  }

#ifdef MMOC_STRING_MAPS_ACTIVE

  /* Print state with interpreted strings */
  if(mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1] != NULL)
  {
#ifdef FEATURE_MMODE_DUAL_SIM
    if(mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2] != NULL)
    {
      /* 3 + 3 stacks */
      if(mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_2] != NULL &&
          mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_2] != NULL)
      {
        MMOC_PRINT_MSG_6_EXT("prot_states PRI_SUB: %d(%s), %d(%s), %d(%s)",
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_0]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_0]->prot_state],
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_1]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_1]->prot_state],
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_2]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_2]->prot_state],
                             &diag_sub_pri);
        
        MMOC_PRINT_MSG_6_EXT("prot_states SEC_SUB: %d(%s), %d(%s), %d(%s)",
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_0]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_0]->prot_state],
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_1]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_1]->prot_state],
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_2]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_2]->prot_state],
                             &diag_sub_sec);
      }
      /* 3 + 2 stacks */
      else if(mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_2] != NULL &&
              mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_2] == NULL)
      {
        MMOC_PRINT_MSG_6_EXT("prot_states PRI_SUB: %d(%s), %d(%s), %d(%s)", 
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_0]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_0]->prot_state],
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_1]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_1]->prot_state],
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_2]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_2]->prot_state],
                             &diag_sub_pri);

        MMOC_PRINT_MSG_4_EXT("prot_states SEC_SUB: %d(%s), %d(%s)",
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_0]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_0]->prot_state],
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_1]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_1]->prot_state],
                             &diag_sub_sec);
      }
      /* 2 + 3 stacks */
      else if(mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_2] == NULL &&
              mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_2] != NULL)
      {
        MMOC_PRINT_MSG_4_EXT("prot_states PRI_SUB: %d(%s), %d(%s) ",
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_0]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_0]->prot_state],
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_1]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_1]->prot_state],
                             &diag_sub_pri);

        MMOC_PRINT_MSG_6_EXT("prot_states SEC_SUB: %d(%s), %d(%s), %d(%s)",
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_0]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_0]->prot_state],
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_1]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_1]->prot_state],
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_2]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_2]->prot_state],
                             &diag_sub_sec);
      }
      /* 2 + 2 stacks */
      else if(mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_2] == NULL &&
              mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_2] == NULL)
      {
        MMOC_PRINT_MSG_4_EXT("prot_states PRI_SUB: %d(%s), %d(%s)",
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_0]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_0]->prot_state],
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_1]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_1]->prot_state],
                             &diag_sub_pri);

        MMOC_PRINT_MSG_4_EXT("prot_states SEC_SUB: %d(%s), %d(%s)",
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_0]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_0]->prot_state],
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_1]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_1]->prot_state],
                             &diag_sub_sec);
      }
    }
    else
#endif
    {
      /* 3 stacks */
      if(mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_2] != NULL)
      {
        MMOC_PRINT_MSG_6_EXT("prot_states PRI_SUB: %d(%s), %d(%s), %d(%s)",
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_0]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_0]->prot_state],
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_1]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_1]->prot_state],
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_2]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_2]->prot_state],
                             &diag_sub_pri);
      }
      /* 2 stacks */
      else
      {
        MMOC_PRINT_MSG_4_EXT("prot_states PRI_SUB: %d(%s), %d(%s)",
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_0]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_0]->prot_state],
                             mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_1]->prot_state,
                             mmocdbg_prot_state_e_type_string_map[mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_1]->prot_state],
                             &diag_sub_pri);
      }
    }

  }

#else

  /* Print state without interpreted strings */
  if(mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1] != NULL)
  {
#ifdef FEATURE_MMODE_DUAL_SIM
    if(mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2] != NULL)
    {
      /* 3 + 3 stacks */
      if(mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_2] != NULL &&
          mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_2] != NULL)
      {
        MMOC_MSG_HIGH_6("prot_states PRI_SUB: %d %d %d SEC_SUB: %d %d %d",
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_0]->prot_state,
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_1]->prot_state,
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_2]->prot_state,
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_0]->prot_state,
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_1]->prot_state,
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_2]->prot_state); /* Print has information regarding both subs, sub value passed is 0 */
      }
      /* 3 + 2 stacks */
      else if(mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_2] != NULL &&
              mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_2] == NULL)
      {
        MMOC_MSG_HIGH_5("prot_states PRI_SUB: %d %d %d SEC_SUB: %d %d",
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_0]->prot_state,
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_1]->prot_state,
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_2]->prot_state,
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_0]->prot_state,
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_1]->prot_state); /* Print has information regarding both subs, sub value passed is 0 */
      }
      /* 2 + 3 stacks */
      else if(mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_2] == NULL &&
              mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_2] != NULL)
      {
        MMOC_MSG_HIGH_5("prot_states PRI_SUB: %d %d SEC_SUB: %d %d %d",
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_0]->prot_state,
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_1]->prot_state,
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_0]->prot_state,
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_1]->prot_state,
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_2]->prot_state); /* Print has information regarding both subs, sub value passed is 0 */
      }
      /* 2 + 2 stacks */
      else if(mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_2] == NULL &&
              mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_2] == NULL)
      {
        MMOC_MSG_HIGH_4("prot_states PRI_SUB: %d %d SEC_SUB: %d %d",
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_0]->prot_state,
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_1]->prot_state,
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_0]->prot_state,
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_2]->stack_prop[MM_STACK_1]->prot_state); /* Print has information regarding both subs, sub value passed is 0 */
      }
    }
    else
#endif
    {
      /* 3 stacks */
      if(mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_2] != NULL)
      {
        MMOC_MSG_HIGH_3("prot_states PRI_SUB: %d %d %d",
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_0]->prot_state,
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_1]->prot_state,
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_2]->prot_state);
      }
      /* 2 stacks */
      else
      {
        MMOC_MSG_HIGH_2("prot_states PRI_SUB: %d %d",
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_0]->prot_state,
                            mmoc_info_ptr->sub_prop[SYS_MODEM_AS_ID_1]->stack_prop[MM_STACK_1]->prot_state);
      }
    }

  }

#endif /* MMOC_STRING_MAPS_ACTIVE */
}


/*===========================================================================

FUNCTION mmocdbg_print_transaction_state

DESCRIPTION
  Prints out the current transaction state

DEPENDENCIES
  mmocdbg_print_message()

===========================================================================*/
void mmocdbg_print_transaction_state
(
  mmoc_state_info_s_type *mmoc_info_ptr
  /* Pointer to MMoC's state information */
)
{
  mmoc_trans_e_type                    current_trans;
  mmoc_trans_state_e_type              current_trans_state;
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = &mmoc_info_ptr->dev_prop;

  /* Validate input parameter(s) */
  if ( mmoc_info_ptr == NULL )
  {
    MMOC_ERR_0( "NULL mmoc_info_ptr");
    MMOC_ASSERT(FALSE); /* Fail test if this happens */
    return;
  }

  /* Initialize local after check for null */
  current_trans             = mmoc_dev_info->trans;
  current_trans_state       = mmoc_dev_info->trans_state;

  /* Validate index into string array */
  if ( current_trans < 0 || current_trans >= MMOC_TRANS_MAX )
  {
    MMOC_ERR_1( "Bad current_trans %d", current_trans);
    MMOC_ASSERT(FALSE); /* Fail test if this happens */
    return;
  }

  if ( current_trans_state < 0 ||
       current_trans_state >= MMOC_TRANS_STATE_MAX )
  {
    MMOC_ERR_1( "Bad current_trans_state %d", current_trans_state);
    MMOC_ASSERT(FALSE); /* Fail test if this happens */
    return;
  }

#ifdef MMOC_STRING_MAPS_ACTIVE
  /* Print state with interpreted strings */
  MMOC_PRINT_MSG_HIGH (
    "Curr_trans  %d(%s), Trans_state %d(%s)",
    current_trans,
    mmocdbg_trans_e_type_string_map [current_trans],
    current_trans_state,
    mmocdbg_trans_state_e_type_string_map [current_trans_state]);

#else
  /* Print state without interpreted strings */
  MMOC_MSG_HIGH_2(
    "Curr_trans  %d, Trans_state %d", current_trans, current_trans_state);

#endif /* MMOC_STRING_MAPS_ACTIVE */

} /* mmocdbg_print_transaction_state */


/*===========================================================================

FUNCTION mmocdbg_print_report_received

DESCRIPTION
  Prints out the received report

DEPENDENCIES
  mmocdbg_print_message()

RETURNS
  None

SIDE EFFECTS
  None

===========================================================================*/
void mmocdbg_print_report_received
(
  mmoc_rpt_msg_s_type *rpt_ptr
  /* The report that was received */
)
{
  mmoc_rpt_e_type                      report_name;

  /* Validate input parameter(s) */
  if ( rpt_ptr == NULL )
  {
    MMOC_ERR_0( "NULL rpt_ptr");
    MMOC_ASSERT(FALSE); /* Fail test if this happens */
    return;
  }

  /* Initialize local after check for null */
  report_name = rpt_ptr->rpt.name;

  /* Validate index into string array */
  if ( report_name < 0 || report_name >= MMOC_RPT_MAX )
  {
    MMOC_ERR_1( "Bad rpt_name %d", report_name);
    MMOC_ASSERT(FALSE); /* Fail test if this happens */
    return;
  }

#ifdef MMOC_STRING_MAPS_ACTIVE
  /* Print report with interpreted string */
  MMOC_PRINT_MSG_HIGH( "Recvd rpt %d(%s)", report_name,
                       mmocdbg_rpt_e_type_string_map [report_name]);

#else
  /* Print report without interpreted string */
  MMOC_MSG_HIGH_1( "Recvd rpt %d", report_name);

#endif /* MMOC_STRING_MAPS_ACTIVE */

} /* mmocdbg_print_report_received */


/*===========================================================================

FUNCTION mmocdbg_print_command_received

DESCRIPTION
  Prints out the received command

DEPENDENCIES
  mmocdbg_print_message()

RETURNS
  None

SIDE EFFECTS
  None

===========================================================================*/
void mmocdbg_print_command_received
(
  mmoc_cmd_msg_s_type *cmd_ptr
  /* The report that was received */
)
{
  mmoc_cmd_e_type                      command_name;

  /* Validate input parameter(s) */
  if ( cmd_ptr == NULL )
  {
    MMOC_ERR_0( "NULL cmd_ptr");
    MMOC_ASSERT(FALSE); /* Fail test if this happens */
    return;
  }

  /* Initialize local after check for null */
  command_name = cmd_ptr->cmd.name;


  /* Validate index into string array */
  if ( command_name < 0 || command_name >= MMOC_CMD_MAX )
  {
    MMOC_ERR_1( "Bad cmd_name %d", command_name);
    MMOC_ASSERT(FALSE); /* Fail test if this happens */
    return;
  }

#ifdef MMOC_STRING_MAPS_ACTIVE
  /* Print command with interpreted string */
  MMOC_PRINT_MSG_HIGH( "Recvd cmd %d(%s)", command_name,
                       mmocdbg_cmd_e_type_string_map [command_name] );

#else
  /* Print command without interpreted string */
  MMOC_MSG_HIGH_1( "Recvd cmd %d", command_name);

#endif /* MMOC_STRING_MAPS_ACTIVE */

} /* mmocdbg_print_command_received */


/*===========================================================================

FUNCTION mmocdbg_print_before_trans

DESCRIPTION
  Prints out the transaction state and protocol state before the transaction

DEPENDENCIES
  mmocdbg_print_message()

RETURNS
  None

SIDE EFFECTS
  None

===========================================================================*/
void mmocdbg_print_before_trans
(
  mmoc_state_info_s_type *mmoc_info_ptr
  /* Pointer to MMoC's state information */
)
{
  mmoc_trans_e_type  current_trans;
  mmoc_trans_state_e_type   current_trans_state;
  dword                              curr_uptime = time_get_uptime_secs();
  /* Validate input parameter(s) */
  if ( mmoc_info_ptr == NULL )
  {
    MMOC_ERR_0( "NULL mmoc_info_ptr");
    MMOC_ASSERT(FALSE); /* Fail test if this happens */
    return;
  }

  /* Initialize local after check for null */
  current_trans = mmoc_info_ptr->dev_prop.trans;
  current_trans_state = mmoc_info_ptr->dev_prop.trans_state;

  /* Validate index into string array */
  if ( current_trans < 0 || current_trans >= MMOC_TRANS_MAX )
  {
    MMOC_ERR_1( "Bad current_trans %d", current_trans);
    MMOC_ASSERT(FALSE); /* Fail test if this happens */
    return;
  }

  if ( current_trans_state < 0 ||
       current_trans_state >= MMOC_TRANS_STATE_MAX )
  {
    MMOC_ERR_1( "Bad current_trans_state %d", current_trans_state);
    MMOC_ASSERT(FALSE); /* Fail test if this happens */
    return;
  }

  if(current_trans == dbg_current_trans
      && current_trans_state == dbg_current_trans_state
      && curr_uptime == dbg_curr_uptime)
  {
    /* To avoid too many debugs in same second*/
    return;
  }

  dbg_current_trans = current_trans;
  dbg_current_trans_state = current_trans_state;
  dbg_curr_uptime = curr_uptime;


#ifdef MMOC_STRING_MAPS_ACTIVE
  /* Print state with interpreted strings */
  MMOC_PRINT_MSG_HIGH (
    "Before event was processed: Curr_trans %d(%s), Trans_state %d(%s)",
    current_trans,
    mmocdbg_trans_e_type_string_map [current_trans],
    current_trans_state,
    mmocdbg_trans_state_e_type_string_map [current_trans_state]);

#else
  /* Print state without interpreted strings */
  MMOC_MSG_HIGH_2(
    "Before event was processed: Curr_trans %d, Trans_state %d",
    current_trans, current_trans_state);

#endif /* MMOC_STRING_MAPS_ACTIVE */

  /* print out protocol state */
  mmocdbg_print_protocol_state(mmoc_info_ptr);
}


/*===========================================================================

FUNCTION mmocdbg_print_after_trans

DESCRIPTION
  Prints out the transaction state and protocol state after the transaction

DEPENDENCIES
  mmocdbg_print_message()

RETURNS
  None

SIDE EFFECTS
  None

===========================================================================*/
void mmocdbg_print_after_trans
(
  mmoc_state_info_s_type *mmoc_info_ptr
  /* Pointer to MMoC's state information */
)
{
  mmoc_trans_e_type current_trans;
  mmoc_trans_state_e_type   current_trans_state;
  dword                              curr_uptime = time_get_uptime_secs();

  /* Validate input parameter(s) */
  if ( mmoc_info_ptr == NULL )
  {
    MMOC_ERR_0( "NULL mmoc_info_ptr");
    MMOC_ASSERT(FALSE); /* Fail test if this happens */
    return;
  }

  /* Initialize local after check for null */
  current_trans = mmoc_info_ptr->dev_prop.trans;
  current_trans_state = mmoc_info_ptr->dev_prop.trans_state;

  /* Validate index into string array */
  if ( current_trans < 0 || current_trans >= MMOC_TRANS_MAX )
  {
    MMOC_ERR_1( "Bad current_trans %d", current_trans);
    MMOC_ASSERT(FALSE); /* Fail test if this happens */
    return;
  }

  if ( current_trans_state < 0 ||
       current_trans_state >= MMOC_TRANS_STATE_MAX )
  {
    MMOC_ERR_1( "Bad current_trans_state %d", current_trans_state);
    MMOC_ASSERT(FALSE); /* Fail test if this happens */
    return;
  }

  if(current_trans == dbg_current_trans
      && current_trans_state == dbg_current_trans_state
      && curr_uptime == dbg_curr_uptime)
  {
    /* To avoid too many debugs in same second*/
    return;
  }

  dbg_current_trans = current_trans;
  dbg_current_trans_state = current_trans_state;
  dbg_curr_uptime = curr_uptime;

#ifdef MMOC_STRING_MAPS_ACTIVE
  /* Print state with interpreted strings */
  MMOC_PRINT_MSG_HIGH (
    "After event was processed: Curr_trans %d(%s), Trans_state %d(%s)",
    current_trans,
    mmocdbg_trans_e_type_string_map [current_trans],
    current_trans_state,
    mmocdbg_trans_state_e_type_string_map [current_trans_state]);

#else
  /* Print state without interpreted strings */
  MMOC_MSG_HIGH_2(
    "After event was processed: Curr_trans %d, Trans_state %d",
    current_trans, current_trans_state);

#endif /* MMOC_STRING_MAPS_ACTIVE */

  /* print out protocol state */
  mmocdbg_print_protocol_state(mmoc_info_ptr);
}

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION mmocdebug_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for mmocdbg global variables

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
void mmocdebug_qsh_mdump_collect()
{
  qsh_mdump_collect_high(&dbg_current_trans,sizeof(mmoc_trans_e_type));
  qsh_mdump_collect_high(&dbg_current_trans_state,sizeof(mmoc_trans_state_e_type));
  qsh_mdump_collect_high(&dbg_curr_uptime,sizeof(dword));
}
#endif

#endif /* !FEATURE_MMOC_LOW_MEM_TARGET */

