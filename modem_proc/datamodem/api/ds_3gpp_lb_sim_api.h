#ifndef LB_SIM_API_H
#define LB_SIM_API_H
/*===========================================================================

                                   L2 SIMULATOR

                  E X T E R N A L   H E A D E R   F I L E


  DESCRIPTION
  This file contains external data declarations and function prototypes to
  interact with LB Simulator.
  
  INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2018 Qualcomm Technologies, Incorporated.  All Rights Reserved.
  QUALCOMM Proprietary.  Export of this technology or software is regulated
  by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/api/ds_3gpp_lb_sim_api.h#1 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/30/18   rs      Initial version
===========================================================================*/


/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "comdef.h"
#include "ds3gpp_ext_msg.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
  Enum to indicate the initial bearer configuration 
---------------------------------------------------------------------------*/
typedef enum
{
  DS_3GPP_LB_MCG_ONLY = 0,     /* UL - 4G and DL - 4G      */
  DS_3GPP_LB_SCG_ONLY       ,  /* UL - 5G and DL - 5G      */
  DS_3GPP_LB_UL_MCG_SPLIT_DL,  /* UL - 4G and DL - 4G + 5G */
  DS_3GPP_LB_UL_SCG_SPLIT_DL,  /* UL - 5G and DL - 4G + 5G */
  DS_3GFPP_LB_INVALID_CONFIG
}ds_3gpp_lb_sim_init_bearer_dir_e_type;

/*---------------------------------------------------------------------------
  Structure that defines all the information needed for DS to enter
  loopback mode
---------------------------------------------------------------------------*/
typedef struct
{
  uint8                                        eps_id;
  ds_3gpp_lb_sim_init_bearer_dir_e_type        initial_direction;  
}ds_3gpp_lb_sim_info_s_type;

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================
FUNCTION      DS_3GPP_LB_SIM_TRIGGER_DS_LOOPBACK

DESCRIPTION
  This function handles the loopback simulator request to put DS in loopback
  mode.
   
PARAMETERS  
 lb_sim_info_ptr  -- Pointer to loopback info structure
 
DEPENDENCIES
  None.

RETURN VALUE 
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void ds_3gpp_lb_sim_trigger_ds_loopback
(
  ds_3gpp_lb_sim_info_s_type                 *lb_sim_info_ptr
);

#endif /* LB_SIM_API_H */
