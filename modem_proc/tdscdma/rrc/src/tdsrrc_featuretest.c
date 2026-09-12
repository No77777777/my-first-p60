/*===========================================================================
                        FEATURE FILE 

DESCRIPTION 

This file lists out the features which are 
 - being mainlined in L2/L3 code and no target should disable them
 - being removed from L2/L3 code and no target should enable them
 
If any of the listed features is defined by the target then a compilation 
error will be posted. 

If any listed feature is required to enable/disable then related check need 
to be removed from this file. 

Copyright (c) 2010-2012 Qualcomm Technologies, Incorporated. 
All Rights Reserved.QUALCOMM Proprietary.  Export of this technology or 
software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrc_featuretest.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/13/12   mkh     Fix Multimode build compile issue
06/15/11   mkh     Initial cut after feature cleanup
===========================================================================*/

#include "tdscdma_variation.h"
#include "comdef.h"


/*==========================================================================
     Features Mainlined
==========================================================================*/


#ifndef FEATURE_DL_BLER_QUAL_CTL
#error "FEATURE_DL_BLER_QUAL_CTL can not be undefined."
#endif
#ifndef FEATURE_RRC_STATIC_AMR
#error "FEATURE_RRC_STATIC_AMR can not be undefined."
#endif






  

/*==========================================================================
     Features Removed
==========================================================================*/

/*
#ifdef CSP_NEW_CELL_PARM
#error "CSP_NEW_CELL_PARM can not be defined."
#endif
*/
/* still has impact on TDS code */
#ifdef FEATURE_MMGPS
#error "FEATURE_MMGPS can not be defined."
#endif
/* still has impact on TDS code */
/* still has impact on TDS code */
/* still has impact on TDS code */
/* still has impact on TDS code */
/* still has impact on TDS code */
#ifdef FEATURE_UMTS_UNIFORM_OOS_HANDLING_P2
#error "FEATURE_UMTS_UNIFORM_OOS_HANDLING_P2 can not be defined."
#endif
/* still has impact on TDS code */


/*
#ifdef LLC_NEW
#error "LLC_NEW can not be defined."
#endif
#ifdef PDSCH_SUPPORTED
#error "PDSCH_SUPPORTED can not be defined."
#endif
#ifdef _OSSNOVALUES
#error "_OSSNOVALUES can not be defined."
#endif
*/

/*==========================================================================
     Features renamed and kept disabled
==========================================================================*/

#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA
#error "FEATURE_CGPS_UMTS_CP_TDSCDMA can not be defined."
#endif

#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
#error "FEATURE_TDSCDMA_GPS_CELL_ID_READING can not be defined."
#endif

#ifdef FEATURE_INTERRAT_HANDOVER_G2TD
//#error "FEATURE_INTERRAT_HANDOVER_G2TD can not be defined."
#endif

#ifdef FEATURE_TDSCDMA_REL7_SPECRS
#error "FEATURE_TDSCDMA_REL7_SPECRS can not be defined."
#endif

#if !defined(FEATURE_3GPP_FAST_DORMANCY)
#error "FEATURE_TDSCDMA_3GPP_FAST_DORMANCY can't be supported when FEATURE_3GPP_FAST_DORMANCY is diabled."
#endif

/*==========================================================================
     Features not tested yet. (feature should not be enabled till tested)
==========================================================================*/


