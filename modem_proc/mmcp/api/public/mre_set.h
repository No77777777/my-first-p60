#ifndef _MRE_SET_H_
#define _MRE_SET_H_

/**
  @file mre_set.h

  @brief  APIs to deal with MRE sets.
*/

/*
    Copyright (c) 2013-2017 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/api/public/mre_set.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/


#include "dynarray.h"


typedef struct mre_set_t  mre_set_t;


/*=============================================================================
  Set APIS (a set is a dynamically sized set of items).
=============================================================================*/


typedef boolean (*elem_compare_fn_t)(void const *e1, void const *e2);
typedef boolean (*elem_filter_fn_t)(void const *e1, void const *pCtx);
typedef void    (*elem_dtor_fn_t)(void *e);
typedef boolean (*elem_iter_fn_t)(void *e, void *pCtx);

#define MRE_SET_NO_MAX_SIZE DYN_ARRAY_NO_MAX_SIZE



/*-------- mre_set_new --------*/
/**
@brief  Create a new mre set.

@param[in]  elemSize    Size of each element of the set.
@param[in]  nInitElems  Initial number of elements to allocate.
@param[in]  maxElems    Maximum number elements to which the set is allowed to
                        grow.  Pass MRE_SET_NO_MAX_SIZE to let the set
                        grow without bounds.
@param[in]  pfnCompare  Pointer to a function that compares two set elements,
                        returning TRUE if equal, FALSE if not. If NULL is passed
                        a memcmp() will be done on the elements.
@param[in]  pfnDtor     Pointer to a function to be called for each element of
                        the set when the destructor of the set is called.
                        Can be NULL, in which case nothing is done.

@return
  Pointer to a newly allocated mre set.
*/
mre_set_t * mre_set_new(
  size_t            elemSize,
  size_t            nInitElems,
  size_t            maxElems,
  elem_compare_fn_t pfnCompare,
  destructor_ptr    pfnElemDtor
);


/*-------- mre_set_new_with_type --------*/
/**
@brief  Create a new mre set.

@param[in]  setSize      Size of the set type.
@param[in]  elemSize    Size of each element of the set.
@param[in]  nInitElems  Initial number of elements to allocate.
@param[in]  maxElems    Maximum number elements to which the set is allowed to
                        grow.  Pass MRE_SET_NO_MAX_SIZE to let the set
                        grow without bounds.
@param[in]  pfnCompare  Pointer to a function that compares two set elements,
                        returning TRUE if equal, FALSE if not. If NULL is passed
                        a memcmp() will be done on the elements.
@param[in]  pfnDtor     Pointer to a function to be called for each element of
                        the set when the destructor of the set is called.
                        Can be NULL, in which case nothing is done.

@return
  Pointer to a newly allocated mre set.
*/
mre_set_t * mre_set_new_with_type(
  size_t            setSize,
  size_t            elemSize,
  size_t            nInitElems,
  size_t            maxElems,
  elem_compare_fn_t pfnCompare,
  destructor_ptr    pfnElemDtor
);


/*-------- mre_set_clone --------*/
/**
@brief  Return a copy of a set.

@param[in]  pSet  Pointer to set to clone.

@return
  Pointer to a new set that is identical to the old set.  If the source set
  is a reference-counted set, a reference will have been added to each
  element of the set.
*/
mre_set_t *mre_set_clone(
  mre_set_t const *pSet
);


/*-------- mre_set_refcnt_new --------*/
/**
@brief  Create a new mre set that holds pointers to reference-counted
        objects.

@param[in]  nInitElems  Initial number of elements to allocate.
@param[in]  maxElems    Maximum number elements to which the set is allowed to
                        grow.  Pass MRE_SET_NO_MAX_SIZE to let the set
                        grow without bounds.
@param[in]  pfnCompare  Pointer to a function that compares two set elements,
                        returning TRUE if equal, FALSE if not. If NULL is passed
                        a memcmp() will be done on the elements.

@return
  Pointer to a newly allocated mre set.
*/
mre_set_t * mre_set_refcnt_new(
  size_t            nInitElems,
  size_t            maxElems,
  elem_compare_fn_t pfnCompare
);



/*-------- mre_set_add --------*/
/**
@brief  Add an element to a mre set.

@param[in]  pSet  Pointer to the mre set.
@param[in]  pElem Pointer to the element to add.  NOTE: If you want to put
                  a *pointer* to an element in a set, this must be a pointer
                  to the variable in which that pointer is stored.

@return
  TRUE if the element is now in the set, FALSE if not (i.e. the set was
  not able to grow to accomodate the element).
*/
boolean mre_set_add(
  mre_set_t  *pSet,
  void       *pElem
);


/*-------- mre_set_append --------*/
/**
@brief  Append an element to a mre set.

@param[in]  pSet  Pointer to the mre set.
@param[in]  pElem Pointer to the element to add.  NOTE: If you want to put
                  a *pointer* to an element in a set, this must be a pointer
                  to the variable in which that pointer is stored.

@return
  TRUE if the element is now in the set, FALSE if not (i.e. the set was
  not able to grow to accomodate the element).
*/
boolean mre_set_append(
  mre_set_t  *pSet,
  void       *pElem
);


/*-------- mre_set_replace --------*/
/**
@brief  Replace an element that compares, or add it if not present.

@param

@return

*/
boolean mre_set_replace(
  mre_set_t  *pSet,
  void       *pElem
);


/*-------- mre_set_remove --------*/
/**
@brief  Remove an element from a mre set.

@param[in]  pSet  Pointer to the mre set.
@param[in]  pElem Pointer to an element that should be removed from the
                  set.

@return
  None
*/
void mre_set_remove(
  mre_set_t  *pSet,
  void       *pElem
);


/*-------- mre_set_remove_at_index --------*/
/**
@brief  Remove an element at a given index.

@param[in]  pSet  Pointer to the mre set.
@param[in]  index Index of the item to be removed.

@return
  Whether the index was a valid index for the set.
*/
boolean mre_set_remove_at_index(
  mre_set_t *pSet,
  size_t     index
);


/*-------- mre_set_contains --------*/
/**
@brief  Determine if a mre set contains an element.

@param[in]  pSet  Pointer to the mre set.
@param[in]  pElem Pointer to an element to be found in the set.

@return
  TRUE if the element is in the set, FALSE if not.
*/
boolean mre_set_contains(
  mre_set_t const *pSet,
  void const      *pElem
);


/*-------- mre_set_sets_contain_same_elements --------*/
/**
@brief  Test if two sets contain the same elements (order independent).

@param[in]  pSet1   Pointer to one set to compare.
@param[in]  pSet2   Pointer to other set to compare.

@return
  TRUE if sets have same element size, comparison functions, and contain the
  same elements regardless of ordering.
  FALSE otherwise.
*/
boolean mre_set_sets_contain_same_elements(
  mre_set_t const *pSet1,
  mre_set_t const *pSet2
);


/*-------- mre_set_sets_are_identical --------*/
/**
@brief  Test if two sets contain the same elements in the same order.

@param[in]  pSet1   Pointer to one set to compare.
@param[in]  pSet2   Pointer to other set to compare.

@return
  TRUE if sets have same element size, comparison functions, and contain the
  same elements occurring in the same order.
  FALSE otherwise.
*/
boolean mre_set_sets_are_identical(
  mre_set_t const *pSet1,
  mre_set_t const *pSet2
);


/*-------- mre_set_intersection --------*/
/**
@brief  Intersect two sets, resulting in a set with only those elements common
        to both (based on the comparison function used by the sets)

@param[in]  pSet1   One set to be used in the intersection
@param[in]  pSet2   Other set to be used in the intersection

@return
  Pointer to a set object that has those elements that are in both source sets.
  NULL will be returned if the element sizes or comparison functions differ
  between the sets.
  It is the caller's responsibility to release its reference on this object
  when done with it.
*/
mre_set_t *mre_set_intersection(
  mre_set_t const *pSet1,
  mre_set_t const *pSet2
);


/*-------- mre_set_union --------*/
/**
@brief  Combine two sets, resulting in a set with elements that are in either set.

@param[in]  pSet1   One set to be used in the union
@param[in]  pSet2   Other set to be used in the union

@return
  Pointer to a set object that has those elements that are in either of the
  source sets.  Each element will only be included once (elements are compared
  using the set comparison function).
  NULL will be returned if the element sizes or comparison functions differ
  between the sets.
  It is the caller's responsibility to release its reference on this object
  when done with it.
*/
mre_set_t *mre_set_union(
  mre_set_t const *pSet1,
  mre_set_t const *pSet2
);


/*-------- mre_set_difference --------*/
/**
@brief  Take the difference of two sets, returning a set with elements that are
        in the first set but *not* in the second.

@param[in]  pSourceSet  The set from which elements of pRemoveSet will be removed.
@param[in]  pRemoveSet  The set of elements which should be removed from pSourceSet.

@return
  Pointer to a set object that has those elements that are in the first set
  but *not* in the second.
  It is the caller's responsibility to release its reference on this object
  when done with it.
*/
mre_set_t *mre_set_difference(
  mre_set_t const *pSourceSet,
  mre_set_t const *pRemoveSet
);


/*-------- mre_set_filter --------*/
/**
@brief  Filter a set based on certain criteria

@param[in]  pSourceSet  Pointer to a set that is to be filtered.
@param[in]  pfnFilter   Pointer to a function that returns TRUE if the element
                        is to be added to the result set, FALSE if it is to
                        be filtered out.
@param[in]  pCtx        Pointer to context in which to filter (may be NULL if
                        not necessary).

@return
  A pointer to a new, filtered set.
*/
mre_set_t *mre_set_filter(
  mre_set_t const   *pSourceSet,
  elem_filter_fn_t   pfnFilter,
  void const        *pCtx
);


/*-------- mre_set_find --------*/
/**
@brief  Find an element in the set that meets the criteria defined by a
        comparison function and the data passed to it.

@param[in]  pSet    Pointer to the mre set.
@param[in]  pfnCmp  Pointer to a comparison function.
@param[in]  pData   Pointer to data to be used in comparison function.

@return
  Pointer to the first element meeting the criteria, NULL if there are none.
*/
void  *mre_set_find(
  mre_set_t const   *pSet,
  elem_compare_fn_t  pfnCmp,
  void const        *pData
);


/*-------- mre_set_num_elems --------*/
/**
@brief  Return the number of elements in the set.

@param[in]  pSet  Pointer to the mre set.

@return
  The number of elements that have been added to the set.
*/
size_t mre_set_num_elems(
  mre_set_t const *pSet
);



/*-------- mre_set_is_empty --------*/
/**
@brief  Determine if a set contains elements

@param[in]  pSet  Pointer to the mre set.

@return
  TRUE if the set is empty, FALSE otherwise.
*/
boolean mre_set_is_empty(
  mre_set_t const *pSet
);



/*-------- mre_set_get_buf_ptr --------*/
/**
@brief  Return a pointer to the buffer maintained by the set.

NOTE: You need to know what you are doing to use this - this needs to be cast
to the type of object that you added to the set, and you must ensure thread
safety of access.

@param[in]  pSet  Pointer to the mre set.

@return
  Pointer to the buffer maintained by the set.
*/
void  * mre_set_get_buf_ptr(
  mre_set_t const *pSet
);



/*-------- mre_set_iterate --------*/
/**
@brief  Call a specified function for each element of the set.

The iteration function to be called takes two arguments:
- A pointer to the element for which the function is being called.
- A pointer to whatever data structure the caller wants to pass to the function.

If return value of the iteration function called on an element is TRUE, the
iteration will continue with the next element until there are no more elements.
If it is FALSE, the iteration will stop.

@param[in]  pSet    Pointer to the mre set.
@param[in]  pfnIter Pointer to the iteration function to be called.
@param[in]  pData1  Pointer to data to be passed as the second parameter
                    to the iteration function.
@param[in]  pData2  Pointer to data to be passed as the third parameter
                    to the iteration function.

@return
  TRUE if pfnIter returned TRUE for every element, FALSE otherwise.
*/
boolean mre_set_iterate(
  mre_set_t const *pSet,
  elem_iter_fn_t   pfnIter,
  void            *pCtx
);


typedef struct mre_set_iterator_t mre_set_iterator_t;

/*-------- mre_set_get_iterator --------*/
/**
@brief  Get an iterator over a particular set.

@param[in]  pSet  Set over which to iterate.

@return
  Pointer to a ref-counted iterator to be used in mre_set_iterator_next_band().
  Client should call ref_cnt_obj_release on the iterator when done with it.
*/
mre_set_iterator_t *mre_set_get_iterator(
  mre_set_t const *pSet
);


/*-------- mre_set_iterator_next_element --------*/
/**
@brief  Return a pointer to the next element in a set.

@param[in]  pIterator Pointer to an iterator returned from mre_set_get_iterator().

@return
  Pointer to the next element in the set.  If the return is NULL, the end of
  the set has been reached.
*/
void *mre_set_iterator_next_element(
  mre_set_iterator_t *pIterator
);



/*-------- mre_set_elem_ptr --------*/
/**
@brief  Return a pointer to an element at a specified index.

@param[in]  pSet  Pointer to the mre set.
@param[in]  i     Index into the set.

@return
  Pointer to the element at the given index in the set, NULL if i is greater
  than the number of elements.
*/
void * mre_set_elem_ptr(
  mre_set_t const *pSet,
  size_t           i
);

#endif /* _MRE_SET_H_ */
