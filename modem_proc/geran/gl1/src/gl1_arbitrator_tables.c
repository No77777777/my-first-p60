/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
                    G L 1  A R B I T R A T O R  T A B L E S

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gl1_arbitrator_tables.c#1 $ 
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
26/05/15   ab        CR833968 : GL1 COEX - RRBP_HI gets incorrectly de-priortised over Paging CCCH .
13/03/15   mc        CR773788: COEX Enhancements : FACCH delay, RACH delay, PS monitor reduction and DTX timeline changes
27/02/15   ab        CR750511: GSM Timeline reporting for WLAN COEX
27/02/15   am        CR646584 WLAN coex
11/02/15   cws      CR789401 Revert G high priority to tier_10 except for data + MM
17/12/14  shr        CR747635   :Unification of micro priority tiers for GERAN
03/12/14   npt       CR.759467 : Fix compiler warnings
01/12/14   mc        CR.725853 : DSDA G[DDS]+G[MMS] : MCS using floored values for T10, T15, T20 hence T5 adaptation is required
02/07/14   mc        CR.687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
27/12/14   pjr       CR.579214 : COEX : Prioritise PDTCH over TCH during 2 phase access
19/02/14   ab        CR.615709 : DSDA: Priortise FACCH over SACCH for quicker L2 Link Establishment or Handover
14/01/14   og        CR589868. T+G DSDA support.
05/02/13   mc        CR.611586 : Corrected missing entry in GL1 arbitrator conflict table
29/11/13   js        CR.574402 : COEX : VBATT : Accurate prediction of N+1 frame Tx power for CS (TCH and SDCCH)
09/11/13   mc        CR.572746 : COEX : DSDA G+G VBATT power back-offs for TxTx collisions are not correctly applied
11/26/13   mc        CR.564924 : Corrections and updates to GL1 arbitrator resolver tables
07/11/13   js        CR 563464 : NCELL acquisition are being prioritized over SACCH TXs
11/10/13   pjr       CR.555300 : Changes to correctly register freqid during monitors  and acq
06/11/13   nm        CR.564924 : Corrections and updates to GL1 arbitrator resolver tables
01/10/13   sk        CR552259 CR sync from 1.0/1.1
23/09/13   sk        2.0 syncup from 1.1
15/08/13   sk        CR524039 GBTA bringup changes
29/07/13   sk        CR519663 Partial QBTA bringup changes
15/05/13   mc        CR.476825 : COEX Support - GERAN arbitrator SW CXM integration - Bring-up fixes
25/03/13   mc        CR.464496 : COEX Support - GERAN arbitrator SW CXM integration
13/03/13   mc        CR.459487 : COEX Support - GERAN arbitrator framework
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"

#ifdef FEATURE_GSM_COEX_SW_CXM

#include "geran_dual_sim.h"
#include "geran_dual_sim_g.h"
#include "gl1_arbitrator_tables.h"

/*===========================================================================

                            LOCAL DEFINES

===========================================================================*/
const uint8 garb_paging_frame[9]     = { 6, 12, 16, 22, 26, 32, 36, 42, 46};
const uint8 garb_ext_paging_frame[9] = {16, 22, 26, 32, 36, 42, 46,  6, 12};

const garb_channel_t garb_map_gl1_hw_channel_type_lut[] = {
   /* GL1_HW_TCH_FS */             GARB_CHAN_ULDL_TCH,
   /* GL1_HW_TCH_EFS */            GARB_CHAN_ULDL_TCH,
   /* GL1_HW_TCH_F96 */            GARB_CHAN_ULDL_TCH,
   /* GL1_HW_TCH_F144 */           GARB_CHAN_ULDL_TCH,
   /* GL1_HW_SACCH */              GARB_CHAN_ULDL_SACCH,
   /* GL1_HW_SDCCH */              GARB_CHAN_ULDL_SDCCH,
   /* GL1_HW_BCCH */               GARB_CHAN_DL_CCCH,
   /* GL1_HW_PCH */                GARB_CHAN_DL_CCCH,
   /* GL1_HW_FAACH */              GARB_CHAN_ULDL_FACCH,
   /* GL1_HW_SCH */                GARB_CHAN_UNKNOWN,
   /* GL1_HW_RACH */               GARB_CHAN_UL_RACH,
   /* GL1_HW_TCH_F96_AND_FACCH */  GARB_CHAN_UNKNOWN,
   /* GL1_HW_TCH_F144_AND_FACCH */ GARB_CHAN_UNKNOWN,
   /* GL1_HW_TCH_HS */             GARB_CHAN_ULDL_TCH,
   /* GL1_HW_FACCH_HS */           GARB_CHAN_ULDL_FACCH,
   /* GL1_HW_TCH_AFS */            GARB_CHAN_ULDL_TCH,
   /* GL1_HW_TCH_AHS */            GARB_CHAN_ULDL_TCH,
   /* GL1_HW_FACCH_AFS */          GARB_CHAN_ULDL_FACCH,
   /* GL1_HW_FACCH_AHS */          GARB_CHAN_ULDL_FACCH,
   /* GL1_HW_TCH_WFS */            GARB_CHAN_ULDL_TCH,
   /* GL1_HW_FACCH_WFS */          GARB_CHAN_ULDL_FACCH,
   /* GL1_HW_PTCCHD */             GARB_CHAN_ULDL_PTCCH,
   /* GL1_HW_PRACH_ACC */          GARB_CHAN_UL_RACH,
   /* GL1_HW_PTCCHU_ACC */         GARB_CHAN_ULDL_PTCCH,
   /* GL1_HW_PDTCHU_CS1 */         GARB_CHAN_ULDL_PDTCH,
   /* GL1_HW_PDTCHU_CS2 */         GARB_CHAN_ULDL_PDTCH,
   /* GL1_HW_PDTCHU_CS3 */         GARB_CHAN_ULDL_PDTCH,
   /* GL1_HW_PDTCHU_CS4 */         GARB_CHAN_ULDL_PDTCH,
   /* GL1_HW_PDTCHD */             GARB_CHAN_ULDL_PDTCH,
   /* GL1_HW_PACCHU */             GARB_CHAN_UL_PACCH,
   /* GL1_HW_PRACH_EXT */          GARB_CHAN_UL_RACH,
   /* GL1_HW_PTCCHU_EXT */         GARB_CHAN_ULDL_PTCCH,
   /* GL1_HW_PACCHU_EXT */         GARB_CHAN_UL_PACCH,
   /* GL1_HW_PBCCH */              GARB_CHAN_DL_CCCH,
   /* GL1_HW_PCCCH */              GARB_CHAN_DL_CCCH,
   /* GL1_HW_PDTCHU_MCS1 */        GARB_CHAN_ULDL_PDTCH,
   /* GL1_HW_PDTCHU_MCS2 */        GARB_CHAN_ULDL_PDTCH,
   /* GL1_HW_PDTCHU_MCS3 */        GARB_CHAN_ULDL_PDTCH,
   /* GL1_HW_PDTCHU_MCS4 */        GARB_CHAN_ULDL_PDTCH,
   /* GL1_HW_PDTCHU_MCS5 */        GARB_CHAN_ULDL_PDTCH,
   /* GL1_HW_PDTCHU_MCS6 */        GARB_CHAN_ULDL_PDTCH,
   /* GL1_HW_PDTCHU_MCS7 */        GARB_CHAN_ULDL_PDTCH,
   /* GL1_HW_PDTCHU_MCS8 */        GARB_CHAN_ULDL_PDTCH,
   /* GL1_HW_PDTCHU_MCS9 */        GARB_CHAN_ULDL_PDTCH,
   /* GL1_HW_RRBP_HI */            GARB_CHAN_UL_RRBP_HI,
   /* GL1_HW_RRBP_LO */            GARB_CHAN_UL_RRBP_LO,
   /* GL1_HW_NCELL_HI  */          GARB_CHAN_NCELL_HI,
   /* GL1_HW_NCELL_LO  */          GARB_CHAN_NCELL_HI,
   /* GL1_HW_NUM_CHANNEL_TYPES */  GARB_CHAN_UNKNOWN
};

#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_GSM_COEX
void (*garb_resolve_conflict_lut[GARB_CHAN_MAX][GARB_CHAN_MAX])(uint8, boolean, uint8, cxm_activity_type*,uint8, gas_id_t) = {
   /* Unknown */
   { garb_resolve_none,          /* Unknown */
     garb_resolve_none,          /* TCH   */
     garb_resolve_none,          /* SACCH */
     garb_resolve_none,          /* SDCHH */
     garb_resolve_none,          /* FACCH */
     garb_resolve_none,          /* PTCCH */
     garb_resolve_none,          /* PDTCH */
     garb_resolve_none,          /* RATSCCH */
     garb_resolve_none,          /* CCCH  */
     garb_resolve_none,          /* RACH  */
     garb_resolve_none,          /* PACCH */
     garb_resolve_none,          /* RRBP HI */
     garb_resolve_none,          /* RRBP LO */
     garb_resolve_none,          /* NCELL HI */
     garb_resolve_none           /* NCELL LO */
   },

   /* TCH */
   { garb_resolve_none,          /* Unknown */
     garb_resolve_none,          /* TCH   */
     garb_resolve_lo_prio,       /* SACCH */
     garb_resolve_lo_prio,       /* SDCHH */
     garb_resolve_lo_prio,       /* FACCH */
     garb_resolve_lo_prio,       /* PTCCH */
     garb_resolve_tch_pdtch,     /* PDTCH */
     garb_resolve_none,          /* RATSCCH */
     garb_resolve_lo_prio,       /* CCCH  */
     garb_resolve_lo_prio,       /* RACH  */
     garb_resolve_lo_prio,       /* PACCH */
     garb_resolve_lo_prio,       /* RRBP HI */
     garb_resolve_lo_prio,       /* RRBP LO */
     garb_resolve_lo_prio,       /* NCELL HI */
     garb_resolve_lo_prio        /* NCELL LO */
   },

   /* SACCH */
   { garb_resolve_none,          /* Unknown */
     garb_resolve_hi_prio,       /* TCH   */
     garb_resolve_none,          /* SACCH */
     garb_resolve_lo_prio,       /* SDCHH */
     garb_resolve_lo_prio,       /* FACCH */
     garb_resolve_hi_prio,       /* PTCCH */
     garb_resolve_hi_prio,       /* PDTCH */
     garb_resolve_none,          /* RATSCCH */
     garb_resolve_hi_prio,       /* CCCH  */
#ifdef FEATURE_GSM_COEX_RACH_DELAY
     garb_resolve_hi_prio,       /* RACH  */
#else
     garb_resolve_lo_prio,       /* RACH  */
#endif
     garb_resolve_hi_prio,       /* PACCH */
     garb_resolve_lo_prio,       /* RRBP HI */
     garb_resolve_lo_prio,       /* RRBP LO */
     garb_resolve_hi_prio,       /* NCELL HI */
     garb_resolve_hi_prio        /* NCELL LO */
   },

   /* SDCCH */
   { garb_resolve_none,          /* Unknown */
     garb_resolve_hi_prio,       /* TCH   */
     garb_resolve_hi_prio,       /* SACCH */
     garb_resolve_none,          /* SDCHH */
     garb_resolve_hi_prio,       /* FACCH */
     garb_resolve_hi_prio,       /* PTCCH */
     garb_resolve_hi_prio,       /* PDTCH */
     garb_resolve_none,          /* RATSCCH */
     garb_resolve_hi_prio,       /* CCCH  */
#ifdef FEATURE_GSM_COEX_RACH_DELAY
     garb_resolve_hi_prio,       /* RACH  */
#else
     garb_resolve_lo_prio,       /* RACH */
#endif
     garb_resolve_hi_prio,       /* PACCH */
     garb_resolve_hi_prio,       /* RRBP HI */
     garb_resolve_hi_prio,       /* RRBP LO */
     garb_resolve_hi_prio,       /* NCELL HI */
     garb_resolve_hi_prio        /* NCELL LO */
   },

   /* FACCH */
   { garb_resolve_none,          /* Unknown */
     garb_resolve_hi_prio,       /* TCH   */
     garb_resolve_hi_prio,       /* SACCH */
     garb_resolve_lo_prio,       /* SDCHH */
     garb_resolve_none,          /* FACCH */
     garb_resolve_hi_prio,       /* PTCCH */
     garb_resolve_hi_prio,       /* PDTCH */
     garb_resolve_none,          /* RATSCCH */
     garb_resolve_hi_prio,       /* CCCH  */
#ifdef FEATURE_GSM_COEX_RACH_DELAY
     garb_resolve_lo_prio,       /* RACH  */
#else
     garb_resolve_hi_prio,       /* RACH  */
#endif
     garb_resolve_hi_prio,       /* PACCH */
     garb_resolve_hi_prio,       /* RRBP HI */
     garb_resolve_hi_prio,       /* RRBP LO */
     garb_resolve_hi_prio,       /* NCELL HI */
     garb_resolve_hi_prio        /* NCELL LO */
   },

   /* PTCCH */
   { garb_resolve_none,          /* Unknown */
     garb_resolve_hi_prio,       /* TCH   */
     garb_resolve_lo_prio,       /* SACCH */
     garb_resolve_lo_prio,       /* SDCHH */
     garb_resolve_lo_prio,       /* FACCH */
     garb_resolve_none,          /* PTCCH */
     garb_resolve_hi_prio_dual_data,       /* PDTCH */
     garb_resolve_none,          /* RATSCCH */
     garb_resolve_ptcch_ccch,    /* CCCH  */
#ifdef FEATURE_GSM_COEX_RACH_DELAY
     garb_resolve_hi_prio,       /* RACH  */
#else
     garb_resolve_lo_prio,       /* RACH  */
#endif
     garb_resolve_hi_prio_dual_data,       /* PACCH */
     garb_resolve_lo_prio_dual_data,       /* RRBP HI */
     garb_resolve_hi_prio_dual_data,       /* RRBP LO */
     garb_resolve_ptcch_ncellh,  /* NCELL HI */
     garb_resolve_ptcch_ncelll   /* NCELL LO */
   },

   /* PDTCH */
   { garb_resolve_none,          /* Unknown */
     garb_resolve_pdtch_tch,     /* TCH   */
     garb_resolve_lo_prio,       /* SACCH */
     garb_resolve_lo_prio,       /* SDCHH */
     garb_resolve_lo_prio,       /* FACCH */
     garb_resolve_lo_prio_dual_data,       /* PTCCH */
     garb_resolve_pdtch_pdtch,   /* PDTCH */
     garb_resolve_none,          /* RATSCCH */
     garb_resolve_pdtch_ccch,    /* CCCH  */
     garb_resolve_lo_prio,       /* RACH  */
     garb_resolve_lo_prio_dual_data,       /* PACCH */
     garb_resolve_lo_prio_dual_data,       /* RRBP HI */
     garb_resolve_lo_prio_dual_data,       /* RRBP LO */
     garb_resolve_pdtch_ncellh,  /* NCELL HI */
     garb_resolve_pdtch_ncelll   /* NCELL LO */
   },

    /* RATSCCH */
   { garb_resolve_none,          /* Unknown */
     garb_resolve_none,          /* TCH   */
     garb_resolve_none,          /* SACCH */
     garb_resolve_none,          /* SDCHH */
     garb_resolve_none,          /* FACCH */
     garb_resolve_none,          /* PTCCH */
     garb_resolve_none,          /* PDTCH */
     garb_resolve_none,          /* RATSCCH */
     garb_resolve_none,          /* CCCH  */
     garb_resolve_none,          /* RACH  */
     garb_resolve_none,          /* PACCH */
     garb_resolve_none,          /* RRBP HI */
     garb_resolve_none,          /* RRBP LO */
     garb_resolve_none,          /* NCELL HI */
     garb_resolve_none           /* NCELL LO */
   },

   /* CCCH */
   { garb_resolve_none,          /* Unknown */
     garb_resolve_hi_prio,       /* TCH   */
     garb_resolve_lo_prio,       /* SACCH */
     garb_resolve_lo_prio,       /* SDCHH */
     garb_resolve_lo_prio,       /* FACCH */
     garb_resolve_ccch_ptcch,    /* PTCCH */
     garb_resolve_ccch_pdtch,    /* PDTCH */
     garb_resolve_none,          /* RATSCCH */
     garb_resolve_none,          /* CCCH  */
#ifdef FEATURE_GSM_COEX_RACH_DELAY
     garb_resolve_hi_prio,       /* RACH  */
#else
     garb_resolve_lo_prio,       /* RACH  */
#endif
     garb_resolve_hi_prio,       /* PACCH */
     garb_resolve_ccch_rrbph,    /* RRBP HI */
     garb_resolve_ccch_rrbpl,    /* RRBP LO */
     garb_resolve_ccch_ncellh,   /* NCELL HI */
     garb_resolve_none           /* NCELL LO */
   },

#ifdef FEATURE_GSM_COEX_RACH_DELAY
   /* RACH */
   { garb_resolve_none,          /* Unknown */
     garb_resolve_hi_prio,       /* TCH   */
     garb_resolve_lo_prio,       /* SACCH */
     garb_resolve_lo_prio,       /* SDCHH */
     garb_resolve_lo_prio,       /* FACCH */
     garb_resolve_lo_prio,       /* PTCCH */
     garb_resolve_hi_prio,       /* PDTCH */
     garb_resolve_none,          /* RATSCCH */
     garb_resolve_lo_prio,       /* CCCH  */
     garb_resolve_none,          /* RACH  */
     garb_resolve_lo_prio,       /* PACCH */
     garb_resolve_lo_prio,       /* RRBP HI */
     garb_resolve_hi_prio,       /* RRBP LO */
     garb_resolve_hi_prio,       /* NCELL HI */
     garb_resolve_hi_prio        /* NCELL LO */
   },
#else
   /* RACH */
   { garb_resolve_none,          /* Unknown */
     garb_resolve_hi_prio,       /* TCH   */
     garb_resolve_hi_prio,       /* SACCH */
     garb_resolve_hi_prio,       /* SDCHH */ 
     garb_resolve_hi_prio,       /* FACCH */
     garb_resolve_hi_prio,       /* PTCCH */
     garb_resolve_hi_prio,       /* PDTCH */
     garb_resolve_none,          /* RATSCCH */
     garb_resolve_hi_prio,       /* CCCH  */
     garb_resolve_none,          /* RACH  */
     garb_resolve_hi_prio,       /* PACCH */
     garb_resolve_hi_prio,       /* RRBP HI */
     garb_resolve_hi_prio,       /* RRBP LO */
     garb_resolve_hi_prio,       /* NCELL HI */
     garb_resolve_hi_prio        /* NCELL LO */
   },
#endif

   /* PACCH */
   { garb_resolve_none,          /* Unknown */
     garb_resolve_hi_prio,       /* TCH   */
     garb_resolve_lo_prio,       /* SACCH */
     garb_resolve_lo_prio,       /* SDCHH */
     garb_resolve_lo_prio,       /* FACCH */
     garb_resolve_hi_prio_dual_data,       /* PTCCH */
     garb_resolve_hi_prio_dual_data,       /* PDTCH */
     garb_resolve_none,          /* RATSCCH */
     garb_resolve_hi_prio,       /* CCCH  */
#ifdef FEATURE_GSM_COEX_RACH_DELAY
     garb_resolve_lo_prio,       /* RACH  */
#else
     garb_resolve_hi_prio,       /* RACH  */
#endif
     garb_resolve_none,          /* PACCH */
     garb_resolve_lo_prio_dual_data,       /* RRBP HI */
     garb_resolve_hi_prio_dual_data,       /* RRBP LO */
     garb_resolve_none,          /* NCELL HI */
     garb_resolve_none           /* NCELL LO */
   },

   /* RRBP HI */
   { garb_resolve_none,          /* Unknown */
     garb_resolve_hi_prio,       /* TCH   */
     garb_resolve_lo_prio,       /* SACCH */
     garb_resolve_lo_prio,       /* SDCHH */
     garb_resolve_lo_prio,       /* FACCH */
     garb_resolve_none,          /* PTCCH */
     garb_resolve_hi_prio_dual_data,       /* PDTCH */
     garb_resolve_none,          /* RATSCCH */
     garb_resolve_rrbph_ccch,    /* CCCH  */
#ifdef FEATURE_GSM_COEX_RACH_DELAY
     garb_resolve_hi_prio,       /* RACH */
#else
     garb_resolve_lo_prio,       /* RACH */
#endif
     garb_resolve_hi_prio_dual_data,       /* PACCH */
     garb_resolve_none,          /* RRBP HI */
     garb_resolve_hi_prio_dual_data,       /* RRBP LO */
     garb_resolve_rrbph_ncellh,  /* NCELL HI */
     garb_resolve_rrbph_ncelll   /* NCELL LO */
   },

   /* RRBP LO */
   { garb_resolve_none,          /* Unknown */
     garb_resolve_hi_prio,       /* TCH   */
     garb_resolve_lo_prio,       /* SACCH */
     garb_resolve_lo_prio,       /* SDCHH */
     garb_resolve_lo_prio,       /* FACCH */
     garb_resolve_hi_prio_dual_data,       /* PTCCH */
     garb_resolve_hi_prio_dual_data,       /* PDTCH */
     garb_resolve_none,          /* RATSCCH */
     garb_resolve_rrbpl_ccch,    /* CCCH  */
     garb_resolve_lo_prio,       /* RACH */
     garb_resolve_lo_prio_dual_data,       /* PACCH */
     garb_resolve_lo_prio_dual_data,       /* RRBP HI */
     garb_resolve_none,          /* RRBP LO */
     garb_resolve_rrbpl_ncellh,  /* NCELL HI */
     garb_resolve_rrbpl_ncelll   /* NCELL LO */
   },

   /* NCELL_HI */
   { garb_resolve_none,          /* Unknown */
     garb_resolve_none,          /* TCH   */
     garb_resolve_none,          /* SACCH */
     garb_resolve_none,          /* SDCHH */
     garb_resolve_none,          /* FACCH */
     garb_resolve_none,          /* PTCCH */
     garb_resolve_none,          /* PDTCH */
     garb_resolve_none,          /* RATSCCH */
     garb_resolve_ncellh_ccch,   /* CCCH  */
     garb_resolve_none,          /* RACH  */
     garb_resolve_none,          /* PACCH */
     garb_resolve_none,          /* RRBP HI */
     garb_resolve_none,          /* RRBP LO */
     garb_resolve_none,          /* NCELL HI */
     garb_resolve_ncellh_ncelll  /* NCELL LO */
   },

   /* NCELL_LO */
   { garb_resolve_none,          /* Unknown */
     garb_resolve_none,          /* TCH   */
     garb_resolve_none,          /* SACCH */
     garb_resolve_none,          /* SDCHH */
     garb_resolve_none,          /* FACCH */
     garb_resolve_ncelll_ptcch,  /* PTCCH */
     garb_resolve_ncelll_pdtch,  /* PDTCH */
     garb_resolve_none,          /* RATSCCH */
     garb_resolve_none,          /* CCCH  */
     garb_resolve_none,          /* RACH */
     garb_resolve_ncelll_paach,  /* PACCH */
     garb_resolve_ncelll_rrbph,  /* RRBP HI */
     garb_resolve_ncelll_rrbpl,  /* RRBP LO */
     garb_resolve_ncelll_ncellh, /* NCELL HI */
     garb_resolve_none           /* NCELL LO */
   },
};
#endif /*  FEATURE_GSM_COEX */
#endif /* FEATURE_DUAL_SIM */


#ifdef FEATURE_GSM_COEX_SW_CXM
/* Channel Type to CXM priority - G+G */
const cxm_activity_type garb_map_channel_to_cxm_activity_gpg_lut[] = {
   /* GARB_CHAN_UNKNOWN */      ACTIVITY_TIER_GPG_LO_PRIO,
   /* GARB_CHAN_ULDL_TCH */     ACTIVITY_TIER_GPG_LO_PRIO,
   /* GARB_CHAN_ULDL_SACCH */   ACTIVITY_TIER_GPG_HI_PRIO,
   /* GARB_CHAN_ULDL_SDCCH */   ACTIVITY_TIER_GPG_HI_PRIO,
   /* GARB_CHAN_ULDL_FACCH */   ACTIVITY_TIER_GPG_HI_PRIO,
   /* GARB_CHAN_ULDL_PTCCH */   ACTIVITY_TIER_GPG_HI_PRIO,
   /* GARB_CHAN_ULDL_PDTCH */   ACTIVITY_TIER_GPG_LO_PRIO,
   /* GARB_CHAN_ULDL_RATSCCH */ ACTIVITY_TIER_GPG_HI_PRIO,
   /* GARB_CHAN_DL_CCCH */      ACTIVITY_TIER_GPG_HI_PRIO,
   /* GARB_CHAN_UL_RACH */      ACTIVITY_TIER_GPG_HI_PRIO,
   /* GARB_CHAN_UL_PACCH */     ACTIVITY_TIER_GPG_HI_PRIO,
   /* GARB_CHAN_UL_RRBP_HI */   ACTIVITY_TIER_GPG_HI_PRIO,
   /* GARB_CHAN_UL_RRBP_LO */   ACTIVITY_TIER_GPG_LO_PRIO,
   /* GARB_CHAN_NCELL_HI */     ACTIVITY_TIER_GPG_HI_PRIO,
   /* GARB_CHAN_NCELL_LO */     ACTIVITY_TIER_GPG_LO_PRIO,
   /* GARB_CHAN_MAX */          ACTIVITY_TIER_GPG_LO_PRIO
};


#ifdef FEATURE_MCS_WWCOEX_UNIFIED_PRIORITY

/* Channel Type to CXM priority - X+G */
const cxm_activity_type garb_map_channel_to_cxm_activity_xpg_lut[] = {
   /* GARB_CHAN_UNKNOWN */      ACTIVITY_TIER_20,
   /* GARB_CHAN_ULDL_TCH */     ACTIVITY_TIER_10,
   /* GARB_CHAN_ULDL_SACCH */   ACTIVITY_TIER_5,
   /* GARB_CHAN_ULDL_SDCCH */   ACTIVITY_TIER_5,
   /* GARB_CHAN_ULDL_FACCH */   ACTIVITY_TIER_5,
   /* GARB_CHAN_ULDL_PTCCH */   ACTIVITY_TIER_5,
   /* GARB_CHAN_ULDL_PDTCH */   ACTIVITY_TIER_10,
   /* GARB_CHAN_ULDL_RATSCCH */ ACTIVITY_TIER_5,
   /* GARB_CHAN_DL_CCCH */      ACTIVITY_TIER_5,
   /* GARB_CHAN_UL_RACH */      ACTIVITY_TIER_5,
   /* GARB_CHAN_UL_PACCH */     ACTIVITY_TIER_5,
   /* GARB_CHAN_UL_RRBP_HI */   ACTIVITY_TIER_5,
   /* GARB_CHAN_UL_RRBP_LO */   ACTIVITY_TIER_5,
   /* GARB_CHAN_NCELL_HI */     ACTIVITY_TIER_10,
   /* GARB_CHAN_NCELL_LO */     ACTIVITY_TIER_20,
   /* GARB_CHAN_MAX */          ACTIVITY_TIER_20
};

#else
const cxm_activity_type garb_map_channel_to_cxm_activity_xpg_lut[] = {
   /* GARB_CHAN_UNKNOWN */      ACTIVITY_TIER_20,
   /* GARB_CHAN_ULDL_TCH */     ACTIVITY_TIER_10,
   /* GARB_CHAN_ULDL_SACCH */   ACTIVITY_TIER_10,
   /* GARB_CHAN_ULDL_SDCCH */   ACTIVITY_TIER_10,
   /* GARB_CHAN_ULDL_FACCH */   ACTIVITY_TIER_10,
   /* GARB_CHAN_ULDL_PTCCH */   ACTIVITY_TIER_10,
   /* GARB_CHAN_ULDL_PDTCH */   ACTIVITY_TIER_10,
   /* GARB_CHAN_ULDL_RATSCCH */ ACTIVITY_TIER_10,
   /* GARB_CHAN_DL_CCCH */      ACTIVITY_TIER_10,
   /* GARB_CHAN_UL_RACH */      ACTIVITY_TIER_10,
   /* GARB_CHAN_UL_PACCH */     ACTIVITY_TIER_10,
   /* GARB_CHAN_UL_RRBP_HI */   ACTIVITY_TIER_10,
   /* GARB_CHAN_UL_RRBP_LO */   ACTIVITY_TIER_10,
   /* GARB_CHAN_NCELL_HI */     ACTIVITY_TIER_10,
   /* GARB_CHAN_NCELL_LO */     ACTIVITY_TIER_20,
   /* GARB_CHAN_MAX */          ACTIVITY_TIER_20
};

#endif

/* Max TIER_10 allocation per frame for UL slots based on Multi-RAT mode */
const uint8 max_ul_tier_10_per_frame[] = {
   /* GARB_MODE_GPG_DSDS */ MDSP_MAX_UL_TS,
   /* GARB_MODE_GPG_DSDA */ MDSP_MAX_UL_TS,
   /* GARB_MODE_XPG_DSDS */ MDSP_MAX_UL_TS,
   /* GARB_MODE_XPG_DSDA */ MAX_XPGDSDA_UL_TIER10_PER_FRAME,
   /* GARB_MODE_CPG_DSDA */ MAX_CPGDSDA_UL_TIER10_PER_FRAME,
   /* GARB_MODE_XPG_DSDA */ MAX_WPGDSDA_UL_TIER10_PER_FRAME,
   /* GARB_MODE_TPG_DSDA */ MAX_TPGDSDA_UL_TIER10_PER_FRAME,
   /* GARB_MODE_LPG_DSDA */ MAX_LPGDSDA_UL_TIER10_PER_FRAME,
#ifdef FEATURE_TRIPLE_SIM
   /* GARB_MODE_TSTS */     MDSP_MAX_UL_TS,
#endif
   /* GARB_MODE_INVALID */  MDSP_MAX_UL_TS
};

/* Max TIER_10 allocation per frame for DL slots based on Multi-RAT mode */
const uint8 max_dl_tier_10_per_frame[] = {
   /* GARB_MODE_GPG_DSDS */ MDSP_MAX_DL_TS,
   /* GARB_MODE_GPG_DSDA */ MDSP_MAX_DL_TS,
   /* GARB_MODE_XPG_DSDS */ MDSP_MAX_DL_TS,
   /* GARB_MODE_XPG_DSDA */ MAX_XPGDSDA_DL_TIER10_PER_FRAME,
   /* GARB_MODE_CPG_DSDA */ MAX_CPGDSDA_DL_TIER10_PER_FRAME,
   /* GARB_MODE_WPG_DSDA */ MAX_WPGDSDA_DL_TIER10_PER_FRAME,
   /* GARB_MODE_TPG_DSDA */ MAX_TPGDSDA_DL_TIER10_PER_FRAME,
   /* GARB_MODE_LPG_DSDA */ MAX_LPGDSDA_DL_TIER10_PER_FRAME,
#ifdef FEATURE_TRIPLE_SIM
   /* GARB_MODE_TSTS */     MDSP_MAX_DL_TS,
#endif
   /* GARB_MODE_INVALID */  MDSP_MAX_DL_TS
};

#endif



#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING
const cxm_slot_prio_e garb_map_channel_to_wlan_priority [] = {
   /* GARB_CHAN_UNKNOWN */      CXM_SLOT_PRIO_LOW,
   /* GARB_CHAN_ULDL_TCH */     CXM_SLOT_PRIO_LOW,
   /* GARB_CHAN_ULDL_SACCH */   CXM_SLOT_PRIO_HIGH,
   /* GARB_CHAN_ULDL_SDCCH */   CXM_SLOT_PRIO_HIGH,
   /* GARB_CHAN_ULDL_FACCH */   CXM_SLOT_PRIO_HIGH,
   /* GARB_CHAN_ULDL_PTCCH */   CXM_SLOT_PRIO_HIGH,
   /* GARB_CHAN_ULDL_PDTCH */   CXM_SLOT_PRIO_LOW,
   /* GARB_CHAN_ULDL_RATSCCH */ CXM_SLOT_PRIO_HIGH,
   /* GARB_CHAN_DL_CCCH */      CXM_SLOT_PRIO_HIGH,
   /* GARB_CHAN_UL_RACH */      CXM_SLOT_PRIO_HIGH,
   /* GARB_CHAN_UL_PACCH */     CXM_SLOT_PRIO_HIGH,
   /* GARB_CHAN_UL_RRBP_HI */   CXM_SLOT_PRIO_HIGH,
   /* GARB_CHAN_UL_RRBP_LO */   CXM_SLOT_PRIO_HIGH,
   /* GARB_CHAN_NCELL_HI */     CXM_SLOT_PRIO_LOW,
   /* GARB_CHAN_NCELL_LO */     CXM_SLOT_PRIO_HIGH,
   /* GARB_CHAN_MAX */          CXM_SLOT_PRIO_LOW
};
#endif /* FEATURE_GSM_GPRS_TIMELINE_REPORTING */

#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_GSM_COEX
/* VBATT : P1 (dBm) + P2 (dBm) = MAX(P1, P2) + LUTx[ABS(P1 - P2)] */
const double LUTx[LUTX_MAX_IDX] = {
   /* ABS(x)(TxP1_dB - TxP2_dB) = 0 Step Size (dB) = */ 3.000,
   /* ABS(x)(TxP1_dB - TxP2_dB) = 1 Step Size (dB) = */ 2.539,
   /* ABS(x)(TxP1_dB - TxP2_dB) = 2 Step Size (dB) = */ 2.124,
   /* ABS(x)(TxP1_dB - TxP2_dB) = 3 Step Size (dB) = */ 1.764,
   /* ABS(x)(TxP1_dB - TxP2_dB) = 4 Step Size (dB) = */ 1.455,
   /* ABS(x)(TxP1_dB - TxP2_dB) = 5 Step Size (dB) = */ 1.193,
   /* ABS(x)(TxP1_dB - TxP2_dB) = 6 Step Size (dB) = */ 0.973,
   /* ABS(x)(TxP1_dB - TxP2_dB) = 7 Step Size (dB) = */ 0.790,
   /* ABS(x)(TxP1_dB - TxP2_dB) = 8 Step Size (dB) = */ 0.639,
   /* ABS(x)(TxP1_dB - TxP2_dB) = 9 Step Size (dB) = */ 0.515
};

/* VBATT : Proportion SUB1/SUB2 backoffs */
const double LUTy[LUTY_MAX_IDX+1][2] = {
   {/* SUB1/SUB2 = 1.00/0.00 SUB2 Backoff(SUB1) = */  0.0000, /* Backoff (SUB2) =*/  0.0000, /* Stepsize (dB) = 0.1114 */},
   {/* SUB1/SUB2 = 0.95/0.05 SUB2 Backoff(SUB1) = */  0.2228, /* Backoff (SUB2) =*/ 13.0103, /* Stepsize (dB) = 0.1174 */},
   {/* SUB1/SUB2 = 0.90/0.10 SUB2 Backoff(SUB1) = */  0.4576, /* Backoff (SUB2) =*/ 10.0000, /* Stepsize (dB) = 0.1241 */},
   {/* SUB1/SUB2 = 0.85/0.15 SUB2 Backoff(SUB1) = */  0.7058, /* Backoff (SUB2) =*/  8.2391, /* Stepsize (dB) = 0.1316 */},
   {/* SUB1/SUB2 = 0.80/0.20 SUB2 Backoff(SUB1) = */  0.9691, /* Backoff (SUB2) =*/  6.9897, /* Stepsize (dB) = 0.1401 */},
   {/* SUB1/SUB2 = 0.75/0.25 SUB2 Backoff(SUB1) = */  1.2494, /* Backoff (SUB2) =*/  6.0206, /* Stepsize (dB) = 0.1498 */},
   {/* SUB1/SUB2 = 0.70/0.30 SUB2 Backoff(SUB1) = */  1.5490, /* Backoff (SUB2) =*/  5.2288, /* Stepsize (dB) = 0.1609 */},
   {/* SUB1/SUB2 = 0.65/0.35 SUB2 Backoff(SUB1) = */  1.8709, /* Backoff (SUB2) =*/  4.5593, /* Stepsize (dB) = 0.1738 */},
   {/* SUB1/SUB2 = 0.60/0.40 SUB2 Backoff(SUB1) = */  2.2185, /* Backoff (SUB2) =*/  3.9794, /* Stepsize (dB) = 0.1889 */},
   {/* SUB1/SUB2 = 0.55/0.45 SUB2 Backoff(SUB1) = */  2.5964, /* Backoff (SUB2) =*/  3.4679, /* Stepsize (dB) = 0.2070 */},
   {/* SUB1/SUB2 = 0.50/0.50 SUB2 Backoff(SUB1) = */  3.0103, /* Backoff (SUB2) =*/  3.0103, /* Stepsize (dB) = 0.2288 */},
   {/* SUB1/SUB2 = 0.45/0.55 SUB2 Backoff(SUB1) = */  3.4679, /* Backoff (SUB2) =*/  2.5964, /* Stepsize (dB) = 0.2558 */},
   {/* SUB1/SUB2 = 0.40/0.60 SUB2 Backoff(SUB1) = */  3.9794, /* Backoff (SUB2) =*/  2.2185, /* Stepsize (dB) = 0.2900 */},
   {/* SUB1/SUB2 = 0.35/0.65 SUB2 Backoff(SUB1) = */  4.5593, /* Backoff (SUB2) =*/  1.8709, /* Stepsize (dB) = 0.3347 */},
   {/* SUB1/SUB2 = 0.30/0.70 SUB2 Backoff(SUB1) = */  5.2288, /* Backoff (SUB2) =*/  1.5490, /* Stepsize (dB) = 0.3959 */},
   {/* SUB1/SUB2 = 0.25/0.75 SUB2 Backoff(SUB1) = */  6.0206, /* Backoff (SUB2) =*/  1.2494, /* Stepsize (dB) = 0.4846 */},
   {/* SUB1/SUB2 = 0.20/0.80 SUB2 Backoff(SUB1) = */  6.9897, /* Backoff (SUB2) =*/  0.9691, /* Stepsize (dB) = 0.6247 */},
   {/* SUB1/SUB2 = 0.15/0.85 SUB2 Backoff(SUB1) = */  8.2391, /* Backoff (SUB2) =*/  0.7058, /* Stepsize (dB) = 0.8805 */},
   {/* SUB1/SUB2 = 0.10/0.90 SUB2 Backoff(SUB1) = */ 10.0000, /* Backoff (SUB2) =*/  0.4576, /* Stepsize (dB) = 1.5051 */},
   {/* SUB1/SUB2 = 0.05/0.95 SUB2 Backoff(SUB1) = */ 13.0103, /* Backoff (SUB2) =*/  0.2228, /* Stepsize (dB) = 1.5051 */},
   {/* SUB1/SUB2 = 0.00/1.00 SUB2 Backoff(SUB1) = */  0.0000, /* Backoff (SUB2) =*/  0.0000, /* Stepsize (dB) = 0.0000 */}
};

/* VBATT : p1p2_dbm_to_backoff_index[] - if either P1 or P2 >= 25dBm total budgeted power may require distributing between SUBs according to priority */
const uint8 p1p2_dbm_to_backoff_index[MAX_DBM-25+1][MAX_DBM-25+1] = {
/*p2(dBm) =              25            26            27            28            29            30            31            32   33 */
/*p1=25dBm*/ { LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX,  17 },
/*p1=26dBm*/ { LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX,  17 },
/*p1=27dBm*/ { LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX,           16,  16 },
/*p1=28dBm*/ { LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX,           15,  15 },
/*p1=29dBm*/ { LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX,           14,           14,  14 },
/*p1=30dBm*/ { LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX,           12,           12,           12,  12 },
/*p1=31dBm*/ { LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX, LUTY_MAX_IDX,            8,           10,           10,           10,  10 },
/*p1=32dBm*/ { LUTY_MAX_IDX, LUTY_MAX_IDX,            5,            6,            7,            7,            7,            7,   7 },
/*p1=33dBm*/ {            3,            4,            4,            4,            4,            4,            4,            4,   4 }
};

#endif /* FEATURE_GSM_COEX */
#endif /* FEATURE_DUAL_SIM */


/* VBATT : TXLEV to dBm */
const uint16 txlev_to_dBm[SYS_BAND_CELL_850+1][MAX_TXLEV+1] = {
/*    TxLev                  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31*/
   /* SYS_BAND_EGSM_900 */ {39, 39, 39, 37, 35, 33, 31, 29, 27, 25, 23, 21, 19, 17, 15, 13, 11,  9,  7,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
   /* SYS_BAND_PGSM_900 */ {39, 39, 39, 37, 35, 33, 31, 29, 27, 25, 23, 21, 19, 17, 15, 13, 11,  9,  7,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
   /* SYS_BAND_PCS_1900 */ {30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10,  8,  6,  4,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
   /* SYS_BAND_PCS_1800 */ {30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10,  8,  6,  4,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
   /* SYS_BAND_CELL_850 */ {39, 39, 39, 37, 35, 33, 31, 29, 27, 25, 23, 21, 19, 17, 15, 13, 11,  9,  7,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }
};
#endif /* FEATURE_GSM_COEX_SW_CXM */