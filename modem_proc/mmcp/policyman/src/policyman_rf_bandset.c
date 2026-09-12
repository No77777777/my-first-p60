/**
  @file policyman_rf_bandset.c

  @brief  RAT and RF band information in SET format
*/

/*
    Copyright (c) 2016,2017 QUALCOMM Technologies Incorporated.
    All Rights Reserved.
    Qualcomm Technologies Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by this
  document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/policyman_rf_bandset.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "atomic_ops.h"
#include "policyman_dbg.h"
#include "policyman_rf_bandset.h"
#include "policyman_rf_bandset_i.h"
#include "policyman_util.h"
#include "rfm.h"
#include "rfm_device_types.h"
#include "rfm_path_types.h"
#include "rcevt.h"
#include "mcs_wrappers.h"  // MCS wrappers for 'critical' rcevt_* functions

STATIC policyman_rf_bandset_t *g_pRfDeviceInfo;


/*-------- policyman_rf_bandset_new --------*/
policyman_rf_bandset_t * policyman_rf_bandset_new(
  void
)
{
  policyman_rf_bandset_t *pRfSet;

  pRfSet = mre_set_new(sizeof(sys_band_class_e_type),
                       20,
                       MRE_SET_NO_MAX_SIZE,
                       NULL,
                       NULL);

  return pRfSet;
}


/*-------- policyman_rf_bandset_clone --------*/
policyman_rf_bandset_t  *policyman_rf_bandset_clone(
  policyman_rf_bandset_t const *pBandSet
)
{
  policyman_rf_bandset_t *pRfSet;

  pRfSet = mre_set_clone(pBandSet);

  return pRfSet;
}


/*=============================================================================
  Functions to manipulate bandsets
=============================================================================*/


/*-------- policyman_rf_bandset_add_band_to_set --------*/
boolean policyman_rf_bandset_add_band_to_set(
  policyman_rf_bandset_t *pBandSet,
  sys_band_class_e_type   band
)
{
  boolean bandAdded;

  bandAdded = mre_set_add(pBandSet, &band);

  return bandAdded;
}


/*-------- policyman_rf_bandset_remove_band_from_set --------*/
boolean policyman_rf_bandset_remove_band_from_set(
  policyman_rf_bandset_t *pBandSet,
  sys_band_class_e_type   band
)
{
  mre_set_remove(pBandSet, &band);

  return TRUE;
}


/*-------- policyman_rf_bandset_intersect --------*/
policyman_rf_bandset_t *policyman_rf_bandset_intersect(
  policyman_rf_bandset_t const *pBandSet1,
  policyman_rf_bandset_t const *pBandSet2
)
{
  policyman_rf_bandset_t *pRfBands;

  pRfBands = mre_set_intersection(pBandSet1, pBandSet2);

  return pRfBands;
}


/*-------- policyman_rf_bandset_combine --------*/
policyman_rf_bandset_t *policyman_rf_bandset_combine(
  policyman_rf_bandset_t const *pBandSet1,
  policyman_rf_bandset_t const *pBandSet2
)
{
  policyman_rf_bandset_t *pRfBands;

  pRfBands = mre_set_union(pBandSet1, pBandSet2);

  return pRfBands;
}


/*=============================================================================
  Functions to test contents of bandsets
=============================================================================*/


/*-------- policyman_rf_bandset_sets_are_equal --------*/
boolean policyman_rf_bandset_sets_are_equal(
  policyman_rf_bandset_t const *pBandSet1,
  policyman_rf_bandset_t const *pBandSet2
)
{
  boolean areEqual;

  areEqual = mre_set_sets_contain_same_elements(pBandSet1, pBandSet2);

  return areEqual;
}


/*-------- policyman_rf_bandset_band_is_in_bandset --------*/
boolean policyman_rf_bandset_band_is_in_bandset(
  policyman_rf_bandset_t const    *pBandSet,
  sys_band_class_e_type            band
)
{
  boolean isPresent;

  isPresent = mre_set_contains(pBandSet, &band);

  return isPresent;
}


/*=============================================================================
  Iteration over bandsets via function callbacks
=============================================================================*/

/*-------- policyman_rf_bandset_iterate --------*/
void policyman_rf_bandset_iterate(
  policyman_rf_bandset_t const      *pBandSet,
  policyman_rf_bandset_iterate_fn_t  pfn,
  void                              *pCtx
)
{
  sys_band_class_e_type done = SYS_BAND_CLASS_NONE;

  if (mre_set_iterate(pBandSet, (elem_iter_fn_t)pfn, pCtx))
  {
    pfn(&done, pCtx);
  }
}


/*=============================================================================
  Iteration over bandsets by explicit return of bands from an iterator
=============================================================================*/
/*-------- policyman_rf_bandset_get_iterator --------*/
policyman_rf_bandset_iterator_t *policyman_rf_bandset_get_iterator(
  policyman_rf_bandset_t const  *pBandSet
)
{
  policyman_rf_bandset_iterator_t *pIterator;

  pIterator = mre_set_get_iterator(pBandSet);

  return pIterator;
}


/*-------- policyman_rf_bandset_iterator_next_band --------*/
sys_band_class_e_type policyman_rf_bandset_iterator_next_band(
  policyman_rf_bandset_iterator_t *pIterator
)
{
  sys_band_class_e_type *pElem;

  pElem = (sys_band_class_e_type *) mre_set_iterator_next_element(pIterator);

  if (pElem == NULL)
  {
    return SYS_BAND_CLASS_NONE;
  }
  else
  {
    return *pElem;
  }
}


/*=============================================================================
  Filter a bandset by RATs
=============================================================================*/

typedef struct
{
  sys_band_class_e_type min;
  sys_band_class_e_type max;
} band_range_t;

// CDMA and HDR share a band range
STATIC band_range_t cdma_band_ranges[] =
{
  {SYS_BAND_BC0, SYS_BAND_BC19},
  {SYS_BAND_CLASS_NONE, SYS_BAND_CLASS_NONE}
};

STATIC band_range_t gsm_band_ranges[] =
{
  {SYS_BAND_GSM_450, SYS_BAND_GSM_PCS_1900},
  {SYS_BAND_CLASS_NONE, SYS_BAND_CLASS_NONE}
};

STATIC band_range_t wcdma_band_ranges[] =
{
  {SYS_BAND_WCDMA_I_IMT_2000, SYS_BAND_WCDMA_XIX_850},
  {SYS_BAND_CLASS_NONE, SYS_BAND_CLASS_NONE}
};

STATIC band_range_t lte_band_ranges[] =
{
  {SYS_BAND_LTE_EUTRAN_BAND1, SYS_BAND_LTE_EUTRAN_BAND250},
  {SYS_BAND_LTE_EUTRAN_BAND46, SYS_BAND_LTE_EUTRAN_BAND46},
  {SYS_BAND_LTE_EUTRAN_BAND46, SYS_BAND_LTE_EUTRAN_BAND48},
  {SYS_BAND_LTE_EUTRAN_BAND71, SYS_BAND_LTE_EUTRAN_BAND71},
  {SYS_BAND_CLASS_NONE, SYS_BAND_CLASS_NONE}
};

STATIC band_range_t tds_band_ranges[] =
{
  {SYS_BAND_TDS_BANDA, SYS_BAND_TDS_BANDF},
  {SYS_BAND_CLASS_NONE, SYS_BAND_CLASS_NONE}
};


/*-------- policyman_rf_bandset_band_in_range --------*/
STATIC boolean policyman_rf_bandset_band_in_range(
  sys_band_class_e_type  band,
  band_range_t          *pRange
)
{
  size_t i;

  for (i = 0; pRange[i].min != SYS_BAND_CLASS_NONE; i++)
  {
    if (pRange[i].min <= band && band <= pRange[i].max)
    {
      return TRUE;
    }
  }

  return FALSE;
}


/*-------- band_is_in_rat_mask --------*/
STATIC boolean band_is_in_rat_mask(
  void const  *pElem,
  void const  *pCtx
)
{
  sys_band_class_e_type band    = *((sys_band_class_e_type *) pElem);
  rat_mask_t            ratMask = (rat_mask_t) pCtx;

  return  (    (ratMask & (RAT_MASK_CDMA|RAT_MASK_HDR))
            && policyman_rf_bandset_band_in_range(band, cdma_band_ranges)
          )
       || (   (ratMask & RAT_MASK_GSM)
           && policyman_rf_bandset_band_in_range(band, gsm_band_ranges)
          )
       || (   (ratMask & RAT_MASK_WCDMA)
           && policyman_rf_bandset_band_in_range(band, wcdma_band_ranges)
          )
       || (   (ratMask & RAT_MASK_LTE)
           && policyman_rf_bandset_band_in_range(band, lte_band_ranges)
          )
       || (   (ratMask & RAT_MASK_TDS)
           && policyman_rf_bandset_band_in_range(band, tds_band_ranges)
          );
}


/*-------- policyman_rf_bandset_filter_by_rats --------*/
policyman_rf_bandset_t *policyman_rf_bandset_filter_by_rats(
  policyman_rf_bandset_t const *pBandSet,
  rat_mask_t                    ratMask
)
{
  policyman_rf_bandset_t *pRfBands;

  if (RAT_MASK_ALL == ratMask)
  {
    pRfBands = policyman_rf_bandset_clone(pBandSet);
  }
  else
  {
    pRfBands = mre_set_filter(pBandSet, band_is_in_rat_mask, (void *) ratMask);
  }

  return pRfBands;
}


STATIC sys_band_mask_e_type policyman_rf_bandset_map_sys_band_to_mask(
  sys_band_class_e_type band
)
{
  sys_band_mask_e_type sband = SYS_BAND_MASK_EMPTY;
  switch(band)
  {
    case SYS_BAND_BC0:
      sband = SYS_BAND_MASK_BC0;
      break;
    case SYS_BAND_BC1:
      sband = SYS_BAND_MASK_BC1;
      break;
    case SYS_BAND_BC3:
      sband = SYS_BAND_MASK_BC3;
      break;
    case SYS_BAND_BC4:
      sband = SYS_BAND_MASK_BC4;
      break;
    case SYS_BAND_BC5:
      sband = SYS_BAND_MASK_BC5;
      break;
    case SYS_BAND_BC6:
      sband = SYS_BAND_MASK_BC6;
      break;
    case SYS_BAND_BC7:
      sband = SYS_BAND_MASK_BC7;
      break;
    case SYS_BAND_BC8:
      sband = SYS_BAND_MASK_BC8;
      break;
    case SYS_BAND_BC9:
      sband = SYS_BAND_MASK_BC9;
      break;
    case SYS_BAND_BC10:
      sband = SYS_BAND_MASK_BC10;
      break;
    case SYS_BAND_BC11:
      sband = SYS_BAND_MASK_BC11;
      break;
    case SYS_BAND_BC12:
      sband = SYS_BAND_MASK_BC12;
      break;
    case SYS_BAND_BC14:
      sband = SYS_BAND_MASK_BC14;
      break;
    case SYS_BAND_BC15:
      sband = SYS_BAND_MASK_BC15;
      break;
    case SYS_BAND_BC16:
      sband = SYS_BAND_MASK_BC16;
      break;
    case SYS_BAND_BC17:
      sband = SYS_BAND_MASK_BC17;
      break;
    case SYS_BAND_BC18:
      sband = SYS_BAND_MASK_BC18;
      break;
    case SYS_BAND_BC19:
      sband = SYS_BAND_MASK_BC19;
      break;

    case SYS_BAND_GSM_450:
      sband = SYS_BAND_MASK_GSM_450;
      break;
    case SYS_BAND_GSM_480:
      sband = SYS_BAND_MASK_GSM_480;
      break;
    case SYS_BAND_GSM_750:
      sband = SYS_BAND_MASK_GSM_750;
      break;
    case SYS_BAND_GSM_850:
      sband = SYS_BAND_MASK_GSM_850;
      break;
    case SYS_BAND_GSM_EGSM_900:
      sband = SYS_BAND_MASK_GSM_EGSM_900;
      break;
    case SYS_BAND_GSM_PGSM_900:
      sband = SYS_BAND_MASK_GSM_PGSM_900;
      break;
    case SYS_BAND_GSM_RGSM_900:
      sband = SYS_BAND_MASK_GSM_RGSM_900;
      break;
    case SYS_BAND_GSM_DCS_1800:
      sband = SYS_BAND_MASK_GSM_DCS_1800;
      break;
    case SYS_BAND_GSM_PCS_1900:
      sband = SYS_BAND_MASK_GSM_PCS_1900;
      break;

    case SYS_BAND_WCDMA_I_IMT_2000:
      sband = SYS_BAND_MASK_WCDMA_I_IMT_2000;
      break;
    case SYS_BAND_WCDMA_II_PCS_1900:
      sband = SYS_BAND_MASK_WCDMA_II_PCS_1900;
      break;
    case SYS_BAND_WCDMA_III_1700:
      sband = SYS_BAND_MASK_WCDMA_III_1700;
      break;
    case SYS_BAND_WCDMA_IV_1700:
      sband = SYS_BAND_MASK_WCDMA_IV_1700;
      break;
    case SYS_BAND_WCDMA_V_850:
      sband = SYS_BAND_MASK_WCDMA_V_850;
      break;
    case SYS_BAND_WCDMA_VI_800:
      sband = SYS_BAND_MASK_WCDMA_VI_800;
      break;
    case SYS_BAND_WCDMA_VII_2600:
      sband = SYS_BAND_MASK_WCDMA_VII_2600;
      break;
    case SYS_BAND_WCDMA_VIII_900:
      sband = SYS_BAND_MASK_WCDMA_VIII_900;
      break;
    case SYS_BAND_WCDMA_IX_1700:
      sband = SYS_BAND_MASK_WCDMA_IX_1700;
      break;
    case SYS_BAND_WCDMA_XI_1500:
      sband = SYS_BAND_MASK_WCDMA_XI_1500;
      break;
    case SYS_BAND_WCDMA_XIX_850:
      sband = SYS_BAND_MASK_WCDMA_XIX_850;
      break;

    case SYS_BAND_TDS_BANDA:
      sband = SYS_BAND_MASK_TDS_BANDA;
      break;
    case SYS_BAND_TDS_BANDB:
      sband = SYS_BAND_MASK_TDS_BANDB;
      break;
    case SYS_BAND_TDS_BANDC:
      sband = SYS_BAND_MASK_TDS_BANDC;
      break;
    case SYS_BAND_TDS_BANDD:
      sband = SYS_BAND_MASK_TDS_BANDD;
      break;
    case SYS_BAND_TDS_BANDE:
      sband = SYS_BAND_MASK_TDS_BANDE;
      break;
    case SYS_BAND_TDS_BANDF:
      sband = SYS_BAND_MASK_TDS_BANDF;
      break;

    default:
      break;
  }

  return sband;
}


STATIC boolean  map_sys_band_to_lte_sband(
  sys_band_class_e_type   band,
  sys_sband_lte_e_type   *pSBand
)
{
  boolean bandWasMapped = TRUE;

  switch (band)
  {
    case SYS_BAND_LTE_EUTRAN_BAND1:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND1;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND2:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND2;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND3:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND3;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND4:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND4;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND5:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND5;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND6:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND6;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND7:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND7;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND8:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND8;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND9:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND9;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND10:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND10;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND11:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND11;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND12:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND12;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND13:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND13;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND14:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND14;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND17:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND17;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND18:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND18;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND19:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND19;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND20:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND20;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND21:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND21;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND23:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND23;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND24:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND24;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND25:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND25;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND26:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND26;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND27:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND27;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND28:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND28;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND29:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND29;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND30:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND30;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND31:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND31;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND32:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND32;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND33:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND33;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND34:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND34;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND35:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND35;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND36:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND36;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND37:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND37;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND38:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND38;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND39:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND39;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND40:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND40;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND41:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND41;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND42:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND42;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND43:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND43;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND46:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND46;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND48:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND48;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND66:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND66;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND71:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND71;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND125:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND125;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND126:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND126;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND127:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND127;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND250:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND250;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND252:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND252;
      break;
    case SYS_BAND_LTE_EUTRAN_BAND255:
      *pSBand = SYS_SBAND_LTE_EUTRAN_BAND255;
      break;

    default:
      bandWasMapped = FALSE;
  }

  return bandWasMapped;
}


/*-------- policyman_rf_bandset_is_valid --------*/
boolean policyman_rf_bandset_is_valid(
  policyman_rf_bandset_t const *pBandSet
)
{
  return    pBandSet != NULL
         && !mre_set_is_empty(pBandSet);
}


/*  This is used so that UTs can alter the number of LTE bands that are supported.
 */
STATIC size_t number_of_supported_lte_bands = LTE_BAND_NUM;


/*-------- band_is_not_lte --------*/
STATIC boolean band_is_not_lte(
  sys_band_class_e_type band
)
{
  return !policyman_rf_bandset_band_in_range(band, lte_band_ranges);
}


/*-------- lte_band_is_supported_by_modem --------*/
STATIC boolean lte_band_is_supported_by_modem(
  sys_band_class_e_type band
)
{
  sys_sband_lte_e_type  sband;

  return     map_sys_band_to_lte_sband(band, &sband)
          && sband < number_of_supported_lte_bands;
}

/*-------- band_is_supported_by_modem_software --------*/
STATIC boolean band_is_supported_by_modem_software(
  void const *pElem,
  void const *pCtx
)
{
  sys_band_class_e_type band = *((sys_band_class_e_type *) pElem);

  return  band_is_not_lte(band) || lte_band_is_supported_by_modem(band);
}


/*-------- ensure_global_bandset --------*/
STATIC void ensure_global_bandset(
  void
)
{
  policyman_rf_bandset_t  *pRawBandset;
  policyman_rf_bandset_t  *pFilteredBandset;

  if (NULL == g_pRfDeviceInfo)
  {
    pRawBandset = (policyman_rf_bandset_t *) rfm_get_supported_rf_bandset();
    // RF API does not addref the returned global

    if (NULL == pRawBandset)
    {
      pFilteredBandset = policyman_rf_bandset_new();
    }
    else
    {
      pFilteredBandset = mre_set_filter( pRawBandset,
                                         band_is_supported_by_modem_software,
                                         NULL );
    }

    // Install the empty/filtered bandset
    if (!atomic_compare_and_set((atomic_word_t *)    &g_pRfDeviceInfo,
                                (atomic_plain_word_t) NULL,
                                (atomic_plain_word_t) pFilteredBandset)
       )
    {
      ref_cnt_obj_release(pFilteredBandset);
    }
  }
}


/*-------- policyman_rf_bandset_init --------*/
void policyman_rf_bandset_init(
  void
)
{
  if (NULL == g_pRfDeviceInfo)
  {
    mcs_rcevt_wait_name("RFM_INIT_COMPLETE");
    ensure_global_bandset();
  }
}


/*-------- policyman_rf_deinit --------*/
void policyman_rf_bandset_deinit(
  void
)
{
  MRE_RELEASE_IF(g_pRfDeviceInfo);
}


/*-------- band_mask_ctx_t --------*/
typedef struct
{
  uint64      bandMask;
  rat_mask_t  ratMask;
} band_mask_ctx_t;


/*-------- update_band_mask --------*/
STATIC  boolean update_band_mask(
  sys_band_class_e_type *pBand,
  void                  *pCtx
)
{
  band_mask_ctx_t *pBmCtx = (band_mask_ctx_t *) pCtx;

  if (band_is_in_rat_mask(pBand, (void *) pBmCtx->ratMask))
  {
    pBmCtx->bandMask |= policyman_rf_bandset_map_sys_band_to_mask(*pBand);
  }

  return TRUE;
}


/*-------- get_band_mask --------*/
STATIC  uint64  get_band_mask(
  rat_mask_t  ratMask
)
{
  band_mask_ctx_t ctx;

  ctx.bandMask  = 0;
  ctx.ratMask   = ratMask;

  ensure_global_bandset();

  if (policyman_rf_bandset_is_valid(g_pRfDeviceInfo))
  {
    ref_cnt_obj_add_ref(g_pRfDeviceInfo);

    policyman_rf_bandset_iterate(g_pRfDeviceInfo, update_band_mask, &ctx);

    ref_cnt_obj_release(g_pRfDeviceInfo);
  }

  return ctx.bandMask;
}



/*-------- rfm_get_aggregated_band_mask --------*/
uint64 rfm_get_aggregated_band_mask(
  void
)
{
  return get_band_mask(RAT_MASK_CHGW);
} /* rfm_get_aggregated_band_mask() */


/*-------- rfm_get_tds_band_mask --------*/
uint64 rfm_get_tds_band_mask(
  rfcom_device_enum_type device
)
{
  return get_band_mask(RAT_MASK_TDS);
} /* rfm_get_tds_band_mask() PM STUB */


/*-------- rfm_get_band_mask --------*/
uint64 rfm_get_band_mask (
  rfcom_device_enum_type device
)
{
  return get_band_mask(RAT_MASK_GSM);
}


/*-------- update_lte_band_mask --------*/
STATIC  boolean update_lte_band_mask(
  sys_band_class_e_type *pBand,
  void                  *pCtx
)
{
  sys_lte_band_mask_e_type *pBandMask =  (sys_lte_band_mask_e_type *) pCtx;

  sys_sband_lte_e_type lte_sband;

  if (map_sys_band_to_lte_sband(*pBand, &lte_sband))
  {
    SYS_LTE_BAND_MASK_ADD_BAND(pBandMask, lte_sband);
  }

  return TRUE;
}


/*-------- rfm_get_lte_band_mask --------*/
sys_lte_band_mask_e_type rfm_get_lte_band_mask(
  void
)
{
  sys_lte_band_mask_e_type mask;

  SYS_LTE_BAND_MASK_CLR_ALL_BANDS(&mask);

  ensure_global_bandset();

  if (policyman_rf_bandset_is_valid(g_pRfDeviceInfo))
  {
    ref_cnt_obj_add_ref(g_pRfDeviceInfo);

    policyman_rf_bandset_iterate(g_pRfDeviceInfo, update_lte_band_mask, &mask);

    ref_cnt_obj_release(g_pRfDeviceInfo);
  }

  return mask;
} /* rfm_get_lte_band_mask() PM STUB */

