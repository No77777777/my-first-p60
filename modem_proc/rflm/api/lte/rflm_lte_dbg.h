/*!
  @file
  rflm_lte_dbg.h

  @brief
  RFLM LTE debug command handler
  
  @detail
 
*/

/*===========================================================================

  Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rflm.mpss/2.11/api/lte/rflm_lte_dbg.h#1 $

when         who     what, where, why
----------   ---     ----------------------------------------------------------
2015-07-30   krg     Initial version created

===========================================================================*/

#ifndef RFLM_LTE_DBG_H
#define RFLM_LTE_DBG_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/


/*===========================================================================

 Type Declarations

===========================================================================*/

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  Enumeration of masks pertaining to various LTE debug modes                
*/
typedef enum
{
  RFLM_LTE_CONFIG_RXAGC_DEBUG_MASK   = 0x1,
} rf_lte_config_debug_mode_mask_type;


/*===========================================================================

 Function prototypes

===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

extern 
boolean rflm_lte_dbg_cmd_handler( boolean debug_en, uint32 debug_mask );

extern 
boolean rflm_lte_rxagc_configure_debug( boolean debug_en );

#ifdef __cplusplus
}  // extern "C"
#endif 


#endif /* RFLM_LTE_DBG_H */
