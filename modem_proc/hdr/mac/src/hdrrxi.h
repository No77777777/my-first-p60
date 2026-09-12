#ifndef HDRRXI_H
#define HDRRXI_H
/*===========================================================================

       I N T E R N A L   H D R   R X   T A S K   D E F I N I T I O N S

DESCRIPTION
  This contains the internal declarations for the HDR RX task

Copyright (c) 2000 - 2011 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$PVCSPath: O:/src/asw/COMMON/vcs/hdrrxi.h_v   1.4   20 Jun 2002 11:34:54   kevins  $
$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrrxi.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/20/12   mbs     Fixed compilation errors for Triton build
07/27/11   cnx     Merged Optimized Handoff changes.
06/04/09   wsh     Removed unneeded rx.h and tx.h from HDR files
01/31/08   pba     Moved hdrrx_queue_ind() to external header file.
06/20/06   etv     Handle HDRRX timers as events.
06/20/02   kss     Removed write NV function, since it is no longer used.
09/05/01   dna     Added separate handling for in-use vs. in_config messages.
05/24/00   om      Created Module

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "hdr_variation.h"
#include "comdef.h"
#include "hdrbuf.h"

#include "hdrcmaci.h"
#include "hdrfmaci.h"

#include "hdrfw_msg.h"

#ifdef FEATURE_LTE_TO_HDR_OH
#include "emm_irat_if_msg.h"
#endif /* FEATURE_LTE_TO_HDR_OH */

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*--------------------------------------------------------------------------
                    Commands, Indications, and Messages for 
                    protocols running in the RX task context
--------------------------------------------------------------------------*/

/* Because each task handles a disjoint set of commands, it makes sense
 * to have the data type used for command buffers be unique to the task level.
 * Indications are common to all protocols, so the data type used for 
 * indication buffers is shared by all tasks that contain HDR protocols.  
 * Similarly, because DSM items are used for messages, the data type used 
 * for message buffers is shared by all tasks that contain HDR protocols.
 * See hdrbuf.h for more details.
 */
 
/** Number of message buffers available */
#define HDRRX_MSG_BUFS_NUM                                        4
  
typedef union
{
  hdrcmac_cmd_union_type cmac;          /* CCMAC Protocol Command */
  hdrfmac_cmd_union_type fmac;          /* FCMAC Protocol Command */
} hdrrx_protocol_cmd_union_type;

typedef struct
{
  hdrbuf_hdr_type                hdr;   /* Header for q item      */
  hdrrx_protocol_cmd_union_type  cmd;   /* Union of all commands  */
} hdrrx_cmd_struct_type;

typedef union
{
  hdrfw_msg_ind_u fw_ind;
#ifdef FEATURE_LTE_TO_HDR_OH
#ifdef FEATURE_IRAT_DSM_SUPPORT
  emm_irat_3gpp2_dl_msg_ind_type irat_dl_ind;
#else
  emm_irat_dl_msg_ind_type irat_dl_ind;
#endif /* FEATURE_IRAT_DSM_SUPPORT */
#endif /* FEATURE_LTE_TO_HDR_OH */
} hdrrx_ind_union_type;

typedef struct
{
  q_link_type link;
  hdrrx_ind_union_type msg;
} hdrrx_msg_s_type;

/* <EJECT> */ 
/*===========================================================================

FUNCTION HDRRX_QUEUE_CMD

DESCRIPTION
  This function queues a command on the hdrrx_event_q and sets the signal.
  The command data is copied into the command buffer before it is queued.  
  This function is called in the task context of whoever is giving the command.

DEPENDENCIES
  This routine must be multi-thread safe, as two different tasks may give
  commands to protocols in the RX task at the same time.

  cmd_size must not be larger than hdrrx_protocol_cmd_union_type, because 
  this is the data type the command gets copied into.

PARAMETERS
  protocol_name - Who the command is for
  input_cmd_ptr - Pointer to a union of all protocols in HDRRX subtask
  cmd_size      - Size of the command being given

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrrx_queue_cmd
(
  hdrhai_protocol_name_enum_type protocol_name,
  void * input_cmd_ptr, 
  uint32 cmd_size 
);

/* <EJECT> */ 
/*===========================================================================

FUNCTION HDRRX_QUEUE_MSG

DESCRIPTION
  This function queues a message on the hdrrx_event_q and sets the signal.
  The queued buffer simply holds a pointer to the DSM item chain that is
  the message.  This function is called in the task context of whoever is 
  giving the message, which is always the task in which SLP runs.

DEPENDENCIES
  None

PARAMETERS
  protocol_name - Who the message is for
  protocol_instance - Instance of the protocol the message is for
  item_ptr - Pointer to the DSM item chain holding the message

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrrx_queue_msg
( 
  hdrhai_protocol_name_enum_type  protocol_name,
  hdrhai_protocol_instance_enum_type protocol_instance,
  dsm_item_type               *item_ptr 
);

#endif   /* HDRRXI_H */
