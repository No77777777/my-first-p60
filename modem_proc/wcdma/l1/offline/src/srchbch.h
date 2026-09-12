#ifndef SRCHBCH_H
#define SRCHBCH_H
/*===========================================================================

   S R C H    B C H    S T A T E   R E L A T E D   D E C L A R A T I O N S

DESCRIPTION

EXTERNALIZED FUNCTIONS


INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


REFERENCES
  CDMA Mobile Station ASIC Specification Sheet

Copyright (c) 2001 - 2013 by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/srchbch.h_v   1.1   11 Jul 2002 21:28:06   halbhavi  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/srchbch.h#3 $ $DateTime: 2020/01/22 08:56:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/20/15   jd      Global re-org for W + W
01/08/15   da      Searcher changes to support FR23776 : LTE to WCDMA CSFB delay optimizations
07/01/14   amj     Clean up old ASET update code.
09/12/13   jd      Searcher redesign
02/20/13   hk      Removed FEATURE_INTER_SIB_SLEEP
05/03/10   sv      Fixed compiler warnings.
02/06/10   sv      Mainlined feature FEATURE_WCDMA_OFFLINE_SEARCHER.
05/08/09   sup     Changing the includes for the renaming of header files  dec.h, enci.h, l1m.h, srch.h, srchi.h, 
                   srchlog.h to wdec.h, wenci.h, wl1m_v.h, wsrch.h, wsrchi.h, wsrchlog.h
04/29/09   sq      Removed compiler warnings
04/22/09   stk     Compiler warning fixes	
05/26/06   kps     Added function prototype get_rx_agc_cache() to return a cached 
                   value of the Rx AGC.
02/13/06   ub      Added function prototype for evaluating S criteria of target
                   inter ngbr cell - srchbch_cell_select_eval_inter()
11/24/05  nd/gv    Modified prototype of srchbch_cell_select_eval() to use it
                   for W to W BPLMN search
08/30/05   sh      Added proto srchbch_cell_select_eval().
04/05/05   gv      Mainlined the feature FEATURE_L1_CELL_SELECTION and 
                   FEATURE_L1_CELL_RESELECTION.Also added a macro for
                   Cell Selection time out.
07/11/02   sh      Added srchbch_declare_reacq_status().
04/29/02   sh      Added srchbch substates for cell reselection.
03/27/02   sh      Created file. Moved declarations from srchbch.c to here.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "wcdma_variation.h"
#include "comdef.h"
#include "wsrchi.h"
#include "srchcmd.h"
#include "srchset.h"
#include "l1rrcif.h"
#include "wl1m.h"

typedef struct
{
  boolean srchbch_nset_info_rxed;
}srchbch_ext_cntrl_params_type;

extern srchbch_ext_cntrl_params_type srchbch_ext_cntrl_params[WCDMA_NUM_SUBS];

typedef struct
{
  uint8 srchbch_early_scrit_fail_cnt;
  /* Initialize static rx_agc to an Invalid value */
  int16 rx_agc;
}srchbch_int_cntrl_params_type;
/*--------------------------------------------------------------------------
                          BCH SEARCH SUBSTATES

--------------------------------------------------------------------------*/


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION SRCHBCH_ASET_SRCH_DONE

DESCRIPTION
  This function processes HPQ list search Search dump during BCH. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void srchbch_aset_srch_done(srch_done_cmd_type *cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHBCH_NSET_SRCH_DONE

DESCRIPTION
  This function processes LPQ neighbor list search Search dump during BCH. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void srchbch_nset_srch_done(srch_done_cmd_type *cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHBCH_NASTT_DONE

DESCRIPTION
  This function processes NASTT Search dump during BCH. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void srchbch_step1_done(srchsched_step1_nastt_status_struct_type *step1_nastt_status, wsub_id_e_type wsub_id);
void srchbch_nastt_done (srchsched_step1_nastt_status_struct_type* step1_nastt_status, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHBCH_CELL_SELECT_EVAL

DESCRIPTION
  This function evaluates S criteria of serving or target intra frequency 
  neighbor cell if reselection is in progress

DEPENDENCIES
  None.

RETURN VALUE
  TRUE, if S criteria passes, FALSE otherwise
  
SIDE EFFECTS
  None

===========================================================================*/
extern boolean srchbch_cell_select_eval( int16 q_qualmin, int16 q_rxlevmin, srch_cell_struct_type* cell_ptr, int16 max_tx_pwr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION srchbch_int_cntrl_params_init

DESCRIPTION
  This function is used to initialize the srchbch internal variables.


DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None

===========================================================================*/
extern void srchbch_int_cntrl_params_init( wsub_id_e_type wsub_id);

#endif /* SRCHBCH_H */
