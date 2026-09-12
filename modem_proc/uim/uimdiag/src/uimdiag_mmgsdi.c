/*===========================================================================

===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2009 - 2017 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/uimdiag/src/uimdiag_mmgsdi.c#2 $$ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when       who   what, where, why
--------   ---   -----------------------------------------------------------
04/19/17   dt    Fixing index leak in UIMDIAG_MMGSDI_GET_ATR_CMD
03/30/16   dt    removing handling for MMGSDI_TERMINAL_PROFILE_DL_EVT
02/02/17   dt    Memory leak issue correction in uimdiag_mmgsdi_get_atr_cmd
01/02/16   dt    correcting QXDM sim data collection strip pack issues
12/29/16   dt    support changes for QXDM sim data collection 
07/28/16   ar    Handle auto-activation race cond by passing an extra param
06/21/16   ar    Fix compiler warning
05/20/16   vdc   Remove F3 messages for memory allocation failure
03/11/16   ar    Remove FEATURE_UIMDIAG
02/22/16   gm    Deprecate mmgsdi_option_type
11/02/15   stv   Rel 12 refresh enforcement policy support
10/05/15   sp    Reduce signed/unsigned conversions in UIM
09/16/15   tkl   Fix KW errors
08/28/15   hh    Removed JCDMA related code
08/27/15   av    Retain legacy enum values of major existing MMGSDI events
05/28/15   tl    Return error when reg for refresh when file enum not valid
05/15/15   yt    Support for mapping sessions to slots
04/25/15   av    Support for permanently disabling CSIM
02/25/15   yt    Reduction of UIMDIAG functionality
02/20/15   vv    Remove perso related code
09/17/14   vr    Correction in heap variable created in stack reduction
09/09/14   vr    Reduce stack usage
07/31/14   hh    Removed FEATURE_MMGSDIDIAG_TEST_AUTOMATION
07/10/14   tl    Remove ALWAYS OFF feature FEATURE_MMGSDI_PKCS15
04/22/14   at    Critical KW fixes
01/27/14   df    Featurise file with FEATURE_UIMDIAG
01/15/14   ar    Parsing file path in session open and send card status API cnf
01/10/14   df    Use v2.0 diag macros
12/21/13   spo   Fix for opening a non-prov session on slot 3
11/05/13   df    Removed unnecessary explicit type casting, add NULL ptr check
10/08/13   df    Correct handling default case in uimdiag_mmgsdi_event_cb()
09/26/13   ar    Full path to be copied while using memscpy.
09/25/13   bcho  Query type parsed in UIMDIAG_MMGSDI_SESSION_GET_INFO_CMD
09/24/13   bcho  Parsing for SPN disp bit, RPLMN disp bit and roaming status
09/19/13   vv    Fix for a possible buffer overflow
09/16/13   yt    Remove redundant code
05/24/13   bcho  Proper parsing of get feature data in UIMDIAG_MMGSDI_PERSO_CNF
05/17/13   hn    Support for Alpha for Refresh
05/06/13   sw    Changes made for secure version of memcpy
04/24/13   rr    Featurized optional logs
04/04/13   abd   Added utility function to update app information
04/16/13   bcho  Respond to script even if response handling fails
04/12/13   spo   Added code to handle dynamic data in confirmation
03/21/13   bcho  Heap memory corruption resolved
03/19/13   vdc   Fixed MMGSDI's status in immediate response for UIMDIAG cmd
03/14/13   bcho  Replace local variable with heap buffer to reduce stack usage
03/13/13   vdc   Fixing invalid access due to non-memset of API Parameter
12/12/12   at    Fixed critical KW warnings
11/23/12   bcho  Support for RAT type param in get_operator_name API
10/03/12   tl    Fix Perso CNF crash
10/01/12   tl    Added support for dynamically allocated response data for cnf
09/26/12   tl    Added support for unblock dck retries
09/03/12   bcho  Added support for MMGSDI_EONS_OPL_PNN_CACHE_READY_EVT
08/31/12   abg   Updated UPIN replacement in Get All Pin Status response
02/29/12   bcho  Q6 compilation warnings removed
12/27/11   kk    Fixed sync command handling
12/15/11   kk    Fixed compilation warnings
11/23/11   adp   MMGSDI status should be returned correctly in erroroneous
                 conditions too
11/21/11   bcho  Incorporated review comments
11/21/11   kk    Fixed get all available apps async rsp handling
11/17/11   kk    Added get all available apps async support
11/10/11   bcho  Support for Get Operator Name API modified
10/24/11   adp   Removing Num_of_files from request field for naa_refresh
10/24/11   adp   Removing support for UIMDIAG_MMGSDI_SAP_PIN_VERIFY_CMD,
                 UIMDIAG_MMGSDI_SAP_INITIALIZE_CMD and
                 UIMDIAG_MMGSDI_SAP_DEREGISTER_CMD
10/17/11   adp   Fixing KW errors
10/18/11   bcho  Adding support for mmgsdi_session_open_ext API
10/14/11   bcho  Added support for Get Operator Name API
10/10/11   nb    Fix for compilation error
10/10/11   nb    Support for SAP CONNECT/SAP DISCONNECT
10/08/11   adp   Changing uimdiag_mmgsdi_rsp_session_get_info_pyld_type and
                 uimdiag_mmgsdi_session_read_cache_rsp_type to match ICD.
10/03/11   adp   Fixing compiler warning
09/26/11   adp   Fixed crash, where memory pointer is NULL, but was being
                 assigned a value
09/15/11   adp   Adding support fo trigger Refresh
09/15/11   nb    Support for new refresh stage
08/23/11   adp   Support for handling DF_ENUM access confirmation
08/24/11   nk    Fix buffer overflow KW error
08/22/11   av    Added support for pdown,pup,activate onchip,send apdu commands
08/18/11   adp   Filling the pin operation field in response for
                 uimdiag_mmgsdi_proc_pin_operation_cnf
08/05/11   av    UIMDiag now doesn't return error if search pattern is null;
                 It's handled in mmgsdi now.
06/09/11   adp   Fixing get file attr by path access, Fixing search and seek
03/14/11   adp   Initializing the rsp pointer payload to zero, in order to
                 handle error conditions
02/22/11   ea    Fix compiler warnings
02/02/10   ms    Fixed compiler warnings
12/14/10   adp   PIN/PERSO commands when tried with invalid key, should
                 return num of retries
12/03/10   adp   Adding support for a number of commands
11/15/10   adp   Displaying MMGSDI Status (delayed) after the MMGSDI API call
11/09/10   adp   DIAG Header status to be always set to SUCCESS, so that when
                 processing commands with wrong parameters UIMDIAG will not
                 indicate "error".
11/09/10   adp   Adding Event support in UIMDIAG
10/05/10   adp   Fixing Lint, KW  warnings
09/07/10   adp   Adding Support for dck_num_retries,lock_down_enable_ltd_access,
                 seek and search commands
08/09/10   adp   Adding support for service enable, disable, get perso key,
                 is service available, perso unblock.
06/09/10   adp   Fixing Compiler Warnings
05/26/10   yt    Fixed compiler warnings
04/27/10   shr   Added Hiddenkey support
03/25/10   adp   Adding support for mmgsdi_session_deactivate_provisioning,
                 mmgsdi_session_get_app_capabilities,
                 mmgsdi_session_get_cphs_info,
                 isim_authenticate, send_card_status,event handling,
                 create_pkcs15_lookup_table
03/24/10   adp   Fixing compiler warnings
03/19/10   vs    Fixed AID and label length check
03/19/10   rk    Added support for JCDMA get info command and event handling
03/24/10   adp   Fixing compiler warnings
02/22/10   adp   Adding support for Perso Commands
02/03/10   adp   Fix for memory copy issue while writing to record
12/27/09   ap    Adding support for Refresh commands
01/07/10   adp   KlocWork Errors
12/17/09   rk    Fix for uimdiag small heap memory alloction issue
12/03/09   rk    Adding support for get file attributes, read and write commands
10/19/09   adp   Adding pin commands
10/18/09   adp   Restructing the code to make it more readable.
                 Adding support for Activate_or_switch_prov, session_get_info
09/02/09   adp   Initial Revision -Added initial mmgsdi session based commands


=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/
#include "uim_variation.h"
#include "mmgsdisessionlib.h"
#include "mmgsdisessionlib_v.h"
#include "uimdiag_util.h"
#include "mmgsdilib.h"
#include "mmgsdilib_v.h"
#include "mmgsdilib_p.h"
#include "comdef.h"
#include "uimdiag.h"
#include "uimdiag_mmgsdi.h"
#include "uimdiag_mmgsdi_util.h"
#include "mmgsdilib_common.h"
#include "intconv.h"
#include "diagpkt.h"
#include "diagcmd.h"
#include "event.h" /* event_report function */
#include "rex.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

#define UIMDIAG_IMEI_KEY_SEPARATOR                                0x3A

/* Refresh Enforcement Policy values specified in specification 3GPP TS 102.223 */
#define UIMDIAG_MMGSDI_REFRESH_ENFORCEMENT_NAVIGATING_MENU_MASK   0x01
#define UIMDIAG_MMGSDI_REFRESH_ENFORCEMENT_DATA_CALL_MASK         0x02
#define UIMDIAG_MMGSDI_REFRESH_ENFORCEMENT_VOICE_CALL_MASK        0x04

/* Valid enforcement mask is the value OR'ed by the above valid masks */
#define UIMDIAG_MMGSDI_REFRESH_ENFORCEMENT_VALID_MASK            (UIMDIAG_MMGSDI_REFRESH_ENFORCEMENT_NAVIGATING_MENU_MASK | \
                                                                  UIMDIAG_MMGSDI_REFRESH_ENFORCEMENT_DATA_CALL_MASK       | \
                                                                  UIMDIAG_MMGSDI_REFRESH_ENFORCEMENT_VOICE_CALL_MASK)

/*===========================================================================

                        STRUCTURE

===========================================================================*/
/* Structure to store the response and index of a command in case of immediate response */
typedef struct {
  uint8                       index;
  uimdiag_generic_rsp_type   *rsp_ptr;
}uimdiag_sync_rsp_type;

/*=============================================================================

                              FUNCTIONS

=============================================================================*/

/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_PROC_CLIENT_ID_AND_EVT_REG_CNF

DESCRIPTION
  Function to populate the response pointer with the confirmation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_proc_client_id_and_evt_reg_cnf(
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_cnf_type         *cnf_data_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_data_ptr, rsp_ptr);

  (void)memset((void*)&rsp_ptr->rsp_payload.response.client_id_and_evt_reg,
               0x00,sizeof(uimdiag_mmgsdi_rsp_client_id_and_evt_reg_pyld_type));

  rsp_ptr->rsp_payload.response.client_id_and_evt_reg.client_id_low =
    UIMDIAG_LODWORD(
      cnf_data_ptr->client_id_and_evt_reg_cnf.response_header.client_id);

  rsp_ptr->rsp_payload.response.client_id_and_evt_reg.client_id_high =
    UIMDIAG_HIDWORD(
      cnf_data_ptr->client_id_and_evt_reg_cnf.response_header.client_id);

  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_proc_client_id_and_evt_reg_cnf */

/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_PROC_CLIENT_ID_AND_EVT_DEREG_CNF

DESCRIPTION
  Function to populate the response pointer with the confirmation data
  for client id and evt dereg

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_proc_client_id_and_evt_dereg_cnf(
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_cnf_type         *cnf_data_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_data_ptr, rsp_ptr);

  (void)memset((void*)&rsp_ptr->rsp_payload.response.client_id_and_evt_dereg,
               0x00,sizeof(uimdiag_mmgsdi_rsp_client_id_and_evt_dereg_pyld_type));

  rsp_ptr->rsp_payload.response.client_id_and_evt_dereg.client_id_low =
    UIMDIAG_LODWORD(
      cnf_data_ptr->client_id_and_evt_dereg_cnf.response_header.client_id);

  rsp_ptr->rsp_payload.response.client_id_and_evt_dereg.client_id_high =
    UIMDIAG_HIDWORD(
      cnf_data_ptr->client_id_and_evt_dereg_cnf.response_header.client_id);

  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_proc_client_id_and_evt_dereg_cnf */

/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_PROC_SESSION_OPEN_CNF

DESCRIPTION
  Function to populate the response pointer with the confirmation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_proc_session_open_cnf(
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_cnf_type         *cnf_data_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_data_ptr, rsp_ptr);

  (void)memset((void*)&rsp_ptr->rsp_payload.response.session_open,
    0x00,sizeof(uimdiag_mmgsdi_rsp_session_open_pyld_type));

   rsp_ptr->rsp_payload.response.session_open.session_id_low =
     UIMDIAG_LODWORD(
       (cnf_data_ptr->session_open_cnf.session_id));

    rsp_ptr->rsp_payload.response.session_open.session_id_high =
      UIMDIAG_HIDWORD(
        (cnf_data_ptr->session_open_cnf.session_id));

     rsp_ptr->rsp_payload.response.session_open.channel_id =
       (uint32)cnf_data_ptr->session_open_cnf.channel_id;
     rsp_ptr->rsp_payload.response.session_open.app_type   =
       (uint32)cnf_data_ptr->session_open_cnf.app_info.app_type;

     UIMDIAG_UTIL_RETURN_IF_OUT_OF_RANGE(
       cnf_data_ptr->session_open_cnf.app_info.aid.data_len,
        0,
        MMGSDI_MAX_AID_LEN);

     rsp_ptr->rsp_payload.response.session_open.aid_length =
       (uint32)cnf_data_ptr->session_open_cnf.app_info.aid.data_len;

     if(cnf_data_ptr->session_open_cnf.app_info.aid.data_len <=
         MMGSDI_MAX_AID_LEN)
     {
       memscpy((void *)(rsp_ptr->rsp_payload.response.session_open.aid_ptr),
       sizeof(rsp_ptr->rsp_payload.response.session_open.aid_ptr),
       cnf_data_ptr->session_open_cnf.app_info.aid.data_ptr,
       cnf_data_ptr->session_open_cnf.app_info.aid.data_len);
     }
     else
     {
        UIMDIAG_MSG_ERROR_0("Session Open CNF: Invalid AID data ptr");
        return UIMDIAG_ERROR;
     }

     UIMDIAG_UTIL_RETURN_IF_OUT_OF_RANGE(
      cnf_data_ptr->session_open_cnf.app_info.label.data_len,
      0,MMGSDI_MAX_AID_LEN);

     rsp_ptr->rsp_payload.response.session_open.label_len   =
       (uint32)cnf_data_ptr->session_open_cnf.app_info.label.data_len;

     if(cnf_data_ptr->session_open_cnf.app_info.label.data_len <=
        MMGSDI_MAX_AID_LEN)
      {
        memscpy((void *)(rsp_ptr->rsp_payload.response.session_open.label_ptr),
          sizeof(rsp_ptr->rsp_payload.response.session_open.label_ptr),
          cnf_data_ptr->session_open_cnf.app_info.label.data_ptr,
          cnf_data_ptr->session_open_cnf.app_info.label.data_len);
     }
     else
     {
       UIMDIAG_MSG_ERROR_0("Session Open CNF: Invalid Label data ptr");
       return UIMDIAG_ERROR;
     }
     return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_proc_session_open_cnf*/


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_PROC_SESSION_CLOSE_CNF

DESCRIPTION
  Function to populate the response pointer with the conformation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_proc_session_close_cnf(
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_cnf_type         *cnf_data_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_data_ptr, rsp_ptr);

  (void)memset((void*)&rsp_ptr->rsp_payload.response.session_close,
    0x00,sizeof(uimdiag_mmgsdi_rsp_session_close_pyld_type));

   rsp_ptr->rsp_payload.response.session_close.session_id_low   =
     UIMDIAG_LODWORD(
       (cnf_data_ptr->session_close_cnf.session_id));

    rsp_ptr->rsp_payload.response.session_close.session_id_high =
      UIMDIAG_HIDWORD(
        (cnf_data_ptr->session_close_cnf.session_id));

    rsp_ptr->rsp_payload.response.session_close.app_type        =
       (uint32)cnf_data_ptr->session_close_cnf.app_info.app_type;

    UIMDIAG_UTIL_RETURN_IF_OUT_OF_RANGE(
      cnf_data_ptr->session_close_cnf.app_info.aid.data_len,
      0,
      MMGSDI_MAX_AID_LEN);

    rsp_ptr->rsp_payload.response.session_close.aid_length      =
       (uint32)cnf_data_ptr->session_close_cnf.app_info.aid.data_len;

    if(cnf_data_ptr->session_close_cnf.app_info.aid.data_len <=
       MMGSDI_MAX_AID_LEN)
      {
       memscpy((void *)(rsp_ptr->rsp_payload.response.session_close.aid_ptr),
       sizeof(rsp_ptr->rsp_payload.response.session_close.aid_ptr),
       cnf_data_ptr->session_close_cnf.app_info.aid.data_ptr,
       cnf_data_ptr->session_close_cnf.app_info.aid.data_len);
      }
    else
     {
       UIMDIAG_MSG_ERROR_0("Uimdiag Session Close CNF: Invalid AID data ptr");
       return UIMDIAG_ERROR;
     }

    UIMDIAG_UTIL_RETURN_IF_OUT_OF_RANGE(
      cnf_data_ptr->session_close_cnf.app_info.label.data_len,
      0,
      MMGSDI_MAX_AID_LEN);

    rsp_ptr->rsp_payload.response.session_close.label_len       =
      (uint32)cnf_data_ptr->session_close_cnf.app_info.label.data_len;

    if(cnf_data_ptr->session_close_cnf.app_info.label.data_len  <=
       MMGSDI_MAX_AID_LEN)
    {
      memscpy((void *)(rsp_ptr->rsp_payload.response.session_close.label_ptr),
        sizeof(rsp_ptr->rsp_payload.response.session_close.label_ptr),
        cnf_data_ptr->session_close_cnf.app_info.label.data_ptr,
        cnf_data_ptr->session_close_cnf.app_info.label.data_len);
     }
     else
     {
       UIMDIAG_MSG_ERROR_0("UimDiag Session close CNF: Invalid Label data ptr");
       return UIMDIAG_ERROR;
     }
     return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_proc_session_close_cnf */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_PROC_ACT_OR_SWITCH_PROV_CNF

DESCRIPTION
  Function to populate the response pointer with the conformation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_proc_act_or_switch_prov_cnf(
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_cnf_type         *cnf_data_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_data_ptr, rsp_ptr);

  (void)memset(
    (void*)&rsp_ptr->rsp_payload.response.session_act_or_sw_prov,
      0x00, sizeof(uimdiag_mmgsdi_sessionid_type));

  rsp_ptr->rsp_payload.response.session_act_or_sw_prov.session_id_low =
    UIMDIAG_LODWORD(
      (cnf_data_ptr->session_activate_or_switch_provisioning_cnf.
                                                response_header.session_id));

  rsp_ptr->rsp_payload.response.session_act_or_sw_prov.session_id_high =
    UIMDIAG_HIDWORD(
      (cnf_data_ptr->session_activate_or_switch_provisioning_cnf.
                                                response_header.session_id));
  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_proc_act_or_switch_prov_cnf */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_PROC_GET_INFO_CNF

DESCRIPTION
  Function to populate the response pointer with the conformation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_proc_get_info_cnf(
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_cnf_type         *cnf_data_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_data_ptr, rsp_ptr);

  (void)memset((void*)&rsp_ptr->rsp_payload.response.session_get_info,
    0x00,sizeof(uimdiag_mmgsdi_rsp_session_get_info_pyld_type));

  rsp_ptr->rsp_payload.response.session_get_info.session_type    =
                     (uint32) cnf_data_ptr->session_get_info_cnf.session_type;

  rsp_ptr->rsp_payload.response.session_get_info.slot_id         =
    (uint32)cnf_data_ptr->session_get_info_cnf.slot_id;

  rsp_ptr->rsp_payload.response.session_get_info.channel_id         =
    (uint32)cnf_data_ptr->session_get_info_cnf.channel_id;

  rsp_ptr->rsp_payload.response.session_get_info.app_type        =
    (uint32)cnf_data_ptr->session_get_info_cnf.app_data.app_type;

  UIMDIAG_UTIL_RETURN_IF_OUT_OF_RANGE(
    cnf_data_ptr->session_get_info_cnf.app_data.aid.data_len,
    0,
    MMGSDI_MAX_AID_LEN);

  rsp_ptr->rsp_payload.response.session_get_info.aid_length      =
    cnf_data_ptr->session_get_info_cnf.app_data.aid.data_len;

  if(cnf_data_ptr->session_get_info_cnf.app_data.aid.data_len <=
     MMGSDI_MAX_AID_LEN)
    {
      memscpy((void *)(rsp_ptr->rsp_payload.response.session_get_info.aid_ptr),
      sizeof(rsp_ptr->rsp_payload.response.session_get_info.aid_ptr),
      cnf_data_ptr->session_get_info_cnf.app_data.aid.data_ptr,
      cnf_data_ptr->session_get_info_cnf.app_data.aid.data_len);
     }
   else
    {
      UIMDIAG_MSG_ERROR_0("Uimdiag Session Get info CNF: Invalid AID data ptr");
      return UIMDIAG_ERROR;
     }

   UIMDIAG_UTIL_RETURN_IF_OUT_OF_RANGE(
    cnf_data_ptr->session_get_info_cnf.app_data.label.data_len,
    0,
    MMGSDI_MAX_AID_LEN);

   rsp_ptr->rsp_payload.response.session_get_info.label_len      =
     cnf_data_ptr->session_get_info_cnf.app_data.label.data_len;

   if(cnf_data_ptr->session_get_info_cnf.app_data.label.data_len <=
      MMGSDI_MAX_AID_LEN)
   {
     memscpy((void *)(rsp_ptr->rsp_payload.response.session_get_info.label_ptr),
     sizeof(rsp_ptr->rsp_payload.response.session_get_info.label_ptr),
     cnf_data_ptr->session_get_info_cnf.app_data.label.data_ptr,
     cnf_data_ptr->session_get_info_cnf.app_data.label.data_len);
   }
   else
   {
      UIMDIAG_MSG_ERROR_0("Uimdiag Session get info CNF: Invalid Label data ptr");
      return UIMDIAG_ERROR;
   }
   return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_proc_get_info_cnf*/


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_PROC_GET_ALL_PIN_STATUS_CNF

DESCRIPTION
  Function to populate the response pointer with the confirmation data

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_proc_get_all_pin_status_cnf (
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_cnf_type         *cnf_data_ptr)

{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_data_ptr, rsp_ptr);

  (void)memset(
    (void*)&rsp_ptr->rsp_payload.response.session_get_all_pin_status,
    0x00,sizeof(uimdiag_mmgsdi_rsp_session_get_all_pin_stat_pyld_type));

  rsp_ptr->rsp_payload.response.session_get_all_pin_status.pin_id           =
    (uint32)cnf_data_ptr->get_all_pin_status_cnf.pin1.pin_id;

  rsp_ptr->rsp_payload.response.session_get_all_pin_status.pin_status       =
    (uint32)cnf_data_ptr->get_all_pin_status_cnf.pin1.status;

  rsp_ptr->rsp_payload.response.session_get_all_pin_status.pin_replacement  =
    (uint32)cnf_data_ptr->get_all_pin_status_cnf.pin1.pin_replacement;

  rsp_ptr->rsp_payload.response.session_get_all_pin_status.pin_num_retries  =
    (uint32)cnf_data_ptr->get_all_pin_status_cnf.pin1.num_retries;

  rsp_ptr->rsp_payload.response.session_get_all_pin_status.puk_num_retries  =
    (uint32)cnf_data_ptr->get_all_pin_status_cnf.pin1.num_unblock_retries;

  rsp_ptr->rsp_payload.response.session_get_all_pin_status.pin2_pin_id      =
    (uint32)cnf_data_ptr->get_all_pin_status_cnf.pin2.pin_id;

  rsp_ptr->rsp_payload.response.session_get_all_pin_status.pin2_status      =
    (uint32)cnf_data_ptr->get_all_pin_status_cnf.pin2.status;

  rsp_ptr->rsp_payload.response.session_get_all_pin_status.pin2_replacement =
    (uint32)cnf_data_ptr->get_all_pin_status_cnf.pin2.pin_replacement;

  rsp_ptr->rsp_payload.response.session_get_all_pin_status.pin2_num_retries =
    (uint32)cnf_data_ptr->get_all_pin_status_cnf.pin2.num_retries;

  rsp_ptr->rsp_payload.response.session_get_all_pin_status.puk2_num_retries =
    (uint32)cnf_data_ptr->get_all_pin_status_cnf.pin2.num_unblock_retries;

  rsp_ptr->rsp_payload.response.session_get_all_pin_status.upin_id          =
    (uint32)cnf_data_ptr->get_all_pin_status_cnf.universal_pin.pin_id;

  rsp_ptr->rsp_payload.response.session_get_all_pin_status.upin_status      =
    (uint32)cnf_data_ptr->get_all_pin_status_cnf.universal_pin.status;

  rsp_ptr->rsp_payload.response.session_get_all_pin_status.upin_replacement =
    (uint32)cnf_data_ptr->get_all_pin_status_cnf.universal_pin.pin_replacement;

  rsp_ptr->rsp_payload.response.session_get_all_pin_status.upin_num_retries =
    (uint32)cnf_data_ptr->get_all_pin_status_cnf.universal_pin.num_retries;

  rsp_ptr->rsp_payload.response.session_get_all_pin_status.upuk_num_retries =
    (uint32)cnf_data_ptr->get_all_pin_status_cnf.universal_pin.num_unblock_retries;

  rsp_ptr->rsp_payload.response.session_get_all_pin_status.hiddenkey_id =
    (uint32)cnf_data_ptr->get_all_pin_status_cnf.hiddenkey.pin_id;

  rsp_ptr->rsp_payload.response.session_get_all_pin_status.hiddenkey_status =
    (uint32)cnf_data_ptr->get_all_pin_status_cnf.hiddenkey.status;

  rsp_ptr->rsp_payload.response.session_get_all_pin_status.dir_index =
    (uint8)cnf_data_ptr->get_all_pin_status_cnf.dir_index;

  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_proc_get_all_pin_status_cnf */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_PROC_PIN_OPERATION_CNF

DESCRIPTION
  Function to populate the response pointer with the confirmation data
  for verify pin, disable pin and enable pin

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_proc_pin_operation_cnf (
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_cnf_type         *cnf_data_ptr)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_data_ptr, rsp_ptr);

  (void)memset((void*)&rsp_ptr->rsp_payload.response.session_pin_operation,
    0x00,sizeof(uimdiag_mmgsdi_rsp_session_pin_operation_pyld_type));

  rsp_ptr->rsp_payload.response.session_pin_operation.pin_id          =
    (uint32)cnf_data_ptr->pin_operation_cnf.pin_info.pin_id;

  rsp_ptr->rsp_payload.response.session_pin_operation.pin_status      =
    (uint32)cnf_data_ptr->pin_operation_cnf.pin_info.status;

  rsp_ptr->rsp_payload.response.session_pin_operation.pin_replacement =
    (uint32)cnf_data_ptr->pin_operation_cnf.pin_info.pin_replacement;

  rsp_ptr->rsp_payload.response.session_pin_operation.pin_num_retries =
    (uint32)cnf_data_ptr->pin_operation_cnf.pin_info.num_retries;

  rsp_ptr->rsp_payload.response.session_pin_operation.puk_num_retries =
    (uint32)cnf_data_ptr->pin_operation_cnf.pin_info.num_unblock_retries;

  rsp_ptr->rsp_payload.response.session_pin_operation.pin_op =
    (uint32)cnf_data_ptr->pin_operation_cnf.pin_op;

  rsp_ptr->rsp_payload.response.session_pin_operation.vld_pin_num_retries =
    (uint32)cnf_data_ptr->pin_operation_cnf.pin_info.valid_num_retries;

  rsp_ptr->rsp_payload.response.session_pin_operation.vld_puk_num_retries =
    (uint32)cnf_data_ptr->pin_operation_cnf.pin_info.valid_num_unblock_retries;
  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_proc_pin_operation_cnf  */

/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_PROC_GET_FILE_ATTR_CONF

DESCRIPTION
  Function to populate the response pointer with the confirmation data
  for get file attributes

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_proc_get_file_attr_cnf(
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_cnf_type         *cnf_data_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_data_ptr, rsp_ptr);

  (void)memset((void*)&rsp_ptr->rsp_payload.response.session_get_file_attr,
    0x00,sizeof(uimdiag_mmgsdi_rsp_session_get_file_attr_pyld_type));

  rsp_ptr->rsp_payload.response.session_get_file_attr.access_method_type =
       (uint32)cnf_data_ptr->get_file_attr_cnf.access.access_method;

  if (rsp_ptr->rsp_payload.response.session_get_file_attr.access_method_type ==
     (uint32) MMGSDI_EF_ENUM_ACCESS)
  {
    rsp_ptr->rsp_payload.response.session_get_file_attr.ef_enum  =
    (uint32)uimdiag_util_convert_from_mmgsdi_items_enum(
    cnf_data_ptr->get_file_attr_cnf.access.file.file_enum);
  }

  if(rsp_ptr->rsp_payload.response.session_get_file_attr.access_method_type ==
    (uint32) MMGSDI_DF_ENUM_ACCESS)
  {
    rsp_ptr->rsp_payload.response.session_get_file_attr.df_enum  =
      (uint32)cnf_data_ptr->get_file_attr_cnf.access.file.df_enum;
  }

  if(rsp_ptr->rsp_payload.response.session_get_file_attr.access_method_type ==
    (uint32) MMGSDI_BY_PATH_ACCESS)
  {
    rsp_ptr->rsp_payload.response.session_get_file_attr.path_len  =
      (uint32)cnf_data_ptr->get_file_attr_cnf.access.file.path_type.path_len;

    if(rsp_ptr->rsp_payload.response.session_get_file_attr.path_len > 0 &&
       rsp_ptr->rsp_payload.response.session_get_file_attr.path_len < MMGSDI_MAX_PATH_LEN)
    {
      memscpy((void *)rsp_ptr->rsp_payload.response.session_get_file_attr.path_ptr,
        sizeof(rsp_ptr->rsp_payload.response.session_get_file_attr.path_ptr),
        cnf_data_ptr->get_file_attr_cnf.access.file.path_type.path_buf,
        cnf_data_ptr->get_file_attr_cnf.access.file.path_type.path_len * sizeof(uint16));
    }
  }

  if(rsp_ptr->rsp_payload.response.session_get_file_attr.access_method_type ==
                                              (uint32) MMGSDI_BY_APP_ID_ACCESS)
  {
    rsp_ptr->rsp_payload.response.session_get_file_attr.aid_len =
      (uint32)cnf_data_ptr->get_file_attr_cnf.access.file.app_id.data_len;

    if(rsp_ptr->rsp_payload.response.session_get_file_attr.aid_len > 0 &&
       rsp_ptr->rsp_payload.response.session_get_file_attr.aid_len < MMGSDI_MAX_AID_LEN)
    {
      memscpy((void *)rsp_ptr->rsp_payload.response.session_get_file_attr.aid_ptr,
        sizeof(rsp_ptr->rsp_payload.response.session_get_file_attr.aid_ptr),
        cnf_data_ptr->get_file_attr_cnf.access.file.app_id.data_ptr,
        MMGSDI_MAX_AID_LEN);
    }
  }

  rsp_ptr->rsp_payload.response.session_get_file_attr.file_size =
  (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_size;

  memscpy((void *)(rsp_ptr->rsp_payload.response.session_get_file_attr.file_id),
  sizeof(rsp_ptr->rsp_payload.response.session_get_file_attr.file_id),
  cnf_data_ptr->get_file_attr_cnf.file_attrib.file_id,
  sizeof(cnf_data_ptr->get_file_attr_cnf.file_attrib.file_id));

  rsp_ptr->rsp_payload.response.session_get_file_attr.file_type =
  (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_type;

  switch(rsp_ptr->rsp_payload.response.session_get_file_attr.file_type)
    {
    case MMGSDI_LINEAR_FIXED_FILE:
         rsp_ptr->rsp_payload.response.session_get_file_attr.cf_increase_allowed = 0;

         rsp_ptr->rsp_payload.response.session_get_file_attr.num_of_rec =
         (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.num_of_rec;

         rsp_ptr->rsp_payload.response.session_get_file_attr.rec_len =
         (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.rec_len;

         rsp_ptr->rsp_payload.response.session_get_file_attr.file_invalid_flg =
         (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.file_status.file_invalidated;

         rsp_ptr->rsp_payload.response.session_get_file_attr.rw_allowed_when_inv =
         (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.file_status.read_write_when_invalidated;

         rsp_ptr->rsp_payload.response.session_get_file_attr.inc_cond =
         (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.file_security.increase.protection_method;

         rsp_ptr->rsp_payload.response.session_get_file_attr.inc_no_pin =
         (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.file_security.increase.num_protection_pin;

         rsp_ptr->rsp_payload.response.session_get_file_attr.read_cond =
         (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.file_security.read.protection_method;

         rsp_ptr->rsp_payload.response.session_get_file_attr.read_no_pin =
         (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.file_security.read.num_protection_pin;

         rsp_ptr->rsp_payload.response.session_get_file_attr.write_cond =
         (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.file_security.write.protection_method;

         rsp_ptr->rsp_payload.response.session_get_file_attr.write_no_pin =
         (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.file_security.write.num_protection_pin;

         rsp_ptr->rsp_payload.response.session_get_file_attr.inact_cond =
         (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.file_security.invalidate_deactivate.protection_method;

         rsp_ptr->rsp_payload.response.session_get_file_attr.inact_no_pin =
         (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.file_security.invalidate_deactivate.num_protection_pin;

         rsp_ptr->rsp_payload.response.session_get_file_attr.act_cond =
         (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.file_security.rehabilitate_activate.protection_method;

         rsp_ptr->rsp_payload.response.session_get_file_attr.act_no_pin =
         (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.linear_fixed_file.file_security.rehabilitate_activate.num_protection_pin;

         break;

    case  MMGSDI_TRANSPARENT_FILE:

          rsp_ptr->rsp_payload.response.session_get_file_attr.cf_increase_allowed = 0;

          rsp_ptr->rsp_payload.response.session_get_file_attr.num_of_rec = 0;

          rsp_ptr->rsp_payload.response.session_get_file_attr.rec_len = 0;

          rsp_ptr->rsp_payload.response.session_get_file_attr.file_invalid_flg =
      (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.transparent_file.file_status.file_invalidated;

      rsp_ptr->rsp_payload.response.session_get_file_attr.rw_allowed_when_inv =
      (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.transparent_file.file_status.read_write_when_invalidated;

      rsp_ptr->rsp_payload.response.session_get_file_attr.inc_cond =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.transparent_file.file_security.increase.protection_method;

      rsp_ptr->rsp_payload.response.session_get_file_attr.inc_no_pin =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.transparent_file.file_security.increase.num_protection_pin;

          rsp_ptr->rsp_payload.response.session_get_file_attr.read_cond =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.transparent_file.file_security.read.protection_method;

          rsp_ptr->rsp_payload.response.session_get_file_attr.read_no_pin =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.transparent_file.file_security.read.num_protection_pin;

          rsp_ptr->rsp_payload.response.session_get_file_attr.write_cond =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.transparent_file.file_security.write.protection_method;

          rsp_ptr->rsp_payload.response.session_get_file_attr.write_no_pin =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.transparent_file.file_security.write.num_protection_pin;

          rsp_ptr->rsp_payload.response.session_get_file_attr.inact_cond =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.transparent_file.file_security.invalidate_deactivate.protection_method;

          rsp_ptr->rsp_payload.response.session_get_file_attr.inact_no_pin =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.transparent_file.file_security.invalidate_deactivate.num_protection_pin;

          rsp_ptr->rsp_payload.response.session_get_file_attr.act_cond =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.transparent_file.file_security.rehabilitate_activate.protection_method;

          rsp_ptr->rsp_payload.response.session_get_file_attr.act_no_pin =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.transparent_file.file_security.rehabilitate_activate.num_protection_pin;

          break;

     case MMGSDI_CYCLIC_FILE:

          rsp_ptr->rsp_payload.response.session_get_file_attr.num_of_rec =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.num_of_rec;

          rsp_ptr->rsp_payload.response.session_get_file_attr.rec_len =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.rec_len;

          rsp_ptr->rsp_payload.response.session_get_file_attr.file_invalid_flg =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.file_status.file_invalidated;

          rsp_ptr->rsp_payload.response.session_get_file_attr.rw_allowed_when_inv =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.file_status.read_write_when_invalidated;

          rsp_ptr->rsp_payload.response.session_get_file_attr.cf_increase_allowed =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.increase_allowed;

          rsp_ptr->rsp_payload.response.session_get_file_attr.inc_cond =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.file_security.increase.protection_method;

          rsp_ptr->rsp_payload.response.session_get_file_attr.inc_no_pin =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.file_security.increase.num_protection_pin;

          rsp_ptr->rsp_payload.response.session_get_file_attr.read_cond =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.file_security.read.protection_method;

          rsp_ptr->rsp_payload.response.session_get_file_attr.read_no_pin =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.file_security.read.num_protection_pin;

          rsp_ptr->rsp_payload.response.session_get_file_attr.write_cond =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.file_security.write.protection_method;

          rsp_ptr->rsp_payload.response.session_get_file_attr.write_no_pin =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.file_security.write.num_protection_pin;

          rsp_ptr->rsp_payload.response.session_get_file_attr.inact_cond =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.file_security.invalidate_deactivate.protection_method;

          rsp_ptr->rsp_payload.response.session_get_file_attr.inact_no_pin =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.file_security.invalidate_deactivate.num_protection_pin;

          rsp_ptr->rsp_payload.response.session_get_file_attr.act_cond =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.file_security.rehabilitate_activate.protection_method;

          rsp_ptr->rsp_payload.response.session_get_file_attr.act_no_pin =
          (uint32)cnf_data_ptr->get_file_attr_cnf.file_attrib.file_info.cyclic_file.file_security.rehabilitate_activate.num_protection_pin;

          break;

     case MMGSDI_MASTER_FILE:
     case MMGSDI_DEDICATED_FILE:
       break;

     default:
       UIMDIAG_MSG_ERROR_0("Invalid file type passed");
       return UIMDIAG_ERROR;

  }
   return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_proc_get_file_attr_cnf*/

/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_PROC_READ_RECORD_CONF

DESCRIPTION
  Function to populate the response pointer with the confirmation data
  for read record and read transparent

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_proc_read_record_cnf(
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_cnf_type         *cnf_data_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_data_ptr, rsp_ptr);

  (void)memset((void*)&rsp_ptr->rsp_payload.response.session_read_record,
   0x00,sizeof(uimdiag_mmgsdi_rsp_session_read_record_pyld_type));

  rsp_ptr->rsp_payload.response.session_read_record.access_method_type =
    (uint32) cnf_data_ptr->read_cnf.access.access_method;

  if(rsp_ptr->rsp_payload.response.session_read_record.access_method_type ==
     (uint32) MMGSDI_EF_ENUM_ACCESS)
  {
    rsp_ptr->rsp_payload.response.session_read_record.ef_enum  =
     (uint32)uimdiag_util_convert_from_mmgsdi_items_enum(
       cnf_data_ptr->read_cnf.access.file.file_enum);
    rsp_ptr->rsp_payload.response.session_read_record.df_enum  = 0;
    rsp_ptr->rsp_payload.response.session_read_record.path_len = 0;
    rsp_ptr->rsp_payload.response.session_read_record.aid_len  = 0;
  }

  if(rsp_ptr->rsp_payload.response.session_read_record.access_method_type ==
     (uint32) MMGSDI_DF_ENUM_ACCESS)
  {
    rsp_ptr->rsp_payload.response.session_read_record.df_enum  =
    (uint32)cnf_data_ptr->read_cnf.access.file.df_enum;
  }

  if(rsp_ptr->rsp_payload.response.session_read_record.access_method_type ==
     (uint32) MMGSDI_BY_PATH_ACCESS)
  {
    rsp_ptr->rsp_payload.response.session_read_record.path_len  =
      (uint32)cnf_data_ptr->read_cnf.access.file.path_type.path_len;
    if(rsp_ptr->rsp_payload.response.session_read_record.path_len != 0)
    {
      memscpy((void *)(rsp_ptr->rsp_payload.response.session_read_record.path_ptr),
        sizeof(rsp_ptr->rsp_payload.response.session_read_record.path_ptr),
        cnf_data_ptr->read_cnf.access.file.path_type.path_buf,
        (sizeof(uint16)*rsp_ptr->rsp_payload.response.session_read_record.path_len));
    }
  }

  if(rsp_ptr->rsp_payload.response.session_read_record.access_method_type ==
     (uint32) MMGSDI_BY_APP_ID_ACCESS)
  {
    rsp_ptr->rsp_payload.response.session_read_record.aid_len =
      (uint32)cnf_data_ptr->read_cnf.access.file.app_id.data_len;
    if(rsp_ptr->rsp_payload.response.session_read_record.aid_len != 0)
    {
      memscpy((void *)(rsp_ptr->rsp_payload.response.session_read_record.aid_ptr),
        sizeof(rsp_ptr->rsp_payload.response.session_read_record.aid_ptr),
        cnf_data_ptr->read_cnf.access.file.app_id.data_ptr,
        rsp_ptr->rsp_payload.response.session_read_record.aid_len);
    }
  }

  rsp_ptr->rsp_payload.response.session_read_record.rec_num =
    (uint32)cnf_data_ptr->read_cnf.accessed_rec_num;

  rsp_ptr->rsp_payload.response.session_read_record.offset =
    (uint32)cnf_data_ptr->read_cnf.accessed_offset;

  rsp_ptr->rsp_payload.response.session_read_record.data_len =
    (uint32)cnf_data_ptr->read_cnf.read_data.data_len;

  if(rsp_ptr->rsp_payload.response.session_read_record.data_len != 0)
  {
    memscpy((void *)(rsp_ptr->rsp_payload.response.session_read_record.data_ptr),
      sizeof(rsp_ptr->rsp_payload.response.session_read_record.data_ptr),
      cnf_data_ptr->read_cnf.read_data.data_ptr,
      rsp_ptr->rsp_payload.response.session_read_record.data_len);
  }

  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_proc_read_record_cnf*/


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_PROC_WRITE_RECORD_CONF

DESCRIPTION
  Function to populate the response pointer with the confirmation data
  for write record and write transparent

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_proc_write_record_cnf(
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_cnf_type         *cnf_data_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_data_ptr, rsp_ptr);

  (void)memset((void*)&rsp_ptr->rsp_payload.response.session_write_record,
   0x00,sizeof(uimdiag_mmgsdi_rsp_session_write_record_pyld_type));

  rsp_ptr->rsp_payload.response.session_write_record.access_method_type =
  (uint32) cnf_data_ptr->write_cnf.access.access_method;

  if(rsp_ptr->rsp_payload.response.session_write_record.access_method_type ==
     (uint32) MMGSDI_EF_ENUM_ACCESS)
    {
     rsp_ptr->rsp_payload.response.session_write_record.ef_enum  =
     (uint32)uimdiag_util_convert_from_mmgsdi_items_enum(
       cnf_data_ptr->write_cnf.access.file.file_enum);
     rsp_ptr->rsp_payload.response.session_write_record.df_enum  = 0;
     rsp_ptr->rsp_payload.response.session_write_record.path_len = 0;
     rsp_ptr->rsp_payload.response.session_write_record.aid_len  = 0;
    }

  if(rsp_ptr->rsp_payload.response.session_write_record.access_method_type ==
     (uint32) MMGSDI_DF_ENUM_ACCESS)
    {
     rsp_ptr->rsp_payload.response.session_write_record.df_enum  =
     (uint32)cnf_data_ptr->write_cnf.access.file.df_enum;
    }

  if(rsp_ptr->rsp_payload.response.session_write_record.access_method_type ==
     (uint32) MMGSDI_BY_PATH_ACCESS)
    {
     rsp_ptr->rsp_payload.response.session_write_record.path_len  =
     (uint32)cnf_data_ptr->write_cnf.access.file.path_type.path_len;

     if(rsp_ptr->rsp_payload.response.session_write_record.path_len != 0)
      {
       memscpy((void *)(rsp_ptr->rsp_payload.response.session_write_record.path_ptr),
       sizeof(rsp_ptr->rsp_payload.response.session_write_record.path_ptr),
       cnf_data_ptr->write_cnf.access.file.path_type.path_buf,
       rsp_ptr->rsp_payload.response.session_write_record.path_len * sizeof(uint16));
      }
    }

  if(rsp_ptr->rsp_payload.response.session_write_record.access_method_type ==
     (uint32) MMGSDI_BY_APP_ID_ACCESS)
    {
     rsp_ptr->rsp_payload.response.session_write_record.aid_len =
     (uint32)cnf_data_ptr->write_cnf.access.file.app_id.data_len;

     if(rsp_ptr->rsp_payload.response.session_write_record.aid_len != 0)
      {
       memscpy((void *)(rsp_ptr->rsp_payload.response.session_write_record.aid_ptr),
       sizeof(rsp_ptr->rsp_payload.response.session_write_record.aid_ptr),
       cnf_data_ptr->write_cnf.access.file.app_id.data_ptr,
       rsp_ptr->rsp_payload.response.session_write_record.aid_len);
      }
    }

  rsp_ptr->rsp_payload.response.session_write_record.rec_num =
  (uint32)cnf_data_ptr->write_cnf.accessed_rec_num;

  rsp_ptr->rsp_payload.response.session_write_record.offset =
  (uint32)cnf_data_ptr->write_cnf.accessed_offset;

  rsp_ptr->rsp_payload.response.session_write_record.data_len =
  (uint32)cnf_data_ptr->write_cnf.written_data_len;

     return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_proc_write_record_cnf*/


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_DEACTIVATE_PROV_CNF

DESCRIPTION
  Function to populate the response pointer with the confirmation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_session_deactivate_prov_cnf(
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_cnf_type         *cnf_data_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_data_ptr, rsp_ptr);

  (void)memset(
    (void*)&rsp_ptr->rsp_payload.response.session_deact_prov,
      0x00, sizeof(uimdiag_mmgsdi_rsp_session_deact_prov_pyld_type));

  rsp_ptr->rsp_payload.response.session_deact_prov.session_id_low =
    UIMDIAG_LODWORD((cnf_data_ptr->session_deactivate_cnf.response_header.session_id));

  rsp_ptr->rsp_payload.response.session_deact_prov.session_id_high =
    UIMDIAG_HIDWORD(
      (cnf_data_ptr->session_deactivate_cnf.response_header.session_id));

  rsp_ptr->rsp_payload.response.session_deact_prov.app_type   =
    (uint32)cnf_data_ptr->session_deactivate_cnf.app_id.app_type;

  UIMDIAG_UTIL_RETURN_IF_OUT_OF_RANGE(
    cnf_data_ptr->session_deactivate_cnf.app_id.aid.data_len,
      0, MMGSDI_MAX_AID_LEN-1);

  rsp_ptr->rsp_payload.response.session_deact_prov.aid_length =
    (uint32)cnf_data_ptr->session_deactivate_cnf.app_id.aid.data_len;

  if(cnf_data_ptr->session_deactivate_cnf.app_id.aid.data_len <=
       MMGSDI_MAX_AID_LEN)
  {
    memscpy((void *)(rsp_ptr->rsp_payload.response.session_deact_prov.aid_ptr),
      sizeof(rsp_ptr->rsp_payload.response.session_deact_prov.aid_ptr),
      cnf_data_ptr->session_deactivate_cnf.app_id.aid.data_ptr,
      cnf_data_ptr->session_deactivate_cnf.app_id.aid.data_len);
  }

  UIMDIAG_UTIL_RETURN_IF_OUT_OF_RANGE(
    cnf_data_ptr->session_deactivate_cnf.app_id.label.data_len,
    0,MMGSDI_MAX_AID_LEN);

  rsp_ptr->rsp_payload.response.session_deact_prov.label_len   =
    (uint32)cnf_data_ptr->session_deactivate_cnf.app_id.label.data_len;

  if(cnf_data_ptr->session_deactivate_cnf.app_id.label.data_len <=
       MMGSDI_MAX_AID_LEN)
  {
    memscpy((void *)(rsp_ptr->rsp_payload.response.session_deact_prov.label_ptr),
      sizeof(rsp_ptr->rsp_payload.response.session_deact_prov.label_ptr),
      cnf_data_ptr->session_deactivate_cnf.app_id.label.data_ptr,
      cnf_data_ptr->session_deactivate_cnf.app_id.label.data_len);
  }

  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_session_deactivate_prov_cnf */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_GET_APP_CAP_CNF

DESCRIPTION
  Function to populate the response pointer with the confirmation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_session_get_app_cap_cnf(
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_session_app_capabilities_cnf_type         *session_app_cap_cnf_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(session_app_cap_cnf_ptr, rsp_ptr);

  (void)memset(
    (void*)&rsp_ptr->rsp_payload.response.session_get_app_cap,
      0x00, sizeof(uimdiag_mmgsdi_rsp_session_get_app_cap_pyld_type));

  rsp_ptr->rsp_payload.response.session_get_app_cap.fdn_enabled       =
    session_app_cap_cnf_ptr->fdn_enabled;

  rsp_ptr->rsp_payload.response.session_get_app_cap.bdn_enabled       =
    session_app_cap_cnf_ptr->bdn_enabled;

  rsp_ptr->rsp_payload.response.session_get_app_cap.acl_enabled       =
    session_app_cap_cnf_ptr->acl_enabled;

  rsp_ptr->rsp_payload.response.session_get_app_cap.imsi_invalidated  =
    session_app_cap_cnf_ptr->imsi_invalidated;


  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_session_get_app_cap_cnf */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_GET_INFO_SYNC_CNF

DESCRIPTION
  Function to populate the response pointer with the conformation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_session_get_info_sync_cnf(
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_session_get_info_type         *session_get_info_cnf_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(rsp_ptr, session_get_info_cnf_ptr);

  (void)memset((void*)&rsp_ptr->rsp_payload.response.session_get_info_sync_rsp,
               0x00,
               sizeof(uimdiag_mmgsdi_rsp_session_get_info_pyld_type));

  rsp_ptr->rsp_payload.response.session_get_info_sync_rsp.session_type    =
    (uint32) session_get_info_cnf_ptr->session_type;

  rsp_ptr->rsp_payload.response.session_get_info_sync_rsp.slot_id         =
    (uint32)session_get_info_cnf_ptr->slot_id;

  rsp_ptr->rsp_payload.response.session_get_info_sync_rsp.channel_id      =
       (uint32)session_get_info_cnf_ptr->channel_id;

  rsp_ptr->rsp_payload.response.session_get_info_sync_rsp.app_type        =
    (uint32)session_get_info_cnf_ptr->app_data.app_type;

  UIMDIAG_UTIL_RETURN_IF_OUT_OF_RANGE(
    session_get_info_cnf_ptr->app_data.aid.data_len,
    0,
    MMGSDI_MAX_AID_LEN);

  rsp_ptr->rsp_payload.response.session_get_info_sync_rsp.aid_length =
    session_get_info_cnf_ptr->app_data.aid.data_len;

  if(session_get_info_cnf_ptr->app_data.aid.data_len <= MMGSDI_MAX_AID_LEN)
  {
    memscpy((void *)(rsp_ptr->rsp_payload.response.session_get_info_sync_rsp.aid_ptr),
            sizeof(rsp_ptr->rsp_payload.response.session_get_info_sync_rsp.aid_ptr),
            session_get_info_cnf_ptr->app_data.aid.data_ptr,
            session_get_info_cnf_ptr->app_data.aid.data_len);
  }

  UIMDIAG_UTIL_RETURN_IF_OUT_OF_RANGE(
    session_get_info_cnf_ptr->app_data.label.data_len,
    0,
    MMGSDI_MAX_AID_LEN);

  rsp_ptr->rsp_payload.response.session_get_info_sync_rsp.label_len =
    session_get_info_cnf_ptr->app_data.label.data_len;

  if(session_get_info_cnf_ptr->app_data.label.data_len <=  MMGSDI_MAX_AID_LEN)
  {
    memscpy((void *)(rsp_ptr->rsp_payload.response.session_get_info.label_ptr),
            sizeof(rsp_ptr->rsp_payload.response.session_get_info.label_ptr),
            session_get_info_cnf_ptr->app_data.label.data_ptr,
            session_get_info_cnf_ptr->app_data.label.data_len);
  }

  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_session_get_info_sync_cnf */


/*===========================================================================

FUNCTION uimdiag_mmgsdi_sap_generic_cnf

DESCRIPTION
  Function to populate the response pointer with the conformation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_sap_generic_cnf(
  uimdiag_generic_rsp_type           *rsp_ptr,
  const mmgsdi_cnf_type              *cnf_ptr,
  mmgsdi_cnf_enum_type                cnf_type
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_ptr, rsp_ptr);

  switch(cnf_type)
  {
    default:
      UIMDIAG_MSG_ERROR_1("Unexpected confiramtion type: %x ", cnf_type);
      return UIMDIAG_ERROR;
  }
  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_sap_generic_cnf */


/*===========================================================================

FUNCTION uimdiag_mmgsdi_card_pdown_cnf

DESCRIPTION
  Function to populate the response pointer with the conformation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_card_pdown_cnf(
  uimdiag_generic_rsp_type           *rsp_ptr,
  const mmgsdi_cnf_type              *cnf_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_ptr, rsp_ptr);

  (void)memset((void*)&rsp_ptr->rsp_payload.response.card_pdown_rsp,
               0x00,sizeof(uimdiag_mmgsdi_card_pdown_rsp_type));

  rsp_ptr->rsp_payload.response.card_pdown_rsp.power_down_option =
      cnf_ptr->card_pdown_cnf.option;

  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_card_pdown_cnf */


/*===========================================================================

FUNCTION uimdiag_mmgsdi_card_pup_cnf

DESCRIPTION
  Function to populate the response pointer with the conformation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_card_pup_cnf(
  uimdiag_generic_rsp_type           *rsp_ptr,
  const mmgsdi_cnf_type              *cnf_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_ptr, rsp_ptr);

  (void)memset((void*)&rsp_ptr->rsp_payload.response.card_pup_rsp,
               0x00,sizeof(uimdiag_mmgsdi_card_pup_rsp_type));

  rsp_ptr->rsp_payload.response.card_pup_rsp.power_up_option =
      cnf_ptr->card_pup_cnf.option;

  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_card_pup_cnf */


/*===========================================================================

FUNCTION uimdiag_mmgsdi_activate_onchip_sim_cnf

DESCRIPTION
  Function to populate the response pointer with the conformation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_activate_onchip_sim_cnf(
  uimdiag_generic_rsp_type           *rsp_ptr,
  const mmgsdi_cnf_type              *cnf_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_ptr, rsp_ptr);

  (void)memset((void*)&rsp_ptr->rsp_payload.response.activate_onchip_sim_rsp,
               0x00,sizeof(uimdiag_mmgsdi_activate_onchip_sim_rsp_type));

  rsp_ptr->rsp_payload.response.activate_onchip_sim_rsp.onchip_state =
      cnf_ptr->act_onchip_cnf.onchip_sim_state.state;

  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_activate_onchip_sim_cnf */


/*===========================================================================

FUNCTION uimdiag_mmgsdi_send_apdu_cnf

DESCRIPTION
  Function to populate the response pointer with the conformation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_send_apdu_cnf(
  uimdiag_generic_rsp_type           *rsp_ptr,
  const mmgsdi_cnf_type              *cnf_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_ptr, rsp_ptr);

  (void)memset((void*)&rsp_ptr->rsp_payload.response.send_apdu_rsp,
               0x00,sizeof(uimdiag_mmgsdi_send_apdu_rsp_type));

  rsp_ptr->rsp_payload.response.send_apdu_rsp.max_size =
      cnf_ptr->send_apdu_cnf.max_size;
  rsp_ptr->rsp_payload.response.send_apdu_rsp.channel_id =
      cnf_ptr->send_apdu_cnf.channel_id;
  rsp_ptr->rsp_payload.response.send_apdu_rsp.implicit_sw1 =
      cnf_ptr->send_apdu_cnf.implicit_get_rsp_sw1;
  rsp_ptr->rsp_payload.response.send_apdu_rsp.implicit_sw2 =
      cnf_ptr->send_apdu_cnf.implicit_get_rsp_sw2;
  rsp_ptr->rsp_payload.response.send_apdu_rsp.apdu_data_length =
      cnf_ptr->send_apdu_cnf.apdu_data.data_len;
  if(cnf_ptr->send_apdu_cnf.apdu_data.data_len > UIMDIAG_MAX_DATA_BLOCK_LEN)
  {
    UIMDIAG_MSG_ERROR_0("Rsp len is greater than expected value.. rsp data will be truncated");
  }
  memscpy((void*)rsp_ptr->rsp_payload.response.send_apdu_rsp.apdu_data_ptr,
         sizeof(rsp_ptr->rsp_payload.response.send_apdu_rsp.apdu_data_ptr),
         (void*)cnf_ptr->send_apdu_cnf.apdu_data.data_ptr,
         MIN(cnf_ptr->send_apdu_cnf.apdu_data.data_len, UIMDIAG_MAX_DATA_BLOCK_LEN));

  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_send_apdu_cnf */


/*===========================================================================
FUNCTION UIMDIAG_MMGSDI_GET_ATR_CNF

DESCRIPTION
  Function to populate the response pointer with the confirmation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_get_atr_cnf(
  uimdiag_generic_rsp_type           *rsp_ptr,
  const mmgsdi_cnf_type              *cnf_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_ptr, rsp_ptr);

  (void)memset((void*)&rsp_ptr->rsp_payload.response.get_atr_rsp,
               0x00,
               sizeof(uimdiag_mmgsdi_get_atr_rsp_type));

  rsp_ptr->rsp_payload.response.get_atr_rsp.atr_data_len =
           cnf_ptr->get_atr_cnf.atr_data.data_len;

  if(cnf_ptr->get_atr_cnf.atr_data.data_len <= sizeof(rsp_ptr->rsp_payload.response.get_atr_rsp.atr_data))
  {
    memscpy(rsp_ptr->rsp_payload.response.get_atr_rsp.atr_data,
            sizeof(rsp_ptr->rsp_payload.response.get_atr_rsp.atr_data),
            cnf_ptr->get_atr_cnf.atr_data.data_ptr,
            cnf_ptr->get_atr_cnf.atr_data.data_len);

    return UIMDIAG_SUCCESS;
  }
  return UIMDIAG_ERROR;
} /* uimdiag_mmgsdi_get_atr_cnf */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_COPY_APP_INFO_EXT

DESCRIPTION
  This function is called to copy app information to rsp_ptr

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void uimdiag_mmgsdi_copy_app_info_ext(
  uimdiag_mmgsdi_app_info_ext_type   *uimdiag_aid_ptr,
  const mmgsdi_app_info_type         *mmgsdi_aid_ptr,
  uint8                               start_index,
  uint8                               no_of_apps
)
{
  uint8 aid_cnf_index = 0;
  
  if((uimdiag_aid_ptr == NULL) ||
     (mmgsdi_aid_ptr == NULL))
  {
    return;
  }
 
  for (aid_cnf_index = 0; aid_cnf_index < no_of_apps; aid_cnf_index++)
  {  
    uint8 index = 0;  
    index = start_index + aid_cnf_index;
    
    uimdiag_aid_ptr[index].slot_id =
         (uint8)mmgsdi_aid_ptr[aid_cnf_index].slot;

    uimdiag_aid_ptr[index].pin1_status =
         (uint8)mmgsdi_aid_ptr[aid_cnf_index].pin1.status;
    
    uimdiag_aid_ptr[index].pin1_replacement_status =
         (uint8)mmgsdi_aid_ptr[aid_cnf_index].pin1.pin_replacement;
   
    uimdiag_aid_ptr[index].pin1_num_of_retries =
         (uint8)mmgsdi_aid_ptr[aid_cnf_index].pin1.num_retries;
    
    uimdiag_aid_ptr[index].pin1_num_of_unblock_retries =
         (uint8)mmgsdi_aid_ptr[aid_cnf_index].pin1.num_unblock_retries;
    
    uimdiag_aid_ptr[index].pin1_num_of_retries_valid =
         (uint8)mmgsdi_aid_ptr[aid_cnf_index].pin1.valid_num_retries;
   
    uimdiag_aid_ptr[index].pin1_num_of_unblock_retries_valid =
         (uint8)mmgsdi_aid_ptr[aid_cnf_index].pin1.valid_num_unblock_retries;
   
    uimdiag_aid_ptr[index].pin2_status =
         (uint8)mmgsdi_aid_ptr[aid_cnf_index].pin2.status;
   
    uimdiag_aid_ptr[index].pin2_num_of_retries =
         (uint8)mmgsdi_aid_ptr[aid_cnf_index].pin2.num_retries;

    uimdiag_aid_ptr[index].pin2_num_of_unblock_retries =
         (uint8)mmgsdi_aid_ptr[aid_cnf_index].pin2.num_unblock_retries;
	  
    uimdiag_aid_ptr[index].pin2_num_of_retries_valid =
         (uint8)mmgsdi_aid_ptr[aid_cnf_index].pin2.valid_num_retries;
    
    uimdiag_aid_ptr[index].pin2_num_of_unblock_retries_valid =
         (uint8)mmgsdi_aid_ptr[aid_cnf_index].pin2.valid_num_unblock_retries;
    
    uimdiag_aid_ptr[index].upin_status =
         (uint8)mmgsdi_aid_ptr[aid_cnf_index].universal_pin.status;
    
    uimdiag_aid_ptr[index].upin_num_of_retries =
         (uint8)mmgsdi_aid_ptr[aid_cnf_index].universal_pin.num_retries;
    
    uimdiag_aid_ptr[index].upin_num_of_unblock_retries =
         (uint8)mmgsdi_aid_ptr[aid_cnf_index].universal_pin.num_unblock_retries;
    
    uimdiag_aid_ptr[index].upin_num_of_retries_valid =
         (uint8)mmgsdi_aid_ptr[aid_cnf_index].universal_pin.valid_num_retries;
   
    uimdiag_aid_ptr[index].upin_num_of_unblock_retries_valid =
         (uint8)mmgsdi_aid_ptr[aid_cnf_index].universal_pin.valid_num_unblock_retries;
    
    uimdiag_aid_ptr[index].app_type  =
         (uint8)mmgsdi_aid_ptr[aid_cnf_index].app_data.app_type;

    uimdiag_aid_ptr[index].aid_len =
         (uint8)mmgsdi_aid_ptr[aid_cnf_index].app_data.aid.data_len;
    
    memscpy((void *) uimdiag_aid_ptr[index].app_id_ptr,
            sizeof(uimdiag_aid_ptr[index].app_id_ptr),
            (void *) mmgsdi_aid_ptr[aid_cnf_index].app_data.aid.data_ptr,
            mmgsdi_aid_ptr[aid_cnf_index].app_data.aid.data_len);
 
    uimdiag_aid_ptr[index].label_len =
         mmgsdi_aid_ptr[aid_cnf_index].app_data.label.data_len;
 
    memscpy((void *) uimdiag_aid_ptr[index].label_ptr,
            sizeof(uimdiag_aid_ptr[index].label_ptr),
            (void *) mmgsdi_aid_ptr[aid_cnf_index].app_data.label.data_ptr,
            mmgsdi_aid_ptr[aid_cnf_index].app_data.label.data_len);
   
    uimdiag_aid_ptr[index].dir_index =
         mmgsdi_aid_ptr[aid_cnf_index].dir_index;
  }
}/* uimdiag_mmgsdi_copy_app_info_ext */


/*===========================================================================

FUNCTION uimdiag_mmgsdi_get_all_avail_apps_ext_cnf

DESCRIPTION
  Function to populate the response pointer with the conformation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_get_all_avail_apps_ext_cnf(
  uimdiag_generic_rsp_type                      *rsp_ptr,
  const mmgsdi_available_apps_type              *get_all_available_apps_cnf_ptr
)
{
  uimdiag_mmgsdi_app_info_ext_type  *uimdiag_aid_ptr     = NULL;
  const mmgsdi_app_info_type        *mmgsdi_aid_ptr      = NULL;

  UIMDIAG_UTIL_RETURN_IF_NULL_2(get_all_available_apps_cnf_ptr, rsp_ptr);

  uimdiag_aid_ptr =  rsp_ptr->rsp_payload.response.get_all_available_apps_ext_rsp.get_all_available_app_info_ext;

  if (get_all_available_apps_cnf_ptr->card1_num_app >= MMGSDI_MAX_APP_INFO || 
      get_all_available_apps_cnf_ptr->card2_num_app >= MMGSDI_MAX_APP_INFO ||
      get_all_available_apps_cnf_ptr->card3_num_app >= MMGSDI_MAX_APP_INFO ||
      get_all_available_apps_cnf_ptr->card1_num_app < 0 || 
      get_all_available_apps_cnf_ptr->card2_num_app < 0 || 
      get_all_available_apps_cnf_ptr->card3_num_app < 0)
  {  
    return UIMDIAG_INCORRECT_PARAM;
  }
  
  mmgsdi_aid_ptr  =  get_all_available_apps_cnf_ptr->card1_app_info;

  /* Copy app info from card1 */
  (void)uimdiag_mmgsdi_copy_app_info_ext(uimdiag_aid_ptr,
                                         mmgsdi_aid_ptr,
                                         0,
                                         (uint8)get_all_available_apps_cnf_ptr->card1_num_app);

   mmgsdi_aid_ptr = get_all_available_apps_cnf_ptr->card2_app_info;

   /* Copy app info from card2 */
  (void)uimdiag_mmgsdi_copy_app_info_ext(uimdiag_aid_ptr,
                                         mmgsdi_aid_ptr,
                                         (uint8) get_all_available_apps_cnf_ptr->card1_num_app,
                                         (uint8) get_all_available_apps_cnf_ptr->card2_num_app);
 
  mmgsdi_aid_ptr  =  get_all_available_apps_cnf_ptr->card3_app_info;

  /* Copy app info from card3 */
  (void)uimdiag_mmgsdi_copy_app_info_ext(uimdiag_aid_ptr,
                                         mmgsdi_aid_ptr,
                                         (uint8)(get_all_available_apps_cnf_ptr->card1_num_app +
                                         get_all_available_apps_cnf_ptr->card2_num_app),
                                         (uint8)get_all_available_apps_cnf_ptr->card3_num_app);

  rsp_ptr->rsp_payload.response.get_all_available_apps_ext_rsp.total_no_of_apps = 
           get_all_available_apps_cnf_ptr->card1_num_app +
           get_all_available_apps_cnf_ptr->card2_num_app +
           get_all_available_apps_cnf_ptr->card3_num_app;

  return UIMDIAG_SUCCESS;
}/* uimdiag_mmgsdi_get_all_avail_apps_ext_cnf */


#ifdef FEATURE_UIMDIAG_INTERNAL_API
/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_REFRESH_CNF

DESCRIPTION
  Function to populate the response pointer with the confirmation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_refresh_cnf(
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_cnf_type         *cnf_data_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_data_ptr, rsp_ptr);

  (void)memset(
    (void*)&rsp_ptr->rsp_payload.response.session_refresh,
      0x00, sizeof(uimdiag_mmgsdi_sessionid_type));

  rsp_ptr->rsp_payload.response.session_refresh.session_id_low =
    UIMDIAG_LODWORD((cnf_data_ptr->refresh_cnf.response_header.session_id));

  rsp_ptr->rsp_payload.response.session_refresh.session_id_high =
    UIMDIAG_HIDWORD(
      (cnf_data_ptr->refresh_cnf.response_header.session_id));
  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_refresh_cnf */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_DISABLE_SERVICE_CNF

DESCRIPTION
  Function to populate the response pointer with the conformation data for
  disable service

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_session_disable_service_cnf(
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_cnf_type         *cnf_data_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_data_ptr, rsp_ptr);

  (void)memset(
    (void*)&rsp_ptr->rsp_payload.response.session_disable_srv,
      0x00, sizeof(uimdiag_mmgsdi_rsp_session_srv_pyld_type));

  rsp_ptr->rsp_payload.response.session_disable_srv.service_type =
    (uint32)cnf_data_ptr->session_disable_service_cnf.service_type;

  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_session_disable_service_cnf */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_ENABLE_SERVICE_CNF

DESCRIPTION
  Function to populate the response pointer with the conformation data for
  enable service

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_session_enable_service_cnf(
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_cnf_type         *cnf_data_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_data_ptr, rsp_ptr);

  (void)memset(
    (void*)&rsp_ptr->rsp_payload.response.session_enable_srv,
      0x00, sizeof(uimdiag_mmgsdi_rsp_session_srv_pyld_type));

  rsp_ptr->rsp_payload.response.session_enable_srv.service_type =
    (uint32)cnf_data_ptr->session_enable_service_cnf.service_type;

  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_session_enable_service_cnf */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SRV_AVAIL_CNF

DESCRIPTION
  Function to populate the response pointer with the conformation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_srv_avail_cnf(
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_cnf_type         *cnf_data_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_data_ptr, rsp_ptr);

  (void)memset(
    (void*)&rsp_ptr->rsp_payload.response.session_is_srv_avail,
      0x00, sizeof(uimdiag_mmgsdi_rsp_session_is_srv_avail_pyld_type));

  rsp_ptr->rsp_payload.response.session_is_srv_avail.srv_available =
    (uint32)cnf_data_ptr->srv_available_cnf.srv_available;

  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_srv_avail_cnf */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_PROV_APP_INIT_CMPL_CNF

DESCRIPTION
  Function to populate the response pointer with the confirmation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_prov_app_init_cmpl_cnf(
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_cnf_type         *cnf_data_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_data_ptr, rsp_ptr);

  (void)memset(
    (void*)&rsp_ptr->rsp_payload.response.session_prov_app_init_complete_rsp,
      0x00, sizeof(uimdiag_mmgsdi_sessionid_type));

  rsp_ptr->rsp_payload.response.session_prov_app_init_complete_rsp.session_id_low =
    UIMDIAG_LODWORD(
      (cnf_data_ptr->session_prov_app_init_complete_cnf.response_header.session_id));

  rsp_ptr->rsp_payload.response.session_prov_app_init_complete_rsp.session_id_high =
    UIMDIAG_HIDWORD(
      (cnf_data_ptr->session_prov_app_init_complete_cnf.response_header.session_id));
  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_prov_app_init_cmpl_cnf */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_READ_PRL_CNF

DESCRIPTION
  Function to populate the response pointer with the conformation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_read_prl_cnf(
  uimdiag_generic_rsp_type      *rsp_ptr,
  const mmgsdi_cnf_type         *cnf_data_ptr
)
{
  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_data_ptr, rsp_ptr);

  (void)memset(
    (void*)&rsp_ptr->rsp_payload.response.session_read_prl_rsp,
      0x00, sizeof(uimdiag_mmgsdi_session_read_prl_rsp_type));

  rsp_ptr->rsp_payload.response.session_read_prl_rsp.prl_version =
    (uint32)cnf_data_ptr->session_read_prl_cnf.prl_version;

  rsp_ptr->rsp_payload.response.session_read_prl_rsp.size =
    (uint32)cnf_data_ptr->session_read_prl_cnf.size;

  rsp_ptr->rsp_payload.response.session_read_prl_rsp.valid =
    (uint32)cnf_data_ptr->session_read_prl_cnf.valid;

  memscpy((rsp_ptr->rsp_payload.response.session_read_prl_rsp.roaming_list),
    sizeof(rsp_ptr->rsp_payload.response.session_read_prl_rsp.roaming_list),
    (uint8*)cnf_data_ptr->session_read_prl_cnf.roaming_list_ptr,
    MIN((uint32)cnf_data_ptr->session_read_prl_cnf.size,
    UIMDIAG_MAX_ROAMING_LIST_LENGTH));

  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_read_prl_cnf */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_COPY_APP_INFO

DESCRIPTION
  This function is called to copy app information to rsp_ptr

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void uimdiag_mmgsdi_copy_app_info(
  uimdiag_mmgsdi_app_info_type   *temp_aid_info_ptr,
  const mmgsdi_app_info_type     *temp_aid_cnf_data_ptr,
  uint32                          aid_info_index,
  uint32                          no_of_apps
)
{
  uint32 aid_cnf_index = 0;

  if((temp_aid_info_ptr != NULL) &&
     (temp_aid_cnf_data_ptr != NULL))
  {

    for( ; aid_cnf_index < no_of_apps;
         aid_info_index++, aid_cnf_index++)
    {
      temp_aid_info_ptr[aid_info_index].cached_status =
           temp_aid_cnf_data_ptr[aid_cnf_index].cached;

      temp_aid_info_ptr[aid_info_index].slot_id =
           temp_aid_cnf_data_ptr[aid_cnf_index].slot;

      temp_aid_info_ptr[aid_info_index].pin1_id =
           temp_aid_cnf_data_ptr[aid_cnf_index].pin1.pin_id;

      temp_aid_info_ptr[aid_info_index].pin1_status =
           temp_aid_cnf_data_ptr[aid_cnf_index].pin1.status;

      temp_aid_info_ptr[aid_info_index].pin1_replacement_status =
           temp_aid_cnf_data_ptr[aid_cnf_index].pin1.pin_replacement;

      temp_aid_info_ptr[aid_info_index].pin1_num_of_retries =
           temp_aid_cnf_data_ptr[aid_cnf_index].pin1.num_retries;

      temp_aid_info_ptr[aid_info_index].pin1_num_of_unblock_retires =
           temp_aid_cnf_data_ptr[aid_cnf_index].pin1.num_unblock_retries;

      temp_aid_info_ptr[aid_info_index].pin2_id =
           temp_aid_cnf_data_ptr[aid_cnf_index].pin2.pin_id;

      temp_aid_info_ptr[aid_info_index].pin2_status =
           temp_aid_cnf_data_ptr[aid_cnf_index].pin2.status;

      temp_aid_info_ptr[aid_info_index].pin2_replacement_status =
           temp_aid_cnf_data_ptr[aid_cnf_index].pin2.pin_replacement;

      temp_aid_info_ptr[aid_info_index].pin2_num_of_retries =
           temp_aid_cnf_data_ptr[aid_cnf_index].pin2.num_retries;

      temp_aid_info_ptr[aid_info_index].pin2_num_of_unblock_retires =
           temp_aid_cnf_data_ptr[aid_cnf_index].pin2.num_unblock_retries;

      temp_aid_info_ptr[aid_info_index].upin_id =
           temp_aid_cnf_data_ptr[aid_cnf_index].universal_pin.pin_id;

      temp_aid_info_ptr[aid_info_index].upin_status =
           temp_aid_cnf_data_ptr[aid_cnf_index].universal_pin.status;

      temp_aid_info_ptr[aid_info_index].upin_replacement_status =
           temp_aid_cnf_data_ptr[aid_cnf_index].universal_pin.pin_replacement;

      temp_aid_info_ptr[aid_info_index].upin_num_of_retries =
           temp_aid_cnf_data_ptr[aid_cnf_index].universal_pin.num_retries;

      temp_aid_info_ptr[aid_info_index].upin_num_of_unblock_retires =
           temp_aid_cnf_data_ptr[aid_cnf_index].universal_pin.num_unblock_retries;

      temp_aid_info_ptr[aid_info_index].app_type  =
           temp_aid_cnf_data_ptr[aid_cnf_index].app_data.app_type;

      temp_aid_info_ptr[aid_info_index].aid_len =
           temp_aid_cnf_data_ptr[aid_cnf_index].app_data.aid.data_len;

      memscpy((void *) temp_aid_info_ptr[aid_info_index].app_id_ptr,
              sizeof(temp_aid_info_ptr[aid_info_index].app_id_ptr),
             (void *) temp_aid_cnf_data_ptr[aid_cnf_index].app_data.aid.data_ptr,
              temp_aid_cnf_data_ptr[aid_cnf_index].app_data.aid.data_len);

      temp_aid_info_ptr[aid_info_index].label_len =
           temp_aid_cnf_data_ptr[aid_cnf_index].app_data.label.data_len;

      memscpy((void *) temp_aid_info_ptr[aid_info_index].label_ptr,
             sizeof(temp_aid_info_ptr[aid_info_index].label_ptr),
             (void *) temp_aid_cnf_data_ptr[aid_cnf_index].app_data.label.data_ptr,
             temp_aid_cnf_data_ptr[aid_cnf_index].app_data.label.data_len);

      temp_aid_info_ptr[aid_info_index].hkey_id =
           temp_aid_cnf_data_ptr[aid_cnf_index].hiddenkey.pin_id;

      temp_aid_info_ptr[aid_info_index].hkey_status =
           temp_aid_cnf_data_ptr[aid_cnf_index].hiddenkey.status;

      temp_aid_info_ptr[aid_info_index].hkey_replacement_status =
           temp_aid_cnf_data_ptr[aid_cnf_index].hiddenkey.pin_replacement;

      temp_aid_info_ptr[aid_info_index].hkey_num_of_retries =
           temp_aid_cnf_data_ptr[aid_cnf_index].hiddenkey.num_retries;

      temp_aid_info_ptr[aid_info_index].hkey_num_of_unblock_retires =
           temp_aid_cnf_data_ptr[aid_cnf_index].hiddenkey.num_unblock_retries;

      temp_aid_info_ptr[aid_info_index].dir_index =
           temp_aid_cnf_data_ptr[aid_cnf_index].dir_index;
    }
  }
}/* uimdiag_mmgsdi_copy_app_info */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_GET_ALL_AVAILABLE_APPS_CNF

DESCRIPTION
  Function to populate the response pointer with the conformation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_get_all_available_apps_cnf(
  uimdiag_generic_rsp_type                      *rsp_ptr,
  const mmgsdi_get_all_available_apps_cnf_type  *get_all_available_apps_cnf_ptr
)
{
  int32                         total_aids            = 0;
  uimdiag_mmgsdi_app_info_type *temp_aid_info_ptr     = NULL;
  const mmgsdi_app_info_type   *temp_aid_cnf_data_ptr = NULL;

  UIMDIAG_UTIL_RETURN_IF_NULL_2(get_all_available_apps_cnf_ptr, rsp_ptr);

  total_aids = get_all_available_apps_cnf_ptr->card1_num_app +
               get_all_available_apps_cnf_ptr->card2_num_app +
               get_all_available_apps_cnf_ptr->card3_num_app;

  temp_aid_info_ptr =  rsp_ptr->rsp_payload.response.get_all_available_apps_rsp.get_all_available_app_info;
  temp_aid_cnf_data_ptr = get_all_available_apps_cnf_ptr->card1_app_info;

  /* Copy app info from card1 */

  (void)uimdiag_mmgsdi_copy_app_info(temp_aid_info_ptr,
                                     temp_aid_cnf_data_ptr,
                                     0,
                                     (uint32)get_all_available_apps_cnf_ptr->card1_num_app);

   temp_aid_cnf_data_ptr = get_all_available_apps_cnf_ptr->card2_app_info;

   /* Copy app info from card2 */

  (void)uimdiag_mmgsdi_copy_app_info(temp_aid_info_ptr,
                                     temp_aid_cnf_data_ptr,
                                     (uint32) get_all_available_apps_cnf_ptr->card1_num_app,
                                     (uint32) get_all_available_apps_cnf_ptr->card2_num_app);
   /* Copy app info from card3 */

  temp_aid_cnf_data_ptr = get_all_available_apps_cnf_ptr->card3_app_info;

  (void)uimdiag_mmgsdi_copy_app_info(temp_aid_info_ptr,
                                     temp_aid_cnf_data_ptr,
                                    (uint32)(get_all_available_apps_cnf_ptr->card1_num_app +
                                     get_all_available_apps_cnf_ptr->card2_num_app),
                                    (uint32)get_all_available_apps_cnf_ptr->card3_num_app);

  rsp_ptr->rsp_payload.response.get_all_available_apps_rsp.total_no_of_apps =
    total_aids;

  return UIMDIAG_SUCCESS;
}/* uimdiag_mmgsdi_get_all_available_apps_cnf */


/*===========================================================================

FUNCTION uimdiag_mmgsdi_get_operator_name_cnf

DESCRIPTION
  Function to populate the response pointer with the conformation data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
static uimdiag_return_enum_type uimdiag_mmgsdi_get_operator_name_cnf(
  uimdiag_generic_rsp_type           *rsp_ptr,
  const mmgsdi_cnf_type              *cnf_ptr
)
{
  uimdiag_mmgsdi_rsp_session_get_operator_name_type *get_operator_name_rsp_ptr = NULL;
  uint8 *offsetting_rsp_ptr = NULL;
  uint32 index = 0;
  uint32 total_len = 0;
  uint8 *temp_total_name_len_ptr = NULL;
  uint8  temp_data = 0;

  UIMDIAG_UTIL_RETURN_IF_NULL_2(cnf_ptr, rsp_ptr);

   /***
         struct of response
         uint8    encoding_info;
         uint32   operator_name_len;
         uint8    operator_name[1];
         uint8    lang_ind;
         uint8    spn_display_bit;
         uint8    rplmn_display_bit;
         uint8    roaming_status;
         uint32   no_plmn_id;
         uint32   total_buff_size_of_operator_name_info;

         Dynamic part encoding
         uint8    plmn_id[3]
         uint8    rat_type
         uint8    long_name_ci;
         uint8    long_name_spare_bytes;
         uint8    long_name_encoding;
         uint32   long name len
         uint8    *long name
         uint8    short_name_ci;
         uint8    short_name_spare_bytes;
         uint8    short_name_encoding;
         uint32   short name len
         uint8    *short name
      */

  (void)memset((void*)&rsp_ptr->rsp_payload.response.session_get_operator_name,
               0x00,sizeof(uimdiag_mmgsdi_rsp_session_get_operator_name_type));

  get_operator_name_rsp_ptr = &rsp_ptr->rsp_payload.response.session_get_operator_name;

  get_operator_name_rsp_ptr->encoding_info =  cnf_ptr->session_get_operator_name_cnf.spn.eons_encoding;
  get_operator_name_rsp_ptr->operator_name_len =  cnf_ptr->session_get_operator_name_cnf.spn.eons_data.data_len;
  offsetting_rsp_ptr = (uint8 *)(get_operator_name_rsp_ptr->operator_name);

  /* operator name */
  if (cnf_ptr->session_get_operator_name_cnf.spn.eons_data.data_len > 0)
  {
    memscpy((void*)offsetting_rsp_ptr, cnf_ptr->session_get_operator_name_cnf.spn.eons_data.data_len,
                  (void *)cnf_ptr->session_get_operator_name_cnf.spn.eons_data.data_ptr,
                   cnf_ptr->session_get_operator_name_cnf.spn.eons_data.data_len);
    offsetting_rsp_ptr += cnf_ptr->session_get_operator_name_cnf.spn.eons_data.data_len;
  }

  /* Language indicator */
  memscpy((void *)offsetting_rsp_ptr, sizeof(uint8), (void *)&cnf_ptr->session_get_operator_name_cnf.spn_lang_ind,
                 sizeof(uint8));
  offsetting_rsp_ptr += sizeof(uint8);

  /* SPN display bit */
  memscpy((void *)offsetting_rsp_ptr, sizeof(uint8), (void *)&cnf_ptr->session_get_operator_name_cnf.spn_display_bit,
                 sizeof(uint8));
  offsetting_rsp_ptr += sizeof(uint8);

  /* RPLMN display bit */
  memscpy((void *)offsetting_rsp_ptr, sizeof(uint8), (void *)&cnf_ptr->session_get_operator_name_cnf.rplmn_display_bit,
                 sizeof(uint8));
  offsetting_rsp_ptr += sizeof(uint8);

  /* Roaming status */
  memscpy((void *)offsetting_rsp_ptr, sizeof(uint8), (void *)&cnf_ptr->session_get_operator_name_cnf.roaming_status,
                 sizeof(uint8));
  offsetting_rsp_ptr += sizeof(uint8);

  /* no of plmn ids name */
  memscpy((void *)offsetting_rsp_ptr, sizeof(uint32), (void *)&cnf_ptr->session_get_operator_name_cnf.plmn_list.num_of_plmn_ids,
                 sizeof(uint32));
  offsetting_rsp_ptr += sizeof(uint32);

  temp_total_name_len_ptr = offsetting_rsp_ptr;
  /* for now skip 4 bytes for storing total operator name info len */
  /* this will be copied at the end */
  offsetting_rsp_ptr += sizeof(uint32);

  for(index =0; index < cnf_ptr->session_get_operator_name_cnf.plmn_list.num_of_plmn_ids; index ++)
  {
    /* PLMN ID */
    memscpy((void *)offsetting_rsp_ptr,
      sizeof(uint8) * 3,
      (void *)cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_id.plmn_id_val,
      sizeof(uint8) * 3);
    offsetting_rsp_ptr += sizeof(uint8) * 3;
    total_len += sizeof(uint8) * 3;

    /* RAT Type */

    /* copy the enum value into uint8 variable and then do memscpy to make it
       endian independent */
    temp_data =
      cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_id.rat;

    memscpy((void *)offsetting_rsp_ptr,
      sizeof(uint8),
      (void *)&temp_data,
      sizeof(uint8));
    offsetting_rsp_ptr += sizeof(uint8);
    total_len += sizeof(uint8);

    /* Long name CI */
    memscpy((void *)offsetting_rsp_ptr,
      sizeof(uint8),
      (void *)&cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_long_name.plmn_name_ci,
      sizeof(uint8));
    offsetting_rsp_ptr += sizeof(uint8);
    total_len += sizeof(uint8);

    /*Long name Spare Bits */
    memscpy((void *)offsetting_rsp_ptr,
      sizeof(uint8),
      (void *)&cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_long_name.plmn_name_spare_bits,
      sizeof(uint8));
    offsetting_rsp_ptr += sizeof(uint8);
    total_len += sizeof(uint8);

    /* Long name encoding */

    /* copy the enum value into uint8 variable and then do memscpy to make it
       endian independent */
    temp_data =
      cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_long_name.plmn_name.eons_encoding;

    memscpy((void *)offsetting_rsp_ptr,
      sizeof(uint8),
      (void *)&temp_data,
      sizeof(uint8));
    offsetting_rsp_ptr += sizeof(uint8);
    total_len += sizeof(uint8);

    /* Long name len */
    memscpy((void *)offsetting_rsp_ptr,
      sizeof(uint32),
      (void *)&cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_long_name.plmn_name.eons_data.data_len,
      sizeof(uint32));
    offsetting_rsp_ptr += sizeof(uint32);
    total_len += sizeof(uint32);

    /* Long name */
    memscpy((void *)offsetting_rsp_ptr,
      cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_long_name.plmn_name.eons_data.data_len,
      (void *)cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_long_name.plmn_name.eons_data.data_ptr,
      cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_long_name.plmn_name.eons_data.data_len);
    offsetting_rsp_ptr += cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_long_name.plmn_name.eons_data.data_len;
    total_len += cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_long_name.plmn_name.eons_data.data_len;

    /* Short name CI */
    memscpy((void *)offsetting_rsp_ptr,
      sizeof(uint8),
      (void *)&cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_short_name.plmn_name_ci,
      sizeof(uint8));
    offsetting_rsp_ptr += sizeof(uint8);
    total_len += sizeof(uint8);

    /*Short name Spare Bits */
    memscpy((void *)offsetting_rsp_ptr,
      sizeof(uint8),
      (void *)&cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_short_name.plmn_name_spare_bits,
      sizeof(uint8));
    offsetting_rsp_ptr += sizeof(uint8);
    total_len += sizeof(uint8);

    /* Short name encoding */

    /* copy the enum value into uint8 variable and then do memscpy to make it
       endian independent */
    temp_data =
      cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_short_name.plmn_name.eons_encoding;

    memscpy((void *)offsetting_rsp_ptr,
      sizeof(uint8),
      (void *)&temp_data,
      sizeof(uint8));
    offsetting_rsp_ptr += sizeof(uint8);
    total_len += sizeof(uint8);

    /* Short name len */
    memscpy((void *)offsetting_rsp_ptr,
      sizeof(uint32),
      (void *)&cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_short_name.plmn_name.eons_data.data_len,
      sizeof(uint32));
    offsetting_rsp_ptr += sizeof(uint32);
    total_len += sizeof(uint32);


    /* Short name */
    memscpy((void *)offsetting_rsp_ptr,
      cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_short_name.plmn_name.eons_data.data_len,
      (void *)cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_short_name.plmn_name.eons_data.data_ptr,
      cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_short_name.plmn_name.eons_data.data_len);
    offsetting_rsp_ptr += cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_short_name.plmn_name.eons_data.data_len;
    total_len +=  cnf_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[index].plmn_short_name.plmn_name.eons_data.data_len;

  }

  memscpy((void *)temp_total_name_len_ptr, sizeof(uint32), (void *)&total_len, sizeof(uint32));

  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_get_operator_name_cnf */
#endif /* FEATURE_UIMDIAG_INTERNAL_API */


/*===========================================================================
FUNCTION UIMDIAG_MMGSDI_SEND_SYNC_RESPONSE

DESCRIPTION
  This function, handles all synchronous mmgsdi command cnfs and
  prepares diag response packet to send immediate response.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void uimdiag_mmgsdi_send_sync_response(
  mmgsdi_return_enum_type       mmgsdi_status,
  mmgsdi_cnf_enum_type          cnf_type,
  const mmgsdi_cnf_type       * cnf_data_ptr 
)
{
  uimdiag_return_enum_type    status          = UIMDIAG_ERROR;
  uimdiag_sync_rsp_type      *sync_rsp_ptr    = NULL;

  UIMDIAG_MSG_HIGH_2("uimdiag_mmgsdi_send_sync_response: CNF Type is 0x%x & mmgsdi status is 0x%x",
                     cnf_type, mmgsdi_status);

  if (cnf_data_ptr == NULL || (uimdiag_sync_rsp_type *)cnf_data_ptr->response_header.client_data == NULL)  
  {
    UIMDIAG_MSG_ERROR_0("cnf data ptr or client_data in cnf_data_ptr is NULL");
    return;
  }

  sync_rsp_ptr = (uimdiag_sync_rsp_type *)cnf_data_ptr->response_header.client_data;

  if (sync_rsp_ptr->rsp_ptr == NULL )
  {
    UIMDIAG_MSG_ERROR_0("response pointer is NULL");
    UIMDIAGUTIL_TMC_MEM_FREE(sync_rsp_ptr);
    return;
  }

  memset(&(sync_rsp_ptr->rsp_ptr->rsp_payload), 0x00, sizeof(uimdiag_cmd_rsp_pyld_type));

  switch(cnf_type)
  {
    case MMGSDI_GET_ATR_CNF:   
      status = uimdiag_mmgsdi_get_atr_cnf(sync_rsp_ptr->rsp_ptr, cnf_data_ptr);
      if(status != UIMDIAG_SUCCESS)
      {
        sync_rsp_ptr->rsp_ptr->rsp_payload.mmgsdi_status = MMGSDI_ERROR;
      }
      break;
      
    default:
      break;
  }
  diagpkt_subsys_reset_delayed_rsp_id(sync_rsp_ptr->rsp_ptr);
  diagpkt_subsys_set_status(sync_rsp_ptr->rsp_ptr, (uint32) UIMDIAG_SUCCESS);
  diagpkt_commit(sync_rsp_ptr->rsp_ptr);
  UIMDIAGUTIL_TMC_MEM_FREE(sync_rsp_ptr);
} /* uimdiag_mmgsdi_send_sync_response */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SEND_DELAYED_RESPONSE

DESCRIPTION
  This function handles all mmgsdi command cnfs with delayed response and
  prepares diag response packet

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void uimdiag_mmgsdi_send_delayed_response(
  mmgsdi_return_enum_type       mmgsdi_status,
  mmgsdi_cnf_enum_type          cnf_type,
  const mmgsdi_cnf_type       * cnf_data_ptr,
  int32                         index
)

{
  uimdiag_generic_rsp_type   *rsp_ptr                 = NULL;
  uint32                      len                     = 0;
  uimdiag_return_enum_type    uimdiag_return          = UIMDIAG_ERROR;

  UIMDIAG_MSG_HIGH_2("uimdiag_mmgsdi_send_delayed_response: CNF Type is 0x%x & mmgsdi status is 0x%x",
                     cnf_type, mmgsdi_status);

  if (( cnf_data_ptr == NULL )||
      ( uimdiag_req_table_ptr == NULL ))
  {
    UIMDIAG_MSG_ERROR_0("Invalid cnf data or uimdiag_req_table_ptr is NULL");
    return;
  }

  /* calculate len because this cnf has dynamic response */
  switch(cnf_type)
  {
    case MMGSDI_CARD_STATUS_CNF:
      if( mmgsdi_status == MMGSDI_SUCCESS )
      {
        /*  Determine the length of the response pointer
            for header, Send Card Status Payload, 4 bytes for status */
        len = sizeof(uimdiag_cmd_rsp_hdr_type)+
              sizeof(uimdiag_mmgsdi_rsp_session_send_crd_status_pyld_type) + 4;
        /* Add size of Send Card Stauts Cnf data len -1
           one byte for data already available at
           uimdiag_mmgsdi_rsp_session_send_crd_status_pyld_type  */
        len = len + (cnf_data_ptr->status_cnf.status_data.data_len) - 1;
      }
      else
      {
        /* since command handling is not successful -
           allocate a default buffer to hold status */
        len = sizeof( uimdiag_generic_rsp_type );
      }
      break;
    case MMGSDI_READ_CNF:
      len = sizeof(uimdiag_cmd_rsp_hdr_type) +
          sizeof(uimdiag_mmgsdi_rsp_session_read_record_pyld_type) + 4;

      if(cnf_data_ptr->read_cnf.read_data.data_len > MMGSDI_MAX_DATA_BLOCK_LEN)
      {
        len += (cnf_data_ptr->read_cnf.read_data.data_len - MMGSDI_MAX_DATA_BLOCK_LEN);
      }
      break;
#ifdef FEATURE_UIMDIAG_INTERNAL_API
    case MMGSDI_SESSION_READ_PRL_CNF:
      len = sizeof(uimdiag_cmd_rsp_hdr_type) +
          sizeof(uimdiag_mmgsdi_session_read_prl_rsp_type) + 4;

      if(cnf_data_ptr->session_read_prl_cnf.size > UIMDIAG_MAX_ROAMING_LIST_LENGTH)
      {
        len += (cnf_data_ptr->session_read_prl_cnf.size - UIMDIAG_MAX_ROAMING_LIST_LENGTH);
      }
      break;
    case MMGSDI_GET_ALL_AVAILABLE_APPS_CNF:
      if( mmgsdi_status == MMGSDI_SUCCESS )
      {
        /*  Determine the len of the response pointer  */

        /* for header, 4 bytes for status and one set app info by default */
        len = sizeof(uimdiag_cmd_rsp_hdr_type) +
              sizeof(uimdiag_mmgsdi_get_all_available_apps_rsp_type) + 4;

        /* len for one app is already included in
           uimdiag_mmgsdi_get_all_available_apps_rsp_type */
        if((cnf_data_ptr->get_all_available_apps_cnf.card1_num_app +
           cnf_data_ptr->get_all_available_apps_cnf.card2_num_app)  > 0)
        {
          len += ((cnf_data_ptr->get_all_available_apps_cnf.card1_num_app +
                  cnf_data_ptr->get_all_available_apps_cnf.card2_num_app) -1) *
                  sizeof(uimdiag_mmgsdi_app_info_type);
        }
      }
      else
      {
        /* since command handling is not successful -
           allocate a default buffer to hold status */
        len = sizeof( uimdiag_generic_rsp_type );
      }
      break;
    case MMGSDI_SESSION_GET_OPERATOR_NAME_CNF:
      if( mmgsdi_status == MMGSDI_SUCCESS )
      {
        uint32 plmn_index         = 0;
        uint32 temp_dynamic_len   = 0;
        /***
             struct of response
             uint8    encoding_info;
             uint32   operator_name_len;
             uint8    operator_name[1];
             uint8    lang_ind;
             uint8    spn_display_bit;
             uint8    rplmn_display_bit;
             uint8    roaming_status;
             uint32   no_plmn_id;
             uint32   total_buff_size_of_operator_name_info;

             Dynamic part encoding
             uint8    plmn_id[3]
             uint8    rat_type
             uint8    long_name_ci;
             uint8    long_name_spare_bytes;
             uint8    long_name_encoding;
             uint32   long name len
             uint8    *long name
             uint8    short_name_ci;
             uint8    short_name_spare_bytes;
             uint8    short_name_encoding;
             uint32   short name len
             uint8    *short name
          */

        /* calculate len because this cnf has dynamic response */
        len = sizeof(uimdiag_cmd_rsp_hdr_type) + sizeof(uimdiag_mmgsdi_rsp_session_get_operator_name_type) + 4;

        /* operator name length - 1 byte as part of struct already */
        if (cnf_data_ptr->session_get_operator_name_cnf.spn.eons_data.data_len > 1)
        {
          temp_dynamic_len = (cnf_data_ptr->session_get_operator_name_cnf.spn.eons_data.data_len -1);
        }

        temp_dynamic_len += /* for plmn ids (3 bytes each) and RAT type (1 byte each) */
                            ((cnf_data_ptr->session_get_operator_name_cnf.plmn_list.num_of_plmn_ids) * 4) +
                            /* 1 each for encoding, ci and spare bytes for both long and short names */
                            ((cnf_data_ptr->session_get_operator_name_cnf.plmn_list.num_of_plmn_ids) * (6 * sizeof(uint8))) +
                            /* long name and short name length fields*/
                            ((cnf_data_ptr->session_get_operator_name_cnf.plmn_list.num_of_plmn_ids) * sizeof(uint32) * 2);

        for(plmn_index =0; plmn_index<cnf_data_ptr->session_get_operator_name_cnf.plmn_list.num_of_plmn_ids; plmn_index++)
        {
          temp_dynamic_len +=
            cnf_data_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[plmn_index].plmn_long_name.plmn_name.eons_data.data_len;
          temp_dynamic_len +=
            cnf_data_ptr->session_get_operator_name_cnf.plmn_list.plmn_info_ptr[plmn_index].plmn_short_name.plmn_name.eons_data.data_len;
        }

        len += temp_dynamic_len;
      }
      else
      {
        /* since command handling is not successful -
           allocate a default buffer to hold status */
        len = sizeof( uimdiag_generic_rsp_type );
      }
      break;
#endif /* FEATURE_UIMDIAG_INTERNAL_API */

    default:
      len = sizeof( uimdiag_generic_rsp_type );
      break;
  }
  
  UIMDIAG_MSG_HIGH_1("Length to be allocated is %d", len);

  /* Allocate memory for the delayed response pointer */
  rsp_ptr =(uimdiag_generic_rsp_type  * )diagpkt_subsys_alloc_v2_delay(
            (diagpkt_subsys_id_type)DIAG_SUBSYS_UIM,
            (diagpkt_subsys_cmd_code_type)uimdiag_req_table_ptr[index].subcmd,
            (diagpkt_subsys_delayed_rsp_id_type)
            uimdiag_req_table_ptr[index].rsp_id,len );

  if(rsp_ptr == NULL)
  {
    /*do memory allocation for uimdiag_generic_rsp_type only*/
    len = sizeof( uimdiag_generic_rsp_type );

    rsp_ptr =(uimdiag_generic_rsp_type  * )diagpkt_subsys_alloc_v2_delay(
          (diagpkt_subsys_id_type)DIAG_SUBSYS_GSDI,
          (diagpkt_subsys_cmd_code_type)uimdiag_req_table_ptr[index].subcmd,
          (diagpkt_subsys_delayed_rsp_id_type)
          uimdiag_req_table_ptr[index].rsp_id,len );

    if(rsp_ptr == NULL)
    {      
      return;
    }
    mmgsdi_status = MMGSDI_ERROR;

    diagpkt_subsys_set_status(rsp_ptr, (uint32) UIMDIAG_SUCCESS);
    rsp_ptr->rsp_payload.mmgsdi_status = mmgsdi_status;
    diagpkt_delay_commit(rsp_ptr);    
    return;
  }

  switch(cnf_type)
  {
    case MMGSDI_CARD_STATUS_CNF:
      memset((void *)&rsp_ptr->rsp_payload,0x00,(len - sizeof(uimdiag_cmd_rsp_hdr_type)));
      break;
    case MMGSDI_READ_CNF:
    case MMGSDI_COMPUTE_IP_AUTH_CNF:
    case MMGSDI_SESSION_READ_PRL_CNF:
    case MMGSDI_SAP_CARD_READER_STATUS_CNF:
    case MMGSDI_SESSION_GET_OPERATOR_NAME_CNF:
    case MMGSDI_GET_ALL_AVAILABLE_APPS_CNF:
      memset(&rsp_ptr->rsp_payload,0x00,(len - sizeof(uimdiag_cmd_rsp_hdr_type)));
      break;
    default:
      memset(&rsp_ptr->rsp_payload,0x00, sizeof(uimdiag_cmd_rsp_pyld_type));
      break;
  }

  diagpkt_subsys_set_status(rsp_ptr, (uint32) UIMDIAG_SUCCESS);
  rsp_ptr->rsp_payload.mmgsdi_status = mmgsdi_status;

  if (MMGSDI_SUCCESS != mmgsdi_status )
  {
    /* extract cnf data only when cmd handling is successful */
    /* return all confirmation data (e.g. num of retires for PIN case */
    switch(cnf_type)
    {
      case MMGSDI_PIN_OPERATION_CNF:
        break;

      default:
        diagpkt_delay_commit(rsp_ptr);        
        return;
    }

    diagpkt_delay_commit(rsp_ptr);    
    return;
  }

  switch(cnf_type)
  {
    case MMGSDI_CLIENT_ID_AND_EVT_REG_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_proc_client_id_and_evt_reg_cnf(rsp_ptr,cnf_data_ptr);
      break;

    case MMGSDI_CLIENT_ID_AND_EVT_DEREG_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_proc_client_id_and_evt_dereg_cnf(rsp_ptr,cnf_data_ptr);
      break;

    case MMGSDI_SESSION_OPEN_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_proc_session_open_cnf(rsp_ptr,cnf_data_ptr);
      break;

    case MMGSDI_SESSION_CLOSE_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_proc_session_close_cnf(rsp_ptr,cnf_data_ptr);
      break;

    case MMGSDI_SESSION_ACTIVATE_OR_SWITCH_PROVISIONING_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_proc_act_or_switch_prov_cnf(rsp_ptr,cnf_data_ptr);
      break;

    case MMGSDI_SESSION_GET_INFO_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_proc_get_info_cnf(rsp_ptr,cnf_data_ptr);
      break;

    case MMGSDI_GET_ALL_PIN_STATUS_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_proc_get_all_pin_status_cnf(rsp_ptr,cnf_data_ptr);
      break;

    /* below PIN_OPERATION_CNF is called for verify_pin, disable pin
       enable_pin / change pin / unblock pin */
    case MMGSDI_PIN_OPERATION_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_proc_pin_operation_cnf(rsp_ptr,cnf_data_ptr);
      break;

    case MMGSDI_GET_FILE_ATTR_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_proc_get_file_attr_cnf(rsp_ptr,cnf_data_ptr);
      break;

  /* below READ_CNF is called for read_record/read_transparent */
    case MMGSDI_READ_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_proc_read_record_cnf(rsp_ptr,cnf_data_ptr);
      break;

  /* below WRITR_CNF is called for write_record/write_transparent */
    case MMGSDI_WRITE_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_proc_write_record_cnf(rsp_ptr,cnf_data_ptr);
      break;

    case MMGSDI_SESSION_DEACTIVATE_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_session_deactivate_prov_cnf(rsp_ptr,cnf_data_ptr);
      break;

    case MMGSDI_SESSION_GET_APP_CAPABILITIES_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_session_get_app_cap_cnf(rsp_ptr,
                                      &(cnf_data_ptr->session_app_capabilities_cnf));
      break;

    case MMGSDI_SAP_POWER_ON_CNF:
    case MMGSDI_SAP_RESET_CNF:
    case MMGSDI_SAP_CONNECT_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_sap_generic_cnf(rsp_ptr,
          cnf_data_ptr,
          cnf_type);
      break;

    case MMGSDI_CARD_PDOWN_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_card_pdown_cnf(rsp_ptr,cnf_data_ptr);
      break;

    case MMGSDI_CARD_PUP_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_card_pup_cnf(rsp_ptr,cnf_data_ptr);
      break;

    case MMGSDI_ACTIVATE_ONCHIP_SIM_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_activate_onchip_sim_cnf(rsp_ptr,cnf_data_ptr);
      break;

    case MMGSDI_SEND_APDU_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_send_apdu_cnf(rsp_ptr,cnf_data_ptr);
      break;

#ifdef FEATURE_UIMDIAG_INTERNAL_API
    case MMGSDI_REFRESH_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_refresh_cnf(rsp_ptr,cnf_data_ptr);
      break;

    case MMGSDI_SESSION_DISABLE_SERVICE_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_session_disable_service_cnf(rsp_ptr,cnf_data_ptr);
      break;

    case MMGSDI_SESSION_ENABLE_SERVICE_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_session_enable_service_cnf(rsp_ptr,cnf_data_ptr);
      break;

    case MMGSDI_SRV_AVAILABLE_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_srv_avail_cnf(rsp_ptr,cnf_data_ptr);
      break;

    case MMGSDI_SESSION_PROV_APP_INIT_COMPLETE_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_prov_app_init_cmpl_cnf(rsp_ptr,cnf_data_ptr);
      break;

    case MMGSDI_SESSION_READ_PRL_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_read_prl_cnf(rsp_ptr,cnf_data_ptr);
      break;

    case MMGSDI_GET_ALL_AVAILABLE_APPS_CNF:
      uimdiag_return =
          uimdiag_mmgsdi_get_all_available_apps_cnf(rsp_ptr,
                                                    &cnf_data_ptr->get_all_available_apps_cnf);
      break;

    case MMGSDI_SESSION_GET_OPERATOR_NAME_CNF:
      uimdiag_return =
        uimdiag_mmgsdi_get_operator_name_cnf(rsp_ptr,cnf_data_ptr);
      break;

    case MMGSDI_HIDE_CSIM_CNF:
        UIMDIAG_MSG_HIGH_0("MMGSDI_HIDE_CSIM_CNF received");
        uimdiag_return = UIMDIAG_SUCCESS;
      break;

    case MMGSDI_MAP_SESSIONS_TO_SLOTS_CNF:
      UIMDIAG_MSG_HIGH_0("MMGSDI_MAP_SESSIONS_TO_SLOTS_CNF received");
      uimdiag_return = UIMDIAG_SUCCESS;
      break;
#endif /* FEATURE_UIMDIAG_INTERNAL_API */

    default:
      UIMDIAG_MSG_ERROR_1("Unexpected confiramtion type: %x ", cnf_type);
      uimdiag_return = UIMDIAG_ERROR;
      break;
  }

  if (uimdiag_return != UIMDIAG_SUCCESS )
  {
    UIMDIAG_MSG_ERROR_1("CNF: 0x%x handling failed", cnf_type);
    rsp_ptr->rsp_payload.mmgsdi_status = MMGSDI_ERROR;
  }  
  diagpkt_delay_commit(rsp_ptr);
}/* uimdiag_mmgsdi_send_delayed_response */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_GENERIC_RSP_CB

DESCRIPTION
  Generic comand callback function, handles all mmgsdi command cnfs and
  prepares diag response packet

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void uimdiag_mmgsdi_generic_rsp_cb(
  mmgsdi_return_enum_type       mmgsdi_status,
  mmgsdi_cnf_enum_type          cnf_type,
  const mmgsdi_cnf_type       * cnf_data_ptr
)
{
  int32                       index                   = 0;
  boolean                     index_valid             = FALSE;
  uimdiag_sync_rsp_type      *sync_rsp_ptr            = NULL;
  
  /* All commands with delayed responses pass index as userdata,   while commands with immediate 
  response pass uimdiag_sync_rsp_type.   Retrieve the appropriate type, based on command type */
  if(cnf_type != MMGSDI_GET_ATR_CNF)
  {
    index       = (int)cnf_data_ptr->response_header.client_data;
  }
  else
  {
    sync_rsp_ptr = (uimdiag_sync_rsp_type *)cnf_data_ptr->response_header.client_data;
    if ( sync_rsp_ptr == NULL)
    {
      return;
    }
    index        = sync_rsp_ptr->index;
  }

  index_valid = UIMDIAG_REQ_TABLE_IS_INDEX_VALID(index);
  
  if ( FALSE == index_valid )
  {
    UIMDIAG_MSG_ERROR_0("Invalid client data, index is invalid");
    return;
  }
  switch(cnf_type)
  {
    case MMGSDI_GET_ATR_CNF: 
      uimdiag_mmgsdi_send_sync_response(mmgsdi_status, cnf_type, cnf_data_ptr);
      break;
    /* All other commands use delayed responses */
    default:  
      uimdiag_mmgsdi_send_delayed_response(mmgsdi_status, cnf_type, cnf_data_ptr, index);
      break;
  }
  uimdiag_free_index(index);
} /* uimdiag_mmgsdi_generic_rsp_cb */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_FREE_CMD_DATA

DESCRIPTION
  Function to free up allocated memory to command data

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void uimdiag_mmgsdi_free_cmd_data(
  uimdiag_generic_req_type              * req_ptr,
  uimdiag_subsystem_cmd_enum_type         subsys_cmd
)
{
  if (req_ptr == NULL )
  {
    UIMDIAG_MSG_ERROR_0("Invalid req ptr");
    return;
  }

  switch(subsys_cmd)
  {
  case UIMDIAG_MMGSDI_SESSION_OPEN_CMD:
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_open.aid_data_ptr);
    break;

  case UIMDIAG_MMGSDI_SESSION_ACT_OR_SW_PROV_CMD:
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_act_or_sw_prov.data_ptr);
    break;

  case UIMDIAG_MMGSDI_SESSION_VERIFY_PIN_CMD:
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_verify_pin.pin_value_ptr);
    break;

  case UIMDIAG_MMGSDI_SESSION_GET_FILE_ATTR_CMD:
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_get_file_attr.path_ptr);
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_get_file_attr.aid_ptr);
    break;

  case UIMDIAG_MMGSDI_SESSION_READ_RECORD_CMD:
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_read_record.path_ptr);
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_read_record.aid_ptr);
    break;

  case UIMDIAG_MMGSDI_SESSION_READ_TRANSPARENT_CMD:
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_read_transparent.path_ptr);
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_read_transparent.aid_ptr);
    break;

  case UIMDIAG_MMGSDI_SESSION_WRITE_RECORD_CMD:
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_write_record.path_ptr);
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_write_record.aid_ptr);
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_write_record.data_ptr);
    break;

  case UIMDIAG_MMGSDI_SESSION_WRITE_TRANSPARENT_CMD:
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_write_transparent.path_ptr);
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_write_transparent.aid_ptr);
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_write_transparent.data_ptr);
    break;

  case UIMDIAG_MMGSDI_ACTIVATE_ONCHIP_SIM_CMD:
  case UIMDIAG_MMGSDI_ACTIVATE_ONCHIP_USIM_CMD:
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.activate_onchip_sim.onchip_data_ptr);
    break;

  case UIMDIAG_MMGSDI_SEND_APDU_CMD:
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.send_apdu.apdu_data_ptr);
    break;

#ifdef FEATURE_UIMDIAG_INTERNAL_API
  case UIMDIAG_MMGSDI_SESSION_DISABLE_PIN_CMD:
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_disable_pin.pin_value_ptr);
    break;

  case UIMDIAG_MMGSDI_SESSION_ENABLE_PIN_CMD:
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_enable_pin.pin_value_ptr);
    break;

  case UIMDIAG_MMGSDI_SESSION_CHANGE_PIN_CMD:
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_change_pin.pin_value_ptr);
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_change_pin.new_pin_value_ptr);
    break;

  case UIMDIAG_MMGSDI_SESSION_UNBLOCK_PIN_CMD:
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_unblock_pin.puk_value_ptr);
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_unblock_pin.new_pin_value_ptr);
    break;

  case UIMDIAG_MMGSDI_SESSION_REG_FOR_REFRESH_CMD:
  case UIMDIAG_MMGSDI_SESSION_DEREG_FOR_REFRESH_CMD:
  case UIMDIAG_MMGDDI_SESSION_REGISTER_FOR_REFRESH_PROCEED_VOTING_CMD:
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_refresh.file_list_ptr);
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_refresh.file_path_ptr);
    break;

  case UIMDIAG_MMGSDI_NAA_REFRESH_EXT_CMD:
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_trigger_refresh_ext.sim_app_id);
    UIMDIAGUTIL_TMC_MEM_FREE(req_ptr->request.session_trigger_refresh_ext.path);
    break;
#endif /* FEATURE_UIMDIAG_INTERNAL_API */

  default:
    break;
  }
} /* uimdiag_mmgsdi_free_cmd_data */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_PARSE_COMMANDS

DESCRIPTION
  Function to preparse and populate the response pointer with the conformation
  data

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type.

SIDE EFFECTS
  None.
===========================================================================*/
uimdiag_return_enum_type uimdiag_mmgsdi_parse_commands(
  PACKED void                           * cmd_ptr,
  uint16                                  pkt_len,
  uimdiag_generic_rsp_type             ** rsp_pptr,
  diagpkt_subsys_delayed_rsp_id_type    * rsp_id_ptr,
  int32                                 * index_ptr,
  uimdiag_generic_req_type              * request_ptr,
  uimdiag_subsystem_cmd_enum_type         subsys_cmd
)
{
  uimdiag_return_enum_type              uimdiag_status         = UIMDIAG_ERROR;

  UIMDIAG_UTIL_RETURN_IF_NULL_3(rsp_pptr, rsp_id_ptr, request_ptr);

  /*  allocate immediate response pointer  */
  *rsp_pptr = (uimdiag_generic_rsp_type *)
                diagpkt_subsys_alloc_v2(
                  (diagpkt_subsys_id_type) DIAG_SUBSYS_GSDI,
                  (diagpkt_subsys_cmd_code_type) subsys_cmd,
                  sizeof(uimdiag_generic_rsp_type));

  if (*rsp_pptr == NULL)
  {
    return UIMDIAG_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  memset(&(*rsp_pptr)->rsp_payload,0x00, sizeof(uimdiag_cmd_rsp_pyld_type));

  /* Initialize to MMGSDI_ERROR for the immediate response */
  (*rsp_pptr)->rsp_payload.mmgsdi_status = MMGSDI_ERROR;

  *rsp_id_ptr = diagpkt_subsys_get_delayed_rsp_id(*rsp_pptr );

  /* extract data from req packet
  and populate fields in the request structure */
  uimdiag_status = uimdiag_parse_cmd_data(request_ptr, cmd_ptr, pkt_len);

  if (uimdiag_status != UIMDIAG_SUCCESS)
  {
    UIMDIAG_MSG_ERROR_1("uimdiag_mmgsdi_parse_commands: Parsing error 0x%x",
                        uimdiag_status);
    /* Do an immediate rsp - No delayed response expected*/
    diagpkt_subsys_reset_delayed_rsp_id(*rsp_pptr);
    diagpkt_subsys_set_status(*rsp_pptr, (uint32) UIMDIAG_SUCCESS);
    diagpkt_commit(*rsp_pptr);
    uimdiag_mmgsdi_free_cmd_data(request_ptr, subsys_cmd);
    return uimdiag_status;
  }
  /* find a free space in req_table to hold information for callback */
  *index_ptr = uimdiag_get_index();
  if (*index_ptr == UIMDIAG_GET_FN_ERR )
  {
    /*  req_table is full */
    uimdiag_status = UIMDIAG_MAX_ASYNC_REQ_EXCEED_ERR;
    /* Do an immediate rsp - No delayed response expected*/
    diagpkt_subsys_reset_delayed_rsp_id(*rsp_pptr);
    diagpkt_subsys_set_status(*rsp_pptr, (uint32) UIMDIAG_SUCCESS);
    diagpkt_commit(*rsp_pptr);
    uimdiag_mmgsdi_free_cmd_data(request_ptr, subsys_cmd);
    return uimdiag_status;
  }
  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_parse_commands */


/*===========================================================================
FUNCTION UIMDIAG_MMGSDI_EVENT_CB

DESCRIPTION
    This function is the event handling function for different events from MMGSDI.

DEPENDENCIES
    None

INPUT
    mmgsdi_event_data_type *event_ptr

RETURN VALUE
    None

OUTPUT
    None
===========================================================================*/
static void uimdiag_mmgsdi_event_cb(const mmgsdi_event_data_type *event_ptr)
{
  uimdiag_mmgsdi_event_payload_type event_payload; /* used to send event*/
  uint8                             len    = 0;    /* length of the event_payload*/
  uint8                             offset = 0 ;

  if(event_ptr == NULL)
  {
    return;
  }

  UIMDIAG_MSG_HIGH_1("In uimdiag_mmgsdi_event_cb, Event is 0x%x",
                     event_ptr->evt);

  (void)memset((void*)&event_payload,0x00,sizeof(uimdiag_mmgsdi_event_payload_type));
  len   = sizeof(event_payload);

  if (UIMDIAG_SUCCESS !=
      uimdiag_util_convert_from_mmgsdi_events_enum(event_ptr->evt,
                                                   &event_payload.evt))
  {
    /* Return without sending the event */
    return;
  }

  memscpy(event_payload.event_payload, sizeof(event_payload.event_payload),
          &event_ptr->session_id, UIMDIAG_SESSION_ID_SIZE);
  offset = offset + UIMDIAG_SESSION_ID_SIZE;

  switch (event_ptr->evt)
  {
    case MMGSDI_REFRESH_EVT:
      if( (offset+4) > EVENT_MAX_PAYLOAD)
      {
        return;
      }
      event_payload.event_payload[offset] = (uint8)event_ptr->data.refresh.slot;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.refresh.stage;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.refresh.mode;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.refresh.orig_mode;
      break;

    case MMGSDI_FDN_EVT:
      if( (offset+3) > EVENT_MAX_PAYLOAD)
      {
        return;
      }
       /* Typecasting below to uint8 could be a problem, as the rec_num
         is 32 bits long*/
      event_payload.event_payload[offset] = (uint8)event_ptr->data.fdn.slot;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.fdn.enabled;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.fdn.rec_num;
      break;

    case MMGSDI_SUBSCRIPTION_READY_EVT:
      if( (offset+3) > EVENT_MAX_PAYLOAD)
      {
        return;
      }
      event_payload.event_payload[offset] = (uint8)event_ptr->data.subscription_ready.app_info.slot;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.subscription_ready.app_info.app_data.app_type;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.subscription_ready.app_info.cached;
      break;

    case MMGSDI_PERSO_EVT:
      if( (offset+4) > EVENT_MAX_PAYLOAD)
      {
        return;
      }
      event_payload.event_payload[offset] = (uint8)event_ptr->data.perso.feature;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.perso.status;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.perso.num_retries;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.perso.num_unblock_retries;
      offset++;
      break;

    case MMGSDI_SESSION_ILLEGAL_SUBSCRIPTION_EVT:
      if( (offset+1) > EVENT_MAX_PAYLOAD)
      {
        return;
      }
      event_payload.event_payload[offset] = (uint8)event_ptr->data.illegal_session.session_id;
      break;

    case MMGSDI_SESSION_CHANGED_EVT:
      if( (offset+3) > EVENT_MAX_PAYLOAD)
      {
        return;
      }
      event_payload.event_payload[offset] = (uint8)event_ptr->data.session_changed.activated;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.session_changed.app_info.cached;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.session_changed.app_info.slot;
      break;

    case MMGSDI_CARD_INSERTED_EVT:
      if( (offset+2) > EVENT_MAX_PAYLOAD)
      {
        return;
      }
      event_payload.event_payload[offset] = (uint8)event_ptr->data.card_inserted.slot;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.card_inserted.num_aids_avail;
      break;

    case MMGSDI_CARD_ERROR_EVT:
      if( (offset+2) > EVENT_MAX_PAYLOAD)
      {
        return;
      }
      event_payload.event_payload[offset] = (uint8)event_ptr->data.card_error.slot;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.card_error.info;
      break;

    case MMGSDI_CARD_REMOVED_EVT:
      if( (offset+1) > EVENT_MAX_PAYLOAD)
      {
        return;
      }
      event_payload.event_payload[offset] = (uint8)event_ptr->data.card_removed.slot;
      break;

    case MMGSDI_PIN1_EVT:
    case MMGSDI_PIN2_EVT:
    case MMGSDI_UNIVERSAL_PIN_EVT:
      if( (offset+4) > EVENT_MAX_PAYLOAD)
      {
        return;
      }
      event_payload.event_payload[offset] = (uint8)event_ptr->data.pin.slot;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.pin.pin_info.pin_id;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.pin.pin_info.status;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.pin.pin_info.num_retries;
      break;

    case MMGSDI_EONS_OPL_PNN_CACHE_READY_EVT:
      if( (offset+6) > EVENT_MAX_PAYLOAD)
      {
        return;
      }
      event_payload.event_payload[offset] = (uint8)event_ptr->data.eons_opl_pnn_cache_ready.opl_info.num_of_rec;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.eons_opl_pnn_cache_ready.opl_info.rec_len;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.eons_opl_pnn_cache_ready.opl_info.valid_record_list.data_len;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.eons_opl_pnn_cache_ready.pnn_info.num_of_rec;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.eons_opl_pnn_cache_ready.pnn_info.rec_len;
      offset++;
      event_payload.event_payload[offset] = (uint8)event_ptr->data.eons_opl_pnn_cache_ready.pnn_info.valid_record_list.data_len;
      break;

    default:
      /* For unhandled MMGSDI events return without sending diag event */
      return;
  }/*switch (event_ptr->evt)*/

  event_report_payload( EVENT_MMGSDI_EVENT, len, (void *)&event_payload);
}/* uimdiag_mmgsdi_event_cb */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_POST_PARSE_COMMANDS

DESCRIPTION
  Function post parsing the command to commit delayed response

DEPENDENCIES
  None.

RETURN VALUE
  uimdiag_return_enum_type

SIDE EFFECTS
  None.
===========================================================================*/
uimdiag_return_enum_type uimdiag_mmgsdi_post_parse_commands(
  uimdiag_generic_rsp_type             ** rsp_pptr,
  diagpkt_subsys_delayed_rsp_id_type      rsp_id,
  uimdiag_return_enum_type                uimdiag_status,
  int32                                   index,
  mmgsdi_return_enum_type                 mmgsdi_status,
  uimdiag_generic_req_type              * request_ptr,
  uimdiag_subsystem_cmd_enum_type         subsys_cmd
)
{
  uimdiag_generic_rsp_type               * delayed_rsp_ptr = NULL;
  if (rsp_pptr == NULL)
  {
    return UIMDIAG_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  if (request_ptr == NULL)
  {
    return UIMDIAG_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  diagpkt_subsys_set_status(* rsp_pptr, (uint32) UIMDIAG_SUCCESS);
  diagpkt_commit(* rsp_pptr);

  if ( MMGSDI_SUCCESS != mmgsdi_status )
  {
    /* error in handling API , we need a delayed response too */
    UIMDIAG_MSG_ERROR_2("FAILED TO QUEUE MMGSDI SESSION COMMAND, status: 0x%x for cmd: 0x%x",
                        mmgsdi_status, subsys_cmd);
    uimdiag_free_index(index);
    delayed_rsp_ptr = (uimdiag_generic_rsp_type *)
                        diagpkt_subsys_alloc_v2_delay(
                          (diagpkt_subsys_id_type) DIAG_SUBSYS_GSDI,
                          (diagpkt_subsys_cmd_code_type) subsys_cmd,
                           rsp_id,
                           sizeof(uimdiag_generic_rsp_type));

    if (delayed_rsp_ptr == NULL)
    {
      uimdiag_mmgsdi_free_cmd_data(request_ptr, subsys_cmd);
      return UIMDIAG_ERROR;
    }
    memset(&delayed_rsp_ptr->rsp_payload,0x00, sizeof(uimdiag_cmd_rsp_pyld_type));
    delayed_rsp_ptr->rsp_payload.mmgsdi_status = mmgsdi_status;
    diagpkt_subsys_set_status(delayed_rsp_ptr,(uint32)UIMDIAG_SUCCESS);
    diagpkt_delay_commit(delayed_rsp_ptr);
  }
  /*Set the immediate response to SUCCESS*/
  (*rsp_pptr)->rsp_payload.mmgsdi_status = MMGSDI_SUCCESS;
  return UIMDIAG_SUCCESS;
} /* uimdiag_mmgsdi_post_parse_commands */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_OPEN_CMD

DESCRIPTION
  Command handler for session open subsystem command

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_open_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type              request;
  uimdiag_generic_rsp_type             *rsp_ptr                = NULL;
  uimdiag_return_enum_type              delayed_rsp_status     = UIMDIAG_ERROR;
  diagpkt_subsys_delayed_rsp_id_type    rsp_id                 = 0;
  uimdiag_return_enum_type              uimdiag_status         = UIMDIAG_ERROR;
  uimdiag_mmgsdi_session_open_cmd_type *session_open_req_ptr   = NULL;
  mmgsdi_client_id_type                 client_id              = 0;
  int32                                 index                  = 0;
  mmgsdi_return_enum_type               mmgsdi_status          = MMGSDI_ERROR;
  mmgsdi_non_prov_app_info_type         non_prov_app_data;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_open_cmd");

  (void)memset ( (void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );
  (void)memset ( (void*)&non_prov_app_data, 0x00,
                 sizeof(mmgsdi_non_prov_app_info_type));

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,pkt_len,&rsp_ptr,&rsp_id,&index,
                     &request,UIMDIAG_MMGSDI_SESSION_OPEN_CMD);

  if((uimdiag_status != UIMDIAG_SUCCESS))
  {
    return rsp_ptr;
  }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_SESSION_OPEN_CMD;

  session_open_req_ptr = &request.request.session_open;
  UIMDIAG_TOUINT64(client_id,session_open_req_ptr->client_id_low,
                   session_open_req_ptr->client_id_high);

  if((session_open_req_ptr->session_type ==
     (uint32) MMGSDI_NON_PROV_SESSION_SLOT_1) ||
     (session_open_req_ptr->session_type ==
     (uint32) MMGSDI_NON_PROV_SESSION_SLOT_2) ||
     (session_open_req_ptr->session_type ==
     (uint32) MMGSDI_NON_PROV_SESSION_SLOT_3))
  {
     non_prov_app_data.exclusive_channel    =
     (boolean)session_open_req_ptr->exclusive_channel;

     non_prov_app_data.app_id_data.data_len =
       session_open_req_ptr->aid_data_len;

     if (session_open_req_ptr->aid_data_ptr != NULL)
      {
       non_prov_app_data.app_id_data.data_ptr = session_open_req_ptr->aid_data_ptr;
      }
     else
     {
       UIMDIAG_MSG_ERROR_0("UIMDIAG Session Open: Non Prov APP AID Data Pointer is NULL");
     }

   mmgsdi_status = mmgsdi_session_open (
                    client_id,
                    (mmgsdi_session_type_enum_type)
                    session_open_req_ptr->session_type,
                    &non_prov_app_data,
                    uimdiag_mmgsdi_event_cb,
                    (boolean)session_open_req_ptr->set_notify,
                    (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                    (mmgsdi_client_data_type)index);
  }
  else
  {

   mmgsdi_status = mmgsdi_session_open (
                    client_id,
                    (mmgsdi_session_type_enum_type)
                    session_open_req_ptr->session_type,
                    NULL,
                    uimdiag_mmgsdi_event_cb,
                    (boolean)session_open_req_ptr->set_notify,
                    (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                    (mmgsdi_client_data_type)index);
  }

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_OPEN_CMD);

  if( delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_open.aid_data_ptr);
  return rsp_ptr;
}/* uimdiag_mmgsdi_session_open_cmd */

/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_CLOSE_CMD

DESCRIPTION
  Command handler for session close subsystem command

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_close_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status = UIMDIAG_ERROR;
  int32                              index              = 0;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_session_id_type             session_id;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_close_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

 uimdiag_status = uimdiag_mmgsdi_parse_commands(
                    cmd_ptr,
                    pkt_len,
                    &rsp_ptr,
                    &rsp_id,
                    &index,
                    &request,
                    UIMDIAG_MMGSDI_SESSION_CLOSE_CMD);

  if(uimdiag_status != UIMDIAG_SUCCESS)
  {
    return rsp_ptr;
  }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_SESSION_CLOSE_CMD;
  session_id = request.request.session_close.session_id_low;

  mmgsdi_status = mmgsdi_session_close( session_id,
                    (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                    (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_CLOSE_CMD
    );

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }
  return rsp_ptr;
}/* uimdiag_mmgsdi_session_close_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_CLIENT_ID_AND_EVT_REG_CMD

DESCRIPTION
  Command handler for client id and evt reg subsystem command

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_client_id_and_evt_reg_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status = UIMDIAG_ERROR;
  int32                              index              = 0;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_client_id_and_evt_reg_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_CLIENT_ID_AND_EVT_REG_CMD);

  if(uimdiag_status != UIMDIAG_SUCCESS)
  {
    return rsp_ptr;
  }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd =
                        UIMDIAG_MMGSDI_CLIENT_ID_AND_EVT_REG_CMD;

  mmgsdi_status =   mmgsdi_client_id_and_evt_reg(uimdiag_mmgsdi_event_cb,
                      (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                      (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_CLIENT_ID_AND_EVT_REG_CMD
    );

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_client_id_and_evt_reg_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_CLIENT_ID_AND_EVT_DEREG_CMD

DESCRIPTION
  Command handler for client id and evt dereg subsystem command

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_client_id_and_evt_dereg_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type             request;
  uimdiag_generic_rsp_type            *rsp_ptr            = NULL;
  diagpkt_subsys_delayed_rsp_id_type   rsp_id             = 0;
  uimdiag_return_enum_type             uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type             delayed_rsp_status = UIMDIAG_ERROR;
  int32                                index           = 0;
  mmgsdi_client_id_type                client_id       = 0;
  mmgsdi_return_enum_type              mmgsdi_status   = MMGSDI_ERROR;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_client_id_and_evt_dereg_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_CLIENT_ID_AND_EVT_DEREG_CMD);

  if(uimdiag_status != UIMDIAG_SUCCESS)
  {
    return rsp_ptr;
  }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd =
                        UIMDIAG_MMGSDI_CLIENT_ID_AND_EVT_DEREG_CMD;

  UIMDIAG_TOUINT64(client_id,request.request.client_id_and_evt_dereg.client_id_low,
                   request.request.client_id_and_evt_dereg.client_id_high);

  mmgsdi_status =  mmgsdi_client_id_and_evt_dereg(client_id,
                     (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                     (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_CLIENT_ID_AND_EVT_DEREG_CMD);

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_client_id_and_evt_dereg_cmd */

/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_ACT_OR_SW_PROV_CMD

DESCRIPTION
  Command handler for session activate or Switch Provisioning

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_act_or_sw_prov_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status = UIMDIAG_ERROR;
  int32                              index              = 0;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  mmgsdi_session_id_type             session_id;
  mmgsdi_slot_id_enum_type           card_slot;
  mmgsdi_data_type                   app_id_data;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_act_or_switch_prov_cmd");
  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

 uimdiag_status = uimdiag_mmgsdi_parse_commands(
                    cmd_ptr,
                    pkt_len,
                    &rsp_ptr,
                    &rsp_id,
                    &index,
                    &request,
                    UIMDIAG_MMGSDI_SESSION_ACT_OR_SW_PROV_CMD);

  if(uimdiag_status != UIMDIAG_SUCCESS)
  {
    return rsp_ptr;
  }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd =
                    UIMDIAG_MMGSDI_SESSION_ACT_OR_SW_PROV_CMD;

  session_id = request.request.session_act_or_sw_prov.session_id_low;
  card_slot  = (mmgsdi_slot_id_enum_type)
                 request.request.session_act_or_sw_prov.slot_id;

  app_id_data.data_len = request.request.session_act_or_sw_prov.data_len;
  app_id_data.data_ptr = request.request.session_act_or_sw_prov.data_ptr;

  mmgsdi_status =  mmgsdi_session_activate_or_switch_provisioning (
                     session_id,
                     card_slot,
                     app_id_data,
                     FALSE,
                     (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                     (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_ACT_OR_SW_PROV_CMD);

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_act_or_sw_prov.data_ptr);
  return rsp_ptr;
}/* uimdiag_mmgsdi_session_act_or_sw_prov_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_GET_INFO_CMD

DESCRIPTION
  Command handler for session get info subsystem command

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_get_info_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type          request;
  uimdiag_generic_rsp_type          *rsp_ptr             = NULL;
  uimdiag_return_enum_type          uimdiag_status       = UIMDIAG_ERROR;
  uimdiag_return_enum_type          delayed_rsp_status   = UIMDIAG_ERROR;
  int32                             index                = 0;
  mmgsdi_return_enum_type           mmgsdi_status        = MMGSDI_ERROR;
  diagpkt_subsys_delayed_rsp_id_type rsp_id              = 0;
  mmgsdi_session_id_type            session_id;
  mmgsdi_session_info_query_type    session_info_query;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_get_info_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );
  (void)memset((void*)&session_info_query, 0x00, sizeof(mmgsdi_session_info_query_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_GET_INFO_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_SESSION_GET_INFO_CMD;
  session_id = request.request.session_get_info.session_id_low;

  session_info_query.query_type = (mmgsdi_session_info_enum_type)
    request.request.session_get_info.session_info;

  if(session_info_query.query_type == MMGSDI_SESSION_INFO_QUERY_BY_ID)
  {
    session_info_query.query_key.session_id = session_id;
  }
  else if(session_info_query.query_type == MMGSDI_SESSION_INFO_QUERY_BY_TYPE)
  {
    session_info_query.query_key.session_type = (mmgsdi_session_type_enum_type)
      request.request.session_get_info.session_type;
  }

  mmgsdi_status =   mmgsdi_session_get_info(
                      session_info_query ,
                      (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                      (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_GET_INFO_CMD);

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_session_get_info_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_VERIFY_PIN_CMD

DESCRIPTION
  Command handler to perform a PIN verification procedure against a card for
  PIN1, PIN2 or UPIN.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_verify_pin_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type                    request;
  uimdiag_generic_rsp_type                   *rsp_ptr                = NULL;
  uimdiag_mmgsdi_session_verify_pin_cmd_type *session_verify_pin_ptr = NULL;
  diagpkt_subsys_delayed_rsp_id_type          rsp_id                 = 0;
  int32                                       index                  = 0;
  mmgsdi_return_enum_type                     mmgsdi_status          =
                                                                  MMGSDI_ERROR;
  uimdiag_return_enum_type                    uimdiag_status         =
                                                                  UIMDIAG_ERROR;
  uimdiag_return_enum_type                    delayed_rsp_status     =
                                                                  UIMDIAG_ERROR;
  mmgsdi_session_id_type                      session_id;
  mmgsdi_data_type                            pin_data;
  mmgsdi_pin_enum_type                        pin_id;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_verify_pin_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_VERIFY_PIN_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_SESSION_VERIFY_PIN_CMD;

  session_verify_pin_ptr = &request.request.session_verify_pin;

  session_id             = session_verify_pin_ptr->session_id_low;
  pin_id                 = (mmgsdi_pin_enum_type)
                              session_verify_pin_ptr->pin_id;
  pin_data.data_len      = session_verify_pin_ptr->pin_len;


  if (session_verify_pin_ptr->pin_value_ptr != NULL)
  {
    pin_data.data_ptr    = session_verify_pin_ptr->pin_value_ptr;
  }
  else
  {
    UIMDIAG_MSG_ERROR_0("UIMDIAG Session Verify Pin: Pin Value Pointer is NULL");
    return NULL;
  }

  mmgsdi_status = mmgsdi_session_verify_pin(
                    session_id, pin_id, pin_data,
                    (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                    (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_VERIFY_PIN_CMD);

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_verify_pin.pin_value_ptr);
  return rsp_ptr;
}/* uimdiag_mmgsdi_session_verify_pin_cmd */


#ifdef FEATURE_UIMDIAG_INTERNAL_API
/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_GET_ALL_PIN_STATUS_CMD

DESCRIPTION
  Command handler to request the PIN status for all relevant PINs applicable
  to the session ID provided.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_get_all_pin_status_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr             = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id              = 0;
  uimdiag_return_enum_type           uimdiag_status      = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status  = UIMDIAG_ERROR;
  int32                              index               = 0;
  mmgsdi_return_enum_type            mmgsdi_status       = MMGSDI_ERROR;
  mmgsdi_session_id_type             session_id;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_get_all_pin_status_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_GET_ALL_PIN_STATUS_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_SESSION_GET_ALL_PIN_STATUS_CMD;

  session_id = request.request.session_get_all_pin_stat.session_id_low;

  mmgsdi_status = mmgsdi_session_get_all_pin_status ( session_id,
                      (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                      (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_GET_ALL_PIN_STATUS_CMD);

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_session_get_all_pin_status_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_DISABLE_PIN_CMD

DESCRIPTION
  Command handler to build a request to MMGSDI to disable the PIN provided

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_disable_pin_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status = UIMDIAG_ERROR;
  int32                              index              = 0;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_session_id_type             session_id;
  mmgsdi_pin_enum_type               pin_id;
  mmgsdi_pin_replace_enum_type       replace_pin;
  mmgsdi_data_type                   pin_data;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_disable_pin_cmd");
  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_DISABLE_PIN_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_SESSION_DISABLE_PIN_CMD;

  session_id         = request.request.session_disable_pin.session_id_low;
  pin_id             = (mmgsdi_pin_enum_type)
                          request.request.session_disable_pin.pin_id;
  replace_pin        = (mmgsdi_pin_replace_enum_type)
                          request.request.session_disable_pin.pin_replace;
  pin_data.data_len  = request.request.session_disable_pin.pin_len;
  pin_data.data_ptr  = request.request.session_disable_pin.pin_value_ptr;

  mmgsdi_status =   mmgsdi_session_disable_pin (
                      session_id,
                      pin_id,
                      replace_pin,
                      pin_data,
                      (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                      (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_DISABLE_PIN_CMD);

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_disable_pin.pin_value_ptr);
  return rsp_ptr;
}/* uimdiag_mmgsdi_session_disable_pin_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_ENABLE_PIN_CMD

DESCRIPTION
  Command handler to build a request to MMGSDI to enable the PIN provided

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_enable_pin_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status = UIMDIAG_ERROR;
  int32                              index              = 0;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_pin_enum_type               pin_id;
  mmgsdi_data_type                   pin_data;
  mmgsdi_session_id_type             session_id;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_enable_pin_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_ENABLE_PIN_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_SESSION_ENABLE_PIN_CMD;

  session_id        = request.request.session_enable_pin.session_id_low;
  pin_id            = (mmgsdi_pin_enum_type)
                        request.request.session_enable_pin.pin_id;
  pin_data.data_len = request.request.session_enable_pin.pin_len;
  pin_data.data_ptr = request.request.session_enable_pin.pin_value_ptr;

  mmgsdi_status =   mmgsdi_session_enable_pin (
                      session_id,
                      pin_id,
                      pin_data,
                      (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                      (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_ENABLE_PIN_CMD
    );

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_enable_pin.pin_value_ptr);
  return rsp_ptr;
}/* uimdiag_mmgsdi_session_enable_pin_cmd */

/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_CHANGE_PIN_CMD

DESCRIPTION
  Command handler to build a request to MMGSDI to change the PIN provided

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_change_pin_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status = UIMDIAG_ERROR;
  int32                              index              = 0;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_pin_enum_type               pin_id;
  mmgsdi_data_type                   pin_data;
  mmgsdi_data_type                   new_pin_data;
  mmgsdi_session_id_type             session_id;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_change_pin_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_CHANGE_PIN_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_SESSION_CHANGE_PIN_CMD;

  session_id            = request.request.session_change_pin.session_id_low;
  pin_id                = (mmgsdi_pin_enum_type)
                            request.request.session_change_pin.pin_id;
  pin_data.data_len     = request.request.session_change_pin.pin_len;
  pin_data.data_ptr     = request.request.session_change_pin.pin_value_ptr;
  new_pin_data.data_len = request.request.session_change_pin.new_pin_len;
  new_pin_data.data_ptr = request.request.session_change_pin.new_pin_value_ptr;

  mmgsdi_status =   mmgsdi_session_change_pin (
                      session_id,
                      pin_id,
                      pin_data,
                      new_pin_data,
                      (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                      (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_CHANGE_PIN_CMD
    );

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_change_pin.pin_value_ptr);
  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_change_pin.new_pin_value_ptr);
  return rsp_ptr;
}/* uimdiag_mmgsdi_session_change_pin_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_UNBLOCK_PIN_CMD

DESCRIPTION
  Command handler to build a request to MMGSDI to unblock the PIN provided

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_unblock_pin_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status = UIMDIAG_ERROR;
  int32                              index              = 0;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_pin_enum_type               pin_id;
  mmgsdi_data_type                   puk_data;
  mmgsdi_data_type                   new_pin_data;
  mmgsdi_session_id_type             session_id;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_unblock_pin_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_UNBLOCK_PIN_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_SESSION_UNBLOCK_PIN_CMD;

  session_id            = request.request.session_unblock_pin.session_id_low;
  pin_id                = (mmgsdi_pin_enum_type)
                            request.request.session_unblock_pin.pin_id;
  puk_data.data_len     = request.request.session_unblock_pin.puk_len;
  puk_data.data_ptr     = request.request.session_unblock_pin.puk_value_ptr;
  new_pin_data.data_len = request.request.session_unblock_pin.new_pin_len;
  new_pin_data.data_ptr = request.request.session_unblock_pin.new_pin_value_ptr;

  mmgsdi_status =   mmgsdi_session_unblock_pin (
                      session_id,
                      pin_id,
                      puk_data,
                      new_pin_data,
                      (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                      (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_UNBLOCK_PIN_CMD
    );

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_unblock_pin.puk_value_ptr);
  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_unblock_pin.new_pin_value_ptr);
  return rsp_ptr;
}/* uimdiag_mmgsdi_session_unblock_pin_cmd */
#endif /* FEATURE_UIMDIAG_INTERNAL_API */


/*===========================================================================
FUNCTION UIMDIAG_MMGSDI_SESSION_GET_FILE_ATTR_CMD

DESCRIPTION
  Command handler to build a request to MMGSDI to get file attr

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_get_file_attr_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr             = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id              = 0;
  uimdiag_return_enum_type           uimdiag_status      = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status  = UIMDIAG_ERROR;
  int32                              index               = 0;
  mmgsdi_return_enum_type            mmgsdi_status       = MMGSDI_ERROR;

  mmgsdi_session_id_type             session_id;
  mmgsdi_access_type                  file_name;


  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_get_file_attr_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );
  (void)memset((void*)&file_name, 0x00, sizeof(mmgsdi_access_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_GET_FILE_ATTR_CMD);

  if(uimdiag_status != UIMDIAG_SUCCESS)
    {
     return rsp_ptr;
    }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_SESSION_GET_FILE_ATTR_CMD;

  session_id                  = request.request.session_get_file_attr.session_id_low;

  file_name.access_method     = (mmgsdi_access_method_enum_type) request.request.session_get_file_attr.access_method;

  switch(file_name.access_method)
  {
   case MMGSDI_EF_ENUM_ACCESS:
        file_name.file.file_enum = uimdiag_util_convert_to_mmgsdi_items_enum
                                     (request.request.session_get_file_attr.ef_enum);
        break;

   case MMGSDI_DF_ENUM_ACCESS:
        file_name.file.df_enum = (mmgsdi_df_enum_type)
                                   request.request.session_get_file_attr.df_enum;
        break;

   case MMGSDI_BY_PATH_ACCESS:
        file_name.file.path_type.path_len = request.request.session_get_file_attr.path_len;
        if ( (request.request.session_get_file_attr.path_len > 0) &&
             (request.request.session_get_file_attr.path_len  <=  MMGSDI_MAX_PATH_LEN))
          {
           memscpy(file_name.file.path_type.path_buf,
           sizeof(file_name.file.path_type.path_buf),
           request.request.session_get_file_attr.path_ptr,
           (request.request.session_get_file_attr.path_len*sizeof(uint16)));
          }
        else
        {
          UIMDIAG_MSG_ERROR_0("path_len could be <= 0 or exceeds max path len size ");
          return NULL;
          }
        break;

   case MMGSDI_BY_APP_ID_ACCESS:
        file_name.file.app_id.data_len = request.request.session_get_file_attr.aid_len;
        if (request.request.session_get_file_attr.aid_len != 0 )
          {
           memscpy(file_name.file.app_id.data_ptr,
           sizeof(file_name.file.app_id.data_ptr),
           request.request.session_get_file_attr.aid_ptr,
           request.request.session_get_file_attr.aid_len);
          }
        break;

   default:
        break;
  }

  mmgsdi_status =   mmgsdi_session_get_file_attr (
                    session_id,
                    file_name,
                    (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                    (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_GET_FILE_ATTR_CMD
    );

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_get_file_attr.path_ptr);
  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_get_file_attr.aid_ptr);
  return rsp_ptr;
}/* uimdiag_mmgsdi_session_get_file_attr_cmd */


/*===========================================================================
FUNCTION UIMDIAG_MMGSDI_SESSION_READ_RECORD_CMD

DESCRIPTION
  Command handler to build a request to MMGSDI to read record

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_read_record_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr             = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id              = 0;
  uimdiag_return_enum_type           uimdiag_status      = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status  = UIMDIAG_ERROR;
  int32                              index               = 0;
  mmgsdi_return_enum_type            mmgsdi_status       = MMGSDI_ERROR;

  mmgsdi_session_id_type             session_id;
  mmgsdi_access_type                 file_access;
  mmgsdi_rec_num_type                record_number;
  mmgsdi_len_type                    request_length;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_read_record_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );
  (void)memset((void*)&file_access, 0x00, sizeof(mmgsdi_access_type));

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_READ_RECORD_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_SESSION_READ_RECORD_CMD;

  session_id                    = request.request.session_read_record.session_id_low;
  record_number           = request.request.session_read_record.rec_num;
  request_length                = request.request.session_read_record.rec_len;

  file_access.access_method     = (mmgsdi_access_method_enum_type) request.request.session_read_record.access_method;

  switch(file_access.access_method)
  {
  case MMGSDI_EF_ENUM_ACCESS:
     file_access.file.file_enum = uimdiag_util_convert_to_mmgsdi_items_enum(
                                    request.request.session_read_record.ef_enum);
     break;

  case MMGSDI_DF_ENUM_ACCESS:
     file_access.file.df_enum = (mmgsdi_df_enum_type)
                                 request.request.session_read_record.df_enum;
     break;

  case MMGSDI_BY_PATH_ACCESS:
     file_access.file.path_type.path_len = request.request.session_read_record.path_len;
     if( (request.request.session_read_record.path_len > 0 ) &&
         (request.request.session_read_record.path_len <= MMGSDI_MAX_PATH_LEN))
     {
       memscpy(file_access.file.path_type.path_buf,
         sizeof(file_access.file.path_type.path_buf),
         request.request.session_read_record.path_ptr,
         (request.request.session_read_record.path_len*sizeof(uint16)));
     }
     else
     {
       UIMDIAG_MSG_ERROR_0("path_len could be <= 0 or exceeds max path len size ");
       return NULL;
     }
     break;

  case MMGSDI_BY_APP_ID_ACCESS:
     file_access.file.app_id.data_len = request.request.session_read_record.aid_len;
     if (request.request.session_read_record.aid_len != 0 )
     {
      memscpy(file_access.file.app_id.data_ptr,
      sizeof(file_access.file.app_id.data_ptr),
      request.request.session_read_record.aid_ptr,
      request.request.session_read_record.aid_len);
     }
     break;

  default:
     break;

  }

  mmgsdi_status =   mmgsdi_session_read_record (
                      session_id,
                      file_access,
            record_number,
            request_length,
                      (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                      (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_READ_RECORD_CMD
    );

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_read_record.path_ptr);
  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_read_record.aid_ptr);
  return rsp_ptr;
}/* uimdiag_mmgsdi_session_read_record_cmd */


/*===========================================================================
FUNCTION UIMDIAG_MMGSDI_SESSION_READ_FROM_CACHE_CMD

DESCRIPTION
  Command handler to build a request to MMGSDI to read record

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_read_from_cache_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr             = NULL;
  uimdiag_return_enum_type           uimdiag_status      = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status       = MMGSDI_ERROR;
  mmgsdi_file_enum_type              file_enum           = MMGSDI_NO_FILE_ENUM;
  mmgsdi_data_type                   read_data           = {0, NULL};
  mmgsdi_len_type                    file_size = 0;
  uint32                             record_num          = 0;
  uint16                             num_records         = 0;

  memset(&read_data, 0x00, sizeof(mmgsdi_data_type));
  
  if ((cmd_ptr == NULL)||(uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type));
      /* extract data from req packet and populate fields in the request structure */
      
  uimdiag_status = uimdiag_parse_cmd_data(&request, cmd_ptr, pkt_len); 
    if( uimdiag_status != UIMDIAG_SUCCESS)
    {
      return NULL;
    }
    /*  allocate immediate response pointer  */
    rsp_ptr = (uimdiag_generic_rsp_type *)
               diagpkt_subsys_alloc_v2(
               (diagpkt_subsys_id_type) DIAG_SUBSYS_GSDI,
               (diagpkt_subsys_cmd_code_type) 
               UIMDIAG_MMGSDI_SESSION_READ_FROM_CACHE_CMD,
               sizeof(uimdiag_generic_rsp_type)); 
	
    if( rsp_ptr == NULL)
  {
      return NULL;
  }

  diagpkt_subsys_reset_delayed_rsp_id(rsp_ptr); 
  (void)memset((void*)&rsp_ptr->rsp_payload, 0x00, sizeof(uimdiag_cmd_rsp_pyld_type));
  
  /* Retrieve request parameters */
  file_enum = uimdiag_util_convert_to_mmgsdi_items_enum(request.request.session_read_from_cache.ef_enum);
  record_num = request.request.session_read_from_cache.rec_num;

  /* Start filling the response structure */
  rsp_ptr->rsp_payload.response.session_read_from_cache.session_type = 
                                    request.request.session_read_from_cache.session_type;
  rsp_ptr->rsp_payload.response.session_read_from_cache.ef_enum = 
                                     request.request.session_read_from_cache.ef_enum;                                                
  rsp_ptr->rsp_payload.response.session_read_from_cache.start_record = record_num;
  rsp_ptr->rsp_payload.response.session_read_from_cache.more_data = 0;

  /* Retrieve file size */
  mmgsdi_status = mmgsdi_session_read_cache_file_size_by_session_type(
                      request.request.session_read_from_cache.session_type,
                      file_enum,
                      &file_size);

  if( mmgsdi_status != MMGSDI_SUCCESS )
  {
    rsp_ptr->rsp_payload.mmgsdi_status = mmgsdi_status;
    return rsp_ptr;
  }

  if(file_size < request.request.session_read_from_cache.offset)
  {
    UIMDIAG_MSG_ERROR_2("incorrect offset, offset:0x0%x, file_size:0x0%x", 
                         request.request.session_read_from_cache.offset,file_size);
    rsp_ptr->rsp_payload.mmgsdi_status = MMGSDI_ERROR;
    return rsp_ptr;
  }

  if((file_size - request.request.session_read_from_cache.offset) < UIMDIAG_MAX_DATA_BLOCK_LEN)
  {
    read_data.data_len = file_size - request.request.session_read_from_cache.offset;
  }
  else
  {
    read_data.data_len = UIMDIAG_MAX_DATA_BLOCK_LEN;
    rsp_ptr->rsp_payload.response.session_read_from_cache.more_data = 1;
  }
  
  UIMDIAG_UTIL_TMC_MEM_MALLOC_AND_VALIDATE(read_data.data_ptr,
                                           read_data.data_len);
    
  if(read_data.data_ptr == NULL)
  {
    uimdiag_status = UIMDIAG_MEMORY_ERROR_HEAP_EXHAUSTED;
    rsp_ptr->rsp_payload.mmgsdi_status = MMGSDI_ERROR;
    return rsp_ptr;
  } 

  if ( record_num != 0)
  {
    uint16   index = 0;
    do
    {  
      mmgsdi_status = mmgsdi_session_read_cache_by_session_type(
                      request.request.session_read_from_cache.session_type,
                      file_enum,
                      &read_data,
                      0,
                      record_num);
      
      if( mmgsdi_status == MMGSDI_SUCCESS )
      {
        num_records++;
        record_num++;
    
        memscpy(&(rsp_ptr->rsp_payload.response.session_read_from_cache.data_ptr[index]),
            UIMDIAG_MAX_DATA_BLOCK_LEN,
            read_data.data_ptr,
            read_data.data_len);    

        rsp_ptr->rsp_payload.response.session_read_from_cache.data_len = 
                                                            rsp_ptr->rsp_payload.response.session_read_from_cache.data_len + read_data.data_len;
        
        index = rsp_ptr->rsp_payload.response.session_read_from_cache.data_len;
        
        rsp_ptr->rsp_payload.mmgsdi_status = mmgsdi_status;   
        rsp_ptr->rsp_payload.response.session_read_from_cache.record_len = file_size;
      }
      else
      {
        if (num_records == 0)
        {
          rsp_ptr->rsp_payload.mmgsdi_status = mmgsdi_status; 
        }
        break;
      }
    } while ((mmgsdi_status == MMGSDI_SUCCESS) && rsp_ptr->rsp_payload.response.session_read_from_cache.data_len 
                          <= (UIMDIAG_MAX_DATA_BLOCK_LEN - read_data.data_len));

    rsp_ptr->rsp_payload.response.session_read_from_cache.num_records = num_records;

    /* If all records were read successfully until buffer was full, try to read 
       an additional record to indicate if more data is present in the response */
    if (mmgsdi_status == MMGSDI_SUCCESS)
    {
      mmgsdi_status = mmgsdi_session_read_cache_by_session_type(
                      request.request.session_read_from_cache.session_type,
                      file_enum,
                      &read_data,
                      0,
                      record_num);
       
      if(mmgsdi_status == MMGSDI_SUCCESS)
      {
        rsp_ptr->rsp_payload.response.session_read_from_cache.more_data = 1;
      }
    }
  }
  else
  {
    mmgsdi_status = mmgsdi_session_read_cache_by_session_type(request.request.session_read_from_cache.session_type,
                                                            file_enum,
                                                            &read_data,
                                                            request.request.session_read_from_cache.offset,
                                                            request.request.session_read_from_cache.rec_num);
                                                            
  if( mmgsdi_status == MMGSDI_SUCCESS )
  {
    rsp_ptr->rsp_payload.response.session_read_from_cache.data_len = 
                                                            read_data.data_len;
    
    memscpy(rsp_ptr->rsp_payload.response.session_read_from_cache.data_ptr,
            UIMDIAG_MAX_DATA_BLOCK_LEN,
            read_data.data_ptr,
            read_data.data_len);     
    }
    rsp_ptr->rsp_payload.mmgsdi_status = mmgsdi_status;
  }

  diagpkt_subsys_set_status(rsp_ptr, (uint32) UIMDIAG_SUCCESS);
  diagpkt_commit(rsp_ptr);
  UIMDIAGUTIL_TMC_MEM_FREE(read_data.data_ptr);

  return NULL;
}/* uimdiag_mmgsdi_session_read_from_cache_cmd */


/*===========================================================================
FUNCTION UIMDIAG_MMGSDI_SESSION_READ_TRANSPARENT_CMD

DESCRIPTION
  Command handler to build a request to MMGSDI to read transparent file

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_read_transparent_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr             = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id              = 0;
  uimdiag_return_enum_type           uimdiag_status      = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status  = UIMDIAG_ERROR;
  int32                              index               = 0;
  mmgsdi_return_enum_type            mmgsdi_status       = MMGSDI_ERROR;

  mmgsdi_session_id_type             session_id;
  mmgsdi_access_type                 file_access;
  mmgsdi_offset_type         offset;
  mmgsdi_len_type          req_len;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_read_transparent_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );
  (void)memset((void*)&file_access, 0x00, sizeof(mmgsdi_access_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_READ_TRANSPARENT_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_SESSION_READ_TRANSPARENT_CMD;

  session_id                = request.request.session_read_transparent.session_id_low;
  offset        = request.request.session_read_transparent.offset;
  req_len       = request.request.session_read_transparent.req_len;

  file_access.access_method = (mmgsdi_access_method_enum_type)
                              request.request.session_read_transparent.access_method;

  switch(file_access.access_method)
  {
  case MMGSDI_EF_ENUM_ACCESS:
       file_access.file.file_enum = uimdiag_util_convert_to_mmgsdi_items_enum(
                                      request.request.session_read_transparent.ef_enum);
       break;

  case MMGSDI_DF_ENUM_ACCESS:
       file_access.file.df_enum = (mmgsdi_df_enum_type)
                                  request.request.session_read_transparent.df_enum;
       break;

  case MMGSDI_BY_PATH_ACCESS:
       file_access.file.path_type.path_len = request.request.session_read_transparent.path_len;
       if ( (request.request.session_read_transparent.path_len > 0 ) &&
            (request.request.session_read_transparent.path_len <= MMGSDI_MAX_PATH_LEN) )
         {
          memscpy(file_access.file.path_type.path_buf,
          sizeof(file_access.file.path_type.path_buf),
          request.request.session_read_transparent.path_ptr,
         (request.request.session_read_transparent.path_len*sizeof(uint16)));
       }
       else
       {
         UIMDIAG_MSG_ERROR_0("path_len could be <= 0 or exceeds max path len size ");
         return NULL;
         }
       break;

  case MMGSDI_BY_APP_ID_ACCESS:
       file_access.file.app_id.data_len = request.request.session_read_transparent.aid_len;
       if (request.request.session_read_transparent.aid_len != 0 )
         {
          memscpy(file_access.file.app_id.data_ptr,
          sizeof(file_access.file.app_id.data_ptr),
          request.request.session_read_transparent.aid_ptr,
          request.request.session_read_transparent.aid_len);
         }
       break;

  default:
       break;

  }

  mmgsdi_status =   mmgsdi_session_read_transparent (
                      session_id,
                      file_access,
          offset,
          req_len,
                      (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                      (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_READ_TRANSPARENT_CMD
    );

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_read_transparent.path_ptr);
  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_read_transparent.aid_ptr);
  return rsp_ptr;
}/* uimdiag_mmgsdi_session_read_transparent_cmd */


/*===========================================================================
FUNCTION UIMDIAG_MMGSDI_SESSION_WRITE_RECORD_CMD

DESCRIPTION
  Command handler to build a request to MMGSDI to write record

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_write_record_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr             = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id              = 0;
  uimdiag_return_enum_type           uimdiag_status      = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status  = UIMDIAG_ERROR;
  int32                              index               = 0;
  mmgsdi_return_enum_type            mmgsdi_status       = MMGSDI_ERROR;

  mmgsdi_session_id_type             session_id;
  mmgsdi_access_type                 file_access;
  mmgsdi_file_structure_enum_type    record_type;
  mmgsdi_rec_num_type                record_number;
  mmgsdi_write_data_type             write_data;



  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_write_record_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );
  (void)memset((void*)&file_access, 0x00, sizeof(mmgsdi_access_type) );
  (void)memset((void*)&write_data, 0x00, sizeof(mmgsdi_write_data_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_WRITE_RECORD_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_SESSION_WRITE_RECORD_CMD;

  session_id          = (mmgsdi_session_id_type)
                          request.request.session_write_record.session_id_low;
  record_type       = (mmgsdi_file_structure_enum_type)
                          request.request.session_write_record.rec_type;
  record_number       = request.request.session_write_record.rec_num;
  write_data.data_len = request.request.session_write_record.data_len;

  if(request.request.session_write_record.data_len != 0)
  {
    write_data.data_ptr = request.request.session_write_record.data_ptr;
  }
  else
  {
    UIMDIAG_MSG_ERROR_0("request.request.session_write_record.data_len == 0");
  }

  file_access.access_method     = (mmgsdi_access_method_enum_type) request.request.session_write_record.access_method;

  switch(file_access.access_method)
  {
  case MMGSDI_EF_ENUM_ACCESS:
       file_access.file.file_enum = uimdiag_util_convert_to_mmgsdi_items_enum(
                                      request.request.session_write_record.ef_enum);
       break;

  case MMGSDI_DF_ENUM_ACCESS:
       file_access.file.df_enum = (mmgsdi_df_enum_type)
                                  request.request.session_write_record.df_enum;
       break;

  case MMGSDI_BY_PATH_ACCESS:
       file_access.file.path_type.path_len = request.request.session_write_record.path_len;
       if ( (request.request.session_write_record.path_len > 0) &&
            (request.request.session_write_record.path_len <= MMGSDI_MAX_PATH_LEN))
         {
          memscpy(file_access.file.path_type.path_buf,
          sizeof(file_access.file.path_type.path_buf),
          request.request.session_write_record.path_ptr,
          (request.request.session_write_record.path_len*sizeof(uint16)));
        }
       else
       {
         UIMDIAG_MSG_ERROR_0("path_len could be <= 0 or exceeds max path len size ");
         return NULL;
        }
       break;

  case MMGSDI_BY_APP_ID_ACCESS:
       file_access.file.app_id.data_len = request.request.session_write_record.aid_len;
       if (request.request.session_write_record.aid_len != 0 )
         {
          memscpy(file_access.file.app_id.data_ptr,
          sizeof(file_access.file.app_id.data_ptr),
          request.request.session_write_record.aid_ptr,
          request.request.session_write_record.aid_len);
         }
       break;

  default:
       break;
  }

  mmgsdi_status =   mmgsdi_session_write_record (
                      session_id,
                      file_access,
          record_type,
          record_number,
          write_data,
                      (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                      (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_WRITE_RECORD_CMD
    );

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_write_record.path_ptr);
  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_write_record.aid_ptr);
  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_write_record.data_ptr);
  return rsp_ptr;
}/* uimdiag_mmgsdi_session_write_record_cmd */


/*===========================================================================
FUNCTION UIMDIAG_MMGSDI_SESSION_WRITE_TRANSPARENT_CMD

DESCRIPTION
  Command handler to build a request to MMGSDI to write to transparent file.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_write_transparent_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr             = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id              = 0;
  uimdiag_return_enum_type           uimdiag_status      = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status  = UIMDIAG_ERROR;
  int32                              index               = 0;
  mmgsdi_return_enum_type            mmgsdi_status       = MMGSDI_ERROR;

  mmgsdi_session_id_type             session_id;
  mmgsdi_access_type                 file_access;
  mmgsdi_offset_type         offset;
  mmgsdi_write_data_type             write_data;



  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_write_transparent_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );
  (void)memset((void*)&file_access, 0x00, sizeof(mmgsdi_access_type) );
  (void)memset((void*)&write_data, 0x00, sizeof(mmgsdi_write_data_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_WRITE_TRANSPARENT_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_SESSION_WRITE_TRANSPARENT_CMD;

  session_id                          = request.request.session_write_transparent.session_id_low;
  offset                = request.request.session_write_transparent.offset;
  write_data.data_len                 = request.request.session_write_transparent.data_len;

  if(request.request.session_write_transparent.data_len != 0)
  {
    write_data.data_ptr             = request.request.session_write_transparent.data_ptr;
  }
  else
  {
    UIMDIAG_MSG_ERROR_0("request.request.session_write_transparent.data_len == 0");
  }

  file_access.access_method = (mmgsdi_access_method_enum_type)
                              request.request.session_write_transparent.access_method;

  switch(file_access.access_method)
  {
  case MMGSDI_EF_ENUM_ACCESS:
       file_access.file.file_enum = uimdiag_util_convert_to_mmgsdi_items_enum(
                                      request.request.session_write_transparent.ef_enum);
       break;

  case MMGSDI_DF_ENUM_ACCESS:
       file_access.file.df_enum = (mmgsdi_df_enum_type)
                                  request.request.session_write_transparent.df_enum;
       break;

  case MMGSDI_BY_PATH_ACCESS:
       file_access.file.path_type.path_len = request.request.session_write_transparent.path_len;
       if ( (request.request.session_write_transparent.path_len > 0) &&
            (request.request.session_write_transparent.path_len <= MMGSDI_MAX_PATH_LEN) )
       {
         memscpy(file_access.file.path_type.path_buf,
           sizeof(file_access.file.path_type.path_buf),
           request.request.session_write_transparent.path_ptr,
           (request.request.session_write_transparent.path_len*sizeof(uint16)));
       }
       else
       {
         UIMDIAG_MSG_ERROR_0("path_len could be <= 0 or exceeds max path len size ");
         return NULL;
       }
       break;

  case MMGSDI_BY_APP_ID_ACCESS:
       file_access.file.app_id.data_len = request.request.session_write_transparent.aid_len;
       if (request.request.session_write_transparent.aid_len != 0 )
       {
         memscpy(file_access.file.app_id.data_ptr,
         sizeof(file_access.file.app_id.data_ptr),
         request.request.session_write_transparent.aid_ptr,
         request.request.session_write_transparent.aid_len);
       }
       break;

  default:
       break;
  }

  mmgsdi_status =   mmgsdi_session_write_transparent (
                      session_id,
                      file_access,
            offset,
            write_data,
            (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                      (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_WRITE_TRANSPARENT_CMD
    );


  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_write_transparent.path_ptr);
  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_write_transparent.aid_ptr);
  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_write_transparent.data_ptr);
  return rsp_ptr;
}/* uimdiag_mmgsdi_session_write_transparent_cmd */


#ifdef FEATURE_UIMDIAG_INTERNAL_API
/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_REG_FOR_REFRESH_CMD

DESCRIPTION
  Command handler to build a request to register for Refresh to MMGSDI

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_reg_for_refresh_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr             = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id              = 0;
  int32                              index               = 0;
  uint16                             file_index          = 0;
  uimdiag_return_enum_type           uimdiag_status      = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status  = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status       = MMGSDI_ERROR;
  mmgsdi_session_id_type             session_id;
  mmgsdi_refresh_file_list_type      refresh_files;
  boolean                            vote_for_init;
  uint16                             invalid_files       = 0;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_reg_for_refresh_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  (void)memset ((void*)&refresh_files, 0x00,
                 sizeof(mmgsdi_refresh_file_list_type));

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_REG_FOR_REFRESH_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd =
                                    UIMDIAG_MMGSDI_SESSION_REG_FOR_REFRESH_CMD;

  session_id                  =
                        request.request.session_refresh.session_id_low;
  refresh_files.num_files     =
                         request.request.session_refresh.num_of_files;

  if(request.request.session_refresh.num_of_files == 0)
  {
    UIMDIAG_MSG_ERROR_0("UIMDIAG: Invalid number of files");
    return NULL;
  }

  UIMDIAG_UTIL_TMC_MEM_MALLOC_AND_VALIDATE(
    refresh_files.file_list_ptr,
    (request.request.session_refresh.num_of_files * sizeof(mmgsdi_file_enum_type)));
  if(refresh_files.file_list_ptr == NULL)
  {
    return NULL;
  }

  for (file_index = 0; file_index < request.request.session_refresh.num_of_files; file_index++)
  {
    refresh_files.file_list_ptr[file_index] =
                                     uimdiag_util_convert_to_mmgsdi_items_enum(
                                       request.request.session_refresh.file_list_ptr[file_index]);

    if(refresh_files.file_list_ptr[file_index] == MMGSDI_NO_FILE_ENUM)
    {
      invalid_files++;
    }
  }

  /* If all files in the refresh file list are invalid then we should return error */
  if(invalid_files == request.request.session_refresh.num_of_files)
  {
    UIMDIAG_MSG_ERROR_0("UIMDIAG Invalid files to register");
    UIMDIAGUTIL_TMC_MEM_FREE(refresh_files.file_list_ptr);
    return NULL;
  }

  refresh_files.file_path_len =
                         request.request.session_refresh.file_path_len;
  refresh_files.file_path_ptr =
                         request.request.session_refresh.file_path_ptr;
  vote_for_init               = (boolean)
                         request.request.session_refresh.vote_for_init;

  mmgsdi_status               =   mmgsdi_session_register_for_refresh  (
                                    session_id,
                                    refresh_files,
                                    vote_for_init,
                                    (mmgsdi_callback_type)
                                      uimdiag_mmgsdi_generic_rsp_cb,
                                    (mmgsdi_client_data_type)index);

  UIMDIAGUTIL_TMC_MEM_FREE(refresh_files.file_list_ptr);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_REG_FOR_REFRESH_CMD
    );

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_refresh.file_list_ptr);
  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_refresh.file_path_ptr);

  return rsp_ptr;
}/* uimdiag_mmgsdi_session_reg_for_refresh_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_REG_FOR_REFRESH_PROCEEED_VOTING_CMD

DESCRIPTION
  Command handler to build a request to register for Refresh to MMGSDI aling
  with proceed voting

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_reg_for_refresh_proceed_voting_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr             = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id              = 0;
  int32                              index               = 0;
  uimdiag_return_enum_type           uimdiag_status      = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status  = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status       = MMGSDI_ERROR;
  mmgsdi_session_id_type             session_id;
  mmgsdi_refresh_file_list_type      refresh_files;
  boolean                            vote_for_init;
  boolean                            proceed_with_refresh;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_reg_for_refresh_proceed_voting_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  (void)memset ((void*)&refresh_files, 0x00,
                 sizeof(mmgsdi_refresh_file_list_type));

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGDDI_SESSION_REGISTER_FOR_REFRESH_PROCEED_VOTING_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd =
                                    UIMDIAG_MMGDDI_SESSION_REGISTER_FOR_REFRESH_PROCEED_VOTING_CMD;

  session_id                  =
                        request.request.session_refresh_proceed_voting.session_id_low;
  refresh_files.num_files     =
                         request.request.session_refresh_proceed_voting.num_of_files;
  refresh_files.file_list_ptr = (mmgsdi_file_enum_type *)
                         request.request.session_refresh_proceed_voting.file_list_ptr;
  refresh_files.file_path_len =
                         request.request.session_refresh_proceed_voting.file_path_len;
  refresh_files.file_path_ptr =
                         request.request.session_refresh_proceed_voting.file_path_ptr;
  vote_for_init               = (boolean)
                         request.request.session_refresh_proceed_voting.vote_for_init;
  proceed_with_refresh        = (boolean)
                         request.request.session_refresh_proceed_voting.proceed_refresh;


 mmgsdi_status = mmgsdi_session_register_for_refresh_ok_to_proceed_voting(
                    session_id,
                    refresh_files,
                    vote_for_init,
                    proceed_with_refresh,
                    (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                    (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGDDI_SESSION_REGISTER_FOR_REFRESH_PROCEED_VOTING_CMD
    );

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_refresh.file_list_ptr);
  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_refresh.file_path_ptr);

  return rsp_ptr;
}/* uimdiag_mmgsdi_session_reg_for_refresh_proceed_voting_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_OK_TO_REFRESH_CMD

DESCRIPTION
  Command handler to build a request to simulate OK to Refresh to MMGSDI

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_ok_to_refresh_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr             = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id              = 0;
  int32                              index               = 0;
  uimdiag_return_enum_type           uimdiag_status      = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status  = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status       = MMGSDI_ERROR;
  mmgsdi_session_id_type             session_id;
  boolean                            ok_to_refresh;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_ok_to_refresh_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_OK_TO_REFRESH_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd =
                                    UIMDIAG_MMGSDI_SESSION_OK_TO_REFRESH_CMD;

  session_id                  =
                        request.request.session_ok_to_refresh.session_id_low;
  ok_to_refresh               = (boolean)
                         request.request.session_ok_to_refresh.ok_to_refresh;

  mmgsdi_status               =   mmgsdi_session_ok_to_refresh  (
                                    session_id,
                                    ok_to_refresh,
                                    (mmgsdi_callback_type)
                                      uimdiag_mmgsdi_generic_rsp_cb,
                                    (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_OK_TO_REFRESH_CMD
    );

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_session_ok_to_refresh_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_OK_TO_PROCEED_WITH_REFRESH_CMD

DESCRIPTION
  Command handler to build a request to simulate proceed with Refresh to MMGSDI

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_ok_to_proceed_with_refresh_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr             = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id              = 0;
  int32                              index               = 0;
  uimdiag_return_enum_type           uimdiag_status      = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status  = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status       = MMGSDI_ERROR;
  mmgsdi_session_id_type             session_id;
  boolean                            ok_to_proceed;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_ok_to_refresh_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_OK_TO_PROCEED_WITH_REFRESH_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_SESSION_OK_TO_PROCEED_WITH_REFRESH_CMD;

  session_id    = request.request.session_refresh_ok_to_proceed.session_id_low;
  ok_to_proceed = (boolean)request.request.session_refresh_ok_to_proceed.ok_to_proceed;

  mmgsdi_status = mmgsdi_session_ok_to_proceed_with_refresh(
                    session_id,
                    ok_to_proceed,
                    (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                    (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_OK_TO_PROCEED_WITH_REFRESH_CMD
    );

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_session_ok_to_refresh_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_REFRESH_COMPLETE_CMD

DESCRIPTION
  Command handler to build a request to indicate to MMGSDI that the client
  has finished its refresh process

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_refresh_complete_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  int32                              index              = 0;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_session_id_type             session_id;
  boolean                            pass_fail;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_refresh_complete_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_REFRESH_COMPLETE_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd =
                                    UIMDIAG_MMGSDI_SESSION_REFRESH_COMPLETE_CMD;

  session_id                  =
                        request.request.session_refresh_complete.session_id_low;
  pass_fail                   = (boolean)
                         request.request.session_refresh_complete.pass_fail;

  mmgsdi_status               =   mmgsdi_session_refresh_complete   (
                                    session_id,
                                    pass_fail,
                                    (mmgsdi_callback_type)
                                      uimdiag_mmgsdi_generic_rsp_cb,
                                    (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_REFRESH_COMPLETE_CMD
    );

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_session_refresh_complete_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_DEREG_FOR_REFRESH_CMD

DESCRIPTION
  Command handler to build a request to deregister files that were registered
  for Refresh to MMGSDI

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_dereg_for_refresh_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  int32                              index              = 0;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_session_id_type             session_id;
  mmgsdi_refresh_file_list_type      refresh_files;
  boolean                            vote_for_init;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_dereg_for_refresh_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );
  (void)memset((void*)&refresh_files, 0x00, sizeof(mmgsdi_refresh_file_list_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_DEREG_FOR_REFRESH_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd =
                                    UIMDIAG_MMGSDI_SESSION_DEREG_FOR_REFRESH_CMD;

  session_id                  =
                        request.request.session_refresh.session_id_low;
  refresh_files.num_files     =
                         request.request.session_refresh.num_of_files;
  refresh_files.file_list_ptr = (mmgsdi_file_enum_type *)
                         request.request.session_refresh.file_list_ptr;
  refresh_files.file_path_len =
                         request.request.session_refresh.file_path_len;
  refresh_files.file_path_ptr =
                         request.request.session_refresh.file_path_ptr;
  vote_for_init               = (boolean)
                         request.request.session_refresh.vote_for_init;

  mmgsdi_status               =   mmgsdi_session_deregister_for_refresh  (
                                    session_id,
                                    refresh_files,
                                    vote_for_init,
                                    (mmgsdi_callback_type)
                                      uimdiag_mmgsdi_generic_rsp_cb,
                                    (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
                         &rsp_ptr,
                         rsp_id,
                         uimdiag_status,
                         index,
                         mmgsdi_status,
                         &request,
                         UIMDIAG_MMGSDI_SESSION_DEREG_FOR_REFRESH_CMD);

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_refresh.file_list_ptr);
  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_refresh.file_path_ptr);

  return rsp_ptr;
}/* uimdiag_mmgsdi_session_dereg_for_refresh_cmd */
#endif /* FEATURE_UIMDIAG_INTERNAL_API */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_DEACT_PROV_CMD

DESCRIPTION
  Command handler to build a request that allows the client to deactivate
  an application.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_deact_prov_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  int32                              index              = 0;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_session_id_type             session_id;


  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_deact_prov_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_DEACT_PROV_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd =
                                    UIMDIAG_MMGSDI_SESSION_DEACT_PROV_CMD;

  session_id         = request.request.session_deact_prov.session_id_low;
  mmgsdi_status      =  mmgsdi_session_deactivate_provisioning (
                          session_id,
                          (mmgsdi_callback_type)
                          uimdiag_mmgsdi_generic_rsp_cb,
                          (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_DEACT_PROV_CMD
    );

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_session_deact_prov_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_GET_ALL_AVAIL_APPS_SYNC_EXT_CMD

DESCRIPTION
  Command handler to get all available apps.

DEPENDENCIES
  None.

RETURN VALUE
  void pointer.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_get_all_avail_apps_sync_ext_cmd (
  PACKED void  *cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type               request;
  uimdiag_generic_rsp_type          	 *rsp_ptr            = NULL;
  uimdiag_return_enum_type               uimdiag_status      = UIMDIAG_ERROR;
  mmgsdi_return_enum_type                mmgsdi_status       = MMGSDI_ERROR;
  mmgsdi_client_id_type                  client_id           = MMGSDI_CLIENT_ID_ZERO;
  mmgsdi_available_apps_type            *available_apps_ptr  = NULL;

  UIM_MSG_HIGH_0("In Uimdiag_mmgsdi_get_all_avail_apps_sync_ext_cmd");
  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if(cmd_ptr == NULL)
  {
    return NULL;
  }

  /* extract data from req packet and populate fields in the request structure */
  uimdiag_status = uimdiag_parse_cmd_data(&request, cmd_ptr, pkt_len);
  if( uimdiag_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }
  /*  allocate immediate response pointer  */
  rsp_ptr = (uimdiag_generic_rsp_type *)
             diagpkt_subsys_alloc_v2(
             (diagpkt_subsys_id_type) DIAG_SUBSYS_GSDI,
             (diagpkt_subsys_cmd_code_type) 
             UIMDIAG_MMGSDI_GET_ALL_AVAIL_APPS_SYNC_EXT_CMD,
             sizeof(uimdiag_generic_rsp_type));

  if( rsp_ptr == NULL)
  {
    return NULL;
  }

  diagpkt_subsys_reset_delayed_rsp_id(rsp_ptr); 

  (void)memset((void*)&rsp_ptr->rsp_payload, 0x00, sizeof(uimdiag_cmd_rsp_pyld_type));

  UIMDIAG_UTIL_TMC_MEM_MALLOC_AND_VALIDATE(available_apps_ptr,
                                           sizeof(mmgsdi_available_apps_type));  
  
  if(available_apps_ptr == NULL)
  {
    return NULL;
  }
  
  mmgsdi_status = mmgsdi_get_all_available_apps_sync(client_id, available_apps_ptr);

  if(mmgsdi_status == MMGSDI_SUCCESS)
  {
    uimdiag_status = uimdiag_mmgsdi_get_all_avail_apps_ext_cnf(rsp_ptr,
                                                               available_apps_ptr);
  }

  rsp_ptr->rsp_payload.mmgsdi_status = mmgsdi_status;
  diagpkt_subsys_set_status(rsp_ptr, (uint32) UIMDIAG_SUCCESS);
  diagpkt_commit(rsp_ptr);
  UIMDIAGUTIL_TMC_MEM_FREE(available_apps_ptr);
  
  return NULL;
}/* uimdiag_mmgsdi_get_all_avail_apps_sync_ext_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_GET_APP_CAP_CMD

DESCRIPTION
  Command handler to build a request that allows the client to get application
  capabilities.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_get_app_cap_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  int32                              index              = 0;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_session_id_type             session_id;


  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_get_app_cap_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_GET_APP_CAP_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd =
                                    UIMDIAG_MMGSDI_SESSION_GET_APP_CAP_CMD;

  session_id         = request.request.session_get_app_cap.session_id_low;
  mmgsdi_status      = mmgsdi_session_get_app_capabilities(
                         session_id,
                         (mmgsdi_callback_type)
                         uimdiag_mmgsdi_generic_rsp_cb,
                         (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_GET_APP_CAP_CMD
    );

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_session_get_app_cap_cmd */


#ifdef FEATURE_UIMDIAG_INTERNAL_API
/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_DISABLE_SERVICE_CMD

DESCRIPTION
  Command handler for disabling a particular service

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_disable_service_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  int32                              index              = 0;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_service_enum_type           service            = MMGSDI_NONE;
  mmgsdi_session_id_type             session_id;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_disable_service_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_DISABLE_SERVICE_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd =
                                    UIMDIAG_MMGSDI_SESSION_DISABLE_SERVICE_CMD;

  session_id         = request.request.session_disable_srv.session_id_low;
  service            = (mmgsdi_service_enum_type)
                         request.request.session_disable_srv.service;

   mmgsdi_status = mmgsdi_session_disable_service (
                    session_id,
                    service,
                    (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                    (mmgsdi_client_data_type)index);

   delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_DISABLE_SERVICE_CMD);

  if( delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_session_disable_service_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_ENABLE_SERVICE_CMD

DESCRIPTION
  Command handler for enable service

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_enable_service_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  int32                              index              = 0;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_service_enum_type           service            = MMGSDI_NONE;
  mmgsdi_session_id_type             session_id;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_enable_service_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_ENABLE_SERVICE_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd =
                                    UIMDIAG_MMGSDI_SESSION_ENABLE_SERVICE_CMD;

  session_id         = request.request.session_enable_srv.session_id_low;
  service            = (mmgsdi_service_enum_type)
                         request.request.session_enable_srv.service;

  mmgsdi_status = mmgsdi_session_enable_service (
                    session_id,
                    service,
                    (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                    (mmgsdi_client_data_type)index);

   delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_ENABLE_SERVICE_CMD);

  if( delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_session_enable_service_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_IS_SERVICE_AVAIL_CMD

DESCRIPTION
  Command handler to determine if service is available or not.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_is_service_avail_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  int32                              index              = 0;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_service_enum_type           service_type       = MMGSDI_NONE;
  mmgsdi_session_id_type             session_id;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_is_service_available_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_IS_SERVICE_AVAIL_CMD);

   if(uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd =
                                    UIMDIAG_MMGSDI_SESSION_IS_SERVICE_AVAIL_CMD;

  session_id         = request.request.session_is_srv_avail.session_id_low;
  service_type       = (mmgsdi_service_enum_type)
                         request.request.session_is_srv_avail.service;

  mmgsdi_status = mmgsdi_session_is_service_available (
                    session_id,
                    service_type,
                    (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                    (mmgsdi_client_data_type)index);

   delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_IS_SERVICE_AVAIL_CMD);

  if( delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_session_is_service_avail_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_PROV_APP_INIT_COMPLETE_CMD

DESCRIPTION
  Notifies the MMGSDI that the client has finished its initialization procedure
  for the application.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_prov_app_init_complete_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  int32                              index              = 0;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_session_id_type             session_id;
  boolean                            init_status;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_prov_app_init_complete_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_PROV_APP_INIT_COMPLETE_CMD);

   if( uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd =
    UIMDIAG_MMGSDI_SESSION_PROV_APP_INIT_COMPLETE_CMD;

  session_id        =
    request.request.session_prov_app_init_complete.session_id_low;
  init_status       =
    (mmgsdi_service_enum_type)
      request.request.session_prov_app_init_complete.init_status;

  mmgsdi_status = mmgsdi_session_prov_app_init_complete (
                    session_id,
                    init_status,
                    (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                    (mmgsdi_client_data_type)index);

   delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_PROV_APP_INIT_COMPLETE_CMD);

  if( delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_session_prov_app_init_complete_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_READ_PRL_CMD

DESCRIPTION
  Gets the IS-683-C or IS-683-A PRL from either the EF Extended
  PRL (EF-EPRL) or the EF-PRL when the EF-EPRL does not contain a valid PRL.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_read_prl_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  int32                              index              = 0;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_session_id_type             session_id;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_read_prl_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_READ_PRL_CMD);

   if( uimdiag_status != UIMDIAG_SUCCESS)
   {
     return rsp_ptr;
   }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd =
                                    UIMDIAG_MMGSDI_SESSION_READ_PRL_CMD;

  session_id         = request.request.session_read_prl.session_id_low;

  mmgsdi_status = mmgsdi_session_read_prl(
                    session_id,
                    (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                    (mmgsdi_client_data_type)index);

   delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_READ_PRL_CMD);

  if( delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_session_read_prl_cmd */
#endif /* FEATURE_UIMDIAG_INTERNAL_API */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_GET_INFO_SYNC_CMD

DESCRIPTION
This function allows a client to query MMGSDI for session information by
Session ID or Session Type. A client can get Session Type, Slot Number,
Channel ID and AID using this request.


DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_get_info_sync_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  uimdiag_generic_rsp_type          *delayed_rsp_ptr    = NULL;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_session_info_query_type     session_info_query;
  mmgsdi_session_get_info_type       session_get_info;


  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_get_info_sync_cmd");
  memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );
  memset((void*)&session_info_query, 0x00, sizeof(mmgsdi_session_info_query_type) );

  if( cmd_ptr == NULL)
  {
    return NULL;
  }

  /* extract data from req packet
   and populate fields in the request structure */
  uimdiag_status = uimdiag_parse_cmd_data(&request, cmd_ptr, pkt_len);

  if( uimdiag_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  /*  allocate response pointer  */
  rsp_ptr = (uimdiag_generic_rsp_type *)
                diagpkt_subsys_alloc_v2(
                  (diagpkt_subsys_id_type) DIAG_SUBSYS_GSDI,
                  (diagpkt_subsys_cmd_code_type) UIMDIAG_MMGSDI_SESSION_GET_INFO_SYNC_CMD,
                  sizeof(uimdiag_generic_rsp_type));

  if (rsp_ptr == NULL)
  {
    return rsp_ptr;
  }

  rsp_id = diagpkt_subsys_get_delayed_rsp_id(rsp_ptr);
  session_info_query.query_type = (mmgsdi_session_info_enum_type)
      request.request.session_get_info_sync.query_type;
  if( session_info_query.query_type == MMGSDI_SESSION_INFO_QUERY_BY_ID)
  {
    session_info_query.query_key.session_id =
      request.request.session_get_info_sync.session_id_low;
  }
  else if( session_info_query.query_type == MMGSDI_SESSION_INFO_QUERY_BY_TYPE)
  {
    session_info_query.query_key.session_type =
      (mmgsdi_session_type_enum_type)
        request.request.session_get_info_sync.session_type;
  }
  mmgsdi_status = mmgsdi_session_get_info_sync(
                    session_info_query,
                    &session_get_info);

  diagpkt_subsys_set_status(rsp_ptr, (uint32) UIMDIAG_SUCCESS);
  diagpkt_commit(rsp_ptr);

  /* allocate delayed response pointer */
  delayed_rsp_ptr = (uimdiag_generic_rsp_type *)
              diagpkt_subsys_alloc_v2_delay(
              (diagpkt_subsys_id_type)DIAG_SUBSYS_GSDI,
              (diagpkt_subsys_cmd_code_type) UIMDIAG_MMGSDI_SESSION_GET_INFO_SYNC_CMD,
              rsp_id,
              sizeof(uimdiag_generic_rsp_type));

  if (delayed_rsp_ptr == NULL)
  {
    return NULL;
  }

  (void)memset((void*)&delayed_rsp_ptr->rsp_payload,0x00,sizeof(uimdiag_cmd_rsp_pyld_type));

  delayed_rsp_ptr->rsp_payload.mmgsdi_status = mmgsdi_status;

  if( mmgsdi_status == MMGSDI_SUCCESS )
  {
    uimdiag_status =
      uimdiag_mmgsdi_session_get_info_sync_cnf(delayed_rsp_ptr,&session_get_info);
  }
  diagpkt_delay_commit(delayed_rsp_ptr);
  return NULL;
}/* uimdiag_mmgsdi_session_get_info_sync_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_GET_APP_CAPABILITIES_SYNC_CMD

DESCRIPTION
  Command handler to extract all the provisioning application
  capabilities like fdn, bdn, acl, imsi status and phone book related.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_get_app_capabilities_sync_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_session_app_capabilities_cnf_type       app_capabilities_sync;
  mmgsdi_session_id_type             session_id;
  diagpkt_subsys_delayed_rsp_id_type    rsp_id          = 0;
  uimdiag_generic_rsp_type          * delayed_rsp_ptr   = NULL;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_get_app_capabilities_sync_cmd");
  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type));
  (void)memset((void*)&app_capabilities_sync, 0x00, sizeof(mmgsdi_session_app_capabilities_cnf_type));

  if( cmd_ptr == NULL)
  {
    return NULL;
  }

  /* extract data from req packet
   and populate fields in the request structure */
  uimdiag_status = uimdiag_parse_cmd_data(&request, cmd_ptr, pkt_len);

  if( uimdiag_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  /*  allocate response pointer  */
  rsp_ptr = (uimdiag_generic_rsp_type *)
                diagpkt_subsys_alloc_v2(
                  (diagpkt_subsys_id_type) DIAG_SUBSYS_GSDI,
                  (diagpkt_subsys_cmd_code_type) UIMDIAG_MMGSDI_SESSION_GET_APP_CAPABILITIES_SYNC_CMD,
                  sizeof(uimdiag_generic_rsp_type));

  if( rsp_ptr == NULL)
  {
    return NULL;
  }

  rsp_id = diagpkt_subsys_get_delayed_rsp_id(rsp_ptr );

  session_id    = request.request.session_get_app_cap_sync.session_id_low;
  mmgsdi_status = mmgsdi_session_get_app_capabilities_sync(
                    session_id,
                    &app_capabilities_sync);

  diagpkt_subsys_set_status(rsp_ptr, (uint32) UIMDIAG_SUCCESS);
  diagpkt_commit(rsp_ptr);

/* allocate delayed response pointer */
  delayed_rsp_ptr = (uimdiag_generic_rsp_type *)
              diagpkt_subsys_alloc_v2_delay(
              (diagpkt_subsys_id_type) DIAG_SUBSYS_GSDI,
              (diagpkt_subsys_cmd_code_type) UIMDIAG_MMGSDI_SESSION_GET_APP_CAPABILITIES_SYNC_CMD,
              rsp_id,
              sizeof(uimdiag_generic_rsp_type));

  if (delayed_rsp_ptr == NULL)
  {
    return NULL;
  }

  (void)memset((void*)&delayed_rsp_ptr->rsp_payload,0x00,sizeof(uimdiag_cmd_rsp_pyld_type));
   delayed_rsp_ptr->rsp_payload.mmgsdi_status = mmgsdi_status;

  if( mmgsdi_status == MMGSDI_SUCCESS )
  {
    delayed_rsp_ptr->rsp_payload.response.session_get_app_cap_sync_rsp.fdn_enabled =
      app_capabilities_sync.fdn_enabled;

    delayed_rsp_ptr->rsp_payload.response.session_get_app_cap_sync_rsp.bdn_enabled =
      app_capabilities_sync.bdn_enabled;

    delayed_rsp_ptr->rsp_payload.response.session_get_app_cap_sync_rsp.acl_enabled =
      app_capabilities_sync.acl_enabled;

    delayed_rsp_ptr->rsp_payload.response.session_get_app_cap_sync_rsp.imsi_invalidated =
      app_capabilities_sync.imsi_invalidated;

  }
  diagpkt_delay_commit(delayed_rsp_ptr);
  return NULL;
}/* uimdiag_mmgsdi_session_get_app_capabilities_sync_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_CARD_PDOWN_CMD

DESCRIPTION
  Command handler to extract data for CARD PDOWN

DEPENDENCIES
  None.
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_card_pdown_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type            request;
  diagpkt_subsys_delayed_rsp_id_type  rsp_id             = 0;
  int32                               index              = 0;
  uimdiag_generic_rsp_type          * rsp_ptr            = NULL;
  uimdiag_return_enum_type            uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type            delayed_rsp_status = UIMDIAG_ERROR;
  mmgsdi_return_enum_type             mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_client_id_type               client_id          = 0;
  mmgsdi_slot_id_enum_type            slot_id            = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_card_pdown_options_enum_type pdown_option       = MMGSDI_CARD_PDOWN_MAX_ENUM;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_card_pdown_cmd");
  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  /* extract data from req packet
   and populate fields in the request structure */
  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_CARD_PDOWN_CMD);

  if( uimdiag_status != UIMDIAG_SUCCESS)
  {
    UIMDIAG_MSG_ERROR_1("uimdiag_mmgsdi_parse_commands: Parsing error 0x%x",
                        uimdiag_status);
    return rsp_ptr;
  }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd =
                                    UIMDIAG_MMGSDI_CARD_PDOWN_CMD;

  UIMDIAG_TOUINT64(client_id,request.request.card_pdown.client_id_low,
                   request.request.card_pdown.client_id_high);

  slot_id = (mmgsdi_slot_id_enum_type)request.request.card_pdown.slot_id;
  pdown_option = (mmgsdi_card_pdown_options_enum_type)request.request.card_pdown.power_down_option;

  mmgsdi_status = mmgsdi_card_pdown(
                    client_id,
                    slot_id,
                    (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                    pdown_option,
                    (mmgsdi_client_data_type)index);


  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_CARD_PDOWN_CMD);

  if( delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_card_pdown_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_CARD_PUP_CMD

DESCRIPTION
  Command handler to extract data for CARD PUP

DEPENDENCIES
  None.
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_card_pup_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type            request;
  diagpkt_subsys_delayed_rsp_id_type  rsp_id             = 0;
  int32                               index              = 0;
  uimdiag_generic_rsp_type          * rsp_ptr            = NULL;
  uimdiag_return_enum_type            uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type            delayed_rsp_status = UIMDIAG_ERROR;
  mmgsdi_return_enum_type             mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_client_id_type               client_id          = 0;
  mmgsdi_slot_id_enum_type            slot_id            = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_card_pup_options_enum_type   pup_option         = MMGSDI_CARD_PUP_MAX_ENUM;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_card_pup_cmd");
  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  /* extract data from req packet
   and populate fields in the request structure */
  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_CARD_PUP_CMD);

  if( uimdiag_status != UIMDIAG_SUCCESS)
  {
    UIMDIAG_MSG_ERROR_1("uimdiag_mmgsdi_parse_commands: Parsing error 0x%x",
                        uimdiag_status);
    return rsp_ptr;
  }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd =
                                    UIMDIAG_MMGSDI_CARD_PUP_CMD;

  UIMDIAG_TOUINT64(client_id,request.request.card_pup.client_id_low,
                   request.request.card_pup.client_id_high);

  slot_id = (mmgsdi_slot_id_enum_type)request.request.card_pup.slot_id;
  pup_option = (mmgsdi_card_pup_options_enum_type)request.request.card_pup.power_up_option;

  mmgsdi_status = mmgsdi_card_pup(
                    client_id,
                    slot_id,
                    (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                    pup_option,
                    (mmgsdi_client_data_type)index);


  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_CARD_PUP_CMD);

  if( delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_card_pup_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_ACTIVATE_ONCHIP_CMD

DESCRIPTION
  Command handler to extract data for activating ONCHIP

DEPENDENCIES
  None.
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static PACKED void * uimdiag_mmgsdi_activate_onchip_cmd (
  PACKED void                     * cmd_ptr,
  word                              pkt_len,
  uimdiag_subsystem_cmd_enum_type   cmd_enum
)
{
  uimdiag_generic_req_type            request;
  diagpkt_subsys_delayed_rsp_id_type  rsp_id             = 0;
  int32                               index              = 0;
  uimdiag_generic_rsp_type          * rsp_ptr            = NULL;
  uimdiag_return_enum_type            uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type            delayed_rsp_status = UIMDIAG_ERROR;
  mmgsdi_return_enum_type             mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_client_id_type               client_id          = 0;
  mmgsdi_slot_id_enum_type            slot_id            = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_onchip_sim_config_data_type  onchip_sim_config;

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );
  (void)memset((void*)&onchip_sim_config, 0x00, sizeof(mmgsdi_onchip_sim_config_data_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  if(cmd_enum != UIMDIAG_MMGSDI_ACTIVATE_ONCHIP_SIM_CMD &&
     cmd_enum != UIMDIAG_MMGSDI_ACTIVATE_ONCHIP_USIM_CMD)
  {
    UIMDIAG_MSG_ERROR_1("invalid onchip command: cmd_enum = 0x%x", cmd_enum);
    return NULL;
  }

  /* extract data from req packet
   and populate fields in the request structure */
  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     cmd_enum);

  if( uimdiag_status != UIMDIAG_SUCCESS)
  {
    UIMDIAG_MSG_ERROR_1("uimdiag_mmgsdi_parse_commands: Parsing error 0x%x",
                        uimdiag_status);
    return rsp_ptr;
  }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = cmd_enum;

  UIMDIAG_TOUINT64(client_id,request.request.activate_onchip_sim.client_id_low,
                   request.request.activate_onchip_sim.client_id_high);

  slot_id = (mmgsdi_slot_id_enum_type)request.request.activate_onchip_sim.slot_id;

  onchip_sim_config.data_len = request.request.activate_onchip_sim.onchip_data_length;

  if(request.request.activate_onchip_sim.onchip_data_length != 0)
  {
    onchip_sim_config.data_ptr = request.request.activate_onchip_sim.onchip_data_ptr;
  }
  else
  {
    UIMDIAG_MSG_ERROR_0("request.request.activate_onchip_sim.onchip_data_len == 0");
  }

  if(cmd_enum == UIMDIAG_MMGSDI_ACTIVATE_ONCHIP_SIM_CMD)
  {
    mmgsdi_status = mmgsdi_activate_onchip_sim(
                      client_id,
                      slot_id,
                      onchip_sim_config,
                      (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                      (mmgsdi_client_data_type)index);
  }
  else
  {
    mmgsdi_status = mmgsdi_activate_onchip_usim(
                      client_id,
                      slot_id,
                      onchip_sim_config,
                      (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                      (mmgsdi_client_data_type)index);
  }

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    cmd_enum);

  if( delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(request.request.activate_onchip_sim.onchip_data_ptr);
  return rsp_ptr;
}/* uimdiag_mmgsdi_activate_onchip_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_ACTIVATE_ONCHIP_SIM_CMD

DESCRIPTION
  Command handler to extract data for ACTIVATE ONCHIP SIM

DEPENDENCIES
  None.
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_activate_onchip_sim_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_activate_onchip_sim_cmd");

  return uimdiag_mmgsdi_activate_onchip_cmd(
           cmd_ptr,
           pkt_len,
           UIMDIAG_MMGSDI_ACTIVATE_ONCHIP_SIM_CMD);
}/* uimdiag_mmgsdi_activate_onchip_sim_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_ACTIVATE_ONCHIP_USIM_CMD

DESCRIPTION
  Command handler to extract data for ACTIVATE ONCHIP USIM

DEPENDENCIES
  None.
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_activate_onchip_usim_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_activate_onchip_usim_cmd");

  return uimdiag_mmgsdi_activate_onchip_cmd(
           cmd_ptr,
           pkt_len,
           UIMDIAG_MMGSDI_ACTIVATE_ONCHIP_USIM_CMD);
}/* uimdiag_mmgsdi_activate_onchip_usim_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SEND_APDU_CMD

DESCRIPTION
  Command handler to send apdus

DEPENDENCIES
  None.
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_send_apdu_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type            request;
  diagpkt_subsys_delayed_rsp_id_type  rsp_id             = 0;
  int32                               index              = 0;
  uimdiag_generic_rsp_type          * rsp_ptr            = NULL;
  uimdiag_return_enum_type            uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type            delayed_rsp_status = UIMDIAG_ERROR;
  mmgsdi_return_enum_type             mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_client_id_type               client_id          = 0;
  mmgsdi_slot_id_enum_type            slot_id            = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_send_apdu_data_type          apdu_data;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_send_apdu_cmd");

  (void)memset((void *)&request, 0x00, sizeof(uimdiag_generic_req_type));
  (void)memset((void *)&apdu_data, 0x00, sizeof(mmgsdi_send_apdu_data_type));

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  /* extract data from req packet
   and populate fields in the request structure */
  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SEND_APDU_CMD);

  if( uimdiag_status != UIMDIAG_SUCCESS)
  {
    UIMDIAG_MSG_ERROR_1("uimdiag_mmgsdi_parse_commands: Parsing error 0x%x",
                        uimdiag_status);
    return rsp_ptr;
  }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd =
                                    UIMDIAG_MMGSDI_SEND_APDU_CMD;

  slot_id = (mmgsdi_slot_id_enum_type)request.request.send_apdu.slot_id;

  UIMDIAG_TOUINT64(client_id,request.request.send_apdu.client_id_low,
                   request.request.send_apdu.client_id_high);

  apdu_data.data_len = request.request.send_apdu.apdu_data_length;

  if(request.request.send_apdu.apdu_data_length != 0)
  {
    apdu_data.data_ptr = request.request.send_apdu.apdu_data_ptr;
  }
  else
  {
    UIMDIAG_MSG_ERROR_0("request.request.send_apdu.send_apdu_length == 0");
  }

  mmgsdi_status = mmgsdi_send_apdu(
                    client_id,
                    slot_id,
                    apdu_data,
                    (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                    (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SEND_APDU_CMD);

  if( delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(request.request.send_apdu.apdu_data_ptr);
  return rsp_ptr;
}/* uimdiag_mmgsdi_send_apdu_cmd */


/*===========================================================================
FUNCTION UIMDIAG_MMGSDI_GET_ATR_CMD

DESCRIPTION
  Command handler to extract data for GET ATR

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_get_atr_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  int32                              index              = 0;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_client_id_type              client_id          = 0;
  mmgsdi_slot_id_enum_type           card_slot          = MMGSDI_MAX_SLOT_ID_ENUM;
  uimdiag_sync_rsp_type              *sync_rsp_ptr      = NULL;
  uimdiag_generic_rsp_type           *rsp_ptr           = NULL;  
		 
  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type));

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  UIMDIAG_UTIL_TMC_MEM_MALLOC_AND_VALIDATE(sync_rsp_ptr,
    sizeof(uimdiag_sync_rsp_type));
  
  if(sync_rsp_ptr == NULL)
  {
    return NULL;
  }

  /* extract data from req packet and populate fields in the request structure */
  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_GET_ATR_CMD);

  if( rsp_ptr == NULL)
  {
    UIMDIAGUTIL_TMC_MEM_FREE(sync_rsp_ptr);
    return NULL;
  }

  diagpkt_subsys_reset_delayed_rsp_id(rsp_ptr); 
  
  if( uimdiag_status != UIMDIAG_SUCCESS)
  {
    UIMDIAGUTIL_TMC_MEM_FREE(sync_rsp_ptr);
    return rsp_ptr;
  }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_GET_ATR_CMD;

  UIMDIAG_TOUINT64(client_id,
                   request.request.get_atr.client_id_low,
                   request.request.get_atr.client_id_high);

  card_slot     = (mmgsdi_slot_id_enum_type)request.request.get_atr.card_slot;

  sync_rsp_ptr->index = index;
  sync_rsp_ptr->rsp_ptr = rsp_ptr;

  mmgsdi_status = mmgsdi_get_atr(
                    client_id,
                    card_slot,
                    (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                    (mmgsdi_client_data_type)sync_rsp_ptr);
  
  if (mmgsdi_status == MMGSDI_SUCCESS)
  {
    return NULL;
  }

  uimdiag_free_index(index);  
  return rsp_ptr;
}/* uimdiag_mmgsdi_get_atr_cmd */


#ifdef FEATURE_UIMDIAG_INTERNAL_API
/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_SESSION_GET_OPERATOR_NAME_TABLE_CMD

DESCRIPTION
  Command handler for getting operator Name(s)

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_session_get_operator_name_table_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  int32                              index              = 0;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_session_id_type             session_id;
  mmgsdi_plmn_id_list_type           plmn_id_list;
  uint32                             loop_cnt           = 0;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_session_get_operator_name_table_cmd");

  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_SESSION_GET_OPERATOR_NAME_CMD);

  if(uimdiag_status != UIMDIAG_SUCCESS)
  {
    return rsp_ptr;
  }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd =
                                    UIMDIAG_MMGSDI_SESSION_GET_OPERATOR_NAME_CMD;

  session_id         = request.request.session_get_operator_name.session_id_low;
  plmn_id_list.num_of_plmn_ids = request.request.session_get_operator_name.no_of_plmns;
  plmn_id_list.plmn_list_ptr   = NULL;

  if(plmn_id_list.num_of_plmn_ids > 0)
  {
    /* Do memory allocation for plmn_id_list.plmn_list_ptr and copy plmn id
       and RAT type from plmn_list_ptr (buffer pointer). plmn_list_ptr can not
       be typecasted to mmgsdi_plmn_id_type pointer type */
    UIMDIAG_UTIL_TMC_MEM_MALLOC_AND_VALIDATE(
        plmn_id_list.plmn_list_ptr,
        (plmn_id_list.num_of_plmn_ids * sizeof(mmgsdi_plmn_id_type)));

    if(plmn_id_list.plmn_list_ptr == NULL)
    {
      return NULL;
    }

    for(loop_cnt = 0; loop_cnt < plmn_id_list.num_of_plmn_ids; loop_cnt++)
    {
      memscpy(plmn_id_list.plmn_list_ptr[loop_cnt].plmn_id_val,
             sizeof(plmn_id_list.plmn_list_ptr[loop_cnt].plmn_id_val),
             &request.request.session_get_operator_name.plmn_list_ptr[loop_cnt * 4],
             MMGSDI_PLMN_ID_SIZE);

      plmn_id_list.plmn_list_ptr[loop_cnt].rat =
        (mmgsdi_rat_enum_type)request.request.session_get_operator_name.plmn_list_ptr[(loop_cnt * 4) + MMGSDI_PLMN_ID_SIZE];
    }
  }

  mmgsdi_status = mmgsdi_session_get_operator_name(
                    session_id,
                    plmn_id_list,
                    (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                    (mmgsdi_client_data_type)index);

  UIMDIAGUTIL_TMC_MEM_FREE(plmn_id_list.plmn_list_ptr);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_SESSION_GET_OPERATOR_NAME_CMD);

  if( delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_session_get_operator_name_table_cmd */


/*===========================================================================

FUNCTION uimdiag_mmgsdi_get_all_available_apps_async_cmd

DESCRIPTION
  Command handler for get all available apps async

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_get_all_available_apps_async_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{

  uimdiag_generic_req_type              request;
  uimdiag_generic_rsp_type             *rsp_ptr                = NULL;
  uimdiag_return_enum_type              delayed_rsp_status     = UIMDIAG_ERROR;
  diagpkt_subsys_delayed_rsp_id_type    rsp_id                 = 0;
  uimdiag_return_enum_type              uimdiag_status         = UIMDIAG_ERROR;
  mmgsdi_client_id_type                 client_id              = 0;
  int32                                 index                  = 0;
  mmgsdi_return_enum_type               mmgsdi_status          = MMGSDI_ERROR;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_get_all_available_apps_async_cmd");

  (void)memset ( (void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr, pkt_len, &rsp_ptr, &rsp_id, &index,
                     &request, UIMDIAG_MMGSDI_GET_ALL_AVAILABLE_APPS_CMD);

  if((uimdiag_status != UIMDIAG_SUCCESS))
  {
    return rsp_ptr;
  }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_GET_ALL_AVAILABLE_APPS_CMD;


  UIMDIAG_TOUINT64(client_id,request.request.get_all_available_apps.client_id_low,
                   request.request.get_all_available_apps.client_id_high);
   mmgsdi_status      = mmgsdi_get_all_available_apps(client_id,
                                           (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                                           (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_GET_ALL_AVAILABLE_APPS_CMD);

  if( delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_get_all_available_apps_async_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_HIDE_CSIM_CMD

DESCRIPTION
  Command handler to extract data for HIDE CSIM CMD

DEPENDENCIES
  None.
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_hide_csim_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  int32                              index              = 0;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_hide_csim_cmd");
  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  /* extract data from req packet
   and populate fields in the request structure */
  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_HIDE_CSIM_CMD);

  if( uimdiag_status != UIMDIAG_SUCCESS)
  {
    UIMDIAG_MSG_ERROR_1("uimdiag_mmgsdi_parse_commands: Parsing error 0x%x",
                        uimdiag_status);
    return rsp_ptr;
  }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_HIDE_CSIM_CMD;

  mmgsdi_status = mmgsdi_hide_csim(request.request.hide_csim.slot_id,
                                   request.request.hide_csim.hide_csim,
                                   (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                                   (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_HIDE_CSIM_CMD);

  if( delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_hide_csim_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_MAP_SESSIONS_TO_SLOTS_CMD

DESCRIPTION
  Command handler to extract data for MAP SESSIONS TO SLOTS CMD

DEPENDENCIES
  None.
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_map_sessions_to_slots_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type           request;
  diagpkt_subsys_delayed_rsp_id_type rsp_id             = 0;
  int32                              index              = 0;
  uimdiag_generic_rsp_type          *rsp_ptr            = NULL;
  uimdiag_return_enum_type           uimdiag_status     = UIMDIAG_ERROR;
  uimdiag_return_enum_type           delayed_rsp_status = UIMDIAG_ERROR;
  mmgsdi_return_enum_type            mmgsdi_status      = MMGSDI_ERROR;
  mmgsdi_client_id_type              client_id          = 0;
  mmgsdi_sessions_to_slots_mapping_type
                                     sessions_to_slots  = {0};
  uimdiag_mmgsdi_map_sessions_to_slots_cmd_type
                                     *map_req_ptr       = NULL;

  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_map_sessinos_to_slots_cmd");
  (void)memset((void*)&request, 0x00, sizeof(uimdiag_generic_req_type) );

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  /* extract data from req packet
   and populate fields in the request structure */
  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_MAP_SESSIONS_TO_SLOTS_CMD);

  if( uimdiag_status != UIMDIAG_SUCCESS)
  {
    UIMDIAG_MSG_ERROR_1("uimdiag_mmgsdi_parse_commands: Parsing error 0x%x",
                        uimdiag_status);
    return rsp_ptr;
  }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_MAP_SESSIONS_TO_SLOTS_CMD;

  map_req_ptr = &request.request.map_sessions_to_slots;
  UIMDIAG_TOUINT64(client_id, map_req_ptr->client_id_high, map_req_ptr->client_id_low);

  if(map_req_ptr->sessions_to_slots_len == 0)
  {
    UIMDIAG_MSG_ERROR_1("uimdiag_mmgsdi_parse_commands: Parsing error 0x%x",
                        uimdiag_status);
    return rsp_ptr;
  }

  if(map_req_ptr->sessions_to_slots_len > 0)
  {
    sessions_to_slots.pri_slot = map_req_ptr->sessions_to_slots[0];
  }
  if(map_req_ptr->sessions_to_slots_len > 1 && UIMDIAG_MAX_NUM_SLOTS > 1)
  {
    sessions_to_slots.sec_slot = map_req_ptr->sessions_to_slots[1];
  }
  if(map_req_ptr->sessions_to_slots_len > 2 && UIMDIAG_MAX_NUM_SLOTS > 2)
  {
    sessions_to_slots.ter_slot = map_req_ptr->sessions_to_slots[2];
  }

  mmgsdi_status = mmgsdi_map_sessions_to_slots(
                                   client_id,
                                    sessions_to_slots,
                                   (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                                   (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
    &rsp_ptr,
    rsp_id,
    uimdiag_status,
    index,
    mmgsdi_status,
    &request,
    UIMDIAG_MMGSDI_MAP_SESSIONS_TO_SLOTS_CMD);

  if( delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  return rsp_ptr;
}/* uimdiag_mmgsdi_map_sessinos_to_slots_cmd */


/*===========================================================================

FUNCTION UIMDIAG_MMGSDI_NAA_REFRESH_EXT_CMD

DESCRIPTION
  Command handler to build a request to trigger for Refresh to MMGSDI with refresh enforcement policy

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
PACKED void * uimdiag_mmgsdi_naa_refresh_ext_cmd (
  PACKED void * cmd_ptr,
  word          pkt_len
)
{
  uimdiag_generic_req_type                 request;
  uimdiag_generic_rsp_type                *rsp_ptr             = NULL;
  diagpkt_subsys_delayed_rsp_id_type       rsp_id              = 0;
  int32                                    index               = 0;
  uimdiag_return_enum_type                 uimdiag_status      = UIMDIAG_ERROR;
  uimdiag_return_enum_type                 delayed_rsp_status  = UIMDIAG_ERROR;
  mmgsdi_return_enum_type                  mmgsdi_status       = MMGSDI_ERROR;
  mmgsdi_client_id_type                    client_id           = 0;
  uimdiag_mmgsdi_naa_refresh_ext_cmd_type *naa_refresh_ptr     = NULL;
  mmgsdi_slot_id_enum_type                 card_slot;
  mmgsdi_aid_type                          aid;
  mmgsdi_refresh_mode_enum_type            refresh_mode;
  uint8                                    num_files;
  mmgsdi_data_type                         refresh_files;
  mmgsdi_refresh_enforcement_policy_type   refresh_enforcement_policy;


  UIMDIAG_MSG_HIGH_0("uimdiag_mmgsdi_naa_refresh_ext_cmd");

  (void)memset((void *)&request, 0x00, sizeof(uimdiag_generic_req_type));
  (void)memset((void *)&refresh_files, 0x00, sizeof(mmgsdi_data_type));
  (void)memset((void *)&aid, 0x00, sizeof(mmgsdi_aid_type));
  (void)memset((void *)&refresh_enforcement_policy, 0x00, sizeof(refresh_enforcement_policy));

  if ((cmd_ptr == NULL)||
      (uimdiag_req_table_ptr == NULL))
  {
    return NULL;
  }

  uimdiag_status = uimdiag_mmgsdi_parse_commands(
                     cmd_ptr,
                     pkt_len,
                     &rsp_ptr,
                     &rsp_id,
                     &index,
                     &request,
                     UIMDIAG_MMGSDI_NAA_REFRESH_EXT_CMD);

  if(uimdiag_status != UIMDIAG_SUCCESS)
  {
    return rsp_ptr;
  }

  uimdiag_req_table_ptr[index].rsp_id = rsp_id;
  uimdiag_req_table_ptr[index].subcmd = UIMDIAG_MMGSDI_NAA_REFRESH_EXT_CMD;

  naa_refresh_ptr = &request.request.session_trigger_refresh_ext;

  UIMDIAG_TOUINT64(client_id,naa_refresh_ptr->client_id_low,
                   naa_refresh_ptr->client_id_high);

  card_slot              = (mmgsdi_slot_id_enum_type)
                            request.request.session_trigger_refresh_ext.slot_id;

  aid.app_type           = MMGSDI_APP_NONE;

  aid.aid.data_len       = (mmgsdi_len_type)
                            request.request.session_trigger_refresh_ext.sim_app_id_len;

  if(aid.aid.data_len > 0)
  {
    memscpy(aid.aid.data_ptr,
            sizeof(aid.aid.data_ptr),
            request.request.session_trigger_refresh_ext.sim_app_id,
            aid.aid.data_len*sizeof(uint8));
  }
  else
  {
    UIMDIAG_MSG_ERROR_0("UIMDIAG: uimdiag_mmgsdi_naa_refresh_ext aid length <= 0");
  }

  aid.label.data_len     = 0;
  memset(aid.label.data_ptr, 0, MMGSDI_MAX_AID_LEN);

  refresh_mode           = (mmgsdi_refresh_mode_enum_type)
                            request.request.session_trigger_refresh_ext.refresh_mode;

  num_files              = 0; /* num_files set to zero, as it is no longer used.*/

  refresh_files.data_len = (mmgsdi_len_type)
                            request.request.session_trigger_refresh_ext.path_len;
  refresh_files.data_ptr = request.request.session_trigger_refresh_ext.path;

  if((request.request.session_trigger_refresh_ext.enforcement_policy & UIMDIAG_MMGSDI_REFRESH_ENFORCEMENT_VALID_MASK) != 0)
  {
    refresh_enforcement_policy.is_enforcement_present = TRUE;
    if(request.request.session_trigger_refresh_ext.enforcement_policy & UIMDIAG_MMGSDI_REFRESH_ENFORCEMENT_NAVIGATING_MENU_MASK)
    {
      refresh_enforcement_policy.data.force_refresh_navigating_menu = TRUE;
    }
    if(request.request.session_trigger_refresh_ext.enforcement_policy & UIMDIAG_MMGSDI_REFRESH_ENFORCEMENT_DATA_CALL_MASK)
    {
      refresh_enforcement_policy.data.force_refresh_data_call = TRUE;
    }
    if(request.request.session_trigger_refresh_ext.enforcement_policy & UIMDIAG_MMGSDI_REFRESH_ENFORCEMENT_VOICE_CALL_MASK)
    {
      refresh_enforcement_policy.data.force_refresh_voice_call = TRUE;
    }
  }

  mmgsdi_status = mmgsdi_naa_refresh(
                    client_id,
                    card_slot,
                    aid,
                    refresh_mode,
                    refresh_files,
                    num_files,
                    NULL,
                    refresh_enforcement_policy,
                    (mmgsdi_callback_type)uimdiag_mmgsdi_generic_rsp_cb,
                    (mmgsdi_client_data_type)index);

  delayed_rsp_status = uimdiag_mmgsdi_post_parse_commands(
                         &rsp_ptr,
                         rsp_id,
                         uimdiag_status,
                         index,
                         mmgsdi_status,
                         &request,
                         UIMDIAG_MMGSDI_NAA_REFRESH_EXT_CMD);

  if(delayed_rsp_status != UIMDIAG_SUCCESS)
  {
    return NULL;
  }

  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_trigger_refresh_ext.sim_app_id);
  UIMDIAGUTIL_TMC_MEM_FREE(request.request.session_trigger_refresh_ext.path);

  return rsp_ptr;
}/* uimdiag_mmgsdi_naa_refresh_ext_cmd */

#endif /* FEATURE_UIMDIAG_INTERNAL_API */
