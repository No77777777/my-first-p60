#ifndef MSG_I_H
#define MSG_I_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                   Diagnostics Packet Processing

General Description
  Diagnostic packet processing internal declarations.

Copyright (c) 2007-2016, 2018 by QUALCOMM Technologies, Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                           Edit History

$Header: //components/rel/core.mpss/10.0/services/diag/LSM/common/src/msgi.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/09/18   sa      Added Non-Public APIs.
06/04/18   gn      Deprecated unused legacy commands, Featurized commands that 
                   are applicable in a specific configuration
02/09/17   gn      Added handling for Hash values not falling under QSR region
07/25/16   gn      Unused Public API removal
04/11/16   ph      Added support for QShrink 4.0
05/20/15   sa      Added function prototype for msg_get_qsr4_table_entry
04/01/14   rs      Added support for build mask centralization. 
11/22/13   sr      Removed PACK from pointer references 
10/01/13   sa      listeners clean up function
11/15/12   is      Support for preset masks
08/31/12   sr      Support for mask retrieval command 
08/14/12   ra      Added prototype for msg_mask_binary_search 
08/10/12   sa      Mainlined DIAG_CENTRAL_ROUTING
07/05/12   ra      Features for F3 Mask Check Optimization 
06/21/12   sa      Added prototype for diag_f3_searchlist_ext
09/12/11   is      Master Diag forwards "set mask" requests via CTRL msgs
12/23/10   mad     Added prototype for msg_get_ssid_masks_adv 
05/10/10   sg      moved msg_mask type from msg.c to msgi.h
01/15/09   mad     defined MSG_I_H, updated copyright
12/02/08   mad     Created file.
===========================================================================*/

#include "diagcomm_ctrl.h" /* For diag_ctrl_msg_type */

/* This structure is used to transfer message mask
to Diag_LSM. 
*/
typedef struct
{
  uint16 ssid_first;      /* Start of range of supported SSIDs */
  uint16 ssid_last;       /* Last SSID in range */
  uint32* rt_mask_array;  /* Array of (ssid_last - ssid_first + 1) masks */
}
msg_mask_read_buffer_type;

typedef struct{
  uint32 args[10];
}msg_large_args;

#define MSG_LARGE_ARGS(X) (((msg_large_args *)X)->args)

typedef enum
{
  MSG_MASK_BT,          /* Build-time mask */
  MSG_MASK_RT           /* Run-time mask */
} msg_mask_type;
typedef enum
{
	F3_LISTENER,      /* Single Listeners */
	F3_LISTENER_EXT   /* Extended Listeners */
}f3_listener_type;

/* Message operating mode.  If a legacy message request packet is used, the message service will build legacy packet formats for MSG_SSID_LEGACY and ignore all other SSIDs. */

static enum
{
  MSG_MODE_STD = 0,         /* Standard operating mode */
  MSG_MODE_TERSE,           /* Terse mode - send the store packet as is
                               (let the tool expand the pointers). */
  MSG_MODE_LEGACY           /* Legacy packet mode */
} msg_mode;

typedef struct
{
  uint8 cmd_code;		/*!< Command code */
  uint8 sub_cmd;		/*!< Message Sub-command to query SSID range */
}
msg_get_ssid_ranges_req_type;

/*!
@ingroup diag_message_service
Structure that defines response to SSID range request query. 
Response for:
@par
command-code DIAG_EXT_MSG_CONFIG_F (125)
@par
sub-command MSG_EXT_SUBCMD_GET_RANGES (1)
*/
typedef struct
{
  uint8 cmd_code;		/*!< Command code */
  uint8 sub_cmd;		/*!< Message Sub-command */
  uint8 status;			/*!< Status of operation */
  uint8 rsvd;			/*!< Unused */

  uint32 range_cnt;		/*!< number of Subsystem ID ranges that follow, in ssid_ranges[] */

  struct
  {
    uint16 ssid_first;		/*!< First Subsystem ID for range */
    uint16 ssid_last;		/*!< Last Subsystem ID for range */
  }
  ssid_ranges[1];           /*!< SSID range array has range_cnt number of members */
}
msg_get_ssid_ranges_rsp_type;

/*!
@ingroup diag_message_service
Structure that defines request packet sent by external device to query the 
masks (both build and run-time) corresponding to a range of SSIDs.
@par
command-code DIAG_EXT_MSG_CONFIG_F (125)
@par
sub-command MSG_EXT_SUBCMD_GET_BUILD_MASK (2)
sub-command MSG_EXT_SUBCMD_GET_RT_MASK (3)
*/
typedef struct
{
  uint8 cmd_code;		/*!< Command code */
  uint8 sub_cmd;		/*!< Message Sub-command */
  uint16 ssid_start;	/*!< Start of subsystem ID range */
  uint16 ssid_end;      /*!< End of subsystem ID range */
}
msg_get_mask_req_type;

/*!
@ingroup diag_message_service
Structure that defines response to request packet sent by external device 
to query the masks(both build and run-time) corresponding to a range of SSIDs.
@par
command-code DIAG_EXT_MSG_CONFIG_F (125)
@par
sub-command MSG_EXT_SUBCMD_GET_BUILD_MASK (2)
sub-command MSG_EXT_SUBCMD_GET_RT_MASK (3)
*/
typedef struct
{
  uint8 cmd_code;		/*!< Command code */
  uint8 sub_cmd;		/*!< Message Sub-command */
  uint16 ssid_start;	/*!< Start of subsystem ID range */
  uint16 ssid_end;      /*!< End of subsystem ID range */

  uint8 status;			/*!< Status of operation */
  uint8 pad;       		/*!< unused */

  uint32 bld_mask[1];	/*!< Array of buildtime (or run-time, depending on sub-command)
                           masks for subsystems in range.  Array size is:
                           'ssid_end - ssid_start + 1' */
}
msg_get_mask_rsp_type;

/*!
@ingroup diag_message_service
Structure that defines the request packet sent by external device 
to set the run-time masks corresponding to a range of SSIDs.
@par
command-code DIAG_EXT_MSG_CONFIG_F (125)
@par
sub-command MSG_EXT_SUBCMD_SET_RT_MASK (4)
*/
typedef struct
{
  uint8 cmd_code;		/*!< Command code */
  uint8 sub_cmd;		/*!< Message Sub-command */
  uint16 ssid_start;	/*!< Start of subsystem ID range */
  uint16 ssid_end;      /*!< End of subsystem ID range */
  uint16 pad;           /*!< unused */
  uint32 rt_mask[1];	/*!< Array of runtime masks for subsystems in 
                        range.  Array size is: 'ssid_end - ssid_start + 1' */
}
msg_set_rt_mask_req_type;

/*!
@ingroup diag_message_service
Structure that defines response to the request packet sent by external device 
to set the run-time masks corresponding to a range of SSIDs.
@par
command-code DIAG_EXT_MSG_CONFIG_F (125)
@par
sub-command MSG_EXT_SUBCMD_SET_RT_MASK (4)
*/
typedef struct
{
  uint8 cmd_code;		/*!< Command code */
  uint8 sub_cmd;		/*!< Message Sub-command */
  uint16 ssid_start;	/*!< Start of subsystem ID range */
  uint16 ssid_end;      /*!< End of subsystem ID range */
  uint8 status;			/*!< Status of operation */
  uint8 pad;            /*!< unused */
  uint32 rt_mask[1];	/*!< Array of runtime masks for subsystems in 
                        range.  Array size is: 'ssid_end - ssid_start + 1' */
}
msg_set_rt_mask_rsp_type;

/*!
@ingroup diag_message_service
Structure that defines the request packet sent by external device 
to set the run-time masks for ALL SSIDs.
@par
command-code DIAG_EXT_MSG_CONFIG_F (125)
@par
sub-command MSG_EXT_SUBCMD_SET_ALL_RT_MASKS (5)
*/
typedef struct
{
  uint8 cmd_code;		/*!< Command code */
  uint8 sub_cmd;		/*!< Message Sub-command */
  uint16 rsvd;			/*!< unused */
  uint32 rt_mask;		/*!< Run-time mask to set in all Subsystems */
}
msg_set_all_masks_req_type;

/*!
@ingroup diag_message_service
Structure that defines response to the request packet sent by external device 
to set the run-time masks for ALL SSIDs.
@par
command-code DIAG_EXT_MSG_CONFIG_F (125)
@par
sub-command MSG_EXT_SUBCMD_SET_ALL_RT_MASKS (5)
*/
typedef struct
{
  uint8 cmd_code;		/*!< Command code */
  uint8 sub_cmd;		/*!< Message Sub-command */
  uint8 status;			/*!< Status of operation */
  uint8 rsvd;           /*!< unused */
  uint32 rt_mask;		/*!< New Run-time Mask for all SSIDs */
}
msg_set_all_masks_rsp_type;




/*!
@ingroup diag_message_service
@name Constants for file-name and format-string lengths for packet 31.
These constants define the maximum string-length values for command-code 31
(DIAG_MSG_F). For details, please see 80-V1294-1.
*/
/*@{*/ /* start group Constants for file-name and format-string lengths for packet 31. */
#define MSG_FILE_LEN (12+1)	       /*!< Allows full DOS file name + NULL */
#define MSG_FMT_LEN  40		       /*!< Max len of format string + NULL */
/*@}*/ /* end group Constants for file-name and format-string lengths for packet 31. */


#if defined(T_WINNT) || defined(FEATURE_WINCE)
#error code not present
#endif
/*!
@ingroup diag_message_service
This structure defines the packet with command-code 31 (DIAG_MSG_F).
This is sent by the external device to request a buffered debug message.
The structure is dictated by document: 80-V1294-1.
*/
typedef PACK(struct)
{
  byte cmd_code;        /*!< Command-code, this is set to 31 by external device */
  word msg_level;		/*!< Minimum message severity level */
}
msg_legacy_req_type;

/*!
@ingroup diag_message_service
This structure defines the response for the packet with command-code 31 
(DIAG_MSG_F), as defined in 80-V1294-1.
*/
typedef PACK(struct)
{
  byte cmd_code;          /*!< command-code, 31 */
  word qty;		          /*!< number of msg packets in the msg buffer
				            including the msg packet returned in this
				            response  -- if this field is 0, no msg
				            packet is included in this response */
  dword drop_cnt;		  /*!< number of msgs dropped by error services  */
  dword total_msgs;		  /*!< total number of msgs that have occured    */
  byte level;			  /*!< Severity level / Priority of this message */
  char file[MSG_FILE_LEN];/*!< Holds source file name */
  word line;			  /*!< Line number in source file  */
  char fmt[MSG_FMT_LEN];  /*!< printf style format string  */
  dword code1;			  /*!< first parameter to go with the format string */
  dword code2;            /*!< second parameter to go with the format string */
  dword code3;            /*!< third parameter to go with the format string */
  qword time;			  /*!< Time at which message was generated */
}
msg_legacy_rsp_type;

void msg_set_all_listener_masks(uint32 mask);
boolean msg_listeners_masks_enabled(uint32 ssid, uint32 ss_mask, f3_listener_type type);
void diag_clean_f3_listener_ext (void *param);

uint8 msg_mask_binary_search (uint16 curr_ssid);

boolean msg_get_ssid_masks_adv (msg_mask_type mask_type, uint16 ssid_start,
                                uint16 ssid_end, uint32 * mask, byte stream_id, uint8 preset_id);

PACK(void *) msg_pkt_get_local_masks(PACK(void *) req_pkt, uint16 pkt_len);
 
PACK(void *) msg_pkt_get_local_ssid_range(PACK(void *) req_pkt, uint16 pkt_len);     

void * msg_pkt_get_local_masks(void * req_pkt, uint16 pkt_len);
 
void * msg_pkt_get_local_ssid_range(void * req_pkt, uint16 pkt_len);     

void msg_set_all_rt_masks_adv (uint32 mask, byte stream_id, uint8 preset_id);
                                
void diag_switch_msg_preset_mask( void );
                                
void * msg_pkt_get_local_masks(void * req_pkt, uint16 pkt_len);
 
void * msg_pkt_get_local_ssid_range(void * req_pkt, uint16 pkt_len);

boolean diag_f3_searchlist_ext(unsigned int ssid, unsigned int ss_mask, diag_msg_type *msg);

/*===========================================================================

FUNCTION MSG_INIT

DESCRIPTION
  Initialize the message reporting services.

DEPENDENCIES
  diagbuf_init() must be called prior to this call.

RETURN VALUE
  None

SIDE EFFECTS
  Sets the counts to 0, and throws away any buffered messages.

===========================================================================*/
void msg_init (void);


/*===========================================================================

FUNCTION MSG_GET_QSR4_TABLE_ENTRY

DESCRIPTION
  This routine calculate the index in the msg_v4_ss_aam_table or msg_v4_ss_uam_table 
  and extract the ssid and mask from the msg_v4_ss_table accordingly.

PARAMETERS:
  qsr_hash_v4 - Hash value
  msg_ssid  - SSID will be populated from msg_v4_ss_table
  msg_mask  - MASK will be populated from msg_v4_ss_table

RETURN VALUE
 TRUE - QSR hash value lies within the qsr 4 aligned or unaligned boundaries.
 FALSE - otherwise.

===========================================================================*/

boolean msg_get_qsr4_table_entry(uint32 qsr_hash_v4, uint16 *msg_ssid, uint32 *msg_mask);

/*===========================================================================

FUNCTION MSG_GET_QSR_TABLE_ENTRY

DESCRIPTION
   Returns mask and ssid data for a QShrink 3.0 message

PARAMETERS
   ssid_index  -  index of message to check
   msg_ssid    -  pointer to location to store SSID
   msg_mask  -  pointer to location to store mask
            
DEPENDENCIES
   None

RETURN
   TRUE : if index is valid
   FALSE : if mask is invalid.

===========================================================================*/

boolean msg_get_qsr_table_entry(uint16 ssid_index, uint16 *msg_ssid, uint32 *msg_mask);

/*===========================================================================

FUNCTION qsr_v4_incr_drop_count_f3

DESCRIPTION
  Increments qsr_v4_drop_count_f3 when a Hash value is not falling in QSR region
===========================================================================*/
void qsr_v4_incr_drop_count_f3(void);


/* On MP Master only */
#if defined(DIAG_MP_MASTER)
void diagcomm_ctrl_send_msg_mask( diagcomm_port_type port_type, 
                                  diagcomm_enum_port_type port_num, 
                                  uint8 stream_id,
                                  uint8 preset_id,
                                  boolean send_now );

#endif /*DIAG_MP_MASTER */

/* On MP Slave only */
#if defined(DIAG_MP) && !defined(DIAG_MP_MASTER)
void diag_ctrl_update_msg_mask( diag_ctrl_msg_type * ctrl_msg );
void diag_ctrl_update_msg_preset_mask( diag_ctrl_msg_type * ctrl_msg );
void diag_ctrl_send_msg_ssid_range_ctrl_pkt(void);
void diag_ctrl_send_msg_bt_mask_ctrl_pkt(void);
#endif /* DIAG_MP && !DIAG_MP_MASTER */

/* For F3 Mask Check Optimization - Used to set
   corresponding bits in msg_enabled_mask.
   These denote if a msg mask / listener is turned
   "on" or "off"
*/
#define DIAG_MSG_NONE (0x00)
#define DIAG_MSG_STREAM_1 (0x01)
#define DIAG_MSG_STREAM_2 (0x02)
#define DIAG_MSG_LISTENER (0x04)
#define DIAG_MSG_EXT_LISTENER (0x08)

void diag_update_composite_msg_mask(int index, int start, int end);

boolean msg_sleep_vote (void);

void msg_update_ss_table (uint16 curr_ssid, uint32 ss_mask, uint8 stream_id, uint8 preset_id);

#endif /* MSG_I_H */
