#ifndef _MEDIA_SERVICE_EXT_API_H_
#define _MEDIA_SERVICE_EXT_API_H_
/*=*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

MEDIA_SERVICE_EXT_API.H

GENERAL DESCRIPTION

This file contains API's and callbacks for use by Media Service client on modem. 


Copyright (c) 2019 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is
regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*==*=*/

/*===========================================================================


EDIT HISTORY FOR FILE


when        who    what, where, why
--------    ---    ----------------------------------------------------------
11/07/19  amusuath  Initial creation of the file

===========================================================================*/
#include "customer.h"
#include <comdef.h>               /* common target/c-sim defines */

/** @Speech codec mode types
*/

/**
 * \enum MEDIA_SERVICE_SPEECH_CODEC_MODE_E_TYPE
 *  ENUMARATES Speech codec mode types
 */
/**
* \var TYPEDEF ENUM MEDIA_SERVICE_SPEECH_CODEC_MODE_E_TYPE
*  
*/
typedef enum{
  //see the below table for the details of these modes
  MEDIA_SERVICE_SPEECH_CODEC_MODE_0 = 0x00, 
  MEDIA_SERVICE_SPEECH_CODEC_MODE_1 = 0x01, 
  MEDIA_SERVICE_SPEECH_CODEC_MODE_2 = 0x02, 
  MEDIA_SERVICE_SPEECH_CODEC_MODE_3 = 0x03, 
  MEDIA_SERVICE_SPEECH_CODEC_MODE_4 = 0x04, 
  MEDIA_SERVICE_SPEECH_CODEC_MODE_5 = 0x05, 
  MEDIA_SERVICE_SPEECH_CODEC_MODE_6 = 0x06, 
  MEDIA_SERVICE_SPEECH_CODEC_MODE_7 = 0x07, 
  MEDIA_SERVICE_SPEECH_CODEC_MODE_8 = 0x08, 
  MEDIA_SERVICE_SPEECH_CODEC_MODE_9 = 0x09,
  MEDIA_SERVICE_SPEECH_CODEC_MODE_10 = 0x0A,
  MEDIA_SERVICE_SPEECH_CODEC_MODE_11 = 0x0B,
  MEDIA_SERVICE_SPEECH_CODEC_MODE_12 = 0x0C,
  MEDIA_SERVICE_SPEECH_CODEC_MODE_13 = 0x0D,
  MEDIA_SERVICE_SPEECH_CODEC_MODE_14 = 0x0E,
  MEDIA_SERVICE_SPEECH_CODEC_MODE_15 = 0x0F,
  MEDIA_SERVICE_SPEECH_CODEC_MODE_16 = 0x10,
  MEDIA_SERVICE_SPEECH_CODEC_MODE_17 = 0x11,
  MEDIA_SERVICE_SPEECH_CODEC_MODE_18 = 0x12,
  MEDIA_SERVICE_SPEECH_CODEC_MODE_19 = 0x13,
  MEDIA_SERVICE_SPEECH_CODEC_MODE_20 = 0x14 

}media_service_speech_codec_mode_type;

/** Table 1.0
____________________________________________________________________________
For EVS-Primary |   For EVS-AMRIO:  |    For AMR-WB:    |     For AMR:               
----------------|-------------------|-------------------|-------------------
 9 =  5.90  kbps| 0 = 6.6      kbps | 0 = 6.6      kbps |  0 = 4.75   kbps      
10 =  7.20  kbps| 1 = 8.855    kbps | 1 = 8.855    kbps |  1 = 5.15   kbps      
11 =  8.00  kbps| 2 = 12.65    kbps | 2 = 12.65    kbps |  2 = 5.9    kbps     
12 =  9.60  kbps| 3 = 14.25    kbps | 3 = 14.25    kbps |  3 = 6.7    kbps     
13 =  13.20 kbps| 4 = 15.85    kbps | 4 = 15.85    kbps |  4 = 7.4    kbps     
14 =  16.40 kbps| 5 = 18.25    kbps | 5 = 18.25    kbps |  5 = 7.95   kbps      
15 =  24.40 kbps| 6 = 19.85    kbps | 6 = 19.85    kbps |  6 = 10.2   kbps      
16 =  32.00 kbps| 7 = 23.05    kbps | 7 = 23.05    kbps |  7 = 12.2   kbps      
17 =  48.00 kbps| 8 = 23.85    kbps | 8 = 23.85    kbps |------------------
18 =  64.00 kbps|-----------------------------------------------------------
19 =  96.00 kbps|
20 = 128.00 kbps|
----------------------------------------------------------------------------
*/

/**
 * \enum MEDIA_SERVICE_SPEECH_CODEC_E_TYPE
 *  ENUMARATES Speech codec types
 */
/**
* \var TYPEDEF ENUM MEDIA_SERVICE_SPEECH_CODEC_E_TYPE
*  
*/
typedef enum {
  /** AMR NB AUDIO CODEC **/
  MEDIA_SERVICE_SPEECH_CODEC_AMR_NB,
  /** AMR WB AUDIO CODEC **/
  MEDIA_SERVICE_SPEECH_CODEC_AMR_WB,
  /** EVS AUDIO CODEC **/
  MEDIA_SERVICE_SPEECH_CODEC_EVS,
  MEDIA_SERVICE_SPEECH_CODEC_MAX = 0x7FFF
} media_service_speech_codec_type;

typedef enum {
  MEDIA_SERVICE_SPEECH_BW_NB = 0x00, /**  Narrowband  */
  MEDIA_SERVICE_SPEECH_BW_WB = 0x01, /**  Wideband */
  MEDIA_SERVICE_SPEECH_BW_SWB = 0x02, /**  Super-wideband */
  MEDIA_SERVICE_SPEECH_BW_FB = 0x03, /**  Fullband */
  MEDIA_SERVICE_SPEECH_BW_MAX = 0x7FFFF
}media_service_bandwidth_type;

/* structure to hold audio codec info.
*/
/**
 * \struct MEDIA_SERVICE_SPEECH_INFO_S_TYPE
 *  structure to hold audio codec info
 */
/**
* \var TYPEDEF STRUCT MEDIA_SERVICE_SPEECH_INFO_S_TYPE
*  
*/
typedef struct
{
  
  media_service_speech_codec_type  speech_codec;
  /** Speech codec type**/
  media_service_speech_codec_mode_type speech_codec_mode;
  /** Speech codec mode type**/
  media_service_bandwidth_type   speech_enc_bw; 
  /** Speech bandwidth */
  media_service_bandwidth_type   speech_dec_bw;
    /** Speech bandwidth */
}media_service_speech_info_type;

/**
 * \enum MEDIA_SERVICE_SUBSCRIPTION_TYPE
 *  ENUMARATES Speech codec types
 */
/**
* \var TYPEDEF ENUM MEDIA_SERVICE_SPEECH_CODEC_E_TYPE
*  
*/
typedef enum {
  MEDIA_SERVICE_SUBSCRIPTION_TYPE_NONE = -1, /**<  None \n  */
  MEDIA_SERVICE_SUBSCRIPTION_TYPE_PRIMARY = 0, /**<  Primary subscription \n */
  MEDIA_SERVICE_SUBSCRIPTION_TYPE_SECONDARY = 1, /**<  Secondary subscription \n */
  MEDIA_SERVICE_SUBSCRIPTION_TYPE_MAX = 0x7FFF
}media_service_subscription_type;

/**
 * \enum MEDIA_SERVICE_ERROR_TYPE
 *  ENUMARATES Error types
 */
/**
* \var TYPEDEF ENUM MEDIA_SERVICE_ERROR_TYPE
*  
*/
typedef enum {
  MEDIA_SERVICE_ERROR_NONE = 0, /**  No Error */
  MEDIA_SERVICE_ERROR_SUBSCRIPTION_ID_INVALID = 1, /** Subscription Id Invalid */
  MEDIA_SERVICE_ERROR_CALLBACK_NULL = 2, /** Call back parameter is NULL */
  MEDIA_SERVICE_ERROR_CALLBACK_REGISTRATION_REDUNDANT = 3 /** Callback is already registered for this Subscription */
}media_service_error_type;

/*--------------------------------------------------------------------------

  TYPEDEF VOID ( *MEDIA_SERVICE_CODEC_RATE_CHANGE_CB_TYPE )

  This function is called when the codec rate change happens.
  Call back will be triggered in Media Service context and client should not
  continue processing in the same context.
--------------------------------------------------------------------------*/
/**
 * \fn media_service_error_type MEDIA_SERVICE_HANDLE_CODEC_RATE_CHANGE_CB_TYPE(
 *                        media_service_subscription_type subscription_id,
 *												UINT8      session_id,
 *												MEDIA_SERVICE_SPEECH_INFO_S_TYPE  *speech_info_s_type,
 *											);
 * \brief This function is called when the codec rate change happens.
 * Call back will be triggered in Media Service context and client should not
 * continue processing in the same context.
 *
 * \param subscription id - subscription on which media service client is requesting rate change indications
 * \param session_id - session id to which the speech info belongs
 * \param speech_info_s_type - speech codec info
 *
 * \return VOID  
 */

typedef void ( *media_service_codec_rate_change_cb_type )
(
  /** subscription on which media service client is requesting rate change indications */
  media_service_subscription_type subscription_id, 
  uint8   session_id,                 /* session id to which the
                                       * speech codec info belongs */
  media_service_speech_info_type  *speech_info /* speech codec info */
);

/*--------------------------------------------------------------------------

  Function: MEDIA_SERVICE_REGISTER_CODEC_RATE_CHANGE_INDICATION

  This function registers a callback with Media Service for codec rate change indications.
--------------------------------------------------------------------------*/
/**
 * \fn media_service_error_type MEDIA_SERVICE_REGISTER_CODEC_RATE_CHANGE_INDICATION(
 *												media_service_subscription_type subscription_id,
 *												MEDIA_SERVICE_HANDLE_CODEC_RATE_CHANGE_CB_TYPE  codec_rate_change_cb,
 *											);
 * \brief This function registers with Media Service for codec rate change indications.
 * \param subscription id - subscription on which media service client is requesting rate change indications
 * \param codec_rate_change_cb_type - call back function pointer
 * \return media_service_error_type  0 - operation succes, otherwise failure
 *           MEDIA_SERVICE_ERROR_NONE = 0,  No Error 
 *           MEDIA_SERVICE_ERROR_SUBSCRIPTION_ID_INVALID = 1, Subscription Id Invalid
 *           MEDIA_SERVICE_ERROR_CALLBACK_NULL = 2, Call back parameter is NULL 
 *           MEDIA_SERVICE_ERROR_CALLBACK_REGISTRATION_REDUNDANT = 3, Callback is already registered for this Subscription
 *		  
 */
media_service_error_type media_service_register_codec_rate_change_indication(
  media_service_subscription_type subscription_id, 
  media_service_codec_rate_change_cb_type codec_rate_change_cb);


/*--------------------------------------------------------------------------

  Function: MEDIA_SERVICE_DISABLE_AUDIO_PAYLOAD_LOG

  This function enable or disable logging for RTP Payload.
--------------------------------------------------------------------------*/
/**
 * \fn media_service_error_type MEDIA_SERVICE_DISABLE_AUDIO_PAYLOAD_LOG(
 *                        media_service_subscription_type subscription_id,
 *                        uint8  disable_payload,
 *                      );
 * \brief This function is to disable/enable logging for RTP Payload.
 * \param subscription id - subscription on which media service client is requesting logging enable/disable
 * \param disable_payload - 1 - for disable payload in logging 0 - for enable payload in logging
 * \return media_service_error_type  0 - operation succes, otherwise failure
 *           MEDIA_SERVICE_ERROR_NONE = 0,  No Error 
 *           MEDIA_SERVICE_ERROR_SUBSCRIPTION_ID_INVALID = 1, Subscription Id Invalid
 *      
 */
media_service_error_type media_service_disable_audio_payload_log(
  media_service_subscription_type subscription_id, 
  uint8 disable_payload);

#endif /* end of _MEDIA_SERVICE_EXT_API_H_ */ /** @} */