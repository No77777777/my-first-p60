#ifndef DS_UTILS_MEM_MANAGER_H
#define DS_UTILS_MEM_MANAGER_H

/*==========================================================================*/ 
/*! 
  @file 
  ds_Utils_MemManager.h

  @brief
  This file provides methods to initialize the ds::Utils PS Mem pools during
  powerup. It also exports a wrapper on top of ps_mem_get_buf() API.
  
  @see ps_mem_get_buf()

  Copyright (c) 2008-2009 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/utils/inc/ds_Utils_MemManager.h#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2008-06-30 hm  Created module.

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "ps_mem.h"

/*===========================================================================

                     PUBLIC DATA DEFINITIONS

===========================================================================*/

namespace ds
{
namespace Utils
{
/*! 
  @class MemoryManager 

  This class provides methods to perform Powerup initialization of memory 
  pools used by the DSUtils module. It also exports a wrapper API on top
  of ps_mem_get_buf to get the buffers for DSUtils objects.
*/
class MemoryManager
{
public:

  /*!
    @brief
    This method initializes the memory pools for the DSUtils objects.

    @params     None.
    @see        ps_mem_pool_init()
    @see        The values from DSNetMemConfig.h file are taken as default
                values for mem pool initializations.
    @return     None.

  */
  static void MemPoolInit 
  (
    void
  ) 
  throw();

}; /* class MemoryManager */  
}  /* namespace Utils */
}  /* namespace ds */

#endif /* DS_UTILS_MEM_MANAGER_H */

