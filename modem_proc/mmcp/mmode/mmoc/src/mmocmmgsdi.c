/*====*====*===*====*====*====*====*====*====*====*====*====*====*====*====*
               M U L T I M O D E   C O N T R O L L E R ( MMoC )

                              MMGSDI MODULE

GENERAL DESCRIPTION
  This module is the main implementation for the session based PRL read operation
  using MMGSDI APIs.

  The MMoC task has 2 seperate message queues, command queue for messages
  from CM and report queue for messages from the various protocols and MMGSDI.

EXTERNALIZED FUNCTIONS

  Command Interface:


  Report Interface:


  Functional Interface:

    mmocmmgsdi_init
    mmocmmgsdi_open_session
    mmocmmgsdi_session_client_id_reg_cb
    mmocmmgsdi_card_status_cb
    mmocmmgsdi_session_open_cb
    mmocmmgsdi_session_read_prl_cb
    mmocmmgsdi_process_subsc_chgd_mmgsdi_read_enter
    mmocmmgsdi_process_subsc_chgd_mmgsdi_read_cnf


  Others:


REGIONAL FUNCTIONS

  mmocmmgsdi_store_client_id
  mmocmmgsdi_error_e_type mmocmmgsdi_get_session_id_ptr
  mmocmmgsdi_error_e_type mmocmmgsdi_session_get_app_info_ptr


INITIALIZATION AND SEQUENCING REQUIREMENTS
  The MMoC task should be started before MMoC APIs are called.


Copyright (c) 2002 - 2011 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/mmoc/src/mmocmmgsdi.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/30/13   dk      propagate fix for CR 470877, to read PRL only iff subscription is available
04/20/12   sk      Fixing compiler warnings
03/21/12   ab      Added support for handling Auto deact ind
                   whem MMOC is waiting for open session confirm
08/19/11   fc      Depreciate test stubs for QTF.
07/20/11   gm      Support MMSS 3.1 with 1x PRL in NV & 3gpp subscr in RUIM
07/13/11   nk      Updated stubs in UT environment to be in sync with QTF.
06/15/11   rk      Adding featurization to read PRL
03/10/11   aj      Read other MMSS files only if PRL can be read.
01/11/11   am      MUTE migration to qtf changes
12/02/10   gm      Fix to compiler warning
11/11/10   am      Removing the compilation error in MOB
11/10/10   am      Reverting MUTE to MOB migration changes
11/10/10   am      Changes for MUTE to MOB migration
11/05/10   sv/gm   Changes to wait for session open confirmation
10/05/10   pm      Remove clk.h
10/01/10   ak      Merged the Fusion code from Dev branch.
09/16/10   gm      Fix compiler warning for unused variables.
07/20/10   mp      mmgsdi_len_type parameter in mmgsdi_session_read_transparent
                   changed to '0' as requested by MMGSDI team
08/05/10   rk      Added support to update SD with GW session info
07/20/10   mp      mmgsdi_len_type parameter in mmgsdi_session_read_transparent
                   changed to '0' as requested by MMGSDI team
05/24/10   mp      Added support for session close and
                   MMOC_TRANS_MMGSDI_INFO_IND transaction to avoid nested callbacks
05/18/10   mp      Fixed ARM compilation issue
03/21/10   mp      MLPL/MSPL/PLMN Interface for MMSS 3.1 support
01/28/10   mp      Initial creation for EF_EPRL Implementation.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "mmcp_variation.h"
#include "customer.h" /* Customer configuration file */
#ifndef FEATURE_CUSTMMODE_H
#error Need to pull in custmmode.h!!! /* custmmode.h has to be pulled in   */
#endif

#include "comdef.h"   /* Definition for basic types and macros             */
#include "msg.h"      /* Message output services.                          */
#include "err.h"      /* Error and error fatal services.                   */
#include "cmd.h"      /* Command header for inter-task communication.      */

#include "sd.h"       /* Interface for System Determination services.      */
#include "sd_v.h"     /* Internal interface definitions for SD.            */
#include "sd_i.h"     /* Internal interface definitions for SD.            */
#include "prl.h"      /* Interface for PRL services.                       */
#include "prot.h"     /* Common header for mmoc <-> protocol communications*/
#include "mmoc.h"     /* External interface definitions for MMoC.          */
#include "mmoci.h"    /* Internal interface definitions for MMoC.          */
#include "mmocdbg.h"  /* For mmocdbg_print_message support                 */
#include "nv.h"       /* Interface to NV Service.                          */

#include "mmgsdilib.h"          /* Interface to MMGSDI sesion based APIs   */
#include "mmgsdilib_common.h"   /* Interface to MMGSDI sesion based APIs   */
#include "mmgsdisessionlib.h"   /* Interface to MMGSDI sesion based APIs   */

#ifdef FEATURE_UIM
#include "uim.h"
#endif

#ifdef FEATURE_NV_RUIM
#include "nvruimi.h"
#endif

#ifndef FEATURE_MMOC_UNIT_TEST
#include "DDISoftwareSwitches.h" /* Interface for GPIO &other bit i/o services */
#include "DALSys.h"
#include "DALDeviceId.h"
#include "DALStdErr.h"
#endif

#include "task.h"     /* Interface for global task resources.              */
#include "dog.h"      /* Interface to watchdog procedures.                 */
#ifdef MMOC_USES_DB_SERVICES
#include "db.h"       /* Interface to database services.                   */
#endif /* ifdef(MMOC_USES_DB_SERVICES) */

#include "event.h"    /* Interface for system event report service.        */

#ifdef FEATURE_MMOC_UNIT_TEST
#error code not present
#endif
#include  "cmll.h"
//#include "cm_v.h"
//#include "cm_i.h"
//#include "mmoci.h"


/* <EJECT> */
/*===========================================================================
=============================================================================
=============================================================================
============================== DEPENDENT FEATURES ===========================
=============================================================================
=============================================================================
===========================================================================*/

/* This set of features must be defined for Multi-mode operation.
*/
#if !defined(FEATURE_SD20)
#error FEATURE_SD20 must be defined.
#endif

#ifndef FEATURE_MMODE_QTF
#if(defined(FEATURE_MMOC_UNIT_TEST) && !defined(FEATURE_UIM_RUN_TIME_ENABLE))
#error code not present
#endif
#endif /* !FEATURE_MMODE_QTF */

/*===========================================================================
=============================================================================
=============================================================================
==================== MMoC Forward declarations ==============================
=============================================================================
=============================================================================
===========================================================================*/

static void mmocmmgsdi_session_closed
(
  mmgsdi_session_id_type           mmocmmgsdi_session_id
);

/*===========================================================================

FUNCTION mmocmmgsdi_get_mm_id_per_session_type

DESCRIPTION
  Function to get the mm_id based on session type

===========================================================================*/
static mm_sub_stk_id_s_type mmocmmgsdi_get_mm_id_per_session_type(mmgsdi_session_type_enum_type session_type)
{
  mm_sub_stk_id_s_type mm_id;
  mm_id.asubs_id = SYS_MODEM_AS_ID_NONE;
  mm_id.stk_id   = MM_STACK_0;

  switch(session_type)
  {
    case MMGSDI_GW_PROV_PRI_SESSION:
    case MMGSDI_1X_PROV_PRI_SESSION:
    case MMGSDI_CARD_SESSION_SLOT_1:
      mm_id.asubs_id = SYS_MODEM_AS_ID_1;
      break;

    case MMGSDI_GW_PROV_SEC_SESSION:
    case MMGSDI_1X_PROV_SEC_SESSION:
    case MMGSDI_CARD_SESSION_SLOT_2:
      mm_id.asubs_id = SYS_MODEM_AS_ID_2;
      break;

#ifdef FEATURE_MMODE_TRIPLE_SIM
    case MMGSDI_GW_PROV_TER_SESSION:
    case MMGSDI_1X_PROV_TER_SESSION:
    case MMGSDI_CARD_SESSION_SLOT_3:
      mm_id.asubs_id = SYS_MODEM_AS_ID_3;
#endif
      break;

    default:
      break;

  }

  return mm_id;

}

static mmgsdi_session_type_enum_type mmocmmgsdi_map_slot_id_to_card_session(
  mmgsdi_slot_id_enum_type slot_id
)
{

  switch(slot_id)
  {
    case MMGSDI_SLOT_2:
      return MMGSDI_CARD_SESSION_SLOT_2;

    case MMGSDI_SLOT_3:
      return MMGSDI_CARD_SESSION_SLOT_3;

    case MMGSDI_SLOT_1:
    default:
      return MMGSDI_CARD_SESSION_SLOT_1;
  }
}


/* <EJECT> */
/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/* <EJECT> */
/*===========================================================================
=============================================================================
=============================================================================
==================== MMoC IMPLEMENATATION RELATED HELPER FUNCTIONS ==========
=============================================================================
=============================================================================
===========================================================================*/

/*===========================================================================
FUNCTION MMOCMMGSDI_INIT

DESCRIPTION
  Initilize MMOC MGSDI interface.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void mmocmmgsdi_init
(
  mmoc_state_info_s_type   *mmoc_state_info_ptr
)
{
  mmgsdi_return_enum_type mmgsdi_status = MMGSDI_SUCCESS;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Register the MMGSDI event callback function */

  mmgsdi_status = mmgsdi_client_id_and_evt_reg(
                    mmocmmgsdi_card_status_cb,
                    mmocmmgsdi_session_client_id_reg_cb,
                    (mmgsdi_client_data_type)mmoc_state_info_ptr);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMOC_ERR_1 ("MMGSDI MMOC reg cmd failed : STATUS %d",
                mmgsdi_status);
  }

} /* mmocmmgsdi_init() */


/*===========================================================================
FUNCTION MMOCMMGSDI_STORE_CLIENT_ID

DESCRIPTION
  Internal helper function that stores the client ID

DEPENDENCIES
  none

RETURN VALUE
  None.

SIDE EFFECTS
  Updates stored client ID

===========================================================================*/
static void mmocmmgsdi_store_client_id
(           const mmgsdi_cnf_type      *cnf_ptr
)
{
  mmoc_mmgsdi_info_s_type *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr ();

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  *(&(mmgsdi_info_ptr->mmgsdi_client_id)) = *(&(cnf_ptr->client_id_and_evt_reg_cnf.response_header.client_id));

  MMOC_MSG_HIGH_1("MMGSDI reg. MMOC with Client id : mmgsdi_client_id %d",
                  mmgsdi_info_ptr->mmgsdi_client_id);

} /* mmocmmgsdi_store_client_id() */

#ifdef FEATURE_MMODE_DUAL_SIM
boolean mmoc_is_dsdx(void)
{
  if( cm_get_device_mode()== SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY ||
      cm_get_device_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE)
  {
    return TRUE;
  }

  return FALSE;
}
#endif

/*====================================================================

FUNCTION mmocmmgsdi_get_session_index

DESCRIPTION

  This function returns index to the session ID for the specified
  session type.  At most one session ID is associated with each session
  type.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  MMOC_MMGSDI_SUCCESS if successful
  MMOC_MMGSDI_NOTFOUND: Session ID for requested session type was not found

SIDE EFFECTS
  none

=====================================================================*/
static mmocmmgsdi_error_e_type mmocmmgsdi_get_session_index
(
  mmgsdi_session_type_enum_type mmocmmgsdi_session_type,
  unsigned int                  *mmocmmgsdi_session_index
)
{
  mmoc_mmgsdi_info_s_type *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr ();
  unsigned int index;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for(index = 0; index < MMOC_MAX_SESSION_IDS; index++)
  {
    /* Look for a session which matches with the type. No more than one
    ** session for any given session type is supported
    */
    if (mmocmmgsdi_session_type == mmgsdi_info_ptr->
        mmgsdi_session_id_table[index].session_type)
    {
      *mmocmmgsdi_session_index = index;
      return MMOC_MMGSDI_SUCCESS;
    }
  }

  return MMOC_MMGSDI_NOTFOUND;

} /* mmocmmgsdi_get_session_index */

/*====================================================================

FUNCTION mmocmmgsdi_is_session_opened

DESCRIPTION

  This function returns if an session is opened with MMGSDI for specified
  session type.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  TRUE : A valid session is opened for that session type
  FALSE: No valid session type is found

SIDE EFFECTS
  none

=====================================================================*/
static boolean mmocmmgsdi_is_session_opened
(
  mmgsdi_session_type_enum_type mmocmmgsdi_session_type
)
{
  mmoc_mmgsdi_info_s_type *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr();
  unsigned int index;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for(index = 0; index < MMOC_MAX_SESSION_IDS; index++)
  {
    /* Look for a session which matches with the type. No more than one
    ** session for any given session type is supported
    */
    if (mmocmmgsdi_session_type == mmgsdi_info_ptr->
        mmgsdi_session_id_table[index].session_type)
    {
      //if (mmgsdi_info_ptr->mmgsdi_session_id_table[index].app_info != MMGSDI_APP_NONE)
      if (mmgsdi_info_ptr->mmgsdi_session_id_table[index].session_opened)
      {
        return TRUE;
      }
    }
  }

  return FALSE;

} /* mmocmmgsdi_is_session_opened */

/*====================================================================

FUNCTION mmocmmgsdi_session_closed

DESCRIPTION

  This function sets the session_opened  variable to FALSE to indicate
  that session has been closed.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  None

SIDE EFFECTS
  none

=====================================================================*/
static void mmocmmgsdi_session_closed
(
  mmgsdi_session_id_type        mmocmmgsdi_session_id
)
{
  mmoc_mmgsdi_info_s_type *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr ();
  unsigned int index;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for(index = 0; index < MMOC_MAX_SESSION_IDS; index++)
  {
    /* Look for a session which matches with the session ID.
    **
    */
    if (mmocmmgsdi_session_id == mmgsdi_info_ptr->
        mmgsdi_session_id_table[index].session_id)
    {
      mmgsdi_info_ptr->mmgsdi_session_id_table[index].session_opened = FALSE;
      break;
    }
  }

  return;

} /* mmocmmgsdi_session_closed */

/*====================================================================

FUNCTION mmocmmgsdi_get_session_id_ptr

DESCRIPTION

  This function returns a pointer to the session ID for the specified
  session type.  At most one session ID is associated with each session
  type.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  MMOC_MMGSDI_SUCCESS if successful
  MMOC_MMGSDI_UNSUPPORTED: Function is unsupported on this target
  MMOC_MMGSDI_NOTFOUND: Session ID for requested session type was not found

SIDE EFFECTS
  none

=====================================================================*/
static mmocmmgsdi_error_e_type mmocmmgsdi_get_session_id_ptr
(
  mmgsdi_session_type_enum_type mmocmmgsdi_session_type,
  mmgsdi_session_id_type         **mmocmmgsdi_session_id_ptr
)
{
  mmoc_mmgsdi_info_s_type *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr ();
  unsigned int index;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for (index = 0; index < MMOC_MAX_SESSION_IDS; index++)
  {
    /* Look for a session which matches with the type. No more than one
    ** session for any given session type is supported
    */
    if (mmocmmgsdi_session_type == mmgsdi_info_ptr->
        mmgsdi_session_id_table[index].session_type)
    {
      *mmocmmgsdi_session_id_ptr = &(mmgsdi_info_ptr->
                                     mmgsdi_session_id_table[index].session_id);
      return MMOC_MMGSDI_SUCCESS;
    }
  }

  return MMOC_MMGSDI_NOTFOUND;

} /* mmocmmgsdi_get_session_id_ptr */

/*====================================================================

FUNCTION mmocmmgsdi_get_session_type

DESCRIPTION

  This function returns a pointer to the session ID for the specified
  session type.  At most one session ID is associated with each session
  type.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  MMOC_MMGSDI_SUCCESS if successful
  MMOC_MMGSDI_UNSUPPORTED: Function is unsupported on this target
  MMOC_MMGSDI_NOTFOUND: Session ID for requested session type was not found

SIDE EFFECTS
  none

=====================================================================*/
static mmocmmgsdi_error_e_type mmocmmgsdi_get_session_type
(
  mmgsdi_session_id_type            mmocmmgsdi_session_id,
  mmgsdi_session_type_enum_type    *mmocmmgsdi_session_type
)
{
  mmoc_mmgsdi_info_s_type *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr ();
  unsigned int index;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for(index = 0; index < MMOC_MAX_SESSION_IDS; index++)
  {
    /* Look for a session which matches with session id.
    */
    if (mmocmmgsdi_session_id == mmgsdi_info_ptr->
        mmgsdi_session_id_table[index].session_id)
    {
      *mmocmmgsdi_session_type = mmgsdi_info_ptr->
                                 mmgsdi_session_id_table[index].session_type;
      return MMOC_MMGSDI_SUCCESS;
    }
  }

  return MMOC_MMGSDI_NOTFOUND;

} /* mmocmmgsdi_get_session_id_ptr */

/*====================================================================

FUNCTION mmocmmgsdi_session_get_tbl_entry_ptr

DESCRIPTION

  This function returns a pointer to the app info for the specified
  session type.  At most one app_info structure is associated with each
  session type.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.
  app_info is valid only after MMGSDI_SUBSCRIPTION_READY evt is received.

RETURN VALUE
  MMOC_MMGSDI_SUCCESS if successful
  MMOC_MMGSDI_UNSUPPORTED: Function is unsupported on this target
  MMOC_MMGSDI_NOTFOUND: app_info for requested session type was not found

SIDE EFFECTS
  none

=====================================================================*/
static mmocmmgsdi_error_e_type mmocmmgsdi_session_get_tbl_entry_ptr
(
  mmgsdi_session_id_type    mmocmmgsdi_session_id,
  mmocmmgsdi_session_id_table_entry_s_type       **mmocmmgsdi_tbl_entry_ptr
)
{
  mmoc_mmgsdi_info_s_type *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr ();
  unsigned int index;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for(index = 0; index < MMOC_MAX_SESSION_IDS; index++)
  {
    /* Look for a session which matches with session id.
    */
    if (mmocmmgsdi_session_id == mmgsdi_info_ptr->
        mmgsdi_session_id_table[index].session_id)
    {
      *mmocmmgsdi_tbl_entry_ptr = &(mmgsdi_info_ptr->
                                    mmgsdi_session_id_table[index]);
      return MMOC_MMGSDI_SUCCESS;
    }
  }

  return MMOC_MMGSDI_NOTFOUND;

} /*mmocmmgsdi_session_get_tbl_entry_ptr */


/*====================================================================

FUNCTION mmocmmgsdi_slot_get_tbl_entry_ptr

DESCRIPTION

  This function returns a pointer to the app info for the specified
  session type.  At most one app_info structure is associated with each
  session type.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.
  app_info is valid only after MMGSDI_SUBSCRIPTION_READY evt is received.

RETURN VALUE
  MMOC_MMGSDI_SUCCESS if successful
  MMOC_MMGSDI_UNSUPPORTED: Function is unsupported on this target
  MMOC_MMGSDI_NOTFOUND: app_info for requested session type was not found

SIDE EFFECTS
  none

=====================================================================*/
static mmocmmgsdi_error_e_type mmocmmgsdi_slot_get_tbl_entry_ptr
(
    mmgsdi_slot_id_enum_type      slot_id,
    mmocmmgsdi_session_id_table_entry_s_type      ** mmocmmgsdi_tbl_entry_ptr
)
{
  mmoc_mmgsdi_info_s_type *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr ();
  unsigned int index;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for(index = 0; index < MMOC_MAX_SESSION_IDS; index++)
  {
    /* Look for a session which matches with session id.
    */
    if (slot_id == mmgsdi_info_ptr->
         mmgsdi_session_id_table[index].slot_id)
    {
      *mmocmmgsdi_tbl_entry_ptr = &(mmgsdi_info_ptr->
          mmgsdi_session_id_table[index]);
      return MMOC_MMGSDI_SUCCESS;
    }
  }

  return MMOC_MMGSDI_NOTFOUND;

} /*mmocmmgsdi_session_get_tbl_entry_ptr */

/*====================================================================

FUNCTION mmocmmgsdi_get_session_app_info_ptr

DESCRIPTION

  This function returns a pointer to the app info for the specified
  session type.  At most one app_info structure is associated with each
  session type.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.
  app_info is valid only after MMGSDI_SUBSCRIPTION_READY evt is received.

RETURN VALUE
  MMOC_MMGSDI_SUCCESS if successful
  MMOC_MMGSDI_UNSUPPORTED: Function is unsupported on this target
  MMOC_MMGSDI_NOTFOUND: app_info for requested session type was not found

SIDE EFFECTS
  none

=====================================================================*/
static mmocmmgsdi_error_e_type mmocmmgsdi_session_get_app_info_ptr
(
  mmgsdi_session_id_type    mmocmmgsdi_session_id,
  mmgsdi_app_enum_type       **mmocmmgsdi_app_info_ptr
)
{
  mmoc_mmgsdi_info_s_type *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr ();
  unsigned int index;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for(index = 0; index < MMOC_MAX_SESSION_IDS; index++)
  {
    /* Look for a session which matches with session id.
    */
    if (mmocmmgsdi_session_id == mmgsdi_info_ptr->
        mmgsdi_session_id_table[index].session_id)
    {
      *mmocmmgsdi_app_info_ptr = &(mmgsdi_info_ptr->
                                   mmgsdi_session_id_table[index].app_info);
      return MMOC_MMGSDI_SUCCESS;
    }
  }

  return MMOC_MMGSDI_NOTFOUND;

} /*mmocmmgsdi_session_get_app_info_ptr */

/*====================================================================

FUNCTION mmocmmgsdi_session_get_session_type_ptr

DESCRIPTION

  This function returns a pointer to the session type which
  matches passed in session ID

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  MMOC_MMGSDI_SUCCESS if successful
  MMOC_MMGSDI_UNSUPPORTED: Function is unsupported on this target
  MMOC_MMGSDI_NOTFOUND: app_info for requested session type was not found

SIDE EFFECTS
  none

=====================================================================*/
static mmocmmgsdi_error_e_type mmocmmgsdi_session_get_session_type
(
  mmgsdi_session_id_type             mmocmmgsdi_session_id,
  mmgsdi_session_type_enum_type      **mmocmmgsdi_session_type_ptr
)
{
  mmoc_mmgsdi_info_s_type *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr ();
  unsigned int index;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for(index = 0; index < MMOC_MAX_SESSION_IDS; index++)
  {
    /* Look for a session which matches with session id.
    */
    if (mmocmmgsdi_session_id == mmgsdi_info_ptr->
        mmgsdi_session_id_table[index].session_id)
    {
      *mmocmmgsdi_session_type_ptr = &(mmgsdi_info_ptr->mmgsdi_session_id_table[index].session_type);
      return MMOC_MMGSDI_SUCCESS;
    }
  }

  return MMOC_MMGSDI_NOTFOUND;

} /*mmocmmgsdi_session_get_app_info_ptr */

/*===========================================================================

FUNCTION mmocmmgsdi_get_bitmask_to_read_items

DESCRIPTION
  This is a utility function called by the SUBSCRIPTION CHANGE transaction
  handler to figure out the items needed to be read from the MMGSDI card for
  this subscription change.

RETURN VALUE
  bit mask                  - Indicating the items to be read from card.
  MMOC_MMGSDI_MASK_NONE     - Nothing to be read from card.

===========================================================================*/
mmocmmgsdi_read_mask_e_type            mmocmmgsdi_get_bitmask_to_read_items
(
  mmoc_state_info_s_type          *mmoc_info_ptr,
  /* Pointer to MMoC's state information.
  */

  sys_modem_as_id_e_type           asubs_id
)
{
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  mmoc_state_info_dev_prop_s_type *mmoc_dev_info = NULL;
  mmoc_state_info_sub_prop_s_type *mmoc_sub_info[MAX_SIMS];
  mmoc_mmgsdi_info_s_type      *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr();

#if defined(FEATURE_MMSS_3_1) || defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900)
  mmocmmgsdi_error_e_type      mmocmmgsdi_status = MMOC_MMGSDI_READ_FAILED;
#endif

#ifdef FEATURE_MMSS_3_1
  mmgsdi_app_enum_type         *mmocmmgsdi_app_info_ptr = NULL;
  mmgsdi_session_id_type       *mmocmmgsdi_session_id_ptr = NULL;
#endif

#if defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900)
  mmgsdi_session_id_type       *mmocmmgsdi_1x_session_id_ptr = NULL;
  mmgsdi_app_enum_type         *mmocmmgsdi_1x_app_info_ptr = NULL;
#endif

  mmgsdi_session_type_enum_type pp_session_type = MMGSDI_GW_PROV_PRI_SESSION;
  mmgsdi_session_type_enum_type pp2_session_type = MMGSDI_1X_PROV_PRI_SESSION;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_ASSERT( mmoc_info_ptr != NULL );
  MMOC_ASSERT( mmgsdi_info_ptr != NULL );

  mmoc_dev_info = &mmoc_info_ptr->dev_prop;
  memscpy(&mmoc_sub_info[0],
          sizeof(mmoc_sub_info),
          &mmoc_info_ptr->sub_prop[0],
          sizeof(mmoc_info_ptr->sub_prop));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the session ID from session ID table */

  switch(asubs_id)
  {
    case SYS_MODEM_AS_ID_1:
      pp_session_type = MMGSDI_GW_PROV_PRI_SESSION;
      pp2_session_type = MMGSDI_1X_PROV_PRI_SESSION;
      break;

    case SYS_MODEM_AS_ID_2:
      pp_session_type = MMGSDI_GW_PROV_SEC_SESSION;
      pp2_session_type = MMGSDI_1X_PROV_SEC_SESSION;
      break;

#ifdef FEATURE_MMODE_TRIPLE_SIM
    case SYS_MODEM_AS_ID_3:
      pp_session_type = MMGSDI_GW_PROV_TER_SESSION;
      pp2_session_type = MMGSDI_1X_PROV_TER_SESSION;
      break;
#endif

    default:
      pp_session_type = MMGSDI_GW_PROV_PRI_SESSION;
      pp2_session_type = MMGSDI_1X_PROV_PRI_SESSION;
      break;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_MSG_HIGH_4("sub %d prot_subsc_chg %d sub_avail %d, nv_rtre_control() = %d",
                  asubs_id,
                  mmoc_sub_info[asubs_id]->prot_subsc_chg,
                  mmoc_sub_info[asubs_id]->sub_avail,
                  nv_rtre_control_ext(asubs_id));

  if(mmoc_sub_info[asubs_id]->prot_subsc_chg &&
      mmoc_sub_info[asubs_id]->sub_avail
    )
  {
    /* Set the Bit mask to read PRL, if
    **    i.   Subs changed is CDMA or G/W and
    **    ii.  Subsc is available for CDMA or G/W and
    **    iii. MMGSDI session is opened to Read PRL
    **    iv.  RTRE configuration is set to RUIM
    */
    if((mmocmmgsdi_is_session_opened(pp2_session_type)) &&
        (nv_rtre_control_ext(asubs_id) == NV_RTRE_CONTROL_USE_RUIM) &&
        mmoc_sub_info[asubs_id]->sub_avail & PROT_SUBSC_MASK_3GPP2)
    {
      if(mmoc_dev_info->sub_with_3gpp2 == asubs_id  &&
          mmoc_dev_info->subsc_chgd.sub_prop[asubs_id]->subs_capability & BM(SYS_SYS_MODE_CDMA))
      {
        mmgsdi_info_ptr->read_items[asubs_id] |= MMOC_MMGSDI_MASK_PRL;
      }
      else
      {
        MMOC_MSG_HIGH_2("MMSS:Sub not 1x capable, sub %d read_items = 0x%x",
                        asubs_id, mmgsdi_info_ptr->read_items[asubs_id]);
      }
    }
    else
    {
      MMOC_MSG_HIGH_2("MMSS:No PRL in RUIM, sub %d read_items = 0x%x",
                      asubs_id, mmgsdi_info_ptr->read_items[asubs_id]);
    }
  }
  else
  {
    mmgsdi_info_ptr->read_items[asubs_id] = MMOC_MMGSDI_MASK_NONE;
    mmgsdi_info_ptr->mmgsdi_items[asubs_id] = MMOC_MMGSDI_MASK_NONE;
    return (mmgsdi_info_ptr->read_items[asubs_id]);
  }

#if defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900)
  if(((sd_operator_e_type)sd_ss_get_operator_name(asubs_id)) == SD_OPERATOR_CT)
  {
    mmocmmgsdi_status = mmocmmgsdi_get_session_id_ptr(
                          (mmgsdi_session_type_enum_type) pp2_session_type,
                          &mmocmmgsdi_1x_session_id_ptr);

    if( (MMOC_MMGSDI_SUCCESS != mmocmmgsdi_status) || (mmocmmgsdi_1x_session_id_ptr == NULL))
    {
      MMOC_ERR_1("No 1x session type for sub %d", asubs_id);
      return (mmgsdi_info_ptr->read_items[asubs_id]);
    }

    mmocmmgsdi_status = mmocmmgsdi_session_get_app_info_ptr(
                          *mmocmmgsdi_1x_session_id_ptr,
                          &mmocmmgsdi_1x_app_info_ptr);

    if( (MMOC_MMGSDI_SUCCESS != mmocmmgsdi_status) || (mmocmmgsdi_1x_app_info_ptr == NULL))
    {
      MMOC_ERR_2("Failed to get app info: read_items = 0x%x, sub %d",
                 mmgsdi_info_ptr->read_items[asubs_id], asubs_id);
      return (mmgsdi_info_ptr->read_items[asubs_id]);
    }

    if(mmocmmgsdi_1x_app_info_ptr == NULL)
    {
      MMOC_ERR_0("Failed to get app info: mmocmmgsdi_1x_app_info_ptr is null");
      return (mmgsdi_info_ptr->read_items[asubs_id]);
    }

    if(*mmocmmgsdi_1x_app_info_ptr == MMGSDI_APP_CSIM)
    {
      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
      /* Check which record PLMN record to read from MMGSDI card for USIM
      **/
      mmgsdi_info_ptr->read_items[asubs_id] |= MMOC_MMGSDI_MASK_CSIM_IMSI;
    }
    else if(*mmocmmgsdi_1x_app_info_ptr == MMGSDI_APP_RUIM)
    {
      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
      /* Check which record PLMN record to read from MMGSDI card for USIM
      **/
      mmgsdi_info_ptr->read_items[asubs_id] |= MMOC_MMGSDI_MASK_RUIM_IMSI;
    }
    else
    {
      MMOC_ERR_0("CDMA IMSI: Failed to update read items: ");
    }
  }
#endif


#ifdef FEATURE_MMSS_3_1

  /* Get GW session and application information
  */

  mmocmmgsdi_status = mmocmmgsdi_get_session_id_ptr(
                        (mmgsdi_session_type_enum_type) pp_session_type,
                        &mmocmmgsdi_session_id_ptr);

  if( MMOC_MMGSDI_SUCCESS != mmocmmgsdi_status)
  {
    MMOC_ERR_1("No gw session type, sub %d", asubs_id);
    return (mmgsdi_info_ptr->read_items[asubs_id]);
  }

  mmocmmgsdi_status = mmocmmgsdi_session_get_app_info_ptr(
                        *mmocmmgsdi_session_id_ptr,
                        &mmocmmgsdi_app_info_ptr);

  if( MMOC_MMGSDI_SUCCESS != mmocmmgsdi_status)
  {
    MMOC_ERR_2("Failed to get app info: read_items = 0x%x, sub %d",
               mmgsdi_info_ptr->read_items[asubs_id], asubs_id);
    return (mmgsdi_info_ptr->read_items[asubs_id]);
  }

  /* Set the Bit mask to read MLPL/MSPL, if
  **    i.  Subsc is available for CDMA and G/W (even if no Subsc changed) &
  **    ii. MMGSDI session is opened to Read MLPL/MSPL
  **    iii. MMGSDI session is opened to Read plmn db
  **/
  if( !( (mmocmmgsdi_is_session_opened(MMGSDI_CARD_SESSION_SLOT_1) ||
          mmocmmgsdi_is_session_opened(MMGSDI_CARD_SESSION_SLOT_2)) &&
         mmocmmgsdi_is_session_opened(pp_session_type) &&
         mmoc_sub_info[asubs_id]->sub_avail & PROT_SUBSC_MASK_3GPP &&
         mmoc_sub_info[asubs_id]->sub_avail & PROT_SUBSC_MASK_3GPP2 &&
         mmoc_dev_info->sub_with_3gpp2 == asubs_id &&
         mmoc_dev_info->subsc_chgd.sub_prop[asubs_id]->subs_capability & BM(SYS_SYS_MODE_CDMA))
    )
  {
    MMOC_MSG_HIGH_2("EqPRl: legacy behavior, read_items = 0x%x, sub %d",
                    mmgsdi_info_ptr->read_items[asubs_id], asubs_id);
    mmgsdi_info_ptr->mmgsdi_items[asubs_id] = mmgsdi_info_ptr->read_items[asubs_id];
    return (mmgsdi_info_ptr->read_items[asubs_id]);
  }
  mmgsdi_info_ptr->read_items[asubs_id] |= (MMOC_MMGSDI_MASK_MLPL | MMOC_MMGSDI_MASK_MSPL);

  if (*mmocmmgsdi_app_info_ptr == MMGSDI_APP_SIM)
  {
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /* Check which record PLMN record to read from MMGSDI card for SIM
    **/
    mmgsdi_info_ptr->read_items[asubs_id] =
      mmgsdi_info_ptr->read_items[asubs_id]    |
      MMOC_MMGSDI_MASK_GSM_IMSI                |
      MMOC_MMGSDI_MASK_GSM_AD                  |
      MMOC_MMGSDI_MASK_GSM_PLMNWACT            |
      MMOC_MMGSDI_MASK_GSM_OPLMNWACT           |
      MMOC_MMGSDI_MASK_GSM_PLMN;
  }
  else if (*mmocmmgsdi_app_info_ptr == MMGSDI_APP_USIM)
  {
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /* Check which record PLMN record to read from MMGSDI card for USIM
    **/
    mmgsdi_info_ptr->read_items[asubs_id] =
      mmgsdi_info_ptr->read_items[asubs_id]    |
      MMOC_MMGSDI_MASK_USIM_EHPLMN             |
      MMOC_MMGSDI_MASK_USIM_PLMNWACT           |
      MMOC_MMGSDI_MASK_USIM_OPLMNWACT          |
      MMOC_MMGSDI_MASK_USIM_IMSI               |
      MMOC_MMGSDI_MASK_USIM_AD;
  }

  else
  {
    MMOC_ERR_1("MMSS:Failed to update read items, sub %d", asubs_id);
  }
#endif /* #ifdef FEATURE_MMSS_3_1 */

  mmgsdi_info_ptr->mmgsdi_items[asubs_id] = mmgsdi_info_ptr->read_items[asubs_id];

  MMOC_MSG_HIGH_3("read_items = 0x%x, mmgsdi_items = 0x%x, sub %d",
                  mmgsdi_info_ptr->read_items[asubs_id],
                  mmgsdi_info_ptr->mmgsdi_items[asubs_id],
                  asubs_id);

  return (mmgsdi_info_ptr->read_items[asubs_id]);

} /* mmocmmgsdi_get_bitmask_to_read_items() */

/*====================================================================

FUNCTION mmocmmgsdi_open_session

DESCRIPTION
  Internal function which calls mmgsdi_session_open for the specified
  session type, and generates an error if unsuccessful.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.
  Call after client registration with MMGSDI is complete, and MMOC
  has a valid client ID.

RETURN VALUE
  none

SIDE EFFECTS
  Request to open a session enqueued with MMGSDI.
  ERROR if unsuccessful.

=====================================================================*/

static void mmocmmgsdi_open_session
(

  mmgsdi_session_type_enum_type mmocmmgsdi_session_type
  /* Type of session to be opened
  */
)
{
  mmgsdi_return_enum_type        mmgsdi_status = MMGSDI_SUCCESS;
  mmoc_mmgsdi_info_s_type *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr ();

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Request to open a session for the session type using
  * the assigned client id.
  */
  mmgsdi_status = mmgsdi_session_open(mmgsdi_info_ptr->mmgsdi_client_id,
                                      mmocmmgsdi_session_type,
                                      0,
                                      mmocmmgsdi_card_status_cb,
                                      FALSE,
                                      mmocmmgsdi_session_open_cb,
                                      (mmgsdi_client_data_type) mmocmmgsdi_session_type);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMOC_ERR_3( "Session open Failed: Status=%d, Type=%d, Client_id=%d",
                mmgsdi_status, mmocmmgsdi_session_type,mmgsdi_info_ptr->mmgsdi_client_id);
    return;
  }
  MMOC_MSG_LOW_1("Opening session type :%d",mmocmmgsdi_session_type);

  return;

} /* mmocmmgsdi_open_session */


/*===========================================================================

FUNCTION mmocmmgsdi_session_read_prl

DESCRIPTION
  This is a function is used to read PRL through MMGSDI session.

RETURN VALUE
  MMOC_MMGSDI_SUCCESS         - PRL read request accepted by MMGSDI.
  MMOC_MMGSDI_PRL_READ_FAILED - prl read failed.

===========================================================================*/
mmocmmgsdi_error_e_type      mmocmmgsdi_session_read_prl
(
  mmoc_state_info_s_type          *mmoc_info_ptr,

  sys_modem_as_id_e_type           asubs_id

)
{
  mmgsdi_session_id_type     *mmocmmgsdi_session_id_ptr = NULL;
  mmocmmgsdi_error_e_type     mmocmmgsdi_status = MMOC_MMGSDI_PRL_READ_FAILED;
  mmgsdi_return_enum_type     mmgsdi_status = MMGSDI_MAX_RETURN_ENUM;
  mmgsdi_session_type_enum_type session_type = MMGSDI_GW_PROV_PRI_SESSION;
  mmoc_mmgsdi_info_s_type *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr ();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_ASSERT(mmoc_info_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the session ID from session ID table */

  switch(asubs_id)
  {
    case SYS_MODEM_AS_ID_1:
      session_type = MMGSDI_1X_PROV_PRI_SESSION;
      break;

    case SYS_MODEM_AS_ID_2:
      session_type = MMGSDI_1X_PROV_SEC_SESSION;
      break;

#ifdef FEATURE_MMODE_TRIPLE_SIM
    case SYS_MODEM_AS_ID_3:
      session_type = MMGSDI_1X_PROV_TER_SESSION;
      break;
#endif

    default:
      session_type = MMGSDI_1X_PROV_PRI_SESSION;
      break;
  }

  mmocmmgsdi_status = mmocmmgsdi_get_session_id_ptr(
                        session_type,
                        &mmocmmgsdi_session_id_ptr);

  if(MMOC_MMGSDI_SUCCESS != mmocmmgsdi_status)
  {
    MMOC_ERR_2("get_session Failed!!! mmocmmgsdi_status=%d, sub %d",
               mmocmmgsdi_status, asubs_id);

    return(mmocmmgsdi_status);
  }

  if(MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[asubs_id], MMOC_MMGSDI_MASK_PRL))
  {
    mmgsdi_info_ptr->read_items[asubs_id] =
      MMOC_BIT_DELETE(mmgsdi_info_ptr->read_items[asubs_id],
                      MMOC_MMGSDI_MASK_PRL);
  }
  else
  {
    return (MMOC_MMGSDI_READ_NONE);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Read PRL from the card through MMGSDI session api. */

  mmgsdi_status = mmgsdi_session_read_prl(
                    *mmocmmgsdi_session_id_ptr,
                    mmocmmgsdi_session_read_prl_cb,
                    (mmgsdi_client_data_type) asubs_id);

  if (mmgsdi_status == MMGSDI_SUCCESS)
  {
    mmoc_info_ptr->dev_prop.mmoc_mmgsdi_wait_info.file_enum = MMGSDI_CSIM_PRL;
    return (MMOC_MMGSDI_SUCCESS);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Read PRL using MMGSDI session api failed.
  */
  MMOC_ERR_1("mmgsdi_session_read_prl Failed!!! mmgsdi_status=%d",
             mmgsdi_status);

  return (MMOC_MMGSDI_PRL_READ_FAILED);

} /* mmocmmgsdi_session_read_prl */

/*===========================================================================

FUNCTION mmocmmgsdi_session_read_transparent

DESCRIPTION
  This is a function is used to read transparent records through MMGSDI session.

RETURN VALUE
  MMOC_MMGSDI_SUCCESS          - Read request accepted by MMGSDI.
  MMOC_MMGSDI_READ_FAILED      - Transparent read failed.

===========================================================================*/
mmocmmgsdi_error_e_type      mmocmmgsdi_session_read_transparent
(
  mmoc_state_info_s_type          *mmoc_info_ptr,
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
  mmgsdi_session_type_enum_type   session_type,
  /* Type of session used to retrieve data from the card data
  */
  mmgsdi_access_type              file_access_info,
  /* Access information to read data from the card
  */
  mmgsdi_callback_type            session_read_cb
)
{
  mmgsdi_session_id_type          *mmocmmgsdi_session_id_ptr = NULL;
  mmocmmgsdi_error_e_type         mmocmmgsdi_status = MMOC_MMGSDI_READ_FAILED;
  mmgsdi_return_enum_type         mmgsdi_status = MMGSDI_MAX_RETURN_ENUM;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_ASSERT(mmoc_info_ptr != NULL);
  MMOC_ASSERT(session_read_cb != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the session ID from session ID table */

  mmocmmgsdi_status = mmocmmgsdi_get_session_id_ptr(
                        (mmgsdi_session_type_enum_type) session_type,
                        &mmocmmgsdi_session_id_ptr);

  if(MMOC_MMGSDI_SUCCESS != mmocmmgsdi_status)
  {
    MMOC_ERR_1("get_session Failed!!! mmocmmgsdi_status=%d",
               mmocmmgsdi_status);
    return (mmocmmgsdi_status);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Read data using MMGSDI session api
  */
  mmoc_info_ptr->dev_prop.mmoc_mmgsdi_wait_info.file_enum = file_access_info.file.file_enum;
  mmgsdi_status = mmgsdi_session_read_transparent
                  (
                    *mmocmmgsdi_session_id_ptr,
                    file_access_info,
                    0, /* start from the beginning of file */
                    0, /* retrive the entire file */
                    session_read_cb,
                    (mmgsdi_client_data_type) mmoc_info_ptr
                  );

  if (MMGSDI_SUCCESS == mmgsdi_status)
  {
    return (MMOC_MMGSDI_SUCCESS);
  }
  else
  {
    /* Read data using MMGSDI session api failed.
    */
    MMOC_ERR_2("mmgsdi_session_read_transparent Failed! file_enum = %d, mmgsdi_status=%d",
               file_access_info.file.file_enum, mmgsdi_status);
    mmoc_info_ptr->dev_prop.mmoc_mmgsdi_wait_info.file_enum = MMGSDI_NO_FILE_ENUM;
    return (MMOC_MMGSDI_READ_FAILED);
  }

} /* mmocmmgsdi_session_read_transparent */

/*===========================================================================

FUNCTION mmocmmgsdi_session_read_mlpl_mspl

DESCRIPTION
  This is a function is used to read MLPL and MSPL records through MMGSDI
  session.

RETURN VALUE
  MMOC_MMGSDI_SUCCESS          - MLPL/MSPL read request accepted by MMGSDI.
  MMOC_MMGSDI_READ_FAILED      - MLPL/MSPL read failed.

===========================================================================*/
mmocmmgsdi_error_e_type      mmocmmgsdi_session_read_mlpl_mspl
(
  mmoc_state_info_s_type          *mmoc_info_ptr,

  sys_modem_as_id_e_type           asubs_id
)
{
  mmoc_mmgsdi_info_s_type *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr ();
  mmocmmgsdi_error_e_type  mmocmmgsdi_status = MMOC_MMGSDI_READ_FAILED;
  mmgsdi_access_type       file_access_info;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize access enum based on card mode. */

  file_access_info.access_method = MMGSDI_EF_ENUM_ACCESS;

#ifdef FEATURE_MMSS_3_1

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check which record (MLPL/MSPL)record to read from MMGSDI card */

  if(MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[asubs_id],
                         MMOC_MMGSDI_MASK_MLPL))
  {
    file_access_info.file.file_enum = MMGSDI_TELECOM_MLPL;

    mmgsdi_info_ptr->read_items[asubs_id] =
      MMOC_BIT_DELETE(mmgsdi_info_ptr->read_items[asubs_id], MMOC_MMGSDI_MASK_MLPL);
  }
  else if(MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[asubs_id],
                              MMOC_MMGSDI_MASK_MSPL)
         )
  {
    file_access_info.file.file_enum = MMGSDI_TELECOM_MSPL;
    mmgsdi_info_ptr->read_items[asubs_id] =
      MMOC_BIT_DELETE(mmgsdi_info_ptr->read_items[asubs_id], MMOC_MMGSDI_MASK_MSPL);
  }
  else
  {
    return (MMOC_MMGSDI_READ_NONE);
  }

#endif /* #ifdef FEATURE_MMSS_3_1 */


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Read the requested record from MMGSDI card
  **/
  {
    int i=0;
    while(i < MMOC_MAX_SESSION_IDS && mmgsdi_info_ptr->mmgsdi_session_id_table[i].asubs_id != asubs_id)
      i++;

    if(i<MMOC_MAX_SESSION_IDS)
    {
      MMOC_MSG_HIGH_1("Slot %d",mmgsdi_info_ptr->mmgsdi_session_id_table[i].slot_id);
      mmocmmgsdi_status = mmocmmgsdi_session_read_transparent
                          (
                            mmoc_info_ptr,
                            mmocmmgsdi_map_slot_id_to_card_session(mmgsdi_info_ptr->mmgsdi_session_id_table[i].slot_id),
                            file_access_info,
                            mmocmmgsdi_session_read_cb
                          );
    }
    else
    {
      MMOC_MSG_HIGH_1("asubs_id %d session entry not found!!!",asubs_id);
      mmocmmgsdi_status = MMOC_MMGSDI_NOTFOUND;
    }
  }

  if (MMOC_MMGSDI_SUCCESS == mmocmmgsdi_status)
  {
    return (mmocmmgsdi_status);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Read records failed. clear the read items bitmask so that no more
  ** items are read.
  */
  mmgsdi_info_ptr->read_items[asubs_id] = MMOC_MMGSDI_MASK_NONE;

  MMOC_ERR_2("Read mlpl_mspl Failed, sub %d read_items = 0x%x",
             asubs_id,
             mmgsdi_info_ptr->read_items[asubs_id]);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return (mmocmmgsdi_status);

} /* mmocmmgsdi_session_read_mlpl */

/*===========================================================================

FUNCTION mmocmmgsdi_session_read_card_imsi

DESCRIPTION
  This is a function is used to read card imsi through MMGSDI session.

RETURN VALUE
  MMOC_MMGSDI_SUCCESS          - card read accepted by MMGSDI.
  MMOC_MMGSDI_READ_FAILED      - card read failed.

===========================================================================*/
mmocmmgsdi_error_e_type      mmocmmgsdi_session_read_card_imsi
(
  mmoc_state_info_s_type          *mmoc_info_ptr,

  sys_modem_as_id_e_type           asubs_id
)
{
  mmoc_mmgsdi_info_s_type     *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr ();
  mmocmmgsdi_error_e_type      mmocmmgsdi_status = MMOC_MMGSDI_READ_FAILED;
  mmgsdi_access_type           file_access_info;
  mmgsdi_session_type_enum_type session_type = MMGSDI_GW_PROV_PRI_SESSION;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_ASSERT(mmoc_info_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the session ID from session ID table */

  switch(asubs_id)
  {
    case SYS_MODEM_AS_ID_1:
      session_type = MMGSDI_1X_PROV_PRI_SESSION;
      break;

    case SYS_MODEM_AS_ID_2:
      session_type = MMGSDI_1X_PROV_SEC_SESSION;
      break;

#ifdef FEATURE_MMODE_TRIPLE_SIM
    case SYS_MODEM_AS_ID_3:
      session_type = MMGSDI_1X_PROV_TER_SESSION;
      break;
#endif

    default:
      session_type = MMGSDI_1X_PROV_PRI_SESSION;
      break;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize access enum based on card mode.
  */
  file_access_info.access_method = MMGSDI_EF_ENUM_ACCESS;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check which IMSI value to read from MMGSDI card for SIM
  **/

  if ( MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[asubs_id],
                           MMOC_MMGSDI_MASK_CSIM_IMSI)
     )
  {
    file_access_info.file.file_enum = MMGSDI_CSIM_IMSI_M;
    mmgsdi_info_ptr->read_items[asubs_id] = MMOC_BIT_DELETE(mmgsdi_info_ptr->read_items[asubs_id],
                                            MMOC_MMGSDI_MASK_CSIM_IMSI);
  }
  else if ( MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[asubs_id],
                                MMOC_MMGSDI_MASK_RUIM_IMSI)
          )
  {
    file_access_info.file.file_enum = MMGSDI_CDMA_IMSI_M;
    mmgsdi_info_ptr->read_items[asubs_id] = MMOC_BIT_DELETE(mmgsdi_info_ptr->read_items[asubs_id],
                                            MMOC_MMGSDI_MASK_RUIM_IMSI);
  }
  else
  {
    return (MMOC_MMGSDI_READ_NONE);
  }
  MMOC_MSG_HIGH_1("mmocmmgsdi_session_read_card_imsi file enum = %d", file_access_info.file.file_enum);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Read the requested record from MMGSDI card
  **/
  mmocmmgsdi_status = mmocmmgsdi_session_read_transparent
                      (
                        mmoc_info_ptr,
                        session_type,
                        file_access_info,
                        (mmgsdi_callback_type) mmocmmgsdi_session_read_card_imsi_cb
                      );

  if (MMOC_MMGSDI_SUCCESS == mmocmmgsdi_status)
  {
    return (mmocmmgsdi_status);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Read card failed.
  */
  mmgsdi_info_ptr->read_items[asubs_id] = MMOC_MMGSDI_MASK_NONE;

  MMOC_ERR_2("read_plmn Failed, read_items = 0x%x, sub %d",
             mmgsdi_info_ptr->read_items[asubs_id],
             asubs_id);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return (mmocmmgsdi_status);

} /* mmocmmgsdi_session_read_card_imsi */

/*===========================================================================

FUNCTION mmocmmgsdi_session_read_plmn

DESCRIPTION
  This is a function is used to read PLMN records through MMGSDI session.

RETURN VALUE
  MMOC_MMGSDI_SUCCESS          - PLMN read request accepted by MMGSDI.
  MMOC_MMGSDI_READ_FAILED      - PLMN read failed.

===========================================================================*/
mmocmmgsdi_error_e_type      mmocmmgsdi_session_read_plmn
(
  mmoc_state_info_s_type          *mmoc_info_ptr,

  sys_modem_as_id_e_type           asubs_id
)
{
  mmoc_mmgsdi_info_s_type     *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr ();
  mmocmmgsdi_error_e_type      mmocmmgsdi_status = MMOC_MMGSDI_READ_FAILED;
  mmgsdi_access_type           file_access_info;
  mmgsdi_session_type_enum_type session_type = MMGSDI_GW_PROV_PRI_SESSION;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_ASSERT(mmoc_info_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the session ID from session ID table */

  switch(asubs_id)
  {
    case SYS_MODEM_AS_ID_1:
      session_type = MMGSDI_GW_PROV_PRI_SESSION;
      break;

    case SYS_MODEM_AS_ID_2:
      session_type = MMGSDI_GW_PROV_SEC_SESSION;
      break;

#ifdef FEATURE_MMODE_TRIPLE_SIM
    case SYS_MODEM_AS_ID_3:
      session_type = MMGSDI_GW_PROV_TER_SESSION;
      break;
#endif

    default:
      session_type = MMGSDI_GW_PROV_PRI_SESSION;
      break;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize access enum based on card mode. */

  file_access_info.access_method = MMGSDI_EF_ENUM_ACCESS;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check which record PLMN record to read from MMGSDI card for SIM */

  if ( MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[asubs_id],
                           MMOC_MMGSDI_MASK_GSM_IMSI))
  {
    file_access_info.file.file_enum = MMGSDI_GSM_IMSI;
    mmgsdi_info_ptr->read_items[asubs_id] = MMOC_BIT_DELETE(mmgsdi_info_ptr->read_items[asubs_id],
                                            MMOC_MMGSDI_MASK_GSM_IMSI);
  }
  else if ( MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[asubs_id],
                                MMOC_MMGSDI_MASK_GSM_AD)
          )
  {
    file_access_info.file.file_enum = MMGSDI_GSM_AD;
    mmgsdi_info_ptr->read_items[asubs_id] = MMOC_BIT_DELETE(mmgsdi_info_ptr->read_items[asubs_id],
                                            MMOC_MMGSDI_MASK_GSM_AD);
  }
  else if ( MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[asubs_id],
                                MMOC_MMGSDI_MASK_GSM_PLMNWACT)
          )
  {
    file_access_info.file.file_enum = MMGSDI_GSM_PLMNWACT;
    mmgsdi_info_ptr->read_items[asubs_id] = MMOC_BIT_DELETE(mmgsdi_info_ptr->read_items[asubs_id],
                                            MMOC_MMGSDI_MASK_GSM_PLMNWACT);
  }
  else if ( MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[asubs_id],
                                MMOC_MMGSDI_MASK_GSM_OPLMNWACT)
          )
  {
    file_access_info.file.file_enum = MMGSDI_GSM_OPLMNWACT;
    mmgsdi_info_ptr->read_items[asubs_id] = MMOC_BIT_DELETE(mmgsdi_info_ptr->read_items[asubs_id],
                                            MMOC_MMGSDI_MASK_GSM_OPLMNWACT );
  }
  else if ( MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[asubs_id],
                                MMOC_MMGSDI_MASK_GSM_PLMN)
          )
  {
    file_access_info.file.file_enum = MMGSDI_GSM_PLMN;
    mmgsdi_info_ptr->read_items[asubs_id] = MMOC_BIT_DELETE(mmgsdi_info_ptr->read_items[asubs_id],
                                            MMOC_MMGSDI_MASK_GSM_PLMN);
  }
  else if ( MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[asubs_id],
                                MMOC_MMGSDI_MASK_USIM_EHPLMN)
          )
  {
    file_access_info.file.file_enum = MMGSDI_USIM_EHPLMN;

    mmgsdi_info_ptr->read_items[asubs_id] = MMOC_BIT_DELETE(mmgsdi_info_ptr->read_items[asubs_id],
                                            MMOC_MMGSDI_MASK_USIM_EHPLMN);
  }
  else if ( MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[asubs_id],
                                MMOC_MMGSDI_MASK_USIM_PLMNWACT)
          )
  {
    file_access_info.file.file_enum = MMGSDI_USIM_PLMNWACT;
    mmgsdi_info_ptr->read_items[asubs_id] = MMOC_BIT_DELETE(mmgsdi_info_ptr->read_items[asubs_id],
                                            MMOC_MMGSDI_MASK_USIM_PLMNWACT);
  }
  else if ( MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[asubs_id],
                                MMOC_MMGSDI_MASK_USIM_OPLMNWACT)
          )
  {
    file_access_info.file.file_enum = MMGSDI_USIM_OPLMNWACT;
    mmgsdi_info_ptr->read_items[asubs_id] = MMOC_BIT_DELETE(mmgsdi_info_ptr->read_items[asubs_id],
                                            MMOC_MMGSDI_MASK_USIM_OPLMNWACT );
  }
  else if (MMOC_IS_BIT_CONTAIN (mmgsdi_info_ptr->read_items[asubs_id],
                                MMOC_MMGSDI_MASK_USIM_IMSI)
          )
  {
    file_access_info.file.file_enum = MMGSDI_USIM_IMSI;
    mmgsdi_info_ptr->read_items[asubs_id] = MMOC_BIT_DELETE(mmgsdi_info_ptr->read_items[asubs_id],
                                            MMOC_MMGSDI_MASK_USIM_IMSI);
  }
  else if ( MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[asubs_id],
                                MMOC_MMGSDI_MASK_USIM_AD)
          )
  {
    file_access_info.file.file_enum = MMGSDI_USIM_AD;
    mmgsdi_info_ptr->read_items[asubs_id] = MMOC_BIT_DELETE(mmgsdi_info_ptr->read_items[asubs_id],
                                            MMOC_MMGSDI_MASK_USIM_AD);
  }
  else
  {
    return (MMOC_MMGSDI_READ_NONE);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Read the requested record from MMGSDI card */

  mmocmmgsdi_status = mmocmmgsdi_session_read_transparent
                      (
                        mmoc_info_ptr,
                        session_type,
                        file_access_info,
                        (mmgsdi_callback_type) mmocmmgsdi_session_read_cb
                      );

  if (MMOC_MMGSDI_SUCCESS == mmocmmgsdi_status)
  {
    return (mmocmmgsdi_status);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Read records failed. */

  mmgsdi_info_ptr->read_items[asubs_id] = MMOC_MMGSDI_MASK_NONE;

  MMOC_ERR_2("read_plmn Failed, sub %d read_items = 0x%x",
             asubs_id,
             mmgsdi_info_ptr->read_items[asubs_id]);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return (mmocmmgsdi_status);

} /* mmocmmgsdi_session_read_plmn */



/*====================================================================

FUNCTION mmocmmgsdi_session_read_card_imsi_cb

DESCRIPTION
  Callback function for session based MMGSDI interface.  Called by MMGSDI
  in response to call to mmgsdi_session_read_card_imsi() function.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

=====================================================================*/
void   mmocmmgsdi_session_read_card_imsi_cb
(
  mmgsdi_return_enum_type       status,
  mmgsdi_cnf_enum_type          cnf,
  const mmgsdi_cnf_type        *cnf_ptr
)
{

  mmoc_rpt_msg_s_type            *msg_ptr;
  mm_sub_stk_id_s_type mm_id;
  mmgsdi_session_type_enum_type session_type;

  mmocmmgsdi_get_session_type(cnf_ptr->response_header.session_id, &session_type);
  mm_id = mmocmmgsdi_get_mm_id_per_session_type(session_type);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_ASSERT(cnf_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (MMGSDI_SUCCESS != status || MMGSDI_READ_CNF !=  cnf)
  {
    MMOC_ERR_2("MMGSDI read card imsi failed. Status %d, Confirmation %d",
               status, cnf);
  }


#if defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900)
  else
  {
    /* Call the sd function here to update cdma_imsi value */
    sd_misc_update_card_imsi(mm_id.asubs_id,
                             cnf_ptr->read_cnf.read_data.data_ptr,
                             (uint32) cnf_ptr->read_cnf.read_data.data_len);
  }
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Queue a report to clear the transaction when waiting for session open cnf. */

  MMOC_MSG_HIGH_0("MMGSDI sending MMOC_RPT_MMGSDI_CNF ");

  /* Get the report buffer.
  **/
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->rpt.name                   = MMOC_RPT_MMGSDI_CNF;
  msg_ptr->param.mmgsdi_cnf.status    = status;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );



} /* mmocmmgsdi_session_read_card_imsi_cb */

/*===========================================================================
FUNCTION MMOCMMGSDI_SESSION_CLIENT_REG_CB

DESCRIPTION
  Callback function for session based MMGSDI interface.  Called by MMGSDI
  to acknowledge MMOC client registration and event registration.  Client ID
  provided in this callback.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  None

SIDE EFFECTS
  Error Fatal if registration unsuccessful
  Client ID is stored

===========================================================================*/
void mmocmmgsdi_session_client_id_reg_cb
(
  mmgsdi_return_enum_type      status,
  mmgsdi_cnf_enum_type         cnf,
  const mmgsdi_cnf_type        *cnf_ptr
)
{
  mmoc_mmgsdi_info_s_type    *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr ();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_ASSERT(cnf_ptr != NULL);
  MMOC_ASSERT((mmoc_state_info_s_type *) (cnf_ptr->client_id_and_evt_reg_cnf.
                                          response_header.client_data) != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ((status == MMGSDI_SUCCESS)                 &&
      (cnf == MMGSDI_CLIENT_ID_AND_EVT_REG_CNF)  &&
      (cnf_ptr != NULL))
  {
    /* Store the client id assigned for mmoc */
    mmocmmgsdi_store_client_id(cnf_ptr);
  }
  else
  {
    MMOC_ERR_3("mmocmmgsdi_session_client_id_reg_cb Failed!!! client_id %d,status %d,cnf %d",
               mmgsdi_info_ptr->mmgsdi_client_id, status, cnf);
  }

  return;
} /* mmocmmgsdi_session_client_id_reg_cb */

/*===========================================================================

FUNCTION MMOCMMGSDI_CARD_STATUS_CB

DESCRIPTION
  Callback function for MMGSDI generated event.

===========================================================================*/
void mmocmmgsdi_card_status_cb
(
  const mmgsdi_event_data_type *event
)
{
  mmocmmgsdi_session_id_table_entry_s_type          *mmocmmgsdi_tbl_entry_ptr = NULL;
  mmgsdi_session_type_enum_type  mmocmmgsdi_sess_type = MMGSDI_MAX_SESSION_TYPE_ENUM;
  mmoc_mmgsdi_info_s_type       *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr();
  mm_sub_stk_id_s_type mm_id;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_ASSERT(event != NULL);

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(event->evt != MMGSDI_CARD_INSERTED_EVT &&
     event->evt != MMGSDI_SIM_BUSY_EVT &&
     event->evt != MMGSDI_CARD_ERROR_EVT)
  {
    if(MMOC_MMGSDI_SUCCESS != mmocmmgsdi_get_session_type(
          event->session_id,&mmocmmgsdi_sess_type))
    {
      MMOC_MSG_HIGH_2("Invalid session type and id combination:Sess type=%d, Sess Id=%d",\
                      mmocmmgsdi_sess_type,event->session_id);
      return;
    }

    mm_id = mmocmmgsdi_get_mm_id_per_session_type(mmocmmgsdi_sess_type);

    if((mmocmmgsdi_sess_type == MMGSDI_MAX_SESSION_TYPE_ENUM) || (mm_id.asubs_id == SYS_MODEM_AS_ID_NONE))
    {
      MMOC_MSG_HIGH_3("Invalid session, sess_type=%d, sub %d, session_id=%d",
                      mmocmmgsdi_sess_type,
                      mm_id.asubs_id,
                      event->session_id);

      return;
    }
  }

  switch (event->evt)
  {
    case MMGSDI_SUBSCRIPTION_READY_EVT:
    {
      mmgsdi_slot_id_enum_type slot_id = event->data.subscription_ready.app_info.slot;
        
      /* Subscription is available.Update the app type for that session
      **/
      MMOC_MSG_HIGH_3("MMGSDI_SUBSCRIPTION_READY_EVT, session_id=%d, slot=%d, app_type=%d",
                          event->session_id,
                          event->data.subscription_ready.app_info.slot,
                          event->data.subscription_ready.app_info.app_data.app_type);

      if(slot_id != MMGSDI_SLOT_1 &&
         slot_id != MMGSDI_SLOT_2)
      {
        slot_id = MMGSDI_SLOT_1;
      }

      /* Reset SIM removed status for this sub
      */
      if(MMOC_MMGSDI_SUCCESS == mmocmmgsdi_slot_get_tbl_entry_ptr(slot_id, &mmocmmgsdi_tbl_entry_ptr))
      {
        if(mmocmmgsdi_tbl_entry_ptr->asubs_id < MAX_SIMS &&
           mmgsdi_info_ptr->sim_available_status[mmocmmgsdi_tbl_entry_ptr->asubs_id] == FALSE)
        {
          MMOC_MSG_HIGH_1("SUBSC_READY: Set sim_available_status on sub %d", mmocmmgsdi_tbl_entry_ptr->asubs_id);
          mmgsdi_info_ptr->sim_available_status[mmocmmgsdi_tbl_entry_ptr->asubs_id] = TRUE;
        }
      }

      if (MMOC_MMGSDI_SUCCESS == mmocmmgsdi_session_get_tbl_entry_ptr(
            event->session_id,
            &mmocmmgsdi_tbl_entry_ptr))
      {
        mmocmmgsdi_tbl_entry_ptr->app_info = *(&(event->data.subscription_ready.app_info.app_data.app_type));
        mmocmmgsdi_tbl_entry_ptr->slot_id = event->data.subscription_ready.app_info.slot;

        /* Update GW session info to SD so that it is used to
        ** check if LTE needs to be disabled.
        */
        MMOC_MSG_HIGH_3("asubs_id %d, Updating SD with app_type=%d slot %d",
                        mm_id.asubs_id,
                        mmocmmgsdi_tbl_entry_ptr->app_info,
                        mmocmmgsdi_tbl_entry_ptr->slot_id );
        if( (mmocmmgsdi_tbl_entry_ptr->app_info == MMGSDI_APP_SIM ||
             mmocmmgsdi_tbl_entry_ptr->app_info == MMGSDI_APP_USIM)
            &&
            (mmocmmgsdi_sess_type == MMGSDI_GW_PROV_PRI_SESSION ||
             mmocmmgsdi_sess_type == MMGSDI_GW_PROV_SEC_SESSION
#ifdef FEATURE_MMODE_TRIPLE_SIM
             || mmocmmgsdi_sess_type == MMGSDI_GW_PROV_TER_SESSION
#endif
            )
          )
        {
          sd_ss_misc_update_session_info (mmocmmgsdi_tbl_entry_ptr->app_info, mm_id.asubs_id );
        }
      }
      else
      {
        MMOC_ERR_1("Session ID not found: %d", event->session_id);
      }

#ifdef FEATURE_MMODE_SC_SVLTE
      MMOC_MSG_HIGH_2("MMGSDI_SUBSCRIPTION_READY_EVT: mmocmmgsdi_session_type %d, slot %d",
                          mmocmmgsdi_sess_type,
                          event->data.subscription_ready.app_info.slot);
      if (mmocmmgsdi_sess_type == MMGSDI_GW_PROV_PRI_SESSION ||
          mmocmmgsdi_sess_type == MMGSDI_1X_PROV_PRI_SESSION )
      {
        switch(event->data.subscription_ready.app_info.slot)
        {
          case MMGSDI_SLOT_1:
            mmoc_get_state_info_ptr()->dev_prop.pri_slot = MMGSDI_CARD_SESSION_SLOT_1;
            break;
          case MMGSDI_SLOT_2:
            mmoc_get_state_info_ptr()->dev_prop.pri_slot = MMGSDI_CARD_SESSION_SLOT_2;
            break;
          default:
            break;
        }
      }
#endif
    }
      break;

    case MMGSDI_SESSION_CHANGED_EVT:

      /* One of the sessions got changed. Update the app type for that session
      **/
      MMOC_MSG_HIGH_3("MMGSDI_SESSION_CHANGED_EVT, session_id=%d, slot=%d, app_type=%d",
                          event->session_id,
                          event->data.session_changed.app_info.slot,
                          event->data.session_changed.app_info.app_data.app_type);

      if (MMOC_MMGSDI_SUCCESS == mmocmmgsdi_session_get_tbl_entry_ptr(
            event->session_id,
            &mmocmmgsdi_tbl_entry_ptr))
      {
        mmocmmgsdi_tbl_entry_ptr->app_info = *(&(event->data.session_changed.app_info.app_data.app_type));
        mmocmmgsdi_tbl_entry_ptr->slot_id = event->data.session_changed.app_info.slot;

        /* Update GW session info to SD so that it is used to
        ** check if LTE needs to be disabled.
        */

        MMOC_MSG_HIGH_1("Updating SD with app_type=%d", mmocmmgsdi_tbl_entry_ptr->app_info);
        if(( mmocmmgsdi_tbl_entry_ptr->app_info == MMGSDI_APP_SIM ||
             mmocmmgsdi_tbl_entry_ptr->app_info == MMGSDI_APP_USIM)
            &&
            (mmocmmgsdi_sess_type == MMGSDI_GW_PROV_PRI_SESSION ||
             mmocmmgsdi_sess_type == MMGSDI_GW_PROV_SEC_SESSION
#ifdef FEATURE_MMODE_TRIPLE_SIM
             || mmocmmgsdi_sess_type == MMGSDI_GW_PROV_TER_SESSION
#endif
             )
          )
        {
          mm_id.stk_id = MM_STACK_0;
          sd_ss_misc_update_session_info (mmocmmgsdi_tbl_entry_ptr->app_info, mm_id.asubs_id );
        }
      }
      else
      {
        MMOC_ERR_1("Session ID not found: %d", event->session_id);
      }

      break;

    case MMGSDI_CARD_INSERTED_EVT:
    {
      mmgsdi_slot_id_enum_type slot_id = event->data.card_inserted.slot;
      
      /* Queue Command to MMOC to open/re-open the session */
      MMOC_MSG_HIGH_3("MMGSDI_CARD_INSERTED_EVT, session_id=%d, client_id=%d, slot=%d",
                          event->session_id,
                          event->client_id,
                          event->data.card_inserted.slot); /* Device level event, sub value passed is 0 */

      if(slot_id != MMGSDI_SLOT_1 &&
         slot_id != MMGSDI_SLOT_2)
      {
        slot_id = MMGSDI_SLOT_1;
      }

      /* Reset SIM removed status for this sub
      */
      if(MMOC_MMGSDI_SUCCESS == mmocmmgsdi_slot_get_tbl_entry_ptr(slot_id, &mmocmmgsdi_tbl_entry_ptr))
      {
        if(mmocmmgsdi_tbl_entry_ptr->asubs_id < MAX_SIMS &&
           mmgsdi_info_ptr->sim_available_status[mmocmmgsdi_tbl_entry_ptr->asubs_id] == FALSE)
        {
          MMOC_MSG_HIGH_1("CARD_INSERTED: Set sim_available_status on sub %d", mmocmmgsdi_tbl_entry_ptr->asubs_id);
          mmgsdi_info_ptr->sim_available_status[mmocmmgsdi_tbl_entry_ptr->asubs_id] = TRUE;
        }
      }
      
      mmoc_cmd_mmgsdi_info_ind();
    }
      break;

    case MMGSDI_SESSION_CLOSE_EVT:

      /* One of the sessions got Closed. Update the app type for that session
      **/
      MMOC_MSG_HIGH_2("MMGSDI_SESSION_CLOSE_EVT, session_id=%d, slot=%d ",
                      event->data.session_close.session_id,
                      event->data.session_close.slot);

      if (MMOC_MMGSDI_SUCCESS == mmocmmgsdi_session_get_tbl_entry_ptr(
            event->session_id,
            &mmocmmgsdi_tbl_entry_ptr))
      {

        /* Update GW session is closed.Reset SD session GW  info
        ** So that SD can fall back to limited service searching
        ** on LTE rat.
        */
        MMOC_MSG_HIGH_1("Updating SD with app_type=%d", mmocmmgsdi_tbl_entry_ptr->app_info);

        if ((mmocmmgsdi_tbl_entry_ptr->app_info == MMGSDI_APP_SIM ||
             mmocmmgsdi_tbl_entry_ptr->app_info == MMGSDI_APP_USIM)
            &&
            (mmocmmgsdi_sess_type == MMGSDI_GW_PROV_PRI_SESSION ||
             mmocmmgsdi_sess_type == MMGSDI_GW_PROV_SEC_SESSION
#ifdef FEATURE_MMODE_TRIPLE_SIM
             || mmocmmgsdi_sess_type == MMGSDI_GW_PROV_TER_SESSION
#endif
             )
           )
        {
          mm_id.stk_id = MM_STACK_0;
          sd_ss_misc_update_session_info (MMGSDI_APP_NONE, mm_id.asubs_id );
        }

        mmocmmgsdi_tbl_entry_ptr->app_info = MMGSDI_APP_NONE;
      }
      else
      {
        MMOC_ERR_1("Session ID not found: %d", event->session_id);
      }

      /* Update the session_opened variable to FALSE
      **/
      mmocmmgsdi_session_closed (event->session_id);

      break;

    case MMGSDI_SIM_BUSY_EVT:
    {
      mmgsdi_session_type_enum_type  session_slot = MMGSDI_CARD_SESSION_SLOT_1;

      MMOC_MSG_HIGH_3("SIMBUSY: slot:%d, sim_busy:%d, PRI_SLOT:%d",
                      event->data.sim_busy.slot,
                      event->data.sim_busy.sim_busy,
                      mmoc_get_state_info_ptr()->dev_prop.pri_slot);

      switch(event->data.sim_busy.slot)
      {
        case MMGSDI_SLOT_1:
          session_slot = MMGSDI_CARD_SESSION_SLOT_1;
          break;
        case MMGSDI_SLOT_2:
          session_slot = MMGSDI_CARD_SESSION_SLOT_2;
          break;
        default:
          break;
      }

      if (MMOC_MMGSDI_SUCCESS == mmocmmgsdi_slot_get_tbl_entry_ptr(
                                            event->data.sim_busy.slot,
                                            &mmocmmgsdi_tbl_entry_ptr))
      {
        if(mmocmmgsdi_tbl_entry_ptr->asubs_id < MAX_SIMS)
        {
          mmgsdi_info_ptr->sim_busy_status[mmocmmgsdi_tbl_entry_ptr->asubs_id] = event->data.sim_busy.sim_busy;

          /* Set the REX signal corresponding to MMoC's report queue and return.
          */
          (void) rex_set_sigs(get_mmoc_tcb(), MMOC_MMGSDI_ABORT_TIMER_SIG);
        }
      }
    }
    break;

    case MMGSDI_CARD_ERROR_EVT:
    {
      mmgsdi_slot_id_enum_type slot_id = event->data.card_error.slot;

      MMOC_MSG_HIGH_1("CARD_ERROR: slot %d", slot_id);

      if(slot_id != MMGSDI_SLOT_1 &&
         slot_id != MMGSDI_SLOT_2)
      {
        slot_id = MMGSDI_SLOT_1;
      }

      /* Set SIM removed status for this sub
      */
      if(MMOC_MMGSDI_SUCCESS == mmocmmgsdi_slot_get_tbl_entry_ptr(slot_id, &mmocmmgsdi_tbl_entry_ptr))
      {
        if(mmocmmgsdi_tbl_entry_ptr->asubs_id < MAX_SIMS &&
           mmgsdi_info_ptr->sim_available_status[mmocmmgsdi_tbl_entry_ptr->asubs_id] == TRUE)
        {
          MMOC_MSG_HIGH_1("CARD_ERROR: Reset sim_available_status on sub %d", mmocmmgsdi_tbl_entry_ptr->asubs_id);
          mmgsdi_info_ptr->sim_available_status[mmocmmgsdi_tbl_entry_ptr->asubs_id] = FALSE;
        }
      }
    }
      break;
      
    default:
      break;
  } /* switch */

}  /* mmocmmgsdi_card_status_cb() */


/*====================================================================

FUNCTION mmocmmgsdi_session_open_cb

DESCRIPTION
  Callback function for session based MMGSDI interface.  Called by MMGSDI
  in response to call to mmgsdi_open_session() function.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  none

SIDE EFFECTS
  none

=====================================================================*/
void   mmocmmgsdi_session_open_cb
(
  mmgsdi_return_enum_type      status,
  mmgsdi_cnf_enum_type         cnf,
  const mmgsdi_cnf_type        *cnf_ptr
)
{
  mmgsdi_session_id_type        *mmocmmgsdi_session_id_ptr = NULL;
  mmgsdi_app_enum_type          *mmocmmgsdi_app_info_ptr = NULL;
  mmocmmgsdi_error_e_type       mmocmmgsdi_status = MMOC_MMGSDI_NOTFOUND;
  mmgsdi_session_type_enum_type mmocmmgsdi_session_type;
  mmoc_mmgsdi_info_s_type       *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr ();
  unsigned int                  index = 0;
  mmoc_rpt_msg_s_type           *msg_ptr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_ASSERT(cnf_ptr != NULL);
  //MMOC_ASSERT(((void *) cnf_ptr->session_open_cnf.response_header.client_data) != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mmocmmgsdi_session_type = (mmgsdi_session_type_enum_type) (cnf_ptr->
                            session_open_cnf.response_header.client_data);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (MMGSDI_SUCCESS != status || MMGSDI_SESSION_OPEN_CNF !=  cnf)
  {
    MMOC_MSG_MED_3("Session open Failed, Status = %d, Cnf = %d, Type = %d",
                   status, cnf, mmocmmgsdi_session_type);
    return;
  }

  MMOC_MSG_HIGH_3("MMGSDI Session opened, App Type = %d, Session Type = %d Session Id = %d ",
                  cnf_ptr->session_open_cnf.app_info.app_type,
                  mmocmmgsdi_session_type, cnf_ptr->session_open_cnf.session_id);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the session id ptr from the session ID table */
  mmocmmgsdi_status = mmocmmgsdi_get_session_id_ptr(
                        (mmgsdi_session_type_enum_type) mmocmmgsdi_session_type,
                        &mmocmmgsdi_session_id_ptr);

  if( MMOC_MMGSDI_SUCCESS != mmocmmgsdi_status)
  {
    MMOC_MSG_MED_3("MMGSDI fail session open unknown session type %d, session id %d, app type %d",
                   cnf_ptr->session_open_cnf.response_header.client_data,
                   cnf_ptr->session_open_cnf.response_header.session_id,
                   cnf_ptr->session_open_cnf.app_info.app_type);
    return;
  }

  /* Copy the opened session id to the pointer entry
   *  in the session id table.
   */
  *mmocmmgsdi_session_id_ptr = *(&(cnf_ptr->session_open_cnf.response_header.session_id));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Copy the app type to the session id table.
   */
  if( MMOC_MMGSDI_SUCCESS == mmocmmgsdi_session_get_app_info_ptr(
        cnf_ptr->session_open_cnf.response_header.session_id,
        &mmocmmgsdi_app_info_ptr))
  {
    *mmocmmgsdi_app_info_ptr = *(&(cnf_ptr->session_open_cnf.app_info.app_type));
  }
  else
  {
    MMOC_ERR_1("Session ID not found to save App type: %d",
               cnf_ptr->session_open_cnf.response_header.session_id);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Set the session_opened variable.
   */
  if( MMOC_MMGSDI_SUCCESS == mmocmmgsdi_get_session_index(
        (mmgsdi_session_type_enum_type) mmocmmgsdi_session_type,
        &index))
  {
    mmgsdi_info_ptr->mmgsdi_session_id_table[index].session_opened = TRUE;
  }
  else
  {
    MMOC_ERR_1("Session Index not found for session type: %d",
               mmocmmgsdi_session_type);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Queue a report to clear the transaction when waiting for session open cnf.
  */

  /* Get the report buffer.
  **/
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  */
  msg_ptr->rpt.name                   = MMOC_RPT_MMGSDI_CNF;
  msg_ptr->param.mmgsdi_cnf.status    = status;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );

  return;

} /* mmocmmgsdi_session_open_cb */


/*====================================================================

FUNCTION mmocmmgsdi_session_read_prl_cb

DESCRIPTION
  Callback function for session based MMGSDI interface.  Called by MMGSDI
  in response to call to mmgsdi_session_read_prl() function.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  none

SIDE EFFECTS
  none

=====================================================================*/
void   mmocmmgsdi_session_read_prl_cb
(
  mmgsdi_return_enum_type       status,
  mmgsdi_cnf_enum_type          cnf,
  const mmgsdi_cnf_type        *cnf_ptr
)
{
  mmoc_rpt_msg_s_type           *msg_ptr;
  mmoc_mmgsdi_info_s_type       *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr();
  const sys_modem_as_id_e_type        asubs_id = (sys_modem_as_id_e_type)cnf_ptr->response_header.client_data;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_ASSERT(cnf_ptr != NULL);

  MMOC_MSG_HIGH_5("PRL Read: Status %d, Confirmation %d, valid %d, size %d, prl_version %d",
                  status, cnf,
                  cnf_ptr->session_read_prl_cnf.valid,
                  cnf_ptr->session_read_prl_cnf.size,
                  cnf_ptr->session_read_prl_cnf.prl_version);

  if(mmoc_get_state_info_ptr()->dev_prop.sub_with_3gpp2 != asubs_id)
  {
    MMOC_MSG_HIGH_2("PRL sub_id %d not same as 3gpp2 sub id %d",asubs_id,mmoc_get_state_info_ptr()->dev_prop.sub_with_3gpp2);
  }

  mmoc_get_state_info_ptr()->dev_prop.mmoc_mmgsdi_wait_info.file_enum = MMGSDI_NO_FILE_ENUM;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(MMGSDI_SIM_BUSY == status)
  {
    mmoc_send_cm_rpt( CM_MMOC_SUBSCRIPTION_FAILURE, NULL, asubs_id);
    mmgsdi_info_ptr->read_items[asubs_id]   = MMOC_MMGSDI_MASK_NONE;
    mmgsdi_info_ptr->mmgsdi_items[asubs_id] = MMOC_MMGSDI_MASK_NONE;
  }
  else if (MMGSDI_SUCCESS == status && MMGSDI_SESSION_READ_PRL_CNF == cnf)
  {
    if ((cnf_ptr->session_read_prl_cnf.valid) &&
        (cnf_ptr->session_read_prl_cnf.size) &&
        (cnf_ptr->session_read_prl_cnf.roaming_list_ptr != NULL))
    {

      /* Call in to SD lib to update the prl read through MMGSDI to SD buffer
      **/

      prl_update_from_mmgsdi (cnf_ptr->session_read_prl_cnf.size,
                              cnf_ptr->session_read_prl_cnf.prl_version,
                              cnf_ptr->session_read_prl_cnf.roaming_list_ptr,
                              asubs_id
                             );
    }

  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the report buffer. */

  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields. */

  msg_ptr->rpt.name                   = MMOC_RPT_MMGSDI_CNF;
  msg_ptr->param.mmgsdi_cnf.status    = status;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the report. */

  mmoc_send_rpt( msg_ptr );

} /* mmocmmgsdi_session_read_prl_cb */

/*====================================================================

FUNCTION mmocmmgsdi_session_read_cb

DESCRIPTION
  Callback function for session based MMGSDI interface.  Called by MMGSDI
  in response to call to mmgsdi_session_read_transparent() function.

  Check if MMGSDI read failed due to SIM-BUSY ,
  if yes - notify CM CM_MMOC_SUBSCRIPTION_FAILURE ,
  so that it can recover after SIM is ready.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

=====================================================================*/
void   mmocmmgsdi_session_read_cb
(
  mmgsdi_return_enum_type       status,
  mmgsdi_cnf_enum_type          cnf,
  const mmgsdi_cnf_type        *cnf_ptr
)
{
  mmoc_rpt_msg_s_type      *msg_ptr;
  mmoc_state_info_s_type   *mmoc_info_ptr = mmoc_get_state_info_ptr();
  mmoc_mmgsdi_info_s_type  *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr();
  mm_sub_stk_id_s_type      mm_id;
  mmgsdi_session_type_enum_type session_type;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_ASSERT(cnf_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mmocmmgsdi_get_session_type(cnf_ptr->response_header.session_id, &session_type);
  mm_id = mmocmmgsdi_get_mm_id_per_session_type(session_type);

  if(mmoc_info_ptr->dev_prop.mmoc_mmgsdi_wait_info.file_enum !=
      cnf_ptr->read_cnf.access.file.file_enum)
  {
    MMOC_MSG_MED_1("MMOC waiting for %d",mmoc_info_ptr->dev_prop.mmoc_mmgsdi_wait_info.file_enum);
    return;
  }

  mmoc_info_ptr->dev_prop.mmoc_mmgsdi_wait_info.file_enum = MMGSDI_NO_FILE_ENUM;

  if ( (MMGSDI_SUCCESS  != status)                    ||
       (MMGSDI_READ_CNF !=  cnf)                      ||
       (cnf_ptr->read_cnf.read_data.data_ptr == NULL) ||
       (cnf_ptr->read_cnf.read_data.data_len == 0))
  {
    if(status == MMGSDI_SIM_BUSY)
    {
      mmoc_send_cm_rpt( CM_MMOC_SUBSCRIPTION_FAILURE, NULL, mm_id.asubs_id);

      /* Posting subsc failure to CM, clear read_items as sim busy*/
      mmgsdi_info_ptr->read_items[mm_id.asubs_id]   = MMOC_MMGSDI_MASK_NONE;
      mmgsdi_info_ptr->mmgsdi_items[mm_id.asubs_id] = MMOC_MMGSDI_MASK_NONE;
    }
  }
  else
  {

#ifdef FEATURE_MMSS_3_1

    /* Call in to SD lib to update the data read through MMGSDI to SD buffer
    **/
    sd_misc_update_filedb(cnf_ptr->read_cnf.access.file.file_enum,
                          cnf_ptr->read_cnf.read_data.data_ptr,
                          (uint32) cnf_ptr->read_cnf.read_data.data_len);

#endif /* #ifdef FEATURE_MMSS_3_1 */

  } /* else */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get the report buffer.
  **/
  msg_ptr = mmoc_get_rpt_buf_else_err_fatal();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all the fields.
  **/
  msg_ptr->rpt.name                   = MMOC_RPT_MMGSDI_CNF;
  msg_ptr->param.mmgsdi_cnf.status    = status;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Post the report.
  */
  mmoc_send_rpt( msg_ptr );

} /* mmocmmgsdi_session_read_cb */

/*===========================================================================

FUNCTION mmocmmgsdi_process_subsc_chgd_mmgsdi_read_enter

DESCRIPTION
  This is a function for handling the transaction state MMGSDI_READ_ENTER
  for the subscription changed transaction . If a MMGSDI session is used
  to read items such as PRL,PLMN,MSPL/MLPL, move to MMGSDI_READ_CNF
  else move to PH_STAT_ENTER.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Should wait for response for mmgsdi read.
  MMOC_EVT_STATUS_NOT_CONSUMED - Start ph_stat_enter processing.

===========================================================================*/
mmoc_evt_status_e_type      mmocmmgsdi_process_subsc_chgd_mmgsdi_read_enter
(
  const mmoc_rpt_msg_s_type       *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type          *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mmoc_mmgsdi_info_s_type  *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr();
  mmocmmgsdi_error_e_type   mmocmmgsdi_status[MAX_SIMS];
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;
  boolean read[MAX_SIMS];

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(mmgsdi_info_ptr->sim_busy_status[mmoc_get_state_info_ptr()->dev_prop.sub_with_3gpp2] == TRUE)
  {
    MMOC_MSG_HIGH_0("Marking read_items None as sim_busy");

    /* Posting subsc failure to CM, clear read_items as sim busy*/
    mmoc_send_cm_rpt( CM_MMOC_SUBSCRIPTION_FAILURE, NULL,mmoc_get_state_info_ptr()->dev_prop.sub_with_3gpp2 );

    for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
    {
      mmgsdi_info_ptr->read_items[sub] = MMOC_MMGSDI_MASK_NONE;
      mmgsdi_info_ptr->mmgsdi_items[sub] = MMOC_MMGSDI_MASK_NONE;
    }

    mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_PH_STAT_ENTER,
                             mmoc_info_ptr
                           );

    return MMOC_EVT_STATUS_NOT_CONSUMED;
  }

  for(sub = SYS_MODEM_AS_ID_1; sub < MAX_SIMS; sub++)
  {
    mmocmmgsdi_status[sub] = MMOC_MMGSDI_SUCCESS;
    read[sub] = FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check and read PRL from MMGSDI card */

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    if(MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[sub], MMOC_MMGSDI_MASK_PRL))
    {
      mmocmmgsdi_status[sub] = mmocmmgsdi_session_read_prl(mmoc_info_ptr, sub);
      read[sub] = TRUE;
    }
  }

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    if(mmocmmgsdi_status[sub] == MMOC_MMGSDI_SUCCESS && read[sub])
    {
      mmoc_update_trans_state( MMOC_TRANS_STATE_MMGSDI_READ_CNF,
                               mmoc_info_ptr
                             );

      return MMOC_EVT_STATUS_CONSUMED;
    }
    read[sub] = FALSE;
  }

#ifdef FEATURE_MMSS_3_1

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check and read MLPL/MSPL records from MMGSDI card */

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    if(MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[sub], MMOC_MMGSDI_MASK_MLPL) ||
        MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[sub], MMOC_MMGSDI_MASK_MSPL))
    {
      mmocmmgsdi_status[sub] = mmocmmgsdi_session_read_mlpl_mspl(mmoc_info_ptr, sub);
      read[sub] = TRUE;
    }
  }

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    if(mmocmmgsdi_status[sub] == MMOC_MMGSDI_SUCCESS && read[sub])
    {
      mmoc_update_trans_state( MMOC_TRANS_STATE_MMGSDI_READ_CNF,
                               mmoc_info_ptr
                             );

      return MMOC_EVT_STATUS_CONSUMED;
    }
    read[sub] = FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check and read PLMN records from MMGSDI card */

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    if(MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[sub], MMOC_MMGSDI_MASK_GSM_IMSI) ||
        MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[sub], MMOC_MMGSDI_MASK_GSM_AD) ||
        MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[sub], MMOC_MMGSDI_MASK_GSM_PLMNWACT) ||
        MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[sub], MMOC_MMGSDI_MASK_GSM_OPLMNWACT) ||
        MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[sub], MMOC_MMGSDI_MASK_GSM_PLMN) ||
        MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[sub], MMOC_MMGSDI_MASK_USIM_EHPLMN) ||
        MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[sub], MMOC_MMGSDI_MASK_USIM_PLMNWACT) ||
        MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[sub], MMOC_MMGSDI_MASK_USIM_OPLMNWACT) ||
        MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[sub], MMOC_MMGSDI_MASK_USIM_IMSI) ||
        MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[sub], MMOC_MMGSDI_MASK_USIM_AD))
    {
      mmocmmgsdi_status[sub] = mmocmmgsdi_session_read_plmn(mmoc_info_ptr, sub);
      read[sub] = TRUE;
    }
  }

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    if(mmocmmgsdi_status[sub] == MMOC_MMGSDI_SUCCESS && read[sub])
    {
      mmoc_update_trans_state( MMOC_TRANS_STATE_MMGSDI_READ_CNF,
                               mmoc_info_ptr
                             );

      return MMOC_EVT_STATUS_CONSUMED;
    }
    read[sub] = FALSE;
  }

#endif /* #ifdef FEATURE_MMSS_3_1 */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check and read IMSI from MMGSDI card */

#if defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900)
  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    if(((sd_operator_e_type)sd_ss_get_operator_name(sub)) == SD_OPERATOR_CT)
    {
      if(MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[sub], MMOC_MMGSDI_MASK_CSIM_IMSI) ||
          MMOC_IS_BIT_CONTAIN(mmgsdi_info_ptr->read_items[sub], MMOC_MMGSDI_MASK_RUIM_IMSI))
      {

        mmocmmgsdi_status[sub] = mmocmmgsdi_session_read_card_imsi(mmoc_info_ptr, sub);
        read[sub] = TRUE;
      }
    }

    if(mmocmmgsdi_status[sub] == MMOC_MMGSDI_SUCCESS && read[sub])
    {
      mmoc_update_trans_state( MMOC_TRANS_STATE_MMGSDI_READ_CNF,
                               mmoc_info_ptr
                             );

      return MMOC_EVT_STATUS_CONSUMED;
    }
  }
#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Either all items are read successfully or tried to read them
  ** through MMGSDI session. Now move to PH_STATE_ENTER and
  ** continue default processing.
  */

  for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
  {
    mmgsdi_info_ptr->read_items[sub] = MMOC_MMGSDI_MASK_NONE;
  }

  mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_PH_STAT_ENTER,
                           mmoc_info_ptr
                         );

  return MMOC_EVT_STATUS_NOT_CONSUMED;

} /* mmocmmgsdi_process_subsc_chgd_mmgsdi_read_enter*/

/*===========================================================================

FUNCTION mmocmmgsdi_process_subsc_chgd_mmgsdi_read_cnf

DESCRIPTION
  This is a function for handling the transaction state MMGSDI_READ_CNF for the
  subscription changed transaction . If the report is MMOC_RPT_MMGSDI_CNF,
  move to PH_STAT_ENTER else stay in the same state.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Processing finished.
  MMOC_EVT_STATUS_NOT_CONSUMED - Start ph_stat_enter processing.

===========================================================================*/
mmoc_evt_status_e_type         mmocmmgsdi_process_subsc_chgd_mmgsdi_read_cnf
(
  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mmoc_mmgsdi_info_s_type *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr();
  sys_modem_as_id_e_type sub = SYS_MODEM_AS_ID_NONE;

  /* Check if the rpt ptr is NULL
  */
  if ( rpt_ptr == NULL )
  {
    MMOC_ERR_0("rpt NULL");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Process the received report.
  */
  switch(rpt_ptr->rpt.name)
  {
    case MMOC_RPT_PROT_AUTO_DEACTD_IND:
      return mmoc_buf_auto_deact_ind_rpt(rpt_ptr,mmoc_info_ptr,rpt_ptr->param.prot_deactd_ind.mm_id);

    case MMOC_RPT_MMGSDI_CNF:
      for(sub = SYS_MODEM_AS_ID_1; sub < mmoc_info_ptr->nSubs && sub < MAX_SIMS; sub++)
      {
        if(mmgsdi_info_ptr->read_items[sub])
        {
          /* More items need to be read from MMGSDI card
          ** Move to MMOC_TRANS_STATE_MMGSDI_READ_ENTER to read those items.
          */
          mmoc_update_trans_state( MMOC_TRANS_STATE_MMGSDI_READ_ENTER,
                                   mmoc_info_ptr
                                 );
          return MMOC_EVT_STATUS_NOT_CONSUMED;
        }
        else
        {
          /* All items are read from the card.
          ** Move to MMOC_TRANS_STATE_PROT_PH_STAT_ENTER to continue.
          */
          mmoc_update_trans_state( MMOC_TRANS_STATE_PROT_PH_STAT_ENTER,
                                   mmoc_info_ptr
                                 );
        }
      }
      return MMOC_EVT_STATUS_NOT_CONSUMED;

    default:
      MMOC_MSG_MED_1( "Unexpected report received %d",
                      rpt_ptr->rpt.name);
      return MMOC_EVT_STATUS_CONSUMED;
  } /* switch ( rpt_ptr->rpt.name ) */

} /* mmocmmgsdi_process_subsc_chgd_mmgsdi_read_cnf */


/*===========================================================================
=============================================================================
=============================================================================
==================== MMGSDI_INFO_IND TRANSACTION ============================
=============================================================================
=============================================================================
===========================================================================*/


/*===========================================================================

FUNCTION mmocmmgsdi_process_mmgsdi_info_null

DESCRIPTION
  This function handles the MMOC_CMD_MMGSDI_INFO_IND
  command in the NULL transaction state.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmocmmgsdi_process_mmgsdi_info_null
(
  const mmoc_cmd_msg_s_type      *cmd_ptr,
  /* Pointer to the command event that was received.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  boolean         wait_session_open = FALSE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_ASSERT( cmd_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* The MMOC_CMD_MMGSDI_INFO_IND should only create this transaction.
  */
  if ( cmd_ptr == NULL ||
       cmd_ptr->cmd.name != MMOC_CMD_MMGSDI_INFO_IND
     )
  {
    MMOC_ERR_0("Autonomous MMGSDI_INFO_IND transaction created incorrectly!");

    return mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get 1X Provisioning session information. If there is no session opened,
  ** open the session now.
  */
  if( !( mmocmmgsdi_is_session_opened(MMGSDI_1X_PROV_PRI_SESSION) ) )
  {
    /* Use the client ID to open a session to read PRL*/
    mmocmmgsdi_open_session(MMGSDI_1X_PROV_PRI_SESSION);
    wait_session_open = TRUE;
  }

  /* Get CARD_SESSION_SLOT_1 session information. If there is no session opened,
  ** open the session now.
  */
  if( !( mmocmmgsdi_is_session_opened(MMGSDI_CARD_SESSION_SLOT_1) ) )
  {
    /* Use the client ID to open a session to read MLPL/MSPL*/
    mmocmmgsdi_open_session(MMGSDI_CARD_SESSION_SLOT_1);
    wait_session_open = TRUE;
  }

  /* Get CARD_SESSION_SLOT_2 session information. If there is no session opened,
  ** open the session now.
  */
  if( !( mmocmmgsdi_is_session_opened(MMGSDI_CARD_SESSION_SLOT_2) ) )
  {
    /* Use the client ID to open a session to read MLPL/MSPL*/
    mmocmmgsdi_open_session(MMGSDI_CARD_SESSION_SLOT_2);
    wait_session_open = TRUE;
  }

  /* Get GW Provisioning session information. If there is no session opened,
  ** open the session now.
  */
  if( !( mmocmmgsdi_is_session_opened(MMGSDI_GW_PROV_PRI_SESSION) ) )
  {
    /* Use the client ID to open a session to read PLMN*/
    mmocmmgsdi_open_session(MMGSDI_GW_PROV_PRI_SESSION);
    wait_session_open = TRUE;
  }

#if defined(FEATURE_MMODE_DUAL_SIM)
  /* Get hybr GW provisioning session information. If there is no session opened,
  ** open the session now.
  */
  if( !( mmocmmgsdi_is_session_opened(MMGSDI_GW_PROV_SEC_SESSION) ) )
  {
    /*open a sec  session */
    mmocmmgsdi_open_session(MMGSDI_GW_PROV_SEC_SESSION);
    wait_session_open = TRUE;
  }

  if( !( mmocmmgsdi_is_session_opened(MMGSDI_1X_PROV_SEC_SESSION) ) )
  {
    /* Use the client ID to open a session to read PRL*/
    mmocmmgsdi_open_session(MMGSDI_1X_PROV_SEC_SESSION);
    wait_session_open = TRUE;
  }

#if defined FEATURE_MMODE_TRIPLE_SIM
  /* Get hybr GW3 provisioning session information. If there is no session opened,
  ** open the session now.
  */
  if( !( mmocmmgsdi_is_session_opened(MMGSDI_GW_PROV_TER_SESSION) ) )
  {
    /*open a ter session */
    mmocmmgsdi_open_session(MMGSDI_GW_PROV_TER_SESSION);
    wait_session_open = TRUE;
  }

#endif

#endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Session open request sent, wait for confirmation.
  */
  if( wait_session_open)
  {
    mmoc_update_trans_state( MMOC_TRANS_STATE_WAIT_SESSION_OPEN_CNF,
                             mmoc_info_ptr
                           );
  }
  else
  {
    /* Transaction cleared.
    **/
    return mmoc_clear_transaction(cmd_ptr, mmoc_info_ptr);
  }

  return MMOC_EVT_STATUS_CONSUMED;

} /* mmocmmgsdi_process_mmgsdi_info_null() */


/*===========================================================================

FUNCTION mmocmmgsdi_process_session_open_cnf

DESCRIPTION
  This function handles the MMOC_CMD_MMGSDI_INFO_IND
  command in the NULL transaction state.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
static  mmoc_evt_status_e_type         mmocmmgsdi_process_session_open_cnf
(
  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mmoc_mmgsdi_info_s_type       *mmgsdi_info_ptr = mmoc_get_mmgsdi_info_ptr();
  unsigned int                  index_1x_pri;
  unsigned int                  index_gw_pri;
#if defined(FEATURE_MMODE_DUAL_SIM)
  unsigned int                  index_gw_sec;
  unsigned int                  index_1x_sec;
  boolean                       all_sec_sessions_opened = FALSE;
#endif
#if defined(FEATURE_MMODE_TRIPLE_SIM)
  unsigned int                  index_gw_ter;
  boolean                       all_ter_sessions_opened = FALSE;
#endif
  unsigned int                  index_card_slot_1;
  unsigned int                  index_card_slot_2;
  boolean                       all_pri_sessions_opened = FALSE;
  mm_sub_stk_id_s_type          mm_id;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MMOC_ASSERT( rpt_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* The MMOC_CMD_MMGSDI_INFO_IND should only create this transaction.
  */

  if (rpt_ptr == NULL)
  {
    MMOC_ERR_0("unexpected report ");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  if (rpt_ptr->rpt.name == MMOC_RPT_PROT_AUTO_DEACTD_IND)
  {
    MMOC_MSG_LOW_0("Received autonomous deactivated report");

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Process LTE to DO redirection abort
    */
    if( rpt_ptr->param.prot_deactd_ind.prot_state == PROT_STATE_ONLINE_HDR &&
        rpt_ptr->param.prot_deactd_ind.reason     == PROT_DEACT_REDIR_ABORT &&
        mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->stack_prop[MM_STACK_1]->prot_state
        != PROT_STATE_ONLINE_HDR)
    {
      MMOC_MSG_HIGH_0("DO Redir aborted");

      mmoc_info_ptr->dev_prop.is_redir_allowed = FALSE;

      return MMOC_EVT_STATUS_CONSUMED;
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef MMOC_HYBR_SUPPORTED
    /* Check if the deactivation was from SS_HDR system selection instance.
    */
    if ( rpt_ptr->param.prot_deactd_ind.prot_state  == PROT_STATE_ONLINE_HDR &&
         mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->stack_prop[MM_STACK_1]->prot_state
         == PROT_STATE_ONLINE_HDR)
    {
      /* If hybrid preference is not ON then hybrid hdr instance should
      ** not be triggered to go ONLINE
      */
      if (sd_misc_is_hybr_opr(mmoc_info_ptr->dev_prop.sub_with_3gpp2))
      {
        /* It is for SS_HDR instance, activate SS_HDR and return.
        */
        (void)mmoc_activate_prot_online_ss_hdr( sd_ss_hdr_act_get(mmoc_info_ptr->dev_prop.sub_with_3gpp2, NULL),
                                                mmoc_info_ptr->dev_prop.sub_with_3gpp2,
                                                mmoc_info_ptr
                                              );
      }
      else
      {
        mm_id.asubs_id = mmoc_info_ptr->dev_prop.sub_with_3gpp2;
        mm_id.stk_id   = MM_STACK_1;

        MMOC_ERR_0 ("AUTO_DEACTD_IND from HYBR_HDR");

        mmoc_update_prot_state( PROT_STATE_NULL,
                                mm_id,
                                mmoc_info_ptr
                              );
      }

      return MMOC_EVT_STATUS_CONSUMED;
    }
#endif /* MMOC_HYBR_SUPPORTED */

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Check if auto deactivation from HYBR_2 stack */

    if(mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM) &&
        rpt_ptr->param.prot_deactd_ind.mm_id.stk_id == MM_STACK_2 &&
        mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->stack_prop[MM_STACK_2]->prot_state
        == PROT_STATE_ONLINE_GWL)
    {
      if((mmoc_is_sxlte(mmoc_info_ptr->dev_prop.sub_with_3gpp2) &&
          mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->active_stacks & MM_STACK_0_MASK) ||
          mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->active_stacks & MM_STACK_2_MASK)
      {
        /* check main active for SG/SV  + G */
        mm_id.asubs_id = mmoc_info_ptr->dev_prop.sub_with_3gpp2;
        mm_id.stk_id   = MM_STACK_2;
        (void)mmoc_activate_prot_online_ss_hybr_2( sd_ss_act_get_per_mm_id(NULL, mm_id),
            mmoc_info_ptr->dev_prop.sub_with_3gpp2,
            mmoc_info_ptr
                                                 );
      }
      else
      {
        mm_id.asubs_id = mmoc_info_ptr->dev_prop.sub_with_3gpp2;
        mm_id.stk_id   = MM_STACK_2;

        MMOC_ERR_2 ("AUTO_DEACTD_IND from HYBR_2, DS_Pref = %d, active_subs = %d",
                    mmoc_info_ptr->dev_prop.standby_pref,
                    mmoc_info_ptr->dev_prop.active_subs);

        mmoc_update_prot_state( PROT_STATE_NULL,
                                mm_id,
                                mmoc_info_ptr
                              );
      }

      return MMOC_EVT_STATUS_CONSUMED;
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    if(mmoc_is_1x_sxlte(mmoc_info_ptr->dev_prop.sub_with_3gpp2))
    {
      /* Check if auto deactivation from HYBR_2 stack
      */
      if(rpt_ptr->param.prot_deactd_ind.mm_id.stk_id == MM_STACK_2 &&
          mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->stack_prop[MM_STACK_2]->prot_state
          == PROT_STATE_ONLINE_GWL)
      {
        mm_id.asubs_id = mmoc_info_ptr->dev_prop.sub_with_3gpp2;
        mm_id.stk_id   = MM_STACK_2;
        (void)mmoc_activate_prot_online_ss_hybr_2( sd_ss_act_get_per_mm_id(NULL, mm_id),
            mmoc_info_ptr->dev_prop.sub_with_3gpp2,
            mmoc_info_ptr
                                                 );
        return MMOC_EVT_STATUS_CONSUMED;
      }
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    if(mmoc_is_featuremode(SYS_OVERALL_FEATURE_MODE_MULTISIM))
    {
      /* For dual standby, activate MAIN only if MAIN should be active.
      */
      if(mmoc_info_ptr->sub_prop[mmoc_info_ptr->dev_prop.sub_with_3gpp2]->active_stacks & MM_STACK_0_MASK)
      {
        mm_sub_stk_id_s_type mm_id;
        mm_id.asubs_id = mmoc_info_ptr->dev_prop.sub_with_3gpp2;
        mm_id.stk_id   = MM_STACK_0;

        /*  Activate main at the end of transaction.
        */
        return mmoc_activate_prot_online_ss_main( sd_ss_act_get_per_mm_id( NULL, mm_id ),
               mmoc_info_ptr->dev_prop.sub_with_3gpp2,
               mmoc_info_ptr
                                                );
      }
      return MMOC_EVT_STATUS_CONSUMED;
    }
    else
    {
      mm_sub_stk_id_s_type mm_id;
      mm_id.asubs_id = mmoc_info_ptr->dev_prop.sub_with_3gpp2;
      mm_id.stk_id   = MM_STACK_0;

      /*  Activate main at the end of transaction.
      */
      return mmoc_activate_prot_online_ss_main( sd_ss_act_get_per_mm_id( NULL, mm_id ),
             mmoc_info_ptr->dev_prop.sub_with_3gpp2,
             mmoc_info_ptr);
    }
  }

  if ( rpt_ptr->rpt.name != MMOC_RPT_MMGSDI_CNF )
  {
    MMOC_ERR_1("Invalid rpt %d, while waiting for session open cnf",
               rpt_ptr->rpt.name);

    return MMOC_EVT_STATUS_CONSUMED;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( (MMOC_MMGSDI_SUCCESS == mmocmmgsdi_get_session_index(
         (mmgsdi_session_type_enum_type) MMGSDI_1X_PROV_PRI_SESSION,
         &index_1x_pri)) &&
      (MMOC_MMGSDI_SUCCESS == mmocmmgsdi_get_session_index(
         (mmgsdi_session_type_enum_type) MMGSDI_GW_PROV_PRI_SESSION,
         &index_gw_pri)) &&
      (MMOC_MMGSDI_SUCCESS == mmocmmgsdi_get_session_index(
         (mmgsdi_session_type_enum_type) MMGSDI_CARD_SESSION_SLOT_1,
         &index_card_slot_1))   &&
      (MMOC_MMGSDI_SUCCESS == mmocmmgsdi_get_session_index(
         (mmgsdi_session_type_enum_type) MMGSDI_CARD_SESSION_SLOT_2,
         &index_card_slot_2))
    )
  {
    if( mmgsdi_info_ptr->mmgsdi_session_id_table[index_1x_pri].session_opened &&
        mmgsdi_info_ptr->mmgsdi_session_id_table[index_gw_pri].session_opened &&
        mmgsdi_info_ptr->mmgsdi_session_id_table[index_card_slot_1].session_opened &&
        mmgsdi_info_ptr->mmgsdi_session_id_table[index_card_slot_2].session_opened
      )
    {
      all_pri_sessions_opened = TRUE;
    }
  }
  else
  {
    MMOC_MSG_HIGH_0("Pri Session Index not found");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  MMOC_MSG_LOW_4("sess opn cnf: 1x_pri=%d, gw_pri=%d, card_1=%d, card_2=%d",
                 mmgsdi_info_ptr->mmgsdi_session_id_table[index_1x_pri].session_opened,
                 mmgsdi_info_ptr->mmgsdi_session_id_table[index_gw_pri].session_opened,
                 mmgsdi_info_ptr->mmgsdi_session_id_table[index_card_slot_1].session_opened,
                 mmgsdi_info_ptr->mmgsdi_session_id_table[index_card_slot_2].session_opened);

#ifdef FEATURE_MMODE_DUAL_SIM
  if( (MMOC_MMGSDI_SUCCESS == mmocmmgsdi_get_session_index(
         (mmgsdi_session_type_enum_type) MMGSDI_GW_PROV_SEC_SESSION,
         &index_gw_sec)) &&
      (MMOC_MMGSDI_SUCCESS == mmocmmgsdi_get_session_index(
         (mmgsdi_session_type_enum_type) MMGSDI_1X_PROV_SEC_SESSION,
         &index_1x_sec))
    )
  {
    if(mmgsdi_info_ptr->mmgsdi_session_id_table[index_gw_sec].session_opened &&
        mmgsdi_info_ptr->mmgsdi_session_id_table[index_1x_sec].session_opened)
    {
      all_sec_sessions_opened = TRUE;
    }
  }
  else
  {
    MMOC_MSG_HIGH_0("Sec Session Index not found");
    return MMOC_EVT_STATUS_CONSUMED;
  }

  MMOC_MSG_HIGH_2("sess opn cnf: 1x_sec=%d, gw_sec=%d",
                  mmgsdi_info_ptr->mmgsdi_session_id_table[index_1x_sec].session_opened,
                  mmgsdi_info_ptr->mmgsdi_session_id_table[index_gw_sec].session_opened);


#if defined FEATURE_MMODE_TRIPLE_SIM

  if( MMOC_MMGSDI_SUCCESS == mmocmmgsdi_get_session_index(
        (mmgsdi_session_type_enum_type) MMGSDI_GW_PROV_TER_SESSION,
        &index_gw_ter)
    )
  {
    if(mmgsdi_info_ptr->mmgsdi_session_id_table[index_gw_ter].session_opened)
    {
      all_ter_sessions_opened = TRUE;
    }
  }
  else
  {
    MMOC_MSG_HIGH_0("Ter Session Index not found");
    return MMOC_EVT_STATUS_CONSUMED;
  }
  MMOC_MSG_HIGH_1("sess opn cnf: gw_ter=%d",mmgsdi_info_ptr->mmgsdi_session_id_table[index_gw_ter].session_opened);

#endif

#endif

  /* Once all the sessions are opened go to the next state */
  MMOC_MSG_HIGH_1("sess: all_pri_sessions_opened=%d",all_pri_sessions_opened);
#ifdef FEATURE_MMODE_DUAL_SIM
  MMOC_MSG_HIGH_1("sess: all_sec_sessions_opened=%d",all_sec_sessions_opened);
#endif
#ifdef FEATURE_MMODE_TRIPLE_SIM
  MMOC_MSG_HIGH_1("sess: all_ter_sessions_opened=%d",all_ter_sessions_opened);
#endif
  if(all_pri_sessions_opened
#ifdef FEATURE_MMODE_DUAL_SIM
      && all_sec_sessions_opened
#endif
#ifdef FEATURE_MMODE_TRIPLE_SIM
      && all_ter_sessions_opened
#endif
    )
  {
    /* Transaction cleared.
    **/
    return mmoc_clear_transaction(NULL, mmoc_info_ptr);
  }
  else
  {
    return MMOC_EVT_STATUS_CONSUMED;
  }

} /* mmocmmgsdi_process_session_open_cnf() */


/*===========================================================================

FUNCTION mmocmmgsdi_process_mmgsdi_info_ind

DESCRIPTION
  This function is the transaction handler for MMOC_TRANS_MMGSDI_INFO_IND
  transaction.

RETURN VALUE
  MMOC_EVT_STATUS_CONSUMED     - Event was processed to completion.
  MMOC_EVT_STATUS_NOT_CONSUMED - Event was not processed. Caller should
                                  invoke the appropriate transaction handler
                                  again.

===========================================================================*/
mmoc_evt_status_e_type         mmocmmgsdi_process_mmgsdi_info_ind
(
  const mmoc_cmd_msg_s_type      *cmd_ptr,
  /* Pointer to the command event that was received.
  ** if this was not called because of command event, then
  ** cmd_ptr = null.
  */

  const mmoc_rpt_msg_s_type      *rpt_ptr,
  /* Pointer to the report event that was received.
  ** if this was called because of report event, then
  ** rpt_ptr = null.
  */

  mmoc_state_info_s_type         *mmoc_info_ptr
  /* Pointer to MMoC's state information, may/may not be updated
  ** by the transaction state handlers.
  */
)
{
  mmoc_evt_status_e_type evt_status = MMOC_EVT_STATUS_CONSUMED;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Invoke the appropriate transaction state handler.
  */
  switch(mmoc_info_ptr->dev_prop.trans_state)
  {
    case MMOC_TRANS_STATE_NULL:
      evt_status = mmocmmgsdi_process_mmgsdi_info_null( cmd_ptr,
                   mmoc_info_ptr
                                                      );
      break;

    case MMOC_TRANS_STATE_WAIT_SESSION_OPEN_CNF:
      evt_status = mmocmmgsdi_process_session_open_cnf( rpt_ptr,
                   mmoc_info_ptr
                                                      );
      break;

    default:
      MMOC_ERR_1("Received event in unknown state, state =%d",
                 mmoc_info_ptr->dev_prop.trans_state);

      evt_status = mmoc_clear_transaction( cmd_ptr, mmoc_info_ptr );

      break;
  }

  return evt_status;

} /* mmocmmgsdi_process_mmgsdi_info_ind() */

/* <EJECT> */

