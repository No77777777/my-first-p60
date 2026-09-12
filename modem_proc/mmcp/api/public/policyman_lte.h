#ifndef _POLICYMAN_LTE_H_
#define _POLICYMAN_LTE_H_

/**
  @file policyman_lte.h

  @brief Interface to Policy Manager for LTE RRC APIs
*/

/*
    Copyright (c) 2018 QUALCOMM Technologies Incorporated.
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

  $Header: //components/rel/mmcp.mpss/7.9.0/api/public/policyman_lte.h#1 $
  $DateTime: 2021/06/15 22:40:27 $
  $Author: pwbldsvc $
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "comdef.h"
#include <sys/types.h>
#include "sys.h"
#include "lte_rrc_ext_api.h"

#ifdef __cplusplus
}
#endif


/*-------- policyman_get_lte_capabilities_for_plmn --------*/
/**
@brief  Retrieve the LTE capabilities for the specified plmn.

@param[in]  subsId     Subscription ID for the PLMN
@param[in]  pPlmn     PLMN to retrieve the CA Band Combos for.
@param[in]  pBandCombos   Pointer to memory into which the CA band combo 
                            string is to be copied.
@param[in]  bandCombosLen   Size of the memory pointed to by pBandCombos, in 
                            bytes.
@param[out] pCaComboLen   The number of bytes (including the NUL terminator) in 
                            the string for this PLMN. This can be compared to 
                            bandCombosLen to determine if truncation occurred.
                            This value will be 0 if the NULL string is being returned.
@param[out] pFeatureMask   The mask of features that are supported for the CA
                            band combo string.

@return
  - POLICYMAN_STATUS_SUCCESS if no errors.
  - POLICYMAN_STATUS_ERR_INVALID_ARGS
  - POLICYMAN_STATUS_ERR_NOT_READY if Policyman is suspended or still initializing
*/
policyman_status_t policyman_get_lte_capabilities_for_plmn(
  sys_modem_as_id_e_type  subsId,
  sys_plmn_id_s_type     *pPlmn,
  char                   *pBandCombos,
  size_t                  bandCombosLen,
  size_t                 *pCaComboLen,
  uint64                 *pFeatureMask
);


#endif /* _POLICYMAN_LTE_H_ */

