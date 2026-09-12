/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  GSM VOICE SERVER API and HANDLING FUNCTIONS

GENERAL DESCRIPTION

   This modules provides the API for voice clients to exchange voice packets
   with GSM. This module also implements all the functions used necesary 
   for gsm to interact with GSM voice Server

EXTERNALIZED FUNCTIONS



INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/geran.mpss/7.4.0/gl1/src/gl1_voice_server.c#2 $

when       who       what, where, why
27/01/20   rks     CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
03/05/18   rv        CR2233364 : Add feature flags for FR38904 changes - GERAN
14/11/16   mk        CR1082549 : FR 38904: Audio gap optimization during Handovers in Voice call - GERAN
06/02/18   rv       CR2069764: Excessive F3 logging reduction in GL1
02/05/16   mk        CR1010068 Add DTF events in dedicated mode
09/11/15   hd        CR936986 Do not check for bfi flag set by vocoder, while transmitting frames in UL
02/06/15   pjr       CR847067 Forward vocoder sample rate to RR 
                     with MPH_VOICE_SAMPLE_RATE_CHANGE_IND
21/05/15   pjr       CR839646 Add support for commad GSM_IVOICE_CMD_SEND_SAMPLE_RATE
10/01/15   pjr       CR818535 Initial Revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"

#ifdef CUST_H
#include "customer.h"
#endif

#include "qurt_atomic_ops.h"
#include "event.h"  

#include "gl1_voice_server.h"
#include "geran_multi_sim.h"
#include "geran_dual_sim_g.h"
#include "l1_task.h"
#include "gsmdiag.h"

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "l1_log_qsh.h"
#endif

/*===========================================================================

                     Private Type Definitions

===========================================================================*/

/* codec_mode_to_len_t is used to define a table to map
 * amr codec modes with their lenght */
typedef struct
{
  uint32 amr_codec_mode;
  uint32 len;
} codec_mode_to_len_t;

/* ivoc_codec_mode_to_amr_codec_mode_t is used to define
 * table to map gsm_ivocamr codec mode to gl1_msg_amr_mode_type */
typedef struct
{
  uint32 ivoc_codec_mode;
  gl1_msg_amr_mode_type amr_codec_mode;
} ivoc_codec_mode_to_amr_codec_mode_t;

/* ivocamr_frame_type_to_amr_frame_type_t is used to define
 * table to map gsm_ivocamrwb frame type to gl1_msg_amr_frame_type */ 
typedef struct
{
  uint32 ivocamr_frame_type;
  gl1_msg_amr_frame_type amr_frame_type;
} ivocamr_frame_type_to_amr_frame_type_t;

/*===========================================================================

                     Private Macros

===========================================================================*/
#define L1_VS_ATOMIC_LOCK_DATA(s)    while(!qurt_atomic_compare_and_set(&(s)->lock, 0, 1))
#define L1_VS_ATOMIC_UNLOCK_DATA(s)  qurt_atomic_set(&(s)->lock, 0)

/*===========================================================================

                     Private Prototypes

===========================================================================*/
static uint32 l1_vs_get_vocoder_type(gl1_msg_chan_type chan_type, 
                                     gl1_msg_voc_type voc_type);

static uint32 l1_vs_send_event_to_client(uint32 event_id, void* params, 
                                         uint32 size, gas_id_t gas_id);

static uint32 l1_vs_get_codec_packet_len(uint32 vocoder_id, void* frame_info);

static voice_server_session_data_t* get_session_data_ptr(sys_modem_as_id_e_type as_id);

static void l1_vs_report_event_type_1(l1_vs_event_code_t type, 
                                      l1_vs_event_status_t status,
                                      gas_id_t gas_id);

static void l1_vs_report_event_type_2(l1_vs_event_code_t type, 
                                      l1_vs_event_status_t status, 
                                      l1_vs_event_mode_t mode,
                                      gas_id_t gas_id);

static l1_vs_event_mode_t map_vocoder_type_to_event_mode(uint32 vocoder_type);
static l1_vs_event_mode_t map_amr_mode_to_event_mode(gl1_msg_amr_mode_type amr_mode);

/*===========================================================================

                     Private Variables

===========================================================================*/
voice_server_session_data_t server_session_data[NUM_GERAN_DATA_SPACES] = { {0} };

/* Table used to map packet length for different codec modes */
static const codec_mode_to_len_t codec_mode_to_len[] = 
  { { GSM_IVOCAMR_CODEC_MODE_0475,   12 },
    { GSM_IVOCAMR_CODEC_MODE_0515,   13 },
    { GSM_IVOCAMR_CODEC_MODE_0590,   15 },
    { GSM_IVOCAMR_CODEC_MODE_0670,   17 },
    { GSM_IVOCAMR_CODEC_MODE_0740,   19 },
    { GSM_IVOCAMR_CODEC_MODE_0795,   20 },
    { GSM_IVOCAMR_CODEC_MODE_1020,   26 },
    { GSM_IVOCAMR_CODEC_MODE_1220,   31 },
    { GSM_IVOCAMRWB_CODEC_MODE_0660, 17 },
    { GSM_IVOCAMRWB_CODEC_MODE_0885, 23 },
    { GSM_IVOCAMRWB_CODEC_MODE_1265, 32 } };

/* Table to map gl1_msg_amr_frame_type to gsm_ivocamr frame type 
 * Entries in this table are in order to match gl1_msg_amr_frame_type*/
static const uint32 ivocamr_frame_types_tbl[] = 
    { GSM_IVOCAMR_FRAME_TYPE_SPEECH_GOOD,
      GSM_IVOCAMR_FRAME_TYPE_SPEECH_DEGRADED,
      GSM_IVOCAMR_FRAME_TYPE_SPEECH_ONSET,
      GSM_IVOCAMR_FRAME_TYPE_SPEECH_BAD,
      GSM_IVOCAMR_FRAME_TYPE_SID_FIRST,
      GSM_IVOCAMR_FRAME_TYPE_SID_UPDATE,
      GSM_IVOCAMR_FRAME_TYPE_SID_BAD,
      GSM_IVOCAMR_FRAME_TYPE_NO_DATA };

/* Table to map gl1_msg_amr_frame_type to gsm_ivocamrwb frame type 
 * Entries in this table are in order to match gl1_msg_amr_frame_type*/
static const uint32 ivocamrwb_frame_types_tbl[] = 
    { GSM_IVOCAMRWB_FRAME_TYPE_SPEECH_GOOD,
      GSM_IVOCAMRWB_FRAME_TYPE_SPEECH_DEGRADED,
      GSM_IVOCAMRWB_FRAME_TYPE_ONSET,
      GSM_IVOCAMRWB_FRAME_TYPE_SPEECH_BAD,
      GSM_IVOCAMRWB_FRAME_TYPE_SID_FIRST,
      GSM_IVOCAMRWB_FRAME_TYPE_SID_UPDATE,
      GSM_IVOCAMRWB_FRAME_TYPE_SID_BAD,
      GSM_IVOCAMRWB_FRAME_TYPE_NO_DATA };

/* Table to map gl1_msg_amr_mode_type to gsm_ivocamr codec mode
 * Entries in this table are in order to match gl1_msg_amr_mode_type */
static const uint32 codec_mode_tbl[] = 
    { GSM_IVOCAMR_CODEC_MODE_0475,
      GSM_IVOCAMR_CODEC_MODE_0515,
      GSM_IVOCAMR_CODEC_MODE_0590,
      GSM_IVOCAMR_CODEC_MODE_0670,
      GSM_IVOCAMR_CODEC_MODE_0740,
      GSM_IVOCAMR_CODEC_MODE_0795,
      GSM_IVOCAMR_CODEC_MODE_1020,
      GSM_IVOCAMR_CODEC_MODE_1220,
      GSM_IVOCAMRWB_CODEC_MODE_0660,
      GSM_IVOCAMRWB_CODEC_MODE_0885,
      GSM_IVOCAMRWB_CODEC_MODE_1265 }; 

/* Table to map gsm_ivocamr codec mode to gl1_msg_amr_mode_type  */
static const ivoc_codec_mode_to_amr_codec_mode_t ivoc_codec_mode_to_amr_codec_mode[] = 
  { { GSM_IVOCAMR_CODEC_MODE_0475,     GL1_MSG_AMR_MODE_0475 },   
    { GSM_IVOCAMR_CODEC_MODE_0515,     GL1_MSG_AMR_MODE_0515 },   
    { GSM_IVOCAMR_CODEC_MODE_0590,     GL1_MSG_AMR_MODE_0590 },   
    { GSM_IVOCAMR_CODEC_MODE_0670,     GL1_MSG_AMR_MODE_0670 },   
    { GSM_IVOCAMR_CODEC_MODE_0740,     GL1_MSG_AMR_MODE_0740 },   
    { GSM_IVOCAMR_CODEC_MODE_0795,     GL1_MSG_AMR_MODE_0795 },   
    { GSM_IVOCAMR_CODEC_MODE_1020,     GL1_MSG_AMR_MODE_1020 },   
    { GSM_IVOCAMR_CODEC_MODE_1220,     GL1_MSG_AMR_MODE_1220 },   
    { GSM_IVOCAMRWB_CODEC_MODE_0660,   GL1_MSG_AMR_WB_MODE_0660 },
    { GSM_IVOCAMRWB_CODEC_MODE_0885,   GL1_MSG_AMR_WB_MODE_0885 },
    { GSM_IVOCAMRWB_CODEC_MODE_1265,   GL1_MSG_AMR_WB_MODE_1265 } };

/* Table to map gsm_ivocamrwb frame type to gl1_msg_amr_frame_type */ 
static const ivocamr_frame_type_to_amr_frame_type_t ivocamr_frame_type_to_amr_frame_type[] =
  { { GSM_IVOCAMR_FRAME_TYPE_SPEECH_GOOD,        GL1_MSG_AMR_SPEECH_GOOD     },
    { GSM_IVOCAMR_FRAME_TYPE_SPEECH_BAD,         GL1_MSG_AMR_SPEECH_BAD      },
    { GSM_IVOCAMR_FRAME_TYPE_SID_FIRST,          GL1_MSG_AMR_SID_FIRST       },
    { GSM_IVOCAMR_FRAME_TYPE_SID_UPDATE,         GL1_MSG_AMR_SID_UPDATE      },
    { GSM_IVOCAMR_FRAME_TYPE_SID_BAD,            GL1_MSG_AMR_SID_BAD         },
    { GSM_IVOCAMR_FRAME_TYPE_NO_DATA,            GL1_MSG_AMR_NO_DATA         },
    { GSM_IVOCAMR_FRAME_TYPE_SPEECH_DEGRADED,    GL1_MSG_AMR_SPEECH_DEGRADED },
    { GSM_IVOCAMR_FRAME_TYPE_SPEECH_ONSET,       GL1_MSG_AMR_ONSET           },
    { GSM_IVOCAMRWB_FRAME_TYPE_SPEECH_GOOD,      GL1_MSG_AMR_SPEECH_GOOD     },
    { GSM_IVOCAMRWB_FRAME_TYPE_SPEECH_BAD,       GL1_MSG_AMR_SPEECH_BAD      },
    { GSM_IVOCAMRWB_FRAME_TYPE_SID_FIRST,        GL1_MSG_AMR_SID_FIRST       },
    { GSM_IVOCAMRWB_FRAME_TYPE_SID_UPDATE,       GL1_MSG_AMR_SID_UPDATE      },
    { GSM_IVOCAMRWB_FRAME_TYPE_SID_BAD,          GL1_MSG_AMR_SID_BAD         },
    { GSM_IVOCAMRWB_FRAME_TYPE_NO_DATA,          GL1_MSG_AMR_NO_DATA         },
    { GSM_IVOCAMRWB_FRAME_TYPE_SPEECH_DEGRADED,  GL1_MSG_AMR_SPEECH_DEGRADED },
    { GSM_IVOCAMRWB_FRAME_TYPE_ONSET,            GL1_MSG_AMR_ONSET           },
    { GSM_IVOCAMRWB_FRAME_TYPE_SPEECH_LOST,      GL1_MSG_AMR_NO_DATA         } };

/*===========================================================================

                     FUNCTIONS DEFINITION

===========================================================================*/

/*===========================================================================

FUNCTION l1_vs_start_client

DESCRIPTION
  Request GSM Voice Adapter to acquire and enable then vocoder,
  This function set the vocoder ID (AMR, AMRWB, FR, EFR or HR)

DEPENDENCIES
  None.

RETURN VALUE
  GSM_EOK, indicating the command has been successfully executed, or
  GSM_EFAILED, indicating the command has failed.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 l1_vs_start_client(gl1_msg_chan_type msg_chan_type, 
                          gl1_msg_voc_type msg_voc_type,
                          gas_id_t gas_id)
{
  gsm_ivoice_event_start_request_t params;
  uint32 ret_val = GSM_EFAILED;
  l1_vs_event_status_t event_status = VS_EVENT_STATUS_FAIL;

  voice_server_session_data_t* s = get_session_data_ptr(geran_map_gas_id_to_nas_id(gas_id));

  L1_VS_ATOMIC_LOCK_DATA(s);

  params.vocoder_id = l1_vs_get_vocoder_type(msg_chan_type, msg_voc_type);

  MSG_GERAN_HIGH_2_G("l1_vs_start_vocoder, vocoderid: 0x%x, vocoder_state %d", 
                     params.vocoder_id, s->vocoder_state);

  if (s->vocoder_state == GL1_VOICE_SERVER_OPENED)
  {
    ret_val = l1_vs_send_event_to_client(GSM_IVOICE_EVENT_START_REQUEST, 
                                         (void*)&params, sizeof(params), 
                                         gas_id);
    if (ret_val == GSM_EOK)
    {
      s->vocoder_id = params.vocoder_id;
      s->vocoder_state = GL1_VOICE_SERVER_STARTED;
      event_status = VS_EVENT_STATUS_SUCCESS;
    }
  }

  L1_VS_ATOMIC_UNLOCK_DATA(s);

  l1_vs_report_event_type_2(VS_EVENT_TYPE_START_VOCODER, event_status, 
                            map_vocoder_type_to_event_mode(params.vocoder_id), gas_id);

  return ret_val;
}


/*===========================================================================

FUNCTION l1_vs_stop_client

DESCRIPTION
  Request GSM Voice Adapter to disable the vocoder

DEPENDENCIES
  None.

RETURN VALUE
  GSM_EOK, indicating the command has been successfully executed, or
  GSM_EFAILED, indicating the command has failed.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 l1_vs_stop_client(gas_id_t gas_id)
{
  uint32 ret_val = GSM_EFAILED;
  voice_server_session_data_t* s = get_session_data_ptr(geran_map_gas_id_to_nas_id(gas_id));
  l1_vs_event_status_t event_status = VS_EVENT_STATUS_FAIL;

  L1_VS_ATOMIC_LOCK_DATA(s);

  MSG_GERAN_HIGH_1_G("l1_vs_stop_vocoder, vocoder_state %d", s->vocoder_state);

  if (s->vocoder_state == GL1_VOICE_SERVER_STARTED ||
      s->vocoder_state == GL1_VOICE_SERVER_READY )
  {
    ret_val = l1_vs_send_event_to_client(GSM_IVOICE_EVENT_STOP_REQUEST, 
                                         NULL, 0, gas_id);

    if (ret_val == GSM_EOK)
    {
      s->vocoder_state = GL1_VOICE_SERVER_STOPPED;
      event_status = VS_EVENT_STATUS_SUCCESS;
    }
  }

  L1_VS_ATOMIC_UNLOCK_DATA(s);

  l1_vs_report_event_type_1(VS_EVENT_TYPE_STOP_VOCODER, event_status, gas_id);

  return ret_val;
}

#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION
/*===========================================================================

FUNCTION l1_vs_change_in_voc

DESCRIPTION
  Check if there is difference in between active vocoder and new vocoder type 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE, If there is change between active and new vocoder id, or
  FALSE,If there is no change between active and new vocoder id.

SIDE EFFECTS
  None.

===========================================================================*/

uint32 l1_vs_change_in_voc(gl1_msg_chan_type msg_chan_type, 
                                   gl1_msg_voc_type msg_voc_type,
                                   gas_id_t gas_id)
{

  uint32 vocoder_id;
  
  voice_server_session_data_t* s = get_session_data_ptr(geran_map_gas_id_to_nas_id(gas_id));

  L1_VS_ATOMIC_LOCK_DATA(s);

  vocoder_id = l1_vs_get_vocoder_type(msg_chan_type, msg_voc_type);

  L1_VS_ATOMIC_UNLOCK_DATA(s);

  MSG_GERAN_HIGH_2_G("l1_vs_change_in_voc, Active vocoder_id %d, New Vocoder_id %d", s->vocoder_id, vocoder_id);

  if(s->vocoder_id == vocoder_id)
  {
      return FALSE;
  }  
  else
  {
      return TRUE;
  }  

}

#endif /*FEATURE_GSM_AUDIO_GAP_OPTIMIZATION*/
/*===========================================================================

FUNCTION l1_vs_get_packet_from_client

DESCRIPTION
  Request an UL packet to GVA

DEPENDENCIES
  None.

RETURN VALUE
  GSM_EOK, indicating the command has been successfully executed, or
  GSM_EFAILED, indicating the command has failed.

SIDE EFFECTS
  None.

===========================================================================*/
static uint32 l1_vs_get_packet_from_client(gsm_ivocoder_buffer_t* buffer, 
                                           gas_id_t gas_id)
{
  voice_server_session_data_t* s = get_session_data_ptr(geran_map_gas_id_to_nas_id(gas_id));
  uint32 ret_val = GSM_EFAILED;

  L1_VS_ATOMIC_LOCK_DATA(s);

  if (s->ul_cb && s->vocoder_state == GL1_VOICE_SERVER_READY)
  {
    s->ul_cb(s->client_session_data, buffer);
    ret_val = GSM_EOK;
  }
  else
  {
    MSG_GERAN_MED_1_G("l1_vs_get_packet_from_client, vocoder_state %d", 
                    s->vocoder_state);
  }  

  L1_VS_ATOMIC_UNLOCK_DATA(s);

  return ret_val;
}


/*===========================================================================

FUNCTION l1_vs_send_packet_to_client

DESCRIPTION
  Pass a DL packet to GVA

DEPENDENCIES
  None.

RETURN VALUE
  GSM_EOK, indicating the command has been successfully executed, or
  GSM_EFAILED, indicating the command has failed.

SIDE EFFECTS
  None.

===========================================================================*/
static uint32 l1_vs_send_packet_to_client(gsm_ivocoder_buffer_t* buffer, 
                                          gas_id_t gas_id)
{
  uint32 ret_val = GSM_EFAILED;
  voice_server_session_data_t* s = get_session_data_ptr(geran_map_gas_id_to_nas_id(gas_id));

  L1_VS_ATOMIC_LOCK_DATA(s);

  if (s->dl_cb && s->vocoder_state == GL1_VOICE_SERVER_READY)
  {
    s->dl_cb(s->client_session_data, buffer);
    ret_val =  GSM_EOK;
  }
  else
  {
    MSG_GERAN_MED_1_G("l1_vs_send_packet_to_client, vocoder_state %d", 
                    s->vocoder_state);
  }
  L1_VS_ATOMIC_UNLOCK_DATA(s);

  return ret_val;
}


/*===========================================================================

FUNCTION l1_vs_set_ul_amr_mode

DESCRIPTION
  Sets the codec mode for UL frames

DEPENDENCIES
  None.

RETURN VALUE
  GSM_EOK, indicating the command has been successfully executed, or
  GSM_EFAILED, indicating the command has failed.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 l1_vs_set_ul_amr_mode(gl1_msg_amr_mode_type codec_mode, gas_id_t gas_id)
{
  uint32 ret_val = GSM_EFAILED;
  gsm_ivoice_event_request_codec_mode_t params;
  voice_server_session_data_t* s = get_session_data_ptr(geran_map_gas_id_to_nas_id(gas_id));
  l1_vs_event_status_t event_status;

  L1_VS_ATOMIC_LOCK_DATA(s);

  MSG_GERAN_HIGH_2_G("l1_vs_set_ul_amr_mode, amr_codec_mode: %d, vocoder_state %d", 
                     codec_mode, s->vocoder_state);

  if (s->vocoder_state > GL1_VOICE_SERVER_CLOSED)
  {
    params.codec_mode = l1_vs_map_amr_mode_type_to_gsm_ivoc_codec_mode(codec_mode);
    s->codec_mode = params.codec_mode;
    ret_val = l1_vs_send_event_to_client(GSM_IVOICE_EVENT_REQUEST_CODEC_MODE, 
                                         (void*)&params, sizeof(params), gas_id);
  }

  L1_VS_ATOMIC_UNLOCK_DATA(s);

  event_status = ret_val == GSM_EOK ? VS_EVENT_STATUS_SUCCESS : VS_EVENT_STATUS_FAIL;

  l1_vs_report_event_type_2(VS_EVENT_TYPE_SET_AMR_MODE, event_status, 
                            map_amr_mode_to_event_mode(codec_mode), gas_id);

  return ret_val;
}


/*===========================================================================

FUNCTION l1_vs_set_dtx_mode

DESCRIPTION
  Enable/Disable DTX mode

DEPENDENCIES
  None.

RETURN VALUE
  GSM_EOK, indicating the command has been successfully executed, or
  GSM_EFAILED, indicating the command has failed.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 l1_vs_set_dtx_mode(l1_vs_dtx_mode_t dtx_mode, gas_id_t gas_id)
{
  gsm_ivoice_event_request_dtx_mode_t params;
  uint32 ret_val = GSM_EFAILED;
  l1_vs_event_status_t event_status;

  l1_vs_event_mode_t event_mode = dtx_mode == DTX_ENABLED ? 
                                  VS_EVENT_MODE_DTX_ON : 
                                  VS_EVENT_MODE_DTX_OFF;

  voice_server_session_data_t* s = get_session_data_ptr(geran_map_gas_id_to_nas_id(gas_id));

  L1_VS_ATOMIC_LOCK_DATA(s);

  MSG_GERAN_HIGH_2_G("l1_vs_set_dtx_mode dtx_enabled %d, vocoder_state %d", 
                     dtx_mode, s->vocoder_state);

  if (s->vocoder_state > GL1_VOICE_SERVER_CLOSED)
  {
    s->dtx_mode = dtx_mode;
    params.enable_flag = dtx_mode == DTX_ENABLED ? 1 : 0;
    ret_val = l1_vs_send_event_to_client(GSM_IVOICE_EVENT_REQUEST_DTX_MODE, 
                                         (void*)&params, sizeof(params), gas_id);
  }

  L1_VS_ATOMIC_UNLOCK_DATA(s);

  event_status = ret_val == GSM_EOK ? VS_EVENT_STATUS_SUCCESS : VS_EVENT_STATUS_FAIL;

  l1_vs_report_event_type_2(VS_EVENT_TYPE_SET_DTX_MODE, event_status, event_mode, gas_id);

  return ret_val;
}

/*===========================================================================

FUNCTION l1_vs_request_vocoder_ownership

DESCRIPTION
  Notify Voice Agent that GSM will need the vocoder

DEPENDENCIES
  None.

RETURN VALUE
  GSM_EOK, indicating the command has been successfully executed, or
  GSM_EFAILED, indicating the command has failed.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 l1_vs_request_vocoder_ownership(gas_id_t gas_id) 
{
  uint32 ret_val = GSM_EFAILED;
  voice_server_session_data_t* s = get_session_data_ptr(geran_map_gas_id_to_nas_id(gas_id));
  l1_vs_event_status_t event_status;

  L1_VS_ATOMIC_LOCK_DATA(s);

  MSG_GERAN_HIGH_1_G("l1_vs_request_vocoder_ownership, vocoder_state %d", 
                     s->vocoder_state);

  if (s->vocoder_state == GL1_VOICE_SERVER_OPENED) 
  {
    ret_val = l1_vs_send_event_to_client(GSM_IVOICE_EVENT_SELECT_OWNER, 
                                         NULL, 0, gas_id);
  }

  L1_VS_ATOMIC_UNLOCK_DATA(s);

  event_status = ret_val == GSM_EOK ? VS_EVENT_STATUS_SUCCESS : VS_EVENT_STATUS_FAIL;

  l1_vs_report_event_type_1(VS_EVENT_TYPE_REQ_OWNERSHIP, event_status, gas_id);

  return ret_val;
}

/*===========================================================================

FUNCTION l1_vs_is_client_ready

DESCRIPTION
  Returns TRUE if the client is ready to exchange voice frames

DEPENDENCIES
  None.

RETURN VALUE
  GSM_EOK, indicating the command has been successfully executed, or
  GSM_EFAILED, indicating the command has failed.

SIDE EFFECTS
  None.

===========================================================================*/
boolean l1_vs_is_client_ready(gas_id_t gas_id)
{
  voice_server_session_data_t* s = get_session_data_ptr(geran_map_gas_id_to_nas_id(gas_id));
  boolean ret_val;

  L1_VS_ATOMIC_LOCK_DATA(s);                                 

  ret_val = s->vocoder_state == GL1_VOICE_SERVER_READY;

  L1_VS_ATOMIC_UNLOCK_DATA(s);

  return ret_val;
}


/*===========================================================================

FUNCTION l1_vs_get_voice_server_state

DESCRIPTION
  Returns the voice server state

DEPENDENCIES
  None.

RETURN VALUE
  GSM_EOK, indicating the command has been successfully executed, or
  GSM_EFAILED, indicating the command has failed.

SIDE EFFECTS
  None.

===========================================================================*/
l1_voice_server_state_t l1_vs_get_voice_server_state(gas_id_t gas_id)
{
  voice_server_session_data_t* s = get_session_data_ptr(geran_map_gas_id_to_nas_id(gas_id));

  l1_voice_server_state_t ret_val;

  L1_VS_ATOMIC_LOCK_DATA(s);                                 

  ret_val = s->vocoder_state;

  L1_VS_ATOMIC_UNLOCK_DATA(s);

  return ret_val;
}


/*===========================================================================

FUNCTION l1_vs_get_vocoder_id

DESCRIPTION
  Returns the Vocoder in use (AMR, AMRWB, FR, EFR or HR)

DEPENDENCIES
  None.

RETURN VALUE
  GSM_EOK, indicating the command has been successfully executed, or
  GSM_EFAILED, indicating the command has failed.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 l1_vs_get_vocoder_id(gas_id_t gas_id)
{
  voice_server_session_data_t* s = get_session_data_ptr(geran_map_gas_id_to_nas_id(gas_id));
  uint32 ret_val;

  L1_VS_ATOMIC_LOCK_DATA(s);                                 

  ret_val = s->vocoder_id;

  L1_VS_ATOMIC_UNLOCK_DATA(s);

#ifdef VS_EXTRA_DEBUG
  MSG_GERAN_MED_1_G("l1_vs_get_vocoder_id, 0x%x", ret_val);
#endif

  return ret_val;
}


/*===========================================================================

FUNCTION l1_vs_get_dtx_mode

DESCRIPTION
  Returns true if dtx mode is enabled

DEPENDENCIES
  None.

RETURN VALUE
  GSM_EOK, indicating the command has been successfully executed, or
  GSM_EFAILED, indicating the command has failed.

SIDE EFFECTS
  None.

===========================================================================*/
boolean l1_vs_get_dtx_mode(gas_id_t gas_id)
{
  voice_server_session_data_t* s = get_session_data_ptr(geran_map_gas_id_to_nas_id(gas_id));
  boolean ret_val;

  L1_VS_ATOMIC_LOCK_DATA(s);                                 

  ret_val = s->dtx_mode;

  L1_VS_ATOMIC_UNLOCK_DATA(s);

  return ret_val;
}


/*===========================================================================
FUNCTION l1_vs_send_amr_packet_to_client

DESCRIPTION
   This function transfers data received from over the air to the vocoder.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void l1_vs_send_amr_packet_to_client (uint8 *data, 
                                      gl1_msg_amr_frame_info_type *frame, 
                                      gas_id_t gas_id)
{
  gsm_ivocoder_buffer_t      buffer;
  gsm_ivocamr_frame_info_t   amr_info;
  gsm_ivocamrwb_frame_info_t amrwb_info;

  if (!frame || !data)
  {
    MSG_GERAN_ERROR_2_G("Rcvd NULL frame %d or data %d pointers", frame, data);
    return;
  }

  buffer.flags = 1;
  buffer.vocoder_id = l1_vs_get_vocoder_id(gas_id);

  switch (buffer.vocoder_id)
  {                           
    case GSM_IVOCODER_ID_AMR:
      amr_info.frame_type = l1_vs_map_amr_frame_type_to_gsm_ivocamr(frame->dl_info.frame);
      amr_info.codec_mode = l1_vs_map_amr_mode_type_to_gsm_ivoc_codec_mode(frame->dl_info.mode);
      buffer.frame_info = (void*)&amr_info;
      break;

    case GSM_IVOCODER_ID_AMRWB:
      amrwb_info.frame_type = l1_vs_map_amr_frame_type_to_gsm_ivocamrwb(frame->dl_info.frame);
      amrwb_info.codec_mode = l1_vs_map_amr_mode_type_to_gsm_ivoc_codec_mode(frame->dl_info.mode);
      buffer.frame_info = (void*)&amrwb_info;
      break;

    default:
      buffer.flags = 0;
      MSG_GERAN_ERROR_1_G("Invalid Vocoder Type: %d", buffer.vocoder_id);
  }

  if (buffer.flags == 1)
  {
    buffer.frame = data;
    buffer.size = l1_vs_get_codec_packet_len(buffer.vocoder_id, buffer.frame_info);
    l1_vs_send_packet_to_client(&buffer, gas_id);
  }
}

/*===========================================================================

FUNCTION l1_vs_send_gsm_packet_to_client

DESCRIPTION
  This function transfers data received from over the air to the vocoder.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_vs_send_gsm_packet_to_client ( uint8 *data, boolean bfi, boolean ufi, 
                                       uint8 sid, boolean taf, gas_id_t gas_id)
{
  gsm_ivocoder_buffer_t    buffer;
  gsm_ivocefr_frame_info_t efr_info;
  gsm_ivocfr_frame_info_t  fr_info;
  gsm_ivochr_frame_info_t  hr_info;

  if (data == NULL)
  {
    MSG_GERAN_ERROR_0_G("Rcvd NULL data pointer");
    return;
  }

  buffer.flags = 1;
  buffer.vocoder_id = l1_vs_get_vocoder_id(gas_id);

  switch (buffer.vocoder_id)
  {
    case GSM_IVOCODER_ID_EFR:
      efr_info.bfi = bfi ? 1 : 0;
      efr_info.sid = 0;
      efr_info.taf = taf ? 1 : 0;
      buffer.frame_info = (void*)&efr_info;
      break;

    case GSM_IVOCODER_ID_FR:
      fr_info.bfi = bfi ? 1 : 0;
      fr_info.sid = 0;
      fr_info.taf = taf ? 1 : 0;
      buffer.frame_info = (void*)&fr_info;
      break;

    case GSM_IVOCODER_ID_HR:
      hr_info.bfi = bfi ? 1 : 0;
      hr_info.sid = sid & 0x03;
      hr_info.taf = taf ? 1 : 0;
      hr_info.ufi = ufi ? 1 : 0;
      buffer.frame_info = (void*)&hr_info;
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH	  
      if(sid == 2)   // 2 : Valid SID frame
         gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_HR_VALID_SID_RCVD);
#endif
      break;

    default:
      buffer.flags = 0;
      MSG_GERAN_ERROR_1_G("Invalid Vocoder Type: %d", buffer.vocoder_id);

  }

  if (buffer.flags == 1)
  {
    buffer.frame = data;
    buffer.size = l1_vs_get_codec_packet_len(buffer.vocoder_id, NULL);
    l1_vs_send_packet_to_client(&buffer, gas_id);
  }
}


/*===========================================================================
FUNCTION gl1_msg_vocoder_tx_amr_fr

DESCRIPTION
   This function transfers data from the vocoder to transmit over the air.

DEPENDENCIES
   None

RETURN VALUE
  DTX - FALSE means packet was good and should be transmitted,
       TRUE means packet is bad (or unavailable) and discontinuous
       transmit protocol should be used instead.

SIDE EFFECTS
   None
===========================================================================*/
boolean l1_vs_get_amr_packet_from_client (uint8 *data, uint8 datalength, 
                                          gl1_msg_amr_frame_info_type *frame,
                                          gas_id_t gas_id)
{
  gsm_ivocoder_buffer_t      buffer;
  gsm_ivocamr_frame_info_t   amr_frame_info;
  gsm_ivocamrwb_frame_info_t amrwb_frame_info;

  if (data == NULL || frame == NULL)
  {
     MSG_GERAN_ERROR_2_G("Rcvd NULL frame %d or data %d pointers", frame, data);
     return FALSE;
  }

  buffer.vocoder_id = l1_vs_get_vocoder_id(gas_id);
  buffer.frame = data;
  buffer.flags = 0;
  buffer.size = datalength;

  if (buffer.vocoder_id == GSM_IVOCODER_ID_AMRWB)
  {
    buffer.frame_info = (void*)&amrwb_frame_info;
  }
  else if (buffer.vocoder_id == GSM_IVOCODER_ID_AMR)
  {
    buffer.frame_info = (void*)&amr_frame_info;
  }
  else
  {
    frame->ul_info.frame = GL1_MSG_AMR_NO_DATA;
    frame->ul_info.mode = GL1_MSG_AMR_MODE_UNDEF;
    return FALSE;
  }

  l1_vs_get_packet_from_client(&buffer, gas_id);

  if ((buffer.flags & 0x01) == 0x1)
  {
    if (buffer.vocoder_id == GSM_IVOCODER_ID_AMRWB)
    {
      frame->ul_info.frame = 
        l1_vs_map_gsm_ivoc_frame_type_to_amr_frame_type( amrwb_frame_info.frame_type );

      frame->ul_info.mode = 
        l1_vs_map_gsm_ivoc_codec_mode_to_amr_codec_mode( amrwb_frame_info.codec_mode );
    }
    else
    {
      frame->ul_info.frame = 
        l1_vs_map_gsm_ivoc_frame_type_to_amr_frame_type( amr_frame_info.frame_type );

      frame->ul_info.mode = 
        l1_vs_map_gsm_ivoc_codec_mode_to_amr_codec_mode( amr_frame_info.codec_mode );
    }
  }
  else
  {
    frame->ul_info.frame = GL1_MSG_AMR_NO_DATA;
    frame->ul_info.mode = GL1_MSG_AMR_MODE_UNDEF;
  }

  return frame->ul_info.frame == GL1_MSG_AMR_SID_FIRST ||
         frame->ul_info.frame == GL1_MSG_AMR_SID_UPDATE;
}


/*===========================================================================
FUNCTION l1_vs_get_gsm_packet_from_client

DESCRIPTION
  This function transfers data from the vocoder to transmit over the air.

RETURN VALUE
  DTX - FALSE means packet was good and should be transmitted,
       TRUE means packet is bad (or unavailable) and discontinuous
       transmit protocol should be used instead.
===========================================================================*/
boolean l1_vs_get_gsm_packet_from_client (uint8 *data, uint8 datalength, 
                                          uint8 *packet_available, gas_id_t gas_id )
{
  gsm_ivocoder_buffer_t    buffer;
  gsm_ivocefr_frame_info_t efr_frame_info;
  gsm_ivocfr_frame_info_t  fr_frame_info;
  gsm_ivocfr_frame_info_t  hr_frame_info;
  boolean                  sid = FALSE;

  if (data == NULL || packet_available == NULL)
  {
    MSG_GERAN_ERROR_2_G("Rcvd NULL data %d or packet_available %d ptrs",
        data, packet_available);
    return FALSE;
  }

  *packet_available = FALSE;

  buffer.vocoder_id = l1_vs_get_vocoder_id(gas_id);
  buffer.frame = data;
  buffer.flags = 0;
  buffer.size = datalength;

  switch (buffer.vocoder_id)
  {
    case GSM_IVOCODER_ID_EFR:
      buffer.frame_info = (void*)&efr_frame_info;
      break;

    case GSM_IVOCODER_ID_FR:
      buffer.frame_info = (void*)&fr_frame_info;
      break;

    case GSM_IVOCODER_ID_HR:
      buffer.frame_info = (void*)&hr_frame_info;
      break;

    default:
      return FALSE;
  }

  l1_vs_get_packet_from_client(&buffer, gas_id);

  if ((buffer.flags & 0x01) == 0x1)
  {
    switch (buffer.vocoder_id)
    {
      case GSM_IVOCODER_ID_EFR:
        sid = efr_frame_info.sid == 0x02;
        break;

      case GSM_IVOCODER_ID_FR:
        sid = fr_frame_info.sid == 0x02;
        break;

      case GSM_IVOCODER_ID_HR:
        sid = hr_frame_info.sid == 0x02;
        break;

      default:
        break;
    }
    *packet_available = TRUE; 
  }

  return sid;
}


/*===========================================================================

FUNCTION gsm_call

DESCRIPTION
  Entry point for client to configure GSM voice Server

DEPENDENCIES
  None.

RETURN VALUE
  GSM_EOK, indicating the command has been successfully executed, or
  GSM_EFAILED, indicating the command has failed.

SIDE EFFECTS
  None.

===========================================================================*/
GSM_EXTERNAL uint32 gsm_call ( uint32 cmd_id,
                               void* generic_params,  
                               uint32 size )
{
  uint32 ret_val;
  voice_server_session_data_t* s;

  l1_vs_event_status_t event_status;
  l1_vs_event_code_t event_code = VS_EVENT_TYPE_INVALID;

  switch (cmd_id)
  {
    /* Client Registers a session with GSM voice Server */
    case GSM_IVOICE_CMD_OPEN:
    {
      gsm_ivoice_cmd_open_t* params = (gsm_ivoice_cmd_open_t*)generic_params;

      event_code = VS_EVENT_TYPE_ACQUIRE;

      if (sizeof(gsm_ivoice_cmd_open_t) != size)
      {
        ret_val = GSM_EBADPARAM;
        break;
      }

      if (params->asid < SYS_MODEM_AS_ID_1 || params->asid > SYS_MODEM_AS_ID_2)
      {
        ret_val = GSM_EBADPARAM;
        break;
      }

      s = get_session_data_ptr(params->asid);

      L1_VS_ATOMIC_LOCK_DATA(s);

      if (s->vocoder_state != GL1_VOICE_SERVER_CLOSED)
      {
        ret_val = GSM_EALREADY;
      }
      else
      {
        s->client_session_data = params->session_data;
        s->gsm_event_cb        = params->event_cb;
        s->ul_cb               = params->ul_cb;
        s->dl_cb               = params->dl_cb;
        s->as_id               = params->asid;
        s->vocoder_state       = GL1_VOICE_SERVER_OPENED;

        *params->ret_handle    = (uint32)s;
        ret_val = GSM_EOK;
      }
      L1_VS_ATOMIC_UNLOCK_DATA(s);
      break;
    }

    /* Client close a session with GSM voice Server */
    case GSM_IVOICE_CMD_CLOSE:
    {
      gsm_ivoice_cmd_close_t* params = (gsm_ivoice_cmd_close_t*)generic_params;
      
      event_code = VS_EVENT_TYPE_RELEASE;

      if (sizeof(gsm_ivoice_cmd_close_t) != size)
      {
        ret_val = GSM_EBADPARAM;
        break;
      }

      s = (voice_server_session_data_t*)params->handle;

      if (s != &server_session_data[SYS_MODEM_AS_ID_1] && 
          s != &server_session_data[SYS_MODEM_AS_ID_2] )
      {
        ret_val = GSM_EBADPARAM;
        break;
      }

      L1_VS_ATOMIC_LOCK_DATA(s);

      s->client_session_data = NULL;
      s->gsm_event_cb        = NULL;
      s->ul_cb               = NULL;
      s->dl_cb               = NULL;
      s->vocoder_state       = GL1_VOICE_SERVER_CLOSED; 

      L1_VS_ATOMIC_UNLOCK_DATA(s);

      ret_val = GSM_EOK;
      break;
    }

    /* Indication that GSM Voice Adapter succesfully enabled VS session.
     * vocoder is ready to exchange voice frames */
    case GSM_IVOICE_CMD_START:
    {
      gsm_ivoice_cmd_start_t* params = (gsm_ivoice_cmd_start_t*)generic_params;

      event_code = VS_EVENT_TYPE_READY_CB;

      if (sizeof(gsm_ivoice_cmd_start_t) != size)
      {
        ret_val = GSM_EBADPARAM;
        break;
      }

      s = (voice_server_session_data_t*)params->handle;

      if (s != &server_session_data[SYS_MODEM_AS_ID_1] && 
          s != &server_session_data[SYS_MODEM_AS_ID_2] )
      {
        ret_val = GSM_EBADPARAM;
        break;
      }

      L1_VS_ATOMIC_LOCK_DATA(s);

      if (s->vocoder_state == GL1_VOICE_SERVER_STARTED)
      {
        s->vocoder_state = GL1_VOICE_SERVER_READY;
        ret_val = GSM_EOK;
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
        gl1_qsh_event_notify(geran_map_nas_id_to_gas_id(s->as_id), GL1_QSH_EVENT_VOCODER_READY);
#endif
      }
      else
      {
        ret_val = GSM_EFAILED;
      }
      L1_VS_ATOMIC_UNLOCK_DATA(s);

      break;
    }

    /* Indication that the GVA succesfully placed in standby vs session
     * indicating it is safe to change VFR settings */
    case GSM_IVOICE_CMD_STOP:
    {
      gsm_ivoice_cmd_stop_t* params = (gsm_ivoice_cmd_stop_t*)generic_params;

      event_code = VS_EVENT_TYPE_NOT_READY_CB;

      if (sizeof(gsm_ivoice_cmd_stop_t) != size)
      {
        ret_val = GSM_EBADPARAM;
        break;
      }

      s = (voice_server_session_data_t*)params->handle;

      if (s != &server_session_data[SYS_MODEM_AS_ID_1] && 
          s != &server_session_data[SYS_MODEM_AS_ID_2] )
      {
        ret_val = GSM_EBADPARAM;
        break;
      }

      L1_VS_ATOMIC_LOCK_DATA(s);

      if (s->vocoder_state != GL1_VOICE_SERVER_CLOSED)
      {
        s->vocoder_state = GL1_VOICE_SERVER_OPENED;
        ret_val = GSM_EOK;
      }
      else
      {
        ret_val = GSM_EFAILED;
      }

      L1_VS_ATOMIC_UNLOCK_DATA(s);

      break;
    }

    case GSM_IVOICE_CMD_SEND_SAMPLE_RATE:
    {
      gsm_ivoice_cmd_send_sample_rate_t* params = (gsm_ivoice_cmd_send_sample_rate_t*)generic_params;

      if (sizeof(gsm_ivoice_cmd_send_sample_rate_t) != size)
      {
        ret_val = GSM_EBADPARAM;
        break;
      }

      s = (voice_server_session_data_t*)params->handle;

      if (s != &server_session_data[SYS_MODEM_AS_ID_1] && 
          s != &server_session_data[SYS_MODEM_AS_ID_2] )
      {
        ret_val = GSM_EBADPARAM;
        break;
      }

      ret_val = GSM_EOK;

      /* Forward sample rate information to RR */
      L1_send_MPH_VOICE_SAMPLE_RATE_CHANGE_IND( params->sample_rate, geran_map_nas_id_to_gas_id(s->as_id) );

      break;
    }
    default:
      ret_val = GSM_EFAILED;
  }

  MSG_GERAN_HIGH_2("VOICE SERVER, Command received: 0x%x, return value: %d", 
                   cmd_id, ret_val);

  event_status = ret_val == GSM_EOK ? VS_EVENT_STATUS_SUCCESS : VS_EVENT_STATUS_FAIL;

  l1_vs_report_event_type_1(event_code, event_status, GERAN_ACCESS_STRATUM_ID_2);

  return ret_val;
}


/*===========================================================================

FUNCTION l1_vs_send_event_to_client

DESCRIPTION
  Notify Voice client that a voice event has occurred

DEPENDENCIES
  None.

RETURN VALUE
  GSM_EOK, indicating the command has been successfully executed, or
  GSM_EFAILED, indicating the command has failed.

SIDE EFFECTS
  None.

===========================================================================*/
static uint32 l1_vs_send_event_to_client(uint32 event_id, void* params, 
                                         uint32 size, gas_id_t gas_id)
{
  voice_server_session_data_t* s = get_session_data_ptr(geran_map_gas_id_to_nas_id(gas_id));

  if (!s->gsm_event_cb)
  {
    return GSM_EFAILED;
  }

  MSG_GERAN_HIGH_1_G("l1_vs_send_event_to_client, event id 0x%x", event_id);

  return s->gsm_event_cb(s->client_session_data, event_id, params, size);
}


/*===========================================================================

FUNCTION l1_vs_get_vocoder_type

DESCRIPTION
  This function returns the vocoder type based on the chan type and
  the voc_type

DEPENDENCIES
  None.

RETURN VALUE
  ivocoder type.

SIDE EFFECTS
  None.

===========================================================================*/
static uint32 l1_vs_get_vocoder_type(gl1_msg_chan_type chan_type, 
                                     gl1_msg_voc_type voc_type)
{
  uint32 mode;

  switch (chan_type)
  {
    case GL1_MSG_TCH:
      if (voc_type == GL1_MSG_FS)
      {
        mode = GSM_IVOCODER_ID_FR;
      }
      else if (voc_type == GL1_MSG_EFS)
      {
        mode = GSM_IVOCODER_ID_EFR;
      }
      else if (voc_type == GL1_MSG_HS)
      {
        mode = GSM_IVOCODER_ID_HR;
      }      
      else
      {
        MSG_ERROR("Invalid combo: chan_type %d voc_type %d",chan_type,voc_type,0 );
        mode = GSM_IVOCODER_ID_FR;
      }
      break;

    case GL1_MSG_TCH_AFS:
    case GL1_MSG_TCH_AHS:
      mode = GSM_IVOCODER_ID_AMR;
      break;

    case GL1_MSG_TCH_WFS:
      mode = GSM_IVOCODER_ID_AMRWB;
      break;

    default:
      MSG_ERROR("VS: chan_type not set: %d default to FR",chan_type,0,0);
      mode = GSM_IVOCODER_ID_FR;
      break;

  }
  return mode;
}


/*===========================================================================

FUNCTION l1_vs_get_codec_packet_len

DESCRIPTION
  Return the buffer size in function of the codec
  type and codec mode

DEPENDENCIES
  None.

RETURN VALUE
  codec type  codec mode  bitrate kbps buffer size
  AMR-NB      0           4.75         12
  AMR-NB      1           5.15         13
  AMR-NB      2           5.9          15
  AMR-NB      3           6.7          17
  AMR-NB      4           7.4          19
  AMR-NB      5           7.95         20
  AMR-NB      6           10.2         26
  AMR-NB      7           12.2         31

  AMR-WB      0           6.6          17
  AMR-WB      1           8.85         23
  AMR-WB      2           12.65        32
  AMR-WB      3           14.25        36
  AMR-WB      4           15.85        40
  AMR-WB      5           18.25        46
  AMR-WB      6           19.85        50
  AMR-WB      7           23.05        58
  AMR-WB      8           23.85        61

  EFR                                  31
  FR                                   33
  HR                                   14

SIDE EFFECTS
  None.

===========================================================================*/

/* Helper function to get the packet size for AMR and AMRWB frames */
static uint32 l1_vs_map_amr_codec_to_len(uint32 gsm_ivocamr_codec_mode)
{
  uint32 num_entries = ARR_SIZE(codec_mode_to_len);
  uint32 i;

  for (i = 0; i < num_entries; i++)
  {
    if (gsm_ivocamr_codec_mode == codec_mode_to_len[i].amr_codec_mode)
    {
      return codec_mode_to_len[i].len;
    }
  }
  return 0;
}

static uint32 l1_vs_get_codec_packet_len(uint32 vocoder_id, void* frame_info)
{
  switch (vocoder_id)
  {
    case GSM_IVOCODER_ID_FR:
      MSG_GERAN_MED_0("FR, length 33");
      return 33;

    case GSM_IVOCODER_ID_HR:
      MSG_GERAN_MED_0("HR, length 14");
      return 14;

    case GSM_IVOCODER_ID_EFR:
      MSG_GERAN_MED_0("EFR length 31");
      return 31;

    case GSM_IVOCODER_ID_AMR:
    case GSM_IVOCODER_ID_AMRWB:
    {
      uint32 ret_value;
      uint32 codec_mode;
      uint32 frame_type;

      if (!frame_info)
      {
        return 0;
      }

      if (vocoder_id == GSM_IVOCODER_ID_AMR)
      {
        gsm_ivocamr_frame_info_t* amr_frame_info = 
                     (gsm_ivocamr_frame_info_t*)frame_info;
        codec_mode = amr_frame_info->codec_mode;
        frame_type = amr_frame_info->frame_type;
      }
      else
      {
        gsm_ivocamrwb_frame_info_t* amrwb_frame_info = 
                     (gsm_ivocamrwb_frame_info_t*)frame_info;
        codec_mode = amrwb_frame_info->codec_mode;
        frame_type = amrwb_frame_info->frame_type;
      }

      switch (frame_type)
      {
        case GSM_IVOCAMR_FRAME_TYPE_SPEECH_GOOD:
        case GSM_IVOCAMR_FRAME_TYPE_SPEECH_DEGRADED:
        case GSM_IVOCAMR_FRAME_TYPE_SPEECH_BAD:
        case GSM_IVOCAMRWB_FRAME_TYPE_SPEECH_GOOD:
        case GSM_IVOCAMRWB_FRAME_TYPE_SPEECH_DEGRADED:
        case GSM_IVOCAMRWB_FRAME_TYPE_SPEECH_BAD:
          ret_value = l1_vs_map_amr_codec_to_len(codec_mode);
          break;
          
        case GSM_IVOCAMR_FRAME_TYPE_SID_FIRST:
        case GSM_IVOCAMR_FRAME_TYPE_SID_UPDATE:
        case GSM_IVOCAMR_FRAME_TYPE_SID_BAD:
        case GSM_IVOCAMRWB_FRAME_TYPE_SID_FIRST:
        case GSM_IVOCAMRWB_FRAME_TYPE_SID_UPDATE:
        case GSM_IVOCAMRWB_FRAME_TYPE_SID_BAD:
          ret_value = 5;
          break;
    
        case GSM_IVOCAMR_FRAME_TYPE_SPEECH_ONSET:
        case GSM_IVOCAMR_FRAME_TYPE_NO_DATA:
        case GSM_IVOCAMRWB_FRAME_TYPE_SPEECH_LOST:
        case GSM_IVOCAMRWB_FRAME_TYPE_ONSET:
        default:
          ret_value = 0;
          break;
      }
      
      MSG_GERAN_MED_3("AMR codec: 0x%x, Frame type 0x%x, length %d", 
                       codec_mode, frame_type, ret_value);
      return ret_value;
    }

    default:
      MSG_GERAN_MED_0("Default, return 0");
      return 0;
  }
}


/*===========================================================================

FUNCTION l1_vs_map_amr_frame_type_to_gsm_ivocamr

DESCRIPTION
  Map gl1_msg_amr_frame_type for AMR to GSM_IVOCAMR_FRAME_TYPE

DEPENDENCIES
  None.

RETURN VALUE
  GSM_EOK, indicating the command has been successfully executed, or
  GSM_EFAILED, indicating the command has failed.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 l1_vs_map_amr_frame_type_to_gsm_ivocamr(gl1_msg_amr_frame_type frame_type)
{
  uint32 ret_val;

  if (frame_type >= GL1_MSG_AMR_SPEECH_GOOD && frame_type <= GL1_MSG_AMR_NO_DATA)
  {
    ret_val = ivocamr_frame_types_tbl[frame_type];
  }
  else
  {
    ret_val = GSM_IVOCAMR_FRAME_TYPE_NO_DATA;
  }

#ifdef VS_EXTRA_DEBUG
  MSG_GERAN_MED_2("map amr_frame_type to gsm_ivocamr, msg_amr_frame_type %d, "
                  "ivoc_frame_type 0x%x", frame_type, ret_val);
#endif

  return ret_val;
} 

/*===========================================================================

FUNCTION l1_vs_map_amr_frame_type_to_gsm_ivocamrwb

DESCRIPTION
  Map gl1_msg_amr_frame_type for AMRWB to GSM_IVOCAMRWB_FRAME_TYPE

DEPENDENCIES
  None.

RETURN VALUE
  GSM_EOK, indicating the command has been successfully executed, or
  GSM_EFAILED, indicating the command has failed.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 l1_vs_map_amr_frame_type_to_gsm_ivocamrwb(gl1_msg_amr_frame_type frame_type)
{
  uint32 ret_val;

  if (frame_type >= GL1_MSG_AMR_SPEECH_GOOD && frame_type <= GL1_MSG_AMR_NO_DATA)
  {
    ret_val = ivocamrwb_frame_types_tbl[frame_type];
  }
  else
  {
    ret_val = GSM_IVOCAMR_FRAME_TYPE_NO_DATA;
  }
    
#ifdef VS_EXTRA_DEBUG
  MSG_GERAN_MED_2("map amr frame_type to gsm_ivocamrwb, msg_amr_frame_type %d, "
                  "ivoc_frame_type 0x%x", frame_type, ret_val);
#endif

  return ret_val;
} 

/*===========================================================================

FUNCTION l1_vs_map_amr_mode_type_to_gsm_ivoc_codec_mode

DESCRIPTION
  Map gl1_msg_amr_mode_type for AMR to GSM_IVOCAMR_CODEC_MODE/GSM_IVOCAMRWB_CODEC_MODE

DEPENDENCIES
  None.

RETURN VALUE
  GSM_EOK, indicating the command has been successfully executed, or
  GSM_EFAILED, indicating the command has failed.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 l1_vs_map_amr_mode_type_to_gsm_ivoc_codec_mode(gl1_msg_amr_mode_type codec_mode)
{
  uint32 ret_val;

  if (codec_mode >= GL1_MSG_AMR_MODE_0475 && codec_mode < GL1_MSG_AMR_MODE_MAX)
  {
    ret_val = codec_mode_tbl[codec_mode];
  }
  else
  {
    ret_val = GSM_IVOCAMR_CODEC_MODE_0475;
  }
    
#ifdef VS_EXTRA_DEBUG
  MSG_GERAN_MED_2("l1_vs_map_amr_mode_type_to_gsm_ivoc_codec_mode, amr_codec_mode %d, "
                   "ivoc_codec_mode 0x%x", codec_mode, ret_val);
#endif

  return ret_val;
}




/*===========================================================================

FUNCTION l1_vs_map_gsm_ivoc_codec_mode_to_amr_codec_mode

DESCRIPTION
  Map gl1_msg_amr_mode_type for AMR to GSM_IVOCAMR_CODEC_MODE/GSM_IVOCAMRWB_CODEC_MODE

DEPENDENCIES
  None.

RETURN VALUE
  GSM_EOK, indicating the command has been successfully executed, or
  GSM_EFAILED, indicating the command has failed.

SIDE EFFECTS
  None.

===========================================================================*/
gl1_msg_amr_mode_type l1_vs_map_gsm_ivoc_codec_mode_to_amr_codec_mode(uint32 gsm_ivocamr_codec_mode)
{
  uint32 num_entries = ARR_SIZE(ivoc_codec_mode_to_amr_codec_mode);
  uint32 i;
  gl1_msg_amr_mode_type ret_val = GL1_MSG_AMR_MODE_0475;

  for (i = 0; i < num_entries; i++)
  {
    if (gsm_ivocamr_codec_mode == ivoc_codec_mode_to_amr_codec_mode[i].ivoc_codec_mode)
    {
      ret_val = ivoc_codec_mode_to_amr_codec_mode[i].amr_codec_mode;
    }
  }
      
#ifdef VS_EXTRA_DEBUG
  MSG_GERAN_MED_2("map gsm_ivoc_codec_mode to amr_codec_mode, ivoc_codec_mode 0x%x, "
                  "amr_codec_mode %d", gsm_ivocamr_codec_mode, ret_val);
#endif

  return ret_val;
}



/*===========================================================================

FUNCTION l1_vs_map_gsm_ivoc_frame_type_to_amr_frame_type

DESCRIPTION
  Map gsm_ivoc_frame_type to amr_frame_type

DEPENDENCIES
  None.

RETURN VALUE
  GSM_EOK, indicating the command has been successfully executed, or
  GSM_EFAILED, indicating the command has failed.

SIDE EFFECTS
  None.

===========================================================================*/
gl1_msg_amr_frame_type l1_vs_map_gsm_ivoc_frame_type_to_amr_frame_type(uint32 frame_type)
{
  uint32 num_entries = ARR_SIZE(ivocamr_frame_type_to_amr_frame_type);
  uint32 i;
  gl1_msg_amr_frame_type ret_val = GL1_MSG_AMR_SPEECH_BAD;

  for (i = 0; i < num_entries; i++)
  {
    if (frame_type == ivocamr_frame_type_to_amr_frame_type[i].ivocamr_frame_type)
    {
      ret_val = ivocamr_frame_type_to_amr_frame_type[i].amr_frame_type;
    }
  }

#ifdef VS_EXTRA_DEBUG
  MSG_GERAN_MED_2("map gsm_ivoc_frame_type to amr_frame_type, ivoc_frame_type 0x%x, "
                   "msg_amr_frame_type %d", frame_type, ret_val);
#endif

  return ret_val;
}



/*===========================================================================

FUNCTION get_session_data_ptr

DESCRIPTION
  Returns a pointer to the voice session data structure associated
  with a particular as_id

DEPENDENCIES
  None.

RETURN VALUE
  server_session_data pointer

SIDE EFFECTS
  None.

===========================================================================*/
static voice_server_session_data_t* get_session_data_ptr(sys_modem_as_id_e_type as_id)
{
  if (as_id >= SYS_MODEM_AS_ID_1 && as_id <= SYS_MODEM_AS_ID_2)
  {
    return &server_session_data[as_id];
  }

  MSG_GERAN_ERROR_1("Invalid as_id: %d, returning &server_session_data[0]", as_id);

  return &server_session_data[0];
}

/*===========================================================================

FUNCTION map_vocoder_type_to_event_mode

DESCRIPTION
  This function converts ivocoder codec type to event_modes. Should only be used to
  report events

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static l1_vs_event_mode_t map_vocoder_type_to_event_mode(uint32 vocoder_type)
{
  switch (vocoder_type)
  {
    case GSM_IVOCODER_ID_AMR:
      return VS_EVENT_MODE_AMR;

    case GSM_IVOCODER_ID_EFR:
      return VS_EVENT_MODE_EFR;

    case GSM_IVOCODER_ID_HR:
      return VS_EVENT_MODE_HR;

    case GSM_IVOCODER_ID_AMRWB:
      return VS_EVENT_MODE_AMR_WB;

    case GSM_IVOCODER_ID_FR:
      return VS_EVENT_MODE_FR;

    default:
      return VS_EVENT_MODE_NULL;
  }
}

/*===========================================================================

FUNCTION map_amr_mode_to_event_mode

DESCRIPTION
  This function converts msg_amr_mode to event_modes. Should only be used to
  report events

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static l1_vs_event_mode_t map_amr_mode_to_event_mode(gl1_msg_amr_mode_type amr_mode)
{
  if (amr_mode >= GL1_MSG_AMR_MODE_MAX)
  {
    return VS_EVENT_MODE_NULL;
  }
  else
  {
    return amr_mode + VS_EVENT_MODE_0475;
  }
}
/*===========================================================================

FUNCTION l1_vs_report_event_type_1

DESCRIPTION
  This function reports an event determined by type and pass the status

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static void l1_vs_report_event_type_1(l1_vs_event_code_t type, l1_vs_event_status_t status, gas_id_t gas_id)
{
  uint8 vs_event[2];
  vs_event[0] = type;
  vs_event[1] = status;

  GERAN_EVENT_REPORT_PAYLOAD(EVENT_GSM_VS_INTERFACE_TYPE_1, 2, &vs_event);
}

/*===========================================================================

FUNCTION gl1_vs_report_event_type_2

DESCRIPTION
  This function reports an event determined by type and pass the status and 
  mode

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static void l1_vs_report_event_type_2(l1_vs_event_code_t type, l1_vs_event_status_t status, l1_vs_event_mode_t mode,
                                      gas_id_t gas_id)
{
  uint8 vs_event[3];
  vs_event[0] = type;
  vs_event[1] = status;
  vs_event[2] = mode;

  GERAN_EVENT_REPORT_PAYLOAD(EVENT_GSM_VS_INTERFACE_TYPE_2, 3, &vs_event);
}
