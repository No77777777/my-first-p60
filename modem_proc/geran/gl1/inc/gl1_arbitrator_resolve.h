#ifndef GARB_RESOLVE_H
#define GARB_RESOLVE_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
                   G L 1  A R B I T R A T O R  R E S O L V E

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/inc/gl1_arbitrator_resolve.h#1 $ $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
26/05/15   ab        CR833968 : GL1 COEX - RRBP_HI gets incorrectly de-priortised over Paging CCCH .
27/02/15   ab        CR750511: GSM Timeline reporting for WLAN COEX
11/02/15   cws       CR789401 Revert G high priority to tier_10 except for data + MMS
03/12/14   npt       CR.759467 : Fix compiler warnings
01/12/14   mc        CR.725853 : DSDA G[DDS]+G[MMS] : MCS using floored values for T10, T15, T20 hence T5 adaptation is required
27/11/13   pjr       CR.579214 : COEX : Prioritise PDTCH over TCH during 2 phase access
09/11/13   mc        CR.572746 : COEX : DSDA G+G VBATT power back-offs for TxTx collisions are not correctly applied
15/08/13   sk        CR524039 GBTA bringup changes
29/07/13   sk        CR519663 Partial QBTA bringup changes
13/03/13   mc        CR.459487 : COEX Support - GERAN arbitrator framework
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#ifndef CUSTOMER_H
  #include "customer.h"
#endif
#ifndef COMDEF_H
  #include "comdef.h"
#endif

#include "gl1_arbitrator_interface.h"
#include "gl1_arbitrator_types.h"

#ifdef FEATURE_DUAL_SIM
#if defined(FEATURE_GSM_COEX)
/*===========================================================================

                         DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION      garb_resolve_none

DESCRIPTION   Resolver for NULL conflict

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_none(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_lo_prio

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_lo_prio(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_hi_prio

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_hi_prio(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);


/*===========================================================================

FUNCTION      garb_resolve_lo_prio_dual_data

DESCRIPTION   This function is used resolve conflict in Data + MMS 

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_lo_prio_dual_data(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_hi_prio_dual_data

DESCRIPTION   This function is used resolve conflict in Data + MMS 

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_hi_prio_dual_data(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index,gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_tch_pdtch
 
DESCRIPTION   should resolve high priority for tch except when 
              transfer is in L1_SINGLE_BLOCK_ALLOC 
 
PARAMS
 
RETURN VALUE  None

===========================================================================*/
void garb_resolve_tch_pdtch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================
 
FUNCTION      garb_resolve_pdtch_tch
 
DESCRIPTION   should resolve low priority for pdtch except 
              when transfer is in L1_SINGLE_BLOCK_ALLOC
 
PARAMS
 
RETURN VALUE  None
 
===========================================================================*/
void garb_resolve_pdtch_tch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_pdtch_ccch

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_pdtch_ccch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_pdtch_pdtch
 
DESCRIPTION   should resolve low priority for pdtch except 
              when transfer is in L1_SINGLE_BLOCK_ALLOC
 
PARAMS
 
RETURN VALUE  None
 
===========================================================================*/
void garb_resolve_pdtch_pdtch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);


/*===========================================================================

FUNCTION      garb_resolve_ccch_pdtch

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ccch_pdtch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);
/*===========================================================================

FUNCTION      garb_resolve_ccch_rrbph

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ccch_rrbph(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_ccch_rrbpl

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ccch_rrbpl(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_rrbph_ccch

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_rrbph_ccch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_rrbpl_ccch

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_rrbpl_ccch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_ptcch_ccch

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ptcch_ccch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_ccch_ptcch

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ccch_ptcch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_ptcch_ncellh

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ptcch_ncellh(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_ptcch_ncellh

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ptcch_ncelll(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_pdtch_ncellh

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_pdtch_ncellh(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_pdtch_ncelll

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_pdtch_ncelll(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_ccch_ncellh

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ccch_ncellh(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_rrbph_ncellh

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_rrbph_ncellh(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_rrbph_ncelll

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_rrbph_ncelll(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_rrbpl_ncellh

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_rrbpl_ncellh(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_rrbpl_ncelll

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_rrbpl_ncelll(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_ncellh_ccch

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ncellh_ccch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_ncellh_ncelll

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ncellh_ncelll(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_ncelll_ptcch

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ncelll_ptcch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_ncelll_paach

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ncelll_paach(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_ptcch_ncellh

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ncelll_rrbpl(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_ncelll_ncellh

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ncelll_ncellh(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_ncelll_pdtch

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ncelll_pdtch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_resolve_ncelll_rrbph

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ncelll_rrbph(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id);
#endif /* (FEATURE_GSM_COEX) */
#endif /* FEATURE_DUAL_SIM */


#endif /*GARB_RESOLVE_H*/
