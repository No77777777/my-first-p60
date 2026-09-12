#ifndef RFLTE_MC_EXT_H
#define RFLTE_MC_EXT_H

/*
   @file
   rflte_ext_mc.h

   @brief
   RF LTE Driver's external interface file.

   @details

*/

/*===========================================================================
Copyright (c) 2002 - 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //commercial/MPSS.TA.3.0.c1/Main/modem_proc/stubs/api/rfa_api/lte/rflte_ext_mc.h#1 $
$DateTime: 2018/04/15 02:22:08 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
03/09/16   mah      Adding a flag to indicate if retune is not required i.e. if 
                   gapless measurements are supported for a CA combo. 
12/07/15   sg      Remove code featurized under RFLTE_BOLT_UL_CA_SUPPORTED
11/26/15   lhb     FR 31436: API to provide the HW band limitation to LTE RRC
10/26/15   kr     Add support for Multi cluster NV band mask
10/13/15   mah     Add support for return band range informaton of partial band to RRC
10/01/15    kr     Added support for Multi cluster band mask
07/29/15    br     Added support for NLIC with ULCA
07/21/15   stw     RF-RRC Interface for Bandwidth Class Combos Update
07/16/15   jd      WTR hopping phase2.
06/16/15   sb      Prerequisite changes for TX c1 calibration
06/08/15   stw     RF-RRC Interface for Bandwidth Class Combos
05/14/15   stw     Added support for 256 lte band mask check
04/08/15   pkg     Changes to support Tx Rx Split Band design.
03/10/15    br     Removed NLIC support for 3DLCA for TABASCO
01/21/15    br     NLIC support for 3DLCA
10/20/14   st      RRC interface for Gapless Support BandMask
09/15/14   kab     Modify RF API for RRC integration
09/15/14   kab     Inform RRC about LTE CA Band Combo Support 
08/12/14    vb     NLIC clock mode support
08/12/14   jf      MCL LTE support
07/31/14   ndb     Added support for extended EARFCN for LTE
07/18/14   jf      Return TX power in linear region to ML1
07/08/14   gvn     API for RRC to query split band channel raster
05/01/14   bsh     NLIC status to ML1
03/28/14   svi     Expose Intra Band CA capabilities to RRC
02/28/14   php     Add APIs for Light Sleep/Wakeup 
10/28/13   npi     Define LTE cell type enum
05/07/13   cri     Export temp comp scaling API
01/17/13   gvn     Expose some rflte_core_txpl APIs which are used by MCS team 
01/15/13   gvn     Fix compiler error 
01/04/13   gvn     Move interface to query RF LTE CA bc config from rfm to rflte_mc 
12/28/12   gvn     Initial Check-in

============================================================================*/

#include "rfcom.h"
#include "sys.h"
#include "rfm_device_types.h"
#include "rflte_msg.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Max Number of Unique Bands in a bandwidth class combo */
#define MAX_NUM_UNIQUE_BAND_COMBOS 6

/* Number of default bandwidth class combos for memory allocation */
#define NUM_DEFAULT_BANDWIDTH_CLASS_COMBOS 1000

/* Number of default bandwidth class combos per set for memory allocation */
#define NUM_DEFAULT_BANDWIDTH_CLASS_COMBOS_PER_SET 200

/* Maimum Number of bandwidth class subsets for memory allocation : 4 Bands possible with each having Class upto F => 6^2 = 36 */
#define MAX_NUM_BANDWIDTH_CLASS_SUBSETS 36

/* Maximum Size of UL intersection list 6C3 = 20 
Explanation: with 6 unique DL combos , the maximum number of UL combos are generated when you have 6/2 = 3 UL combos*/
#define MAX_SIZE_UL_INTERSECTION_LIST 20

#define rflte_mc_rrc_max_partial_band  10
/*!
   @brief
   Enumeration for LTE Cells.

*/
typedef enum {
  RFLTE_MC_PCELL_IDX   = 0x00000000,
  RFLTE_MC_SCELL1_IDX  = 1,
  RFLTE_MC_SCELL2_IDX  = 2,
  RFLTE_MC_SCELL3_IDX  = 3,
  RFLTE_MC_SCELL4_IDX  = 4,
  RFLTE_MC_MAX_CELL_IDX_SUPPORTED,
  RFLTE_MC_INVALID_CELL = 0xFFFF
} rflte_mc_cell_idx_type;


typedef enum
{
  RF_LTE_TX,
  RF_LTE_PRX,
  RF_LTE_DRX,
  RF_LTE_DRX2,
  RF_LTE_DRX3,
  RF_LTE_MAX_DEVICES
}rf_lte_dev_type;


/*!
   @brief
   Enumeration for Clients of Bandwidth Class Combos/Band Combos Information
*/
typedef enum 
{
   RFLTE_MC_RRC_CLIENT  = 0,
   RFLTE_MC_MCS_CLIENT  = 1,
   RFLTE_MC_RRC_INVALID_CLIENT 
}rflte_mc_bw_class_client_type;

typedef struct
{
  rfm_device_enum_type device[RF_LTE_MAX_DEVICES];
  uint32 alt_path; //@to do: add support for alt path for all device types
}
rflte_mc_device_info_type;

typedef struct
{
  rflte_mc_device_info_type lte_devs_per_carr[RFLTE_MC_MAX_CELL_IDX_SUPPORTED];
}
rflte_mc_carr_to_device_map_type;


/*!
   @brief
   Enumeration for NLIC clock modes
*/
 typedef enum { 
     /* mapped to 285300 khz */
     RFLTE_MC_NLIC_CLOCK_MODE_MCVS_SVS2  = 0,

     /* mapped to 499200 khz */
     RFLTE_MC_NLIC_CLOCK_MODE_MCVS_SVS   = 1,

     /* mapped to 665600 khz */
     RFLTE_MC_NLIC_CLOCK_MODE_MCVS_NOM   = 2,

     /* mapped to 768000 khz */
     RFLTE_MC_NLIC_CLOCK_MODE_MCVS_TURBO = 3,

     RFLTE_MC_NLIC_CLOCK_MODE_MAX_MODE, 

     RFLTE_MC_NLIC_CLOCK_MODE_INVALID 
 }rflte_mc_nlic_clock_mode_type;

typedef enum
{
  RFLTE_MC_EXT_CARR_TYPE_PRX = 0,
  RFLTE_MC_EXT_CARR_TYPE_DRX,
  RFLTE_MC_EXT_CARR_TYPE_INVALID = 0xFF
} rflte_mc_ext_carrier_type;

typedef struct 
{
    rfcom_lte_earfcn_type              pcc_tx_chan;
    rfcom_lte_earfcn_type              scc_rx_chan;
    rfcom_lte_bw_type   pcc_ul_bw;          /*PCELL UL BW*/ 
    rfcom_lte_bw_type   scc_dl_bw;          /*SCELL Rx BW*/ 
} nlic_status_input_params;

typedef struct 
{
    boolean                 nlic_possible;
    uint8                   victim_id;
} nlic_status_return_params;

typedef struct
{
    rfcom_lte_earfcn_type              ul_chan_per_cell[RFA_RF_LTE_MAX_CELL_SUPPORTED];
    rfcom_lte_bw_type                  ul_bw_per_cell[RFA_RF_LTE_MAX_CELL_SUPPORTED];  
    rfcom_lte_earfcn_type              dl_chan_per_cell[RFA_RF_LTE_MAX_CELL_SUPPORTED];
    rfcom_lte_bw_type                  dl_bw_per_cell[RFA_RF_LTE_MAX_CELL_SUPPORTED];
    boolean                            dl_active_bitmask_per_cell[RFA_RF_LTE_MAX_CELL_SUPPORTED];                      
    boolean                            ul_active_bitmask_per_cell[RFA_RF_LTE_MAX_CELL_SUPPORTED];                      
} nlic_status_input_enhanced_params;

typedef struct 
{
    boolean                 nlic_possible;
    uint8                   victim_id;
    uint8                   aggr_id;
} nlic_status_return_enhanced_params;

typedef struct
{
   uint8 sf_num;
} rflte_mc_linear_tx_power_input_s;

typedef struct
{
   uint32 tx_power;
   uint32 tx_power_srs;
} rflte_mc_linear_tx_power_output_s;

/*!
   @brief
   CA Info per LTE CA Band combo - shared with RRC.

*/
typedef struct 
{ 
  /* Number of bands in a given CA Combo.*/
   uint8 num_bands_per_combo; 
   
   /*Band Info - Bit[0]Pcell + Bit[1]SCELL1 + Bit[2]SCELL2 + Bit[3]SCELL3 + Bit[4]SCELL4. */
   uint8 band[5];             

   /*Per Cell bw support.Bit[0] -> InterBand CA,Bit[1] -> IntraBand CA(20MHz),Bit[2] -> IntraBand CA(40MHz)*/
   uint8 bw_class[5];         
   
   /*UL Support - Bit[0]Pcell + Bit[1]SCELL1 + Bit[2]SCELL2 + Bit[3]SCELL3 + Bit[4]SCELL4.*/
   uint8 ul_supported; 

} rflte_mc_rrc_ca_info_per_band_combo_s; 

/*!
   @brief
   Structure to hold LTEE CA band combos - shared with RRC.

*/
typedef struct 
{ 
  /* Number of Supported CA band combos*/ 
   uint8 num_combos;  
                               
   /* As per ASNI, 36331 spec at max we can only support 128 combos */
   rflte_mc_rrc_ca_info_per_band_combo_s band_combo[128]; 

} rflte_mc_rrc_ca_band_combos_s;

 typedef struct 
 {
   uint64 pcell_band;
   uint64 gapless_support_permitted_mask;
 }rflte_rrc_gapless_support_cfg_t;

/*!
   @brief
   Structure to hold LTE bandwith class information per band - shared with RRC.

*/
typedef struct 
{ 
  
   /*Band Number : 1 -> Band1, 2 -> Band2 and so on . Note: 0 means Not Supported*/
   uint16 band;             

   /* dl_bw_class: 0  -> Not Supported, 1 – Class A(20MHz), 2 – Class B(20 Mhz , 2 CCs) , 3 – Class C (40 MHz – 2 CCs) and so on*/
   uint8 dl_bw_class; 

   /* ul_bw_class: 0  -> Not Supported, 1 – Class A(20MHz), 2 – Class B(20 Mhz , 2 CCs) , 3 – Class C (40 MHz – 2 CCs) and so on*/
   uint8 ul_bw_class;            

} rflte_mc_bw_class_info_per_band_s; 

/*!
   @brief
   Structure to hold LTE CA bandwith class information per bandwidth class combo - shared with RRC.

*/
typedef struct 
{ 
  rflte_mc_bw_class_info_per_band_s bw_class[MAX_NUM_UNIQUE_BAND_COMBOS];
/* Flag to indicate if no retune required for a particulare CA combo - always FALSE for Phase 1 change */
  boolean no_retune_required;
} rflte_mc_bw_class_info_per_band_combo_s; 


/*!
   @brief
   Structure to hold LTE CA bandwith class information for all bandwidth class combos - shared with RRC.

*/
typedef struct 
{ 
  /* Number of Supported band combos*/ 
   uint16 num_combos;  
                               
   /* Band_combos*/
   rflte_mc_bw_class_info_per_band_combo_s* band_combos; 

} rflte_mc_bw_class_combos_s;

/*
@brief
  Structure to store band range information for partial band ,shared with RRC
*/

typedef struct 
{ 
  uint32  band;
  uint32  start_dl_earfcn;  
  uint32  stop_dl_earfcn;
}rflte_mc_rrc_band_range_info_s;


/*
@brief
 Struture to store all partial band informaton got from RFC, share with RRC

*/

typedef struct 
{ 
  /* Number of partial band*/ 
   uint16 num_partial_bands;  
                               
   /* partial band information*/
   rflte_mc_rrc_band_range_info_s  lte_band_range[rflte_mc_rrc_max_partial_band];

}rflte_mc_rrc_partial_band_info_s;
 /*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

boolean rflte_mc_tx_coex_wlan_set_txplim(int16 coex_limit_db10) ;

#ifdef LTE_BAND_NUM
sys_lte_band_mask_e_type rflte_mc_get_lte_ca_band_mask
(
  rfcom_lte_band_type pcell_band
); 
#else
uint64 rflte_mc_get_lte_ca_band_mask(rfcom_lte_band_type pcell_band);
#endif 

void rflte_mc_get_lte_ca_gapless_support_band_mask(rflte_rrc_gapless_support_cfg_t *gapless_support_mask);


uint32 rflte_core_get_tx_freq_from_uarfcn(rfcom_lte_earfcn_type tx_chan, rfcom_lte_band_type band);

uint32 rflte_core_get_rx_freq_from_uarfcn(rfcom_lte_earfcn_type rx_chan, rfcom_lte_band_type band);

uint16 rflte_core_get_bw_in_khz(rfcom_lte_bw_type bw);

uint16 rflte_core_temp_comp_raw_to_scaled(uint32 carrier_idx, uint16 therm_raw_value);

void rflte_mc_light_sleep(void);

void rflte_mc_light_wakeup(void);

uint8 rflte_mc_single_rx_ca_get_intra_band_ca_support(rfcom_lte_band_type band);

#ifndef FEATURE_TABASCO_MODEM
/*  Follwing is only required for 3DLCA NLIC     */
void rflte_mc_get_nlic_enhanced_status(nlic_status_input_enhanced_params *nlic_input_params, 
                                       nlic_status_return_enhanced_params *nlic_return_params);
#endif

void rflte_mc_get_nlic_status(nlic_status_input_params *nlic_input_params, 
                              nlic_status_return_params *nlic_return_params);

void rflte_mc_get_split_rx_band_channel_raster(rfcom_lte_band_type split_band, 
                                               rfcom_lte_earfcn_type *low_earfcn,
                                               rfcom_lte_earfcn_type *high_earfcn,
                                               uint8 *num_of_splits);

void rflte_mc_fed_get_linear_tx_power(rflte_mc_linear_tx_power_input_s* tx_power_input,
                                      rflte_mc_linear_tx_power_output_s* tx_power_output);

boolean rflte_mc_get_tuner_scenario_list(rfm_device_enum_type device,
                                         rflte_mc_ext_carrier_type carrier_type,
                                         uint32 channel,
                                         rfm_antenna_tuner_scenario_data *scenario_data,
                                         uint8 *free_space_index,
                                         rfm_tuner_handle_type *tuner_handle);

boolean rflte_mc_tuner_stop_mcl(rfm_device_enum_type device,
                                rflte_mc_ext_carrier_type carrier_type,
                                uint32 channel,
                                rfm_tuner_handle_type tuner_handle);

void rflte_mc_get_lte_ca_band_combo_support(rflte_mc_rrc_ca_band_combos_s *ca_band_combo);

/*This function will use NV information to provide Client {RRC,MCS} with all the LTE CA bandwidth class combos support */

rflte_mc_bw_class_combos_s* rfm_lte_get_lte_ca_bandwidth_class_combos_support(rflte_mc_bw_class_client_type client);

/*This function will be invoked when the Client of rflte_mc_get_lte_ca_bandwidth_class_combos_support is done using the information {bandwidth class combos} 
  provided so that we can de-allocate memory containing the information provided {bandwidth class combos}*/
boolean rfm_lte_dealloc_bandwidth_class_combos(rflte_mc_bw_class_client_type client);


void rfm_lte_get_multi_cluster_support(sys_lte_band_mask_e_type *single_band_support_mask_ptr, 
                                                               sys_lte_band_mask_e_type *intra_band_ca_support_mask_ptr);

void rfm_lte_get_partial_band_range_info(rflte_mc_rrc_partial_band_info_s* partial_band_info);
/*----------------------------------------------------------------------------*/
/*!
   @brief
   Enumeration for RFLTE Rx/Tx path types.
*/
typedef enum {
  RFLTE_PATH_TYPE_RX0,
  RFLTE_PATH_TYPE_RX1,
  RFLTE_PATH_TYPE_RX2,
  RFLTE_PATH_TYPE_RX3,
  RFLTE_PATH_TYPE_MAX_RX,
  RFLTE_PATH_TYPE_TX0 = RFLTE_PATH_TYPE_MAX_RX,
  RFLTE_PATH_TYPE_TX1,
  RFLTE_PATH_TYPE_MAX,
  RFLTE_PATH_TYPE_INVALID = 0xFF,
}rflte_path_type;

/*! @brief maximum number of tx paths = max paths subtracted by start of tx path enum*/
#define RFLTE_PATH_TYPE_MAX_TX  (RFLTE_PATH_TYPE_MAX - RFLTE_PATH_TYPE_TX0)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RFLTE_MC_EXT_H */

