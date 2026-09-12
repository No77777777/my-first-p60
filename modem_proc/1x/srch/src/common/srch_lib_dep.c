/*====*====*====*====*====*====*====*========*====*====*====*====*====*====*==*

        S E A R C H   T A S K  --
        S E A R C H   L I B R A R Y   D E P E N D E N C I E S

GENERAL DESCRIPTION
  This file contains the feature dependencies for the search library.
  This file must be updated to reflect the features used when building the
  searcher library.  A compiler error will occur if the currently defined
  feature set differs from that used to build the searcher library.
  In addition this file must always be shipped together with the searcher
  library.

EXTERNALIZED FUNCTIONS
  None

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

  Copyright (c) 2004 - 2015 Qualcomm Technologies, Inc.
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.


*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*=*/


/*=============================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/srch/src/common/srch_lib_dep.c#1 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
01/09/15   srk     Feature cleanup.
05/31/13   srk     Mainlined FEATURE_MODEM_1X_NON_SIG.
02/23/12   srk     Mainlined FEATURE_IS2000_SCH
02/03/12   srk     Removed T_QUASAR
01/23/12   srk     Feature Cleanup
01/13/12   srk     Feature Cleanup: Mainlined Always ON features
01/06/12   ppr     Mainlined AlwaysOn FEATURE_DH feature
01/11/12   srk     Feature Cleanup - Removed Always OFF features
01/11/12   srk     Feature Cleanup.
12/20/11   srk     Feature clean up: Removed FEATURE_IS95B_MDR
12/13/11   srk     Changed all FEATURE_IS2000_REL_A_* to FEATURE_IS2000_REL_A
                   and removed obsolete FEATURE_RELATIVE_DMOD_POS
11/22/11   srk     Feature clean up: Mainlined and Internalized Features
10/31/11   jtm     Feature clean up.
02/16/11   vks     Mainlined FEATURE_SRCH_CLKREGIM_RM.
12/16/10   vks     FEATURE_IS95B_MDR not required and no longer supported.
11/01/10   jtm     Modem 1x SU API changes.
03/03/10   adw     Ensure FEATURE_SRCH_CLKREGIM_RM is enabled.
09/18/09   adw     Mainlined FEATURE_ACQ2 to support dynamic memory allocation.
04/02/09   adw     Categorized included header files.
03/26/09   adw     Remove T_QSC60X5 and T_QSC11x0 featurization.
03/26/09   adw     Remove T_MSM6700 featurization.
03/25/09   adw     Include modem_1x_defs.h for 1x specific modem defines.
02/24/09   adw     Merge from 1h08 branch, based on VU_MODEM_1X_SRCH.12.00.38
02/19/09   adw     Removed FEATURE_TCXOMGR_DIAG dependency.
10/31/08   adw     Removed srchzz_sm.c and srchzz_qpch_offtl dependency on
                   FEATURE_SRCH_ROT_ONLY_1X_FULL_CTRL.
10/20/08   adw     Added FEATURE_SRCH_ROT_ONLY_1X_FULL_CTRL dependency checks.
08/28/08   adw     Cleaned up featurization and seperated feature dependencies.
08/18/08   adw     Added srch_rx featurization dependency checks for ULC.
08/13/08   vks     Added FEATURE_SRCH_HAS_NO_RTL and EATURE_SRCH_HAS_NO_IRM
                   for ULC
07/13/08   vks     Added FEATURE_SRCH_SINGLE_ANTENNA for ULC
06/20/08   mca     Merge from //depot
03/31/08   mca     Changed FEATURE_FULL_TIME_SHDR to FEATURE_FULL_TIME_SHDR_3
03/18/08   mca     Properly featurized FTS
03/18/08   mca     Added clock regime resource manager support
03/13/08   sst     Modify FTS version to be determined at runtime
10/08/07   aps     Featurized out IS95B_MDR requirements for QSC60X5
02/21/07   aps     Featurized out SCH requirements for QSC60X5
02/07/07   aps     Featurized out REL A requirements for QSC60X5
06/29/06   tjc     Mainline FEATURE_HHO_ENHANCEMENTS
08/03/05   ejv     Remove checks for FEATURE_GPS_MODE, FEATURE_GPSONE_PPM, and
                   FEATURE_PLCM_ENHANCEMENTS.
07/14/05   sfm     Mainlined FEATURE_1X_SRCH_USES_ROTATOR_AFC and
                   FEATURE_TCXOMGR
05/31/05   rng     Merged in from SHDR Sandbox.
05/24/05   ejv     Check for FEATURE_TCXOMGR and FEATURE_RELATIVE_DMOD_POS.
05/20/05   ejv     Make sure FEATURE_ACQ2 is on.
03/14/05   ejv     Add deps for new library members.
03/01/05   sst     Commented out check for FEATURE_ACQ2
11/12/04   ejv     Removed check for FEATURE_IS2000_REL_A_CC_BCSMS.
10/27/04   dd      Removed gpsOne feature checks.
10/15/04   ejv     Remove feature checks for QUASAR.
09/30/04   sfm     Removed FEATURE_FINGER_DRIVER
09/23/04   ejv     Remove FEATURE_BROADCAST_SMS
09/13/04   sst     Added !FEATURE_ACQ2
09/08/04   jcm     FEATURE_FINGER_DRIVER check now reports correct requirement
09/03/04   sfm     added FEATURE_FINGER_DRIVER
08/03/04   ejv     Removed IS-95B feature checks.
07/19/04   ejv     Added additional IS-95B checks.  Check for
                   FEATURE_BROADCAST_SMS.
07/11/04   ajn     Added FEATURE_SRM_CHIPX8
06/25/04   ejv     Reverse tense on AFC feature check.
06/25/04   jcm     Mainlined FEATURE_FACTORY_TESTMODE in search library
06/25/04   jcm     Mainlined FEATURE_HDR, SRCH4_AFLT_ENABLED
06/25/04   jcm     Removed Dependency on SRCH_microHpq1HPQResults,
                   FEATURE_SEARCH1 conditional on T_MSM6700
06/23/04   jcm     implementation, first cut

=============================================================================*/


/*=============================================================================

                        INCLUDE FILES FOR MODULE

=============================================================================*/

/* Common */
#include "1x_variation.h"
#include "customer.h"
#include "modem_1x_defs.h"

/* Srch */
#include "srchi.h"
#include "srchi_i.h"


/*===========================================================================

               SEARCHER LIBRARY FEATURE REQUIREMENTS

===========================================================================*/

/* Required by:
     l1m.c
     srch.c
     srchint.c
     srchtri.c
     srch_com.c
     srch_fing_driver.c
     srch_hw.c
     srch_sect.c
     srch_srch4.c
     srchidle_sm.c
     srchzz_is2000_sm.c
     srchzz_qpch_sm.c
     srchzz_sm.c
     srchzz_tl_common.c
     srch_chan.c
     srch_idle_util.c
     srch_sched_sm.c
     srch_slot.c
     srchsc_sm.c
     srchtc_triage.c
*/
#ifndef FEATURE_IS2000_REL_A
  #error Required by search library: FEATURE_IS2000_REL_A
#endif /* !FEATURE_IS2000_REL_A */
