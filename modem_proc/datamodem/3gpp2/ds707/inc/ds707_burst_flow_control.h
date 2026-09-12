#ifndef DS707_BURST_FLOW_CONTROL_H
#define DS707_BURST_FLOW_CONTROL_H
/*===========================================================================
                         D S 7 0 7 _ W M K
GENERAL DESCRIPTION
  This file contains the configuration for the watermarks which go between
  PPP and RLP.  Includes the functions called at particular watermark 
  events. 
  
  This is specific to packet data calls.

EXTERNALIZED FUNCTIONS
 DS707_WMK_UM_TX_LO_WM_FUNC
   Registered with watermark.  Called when low watermark setting reached.
   Enables flow control on the 1x ps_iface (basically allows PS to start
   sending data to RLP).
   
 DS707_WMK_UM_TX_HI_WM_FUNC
   Registered with watermark.  Called when hi watermark setting reached.
   Disables flow control on the 1x ps_iface (basically tells PS to stop
   sending data to RLP).
   
 DS707_WMK_INIT
   Called at startup.  Initializes the watermarks and underlying 
   queues.
 
 DS707_WMK_SETUP_WMKS
   Called to setup the watermarks and set various levels, etc.
 
 DS707_WMK_CLEANUP_WMKS
   Cleans up the watermarks at the end of a call.  Frees up any
   remaining DSM items.
 
 INITIALIZATION AND SEQUENCING REQUIREMENTS
   Must call ds707_wmk_init() at startup.

 Copyright (c) 2002 - 2014 by Qualcomm Technologies Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

  $PVCSPath: L:/src/asw/MM_DATA/vcs/ds707_wmk.h_v   1.2   19 Nov 2002 19:23:58   akhare  $
  $Header: //components/rel/data.mpss/3.5..2.1/3gpp2/ds707/inc/ds707_burst_flow_control.h#1 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $
  
when       who        what, where, why
--------   ---        -------------------------------------------------------
06/04/14   sd         Fix to utilize DSM memory event callbacks to enable and
                      disable flow control.
07/17/12   ms         Dynamic back pressure removal support for TCP throughput
                      optimization.
08/16/11   vpk        Support memory flow control logging
03/15/11   mg         Global variable cleanup
02/11/11   ack        Global Variable Cleanup
01/11/11   ssh        Featurize NPA code
04/26/10   gc         Added function get_prev_call_byte_counts
11/10/08   yz         Added a few functions for RX/TX byte count fix
08/20/07   sk         API is provided to count free watermark
11/09/06   vrk        Post command to DS to process tx high and low wmk
03/16/06   sk         Updated watermark behavior
12/23/05   ksu        EMPA RSP: two fwd rx queues
12/23/05   ksu        EMPA RSP: two rev watermarks per ip flow
12/16/05   TMR        Replaced RLP RX queue with watermark
10/03/05   vas        Changed prototype of ds707_wmk_alloc_sec_wm to take PS 
                      Flow as argument
06/13/05   vas        Rewrite of code to support Semi-dynamic QoS. Added new
                      external functions.
09/10/04   vas        QOS related changes
05/21/03   vas        Removed AN watermarks. moved them to separate file.
07/22/02   ak         Added ds707_cleanup_wmk to file.
02/20/02   ak         First version of file.
===========================================================================*/


/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"
#ifdef FEATURE_DATA_IS707

#ifdef FEATURE_DATA_FLOW_MGMT
#include "ds3g_burst_flow_control.h"

/*---------------------------------------------------------------------------
  Structure to hold burst parameters
---------------------------------------------------------------------------*/
typedef struct 
{
  ds3g_burst_based_flow_control_s    burst_params;
  rex_timer_type                     burst_periodic_timer;
  boolean                            burst_periodic_timer_running;
}ds707_burst_flow_control_s;


/*===========================================================================
                        EXTERNAL FUNCTIONS
===========================================================================*/

/*===========================================================================
FUNCTION      DS707_BURST_CNTRL_HYSTERESIS_TIMER_CB

DESCRIPTION   This is the callback that is called when the bursty flow 
              hysteresis timer expires.               

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds707_burst_cntrl_periodic_timer_cb
(
  unsigned long cb_data
);

/*===========================================================================
FUNCTION      DS707_BURST_CNTRL_PROHIBIT_TIMER_CB

DESCRIPTION   This is the callback that is called when the bursty flow 
              hysteresis timer expires.               

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/

void ds707_burst_cntrl_prohibit_timer_cb
(
  unsigned long cb_data
);

/*===========================================================================
FUNCTION      DS707_BURST_CNTRL_PROCESS_HYSTERESIS_TIMER_EXPIRY_CMD

DESCRIPTION   This function takes care of incrementing the unblock_timer_count 
              associated with each bearer after the hysteresis timer expiry.
              If the count reaches 10, flow enable will be sent

DEPENDENCIES  None

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds707_burst_cntrl_process_periodic_timer_expiry_cmd
(
  void                           *payload_ptr
);

/*===========================================================================
FUNCTION      DS707_BURST_CNTRL_PROCESS_PROHIBIT_TIMER_EXPIRY_CMD

DESCRIPTION   This function takes care of processing the prohibit timer 
              expiry command 

DEPENDENCIES  None

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds707_burst_cntrl_process_prohibit_timer_expiry_cmd
(
  void                           *payload_ptr
);


/*===========================================================================
FUNCTION      DS707_BURST_CNTRL_SET_BURST_HYSTERESIS_TIMER

DESCRIPTION   This util function sets the burst hysteresis timer as part of 
              1st bearer getting added or after the expiry of hysteresis
              timer to reset the timer              

DEPENDENCIES  None

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds707_burst_cntrl_set_burst_periodic_timer
(
  void
);

/*===========================================================================
FUNCTION      DS707_BURST_CNTRL_STOP_BURST_HYSTERESIS_TIMER

DESCRIPTION   This util function stops the burst hysteresis timer as part of 
              last bearer going down.

DEPENDENCIES  None

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds707_burst_cntrl_stop_burst_periodic_timer
(
  void
);

/*===========================================================================
FUNCTION      DS_707_BURST_FLOW_TX_GONE_EMPTY_FUNC

DESCRIPTION   This function enables flow as part of wm going empty

DEPENDENCIES  None

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_707_burst_flow_tx_gone_empty_func
(
  struct dsm_watermark_type_s     *wm_ptr,
  void                            *user_data_ptr
);

/*===========================================================================
FUNCTION      DS707_BURST_CNTRL_GONE_EMPTY_CMD_HDLR

DESCRIPTION   This function enables flow as part of wm going empty

DEPENDENCIES  None

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds707_burst_cntrl_gone_empty_cmd_hdlr
(
  void                           *payload_ptr
);

#endif /* FEATURE_DATA_FLOW_MGMT */

#endif /* FEATURE_DATA_IS707 */
#endif /* DS707_BURST_FLOW_CONTROL_H */
