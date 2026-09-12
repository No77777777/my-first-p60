#ifndef UL_EVT_H
#define UL_EVT_H

/*===========================================================================
                             U L _ E V T . H

GENERAL DESCRIPTION
  This contains prototypes for uplink controller sequencer event handlers.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS  
  

Copyright (c) 2000,2001 by Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================*/
#include "customer.h"

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/ulevt.h_v   1.4   19 Apr 2001 18:27:12   skeshava  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/ulevt.h#3 $ $DateTime: 2020/01/22 08:56:27 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
12/23/15    raj     Ensure that EUL start and stop are not sent in the same frame
07/24/12    geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
06/08/12    gv      Feature HS RACH Initial code check in
02/10/12    rgn     Nikel feature cleanup
02/01/12    gnk     Nikel feature cleanup
02/23/11    asm     Cleanup of unused, mainlined or irrelevant features in offline UL code
05/22/09    sa      Fixed compiler warnings
04/22/09    gnk     Fixed compiler warnings
10/13/08    oh      Fixed a featurization issue.
10/03/08    hk      Bringing in 7k Mailine fixes
08/14/06    yiz     Mainline HSUPA code
09/22/00    sk      file created.
                    
===========================================================================*/

/*===========================================================================
FUNCTION        wenc_phychan_get_data

DESCRIPTION     This function processes data received from L2. It sets up
                appropriate control in the encoder hardware so that the data
                is transmitted properly after encoding.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern void wenc_phychan_get_data( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        E_UL_EDPCH_CFG_EVENT_HANDLER

DESCRIPTION     This function sends the mdsp cmd related to EDPCH

DEPENDENCIES    The config db information

RETURN VALUE    

SIDE EFFECTS    The mdsp EDPCH shadow buffer will be filled.
===========================================================================*/

extern void e_ul_edpch_cfg_event_handler
( wsub_id_e_type wsub_id);

//#ifdef FEATURE_WCDMA_HSUPA_PSEUDO_FIQ
/*===========================================================================
FUNCTION    EUL_START_EDCH_DATA_PATH_FIQ_HANDLER

DESCRIPTION This function updates the serving grant and HARQ process  at every
            TTI boundary and shall take into account the Absolute Grant message,
            Serving Relative Grant and non-serving Relative Grants that apply 
            to the TTI. UEs configured with an E-DCH transport channel shall
            maintain a Serving Grant and the list of active HARQ processes 
            based on the absolute and relative grant commands decoded on the
            configured E-AGCH and E-RGCH(s).

DEPENDENCIES Called when FIQ is raised by FW

RETURN VALUE None

SIDE EFFECTS serving grant may change and HARQ process may become inactive
===========================================================================*/
extern void eul_start_edch_data_path_fiq_handler(void);
//#endif /* FEATURE_WCDMA_HSUPA_PSEUDO_FIQ */

/*===========================================================================
FUNCTION        eulcfg_set_cfg_cmd_sent_state_en_frm_bdry_evt

DESCRIPTION     This function will set the state indicating that EUL config 
                command was sent to MDSP this frame and will enable the EUL
                frame boundary event.

DEPENDENCIES  

RETURN VALUE

SIDE EFFECTS    
===========================================================================*/
extern void eulcfg_set_cfg_cmd_sent_state_en_frm_bdry_evt(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        eulcfg_get_cfg_cmd_sent_state

DESCRIPTION     This function returns whether EUL config command was sent to 
                FW this frame or not 

DEPENDENCIES    

RETURN VALUE    whether config command for EUL was sent in this frame or not

SIDE EFFECTS    
===========================================================================*/
extern boolean eulcfg_get_cfg_cmd_sent_state(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        eulcfg_reset_cfg_cmd_sent_state_dis_frm_bdry_evt

DESCRIPTION     This function will clear the state for whether the config 
                command has been sent to FW in this frame and disable the frame
                boundary event which called this function.

DEPENDENCIES    

RETURN VALUE

SIDE EFFECTS    
===========================================================================*/
extern void eulcfg_reset_cfg_cmd_sent_state_dis_frm_bdry_evt(wsub_id_e_type wsub_id);


#endif
