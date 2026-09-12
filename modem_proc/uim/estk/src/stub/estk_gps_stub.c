/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                          ESTK GPS File

GENERAL DESCRIPTION 
This file contains the GPS APIs needed by the ESTK module. 
  These APIs internally call the QMI_LOC APIs

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2016 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.
 
$Header: //components/rel/uim.mpss/6.1.0/estk/src/stub/estk_gps_stub.c#1 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/estk/src/stub/estk_gps_stub.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/26/15   gm      Initial Version

===========================================================================*/


/*===========================================================================

          C O M M O N   I N C L U D E   F I L E S   A N D   D A T A

===========================================================================*/
#include "estk_gps.h"

/*===========================================================================

                        EXTERNAL FUNCTION DEFINITION

===========================================================================*/

/*===========================================================================
FUNCTION ESTK_QMI_LOC_CLIENT_INIT

DESCRIPTION 
  Initializes the QMI-LOC client during power-up and register
  with QMI LOC service for system information indication

DEPENDENCIES 
  FEATURE_ESTK_HAS_QMI_LOC 

RETURN VALUE 
  None. 

SIDE EFFECTS 
  None. 
===========================================================================*/
boolean estk_qmi_loc_client_init(void)
{
  return TRUE;
}/* ESTK_GPS_INIT */

/*==========================================================================
FUNCTION ESTK_QMI_LOC_CLIENT_PROCESS_SIGNAL

DESCRIPTION 
  Funtion to process LOC service notify signal. This function is
  used to process signal sent to estk_ivs_task by QCCI notifying LOC
  service. 
    
DEPENDENCIES
  FEATURE_ESTK_HAS_QMI_LOC

RETURN VALUE
  None

SIDE EFFECTS 
  None 
===========================================================================*/
void estk_qmi_loc_client_process_signal
(
  void
)
{
}/*estk_qmi_loc_client_process_signals()*/

/*===========================================================================
FUNCTION       ESTK_QMI_LOC_CLIENT_IND_CB

DESCRIPTION 
  QMI-LOC indication callback handler. This function is called
  by QCCI whenver there is a LOC indication message.

DEPENDENCIES  

RETURN VALUE 
  None

SIDE EFFECTS 
  None
===========================================================================*/
void  estk_qmi_loc_client_ind_cb
(
  qmi_client_type                user_handle,
  unsigned int                   msg_id,
  void                          *ind_buf_ptr,
  unsigned int                   ind_buf_len,
  void                          *ind_cb_data
)
{
  (void) user_handle;
  (void) msg_id;
  (void) ind_buf_ptr;
  (void) ind_buf_len;
  (void) ind_cb_data;
}/*estk_qmi_loc_client_ind_cb()*/

/*===========================================================================
FUNCTION: ESTK_PROCESS_GPS_DATA_CMD

DESCRIPTION :
  This function will be called by ESTK to process GPS data response from QMI_LOC

PARAMETERS:
  cmd_ptr : [Input] ESTK command pointer

DEPENDENCIES:
  None

RETURN VALUE:
  estk_result_enum_type
    ESTK_SUCCESS : If processing of WMS event is successful
    ESTK_ERROR   : If processing results in error

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
estk_result_enum_type estk_process_gps_data_cmd(
  const estk_cmd_type *cmd_ptr
)
{
  (void) cmd_ptr;
  return ESTK_SUCCESS;
}

/*===========================================================================
FUNCTION: ESTK_GEO_LOC_RSP_TIMER_CB

DESCRIPTION :
  The callback funciton supplied to when geographical location timer expire

PARAMETERS:
  mask: mask contain slot information.

DEPENDENCIES:
  None

RETURN VALUE:
  None

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
void estk_geo_loc_rsp_timer_cb(uint32 slot)
{
  (void) slot;
} /* estk_bip_keep_ch_open_timer_cb */

/*===========================================================================

FUNCTION: ESTK_PROCESS_GEO_LOC_RSP_TIMEOUT

DESCRIPTION:
  This ESTK function builds a geographical location envelopel with less accuracy if final 
  report is received from QMI_LOC.

PARAMETERS:
  None

DEPENDENCIES:
  None

RETURN VALUE:
  estk_result_enum_type

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
estk_result_enum_type estk_process_geo_loc_rsp_timeout()
{
  return ESTK_SUCCESS;
}

/*===========================================================================
FUNCTION:   estk_gps_full_clean_up_slot

DESCRIPTION :
  Free memory allocated for the GPS datal

PARAMETERS:
  slot_id : [Input] slot on which the gps data to be cleaned

DEPENDENCIES:
  None

RETURN VALUE:
  None

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
void estk_gps_full_clean_up_slot(gstk_slot_id_enum_type slot_id)
{
  (void) slot_id;
}

/*===========================================================================
FUNCTION: ESTK_PROCESS_GEO_LOC_REQ

DESCRIPTION:
  ESTK calls this ESTK function to process the Geographical location request.

PARAMETERS:
  estk_req_ptr : [Input] estk geographical location cmd pointer

DEPENDENCIES:
  None

RETURN VALUE:
  estk_status_enum_type
    ESTK_SUCCESS: - When GEO lcoation is successfully handled
    ESTK_ERROR:   - When there is failure in handling GEO location

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
estk_result_enum_type estk_process_geo_loc_req
(
  gstk_cmd_from_card_type *gstk_req_ptr
)
{
  (void) gstk_req_ptr;
  return ESTK_SUCCESS;
}

