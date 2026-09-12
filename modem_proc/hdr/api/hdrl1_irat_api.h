#ifndef HDRL1_IRAT_API_H
#define HDRL1_IRAT_API_H

/*===========================================================================
                  H D R    L1    IRAT    D E F I N I T I O N

GENERAL DESCRIPTION
      This file contains hdr L1 data structures and API definitions
      exposed to LTE

  Copyright (c) 2011-2015 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/hdr.mpss/6.0/api/hdrl1_irat_api.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when         who     what, where, why
----------   ---     -------------------------------------------------------- 
06/09/2015   tnp     Update feature flag configuration
05/15/2015   vko/vke Added TABASCO to featurization
04/13/2015   mbs     Featurized the changes
03/26/2015   mbs     Created this file.
===========================================================================*/


/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "sys.h"
#include "IxErrno.h"
#include "rfmeas_types.h"

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/


/* The enum defines parameters that will be passed down from
 * source tech to target tech for adhereing to enter-build
 * script-exit call flow for IRAT */
typedef struct {

  rfm_meas_common_param_type *rfm_common_meas_param;

  struct
  {
    rfcom_device_enum_type              rf_device;
     /* Which rf device to make measurement */

    rfcom_device_enum_type              div_device;
     /* rf div device used by RF to build script */

    sys_channel_type                    channel;
      /* channel information - includes band and frequency */
         
    lm_handle_type                      rxlm_handle_hdr_0;
      /* RxLM of HDR 0 */    
  } target;

  struct
  {
    boolean is_gap_aborted;
    /* TRUE if scripts were built but not executed.
       This flag is only checked at the time of calling RF exit */
  } flag;                

} hdrl1_irat_lte_rfm_param_type;


/*==========================================================================

                    FUNCTION PROTOTYPES USED IN THE MODULE

This section contains function prototypes for the functions used in this
module.

==========================================================================*/

/*=========================================================================

FUNCTION     : HDRL1_IRAT_LTE_MEAS_ENTER

DESCRIPTION  : This function initializes the target fields of the RF
               common structure for enter operation and triggers RF
               enter procedure

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Executes enter procedure to be followed by build scripts
               and exit. Always enter-build scripts-exit sequence must
               be followed by source and target RAT for RF state to be
               in coherent state

=========================================================================*/

void hdrl1_irat_lte_meas_enter
(
  hdrl1_irat_lte_rfm_param_type   *rf_params
    /* All the RF parameters needed to trigger enter procedure */
);

/*=========================================================================

FUNCTION     : HDRL1_IRAT_LTE_MEAS_EXIT

DESCRIPTION  : This function initializes the target fields of the RF
               common structure for exit operation and triggers RF
               exit procedure

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Executes exit procedure followed by enter and build
               scripts. Always enter-build scripts-exit sequence must
               be followed by source and target RAT for RF state to be
               in coherent state

=========================================================================*/

void hdrl1_irat_lte_meas_exit
(
  hdrl1_irat_lte_rfm_param_type   *rf_params
    /* All the RF parameters needed to trigger enter procedure */
);


#endif /* HDRL1_IRAT_API_H */
