#ifndef HDRAMACI_H
#define HDRAMACI_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                 H D R   A C C E S S   C H A N N E L   M A C   
                            
                             P R O T O C O L       
                                  
                   I N T E R N A L   H E A D E R   F I L E     
                     
GENERAL DESCRIPTION
  This module contains type definitions and symbol declarations that are
  internal to the HDR Access Channel Media Access Control (MAC) Protocol.

EXTERNALIZED FUNCTIONS
  hdramac_process_cmd
    Process Access Channel MAC commands from the task command queue.
    
  hdramac_process_sig
    Process Access Channel MAC signals received at task level.
    
  hdramac_process_msg
    Process received Access Channel MAC messages.
    
  hdramac_msg_cb
    Callback function that queues received Access Channel MAC messages onto 
    the task message queue.
    
  hdramacsm_change_state
    Change the protocol state.
    
  hdramacsm_get_state
    Get the current protocol state.
    
INITIALIZATION AND SEQUENCING REQUIREMENTS
  The protocol must be initialized after system startup prior to activating 
  the protocol.  The procotol must be activated before an access probe 
  sequence can be sent.

  Copyright (c) 2000-2006 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* EJECT */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath: O:/src/asw/COMMON/vcs/hdramaci.h_v   1.19   30 Nov 2001 16:33:12   omichael  $
$Header: //components/rel/hdr.mpss/6.0/mac/src/hdramaci.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/23/11   kss     Clean up of NikeL changes.
04/27/11   kss     Supported FW-controlled Tx enable. 
09/21/07   kss     Moved some definitions to hdramac.h for cleanup.
08/23/06   hal     Added support to reset the current access attempt.
06/07/06   hrk     Support for tune away during access.
11/30/05   hrk     Enhanced ACMAC updates
10/06/05   dna     Inform SD if PSIST value makes HDR unusable
07/08/05   hrk     Rev A changes for Enhanced Access Channel MAC.
06/16/05   pba     Fixed ACMAC maximum payload bytes calculation.
11/30/01   om      Renamed ENC_BIT to SEC_BIT
11/12/01   kss     Corrected type in zero persistence definition.
10/25/01   kss     Moved several declarations to hdramac.c
09/05/01   dna     Added separate handling for in-use vs. in_config messages.
08/24/01   kss     Moved config declarations to hdramacc.h
08/21/01   kss     Added two substates (AP_MSG and schedule) and moved config
                   structures here.
05/08/01   kss     Added several definitions for MSM5500.
12/11/00   kss     Added logging structure.
11/03/00   dna     Added HDR event logging
03/30/00   st      Created.

===========================================================================*/

/* EJECT */
/*===========================================================================

                         INCLUDE FILES FOR MODULE

===========================================================================*/
#include "hdr_variation.h"
#include "comdef.h"
#include "hdrbit.h"

/* EJECT */
/*===========================================================================
                   
                   DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions and declarations for constants, macros,
types, variables and other items needed by this module.

===========================================================================*/

#ifndef PERLUTF_TDTGEN_EXCLUDE
/*---------------------------------------------------------------------------
                              PROTOCOL STATES
                              
 The protocol can operate in the following states.
---------------------------------------------------------------------------*/

/* Protocol state enum type. */

/* 
   Note: The state enum values are fixed and they are used by the QXDM database
   New enum tag should be added after the current enum tag and also
   assign a fixed value
*/

typedef enum 
{
  HDRAMAC_INACTIVE_STATE=0,                      
  HDRAMAC_ACTIVE_STATE=1
} 
hdramac_state_enum_type;

/* Number of protocol states. */

#define HDRAMAC_NUM_STATES   2

/* Maximum bytes for ACMAC payload which is ACMAC data plus header without
 * length field (1 byte) */
#define HDRAMAC_PKT_MAX_DATA_LEN     255
#define HDRAMAC_PKT_FRAME_DATA_LEN    29  
#define HDRAMAC_PKT_FRAME_LEN         30  
#define HDRAMAC_PKT_HDR_LEN            8 
#define HDRAMAC_PKT_HDR_LEN_FIELD_LEN  1
#define HDRAMAC_PKT_FCS_LEN            4  

/* Packet header field macros. */

#define HDRAMAC_CLR_PKT_HDR( ptr )\
          ( *((byte*)(ptr)  ) = 0, \
            *((byte*)(ptr)+1) = 0, \
            *((byte*)(ptr)+2) = 0, \
            *((byte*)(ptr)+3) = 0, \
            *((byte*)(ptr)+4) = 0, \
            *((byte*)(ptr)+5) = 0, \
            *((byte*)(ptr)+6) = 0, \
            *((byte*)(ptr)+7) = 0  )

#define HDRAMAC_SET_PKT_LEN( ptr, var )\
          ( *(byte*)(ptr) = (byte)(var) )

#define HDRAMAC_SET_PKT_CONFIG_TOKEN( ptr, var )\
          ( *((byte*)(ptr)+1) = (byte)((var) >> 8), \
            *((byte*)(ptr)+2) = (byte)((var) >>  0))

#define HDRAMAC_SET_PKT_SIZE_BIT( ptr )\
          ( *((byte*)(ptr)+3) |= 0x40 )

#define HDRAMAC_SET_PKT_SEC_BIT( ptr )\
          ( *((byte*)(ptr)+3) |= 0x80 )

#ifdef FEATURE_HDR_REVA_L1
#define HDRAMAC_SET_PKT_PROBE_NUM( ptr, var )\
          ( *((byte*)(ptr)+3) |= ((byte)(var) & 0x0f) << 2 )
#endif /* FEATURE_HDR_REVA_L1 */

#define HDRAMAC_SET_PKT_ATI_TYPE( ptr, var )\
          ( *((byte*)(ptr)+3) |= (byte)(var) & 0x03 )

#define HDRAMAC_SET_PKT_ATI_VAL( ptr, var )\
          ( *((byte*)(ptr)+4) = (byte)((var) >> 24), \
            *((byte*)(ptr)+5) = (byte)((var) >> 16), \
            *((byte*)(ptr)+6) = (byte)((var) >>  8), \
            *((byte*)(ptr)+7) = (byte)((var)      )  )

#define HDRAMAC_SET_PKT_FCS( ptr, var)\
          ( *((byte*)(ptr)) = (byte)((var) >> 24), \
            *((byte*)(ptr)+1) = (byte)((var) >> 16), \
            *((byte*)(ptr)+2) = (byte)((var) >>  8), \
            *((byte*)(ptr)+3) = (byte)((var) >>  0))

/*---------------------------------------------------------------------------
                           PROTOCOL MESSAGE HEADER                       
---------------------------------------------------------------------------*/

/* Message ID field type. */

typedef uint8 hdramac_msg_id_type;

/* Message transaction ID field type. */

typedef uint8 hdramac_msg_trans_id_type;


/* Get message ID field. */

#define HDRAMAC_GET_MSG_ID( dsm_ptr, var )\
          HDRBIT_UNPACK8( dsm_ptr, &(var), 0, 8 )

/* Get transaction ID field. */

#define HDRAMAC_GET_TRANS_ID( dsm_ptr, var )\
          HDRBIT_UNPACK8( dsm_ptr, &(var), 8, 8 )

/*---------------------------------------------------------------------------
                        CONFIGURATION REQUEST MESSAGE                      
---------------------------------------------------------------------------*/

/* Message ID value. */

#define HDRAMAC_CFG_REQ_MSG_ID       0x50

/* Message priority. */

#define HDRAMAC_CFG_REQ_MSG_PRI        40

/*---------------------------------------------------------------------------
                       CONFIGURATION RESPONSE MESSAGE                      
---------------------------------------------------------------------------*/

/* Message ID value. */

#define HDRAMAC_CFG_RSP_MSG_ID       0x51

/* Message priority. */

#define HDRAMAC_CFG_RSP_MSG_PRI   40

/* Message length. */

#define HDRAMAC_CFG_RSP_MSG_LEN    4

/*---------------------------------------------------------------------------
                                ACACK MESSAGE                      
---------------------------------------------------------------------------*/

/* Message ID value. */

#define HDRAMAC_ACACK_MSG_ID         0x00

/* Message priority. */

#define HDRAMAC_ACACK_MSG_PRI         10

/*---------------------------------------------------------------------------
                            ACCESS PARAMETERS MESSAGE                      
---------------------------------------------------------------------------*/

/* Message ID value. */

#define HDRAMAC_ACC_PARAM_MSG_ID     0x01

/* Message priority. */

#define HDRAMAC_ACC_PARAM_MSG_PRI      30

/*---------------------------------------------------------------------------
                             ACCESS ATTEMPT CONTROL                        
---------------------------------------------------------------------------*/



/* ------------------ Access attempt timing constants -------------------- */

/* Number of frames required for transmit hardware setup prior to beginning 
   of an Access Channel Slot. */
#define HDRAMAC_TX_SETUP_FRAMES                2

/* EJECT */
/*===========================================================================
FUNCTION HDRAMACSM_CHANGE_STATE

DESCRIPTION
  This function manages the protocol state transitions.  For valid state
  transitions, the current state is exited by performing any required exit
  processing, then the new state is entered by perfroming any required entry
  processing.
  
DEPENDENCIES
  None.

PARAMETERS
  new_state - new state to enter.
  
RETURN VALUE
  None.

SIDE EFFECTS
  Protocol state is changed, unless the state transition is not permitted.
===========================================================================*/
extern void hdramacsm_change_state 
( 
  hdramac_state_enum_type new_state                  
);

/* EJECT */
/*===========================================================================
FUNCTION HDRAMACSM_GET_STATE

DESCRIPTION
  This function returns the current state of the protocol.
  
DEPENDENCIES
  None.

PARAMETERS
  None.
  
RETURN VALUE
  Current protocol state.

SIDE EFFECTS
  None.
===========================================================================*/
extern hdramac_state_enum_type hdramacsm_get_state ( void );
#endif /* PERLUTF_TDTGEN_EXCLUDE */

#endif /* HDRAMACI_H */

