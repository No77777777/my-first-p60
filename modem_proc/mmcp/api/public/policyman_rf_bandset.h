#ifndef _POLICYMAN_RF_BANDSET_H_
#define _POLICYMAN_RF_BANDSET_H_

/**
  @file policyman_rf_bandset.h

  @brief  
*/
/*
    Copyright (c) 2016 Qualcomm Technologies, Inc.
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

$Header: //components/rel/mmcp.mpss/7.9.0/api/public/policyman_rf_bandset.h#1 $
$DateTime: 2021/06/15 22:40:27 $
$Author: pwbldsvc $
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "mre_set.h"
#include "sys.h"


typedef mre_set_t policyman_rf_bandset_t;


/*=============================================================================
  Functions to create bandsets.
=============================================================================*/


/*-------- policyman_rf_bandset_new --------*/
/**
@brief  Create a new empty RF bandset object.

@return
  Pointer to a reference-counted RF bandset object.  A reference is already made
  on this object to account for the caller's use of it.  When the caller is done
  with the object, it must release its reference by calling ref_cnt_obj_release.
*/
policyman_rf_bandset_t  *policyman_rf_bandset_new(
  void
);


/*-------- policyman_rf_bandset_clone --------*/
/**
@brief  Make a copy of a bandset

@param[in]  pBandSet  Pointer to the bandset to copy

@return
  Pointer to a reference-counted RF bandset object that has the same bands
  as the bandset passed to the function.  It is the caller's responsibility to
  release its reference on this object when done with it.
*/
policyman_rf_bandset_t  *policyman_rf_bandset_clone(
  policyman_rf_bandset_t const *pBandSet
);


/*=============================================================================
  Functions to manipulate bandsets
=============================================================================*/


/*-------- policyman_rf_bandset_add_band_to_set --------*/
/**
@brief  Add a particular band to a bandset

@param[in]  pBandSet  Pointer to the bandset to which the band is to be added
@param[in]  band      The band to add to the set

@return
  TRUE if the band was added to the set
  FALSE if it is not possible to add this band (band out of range)
*/
boolean policyman_rf_bandset_add_band_to_set(
  policyman_rf_bandset_t *pBandSet,
  sys_band_class_e_type   band
);


/*-------- policyman_rf_bandset_remove_band_from_set --------*/
/**
@brief  Remove a particular band from a bandset

@param[in]  pBandSet  Pointer to the bandset from which the band is to be removed
@param[in]  band      The band to remove from the set

@return
  TRUE if the band was removed from the set
  FALSE if it is not possible to remove this band (band out of range)
*/
boolean policyman_rf_bandset_remove_band_from_set(
  policyman_rf_bandset_t *pBandSet,
  sys_band_class_e_type   band
);


/*-------- policyman_rf_bandset_intersect --------*/
/**
@brief  Intersect two bandsets, resulting in a set with only those bands common
        to both

@param[in]      pBandSet1   One bandset to be used in the intersection
@param[in]      pBandSet2   Other bandset to be used in the intersection

@return
  Pointer to a reference-counted RF bandset object that has those bands that are
  in both source bandsets.  It is the caller's responsibility to release its
  reference on this object when done with it.
*/
policyman_rf_bandset_t *policyman_rf_bandset_intersect(
  policyman_rf_bandset_t const *pBandSet1,
  policyman_rf_bandset_t const *pBandSet2
);


/*-------- policyman_rf_bandset_combine --------*/
/**
@brief  Combine two bandsets, resulting in a bandset that contains all bands
        that are in either bandset

@param[in/out]  pBandSet1   One bandset to be used in the combination
@param[in]      pBandSet2   Other bandset to be used in the combination

@return
  Pointer to a reference-counted RF bandset object that has those bands that are
  in one or the other source bandsets.  It is the caller's responsibility to
  release its reference on this object when done with it.
*/
policyman_rf_bandset_t *policyman_rf_bandset_combine(
  policyman_rf_bandset_t const *pBandSet1,
  policyman_rf_bandset_t const *pBandSet2
);


/*=============================================================================
  Functions to test contents of bandsets
=============================================================================*/


/*-------- policyman_rf_bandset_sets_are_equal --------*/
/**
@brief  Test whether two bandsets are equal

@param[in]  pBandSet1   Pointer to one bandset to compare
@param[in]  pBandSet2   Pointer to one bandset to compare

@return
  TRUE if the bandsets are equal, FALSE if not
*/
boolean policyman_rf_bandset_sets_are_equal(
  policyman_rf_bandset_t const *pBandSet1,
  policyman_rf_bandset_t const *pBandSet2
);


/*-------- policyman_rf_bandset_band_is_in_bandset --------*/
/**
@brief  Determine whether a given band is in a bandset

@param[in]  pBandSet  The bandset for the query
@param[in]  band      The band for the query

@return
  TRUE if the band is in the bandset, FALSE otherwise
*/
boolean policyman_rf_bandset_band_is_in_bandset(
  policyman_rf_bandset_t const    *pBandSet,
  sys_band_class_e_type            band
);


/*=============================================================================
  Iteration over bandsets via function callbacks
=============================================================================*/


/*-------- policyman_rf_bandset_iterate_fn_t --------*/
/**
@brief  Function type called by policyman_rf_bandset_iterate() for each band in a bandset

@param[in]  band  A band from the bandset.  SYS_BAND_CLASS_NONE is passed when
                  when the iteration is complete.
@param[in]  pCtx  Pointer to the context passed into policyman_rf_bandset_iterate().

@return
  Function should return TRUE if the iterator should continue, FALSE if it
  should stop.
*/
typedef boolean (*policyman_rf_bandset_iterate_fn_t)(
  sys_band_class_e_type *band,
  void                  *pCtx
);


/*-------- policyman_rf_bandset_iterate --------*/
/**
@brief  Call a function for each band in the bandset.

@param[in]  pBandSet  Pointer to the bandset over which to interate.
@param[in]  pfn       Pointer to a function with the rf_bandset_iterate_fn_t
                      signature.  This will be called once for each band in the
                      set, followed by a call with SYS_BAND_CLASS_NONE to
                      indicate that the iteration has completed.
@param[in]  pCtx      Pointer to caller-defined context to be passed to the
                      function called during iteration.  May be NULL.

@return
  None
*/
void policyman_rf_bandset_iterate(
  policyman_rf_bandset_t const      *pBandSet,
  policyman_rf_bandset_iterate_fn_t  pfn,
  void                              *pCtx
);


/*=============================================================================
  Iteration over bandsets by explicit return of bands from an iterator
=============================================================================*/

typedef mre_set_iterator_t policyman_rf_bandset_iterator_t;


/*-------- policyman_rf_bandset_get_iterator --------*/
/**
@brief  Return an iterator on a bandset to be used by
        policyman_rf_bandset_iterator_next_band().

@param[in]  pBandSet  Pointer to bandset over which to iterate.

@return
  Pointer to an rf_bandset_iterator_t that used in a call to
  rf_bandset_iterator_next_band().  This iterator is a reference-counted object
  with a single reference on it.  The caller must call ref_cnt_obj_release on it
  when done.
*/
policyman_rf_bandset_iterator_t *policyman_rf_bandset_get_iterator(
  policyman_rf_bandset_t const  *pBandSet
);


/*-------- policyman_rf_bandset_iterator_next_band --------*/
/**
@brief  Get the next band from a bandset iterator.

@param[in]  pIterator Pointer to an iterator returned from
            policyman_rf_bandset_get_iterator().

@return
  The next band in the bandset.  SYS_BAND_CLASS_NONE is returned once the
  last band in the set has been returned.
*/
sys_band_class_e_type policyman_rf_bandset_iterator_next_band(
  policyman_rf_bandset_iterator_t *pIterator
);


/*=============================================================================
  Filter a bandset by RATs
=============================================================================*/

/*-----------------------------------------------------------------------------
  Define a type for RAT masks.
-----------------------------------------------------------------------------*/
typedef uint32          rat_mask_t;

#define RAT_MASK_CDMA   ((rat_mask_t) SYS_SYS_MODE_MASK_CDMA)
#define RAT_MASK_HDR    ((rat_mask_t) SYS_SYS_MODE_MASK_HDR)
#define RAT_MASK_GSM    ((rat_mask_t) SYS_SYS_MODE_MASK_GSM)
#define RAT_MASK_WCDMA  ((rat_mask_t) SYS_SYS_MODE_MASK_WCDMA)
#define RAT_MASK_LTE    ((rat_mask_t) SYS_SYS_MODE_MASK_LTE)
#define RAT_MASK_TDS    ((rat_mask_t) SYS_SYS_MODE_MASK_TDS)

#define RAT_MASK_ALL    (   RAT_MASK_CDMA   \
                          | RAT_MASK_HDR    \
                          | RAT_MASK_GSM    \
                          | RAT_MASK_WCDMA  \
                          | RAT_MASK_LTE    \
                          | RAT_MASK_TDS    \
                        )

#define RAT_MASK_CHGW   (   RAT_MASK_CDMA   \
                          | RAT_MASK_HDR    \
                          | RAT_MASK_GSM    \
                          | RAT_MASK_WCDMA  \
                        )

/*-------- policyman_rf_bandset_filter_by_rats --------*/
/**
@brief  Get a bandset that only contains bands for a specific set of RATs

@param[in]  pBandSet  Pointer to the bandset to filter.
@param[in]  rats      A mask using the rf_bandset_t type to specify the
                      RATs whose bands should be returned

@return
  Pointer to a new bandset that contains only those bands corresponding to
  the RATs passed in the RAT mask.
*/
policyman_rf_bandset_t *policyman_rf_bandset_filter_by_rats(
  policyman_rf_bandset_t const *pBandSet,
  rat_mask_t                    rats
);


#ifdef __cplusplus
}
#endif

#endif /* _POLICYMAN_RF_BANDSET_H_ */
