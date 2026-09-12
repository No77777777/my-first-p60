#ifndef TDSRRCLCM_H
#define TDSRRCLCM_H
/*===========================================================================
              R R C L C M  H E A D E R  F I L E

DESCRIPTION
  This file contains data types and declarations associated with the
  L2 ACK manager and RRC Logical Channel Id manager
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/inc/tdsrrclcm.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
02/07/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.90.50
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/12/11   ad      Added changes for RRC SW decoupling
11/09/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.84.50
10/28/10   as      Added changes to check for DL ACK for Tuneaway

===========================================================================*/

/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/

/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/
#include "tdscdma_variation.h"
#include "comdef.h"

#include "dsm.h"
#include "dsmutil.h"

#include "queue.h"
#include "rex.h"

#include "tdsrrcasn1.h"
#include "tdsrrcintif.h"
#include "tdsrrccmd_v.h"
#include "tdsuecomdef.h"

/*===========================================================================
**                  CONSTANT/DEFINE DECLARATIONS
**=========================================================================*/

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
#define TDSRRCLCM_DL_DCCH_SRB5_SDU_TYPE 0xEF
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

#define TDSRRCLCM_INVALID_SDU_TYPE    0xFF     /* Invalid SDU type. This same
                                               as PDU type in ASN1 */
#define TDSRRCLCM_RLC_LC_ID_ERROR     0xFF     /* This identifies the RLC 
                                               Logical Channel allocation is
                                               failed */
                               
#define TDSRRCLCM_RLC_LC_ID_NOT_FOUND 0xFF     /* This identifies the particular
                                               requested RLC Id is not 
                                               found. */
#define TDSRRCLCM_RB_ID_NOT_ALLOCATED 0xFF     /* This identifies that particular 
                                               RB Id is not allocated */

#define TDSRRCLCM_WATERMARK_SIZE      0x1FF    /* This size is used to set 
                                               hi_watermark, low_watermark and
                                               dont_exceed_cnt fields. This 
                                               needs adjusted during the
                                               testing. */
#define TDSRRCLCM_UL_TM_START_IDX      0       /* Uplink Starting index for 
                                               Transparent mode Logical 
                                               Channel Id */
#define TDSRRCLCM_UL_UM_START_IDX UE_MAX_UL_TM_CHANNEL
                                            /* Uplink Starting index for 
                                               Unacknowledged mode Logical 
                                               Channel Id */
#define TDSRRCLCM_UL_AM_DATA_START_IDX (UE_MAX_UL_TM_CHANNEL + \
                                    TDSUE_MAX_UL_UM_CHANNEL)
                                            /* Uplink Starting index for 
                                               Acknowledged mode Logical 
                                               Channel Id for Data. */
#define TDSRRCLCM_UL_AM_CONTROL_START_IDX (UE_MAX_UL_TM_CHANNEL + \
                                       TDSUE_MAX_UL_UM_CHANNEL + \
                                       TDSUE_MAX_UL_AM_DATA_CHANNEL)
                                            /* Uplink Starting index for 
                                               Acknowledged mode Logical
                                               Channel Id for Control. */
#define TDSRRCLCM_DL_TM_START_IDX      0       /* Downlink Starting index for 
                                               Transparent mode Logical 
                                               Channel Id */
#define TDSRRCLCM_DL_UM_START_IDX UE_MAX_DL_TM_CHANNEL
                                            /* Downlink Starting index for 
                                               Unacknowledged mode Logical 
                                               Channel Id */
#define TDSRRCLCM_DL_AM_DATA_START_IDX (UE_MAX_DL_TM_CHANNEL + \
                                    TDSUE_MAX_DL_UM_CHANNEL)
                                            /* Downlink Starting index for
                                               Acknowledged mode Logical
                                               Channel Id. for Data */
#define TDSRRCLCM_DL_AM_CONTROL_START_IDX (UE_MAX_DL_TM_CHANNEL + \
                                       TDSUE_MAX_DL_UM_CHANNEL + \
                                       TDSUE_MAX_DL_AM_DATA_CHANNEL)
                                            /* Downlink Starting index for
                                               Acknowledged mode Logical 
                                               Channel Id. for control */
/*===========================================================================
**                  ENUMERATED TYPES
**=========================================================================*/

/*--------------------------------------------------------------------------- 
  Type for defining the status of a logical channel entity.  
---------------------------------------------------------------------------*/
typedef enum 
{
  TDSRRCLCM_LC_NOT_IN_USE = 0,                /* This identifies that the 
                                              particular RLC Logical Channel
                                              entity is not allocated and 
                                              not in use. */
  TDSRRCLCM_LC_IN_USE                         /* This identifies that the 
                                              particular RLC Logical Channel
                                              entity is allocated and in 
                                              use. */ 
} tdsrrclcm_lc_status_e_type; 


 
/*===========================================================================
**                      TYPE DECLARATIONS
**=========================================================================*/

/*---------------------------------------------------------------------------
** Type for RLC Logical Channel Identifer. 
---------------------------------------------------------------------------*/

typedef uint8 tdsrrclcm_mui_type;             /* Message Unit Id type for SDU
                                              to transmit in Acknowledged 
                                              mode. */
typedef enum 
{
  TDSRRCLCM_SUCCESS,                          /* Indicates successful status.*/

  TDSRRCLCM_FAILURE                           /* Indicates Failure status. */

} tdsrrclcm_status_e_type ;    

typedef struct
{
  tdsrrc_proc_e_type rrc_proc;                /* Unique identifier for RRC
                                              procedure. */
  tdsrrclcm_mui_type mui;                     /* Unique Message Unit Id for 
                                              L2 ACK.*/
} tdsrrclcm_l2_ack_info_type;
                                            
/*---------------------------------------------------------------------------
  Type for storing the Uplink RLC Logical Channel IDs infomation.
---------------------------------------------------------------------------*/
typedef struct 
{
  rlc_lc_id_type lc_id;                    /* Unique identifier for the Uplink 
                                              RLC logical channel.*/
  tdsrrclcm_lc_status_e_type status;          /* Status of RLC Logical Channel 
                                              Indentifier entity. */
  uecomdef_logch_mode_e_type  mode;        /* Identifies mode of transmission 
                                              of RLC Logical Channel entity.*/
  uecomdef_logchan_e_type lc_type;         /* Type of Logical Channel. */
  
  tdsrrc_RB_Identity rb_id;                   /* Unique Identifier for Radio 
                                              bearer.  */
  dsm_watermark_type *tx_queue;            /* Transmit data queue. */
                                          
} tdsrrclcm_ul_rlc_lc_info_type;


/*---------------------------------------------------------------------------
  Type for storing the Downlink RLC Logical Channel IDs infomation.
---------------------------------------------------------------------------*/
typedef struct 
{
  rlc_lc_id_type lc_id;                    /* Unique identifier for the 
                                              Downlink RLC logical 
                                              channel. */
  tdsrrclcm_lc_status_e_type status;          /* Status of RLC Logical Channel 
                                              Indentifier entity. */
  uecomdef_logch_mode_e_type  mode;         /* Identifies mode of transmission 
                                              of RLC Logical Channel entity. */
  uecomdef_logchan_e_type lc_type;          /* Type of Logical Channel. */
  
  tdsrrc_RB_Identity rb_id;                    /* Unique Identifier for Radio 
                                               bearer. */
  dsm_watermark_type *rx_queue;            /* Receive data queue. */
                                          
} tdsrrclcm_dl_rlc_lc_info_type;



/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION tdsrrclcm_init_ul_rlc_lc_info

DESCRIPTION
  This function will initialize Uplink RLC Logical Channel Info to the default
  values. All the Uplink RLC Logical Channel entities will be set to "Not in use"
  status.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrclcm_init_ul_rlc_lc_info
(    
  void  
);

/*===========================================================================

FUNCTION tdsrrclcm_init_dl_rlc_lc_info

DESCRIPTION
  This function will initialize Downlink RLC Logical Channel Info to the default
  values. All the Downlink RLC Logical Channel entities will be set to "Not in 
  use" status.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrclcm_init_dl_rlc_lc_info
(    
  void  
);
/*===========================================================================

FUNCTION tdsrrclcm_create_ul_dl_rlc_lc_q

DESCRIPTION
  This function will crete the Uplink/Downlinkwatermark queues

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrclcm_create_ul_dl_rlc_lc_q
(
  void
);
/*===========================================================================

FUNCTION tdsrrclcm_allocate_ul_rlc_lc_id

DESCRIPTION
  This function will allocate Uplink RLC Logical Channel Id for the requested
  RLC service entity.

DEPENDENCIES
  None

RETURN VALUE
   The allocated RLC Logical Channel Id. If failed, it'll return 
   TDSRRCLCM_RLC_LC_ID_ERROR.

SIDE EFFECTS
  None
===========================================================================*/

rlc_lc_id_type tdsrrclcm_allocate_ul_rlc_lc_id
(      
  uecomdef_logchan_e_type ul_lc_type,      /* Uplink Logical Channel Type. */
  tdsrrc_RB_Identity rb_id,                   /* Radio Bearer Id assoicated with
                                              the above Uplink Logical Channel
                                              Type. */
  uecomdef_logch_mode_e_type mode,         /* Mode of transmission for the RLC 
                                              service entity. */
  dsm_watermark_type **dsm_tx_queue        /* Pointer to the DSM Water Mark ptr
                                              associated with Uplink RLC Logical
                                              Channel Id. For Radio Bearers other
                                              than 0,1,2,3and 4 this will be 
                                              set to NULL pointer. */
);

/*===========================================================================

FUNCTION tdsrrclcm_allocate_dl_rlc_lc_id

DESCRIPTION
  This function will allocate Downlink RLC Logical Channel Id for the 
  requested RLC service entity.

DEPENDENCIES
  None

RETURN VALUE
   The allocated RLC Logical Channel Id. If failed, it'll return 
   TDSRRCLCM_RLC_LC_ID_ERROR.

SIDE EFFECTS
  None
===========================================================================*/

rlc_lc_id_type tdsrrclcm_allocate_dl_rlc_lc_id
(    
  uecomdef_logchan_e_type dl_lc_type,      /* Downlink Logical Channel Type.*/
  tdsrrc_RB_Identity rb_id,                   /* Radio Bearer Id assoicated with
                                              the above Downlink Logical 
                                              Channel type. */
  uecomdef_logch_mode_e_type mode,         /* Mode of transmission for the RLC 
                                              service entity. */
  dsm_watermark_type **dsm_rx_queue        /* Pointer to the DSM Water Mark 
                                              ptr associated with Downlink RLC 
                                              Logical Channel Id. For Radio
                                              Bearers other than 0,1,2,3,4,32
                                              33 and 34,this will be set to 
                                              NULL pointer. */
);

/*===========================================================================

FUNCTION tdsrrclcm_check_ul_rlc_lc_id

DESCRIPTION
  This function will check whether Uplink RLC Logical Channel Id is alloated 
  or not for the requested RLC service entity.

DEPENDENCIES
  None

RETURN VALUE
   The allocated RLC Logical Channel Id. If it is not alreday allocated,
   it'll return TDSRRCLCM_RLC_LC_ID_NOT_FOUND.

SIDE EFFECTS
  None
===========================================================================*/

rlc_lc_id_type tdsrrclcm_check_ul_rlc_lc_id
(    
  uecomdef_logchan_e_type ul_lc_type,      /* Uplink Logical Channel Type. */
  tdsrrc_RB_Identity rb_id,                   /* Radio Bearer Id assoicated with
                                              the above Downlink Logical 
                                              Channel type. */
  uecomdef_logch_mode_e_type mode          /* Mode of transmission for the RLC 
                                              service entity. */ 
);
/*===========================================================================

FUNCTION tdsrrclcm_check_dl_rlc_lc_id

DESCRIPTION
  This function will check whether Downlink RLC Logical Channel Id is alloated 
  or not for the requested RLC service entity. RRC LLC can use this
  function to check whether a particular Logical channel entity is already 
  setup or not. If this function returns TDSRRCLCM_RLC_LC_ID_NOT_FOUND, then the
  RRC LLC needs to setup Logical Channel entity  and the corresponding 
  physical channel for transmitting SDU on the uplink.

DEPENDENCIES
  None

RETURN VALUE
   The allocated RLC Logical Channel Id. If it is not alreday allocated,
   it'll return TDSRRCLCM_RLC_LC_ID_NOT_FOUND.

SIDE EFFECTS
  None
===========================================================================*/

rlc_lc_id_type tdsrrclcm_check_dl_rlc_lc_id
(    
  uecomdef_logchan_e_type dl_lc_type,       /* Downlink Logical Channel Type. */
  tdsrrc_RB_Identity rb_id,                    /* Radio Bearer Id assoicated with
                                               the above Downlink Logical 
                                               Channel type. */
  uecomdef_logch_mode_e_type mode           /* Mode of transmission for the RLC 
                                               service entity. */
 
);
/*===========================================================================

FUNCTION tdsrrclcm_deallocate_ul_rlc_lc_id

DESCRIPTION
  This function will deallocate Uplink RLC Logical Channel Id requested. It also 
  flushes out DSM items in the watermark queue before deallocating RLC Logical
  Channel Id.

DEPENDENCIES
  None

RETURN VALUE
   TRUE if deallocation is successful. Otherwise it returns FALSE.

SIDE EFFECTS
  None
===========================================================================*/

tdsrrclcm_status_e_type tdsrrclcm_deallocate_ul_rlc_lc_id
(    
  rlc_lc_id_type ul_lc_id               /* Uplink  RLC Logical Channel ID */
);

/*===========================================================================

FUNCTION tdsrrclcm_deallocate_dl_rlc_lc_id

DESCRIPTION
  This function will deallocate Downlink RLC Logical Channel Id requested. It 
  also flushes out DSM items in the watermark queue before deallocating RLC 
  Logical Channel Id.

DEPENDENCIES
  None

RETURN VALUE
   TRUE if deallocation is successful. Otherwise it returns FALSE.

SIDE EFFECTS
  None
===========================================================================*/

tdsrrclcm_status_e_type tdsrrclcm_deallocate_dl_rlc_lc_id
(    
  rlc_lc_id_type dl_lc_id               /* Downlink  RLC Logical Channel ID */
);
/*===========================================================================

FUNCTION tdsrrclcm_get_dl_lc_type

DESCRIPTION
  This function will identify the Logical Channel to which the OTA message
  belongs. The RRC procedure uses this infomation for processing the received 
  message.

DEPENDENCIES
  None

RETURN VALUE
   TDSRRCLCM_SUCCESS if match was found. Otherwise returns TDSRRCLCM_FAILURE.

SIDE EFFECTS
  This function should normally never return TDSRRCLCM_FAILURE. 
  If it returns TDSRRCLCM_FAILURE, then there is a race condition between Logical
  Channel Manager and RRC LLC. 
===========================================================================*/

tdsrrclcm_status_e_type tdsrrclcm_get_dl_lc_type
(    
  rlc_lc_id_type dl_lc_id,                    /* The Downlink RLC Logical
                                                 Channel Id. */
  uecomdef_logchan_e_type *dl_lc_type_ptr     /* Pointer to the Logical Channel
                                                 type assocated with the above 
                                                 Id. */
);
/*===========================================================================

FUNCTION tdsrrclcm_get_ul_lc_type

DESCRIPTION
  This function will identify the Logical Channel to which the OTA message
  belongs. The RRC procedure uses this infomation for processing the received 
  message.

DEPENDENCIES
  None

RETURN VALUE
   TDSRRCLCM_SUCCESS if match was found. Otherwise returns TDSRRCLCM_FAILURE.

SIDE EFFECTS
  This function should normally never return TDSRRCLCM_FAILURE. 
  If it returns TDSRRCLCM_FAILURE, then there is a race condition between Logical
  Channel Manager and RRC LLC. 
===========================================================================*/

tdsrrclcm_status_e_type tdsrrclcm_get_ul_lc_type
(    
  rlc_lc_id_type ul_lc_id,                    /* The uplink RLC Logical
                                                 Channel Id. */
  uecomdef_logchan_e_type *ul_lc_type_ptr     /* Pointer to the Logical Channel
                                                 type assocated with the above 
                                                 Id. */
);

/*===========================================================================

FUNCTION tdsrrclcm_identify_proc

DESCRIPTION
  This function will processes the event from the Dispatcher and identifies 
  the procedure to which the L2 ACK belongs.

DEPENDENCIES
  None

RETURN VALUE
  TDSRRCLCM_SUCCESS if there is a match for MUI received. Other it returns
  TDSRRCLCM_FAILURE

SIDE EFFECTS
  None
===========================================================================*/

tdsrrclcm_status_e_type tdsrrclcm_identify_proc
(    
  tdsrrc_cmd_type *cmd_ptr,                    /* RRC Command Type */
  tdsrrc_RB_Identity rb_id,                    /* RB Id for which MUI need to
                                               be allocated. */  
  tdsrrc_proc_e_type *proc_name                /* RRC procedure    */
);

/*===========================================================================

FUNCTION tdsrrclcm_get_mui

DESCRIPTION
  This function allocatess the MUI for the RRC procedure and RRC procedure can 
  include the same in the SDU if it is transmitting SDU in Acknowledged mode.

DEPENDENCIES
  None

RETURN VALUE
  TDSRRCLCM_SUCCESS if allocation of MUI is successful. Other it returns
  TDSRRCLCM_FAILURE

SIDE EFFECTS
  None
===========================================================================*/

tdsrrclcm_status_e_type tdsrrclcm_get_mui
(    
  tdsrrc_proc_e_type rrc_proc,                /* The RRC Procedure type for 
                                              which MUI required. */
  tdsrrc_RB_Identity rb_id,                   /* RB Id for which MUI need to
                                              be allocated. */  
  boolean l2ack_status,                    /* Indicates whether RRC needs
                                               L2 ACK or not */
  tdsrrclcm_mui_type *mui                     /* The MUI allocated for the RRC
                                              procedure. */ 
);

/*===========================================================================

FUNCTION tdsrrclcm_init_l2_ack_info

DESCRIPTION
  This function will initailize the L2 ACK info of all RRC procedures.

DEPENDENCIES
  None

RETURN VALUE
   None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrclcm_init_l2_ack_info
(    
  void                  
);

/*===========================================================================

 FUNCTION tdsrrclcm_post_event_for_dl_sdu

DESCRIPTION
  This function will post an internal event to RRC for the downlink SDU.

DEPENDENCIES
  None

RETURN VALUE
   None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrclcm_post_event_for_dl_sdu
(    
  rlc_lc_id_type dl_lc_id,                    /* The Downlink RLC Logical
                                                 Channel Id. */  
  uint8 no_of_sdus                            /* No of SDUs that are
                                                 queued */          
 , void *context
);

/*===========================================================================

 FUNCTION tdsrrclcm_post_event_for_dl_l2ack_ind

DESCRIPTION
  This function will post an internal event to RRC for the downlink L2 ACK 
  confirm indication.

DEPENDENCIES
  None

RETURN VALUE
   None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrclcm_post_event_for_dl_l2ack_ind
(    
  rlc_lc_id_type dl_lc_id,                     /* The Downlink RLC Logical
                                                  Channel Id. */  
  uecomdef_status_e_type  status,              /* Indicates the status of 
                                                  success or failure of SDU
                                                  transmission */
  uint8 mui,                                   /* Message Unit Id */
  rlc_sdu_tx_fail_reason_e_type failure_reason /* Failure reason for failed l2ack */
);
/*===========================================================================

FUNCTION tdsrrclcm_get_dl_watermark_ptr

DESCRIPTION
  This function will get watermark pointer corresponding to the RLC Logical
  channel Id.

DEPENDENCIES
  None

RETURN VALUE
   Pointer the DSM items' queue.

SIDE EFFECTS
  None
===========================================================================*/

dsm_watermark_type *tdsrrclcm_get_dl_watermark_ptr
(    
  rlc_lc_id_type dl_lc_id                  /* The Downlink RLC Logical
                                              Channel Id. */  
  
);
/*===========================================================================

FUNCTION tdsrrclcm_get_ul_watermark_ptr

DESCRIPTION
  This function will get watermark pointer corresponding to the RLC Logical
  channel Id.

DEPENDENCIES
  None

RETURN VALUE
   Pointer the DSM items' queue.

SIDE EFFECTS
  None
===========================================================================*/

dsm_watermark_type *tdsrrclcm_get_ul_watermark_ptr
(    
  rlc_lc_id_type ul_lc_id                  /* The Downlink RLC Logical
                                              Channel Id. */  
  
);
/*===========================================================================

FUNCTION tdsrrclcm_get_dl_rb_id

DESCRIPTION
  This function will get radio bearer Id  corresponding to the RLC Logical
  channel Id.

DEPENDENCIES
  None

RETURN VALUE
   tdsrrc_RB_Identity.

SIDE EFFECTS
  None
===========================================================================*/

tdsrrc_RB_Identity tdsrrclcm_get_dl_rb_id
(    
  rlc_lc_id_type dl_lc_id                  /* The Downlink RLC Logical
                                              Channel Id. */  
  
);
/*===========================================================================

FUNCTION tdsrrclcm_get_ul_rb_id

DESCRIPTION
  This function will get radio bearer Id  corresponding to the RLC Logical
  channel Id.

DEPENDENCIES
  None

RETURN VALUE
  tdsrrc_RB_Identity.

SIDE EFFECTS
  None
===========================================================================*/

tdsrrc_RB_Identity tdsrrclcm_get_ul_rb_id
(    
  rlc_lc_id_type ul_lc_id                  /* The uplink RLC Logical
                                              Channel Id. */   
);

/*===========================================================================

FUNCTION tdsrrclcm_clear_rlc_lc_info

DESCRIPTION
  This function will clear and initialize the RLC Logical Channel Id
  Info for both Uplink and Downlink.
  Data items in all watermark queues will be dequeued and released.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrclcm_clear_rlc_lc_info
( 
 void
);

/*===========================================================================

FUNCTION tdsrrclcm_get_ul_rlc_lc_id

DESCRIPTION
  This function will check whether Uplink RLC Logical Channel Id is alloated 
  or not for the requested RLC service entity. Any RRC procedure can use this
  function to check whether a particular Logical channel entity is already 
  setup or not.

DEPENDENCIES
  None

RETURN VALUE
   The allocated RLC Logical Channel Id. If it is not alreday allocated,
   it'll return TDSRRCLCM_RLC_LC_ID_NOT_FOUND.

SIDE EFFECTS
  None
===========================================================================*/

rlc_lc_id_type tdsrrclcm_get_ul_rlc_lc_id
(    
  tdsrrc_RB_Identity rb_id                     /* Radio Bearer Id assoicated with
                                               the above Downlink Logical 
                                               Channel type. */ 
);

/*===========================================================================

FUNCTION tdsrrclcm_get_dl_rlc_lc_id

DESCRIPTION
  This function will check whether Downlink RLC Logical Channel Id is alloated 
  or not for the requested RLC service entity. RRC LLC can use this
  function to check whether a particular Logical channel entity is already 
  setup or not. 

DEPENDENCIES
  None

RETURN VALUE
   The allocated RLC Logical Channel Id. If it is not alreday allocated,
   it'll return TDSRRCLCM_RLC_LC_ID_NOT_FOUND.

SIDE EFFECTS
  None
===========================================================================*/

rlc_lc_id_type tdsrrclcm_get_dl_rlc_lc_id
(    
  tdsrrc_RB_Identity rb_id                     /* Radio Bearer Id assoicated with
                                               the above Downlink Logical 
                                               Channel type. */
 
);

/*===========================================================================

FUNCTION tdsrrclcm_get_ul_rlc_mode

DESCRIPTION
  This function will check whether Uplink RLC Logical Channel Id is alloated 
  or not for the requested RLC service entity. If allocated then it returns
  assoctaed mode of transmission.

DEPENDENCIES
  None

RETURN VALUE
   Returns TDSRRCLCM_SUCCESS if RLC Logical Channel Id allocated. Otherwise returns
   TDSRRCLCM_FAILURE.

SIDE EFFECTS
  None
===========================================================================*/

tdsrrclcm_status_e_type tdsrrclcm_get_ul_rlc_mode
(    
  rlc_lc_id_type ul_lc_id,                     /* RLC Logical Channel Id */
  uecomdef_logch_mode_e_type *tx_mode          /* Mode of transmission */
);

/*===========================================================================

FUNCTION tdsrrclcm_get_dl_rlc_mode

DESCRIPTION
  This function will check whether Downlink RLC Logical Channel Id is alloated 
  or not for the requested RLC service entity. If allocated then it returns 
  associated mode reception.

DEPENDENCIES
  None

RETURN VALUE
   Returns TDSRRCLCM_SUCCESS if RLC Logical Channel Id allocated. Otherwise returns
   TDSRRCLCM_FAILURE.

SIDE EFFECTS
  None
===========================================================================*/

tdsrrclcm_status_e_type tdsrrclcm_get_dl_rlc_mode
(    
  rlc_lc_id_type dl_lc_id,                   /* RLC Logical Channel Id */
  uecomdef_logch_mode_e_type *rx_mode        /* Mode of reception */

 
);

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*===========================================================================

FUNCTION tdsrrclcm_update_db_with_srb5_wmq_info

DESCRIPTION
   This function ideally is expected to be called after a logical
   channel ID gets allocated to SRB#5 RB. During LCM initialization,
   watermark pointer is set to NULL. 

DEPENDENCIES
  None

RETURN VALUE
   Returns TRUE SRB#5 watermark is successfully updated.
           FALSE otherwise.

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrclcm_update_db_with_srb5_wmq_info(rlc_lc_id_type srb5_lc_id);

/*===========================================================================

FUNCTION tdsrrclcm_get_rb_id_if_dl_lc_type_exists

DESCRIPTION
   This function checks whether Downlink RLC Logical Channel type exists in LCM
   database or not & returns the RB Id if present. See side effect.

DEPENDENCIES
  None

RETURN VALUE
   Returns TDSRRCLCM_SUCCESS if DL RLC Logical Channel type exists.
   Otherwise returns TDSRRCLCM_FAILURE.

SIDE EFFECTS
  Please note that for the given logical channel type, only the first 
  occurence of RB ID is returned.
===========================================================================*/
tdsrrclcm_status_e_type tdsrrclcm_get_rb_id_if_dl_lc_type_exists
(    
  uecomdef_logchan_e_type  lc_type,        /* Mode of reception */
  tdsrrc_RB_Identity         *rb_id           /* Radio bearer ID */
);
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

/*===========================================================================

FUNCTION tdsrrc_check_for_pending_ul_srb_data

DESCRIPTION
  This function checks whether UE is waiting for ul srb pending data
  for SRB

DEPENDENCIES
  None

RETURN VALUE
   TRUE : If any SRB is waiting for UL srb pending data
   else returns  FALSE

SIDE EFFECTS
  None
===========================================================================*/

boolean tdsrrc_check_for_pending_ul_srb_data(void);
/*===========================================================================

FUNCTION tdsrrc_check_for_pending_dl_srb_data

DESCRIPTION
  This function checks whether UE is waiting for dl srb pending data
  for SRB

DEPENDENCIES
  None

RETURN VALUE
   TRUE : If any SRB is waiting for dl srb pending data
   else returns  FALSE

SIDE EFFECTS
  None
===========================================================================*/

boolean tdsrrc_check_for_pending_dl_srb_data(void);


/*===========================================================================

FUNCTION tdsrrclcm_get_nchan_dl_tm

DESCRIPTION
  This function will return number of DL TM channel that are used

DEPENDENCIES
  None

RETURN VALUE
   Returns number of CL TM channels that are used

SIDE EFFECTS
  None
===========================================================================*/

uint8 tdsrrclcm_get_nchan_dl_tm
(    
  void
);

#endif /* TDSRRCLCM_H */

/*==========================================================================*/
