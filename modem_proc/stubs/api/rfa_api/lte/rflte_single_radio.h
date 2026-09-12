#ifndef RFLTE_MC_SINGLE_RADIO_H
#define RFLTE_MC_SINGLE_RADIO_H

/*!
   @file
   rflte_mc_single_radio.h

   @brief

*/

/*===========================================================================

Copyright (c) 2010 - 2015 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header  : //source/qcom/qct/modem/rfa3/main/3.0/api/lte/rflte_single_radio.h $

when       who     what, where, why
-------------------------------------------------------------------------------
08/04/15   vc      Initial version
============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#ifdef __cplusplus
extern "C" {
#endif
#include "rfcom.h"
#include "rflte_msg.h"
#include "rflte_ext_mc.h"

/*===========================================================================
                           Data Definitions
===========================================================================*/
/* Added one extra element for TX pair */
#define RFLTE_MC_SINGLE_RADIO_MAX_ELEMENTS 5

#define RFLTE_MC_SINGLE_RADIO_COMBO_MAX RFA_RF_LTE_MAX_CELL_SUPPORTED + 2

#define RFLTE_MC_SINGLE_RADIO_CARRIER_PATH_COMBO_MAX RFA_RF_LTE_MAX_CELL_SUPPORTED+2

#define RFLTE_SINGLE_RADIO_CA_CELL1 0
#define RFLTE_SINGLE_RADIO_CA_CELL2 1

/*
 * rflte_mc_single_radio_type_s
 *   Passed to RF to determine all potential Srx pairing
 *   carriers - potential cells to be considered for SR comb 
 *   Required fields: carrier_idx, channel, bandwidth, path_idx which include tx carriers
 *                           other fields are not considers at this moment.
 */
typedef struct
{
  /* all considered carriers  */
  rfa_rf_config_param_type_s carrier_path_combo[RFLTE_MC_SINGLE_RADIO_CARRIER_PATH_COMBO_MAX];
} rflte_mc_single_radio_type_s;


/*
 * rflte_mc_single_radio_element_type_s
 *   Return back to caller with all potential Srx carrier group and new bw.
 *   With each group of rfa_rf_config_param_type_s:
 *      carriers:    Array of carriers supported by this single_radio device.  value is rflte_mc_cell_idx_type
 *                    use RFLTE_MC_CELL_IDX_INVALID for unused slote
 *      path          indicate if it tx device or rx device.
 *      bw:           new bandwidth of this carrier pairing.
 *      Device:       device to be assigned by TRM.
 */

typedef struct
{
  uint8 carriers[RFA_RF_LTE_MAX_CELL_SUPPORTED];   
  rflte_path_type    path;
  uint8   bandwidth;
  rfm_device_enum_type  device;  
} rflte_mc_single_radio_element_type_s;

/*
 * rflte_mc_single_radio_group_elements_type_s
 *   Return back to caller with all potential Srx carrier group and new bw.
 *   With each group of rfa_rf_config_param_type_s:
 *      carriers:     mask of carriers in this group.  -1 for invalid case.
 *      bw:           new bandwidth of this carrier pairing.
 *      Device:       device to be assigned by TRM.
 */



typedef struct
{
  uint8   num_elements;
  rflte_mc_single_radio_element_type_s group_elements[RFLTE_MC_SINGLE_RADIO_MAX_ELEMENTS];
} rflte_mc_single_radio_group_type_s;



/*
 * rflte_mc_single_radio_combo_type_s
 *   Return back to caller with all potential Srx pairing.
 *   Ranking: lowest index with highest ranking.  index 0 > RFLTE_SINGLE_RADIO_MGR_COMBO_MAX -1
 *   With each combo of rfa_rf_config_param_type_s:
 *      bw:            become the new bandwidth of this carrier pairing.
 */
typedef struct
{
  rflte_mc_single_radio_element_type_s single_radio_groups[RFLTE_MC_SINGLE_RADIO_COMBO_MAX][RFLTE_MC_SINGLE_RADIO_CARRIER_PATH_COMBO_MAX];
} rflte_mc_single_radio_combo_type_s;


/*
 * rflte_mc_single_radio_status_type
 *   rflte_mc_single_radio_check_combo() to return caller if current comb capable supporting single_radio
 *   - RF_LTE_MC_SINGLE_RADIO_OK: Combination OK for SRx
 *   - RF_LTE_MC_SINGLE_RADIO_FAILED: Device or other condition failed at device level.
 *   - RF_LTE_MC_SINGLE_RADIO_ERROR: carrier pair or parameter is invalid
 *   - RF_LTE_MC_SINGLE_RADIO_MAX,
 */

typedef enum 
{
  RF_LTE_MC_SINGLE_RADIO_OK,
  RF_LTE_MC_SINGLE_RADIO_FAILED,
  RF_LTE_MC_SINGLE_RADIO_ERROR,
  RF_LTE_MC_SINGLE_RADIO_MAX,
} rflte_mc_single_radio_status_type;

/*===========================================================================

                        FUNCTION PROTOTYPES

===========================================================================*/


/*----------------------------------------------------------------------------*/
/*!
  @brief 
  RFLTE MC external function to calculate Rx possbilities based on input carriers
  
  @rflte_mc_single_radio_type_s* single_radio_carriers
  All potential single_radio carriers

  @rflte_mc_single_radio_combo_type_s* single_radio_comb
  Calculated and ranked single_radio combo pairs.
 
  @return 
  boolean true for procedure pass.
  
*/
boolean rflte_mc_single_radio_get_combo( rflte_mc_single_radio_type_s* single_radio_carriers, rflte_mc_single_radio_combo_type_s* single_radio_comb);

/*----------------------------------------------------------------------------*/
/*!
  @brief 
  RFLTE MC external function to calculate Rx possbilities based on input carriers
  
  @rflte_mc_srx_type_s* srx_carriers
  All potential srx carriers

  @rflte_mc_srx_combo_type_s* srx_comb
  Calculated and ranked srx combo pairs.
 
  @return 
  rflte_mc_status_type
  RF_LTE_MC_SINGLE_RADIO_OK,
  RF_LTE_MC_SINGLE_RADIO_FAILED,
  RF_LTE_MC_SINGLE_RADIO_ERROR,
  
*/
rflte_mc_single_radio_status_type rflte_mc_single_radio_check_combo(  rflte_mc_single_radio_group_type_s* group_elements, void* carriers_ptr);


void rflte_mc_single_radio_test_config_api(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

