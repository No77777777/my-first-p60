/**
  @file mre_set.c

  @brief  APIs to deal with policyman sets
*/

/*
    Copyright (c) 2013-2017,2020 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/policyman/src/mre_set.c#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#include "mre_namedobj.h"
#include "mre_set_i.h"
#include "mre_util.h"
#include "mre_dbg.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#else
#define QSH_MDUMP_FN_ATTR 
#endif
#include <stringl/stringl.h>



/*-------- mre_set_elem_ptr --------*/
QSH_MDUMP_FN_ATTR_PM void * mre_set_elem_ptr(
  mre_set_t const *pSet,
  size_t     i
)
{
  void  *pElem;

  pElem = dyn_byte_array_get_ptr_to_offset((dyn_byte_array_t *) pSet, i * pSet->elemSize);

  if (NULL == pElem)
  {
    MRE_ERR_FATAL( "Invalid index %d passed when max elements is %d",
                   i,
                   dyn_byte_array_length((dyn_byte_array_t *) pSet)/pSet->elemSize,
                   0 );
  }

  return pElem;
}

/*-------- mre_set_dtor --------*/
static void mre_set_dtor(
  void  *pObj
)
{
  mre_set_t      *pSet        = (mre_set_t *) pObj;
  destructor_ptr  pfnElemDtor = pSet->pfnElemDtor;
  size_t          i;

  if (pfnElemDtor != NULL)
  {
    for (i = 0 ; i < pSet->nElems ; ++i)
    {
      pfnElemDtor(mre_set_elem_ptr(pSet, i));
    }
  }

  dyn_byte_array_dtor(pSet);
}

/*-------- mre_set_ctor --------*/
STATIC void mre_set_ctor(
  mre_set_t        *pSet,
  size_t            elemSize,
  size_t            nInitElems,
  size_t            maxElems,
  elem_compare_fn_t pfnCompare,
  destructor_ptr    pfnElemDtor
)
{
  size_t     nBufElems;
  size_t     maxSize;

  pSet->elemSize      = elemSize;
  pSet->nElems        = 0;
  pSet->pfnCompare    = pfnCompare;
  pSet->pfnElemDtor   = pfnElemDtor;
  pSet->isRefCounted  = FALSE;

  nBufElems = MIN(nInitElems, maxElems);
  maxSize = (MRE_SET_NO_MAX_SIZE == maxElems)? DYN_ARRAY_NO_MAX_SIZE : maxElems * elemSize;
  dyn_byte_array_ctor((dyn_byte_array_t *) pSet, nBufElems * elemSize, maxSize);
}


/*-------- mre_set_new_with_type --------*/
mre_set_t * mre_set_new_with_type(
  size_t            setSize,
  size_t            elemSize,
  size_t            nInitElems,
  size_t            maxElems,
  elem_compare_fn_t pfnCompare,
  destructor_ptr    pfnElemDtor
)
{
  mre_set_t *pSet;
  size_t     nBufElems;
  size_t     maxSize;

  pSet = (mre_set_t *) mre_mem_alloc(setSize);
  ref_cnt_obj_init(pSet, mre_set_dtor);

  mre_set_ctor( pSet, 
                elemSize,
                nInitElems,
                maxElems,
                pfnCompare,
                pfnElemDtor );

  return pSet;
}


/*-------- mre_set_new --------*/
mre_set_t * mre_set_new(
  size_t            elemSize,
  size_t            nInitElems,
  size_t            maxElems,
  elem_compare_fn_t pfnCompare,
  destructor_ptr    pfnElemDtor
)
{
  mre_set_t *pSet;

  pSet = mre_set_new_with_type( sizeof(mre_set_t),
                                elemSize,
                                nInitElems,
                                maxElems,
                                pfnCompare,
                                pfnElemDtor );
  
  return pSet;
}


/*-------- add_ref_to_element --------*/
static boolean add_ref_to_element(
  void  *pElem,
  void  *pCtx
)
{
  ref_cnt_obj_add_ref(*((void **) pElem));
  return TRUE;
}

/*-------- release_ref_to_element --------*/
static boolean release_ref_to_element(
  void  *pElem,
  void  *pCtx
)
{
  ref_cnt_obj_release(*((void **) pElem));
  return TRUE;
}


/*-------- mre_set_clone --------*/
mre_set_t *mre_set_clone(
  mre_set_t const *pSet
)
{
  mre_set_t *pClone;

  pClone = (mre_set_t *) mre_mem_alloc(sizeof(mre_set_t));

  *pClone = *pSet;

  /*  Only one reference on this object
   */
  pClone->_rcobj_.ref_count.value = 1;

  /*  Clone the underlying byte array.
   */
  dyn_byte_array_clone_buffer((dyn_byte_array_t *) pSet, (dyn_byte_array_t *) pClone);

  /*  If the set contains ref-counted elements, add a reference to them.
   */
  if (pClone->isRefCounted)
  {
    mre_set_iterate(pClone, add_ref_to_element, NULL);
  }

  return pClone;
}


/*-------- mre_set_empty_copy_of --------*/
mre_set_t *mre_set_empty_copy_of(
  mre_set_t const *pSet
)
{
  mre_set_t *pClone;

  pClone = mre_set_clone(pSet);
  if (pClone->isRefCounted)
  {    
    mre_set_iterate(pClone, release_ref_to_element, NULL);
  }
  pClone->nElems = 0;
  dyn_byte_array_clear((dyn_byte_array_t *) pClone);

  return pClone;
}


/*-------- release_element --------*/
static void release_element(
  void  *pObj
)
{
  void  *pRefCntObj = *((void **) pObj);

  ref_cnt_obj_release(pRefCntObj);
}


/*-------- mre_set_refcnt_new --------*/
mre_set_t * mre_set_refcnt_new(
  size_t            nInitElems,
  size_t            maxElems,
  elem_compare_fn_t pfnCompare
)
{
  mre_set_t *pSet;

  pSet = mre_set_new(sizeof(void *), nInitElems, maxElems, pfnCompare, release_element);
  pSet->isRefCounted = TRUE;

  return pSet;
}


/*-------- mre_set_find --------*/
void  * mre_set_find(
  mre_set_t const   *pSet,
  elem_compare_fn_t  pfnCmp,
  void const        *pData
)
{
  byte  *pTest;
  byte  *pEnd;

  if (pSet->nElems == 0)
  {
    goto Done;
  }

  pTest = (byte *) mre_set_elem_ptr(pSet, 0);
  pEnd = (byte *) mre_set_elem_ptr(pSet, pSet->nElems - 1);

  for ( ; pTest <= pEnd ; pTest += pSet->elemSize)
  {
    if (pfnCmp(pTest, pData))
    {
      return pTest;
    }
  }

Done:
  return NULL;
}


/*-------- mre_set_find_with_memcmp --------*/
/**
@brief

@param

@return

*/
static void * mre_set_find_with_memcmp(
  mre_set_t const *pSet,
  void const      *pElem
)
{
  byte *pTest;
  byte *pEnd;

  if (pSet->nElems == 0)
  {
    goto Done;
  }

  pTest = (byte *) mre_set_elem_ptr(pSet, 0);
  pEnd = (byte *) mre_set_elem_ptr(pSet, pSet->nElems - 1);

  for ( ; pTest <= pEnd ; pTest += pSet->elemSize)
  {
    if (memcmp(pTest, pElem, pSet->elemSize) == 0)
    {
      return pTest;
    }
  }

Done:
  return NULL;
}


/*-------- mre_set_find_elem --------*/
/**
@brief  Find a specified element in the set.

@param[in]  pSet  Pointer to the policyman set.
@param[in]  pElem Pointer to an element to be compared to those already in the set.

@return
  Pointer to the first (presumably only) element in the set that matches the
  specified element.  NULL if no matching element was found.
*/
static void * mre_set_find_elem(
  mre_set_t const *pSet,
  void const      *pElem
)
{
  return  (pSet->pfnCompare != NULL) ? mre_set_find(pSet, pSet->pfnCompare, pElem)
                                     : mre_set_find_with_memcmp(pSet, pElem);
}


/*-------- mre_set_offset_of --------*/
size_t mre_set_offset_of(
  mre_set_t const *pSet,
  byte            *pLocation
)
{
  return pLocation - dyn_byte_array_get_array_ptr((dyn_byte_array_t *) pSet);
}


/*-------- mre_set_append --------*/
boolean mre_set_append(
  mre_set_t *pSet,
  void      *pElem
)
{
  boolean succeeded;

  succeeded = dyn_byte_array_append_bytes((dyn_byte_array_t *) pSet, pElem, pSet->elemSize);

  if (succeeded)
  {
    ++pSet->nElems;
    if (pSet->isRefCounted)
    {
      ref_cnt_obj_add_ref(*((void **) pElem));
    }
  }

  return succeeded;
}


/*-------- mre_set_add --------*/
boolean mre_set_add(
  mre_set_t *pSet,
  void      *pElem
)
{
  boolean succeeded = TRUE;

  /*  If the element is already present, we have nothing to do.
   */
  if (!mre_set_contains(pSet, pElem))
  {
    succeeded = mre_set_append(pSet, pElem);
  }

  return succeeded;
}


/*-------- mre_set_replace --------*/
boolean mre_set_replace(
  mre_set_t *pSet,
  void      *pElem
)
{
  byte    *pOld;
  size_t   offset;

  /*  See if the item is present in the set.
   */
  pOld = (byte *) mre_set_find_elem(pSet, pElem);

  /*  If not, just add it.
   */
  if (NULL == pOld)
  {
    return mre_set_add(pSet, pElem);
  }

  /*  If this set contains pointers to reference-counted objects,
   *  add a reference to the new element and release the old.
   */
  if (pSet->isRefCounted)
  {
    ref_cnt_obj_add_ref(*((void **) pElem));
    ref_cnt_obj_release(*((void **) pOld));
  }

  /*  Copy the element over the old one.
   */
  offset = mre_set_offset_of(pSet, pOld);
  dyn_byte_array_replace_bytes((dyn_byte_array_t *) pSet, pElem, pSet->elemSize, offset);

  return TRUE;
}


STATIC  void  remove_elem(
  mre_set_t *pSet,
  void      *pRemove
)
{
  size_t  offset;

  if (pSet->isRefCounted)
  {
    ref_cnt_obj_release(*((void **) pRemove));
  }

  offset = mre_set_offset_of(pSet, pRemove);
  dyn_byte_array_delete_bytes((dyn_byte_array_t *) pSet, offset, pSet->elemSize);
  --pSet->nElems;
}


/*-------- mre_set_remove --------*/
void mre_set_remove(
  mre_set_t *pSet,
  void      *pElem
)
{
  byte    *pRemove;

  pRemove = (byte *) mre_set_find_elem(pSet, pElem);
  if (NULL != pRemove)
  {
    remove_elem(pSet, pRemove);
  }
}


/*-------- mre_set_remove_at_index --------*/
boolean mre_set_remove_at_index(
  mre_set_t *pSet,
  size_t     index
)
{
  byte  *pRemove;

  if (index >= mre_set_num_elems(pSet))
  {
    return FALSE;
  }

  pRemove = mre_set_elem_ptr(pSet, index);
  remove_elem(pSet, pRemove);

  return TRUE;
}


/*-------- mre_set_contains --------*/
boolean mre_set_contains(
  mre_set_t const *pSet,
  void const      *pElem
)
{
  return mre_set_find_elem(pSet, pElem) != NULL;
}


/*-----------------------------------------------------------------------------
  Set comparisons
-----------------------------------------------------------------------------*/

/*-------- mre_set_sets_are_comparable --------*/
STATIC boolean mre_set_sets_are_comparable(
  mre_set_t const *pSet1,
  mre_set_t const *pSet2
)
{

  return    pSet1->nElems       == pSet2->nElems
        &&  pSet1->elemSize     == pSet2->elemSize
        &&  pSet1->isRefCounted == pSet2->isRefCounted
        &&  pSet1->pfnCompare   == pSet2->pfnCompare
        ;
}


/*-------- mre_set_sets_contain_same_elements --------*/
boolean mre_set_sets_contain_same_elements(
  mre_set_t const *pSet1,
  mre_set_t const *pSet2
)
{
  size_t  index;

  if (!mre_set_sets_are_comparable(pSet1, pSet2))
  {
    return FALSE;
  }

  /*  Check that each element of set 1 is contained in set 2
   */
  for (index = 0 ; index < pSet1->nElems ; ++index)
  {
    void  *pElem;

    pElem = mre_set_elem_ptr(pSet1, index);
    if (!mre_set_contains(pSet2, pElem))
    {
      return FALSE;
    }
  }

  return TRUE;
}


/*-------- mre_set_sets_are_identical --------*/
boolean mre_set_sets_are_identical(
  mre_set_t const *pSet1,
  mre_set_t const *pSet2
)
{
  size_t            index;
  elem_compare_fn_t pfnCompare;

  if (!mre_set_sets_are_comparable(pSet1, pSet2))
  {
    return FALSE;
  }

  /*  Check that each element of set 1 is equal to the element
   *  in the corresponding position of set 2.
   */
  pfnCompare = pSet1->pfnCompare;
  for (index = 0 ; index < pSet1->nElems ; ++index)
  {
    void  *pElem1;
    void  *pElem2;

    pElem1 = mre_set_elem_ptr(pSet1, index);
    pElem2 = mre_set_elem_ptr(pSet2, index);
    if (!pfnCompare(pElem1, pElem2))
    {
      return FALSE;
    }
  }

  return TRUE;
}


/*-----------------------------------------------------------------------------
  Set intersection
-----------------------------------------------------------------------------*/

typedef struct
{
  mre_set_t const *pSet;
  mre_set_t       *pResult;
} intersect_ctx_t;


/*-------- do_intersection --------*/
STATIC boolean do_intersection(
  void  *pElem,
  void  *pCtx
)
{
  intersect_ctx_t *pInterCtx = (intersect_ctx_t *) pCtx;
  
  if (mre_set_contains(pInterCtx->pSet, pElem))
  {
    mre_set_add(pInterCtx->pResult, pElem);
  }

  return TRUE;
}


/*-------- mre_set_intersection --------*/
mre_set_t *mre_set_intersection(
  mre_set_t const *pSet1,
  mre_set_t const *pSet2
)
{
  intersect_ctx_t ctx;

  ctx.pSet    = pSet2;
  ctx.pResult = mre_set_empty_copy_of(pSet1);

  mre_set_iterate(pSet1, do_intersection, &ctx);

  return ctx.pResult;
}


/*-----------------------------------------------------------------------------
  Set union
-----------------------------------------------------------------------------*/

/*-------- add_element --------*/
STATIC boolean add_element(
  void  *pElem,
  void  *pCtx
)
{
  mre_set_t *pResult = (mre_set_t *) pCtx;

  mre_set_add(pResult, pElem);

  return TRUE;
}


/*-------- mre_set_union --------*/
mre_set_t *mre_set_union(
  mre_set_t const *pSet1,
  mre_set_t const *pSet2
)
{
  mre_set_t *pResult;

  pResult = mre_set_clone(pSet1);

  mre_set_iterate(pSet2, add_element, pResult);

  return pResult;
}


/*-----------------------------------------------------------------------------
  Set difference
-----------------------------------------------------------------------------*/

/*-------- remove_element --------*/
STATIC boolean remove_element(
  void  *pElem,
  void  *pCtx
)
{
  mre_set_t *pResult = (mre_set_t *) pCtx;

  mre_set_remove(pResult, pElem);

  return TRUE;
}


/*-------- mre_set_difference --------*/
mre_set_t *mre_set_difference(
  mre_set_t const *pSet1,
  mre_set_t const *pSet2
)
{
  mre_set_t *pResult;

  pResult = mre_set_clone(pSet1);

  mre_set_iterate(pSet2, remove_element, pResult);

  return pResult;
}


/*-----------------------------------------------------------------------------
  Filter a set
-----------------------------------------------------------------------------*/

/*-------- mre_set_filter_ctx --------*/
typedef struct
{
  mre_set_t        *pDestSet;
  elem_filter_fn_t  pfnFilter;
  void const       *pCtx;
} mre_set_filter_ctx;


/*-------- filter_set --------*/
STATIC boolean filter_set(
  void *e,
  void *pCtx
)
{
  mre_set_filter_ctx *pFilterCtx = (mre_set_filter_ctx *) pCtx;

  if (pFilterCtx->pfnFilter(e, pFilterCtx->pCtx))
  {
    mre_set_add(pFilterCtx->pDestSet, e);
  }

  return TRUE;
}


/*-------- mre_set_filter --------*/
mre_set_t *mre_set_filter(
  mre_set_t const   *pSourceSet,
  elem_filter_fn_t   pfnFilter,
  void const        *pCtx
)
{
  mre_set_filter_ctx   filterCtx;
  
  filterCtx.pDestSet  = mre_set_empty_copy_of(pSourceSet);
  filterCtx.pfnFilter = pfnFilter;
  filterCtx.pCtx      = pCtx;

  mre_set_iterate(pSourceSet, filter_set, &filterCtx);

  return filterCtx.pDestSet;
}




/*-------- mre_set_num_elems --------*/
size_t mre_set_num_elems(
  mre_set_t const *pSet
)
{
  return pSet->nElems;
}


/*-------- mre_set_is_empty --------*/
boolean mre_set_is_empty(
  mre_set_t const *pSet
)
{
  return (pSet == NULL) ? TRUE : mre_set_num_elems(pSet) == 0;
}


/*-------- mre_set_get_buf_ptr --------*/
void  * mre_set_get_buf_ptr(
  mre_set_t const *pSet
)
{
  return dyn_byte_array_get_array_ptr((dyn_byte_array_t *) pSet);
}


/*-------- mre_set_iterate --------*/
QSH_MDUMP_FN_ATTR_PM boolean mre_set_iterate(
  mre_set_t const *pSet,
  elem_iter_fn_t   pfnIter,
  void            *pCtx
)
{
  size_t  i;
  boolean keepGoing;

  if (pSet == NULL)   { return FALSE; }

  for (i = 0, keepGoing = TRUE ; keepGoing && i < pSet->nElems ; ++i)
  {
    void  *pElem;

    pElem = mre_set_elem_ptr(pSet, i);

    keepGoing = pfnIter(pElem, pCtx);
  }

  return keepGoing;
}


/*-------- mre_set_iterator_t --------*/
struct mre_set_iterator_t
{
  _REF_CNT_OBJ;

  mre_set_t const *pSet;
  size_t           currentIndex;
};


/*-------- mre_set_iterator_dtor --------*/
STATIC  void  mre_set_iterator_dtor(
  void  *pObj
)
{
  mre_set_iterator_t *pMe = (mre_set_iterator_t *) pObj;

  MRE_RELEASE_IF(pMe->pSet);
  mre_mem_free(pObj);
}


/*-------- mre_set_get_iterator --------*/
mre_set_iterator_t *mre_set_get_iterator(
  mre_set_t const *pSet
)
{
  mre_set_iterator_t  *pIterator;

  pIterator = (mre_set_iterator_t *) mre_mem_alloc(sizeof(mre_set_iterator_t));
  ref_cnt_obj_init(pIterator, mre_set_iterator_dtor);

  pIterator->pSet = pSet;
  ref_cnt_obj_add_ref((void *) pIterator->pSet);
  pIterator->currentIndex = 0;

  return pIterator;
}


/*-------- mre_set_iterator_next_element --------*/
void *mre_set_iterator_next_element(
  mre_set_iterator_t *pIterator
)
{
  if (pIterator->currentIndex < mre_set_num_elems(pIterator->pSet))
  {
    return mre_set_elem_ptr(pIterator->pSet, pIterator->currentIndex++);
  }
  else
  {
    return NULL;
  }
}


/*-------- mre_set_find_named_object --------*/
mre_named_object_t * mre_set_find_named_object(
  mre_set_t const         *pSet,
  char const              *pName,
  mre_named_object_type_t  objType
)
{
  size_t      i;

  for (i = 0 ; i < pSet->nElems ; ++i)
  {
    mre_named_object_t **ppNamedObj;
    mre_named_object_t  *pNamedObj;

    ppNamedObj = (mre_named_object_t **) mre_set_elem_ptr(pSet, i);
    pNamedObj = (ppNamedObj != NULL)? *ppNamedObj : NULL;

    if (mre_named_object_matches(pNamedObj, pName, objType))
    {
      return pNamedObj;
    }
  }

  return NULL;
}
