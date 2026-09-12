#ifndef ESTK_GPS_H
#define ESTK_GPS_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                               ESTK _ G P S 
                            H E A D E R   F I L E

GENERAL DESCRIPTION 
This file contains the GPS APIs needed by the Ecall module. These APIs internally call the GPS PDAPIs.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2015, 2020 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.
 
$Header: //components/rel/uim.mpss/6.1.0/estk/src/estk_gps.h#3 $
$DateTime: 2020/01/28 02:49:39 $
$Author: pwbldsvc $
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/estk/src/estk_gps.h#3 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/24/20   ks      Add support to check NFW lock status
01/24/20   ks      Add support to check NFW lock status
03/26/15   gm      Initial Version

===========================================================================*/


/*===========================================================================

                         I N C L U D E   F I L E S

===========================================================================*/

#include "estk.h"
#include "qmi_client.h"
#include "estk_priv.h"
#include "uim_msg.h"

#ifdef FEATURE_GSTK_QMI_LOC
#include "location_service_v02.h"

#define ESTK_MASK_SLOT_1  0x01
#define ESTK_MASK_SLOT_2  0x02
#define ESTK_MASK_SLOT_3  0x04

#define ESTK_QMI_LOC_REPORT_INTERVAL 1000

typedef enum
{
  ESTK_NMEA_RMC_TYPE,
  ESTK_NMEA_GGA_TYPE,
  ESTK_NMEA_GNS_TYPE,
  ESTK_NMEA_OTHER_TYPE,
  ESTK_NMEA_MAX_TYPE
}estk_nmea_sentence_type;

typedef struct {

  /* Mandatory */
  /*  Result Code */
  qmi_response_type_v01 resp;
}estk_geo_loc_resp_msg_v02;

typedef struct
{
  uint8                             *nmea_ptr;
  uint8                              nmea_len;  
  estk_nmea_sentence_type            nmea_type;
}nmea_cache_type;

/*===========================================================================
FUNCTION ESTK_GET_POSITION

DESCRIPTION
  This function triggers pdsm_get_position() during an application tracking 
  session.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
===========================================================================*/
void estk_get_position 
(
  uint32 gps_timeout_ms
);

/*===========================================================================
FUNCTION ESTK_END_POSITION_SESSION

DESCRIPTION
  This function will send the QMI_LOC_STOP_REQ_V02 command to QMI_LOC
  or pdsm_end_session() to PDAPI to stop the location session.

DEPENDENCIES 
  None

RETURN VALUE 
  None

SIDE EFFECTS 
  None
===========================================================================*/
void estk_end_position_session
(
  void
);

/*===========================================================================

          Q M I - L O C   F U N C T I O N   D E C L A R A T I O N S

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
boolean estk_qmi_loc_client_init
(
  void
);

/*===========================================================================
FUNCTION ESTK_GEO_LOC_SLOT_ID_INIT

DESCRIPTION
  This function assigns slot_id based on geo_loc_slot_mask.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  gstk_slot_id_enum_type

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
gstk_slot_id_enum_type estk_geo_loc_slot_id_init
(
  void
);

/*===========================================================================
FUNCTION  ESTK_QMI_LOC_REQUEST_RESP

DESCRIPTION 
  Handle callbacks response of QMI_LOC_START_REQ_V02 and
  QMI_LOC_REG_EVENTS_REQ_V02.

DEPENDENCIES 
  FEATURE_ESTK_HAS_QMI_LOC

RETURN VALUE 
  None 

SIDE EFFECTS 
  Release the memory assigned for response
===========================================================================*/
void estk_qmi_loc_request_resp
(
  qmi_client_type                user_handle,
  unsigned int                   msg_id,
  void                          *resp_c_struct,
  unsigned int                   resp_c_struct_len,
  void                          *resp_cb_data,
  qmi_client_error_type          transp_err
);

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
);

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
  gstk_cmd_from_card_type *estk_req_ptr
);

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
estk_result_enum_type estk_process_gps_data_cmd
(
  const estk_cmd_type *cmd_ptr
);

/*===========================================================================

FUNCTION:   ESTK_SLOT_SEND_ENVELOPE_GEO_LOC_REPORT_CMD

DESCRIPTION:
  This ESTK function builds a geographical location envelopel and send it to UICC

PARAMETERS:
  slot_id : [Input] Slot_id
  
DEPENDENCIES:
  None

RETURN VALUE:
  estk_result_enum_type

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
estk_result_enum_type estk_slot_send_envelope_geo_loc_report_cmd
(
  gstk_slot_id_enum_type slot_id
);

/*===========================================================================

FUNCTION:   ESTK_ENVELOPE_GEO_LOC_RSP_CB

DESCRIPTION:
  This ESTK function is a callback for the geographical location report response from UICC

PARAMETERS:
  req_rsp : [Input] GSTK response from UICC

DEPENDENCIES:
  None

RETURN VALUE:
  Npne

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
void estk_envelope_geo_loc_rsp_cb 
(
  gstk_cmd_from_card_type *         req_rsp
);

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
void estk_geo_loc_rsp_timer_cb
(
  uint32 slot
);

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
estk_result_enum_type estk_process_geo_loc_rsp_timeout
(
);

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
);

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
void estk_gps_full_clean_up_slot
(
  gstk_slot_id_enum_type slot_id
);
/*===========================================================================
FUNCTION       ESTK_GET_ENGINE_LOCK_REQ

DESCRIPTION
  This function will send the QMI_LOC_GET_ENGINE_LOCK_REQ_V02 command to QMI_LOC
  to get the status of location engine lock.

DEPENDENCIES 
  None 

RETURN VALUE 
  None

SIDE EFFECTS 
  None
===========================================================================*/
void estk_get_engine_lock_req
(
  void
);
#endif /*FEATURE_GSTK_QMI_LOC */
#endif /* ESTK_GPS_H */
