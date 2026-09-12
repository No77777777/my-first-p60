/**
  @file dynarray_i.h

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

$Header: //components/rel/mmcp.mpss/7.9.0/policyman/inc/dynarray_i.h#1 $
$DateTime: 2021/06/15 22:40:27 $
$Author: pwbldsvc $
*/


#ifndef _DYNARRAY_I_H_
#define _DYNARRAY_I_H_


#include "dynarray.h"


#define DYN_BYTE_ARRAY_BASE                                                              \
  _REF_CNT_OBJ;                                                                          \
  size_t                     curSize;     /* current size (in bytes) of the array */     \
  size_t                     bufSize;     /* number of bytes the buffer can hold */      \
  size_t                     maxSize;     /* maximum size (in bytes) of the array */     \
  dynarray_growth_strategy   pfnStrategy; /* growth strategy */                          \
  destructor_ptr             pfnElemDtor; /* pointer to the destructor for contents of the array */  \
  byte                      *pBuf         /* pointer to buffer */


struct dyn_byte_array_t
{
  DYN_BYTE_ARRAY_BASE;
};


/*-------- dyn_byte_array_ctor --------*/
void  dyn_byte_array_ctor(
  dyn_byte_array_t *pArray,
  size_t            initSize,
  size_t            maxSize
);


/*-------- dyn_byte_array_clone_buffer --------*/
void  dyn_byte_array_clone_buffer(
  dyn_byte_array_t *pArray,
  dyn_byte_array_t *pClone
);


#endif

