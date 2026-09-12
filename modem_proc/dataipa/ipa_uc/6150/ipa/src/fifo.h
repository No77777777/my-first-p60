/*!
  @file
  fifo.h

  @brief
  Declares the fifo module

*/
/*===========================================================================

  Copyright (c) 2017 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

  $Header: //components/rel/dataipa.mpss/3.2/ipa_uc/6150/ipa/src/fifo.h#1 $
==============================================================================*/
#ifndef FIFO_H_
#define FIFO_H_

#include "global_defs.h"

/**
 * @Struct    sfifo
 *
 * @brief   Simplify fifo optimized for code size under the constraints: DEPTH must be relatively small (up to 8 bits). 
 */
 typedef struct sfifo
 {
   uint32_t  *fifo_base_addr; /**< Address of the fifo location */
   uint8_t   ri;              /**< The read Index */
   uint8_t   wi;              /**< The write Index */
   uint8_t   count;           /**< Number of elements in fifo */
   uint8_t   element_size;    /**< Size of each element */
   uint8_t   fifo_size;       /**< Fifo size in elements */
 }sfifo;

/**
 * @brief   Initializes a given fifo type
 *  
 * @param   fifo_obj  Pointer to the fifo object 
 * @param   elem_size Size of each fifo element 
 * @param   fifo_size Size of fifo 
 * @param   base_addr Address of the fifo location
 *  
 */
void fifo_init(sfifo *fifo_obj,uint8_t elem_size, uint8_t fifo_size,void*base_addr);

/**
 * @brief   Pushes a fifo element in to the fifo
 *  
 * @param   fifo_obj  Pointer to the fifo object 
 * @param   new_elem  New Element to be pushed in to the 
 *                    fifo
 *  
 * @return  TRUE  : Success, FALSE : Failure 
 *  
*/
boolean fifo_push_element(sfifo *fifo_obj, void *new_elem);

/**
 * @brief   Pops a fifo element from the fifo
 *  
 * @param   fifo_obj  Pointer to the fifo object 
 * @param   peek      Indicates whether to pop or just peek 
 * 
 * @return  NULL if no element to pop/peek, else pointer to the 
 *          element 
 *           
 *  
*/
void * fifo_pop_element(sfifo *fifo_obj, boolean peek);
#endif /* FIFO_H_ */
