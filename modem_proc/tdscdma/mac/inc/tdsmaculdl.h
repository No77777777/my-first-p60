#ifndef     TDSMACULDL_H
#define     TDSMACULDL_H
/*=========================================================================================

                   U P L I N K   D O W N L I N K   M A C  H E A D E R   F I L E

This file contains functions that would be used both by the UL and DL task.
Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


========================================================================================*/


/*========================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/msm5200/mac/vcs/maculdl.h_v   1.0   11 Jun 2002 18:37:16   skrishna  $
$Header: //components/rel/tdscdma.mpss/4.3/mac/inc/tdsmaculdl.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------
10/24/10   guo     Initial release Upmerged WCDMA to TDS

=========================================================================================*/
#include "tdscdma_variation.h"
#include  "customer.h"
#include  "rex.h"
#include  "msg.h"
#include  "task.h"
#include  "tdsl2dltask.h"
#include  "tdsl2ultask.h"
#include  "tdsl1sapcommon.h"
#include  "tdsl1mtask.h"
#include "tdsl2ultask.h"
#include "tdsl2dltask.h"

#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY

#define TDSMAX_NUM_CFN 10

#endif


/*==============================================================================================
 FUNCTION NAME  tdsl1_post_ul_mac_config_sig

 FUNCTION   This function is called by physical layer and it posts the config signal to l2 command
            queue.
===============================================================================================*/
void tdsl1_post_ul_mac_config_sig (void) ;


/*==============================================================================================
 FUNCTION NAME  tdsl1_post_dl_mac_config_sig

 FUNCTION   This function is called by physical layer and it posts the config signal to l2 command
            queue.
===============================================================================================*/
void tdsl1_post_dl_mac_config_sig (void) ;

/*==============================================================================================
 FUNCTION NAME  tdsl1_post_dl_mac_config_sig_at_l1_failure

 FUNCTION   This function is called by physical layer and it posts the config signal to l2 command
            queue when there is failure to process TDSL1_CPHY_SETUP_REQ and MAC UL or DL sync for MAC
            config is requested
===============================================================================================*/
void tdsl1_post_dl_mac_config_sig_at_l1_failure (void);

/*==============================================================================================
 FUNCTION NAME  tdsl1_post_mac_hs_config_sig

 FUNCTION   This function is called by physical layer and it posts the HS config signal to l2 command.
===============================================================================================*/
void tdsl1_post_mac_hs_config_sig (void);


/*==============================================================================================
 FUNCTION NAME  tdsmaci_post_l1_config_ind

 FUNCTION   This function posts to L1 the configuration sucess message. This function can be
            called by both the UL and DL MAC to post a configuration sucess message to L1.

===============================================================================================*/
void  tdsmaci_post_l1_config_ind (tdsl1_cmd_enum_type cmd_id);


#ifdef FEATURE_TDSCDMA_TC_CONSTANT_DELAY
#define TDSLB_DELAY_INVALID_CFN 0XFFFF
typedef struct{
 boolean first_tb; /* True Indicates if this is the first transport block that is received */
 uint16   dl_cfn[TDSMAX_NUM_CFN]; /* Downlink CFN */
 uint8   delay;     /* The constant delay which is applied to every transport block.*/
 uint8   cur_idx;   /* Index where the downlink cfn is stored */
 uint8   start_idx; /* Index where the Uplink task starts reading */
}tdslb_delay_struct_type;

/* Stores the current ul cfn */
extern uint8  tdscur_ul_cfn;

#endif /* FEATURE_TDSCDMA_TC_CONSTANT_DELAY */

#endif
