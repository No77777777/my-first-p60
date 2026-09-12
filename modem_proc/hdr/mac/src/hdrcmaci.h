#ifndef HDRCMACI_H
#define HDRCMACI_H
/*! @mainpage This module contains the declarations of the HDR Control 
              Channel Media Access Control (MAC) Protocol exported only
              to MAC module.
    @file hdrcmaci.h
    @brief This module contains definitions and declarations necessary for 
           using the HDR Control Channel MAC Module. */

/*===========================================================================
  Copyright (c) 2000 - 2014 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.
===========================================================================*/

/* EJECT */
/*===========================================================================
                           
                           EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath: O:/src/asw/COMMON/vcs/hdrcmaci.h_v   1.12   19 Feb 2002 12:33:06   kevins  $
$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrcmaci.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/14/14   wsh     Changes to support Jolokia bringup
08/19/13   rmg     Added support to disable DLNA in TAP mode.
06/06/12   lij     Updated for Dime.
12/10/10   kss     Fixed sync issue by validating sync pkt hstr.
10/12/10   kss     Added enable/disable missed scc detection commands.
02/28/07   etv     Removed unused enum type definition.
06/20/06   etv     Handle HDRRX timers as events.
07/06/06   mpa     Added cmac timer start/stop interfaces.
06/21/05   etv     Added support for Rev-A CMAC. 
02/12/02   kss     Removed NO_CMAC_DDR_9_10 support.
06/07/01   kss     Added support for DDRs 9 and 10.
05/08/01   kss     Added macros for SCC offset.
05/02/01   kss     Changed supervision timer for IS-856.
11/03/00   dna     Added HDR event logging
03/30/00   st      Created.

===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE
                               
===========================================================================*/
#include "hdr_variation.h"
#include "comdef.h"
#include "rex.h"
#include "hdrcmac.h"
#include "hdrdec.h"

/* EJECT */
/*===========================================================================
                   
                   DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions and declarations for constants, macros,
types, variables and other items needed by this module.

===========================================================================*/

/*---------------------------------------------------------------------------
                              PROTOCOL COMMANDS
                              
 The following codes are for use with the 'cmd' field of the protocol command
 header type defined below, to define the operation requested by the command.                              
---------------------------------------------------------------------------*/

/**
  Note: The cmd enum values are fixed and they are used by the QXDM database
  New enum tag should be added after the current enum tag and also
  assign a fixed value */
typedef enum
{
  HDRCMAC_DEACTIVATE_CMD=0,
    /**< Deactivate protocol */
  HDRCMAC_ACTIVATE_CMD=1,
    /**< Activate protocol. */
  HDRCMAC_ENABLE_MISSED_SCC_DETECTION_CMD=2,
    /**< Enable missed SCC detection */
  HDRCMAC_DISABLE_MISSED_SCC_DETECTION_CMD=3,
    /**< Disable missed SCC detection */

#ifdef FEATURE_HDR_DLNA
  HDRCMAC_FORCE_DLNA_DISABLE_CMD=4
    /**< Force DLNA disable */
#else
  HDRCMAC_RESERVED_ID_4_CMD=4
    /**< Reserved command id */
#endif /* FEATURE_HDR_DLNA */

} hdrcmac_cmd_enum_type;

/**
  The command header type is contained in every protocol command. It
  contains info common to all commands. */
typedef struct 
{
  hdrcmac_cmd_enum_type  cmd;
    /**< Specifies which command to perform */
  hdrhai_protocol_name_enum_type sender;
    /**< protocol giving command, for debug */
} hdrcmac_cmd_hdr_type;

/**
  ACTIVATE COMMAND. 
  This command is given to activate the protocol. */
typedef struct 
{
  hdrcmac_cmd_hdr_type   hdr;
    /**< Common header */
} hdrcmac_act_cmd_type;

/**
  DEACTIVATE COMMAND 
  This command is given to de-activate the protocol. */
typedef struct 
{    
  hdrcmac_cmd_hdr_type   hdr;
    /**< Common header */
} hdrcmac_deact_cmd_type;

/**
  ENABLE_MISSED_SCC_DETECTION COMMAND */ 
typedef struct 
{    
  hdrcmac_cmd_hdr_type   hdr;
    /**< Common header */
  boolean clear_pending_ind;
    /**< Whether to clear any pending indication on enable. */
} hdrcmac_enable_scc_cmd_type;

/**
  DISABLE_MISSED_SCC_DETECTION COMMAND */ 
typedef struct 
{    
  hdrcmac_cmd_hdr_type   hdr;
    /**< Common header */
} hdrcmac_disable_scc_cmd_type;

#ifdef FEATURE_HDR_DLNA
/**
 FORCE_DLNA_DISALE COMMAND */  
typedef struct 
{    
  hdrcmac_cmd_hdr_type   hdr;
    /**< Common header */
  hdrcmac_caller_id_enum_type caller_id;
    /**< Requesting entity */
  boolean disable;
    /**< Whether to enforce DLNA disable if it is enabled via NV */
} hdrcmac_force_dlna_disable_type;
#endif /* FEATURE_HDR_DLNA */

/**
  UNION OF PROTOCOL COMMANDS 
  This union type includes all command types, as well as the common header
  type, to define a type that can contain any of the command types. */
typedef union 
{
  hdrcmac_cmd_hdr_type   hdr;
    /**< Common header struct */
  hdrcmac_act_cmd_type   act;
    /**< Activate command struct */
  hdrcmac_deact_cmd_type deact;
    /**< Deactivate command struct */
  hdrcmac_enable_scc_cmd_type enable_scc;
    /**< Enable missed SCC detection struct */
  hdrcmac_disable_scc_cmd_type disable_scc;
    /**< Disable missed SCC detection struct */

#ifdef FEATURE_HDR_DLNA
  hdrcmac_force_dlna_disable_type disable_dlna;
    /**< Force DLNA disable struct */
#endif /* FEATURE_HDR_DLNA */

} hdrcmac_cmd_union_type;
                 
/**
  PROTOCOL SUPERVISON TIMER
  The Control Channel supervision timer is set for 12 Control Channel cycles,
  which is 4.8 seconds.  The timer is restarted every time a Synchronous
  Control Channel Capsule is received. */
#define HDRCMAC_SUPV_TIMER_MSECS   5120


/*---------------------------------------------------------------------------
                        SYNCHRONOUS CAPSULE TIMESTAMP

 The demodulator timestamp value is recorded every time the first Control
 Channel packet of a Synchronous Control Channel Capsule is received.
 Initially, the timestamp value is invalid, and only becomes valid after the
 first Synchronous Control Channel Capsule is received.
 
  A demodulator Half Slot Time Reference timestamp is included with each
 received packet.  The timestamp is generated by a free running counter that
 has a precision of 1/2 slot per LSB and rolls over every 400ms.  The valid
 range of values for the timestamp is 0 through 479.
---------------------------------------------------------------------------*/

/** Timestamp status enum type. */
typedef enum 
{
  HDRCMAC_TIMESTAMP_VALID,
  HDRCMAC_TIMESTAMP_INVALID
} hdrcmac_timestamp_status_enum_type;

/* Number of Sync Capsule timestamps to buffer. */
#define HDRCMAC_TIMESTAMP_BUFFER_COUNT 2

/** Timestamp struct type. */
typedef struct
{
  hdrcmac_timestamp_status_enum_type  status;
    /* Indicates we have at least one valid hstr saved in buffer */

  struct 
  {
    boolean                           valid;
    hdrdec_half_slot_time_type        value;
  } info[HDRCMAC_TIMESTAMP_BUFFER_COUNT];
    /* Half-slot timestamp info buffer, used for sync. */

  int                                 idx;
    /* Index for half-slot timestamp buffer. Behaves as a circular buffer. */

  uint8                             scc_offset;
    /* Scc offset */

} hdrcmac_timestamp_struct_type;


/*---------------------------------------------------------------------------
                            PROTOCOL DATA UNIT
                         
 The Control Channel packet is 1003 bits in length.  The packet begins with 
 a header followed by one or more Security Layer packets.  The last three
 bits correspond to the MAC Layer packet Format (FMT) bits, and are always
 set to '000' for Control Channel packets.  Each Security Layer packet is 
 preceeded by a dedicated packet header.  All bits after the end of the last 
 Security Layer packet are set to zero.
---------------------------------------------------------------------------*/

/* ---------------- Control Channel packet field lengths ----------------- */

/** Control Channel packet length, excluding FMT bits. */
#define HDRCMAC_CC_PKT_LEN       125

/** Control Channel packet header length. */
#define HDRCMAC_CC_PKT_HDR_LEN     1


/* ---------------- Control Channel packet header macros ----------------- */
#ifdef FEATURE_HDR_RUMI
#define HDRCMAC_IS_SYNC_CAPS_PKT( ptr )  TRUE
#define HDRCMAC_IS_FIRST_CAPS_PKT( ptr ) TRUE
#define HDRCMAC_IS_LAST_CAPS_PKT( ptr )  TRUE
#define HDRCMAC_GET_OFFSET( ptr )        0
#define HDRCMAC_IS_SLEEP_CAPS_DONE( ptr ) TRUE
#else
/**
  SynchronousCapsule bit. A non-zero value indicates that the Control
  Channel packet is part of a Synchronous Capsule. */
#define HDRCMAC_IS_SYNC_CAPS_PKT( ptr )\
          ( ( ( *(byte*)(ptr) & 0x80 ) != 0 ) ? TRUE : FALSE )

/**
  FirstPacket bit. A non-zero value indicates that the Control Channel packet 
  is the first in a Synchronous Capsule or an Asynchronous Capsule. */
#define HDRCMAC_IS_FIRST_CAPS_PKT( ptr )\
          ( ( ( *(byte*)(ptr) & 0x40 ) != 0 ) ? TRUE : FALSE )

/**
  LastPacket bit. A non-zero value indicates that the Control Channel packet 
  is the last in a Synchronous Capsule or an Asynchronous Capsule. */
#define HDRCMAC_IS_LAST_CAPS_PKT( ptr )\
          ( ( ( *(byte*)(ptr) & 0x20 ) != 0 ) ? TRUE : FALSE )

/**
  Offset bits.  This is the offset of the synchronous control channel
  relative to the control channel cycle. */
#define HDRCMAC_GET_OFFSET( ptr )\
          ( ( *(byte*)(ptr) & 0x18 ) >> 3 )

/**
  SleepStateCapsuleDone bit. Set if the packet is the last in a CC sync
  sleep state capsule. */
#define HDRCMAC_IS_SLEEP_CAPS_DONE( ptr )\
          ( ( ( *(byte*)(ptr) & 0x04 ) != 0 ) ? TRUE : FALSE )

#endif /* FEATURE_HDR_DIME_RUMI */


/* ----------------- Security Layer packet field lengths ----------------- */

/** Security Layer packet header length for BATI packet header. */
#define HDRCMAC_SL_PKT_BATI_HDR_LEN   2

/** Security Layer packet header length for UATI packet header. */
#define HDRCMAC_SL_PKT_UATI_HDR_LEN   6

/** Security Layer packet header length field length. */
#define HDRCMAC_SL_PKT_LEN_FIELD_LEN  1


/* ----------------- Security Layer packet header macros ----------------- */

/** Security Layer packet length. */
#define HDRCMAC_GET_PKT_LEN( ptr )\
          ( *(byte*)(ptr) )

/** Security Layer packet ATI type. */
#define HDRCMAC_GET_PKT_ATI_TYPE( ptr )\
          ( *((byte*)(ptr) + 1) & 0x3 )

/** Security Layer packet ATI value. */
#define HDRCMAC_GET_PKT_ATI_VAL( ptr )\
          ( (*((byte*)(ptr) + 2) << 24) | (*((byte*)(ptr) + 3) << 16) |\
            (*((byte*)(ptr) + 4) <<  8) | (*((byte*)(ptr) + 5)) )

/**
  Security Layer packet format status bit. A non-zero value indicates
  that the Security Layer packet is format B. */
#define HDRCMAC_PKT_IS_SIZE( ptr )\
          ( ( ( *( (byte*)(ptr) + 1 ) & 0x40 ) != 0 ) ? TRUE : FALSE )

/**
  Security Layer packet encryption status bit. A non-zero value indicates
  that the Security Layer packet is encrypted. */
#define HDRCMAC_PKT_IS_ENCR( ptr )\
          ( ( ( *( (byte*)(ptr) + 1 ) & 0x80 ) != 0 ) ? TRUE : FALSE )

/* NOTE: The pointer argument in these macros is assumed to point to the 
   ----  *first* byte of the header field, and the bytes in the header field 
         are assumed to be stored in *network* order. */


/*---------------------------------------------------------------------------
                          VARIABLE DECLARATIONS
                         
 The following variables are used in one or more modules of this protocol.
---------------------------------------------------------------------------*/

/** Demodulator half-slot timestamp. */
extern hdrcmac_timestamp_struct_type hdrcmac_timestamp;

/*============================================================================
  HDRCMAC_PROCESS_CMD
*/
/**
  This function processes the commands for this protocol that were queued
  on the task command queue from a different task context.

  @context HDRRX task. */
/*============================================================================*/
extern void hdrcmac_process_cmd 
( 
  const hdrcmac_cmd_union_type *cmd_ptr                
    /**< pointer to received command buffer */
);

/*============================================================================
  HDRCMAC_PROCESS_TIMER
*/
/**
  This function handles the timer expiry events assigned to this protocol.

  @context HDRRX task. */
/*============================================================================*/
extern void hdrcmac_process_timer
( 
  uint32 timer_id
    /* ID of the timer just expired. */
);

/*============================================================================
  HDRCMAC_PUT_PKT
*/
/**
  This function parses a Control Channel packet for Security Layer packets.
  Security Layer packets that are of broadcast type are forwarded to the 
  Security Layer for processing.  Security Layer packets that are of mobile
  directed type are forwarded to the Security Layer if the ATI value matches
  the ATI value currently in use by the Security Layer.  All other Security
  Layer packets are discarded.

  @sideeffects The input Control Channel packet buffer is freed.
  
  @context HDRRX task. This routine is NOT reentrant. */
/*============================================================================*/
extern void hdrcmac_put_pkt 
( 
  dsm_item_type      *cc_pkt_ptr          
    /**< Pointer to buffered Control Channel Packet. */
);

/*============================================================================
  HDRCMAC_SYNC_CC_IS_MISSING
*/
/**
  Determines whether a SyncCC packet (part of a SyncCC capsule) is missing.
  This is done by checking cc_start and the last_packet_flag.
  Must only be called when DSP detects that the AT did not see preamble for a 
  SyncCC packet when it should have detected one, or when AT did not see 
  preamble for subSync packet when it could have detected one.

  @dependencies Should only be called after DSP detects a missed preamble.

  @return TRUE if synchronous control channel packet preamble was missed and 
  FALSE if a sub-synchronous control channel packet preamble was missed.

  @context HDRRX task. This routine is NOT reentrant. */
/*============================================================================*/
extern boolean hdrcmac_sync_cc_is_missing
(
  uint16  *packet_num
    /**< Determined packet number of packet missed if continued packet. 
         (Packet_num n means nth packet of sync capsule missed, starting 
         from 1.) */
);

#ifdef FEATURE_HDR_REVA_L1

/*============================================================================
  HDRCMAC_PROCESS_IND
*/
/**
  This function processes the indications given to CMAC.

  @context HDRRX task. This routine is NOT reentrant. */
/*============================================================================*/
extern void hdrcmac_process_ind
(
  hdrind_ind_name_enum_type ind_name,
    /**< Name of the indication for CMAC to process. */
  hdrind_ind_data_union_type *ind_data_ptr
    /**< pointer to data associated with the indication. */
);

#endif /* FEATURE_HDR_REVA_L1 */

/*============================================================================
  HDRCMAC_START_TIMER
*/
/**
  This function starts or resumes the CMAC supervision timer

  @context Calling task (HDRRX, HDRMC). This routine is NOT? reentrant. */
/*============================================================================*/
extern void hdrcmac_start_timer( void );

/*============================================================================
  HDRCMAC_STOP_TIMER
*/
/**
  This function stops the CMAC supervision timer

  @context Calling task (HDRRX, HDRMC). This routine is NOT? reentrant. */
/*============================================================================*/
extern void hdrcmac_stop_timer( void );

/*============================================================================
  HDRCMAC_UPDATE_LINK
*/
/**
  This function updates the link id indicating the serving sector. This
  is the sector on which we are monitoring the control channel.

  @context Calling task. This routine is reentrant. */
/*============================================================================*/
extern void hdrcmac_update_link
(
  uint8 link_id
    /**< link id of serving sector. */
);

#endif /* HDRCMACI_H */
