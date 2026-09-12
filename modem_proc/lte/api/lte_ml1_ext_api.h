/*!
  @file
  lte_ml1_ext_api.h

  @brief
  This file describes the API interfaces to LTE ML1
*/


/*===========================================================================

  Copyright (c) 2010-2012 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/lte.mpss/6.6.1/api/lte_ml1_ext_api.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
10/04/16   skk     Added interface for L2 to check if DL CA is active.
05/09/12   ns      Add new API for generic ML1 and GM CMAPI
05/25/11   tjc     Added PRS interface
11/29/10   fhu     Added clock config function here for RF FTM to use
07/09/10   tjc     Add GPS MRL interface
03/26/10   tjc     Initial version
===========================================================================*/

#ifndef LTE_ML1_EXT_API_H
#define LTE_ML1_EXT_API_H
#include "lte_cphy_msg.h"
#include "lte_cphy_rssi_msg.h" 
#include "sys.h"
#include "lte_as.h"
#include "trm.h"
#include "rflte_msg.h"






/*===========================================================================

                           EXTERNAL TYPES FILES

===========================================================================*/

/*! Invalid timing advance */
#define LTE_ML1_GPS_MRL_INVALID_TIMING_ADVANCE 0xFFFF  


/*! Maximum number of 8 neighbor cells + one serving cell in the MRL */
#define LTE_ML1_GPS_MRL_CELLS_MAX 9

/*! The invalid scheduler subframe time. The macro indicates that the
  current time is invalid; it can be returned from the systime query interface.
  It is also used internally within the scheduler for the initialization. */
#define LTE_ML1_SCHDLR_SYS_TIME_INVALID  0xFFFF

/*! SUPL's MeasurementResultsEUTRAType */
typedef struct 
{
  /*! Reference signal - received power [0...97] in dBm */
  uint8      rsrp_result;
  /*! Reference signal - received quality [0...34] in dBm */
  uint8      rsrq_result;

} lte_ml1_gps_mrl_rsrp_rsrq_t;

/*! SUPL's MeasuredResultsEUTRAType */
typedef struct
{
  /*! Physical cell id [0..503] */
  uint16                       phys_cell_id;
  /*! E-ARFCN [0..262143] */
  lte_earfcn_t                 earfcn;
  /*! Timing advance [0...1282], 0xFFFF - Invalid */
  uint16                       timing_advance;
  /*! Measurement results */
  lte_ml1_gps_mrl_rsrp_rsrq_t  z_rsrp_rsrq;

} lte_ml1_gps_mrl_cell_meas_t;  

/*! SUPL's MeasuredResultsListEUTRAType */
typedef struct
{
  /*! Number of cells in the MRL */
  uint8                        num_cells;
  /*! Whether the MRL TA is valid */
  boolean                      ta_valid;
  /*! MRL cell info */
  lte_ml1_gps_mrl_cell_meas_t  cell_mrl_info[LTE_ML1_GPS_MRL_CELLS_MAX];

} lte_ml1_gps_mrl_cell_meas_update_t;

/*! LTE->GPS event types */
typedef enum
{
  LTE_ML1_GPS_EVENT_CELL_MEAS_UPDATE,  /*!< MRL event type */
  LTE_ML1_GPS_EVENT_MAX,               /*!< Max event for range checking */

} lte_ml1_gps_event_e;

/*! LTE->GPS message data type */
typedef struct
{
  uint8  *p_data;
  uint32  length;
} lte_ml1_gps_msg_data_s;

/*! LTE->GPS message type */
typedef struct
{
  lte_ml1_gps_event_e     e_event_type;
  lte_ml1_gps_msg_data_s  z_l1_cgps_msg_data;
} lte_ml1_gps_event_data_s;

/*! LTE->GPS event callback */
typedef boolean (*lte_ml1_gps_event_cb_t)(lte_ml1_gps_event_data_s *event_data_ptr);


/*! LTE scan data per earfcn */
typedef struct
{
  lte_earfcn_t  earfcn;
  uint8         bandwidth; /* 0..5 for (1.4)..20 MHz*/
  int8          rssi;
} lte_ml1_scan_data_per_earfcn_s;


/*! @brief
    CM start scan capture indication structure
*/
typedef struct
{
  /*! Message router header */
  msgr_hdr_struct_type             msgr_hdr;

  /*! Pointer to memory for LTE scan data */
  void                            *scan_data;

  /*! Memory size (in bytes) for LTE scan data */
  uint16                           scan_data_mem_size;

}lte_ml1_common_cm_scan_capture_start_ind_s;


/*! @brief
    CM stop scan capture indication structure
*/
typedef struct
{
  /*! Message router header */
  msgr_hdr_struct_type             msgr_hdr;
}lte_ml1_common_cm_scan_capture_stop_ind_s;


/*! @brief
    CM stop scan capture cnf structure
*/
typedef struct
{
  /*! Message router header */
  msgr_hdr_struct_type             msgr_hdr;
}lte_ml1_common_cm_scan_capture_stop_cnf_s;


/*! LTE Cost of modem power */
typedef enum
{
  LTE_ML1_TX_PWR_COST_LOW,
  LTE_ML1_TX_PWR_COST_MEDIUM,
  LTE_ML1_TX_PWR_COST_HIGH
} lte_ml1_tx_pwr_cost_e;


/*! Structure for uplink stats */
typedef struct 
{
  /* Cost of tx power */
  lte_ml1_tx_pwr_cost_e  tx_pwr_cost;

  /* Plus flag */
  boolean                plus_flag;
} lte_ml1_ul_stats_s;

/*! LTE PMCH stats, for a single PMCH */
typedef struct lte_pmch_info_s
{
  /*! PMCH ID */
  uint32   pmch_id;
  /*! Number of CRC passes */
  uint32   num_crc_pass;
  /*! Number of CRC failures */
  uint32   num_crc_fail;
} lte_pmch_info_s_type;

/*! LTE PMCH stats for an area */
typedef struct lte_pmch_area_stats_s
{
  /*! Area ID */
  uint16                     area_id;
  /*! Number of PMCHs in this area */
  uint16                     num_pmch;
  /*! Stats for each PMCH */
  lte_pmch_info_s_type     pmch_info[LTE_L1_MAX_PMCH_PER_MBSFN_AREA+1];
} lte_pmch_area_stats_s_type;

/*! LTE PMCH stats */
typedef struct lte_pmch_stats_s
{
  /*! Indicates if there is any activated TMGI */
  boolean                      is_valid;
  /*! Number of areas */
  uint32                       num_area;
  /*! Stats for each area */
  lte_pmch_area_stats_s_type pmch_area[LTE_EMBMS_MAX_MBSFN_AREA];
} lte_pmch_stats_s_type;

/*!
  @brief
  This message indicates PMCH stats to upper layers
*/
typedef struct
{
  msgr_hdr_struct_type     msgr_hdr;
  
  /*! Structure to indicate PMCH stats */
  lte_pmch_stats_s_type *pmch_stats;
} lte_ml1_common_cm_pmch_stats_req_s;

/*! Structure to return allowed bandwidths in a band */
typedef struct {
  /*! Band */
  sys_sband_lte_e_type  band;
  /*! num allowed bandwidths */
  uint32                num_allowed_bandwidths;
  /*! allowed bandwidths */
  lte_bandwidth_idx_e   allowed_bw_idx[LTE_MAX_NUM_BANDWIDTHS];
} lte_ml1_common_band_bw_info_s;

typedef struct
{
  /*! the actuall type is sys_modem_as_id_e_type */
  uint32 as_id:4; 
  /*! SCellIndex(0 .. 7 ). Value 0 is for Pcell. the actuall type is uint8 */
  uint32 scell_id:4; 
  /*! HARQ id for this interlace and this carrier. the actuall type is uint8 */
  uint32 harq_id:8;
  /*! what type of HARQ result is cared about by L2. 
      the actuall type is lte_l1_harq_fb_type_e */
  uint32 harq_fb_req_type:8;
  /*! what payload (ACK / NACK) of HARQ result is cared about by L2. 
      the actuall type is lte_l1_harq_fb_value_e */
  uint32 harq_fb_req_value:8;
} lte_ml1_gm_l2_interested_harq_fb_info_cfg_s; 

typedef struct
{
  union
  {
    /*! Interested harq feedback info cfg info */
    lte_ml1_gm_l2_interested_harq_fb_info_cfg_s  harq_fb_info_cfg;
	/*! data_payload */
    uint32                                       data_payload;
  };
} lte_ml1_gm_l2_interested_harq_fb_info_cfg_u; 


typedef struct
{
  /*! Indicates whether UE supports tune-away Back Indication.*/
  boolean tune_back_ind_support;
  /*! Indicates whether UE supports  SCell state synchronization.*/
  boolean scell_state_sync_support;
  /*! Indicates whether UE supports  UL timing sync recovery.*/
  boolean ul_timing_sync_support;
  /*! Indicates whether UE supports DL/UL link efficiency recovery*/
  boolean dlul_link_effic_recovery_support;
  /*! Indicates whether UE supports UL PUSCH Pre-Scheduling Request.*/
  boolean pusch_pre_schedule_req_support;
}lte_els_dsds_ue_mac_ce_cap_s;

typedef struct
{
   boolean  partial_tune_support;
   lte_els_dsds_ue_mac_ce_cap_s  mac_ce_ue_cap;
   boolean dsds_MultipleGapCapability_support;
   uint8 dsds_MultipleGapCapability;
} lte_els_dsds_ue_cap_s;
typedef struct
{
  /*! Message router header */
  msgr_hdr_struct_type                   msgr_hdr;
  boolean                                start;
} lte_ml1_embms_session_ind_s;

/*===========================================================================

                        EXTERNAL FUNCTIONS

===========================================================================*/

/*===========================================================================

  FUNCTION:  lte_ml1_request_ul_stats

===========================================================================*/
/*!
    @brief
    This function is the external function to get UL statistics

    @return
    TRUE if LTE is in connected/FALSE otherwise
    (UL stats are valid only when TRUE is returned)

*/
/*=========================================================================*/
boolean lte_ml1_request_ul_stats
(
  sys_modem_as_id_e_type    as_id,
  /*! LTE UL stats */
  lte_ml1_ul_stats_s *ul_stats
);

/*===========================================================================

  FUNCTION:  lte_ml1_request_gps_timetag

===========================================================================*/
/*!
    @brief
    This function is the external GPS interface for requesting a timetag.

    @return
    None

*/
/*=========================================================================*/
void lte_ml1_request_gps_timetag
(
  sys_modem_as_id_e_type as_id,
  /*! Whether sleep estimation is allowed for timetag */
  boolean allow_sleep_timetag
);

/*===========================================================================

  FUNCTION:  lte_ml1_subscribe_gps_mrl

===========================================================================*/
/*!
    @brief
    This function is the external GPS interface for subscribing a measurement
    results list.
 
    @param
     Boolean start_subscribe:
        TRUE: Subscribe. Send MRL update immediately and then henceforth send update when MRL changes
        FALSE: Unsubscribe. Stop sending MRL updates to GPS.
 
    @return
    None


*/
/*=========================================================================*/
void lte_ml1_subscribe_gps_mrl
(
   sys_modem_as_id_e_type as_id,
   boolean subscribe
);
 /* lte_ml1_subscribe_gps_mrl */

/*===========================================================================

  FUNCTION:  lte_ml1_request_gps_timetag

===========================================================================*/
/*!
    @brief
    This function is the external GPS interface for requesting an MRL.

    @return
    None

*/
/*=========================================================================*/
void lte_ml1_request_gps_mrl( sys_modem_as_id_e_type as_id );

/*===========================================================================

  FUNCTION:  lte_ml1_register_gps_event_cb

===========================================================================*/
/*!
    @brief
    This function is the external GPS interface for registering and event
    callback.

    @return
    None

*/
/*=========================================================================*/
void lte_ml1_register_gps_event_cb( sys_modem_as_id_e_type as_id, lte_ml1_gps_event_cb_t event_cb );

/*===========================================================================

  FUNCTION:  lte_ml1_request_gps_timexfer

===========================================================================*/
/*!
    @brief
    This function is the external GPS interface for requesting a time transfer.

    @return
    None

*/
/*=========================================================================*/
void lte_ml1_request_gps_timexfer
(
  sys_modem_as_id_e_type as_id,
  uint32 seq_num,                        /*!< Sequence number              */
  lte_cphy_gps_timexfer_prio_e priority, /*!< Priority of the time transfer */
  boolean sib8_required,                 /*!< Whether SIB8 is required     */
  uint8 number_of_tt_samples             /*!< Number of samples requested */
);

/*===========================================================================

  FUNCTION:  lte_ml1_abort_gps_timexfer

===========================================================================*/
/*!
    @brief
    This function is the external GPS interface for aborting a time transfer.

    @return
    None

*/
/*=========================================================================*/
void lte_ml1_abort_gps_timexfer
(
  sys_modem_as_id_e_type as_id,
  uint32 seq_num                         /*!< Sequence number              */
);

/*===========================================================================

  FUNCTION:  lte_ml1_pos_ustmr_to_ostmr

===========================================================================*/
/*!
    @brief
    This function is the external GPS interface for converting ustmr to ostmr.

    @return
    None

*/
/*=========================================================================*/
void lte_ml1_pos_ustmr_to_ostmr
(
  sys_modem_as_id_e_type as_id,
  uint32 ustmr,                        /*!< USTMR              */
  uint32* ostmr,                       /*!< OSTMR              */
  uint32* ostmr_ph                     /*!< OSTMR fractional   */
);

/*===========================================================================

  FUNCTION:  lte_ml1_request_gps_rxtx

===========================================================================*/
/*!
    @brief
    This function is the external GPS interface for requesting a RXTX value.

    @return
    None

*/
/*=========================================================================*/
void lte_ml1_request_gps_rxtx
(
  sys_modem_as_id_e_type as_id,
  void* p_Meas   /*!< Static GPS struct to be populated    */
);

/*===========================================================================

  FUNCTION:  lte_ml1_gps_crs_start

===========================================================================*/
/*!
    @brief
    This function is the external GPS interface to start CRS reporting.

    @return
    None

*/
/*=========================================================================*/
void lte_ml1_gps_opcrs_start
(
 sys_modem_as_id_e_type as_id
);

/*===========================================================================

  FUNCTION:  lte_ml1_gps_crs_stop

===========================================================================*/
/*!
    @brief
    This function is the external GPS interface to stop CRS reporting.

    @return
    None

*/
/*=========================================================================*/
void lte_ml1_gps_opcrs_stop
(
  sys_modem_as_id_e_type as_id
);

/*===========================================================================

  FUNCTION:  lte_ml1_common_cm_nbr_rssi_ind_req

===========================================================================*/
/*!
    @brief
    Sends RSSI indication to upper layers.

    @return
    boolean.

*/
/*=========================================================================*/

void lte_ml1_common_cm_nbr_rssi_ind_req(sys_modem_as_id_e_type as_id,cmapi_lte_ngbr_ind_s *lte_ngbr_ind);



/*===========================================================================

  FUNCTION:  lte_ml1_common_cm_rssi_ind_req

===========================================================================*/
/*!
    @brief
    Sends RSSI data to upper layers.

    @return
    Structure for RSSI Ind.

*/
/*=========================================================================*/
extern lte_api_signal_info_s_type lte_ml1_common_cm_rssi_ind_req(sys_modem_as_id_e_type as_id);


/*===========================================================================

  FUNCTION:  lte_ml1_common_cmapi_ml1_mgr_req

===========================================================================*/
/*!
    @brief
    Sends ML1 MGR information to upper layers.

    @return
    Pointer to the ML1 MGR info.

*/
/*=========================================================================*/
lte_ml1_mgr_cm_ml1_mgr_info_s* lte_ml1_common_cmapi_ml1_mgr_req(sys_modem_as_id_e_type as_id);

/*===========================================================================

FUNCTION:  lte_ml1_common_cm_pmch_stats_req

===========================================================================*/
/*!
  @brief
  Sends PMCH stats to upper layers.

  @return
  void

 */
/*=========================================================================*/

void lte_ml1_common_cm_pmch_stats_req(sys_modem_as_id_e_type as_id,lte_pmch_stats_s_type *pmch_stats);


/*===========================================================================

  FUNCTION:  lte_ml1_common_cmapi_ml1_gm_req

===========================================================================*/
/*!
    @brief
    Sends ML1 GM information to upper layers.

    @return
    Pointer to the ML1 GM info.

*/
/*=========================================================================*/
lte_ml1_gm_cm_ml1_gm_info_s* lte_ml1_common_cmapi_ml1_gm_req(sys_modem_as_id_e_type as_id);


/*===========================================================================

  FUNCTION:  lte_ml1_start_capture_scan_data

===========================================================================*/
/*!
    @brief
    Trigger for LTE ML1 to start capturing scan data
    First 32 bits of scan data memory will contain below information
    - First 16 bits: cmapi_sys_mode_e RAT type typecast to uint16
    - Second 16 bits: number of following 32 bit words filled by the RAT
    Subsequent 32 bit words will be filled with LTE scan data as below
    {
        lte_earfcn_t    earfcn;
        uint8           bandwidth;      //0..5 for (1.4)..20 MHz
        int8            rssi;
    }

    @return
    boolean: TRUE if capture is successfully started, FALSE otherwise (should
    never be the case, can use it later for any error cases)

*/
/*=========================================================================*/
extern boolean lte_ml1_start_capture_scan_data
(
  sys_modem_as_id_e_type as_id,
  void   *lte_scan_data, /* Pointer to memory that will store LTE scan data */
  uint16  lte_scan_data_mem_size /* Size allocated for LTE scan data (including 32 bits for RAT and number of results) */
);


/*===========================================================================

  FUNCTION:  lte_ml1_stop_capture_scan_data

===========================================================================*/
/*!
    @brief
    Trigger for LTE ML1 to stop capturing scan data

    @return
    boolean: TRUE if capture is successfully stopped, FALSE otherwise (should
    never be the case, can use it later for any error cases)

*/
/*=========================================================================*/
extern boolean lte_ml1_stop_capture_scan_data
(
  sys_modem_as_id_e_type as_id
);


/*===========================================================================

                        lte_common_change_rx_diversity

===========================================================================*/
/*!
    @brief
    This function changes LTE RX diversity

    @detail
    This function changes LTE RX diversity.
 
    Note:
    1). this action code is different from the code provided by LTE RF.
        LTE ML1 will perform a translation of this
    2).LTE RX diversity can only changed when LTE RX is tuned, and RF S/W
       is not performing any other RF activities.

    @return
    TRUE:   if it is successfully changed
    FALSE:  if not.
     the failure cases include:
       -- RF S/W is busy and cannot take care of RX diversity change
           request
       -- RF S/W returns error when it performs the diversity changing
       -- LTE is busy and cannot take the request 

*/
/*=========================================================================*/
extern boolean lte_common_change_rx_diversity
(
  sys_modem_as_id_e_type as_id,
  boolean enable_rx0,   /* TRUE to enable RX0, FALSE to disable RX0 */
  boolean enable_rx1   /* TRUE to enable RX1, FALSE to disable RX1 */
);

/*===========================================================================

                        lte_common_change_scc_rx_diversity

===========================================================================*/
/*!
    @brief
    This function changes LTE RX diversity on a SCC

    @detail
    This function changes LTE RX diversity on a SCC.
 
    Note:
    1). this action code is different from the code provided by LTE RF.
        LTE ML1 will perform a translation of this
    2).LTE RX diversity can only changed when LTE RX is tuned, and RF S/W
       is not performing any other RF activities.
       The following is the status code 
          0: LTE_CPHY_RFCHAIN_REQ successfully processed
          1: ML1 is busy and can not process the command
          2: RF cannot process the command
          3: RF returns error
          4: general error
    

    @return
    TRUE:   if it is successfully changed
    FALSE:  if not.
     the failure cases include:
       -- RF S/W is busy and cannot take care of RX diversity change
           request
       -- RF S/W returns error when it performs the diversity changing
       -- LTE is busy and cannot take the request 

*/
/*=========================================================================*/
extern boolean lte_common_change_scc_rx_diversity
(
  sys_modem_as_id_e_type as_id,
  uint8   scc_id,      /* SCC index, starting from 0 */
  boolean enable_rx0,  /* TRUE to enable RX0, FALSE to disable RX0 */
  boolean enable_rx1   /* TRUE to enable RX1, FALSE to disable RX1 */
);

/*===========================================================================

                        lte_common_change_horxd_rx_diversity

===========================================================================*/
/*!
    @brief
    This function changes LTE RX diversity on HORXD rx2 and rx3

    @detail
    This function changes LTE RX diversity on HORXD rx2 and rx3
 
    Note:
    1). this action code is different from the code provided by LTE RF.
        LTE ML1 will perform a translation of this
    2).LTE RX diversity can only changed when LTE RX is tuned, and RF S/W
       is not performing any other RF activities.
       The following is the status code 
          0: LTE_CPHY_RFCHAIN_REQ successfully processed
          1: ML1 is busy and can not process the command
          2: RF cannot process the command
          3: RF returns error
          4: general error
    

    @return
    TRUE:   if it is successfully changed
    FALSE:  if not.
     the failure cases include:
       -- RF S/W is busy and cannot take care of RX diversity change
           request
       -- RF S/W returns error when it performs the diversity changing
       -- LTE is busy and cannot take the request 

*/
/*=========================================================================*/
extern boolean lte_common_change_horxd_rx_diversity
(
  sys_modem_as_id_e_type as_id,
  uint8   cc_id,       /* CC index - 0: PCC; 1: SCC_0; 2: SCC_1 etc. */
  boolean enable_rx2,  /* TRUE to enable RX2, FALSE to disable RX2 */
  boolean enable_rx3   /* TRUE to enable RX3, FALSE to disable RX3 */
);

/*===========================================================================

  FUNCTION:  lte_ml1_common_clock_config

===========================================================================*/
/*!
    @brief
    Configures the hw, including clock setting

    @details
    Configures the hw, including clock setting

    @return
    None

*/
/*=========================================================================*/
extern void lte_ml1_common_clock_config ( void );

/*===========================================================================

  FUNCTION:  lte_ml1_crs_meas_req

===========================================================================*/
/*!
    @brief
    This function sends a start CRS measurement request to ML1.
 
    @details
    This function is called by Loc SW module to request CRS measurements
 
    @return
    None

*/
/*=========================================================================*/
void lte_ml1_crs_meas_req
(
  /*! CRS meas request information */
  lte_cphy_crs_meas_req_s *crs_meas_req,
 /*! Max number of cells to be measured */
  uint8 max_num_cells
);

/*===========================================================================

  FUNCTION:  lte_ml1_pos_prs_start

===========================================================================*/
/*!
    @brief
    This function sends a start PRS request to ML1.

    @return
    None

*/
/*=========================================================================*/
void lte_ml1_prs_start
(
  sys_modem_as_id_e_type as_id,
  /*! PRS start request information */
  lte_cphy_prs_start_req_s *prs_start_req
);

/*===========================================================================

  FUNCTION:  lte_ml1_prs_update_cells

===========================================================================*/
/*!
    @brief
    This function sends a PRS update cells request to ML1.

    @return
    None

*/
/*=========================================================================*/
void lte_ml1_prs_update_cells
(
  sys_modem_as_id_e_type as_id,
  /*! PRS update cells request information */
  lte_cphy_prs_update_cells_req_s *prs_update_cells_req
);

/*===========================================================================

  FUNCTION:  lte_ml1_prs_stop

===========================================================================*/
/*!
    @brief
    This function sends a PRS stop request to ML1.

    @return
    None

*/
/*=========================================================================*/
void lte_ml1_prs_stop
(
  sys_modem_as_id_e_type as_id,
  /*! PRS stop request information */
  lte_cphy_prs_stop_req_s *prs_stop_req
);

/*===========================================================================

  FUNCTION:  lte_ml1_prs_is_mbsfn_subframe

===========================================================================*/
/*!
    @brief
    This function checks if a subframe is a mbsfn subframe.

    @return
    TRUE if the subframe is a MBSFN subframe, else FALSE

*/
/*=========================================================================*/
boolean lte_ml1_prs_is_mbsfn_subframe
(
  /* subframe number to be checked */
  uint16 check_subframe
);

/*===========================================================================

  FUNCTION:  lte_ml1_pos_prs_stop_pbch_decode

===========================================================================*/
/*!
    @brief
    This function sends a PBCH decode stop request to the SM.

    @return
    None

*/
/*=========================================================================*/
void prs_ml1_pos_prs_stop_pbch_decode( void );

/*===========================================================================

  FUNCTION:  lte_ml1_pos_prs_inter_frequency_capable

===========================================================================*/
/*!
    @brief
    This function returns whether the PRS module is capable of processing
    inter-frequency cells

    @return
    Whether PRS module is capable of processing inter-frequency cells.

*/
/*=========================================================================*/
boolean lte_ml1_pos_prs_inter_frequency_capable( lte_mem_instance_type instance );

/*===========================================================================

  FUNCTION:  lte_ml1_schdlr_get_sys_sfn

===========================================================================*/
/*!
    @brief
    The funciton returns the current systime frame number

    @detail
    This will be used by an external API, dont use Mutex
    The SFN value will be valid only when LTE ML1 manager has acquired on
    a cell. 

    @return
    systime frame number
 
*/
/*=========================================================================*/
uint32 lte_ml1_schdlr_get_sys_sfn(lte_mem_instance_type instance);

/*==============================================================================

  FUNCTION:  lte_ml1_convert_lte_band_to_sys_band

==============================================================================*/
/*!
  @brief
  Converts lte band to Sys band enum

  @detail
  
  @return
  Sys Band enum 
*/
/*============================================================================*/
sys_band_class_e_type lte_ml1_convert_lte_band_to_sys_band(sys_sband_lte_e_type lte_band);

/*==============================================================================

  FUNCTION:  lte_ml1_common_band_earfcn_bw_is valid

==============================================================================*/
/*!
  @brief
  Validate that for a given earfcn, the UE does not access channels outside
  the band range.

  @detail
  Validate that for a given earfcn, the UE does not access channels outside
  the band range. This is to prevent illegal band access when a UE is given
  a bad earfcn on handover. Validates the dl earfcn since the ul earfcn should
  mirror the bandwidth usage.

  @return
  TRUE if the UE is restricted to the band it is in.
*/
/*============================================================================*/
boolean lte_ml1_common_band_earfcn_bw_is_valid
(
  lte_mem_instance_type instance,
  lte_earfcn_t   dl_earfcn,
  lte_bandwidth_e dl_nrbs
);

/*==============================================================================

  FUNCTION:  lte_ml1_common_band_get_allowed_bw_in_band

==============================================================================*/
/*!
  @brief
  Checks if passed in band is valid and returns allowed BW

  @detail
  Checks is passed in band index is a valid LTE band. If it is, function returns
  the allowed BW in the band

  @return
  True if passed in band is valid, FALSE otherwise
*/
/*============================================================================*/
boolean
lte_ml1_common_band_get_allowed_bw_in_band(
  lte_mem_instance_type           instance,
  /*! Band */
  sys_sband_lte_e_type   band_id,
  /*! Ptr to allowed bands data structure */
  lte_ml1_common_band_bw_info_s  * band_bw_info_ptr
);

/*==============================================================================

  FUNCTION:  lte_ml1_common_band_get_band_from_dl_earfcn

==============================================================================*/
/*!
  @brief
  Checks if passed in DL earfcn is valid and returns the band number

  @detail
  Checks is passed in DL earfcn is a valid value. If it is, function returns
  the band number

  @return
  True if passed in DL earfcn is valid, FALSE otherwise
*/
/*============================================================================*/
boolean 
lte_ml1_common_band_get_band_from_dl_earfcn(
  /*! DL earfcn */
  lte_earfcn_t  dl_earfcn,
  /*! Ptr to band number */
  sys_sband_lte_e_type  * band_ptr
);

/*===========================================================================

  FUNCTION:  lte_ml1_common_cmapi_get_doppler_measurement

===========================================================================*/
/*!
    @brief
    Get doppler measurement of primary serving cell passed by FW.

    @return
    An uint16 value between 0 to 400. Return LTE_ML_MDB_INVALID_DOPPLER_VALUE
    (0xffff) if no measuring is done yet.

*/
/*=========================================================================*/
uint16 lte_ml1_common_cmapi_get_PCC_doppler_measurement(sys_modem_as_id_e_type as_id);

/*===========================================================================

  FUNCTION:  lte_ml1_validate_tdd_cfg_mbsfn_sf_alloc

===========================================================================*/
/*!
    @brief
    Validate tdd subframe allocation with mbsfn allocation. 

    @return
    Validation status "boolean"
        true if parameters are valid
        false otherwise.
*/
/*=========================================================================*/
boolean lte_ml1_validate_tdd_cfg_mbsfn_sf_alloc(
	                          lte_mem_instance_type                 instance,
                              lte_cphy_tdd_cfg_param_s             *tdd_cfg_to_validate,
                              lte_cphy_common_cfg_req_param_s      *common_cfg_params);


/*===========================================================================

  FUNCTION:  lte_ml1_get_test_bsr_val

===========================================================================*/
/*!
    @brief
    This function reads a test BSR value to be sent (if configured)
    from EFS

    @return
    BSR value (0xFF if invalid)

*/
/*=========================================================================*/
uint8 lte_ml1_get_test_bsr_val
(
  void
);

/*===========================================================================

  FUNCTION:  lte_ml1_mclt_override_scenario

===========================================================================*/
/*!
    @brief
    Function to override the scenario value.

    @details
    The current scenario value is overridden with the provided value. This is
    expected to be used only for debug purposes

    @return
    -1 for failure, 0 for success
*/
/*=========================================================================*/
int lte_ml1_mclt_override_scenario
(
  lte_mem_instance_type  instance,
  uint8 antenna,
  uint8 scenario_val
);

/*===========================================================================

  FUNCTION:  lte_ml1_mcvs_raise_q6_for_ho

===========================================================================*/
/*!
    @brief
    Raise Q6 for HO. 

    @details
    Raise Q6 for HO. 
 
    @return
    none

*/
/*=========================================================================*/
void lte_ml1_mcvs_raise_q6_for_ho
(
   lte_mem_instance_type       instance,
   boolean                     ho_started
);

/*===========================================================================

  FUNCTION:  lte_ml1_sleepmgr_enable_disable_deep_sleep

===========================================================================*/
/*!
    @brief
    API to disable LTE deep sleep

    @details
    API to disable LTE deep sleep. If enable flag is TRUE then deep sleep
    is enabled otherwise disabled. 
 
    @return
    none

*/
/*=========================================================================*/
void lte_ml1_sleepmgr_enable_disable_deep_sleep
(
   sys_modem_as_id_e_type      as_id,
   boolean                     enable
);

/*===========================================================================

  FUNCTION:  lte_ml1_mgr_els_dsds_ue_cap

===========================================================================*/
/*!
    @brief
    rrc query ML1 els DSDS UE capbilities

    @return
    None
*/
/*=========================================================================*/

extern void lte_ml1_mgr_els_get_ue_cap(lte_els_dsds_ue_cap_s *els_dsds_ue_cap_ptr, lte_mem_instance_type instance);
/*===========================================================================

  FUNCTION:  lte_ml1_validate_tdd_frame_type2_prach_cfg

===========================================================================*/
/*!
    @brief
    This function validate prach configure of tdd cfg2

    @detail
    according to 36.211 table5.7.1-4, validtate if prach is N/A or not

    @return
    TREU: valid
    FALSE:N/A
*/
/*=========================================================================*/
boolean lte_ml1_validate_tdd_frame_type2_prach_cfg
(
  uint8                        prach_cfg, 
  lte_l1_tdd_ul_dl_cfg_index_e subframe_assign
);
/*==========================================================================
  FUNCTION:  lte_ml1_common_is_dl_ca_active

===========================================================================*/
/*!
    @brief
    Checks whether DL CA is active or not. Used 
    by L2 and UL for throttling UL/DL for bus 
    bandwidth mitigation.

    @return
    TRUE  if DL CA is active.
    FALSE if DL CA is just configured/not confugured.

*/
/*=========================================================================*/
boolean lte_ml1_common_is_dl_ca_active(uint8 instance);

/*===========================================================================
  FUNCTION:  lte_ml1_gm_set_l2_interested_harq_fb_info_cb

===========================================================================*/
/*!
    @brief
    This is a cb function called from A2,
    setting if HARQ result is cared about by L2

    In this function, ML1 will record correspoind info to ML1 global
    
    @retval
    None
 
    @return
    None
*/
/*=========================================================================*/
extern void lte_ml1_gm_set_l2_interested_harq_fb_info_cb
(
  uint32 payload
);

/*===========================================================================

  FUNCTION:  lte_ml1_rfmgr_trm_is_client_registered

===========================================================================*/
/*!
    @brief
    To query a client register status

    @return
    None

*/
/*=========================================================================*/
boolean lte_ml1_rfmgr_trm_is_client_registered
(
  lte_mem_instance_type instance,
  trm_client_enum_t     client_id
);

/*===========================================================================

                        lte_ml1_bplmn_intf_rrc_is_page_merge_needed

===========================================================================*/
/*!
    @brief
    This function returns if page merge is needed or not.

    @detail
    This function returns if page merge is needed or not.

    @return
    TRUE: Page merge needed
    FALSE: Page merge is not needed

*/
/*=========================================================================*/

boolean lte_ml1_bplmn_intf_rrc_is_page_merge_needed
(
  lte_mem_instance_type instance
);

/*===========================================================================

  FUNCTION:  lte_ml1_bplmn_intf_rrc_get_prio_status

===========================================================================*/
/*!
    @brief
    This function returns if RRC/ML1 shall use inver acq priority or
    acq priority

    @details
    XRAT to LTE, it is decided by XRAT
    FPLMN search, always use invert acq priority
    BPLMN search on LTE or XRAT, it is decided by DRX cycle

    @return
    TRUE: use invert acq priority
    FALSE: use acq priority
*/
/*=========================================================================*/
boolean lte_ml1_bplmn_intf_rrc_get_prio_status
(
  lte_mem_instance_type instance
);

/*===========================================================================

FUNCTION:  lte_ml1_get_mem_instance_from_as_id

===========================================================================*/
/*!
    @brief
    This function get LTE instance id from as id

    @detail
    This function get LTE instance id from as id

*/
/*=========================================================================*/
lte_mem_instance_type lte_ml1_get_mem_instance_from_as_id
(
  sys_modem_as_id_e_type as_id
);

/*===========================================================================

  FUNCTION:  lte_ml1_common_nv_cfg_is_lte_ftm_mode

===========================================================================*/
/*!
    @brief
    This function checks if LTE is in either FTM non-signaling mode or
    FTM calibration mode

    @return
    TRUE if LTE is in FTM NS or CAL mode, FALSE otherwise
*/
/*=========================================================================*/
boolean lte_ml1_common_nv_cfg_is_lte_ftm_mode(lte_mem_instance_type instance);

/*===========================================================================

  FUNCTION:  lte_ml1_sleepmgr_enable_disable_deep_sleep_from_rrc

===========================================================================*/
/*!
    @brief
    API to disable LTE deep sleep from RRC

    @details
    API to disable LTE deep sleep. If enable flag is TRUE then deep sleep
    is enabled otherwise disabled.

    @return
    none

*/
/*=========================================================================*/ 
void lte_ml1_sleepmgr_enable_disable_deep_sleep_from_rrc
(
   lte_mem_instance_type        instance,
   boolean                      enable
);

/* Clock 2.1 LTE ML1 scenarios enums */
typedef enum
{ 

  /* #LTE_ML1_SCN_START_BOOST# */
  LTE_ML1_DLM_RX_CONFIG_BIMC_BOOST = 1,             		/* #10ms# */
  LTE_ML1_DLMCA_SCC_CONFIG_BIMC_BOOST,			   			/* #10ms# */
  LTE_ML1_DLMCA_SCC_STATE_ACTIVE_BIMC_BOOST,				/* #10ms# */
  LTE_ML1_IRAT_L2L_BEFORE_GAP_START_BIMC_BOOST,				/* #0ms# */
  LTE_ML1_IRAT_L2W_BEFORE_GAP_START_BIMC_BOOST,				/* #0ms# */
  LTE_ML1_IRAT_L2T_BEFORE_GAP_START_BIMC_BOOST,				/* #0ms# */
  LTE_ML1_IRAT_L21x_BEFORE_GAP_START_BIMC_BOOST,			/* #0ms# */
  LTE_ML1_IRAT_L2PRS_INTER_BEFORE_GAP_START_BIMC_BOOST,		/* #0ms# */
  LTE_ML1_IRAT_L2PRS_INTRA_BEFORE_GAP_START_BIMC_BOOST,		/* #0ms# */
  LTE_ML1_IRAT_L2HRPD_BEFORE_GAP_START_BIMC_BOOST,			/* #0ms# */
  LTE_ML1_IRAT_DEFAULT_BIMC_BOOST,			        		/* #0ms# */		
  LTE_ML1_IRAT_WCDMA_CONN_BUILD_SCRIP_CPU_BOOST,    		/* #10ms# */
  LTE_ML1_TAM_QTA_SETUP_BIMC_AND_CPU_BOOST,		    		/* #40ms# */
  LTE_ML1_SLEEP_IDLE_WAKEUP_BIMC_BOOST,                                 /* #0ms# */
  /* #LTE_ML1_SCN_END_BOOST# */


  /* #LTE_ML1_SCN_START_MCVS_VOTE_Q6_DLM_CA# */
  LTE_ML1_DLM_CA_SCC_RF_TUNE_ON,    				  		/* #MCVS_TURBO# */
  LTE_ML1_DLM_CA_SCC_RF_TUNE_OFF,							/* #MCVS_OFF# */
  LTE_ML1_DLM_CA_SCC_ACT_DEACT_ON,							/* #MCVS_TURBO# */
  LTE_ML1_DLM_CA_SCC_ACT_DEACT_OFF,							/* #MCVS_OFF# */
  LTE_ML1_DLM_CA_SCC_DEL_ON, 								/* #MCVS_TURBO# */
  LTE_ML1_DLM_CA_SCC_ADD_ON, 								/* #MCVS_TURBO# */
  LTE_ML1_DLM_CA_ABORT_CC_STATE_Q6_OFF, 					/* #MCVS_OFF# */
  /* #LTE_ML1_SCN_END_MCVS_VOTE_Q6_DLM_CA# */
  
  /* #LTE_ML1_SCN_START_MCVS_VOTE_Q6_DLM_RX_CFG# */
  LTE_ML1_DLM_RX_CONFIG_ON,    								/* #MCVS_TURBO# */
  LTE_ML1_DLM_RX_CONFIG_OFF,								/* #MCVS_OFF# */
  /* #LTE_ML1_SCN_END_MCVS_VOTE_Q6_DLM_RX_CFG# */

  /* #LTE_ML1_SCN_START_MCVS_VOTE_Q6_PRS# */
  LTE_ML1_PRS_START_ON,							 			/* #MCVS_NOM# */
  LTE_ML1_PRS_START_OFF,									/* #MCVS_OFF# */
  /* #LTE_ML1_SCN_END_MCVS_VOTE_Q6_PRS# */
  
  /* #LTE_ML1_SCN_START_MCVS_VOTE_Q6_GAP# */
  LTE_ML1_SM_CONN_INTER_FREQ_BUILD_RF_SCRIPT_ON, 			/* #MCVS_NOM# */
  LTE_ML1_SM_CONN_INTER_FREQ_BUILD_RF_SCRIPT_OFF,			/* #MCVS_OFF# */
  LTE_ML1_IRAT_L2W_RX_CFG_BEFORE_GAP_END_ON,				/* #MCVS_NOM# */
  LTE_ML1_IRAT_L2W_RX_CFG_BEFORE_GAP_END_OFF,				/* #MCVS_OFF# */
  LTE_ML1_IRAT_L2T_RX_CFG_BEFORE_GAP_END_ON,				/* #MCVS_NOM# */
  LTE_ML1_IRAT_L2T_RX_CFG_BEFORE_GAP_END_OFF,				/* #MCVS_OFF# */
  LTE_ML1_IRAT_L21x_RX_CFG_BEFORE_GAP_END_ON,				/* #MCVS_NOM# */
  LTE_ML1_IRAT_L21x_RX_CFG_BEFORE_GAP_END_OFF,				/* #MCVS_OFF# */
  LTE_ML1_IRAT_L2PRS_INTER_RX_CFG_BEFORE_GAP_END_ON,		/* #MCVS_NOM# */
  LTE_ML1_IRAT_L2PRS_INTER_RX_CFG_BEFORE_GAP_END_OFF,		/* #MCVS_OFF# */
  LTE_ML1_IRAT_L2PRS_INTRA_RX_CFG_BEFORE_GAP_END_ON,		/* #MCVS_NOM# */
  LTE_ML1_IRAT_L2PRS_INTRA_RX_CFG_BEFORE_GAP_END_OFF,		/* #MCVS_OFF# */
  LTE_ML1_IRAT_L2HRPD_RX_CFG_BEFORE_GAP_END_ON,				/* #MCVS_NOM# */
  LTE_ML1_IRAT_L2HRPD_RX_CFG_BEFORE_GAP_END_OFF,			/* #MCVS_OFF# */
  LTE_ML1_IRAT_DEFAULT_ON,									/* #MCVS_NOM# */
  LTE_ML1_IRAT_DEFAULT_OFF,									/* #MCVS_OFF# */
  LTE_ML1_IRAT_GAP_ABORT_ON,								/* #MCVS_NOM# */
  LTE_ML1_IRAT_GAP_SPECIAL_ABORT_ON,						/* #MCVS_TURBO# */
  LTE_ML1_IRAT_GAP_ABORT_OFF,								/* #MCVS_OFF# */
  /* #LTE_ML1_SCN_END_MCVS_VOTE_Q6_GAP# */

  /* #LTE_ML1_SCN_START_MCVS_VOTE_Q6_AUTO_GAP# */
  LTE_ML1_SM_CONN_MEAS_AGAP_ON,								/* #MCVS_TURBO# */
  LTE_ML1_SM_CONN_MEAS_AGAP_OFF,                            /* #MCVS_OFF# */
  /* #LTE_ML1_SCN_END_MCVS_VOTE_Q6_AUTO_GAP# */

  /* #LTE_ML1_SCN_START_MCVS_VOTE_Q6_IRAT# */
  LTE_ML1_SM_CONN_GSM_L2G_MEASUREMENT_DRIVER_ON, 			/* #MCVS_NOM# */
  LTE_ML1_SM_CONN_GSM_L2G_MEASUREMENT_DRIVER_OFF,			/* #MCVS_OFF# */
  /* #LTE_ML1_SCN_END_MCVS_VOTE_Q6_IRAT# */

  /* #LTE_ML1_SCN_START_MCVS_VOTE_Q6_HO# */
  LTE_ML1_TO_MEET_HO_TIMELINE_ON,							/* #MCVS_TURBO# */
  LTE_ML1_TO_MEET_HO_TIMELINE_OFF,							/* #MCVS_OFF# */
  /* #LTE_ML1_SCN_END_MCVS_VOTE_Q6_HO# */

  /* #LTE_ML1_SCN_START_MCVS_VOTE_Q6_HO_RXD_ARD_MEAS# */
  LTE_ML1_HO_RXD_ARD_MEAS_ON, 								/* #MCVS_NOM# */
  LTE_ML1_HO_RXD_ARD_MEAS_OFF,   							/* #MCVS_OFF# */
  /* #LTE_ML1_SCN_END_MCVS_VOTE_Q6_HO_RXD_ARD_MEAS# */


  /* #LTE_ML1_SCN_START_MCVS_VOTE_VPE_DEACT_MEAS# */
  LTE_ML1_DEACT_SCC_MEAS_VPE_ON,							/* #MCVS_NOM# */
  LTE_ML1_DEACT_SCC_MEAS_RX_CONFIG_CNF_VPE_OFF,				/* #MCVS_OFF# */
  LTE_ML1_DEACT_SCC_MEAS_NB_SWITCH_CONFIG_CNF_VPE_OFF,		/* #MCVS_OFF# */
  LTE_ML1_DEACT_SCC_MEAS_VPE_OFF,							/* #MCVS_OFF# */
  /* #LTE_ML1_SCN_END_MCVS_VOTE_VPE_DEACT_MEAS# */
  
  /* #LTE_ML1_SCN_START_MCVS_VOTE_VPE_4RX# */
  LTE_ML1_DLM_HO_RXD_4RX_STATE_ON ,							/* #MCVS_TURBO# */
  LTE_ML1_DLM_HO_RXD_4RX_STATE_OFF,							/* #MCVS_OFF# */
  /* #LTE_ML1_SCN_END_MCVS_VOTE_VPE_4RX# */

  /* #LTE_ML1_SCN_START_MCVS_VOTE_VPE_EMBMS# */
  LTE_ML1_DLM_EMBMS_ENABLE_ON,								/* #MCVS_SVS# */
  LTE_ML1_DLM_EMBMS_ENABLE_OFF,								/* #MCVS_OFF# */
  /* #LTE_ML1_SCN_END_MCVS_VOTE_VPE_EMBMS# */

  /* #LTE_ML1_SCN_START_MCVS_VOTE_VPE_UL_TX# */
  LTE_ML1_UL_TX_ACTIVITY_ON,								/* #MCVS_SVS# */
  LTE_ML1_UL_TX_ACTIVITY_OFF,								/* #MCVS_OFF# */
  /* #LTE_ML1_SCN_END_MCVS_VOTE_VPE_UL_TX# */

  /* #LTE_ML1_SCN_START_MCVS_VOTE_VPE_GAP# */
  LTE_ML1_SM_CONN_MEAS_WCDMA_FULL_WINDOW_BEFORE_GAP_START_ON,/* #MCVS_NOM# */
  LTE_ML1_SM_CONN_MEAS_WCDMA_NASTT_BEFORE_GAP_START_ON,		/* #MCVS_NOM_PLUS# */
  LTE_ML1_SM_CONN_MEAS_WCDMA_BEFORE_GAP_START_OFF,	       	/* #MCVS_OFF# */
  /* #LTE_ML1_SCN_END_MCVS_VOTE_VPE_GAP# */

  /* #LTE_ML1_SCN_START_MCVS_VOTE_VPE_OFFLINE_FREQ_SRCH_MEAS# */
  LTE_ML1_OFFLINE_FREQ_SRCH_MEAS_ENABLE_ON, 				/* #MCVS_SVS# */
  LTE_ML1_OFFLINE_FREQ_SRCH_MEAS_ENABLE_OFF,				/* #MCVS_OFF# */
  /* #LTE_ML1_SCN_END_MCVS_VOTE_VPE_OFFLINE_FREQ_SRCH_MEAS# */

  /* #LTE_ML1_SCN_START_MCVS_VOTE_VPE_PARALLEL_SRCH# */
  LTE_ML1_PARALLEL_SEARCH_ENABLE_ON, 						/* #MCVS_SVS# */
  LTE_ML1_PARALLEL_SEARCH_ENABLE_OFF,				        /* #MCVS_OFF# */
  /* #LTE_ML1_SCN_END_MCVS_VOTE_VPE_PARALLEL_SRCH# */

  /* #LTE_ML1_SCN_START_MCVS_VOTE_VPE_INTRA_UL_CA# */
  LTE_ML1_INTRA_UL_CA_ON, 									/* #MCVS_NOM# */
  LTE_ML1_INTRA_UL_CA_OFF,									/* #MCVS_OFF# */
  /* #LTE_ML1_SCN_END_MCVS_VOTE_VPE_INTRA_UL_CA# */

  /* #LTE_ML1_SCN_START_MCVS_VOTE_VPE_CA_CONFIG_NOT_ACTIVE# */
  LTE_ML1_DLM_CA_HO_SCC_CLEANUP_ON, 						/* #MCVS_SVS# */
  LTE_ML1_DLM_CA_HO_SCC_CLEANUP_OFF,						/* #MCVS_OFF# */
  LTE_ML1_DLM_CA_SCC_DECONFIG_OFF,							/* #MCVS_OFF# */
  LTE_ML1_DLM_CA_ABORT_CC_STATE_VPE_OFF,                    /* #MCVS_OFF# */ 
  /* #LTE_ML1_SCN_END_MCVS_VOTE_VPE_CA_CONFIG_NOT_ACTIVE# */
  
  /* #LTE_ML1_SCN_START_MCVS_VOTE_VPE_SINGLE_RX# */
  LTE_ML1_RFMGR_SINGLE_RX_ENABLE_VPE_ON, 		            /* #MCVS_SVS# */ 
  LTE_ML1_RFMGR_SINGLE_RX_ENABLE_VPE_OFF,                   /* #MCVS_OFF# */ 
  /* #LTE_ML1_SCN_END_MCVS_VOTE_VPE_SINGLE_RX# */

  /* #LTE_ML1_SCN_START_MCVS_VOTE_VPE_BASE# */
  LTE_ML1_NOTIFY_GAP_OBJ_SCHED_ON,                          /* #MCVS_DYN# */ 
  LTE_ML1_NOTIFY_GAP_OBJ_DESCHED_ON,                        /* #MCVS_DYN# */ 
  LTE_ML1_NOTIFY_GAP_OBJ_START_ON,                          /* #MCVS_DYN# */ 
  LTE_ML1_NOTIFY_PAGE_OBJ_SCHED_ON,                         /* #MCVS_DYN# */ 
  LTE_ML1_NOTIFY_PAGE_OBJ_DESCHED_ON,                       /* #MCVS_DYN# */ 
  LTE_ML1_NOTIFY_PAGE_OBJ_START_ON,                         /* #MCVS_DYN# */ 
  LTE_ML1_NOTIFY_SIB_OBJ_SCHED_ON,                          /* #MCVS_DYN# */ 
  LTE_ML1_NOTIFY_SIB_OBJ_DESCHED_ON,                        /* #MCVS_DYN# */ 
  LTE_ML1_NOTIFY_SIB_OBJ_START_ON,                          /* #MCVS_DYN# */ 
  LTE_ML1_NOTIFY_TM8_ON,                                    /* #MCVS_DYN# */ 
  LTE_ML1_NOTIFY_TM8_OFF,                                   /* #MCVS_DYN# */ 
  LTE_ML1_NOTIFY_MIB_ACQ_STATE_ENTRY_ON,                    /* #MCVS_DYN# */ 
  LTE_ML1_NOTIFY_MIB_CSWITCH_RX_CFG_DONE_ON,                /* #MCVS_DYN# */ 
  LTE_ML1_NOTIFY_MIB_CSWITCH_TUNE_DONE_ON,                  /* #MCVS_DYN# */ 
  LTE_ML1_NOTIFY_MIB_PAGE_SHARE_ON,                         /* #MCVS_DYN# */ 
  LTE_ML1_NOTIFY_EVENT_CA_ACTIVATE_ON,                      /* #MCVS_DYN# */ 
  LTE_ML1_NOTIFY_EVENT_CA_DEACTIVATE_ON,                    /* #MCVS_DYN# */ 
  LTE_ML1_NOTIFY_EVENT_CONN_MODE_EXIT_ON,                   /* #MCVS_DYN# */ 
  LTE_ML1_NOTIFY_EVENT_CONN_MODE_ENTRY_ON,                  /* #MCVS_DYN# */ 
  LTE_ML1_NOTIFY_MCVS_INACTIVE_PRESCALE_DONE_ON,            /* #MCVS_DYN# */ 
  LTE_ML1_MCLK_CRS_IC_1_ON,                                 /* #MCVS_DYN# */
  LTE_ML1_MCLK_CRS_IC_2_ON,                                 /* #MCVS_DYN# */
  LTE_ML1_MCLK_CRS_IC_0_ON,                                 /* #MCVS_DYN# */
  /* #LTE_ML1_SCN_END_MCVS_VOTE_VPE_BASE# */

  /* #LTE_ML1_SCN_START_MCVS_VOTE_Q6_EFS# */
  LTE_ML1_EFS_Q6,                                            /* #MCVS_DYN# */ 
  /* #LTE_ML1_SCN_END_MCVS_VOTE_Q6_EFS# */

  /* #LTE_ML1_SCN_START_MCVS_VOTE_VPE_EFS# */
  LTE_ML1_EFS_VPE,                                           /* #MCVS_DYN# */ 
  /* #LTE_ML1_SCN_END_MCVS_VOTE_VPE_EFS# */

  /* #LTE_ML1_SCN_START_MCVS_VOTE_Q6_SLEEP_CDRX# */
  LTE_ML1_SLEEP_CDRX_ON,                                     /* #MCVS_OFF# */
  LTE_ML1_SLEEP_CDRX_OFF,                                    /* #MCVS_OFF# */
  /* #LTE_ML1_SCN_END_MCVS_VOTE_Q6_SLEEP_CDRX# */

  LTE_ML1_SCENARIO_MAX
  
} lte_ml1_scenarios_for_mcpm_e;

/*===========================================================================

  FUNCTION:  lte_ml1_sleepmgr_enable_disable_sleep_type

===========================================================================*/
/*!
    @brief
    API to enable/disable different type of sleep from L2, for low latency

    @details
    API to enable/disable different type of sleep. If enable flag is TRUE then 
    a certain type of sleep is enabled otherwise disabled. 
 
    @return
    none

*/
/*=========================================================================*/ 
void lte_ml1_sleepmgr_enable_disable_sleep_type
(
   lte_mem_instance_type        instance,
   boolean                      enable,
   lte_l1_sleepmgr_sleep_type_e sleep_mode
);

#endif /* LTE_ML1_EXT_API_H */

