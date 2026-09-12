#ifndef _TM_SPOOFDETECTOR_API_H
#define _TM_SPOOFDETECTOR_API_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                         TM Spoof Detector API

GENERAL DESCRIPTION
  This file contains TM Spoof Detector interface to C module.

 Copyright (c) 2018 - 2019 Qualcomm Technologies, Inc.
 All Rights Reserved.
 Confidential and Proprietary - Qualcomm Technologies, Inc.

 Export of this technology or software is regulated by the U.S. Government.
 Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                         EDIT HISTORY FOR MODULE

   This section contains comments describing changes made to the module.
   Notice that changes are listed in reverse chronological order.

   $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/inc/tm_spoofDetector_api.h#1 $
   $DateTime: 2019/10/24 01:30:36 $
   $Author: pwbldsvc $

===========================================================================*/

#include "sm_api.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
  * @fn sdm_spoofDetector_init
  * @brief Interface to initialize the spoof detector module.
  *
  * @retval None
  */
  void sdm_spoofDetector_init(void);

/**
 * @fn sdm_HandleInconsistencyReport
 * @brief Interface to handle the inconsistencies reported to SDM.
 *
 * @param [in] Pointer to Inconsistency Data
 *
 * @retval None
 */
void sdm_HandleInconsistencyReport(sm_InconsistencyDataT* pz_InconsistencyData);

/**
 * @fn sdm_GetSpoofReport
 * @brief Interface to get the Spoof Report from SDM
 *
 * @param [in] None
 *
 * @retval None
 */
pdsm_spoof_report sdm_GetSpoofReport();

/**
* @fn sdm_ClearSpoofReport
* @brief Interface to clear the Spoof Report from SDM
*
* @param [in] None
*
* @retval None
*/
void sdm_ClearSpoofReport();

#ifdef __cplusplus
}
#endif

#endif /* _TM_SPOOFDETECTOR_API_H */
