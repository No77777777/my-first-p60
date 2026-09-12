/**
  @file mre_set_i.h

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

$Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/mre_set_i.h#1 $
$DateTime: 2021/06/15 22:40:27 $
$Author: pwbldsvc $
*/


#ifndef _MRE_SET_I_H_
#define _MRE_SET_I_H_

#include "mre_set.h"
#include "dynarray_i.h"


#define MRE_SET_BASE                                                              \
  DYN_BYTE_ARRAY_BASE;                                                            \
  size_t             elemSize;     /* size of a set element */                    \
  size_t             nElems;       /* current number of elements in the set */    \
  elem_compare_fn_t  pfnCompare;   /* pointer to compare function */              \
  boolean            isRefCounted; /* does this contain ref-counted objects? */   \
  dyn_byte_array_t  *pArray        /* pointer to dynamic array that is the set */


/*-------- mre_set_t --------*/
struct  mre_set_t
{
  MRE_SET_BASE;
};


#endif

