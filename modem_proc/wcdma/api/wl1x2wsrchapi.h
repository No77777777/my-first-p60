#ifndef WL1X2WSRCHAPI_H
#define WL1X2WSRCHAPI_H

/*===========================================================================
                WL1 X2W SRCH APIs header file

DESCRIPTION
  This file is to export API for other tech who wants to search WCDMA
  

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None. All are passive inline function calling RF modules based on feature.
  All initialization requirment of respective RF modules should be followed

Copyright (c) 2006 - 2014 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: 
$DateTime: 
$Author: 

when       who     what, where, why
---------------------------------------------------------------------------------------------------------
11/02/17   rs      L2W in QTA support
09/22/15   us      Fixed compilation errors got after enabling ATLAS and RFM_PATH feature
08/24/15   us      Updates for including rfm_path_type
07/29/15   cc     Send RF EXIT CNF back to source tech
06/22/15   cc      L2W MSGR support
06/11/15   cc      Send PRE_SRCH_CNF for G2W
06/04/15   cc      Send WFW START before RF build script
06/03/15   svh     TA featurisation.
05/21/15   cc      Added G2W MSGR variant support
04/10/15   cc      Add G2W MSGR support
04/06/15   cc      G2W MSGR API check in
04/03/15   cc      Remove GERAN internal feature
02/19/15   ks      Expose API to GSM to init critical section
11/04/14   cc      Migrate a few missed changes from GL1 folder
07/11/14   cc      Fix a wrong type of cleanup_wcdma API 
06/24/14   cc      Moving G2W drivers to WL1 folders
04/27/14   cc      Initiate this file
===========================================================================*/

#include "wfw_sw_cmd_proc_intf.h"
#include "wcdma_msgr.h"
#include "uecomdef.h"

#include "rfm_meas.h"

#include "lte_l1_types.h"
#include "lm_types.h"

/* =======================================================================
**                        L2W Definitions
** ======================================================================= */
/*! Maximum detected cells per freq is 32*/
#define L2W_MAX_PSC_PER_FREQ  32

/*----------------------------------------------------------------------------*/
/*! @brief: Information pertaining to each WCDMA cell */
typedef struct
{
  /*!Scrambling code for this peak  Range 0..511*/
  uint16 psc;

  /* PN position In units of chipx8 Range 0...307200 (38400*8) */
  uint32 pn_pos;

} wl1_l2w_cell_info_s;

/*! @brief: WCDMA searches yield the psc's found along with the energy and
the pn_pos*/
typedef struct
{
  /*! Frequency */
  uint16                              freq;

  /*! Number of peaks found after the NASTT search Max would be 32*/
  uint16                              num_psc;

  /*! Results associated with each PSC  */
  wl1_l2w_cell_info_s                 psc_info[L2W_MAX_PSC_PER_FREQ];

  /*! */
  uint32                              energy[L2W_MAX_PSC_PER_FREQ];

  /*! Flag to indicate whether Step1 is pass or not*/
  boolean                             is_step1_pass;
} wl1_l2w_srch_results_s;

typedef struct
{
  /*! Results pertaining to each cell */
  wl1_l2w_cell_info_s               cell_info;

  /*********************************************************************
  ** Units of the three items below (RSSI, RSCP, Ec/Io) are two times **
  ** the actual dBm value, giving a resolution for each of 0.5 dBm.   **
  *********************************************************************/

  /*!The received wide band power, including thermal noise and
  noise generated in the receiver,within the bandwidth defined
  by the receiver pulse shaping filter Ref 3gpp 25.215*/
  int16                             cpich_rssi;

  /*!Received Signal Code Power, the received power on one code
  measured on the Primary CPICH. Ref 3gpp 25.215*/
  int16                             cpich_rscp;

  /*!The received energy per chip divided by the power density
  in the band. The CPICH Ec/No is identical to CPICH RSCP/UTRA
  Carrier RSSI. Measurement shall be performed on the Primary
  CPICH. Ref : 3gpp spec 25.215*/
  int16                             cpich_ecno;
} wl1_l2w_cell_meas_info_s;

typedef struct
{
  /*! UARFCN */
  uint16  freq;
  /*!Number of cells for which measurement results are available */
  uint16  num_cells ;
  /*! Results for each of the cell  */
  wl1_l2w_cell_meas_info_s meas_cell_res[L2W_MAX_PSC_PER_FREQ];

} wl1_l2w_meas_results_s;

/*-----------------------------------------------------------------------------*/
/* Call backs   */

/*! Function pointer to indicate the search results */
typedef void (*wl1_l2w_nastt_srch_done_cb)(wl1_l2w_srch_results_s *);

/*! Function pointer to indicate the meas results*/
typedef void (*wl1_l2w_pn_srch_done_cb)(wl1_l2w_meas_results_s *);

/*! Function pointer for confirming the abort */
typedef void (*wl1_l2w_abort_srch_meas_cnf_cb)(void);

/*! Function pointer for confirming the abort */
typedef void (*wl1_l2w_cleanup_done_cnf_cb)(void);

/*! Function pointer for confirming the abort */
typedef void (*wl1_l2w_rf_script_build_done_cnf_cb)(boolean);

/*! Function pointer for confirming the abort */
typedef void (*wl1_l2w_wfw_start_cnf_cb)(void);
/* used to let l2w drivers know what the current multisim state is */
typedef enum
{
  /* only one sim being used */
  L2W_SINGLE_SIM,
  /* dual sim dual standby */
  L2W_DSDS,
  /* dual sim dual active */
  L2W_DSDA,
} wl1_l2w_multisim_state_e;

/*----------------------------------------------------------------------------*/
/*! @brief: Information pertaining to each WCDMA UARFCN. Gives the number
of detected cells in the frequency */
typedef struct
{
  /*! Frequency */
  uint16                              freq;

  /*! Number of peaks found after the NASTT search Max would be 32*/
  uint16                              num_psc;

  /*! Results associated with each PSC  */
  wl1_l2w_cell_info_s                 psc_info[L2W_MAX_PSC_PER_FREQ];

  /* W frequency used in rf enter and build script */
  uint16 tuned_w_freq;

  #if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
  /* W RF path for L2W measurment */
  rfm_path_type wcdma_path;
  #else
  /* W RF device for L2W measurment */
  rfm_device_enum_type wcdma_device;
  #endif

  /* Unique transaction id for this sequence */
  uint32                           l2w_trm_transaction_id;

  /*! Whether TRM has been requested for this measurement */
  boolean trm_requested;
} wl1_l2w_freq_info_s;

/* =======================================================================
**                        Macros
** ======================================================================= */

#define WL1_X2W_SRCH_MAX_PSC_PER_FREQ  32

#define WL1_X2W_SRCH_DED_MAX_PSC_PER_FREQ 8

/* =======================================================================
**                        Function Declarations
** ======================================================================= */

typedef enum 
{
  /* an ID search that blindly looks for WCDMA cells (aka PSCs to find aren’t provided) */
  WL1_X2W_IDLE_ID_SRCH,
  /* a reconfirm search that is provided PSCs with locations and returns the latest position/power of those PSCs */
  WL1_X2W_IDLE_RECONF_SRCH,
  /* an ID/Reconfirm search that performs ID searches for a list of PSCs with unknown position AND performs reconfirm measurements for a list of PSCs with known positions */
  WL1_X2W_DEDICATED_SRCH,
  /* an ID search for dedicated mode that blindly looks for WCDMA cells (aka PSCs to find aren’t provided) */
  WL1_X2W_DEDICATED_ID_SRCH,
} wl1_x2w_mode_enum_type;

typedef enum
{
  /* IRAT G2W Idle Mode */
  WL1_X2W_SRCH_G2W_IDLE_MODE = WFW_IRAT_G2W_IDLE_MODE,
  /* IRAT G2W Dedicated Mode */
  WL1_X2W_SRCH_G2W_DEDICATED_MODE = WFW_IRAT_G2W_DEDICATED_MODE,
  WL1_X2W_SRCH_L2W_IDLE_MODE = WFW_IRAT_L2W_IDLE_MODE,
  WL1_X2W_SRCH_L2W_IDLE_RECONF_MODE = WFW_IRAT_L2W_IDLE_RECONF_MODE,
  WL1_X2W_SRCH_L2W_DEDICATED_MODE = WFW_IRAT_L2W_DEDICATED_MODE,
  WL1_X2W_SRCH_L2W_DEDICATED_CELL_ID_MODE = WFW_IRAT_L2W_DEDICATED_CELL_ID_MODE
} wl1_x2w_srch_wfw_mode_enum_type;

typedef struct
{
  /* RF action time in USTMR/xo  */
  uint32 rf_action_time;
  uint32 gap_start_time;
  uint32 gap_end_time;
  /* Freq error in Q6 Hz format */
  int32 freq_err_q6_hz;
  /* Inverse WCDMA DL freq */
  uint32 inv_dlo_freq;
} wl1_x2w_startup_params_struct_type;

typedef struct
{
  uint32 startup_time;
  uint32 cleanup_time;
  uint16 pll_settling_time;
} wl1_x2w_rf_timing_params_struct_type;

/* ================== Init Request ==================== */

typedef struct
{
  msgr_hdr_struct_type hdr;

  /* The device multi-sim operating mode */
  sys_modem_device_mode_e_type  multi_sim_mode;
} wl1_irat_x2w_init_req_struct_type;

typedef struct
{
  msgr_hdr_struct_type hdr;
  
  wl1_x2w_rf_timing_params_struct_type rf_timing_params;

} wl1_irat_x2w_init_cnf_struct_type;

/* ================= Search Request ====================*/

/* structure used to describe a cell associated with some cx8 position */
typedef struct
{
  /* primary scrambling code of cell */
  uint16 psc;

  /* cx8 position of cell */
  uint32 pos;
} wl1_x2w_located_cell_struct_type;

/* describes what cells to search for when performing an idle reconfirm search */
typedef struct
{
  /* Specifically used for L2W search, ML1 decides whether to trigger PN after NASTT
     This flag to differeniate a PN-After-NASTT or RECONF
     Flag is set for PN-After-NASTT, otherwise RECONF */
  boolean                                use_frozen_sample;

  /* how many cells to reconfirm */
  uint8                                  num_cells;

  /* the list of cells to reconfirm */
  wl1_x2w_located_cell_struct_type       cells[WL1_X2W_SRCH_MAX_PSC_PER_FREQ];
  
} wl1_x2w_idle_reconf_input_struct_type;

/* describes what cells to search for when performing a dedicated (id and/or reconfirm) search */
typedef struct
{
  /* Fresh ID search */
  boolean                                first_id_srch;
  /* how many cells for ID search */
  uint8                                  num_id_cells;

  /* the list of pscs to ID */
  uint16                                 id_cell_pscs[WL1_X2W_SRCH_DED_MAX_PSC_PER_FREQ];

  /* how many cells to reconfirm */
  uint8                                  num_reconf_cells;

  /* the list of cells to reconfirm */
  wl1_x2w_located_cell_struct_type       reconf_cells[WL1_X2W_SRCH_DED_MAX_PSC_PER_FREQ];
} wl1_x2w_dedicated_input_struct_type;

/* describes what cells to search for when performing a dedicated (id and/or reconfirm) search */
typedef struct
{
  /* Indication the W driver that this is the first task and
  subsequent ones results will all be accumulated in the fw */
  boolean first_task;

  /* Indication that Step1 searching should end with this.
  Additionally W driver should program the FW to start performing
  the NASTT search once the Step1 results arrive */
  boolean last_task;

  /* Indication if this is a nastt search */
  boolean nastt_search;

} wl1_x2w_dedicated_id_input_struct_type;

/* a union of the various ways to provide WCDMA cells to a search */
typedef union
{
  wl1_x2w_idle_reconf_input_struct_type  idle_reconf;
  wl1_x2w_dedicated_input_struct_type    dedicated;
  wl1_x2w_dedicated_id_input_struct_type dedicated_id;
} wl1_x2w_srch_input_union_type;

typedef struct
{
  msgr_hdr_struct_type hdr;

  /* Idle ID | Idle Reconf | Dedicated | Dedicated ID */
  wl1_x2w_mode_enum_type                        mode;

  /* WCDMA frequency to search */
  uint16                                        uarfcn;

  /* union of ways for client to provide WCDMA cells to search based on mode */
  wl1_x2w_srch_input_union_type                 srch;

  /* Search start up parameters (parameters explaining how and when to tune) */
  wl1_x2w_startup_params_struct_type            startup_params;

  rfm_meas_common_param_type                    *rf_params;
  
} wl1_irat_x2w_srch_req_struct_type;

/* ================= Search Results ====================*/

/* structure used to describe a single cell measurement */
typedef struct 
{
  uint16 psc;
  uint32 pos;
  uint32 eng;
  int16  rscp;
  int16  ecio;
} wl1_x2w_measured_cell_type;

/* describes what cells were found when performing an idle id search */
typedef struct
{
  boolean nastt_retrigger;
  uint8   num_cells;
  wl1_x2w_measured_cell_type cells[WL1_X2W_SRCH_MAX_PSC_PER_FREQ];
} wl1_x2w_idle_id_result_struct_type;

/* describes what cells were found when performing an idle reconf search */
/* (idle reconf and idle id both return results in the same way, so just make them use the same structure) */
typedef wl1_x2w_idle_id_result_struct_type wl1_x2w_idle_reconf_result_struct_type;

/* (Dedicated id and idle id both return results in the same way, so just make them use the same structure) */
typedef wl1_x2w_idle_id_result_struct_type wl1_x2w_dedicated_id_result_struct_type;

/* describes what cells were found when performing a dedicated (ID and/or Reconf) search */
typedef struct
{
  uint8 num_id_cells;
  wl1_x2w_measured_cell_type id_cells[WL1_X2W_SRCH_DED_MAX_PSC_PER_FREQ];
  uint8 num_reconf_cells;
  wl1_x2w_measured_cell_type reconf_cells[WL1_X2W_SRCH_DED_MAX_PSC_PER_FREQ];
} wl1_x2w_dedicated_result_struct_type;

/* a union of the various ways to provide search results back to the client */
typedef union
{
  wl1_x2w_idle_id_result_struct_type        idle_id;
  wl1_x2w_idle_reconf_result_struct_type    idle_reconf;
  wl1_x2w_dedicated_result_struct_type      dedicated;
  wl1_x2w_dedicated_id_result_struct_type   dedicated_id;
} wl1_x2w_srch_result_union_type;

typedef struct
{
  msgr_hdr_struct_type hdr;

  /* Idle ID | Idle Reconf | Dedicated | Dedicated ID */
  wl1_x2w_mode_enum_type                        mode;

  /* Skip gap if it's too late for sending MODE_RF_CFG_CMD for gap preload  */
  boolean                                       srch_allowed;

  /* WCDMA frequency that was searched */
  uint16                                        uarfcn;

  /* the received signal strength indicator for the search */
  int16                                         rssi;

  /* union of ways for driver to provide search results based on mode */
  wl1_x2w_srch_result_union_type                result;

} wl1_irat_x2w_srch_cnf_struct_type;

typedef struct
{
  msgr_hdr_struct_type hdr;

} wl1_irat_x2w_abort_req_struct_type;

typedef struct
{
  msgr_hdr_struct_type hdr;

} wl1_irat_x2w_abort_cnf_struct_type;

typedef struct
{
  msgr_hdr_struct_type hdr;

} wl1_irat_x2w_deinit_req_struct_type;

typedef struct
{
  msgr_hdr_struct_type hdr;

} wl1_irat_x2w_deinit_cnf_struct_type;

typedef struct
{
  msgr_hdr_struct_type hdr;
  
  rfm_meas_common_param_type *rf_params;
   
  /* W Band type */
  uint16               w_freq;
  /* RxLM buffer Index */
  lm_handle_type       w_rxlm_buf_idx;

  #if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED) && defined(FEATURE_WCDMA_RF_PATH_SUPPORT_IN_CXM)
  /* RF Path IDs */
  rfm_path_type w_rf_path;
  #else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED && (FEATURE_WCDMA_RF_PATH_SUPPORT_IN_CXM) */
  /* RF Device IDs */
  rfm_device_enum_type w_rf_device;
  #endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED && (FEATURE_WCDMA_RF_PATH_SUPPORT_IN_CXM) */
  /* AS ID */
  sys_modem_as_id_e_type as_id;

} wl1_irat_x2w_rf_enter_ind_struct_type;

typedef struct
{
  msgr_hdr_struct_type hdr;
  
  rfm_meas_common_param_type *rf_params;
   
  /* For calling exit only, gap aborted if script built */
  boolean gap_aborted;
  /* AS ID */
  sys_modem_as_id_e_type as_id;

} wl1_irat_x2w_rf_exit_ind_struct_type;

typedef struct
{
  msgr_hdr_struct_type hdr;
  
  rfm_meas_common_param_type *rf_params;
   
  /* For calling exit only, gap aborted if script built */
  boolean gap_aborted;
  /* AS ID */
  sys_modem_as_id_e_type as_id;

} wl1_irat_x2w_rf_exit_req_struct_type;

typedef struct
{
  msgr_hdr_struct_type hdr;

} wl1_irat_x2w_rf_exit_cnf_struct_type;

typedef struct
{
  msgr_hdr_struct_type hdr;
  /* Source tech to indicate if this X2W measurement is in a QTA gap */
  boolean in_qta_gap;
  /* RF Common pointer */
  rfm_meas_common_param_type *rf_params;

} wl1_irat_x2w_pre_srch_req_struct_type;

typedef struct
{
  msgr_hdr_struct_type hdr;

} wl1_irat_x2w_pre_srch_cnf_struct_type;

typedef struct
{
  msgr_hdr_struct_type hdr;

} wl1_irat_x2w_online_srch_done_ind_struct_type;

typedef struct
{
  msgr_hdr_struct_type hdr;
  /* RF Common pointer */
  rfm_meas_common_param_type *rf_params;

} wl1_irat_x2w_build_script_req_struct_type;

typedef struct
{
  msgr_hdr_struct_type hdr;

} wl1_irat_x2w_build_script_cnf_struct_type;

enum
{
  /* Define UMIDs */ 
  /* Odd number for G2W; Even number for L2W */
  /* Must follow the above rule to print the right F3 */

  MSGR_DEFINE_UMID(WCDMA, IRAT, REQ, G2W_INIT, 0x0, 
                   wl1_irat_x2w_init_req_struct_type),
  MSGR_DEFINE_UMID(WCDMA, IRAT, CNF, G2W_INIT, 0x0,
                   wl1_irat_x2w_init_cnf_struct_type),
  
  MSGR_DEFINE_UMID(WCDMA, IRAT, REQ, L2W_INIT, 0x1, 
                   wl1_irat_x2w_init_req_struct_type),
  MSGR_DEFINE_UMID(WCDMA, IRAT, CNF, L2W_INIT, 0x1,
                   wl1_irat_x2w_init_cnf_struct_type),

  MSGR_DEFINE_UMID(WCDMA, IRAT, IND, G2W_RF_ENTER, 0x2, 
                   wl1_irat_x2w_rf_enter_ind_struct_type),

  MSGR_DEFINE_UMID(WCDMA, IRAT, IND, L2W_RF_ENTER, 0x3, 
                   wl1_irat_x2w_rf_enter_ind_struct_type),

  MSGR_DEFINE_UMID(WCDMA, IRAT, REQ, G2W_PRE_SRCH, 0x4, 
                   wl1_irat_x2w_pre_srch_req_struct_type),
  MSGR_DEFINE_UMID(WCDMA, IRAT, CNF, G2W_PRE_SRCH, 0x4, 
                   wl1_irat_x2w_pre_srch_cnf_struct_type),

  MSGR_DEFINE_UMID(WCDMA, IRAT, REQ, L2W_PRE_SRCH, 0x5, 
                   wl1_irat_x2w_pre_srch_req_struct_type),
  MSGR_DEFINE_UMID(WCDMA, IRAT, CNF, L2W_PRE_SRCH, 0x5, 
                   wl1_irat_x2w_pre_srch_cnf_struct_type),

  MSGR_DEFINE_UMID(WCDMA, IRAT, REQ, G2W_SRCH, 0x6, 
                   wl1_irat_x2w_srch_req_struct_type),
  MSGR_DEFINE_UMID(WCDMA, IRAT, CNF, G2W_SRCH, 0x6, 
                   wl1_irat_x2w_srch_cnf_struct_type),

  MSGR_DEFINE_UMID(WCDMA, IRAT, REQ, L2W_SRCH, 0x7, 
                   wl1_irat_x2w_srch_req_struct_type),
  MSGR_DEFINE_UMID(WCDMA, IRAT, CNF, L2W_SRCH, 0x7, 
                   wl1_irat_x2w_srch_cnf_struct_type),

  MSGR_DEFINE_UMID(WCDMA, IRAT, IND, G2W_RF_EXIT, 0x8, 
                   wl1_irat_x2w_rf_exit_ind_struct_type),
  MSGR_DEFINE_UMID(WCDMA, IRAT, REQ, G2W_RF_EXIT, 0x8, 
                   wl1_irat_x2w_rf_exit_req_struct_type),
  MSGR_DEFINE_UMID(WCDMA, IRAT, CNF, G2W_RF_EXIT, 0x8, 
                   wl1_irat_x2w_rf_exit_cnf_struct_type),

  MSGR_DEFINE_UMID(WCDMA, IRAT, IND, L2W_RF_EXIT, 0x9, 
                   wl1_irat_x2w_rf_exit_ind_struct_type),
  MSGR_DEFINE_UMID(WCDMA, IRAT, REQ, L2W_RF_EXIT, 0x9, 
                   wl1_irat_x2w_rf_exit_req_struct_type),
  MSGR_DEFINE_UMID(WCDMA, IRAT, CNF, L2W_RF_EXIT, 0x9, 
                   wl1_irat_x2w_rf_exit_cnf_struct_type),

  MSGR_DEFINE_UMID(WCDMA, IRAT, REQ, G2W_ABORT, 0xA, 
                   wl1_irat_x2w_abort_req_struct_type),
  MSGR_DEFINE_UMID(WCDMA, IRAT, CNF, G2W_ABORT, 0xA, 
                   wl1_irat_x2w_abort_cnf_struct_type),

  MSGR_DEFINE_UMID(WCDMA, IRAT, REQ, L2W_ABORT, 0xB, 
                   wl1_irat_x2w_abort_req_struct_type),
  MSGR_DEFINE_UMID(WCDMA, IRAT, CNF, L2W_ABORT, 0xB, 
                   wl1_irat_x2w_abort_cnf_struct_type),

  MSGR_DEFINE_UMID(WCDMA, IRAT, REQ, G2W_DEINIT, 0xC, 
                   wl1_irat_x2w_deinit_req_struct_type),
  MSGR_DEFINE_UMID(WCDMA, IRAT, CNF, G2W_DEINIT, 0xC, 
                   wl1_irat_x2w_deinit_cnf_struct_type),

  MSGR_DEFINE_UMID(WCDMA, IRAT, REQ, L2W_DEINIT, 0xD, 
                   wl1_irat_x2w_deinit_req_struct_type),
  MSGR_DEFINE_UMID(WCDMA, IRAT, CNF, L2W_DEINIT, 0xD, 
                   wl1_irat_x2w_deinit_cnf_struct_type),

  MSGR_DEFINE_UMID(WCDMA, IRAT, IND, L2W_ONLINE_SRCH_DONE, 0xF, 
                   wl1_irat_x2w_online_srch_done_ind_struct_type),

  MSGR_DEFINE_UMID(WCDMA, IRAT, REQ, G2W_BUILD_SCRIPT, 0x10, 
                   wl1_irat_x2w_build_script_req_struct_type),
  MSGR_DEFINE_UMID(WCDMA, IRAT, CNF, G2W_BUILD_SCRIPT, 0x10, 
                   wl1_irat_x2w_build_script_cnf_struct_type),

  MSGR_DEFINE_UMID(WCDMA, IRAT, REQ, L2W_BUILD_SCRIPT, 0x11, 
                   wl1_irat_x2w_build_script_req_struct_type),
  MSGR_DEFINE_UMID(WCDMA, IRAT, CNF, L2W_BUILD_SCRIPT, 0x11, 
                   wl1_irat_x2w_build_script_cnf_struct_type)
};

/*===========================================================================
                         VARIABLES
===========================================================================*/

/*===========================================================================
                        Extern Functions
===========================================================================*/

/*=========================================================================

FUNCTION      WL1_X2W_SCRIPT_ENTER

DESCRIPTION   This function calls rfm_enter with the parameters passed in.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
==========================================================================*/
extern void wl1_x2w_rf_script_enter(wl1_irat_x2w_rf_enter_ind_struct_type *rf_enter_params);

/*=========================================================================

FUNCTION      WL1_X2W_SCRIPT_EXIT

DESCRIPTION   This function calls rfm_enter with the parameters passed in.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
==========================================================================*/
void wl1_x2w_rf_script_exit(wl1_irat_x2w_rf_exit_ind_struct_type *rf_exit_params);

/*===========================================================================

  FUNCTION:   wl1_l2w_config_deinit

===========================================================================*/
/*!
    @brief
    This function calls the wcdma driver to do a
    wcdma app disable

    @detail

    On receiving this function call
    This function calls the wcdma driver to do a
    wcdma app disable

@return


*/
/*=========================================================================*/
void wl1_l2w_config_deinit( sys_modem_as_id_e_type as_id);

/*===========================================================================

  FUNCTION:   wl1_l2w_conn_full_window_srch_req

===========================================================================*/
/*!
    @brief
    This function calFull window search method for performing
    searches in connected mode

    @detail

    On receiving this function call
    1. Provides the ref count when the WFW should tune to the W freq
    2. The W driver would maintain the internal state until the entire
    search is completed.

    @return


*/
/*=========================================================================*/
void wl1_l2w_conn_full_window_srch_req
(
  /*! Cells for which measurements need to be made. */
  wl1_l2w_freq_info_s  *freq_cell_list_ptr,
  int32 wfw_freq_err,
  lte_l1_stmr_w_s wstmr_rf_tune,
  sys_modem_as_id_e_type as_id);

/*===========================================================================

  FUNCTION:   wl1_l2w_conn_step1_nastt_srch_req

===========================================================================*/
/*!
    @brief
    This function calls the WCDMA driver to perform the Step 1search
    followed by NASTT in connected mode
    for the detected cells

    @detail

    On receiving this function call
    1. Provides the ref count when the WFW should tune to the W freq
    2. The W driver would maintain the internal state until the entire
    search is completed.

    @return


*/
/*=========================================================================*/
void wl1_l2w_conn_step1_nastt_srch_req
(
  /* Freq on which the step1 search has to be performed */
  uint16 freq,
  /* Indication the W driver that this is the first task and
  subsequent ones results will all be accumulated in the fw */
  boolean first_task,
  /* Indication that Step1 searching should end with this.
  Additionally W driver should program the FW to start performing
  the NASTT search once the Step1 results arrive */
  boolean last_task,
  int32 wfw_freq_err,
  lte_l1_stmr_w_s wstmr_rf_tune,
  sys_modem_as_id_e_type as_id);

/*===========================================================================

  FUNCTION:  wl1_l2w_config_init

===========================================================================*/
/*!
    @brief
    Initializes the configuration so that W srch meas can be performed

    @detail
    1. Initializes the WFW
    2. Initializes the W clocks etc.
    3. Sets up the interrupt handler to receive commands from the WFW
    4. This will be called every time LTE starts

    @return

*/
/*=========================================================================*/
void wl1_l2w_config_init
(
  /*! FTM mode flag in L2W */
  boolean is_ftm_mode
, sys_modem_as_id_e_type as_id);

/*===========================================================================

  FUNCTION:  wl1_l2w_register_callbacks

===========================================================================*/
/*!
    @brief
    Initializes the configuration so that W srch meas can be performed

    @detail
    1. Register the call backs for processing the WCDMA search, measurement
    results, abort confirm etc
    This will be called once at boot up time.

    @return

*/
/*=========================================================================*/
void wl1_l2w_register_callbacks(
  /*Pass in the function pointers which will be registered by the driver.
  These call backs would be called at appropriate times by the W driver*/
  wl1_l2w_nastt_srch_done_cb            nastt_done_ptr,
  wl1_l2w_pn_srch_done_cb               pn_srch_ptr,
  wl1_l2w_abort_srch_meas_cnf_cb        abort_cnf_ptr,
  wl1_l2w_cleanup_done_cnf_cb           cleanup_cnf_ptr,
  wl1_l2w_rf_script_build_done_cnf_cb   rf_script_build_done_ptr,
  sys_modem_as_id_e_type as_id);

/*===========================================================================

  FUNCTION:  wl1_l2w_abort_srch_meas

===========================================================================*/
/*!
    @brief
    Abort any ongoing search or measurements

    @detail
    This function will be called by ML1 to abort ongoing WCDMA searches
    and measurements. Once WCDMA completes the abort procedure it would
    send ML1 an Abort Confirm.

    @return
    None

*/
/*=========================================================================*/
void wl1_l2w_abort_srch_meas( sys_modem_as_id_e_type as_id);

/*===========================================================================

  FUNCTION:   wl1_l2w_driver_get_l2w_handle

===========================================================================*/
/*!
    @brief
    Returns the L2W handle

*/
/*=========================================================================*/
lm_handle_type wl1_l2w_driver_get_l2w_handle ( sys_modem_as_id_e_type as_id);

/*===========================================================================

  FUNCTION:  wl1_l2w_cell_id_srch_req

===========================================================================*/
/*!
    @brief
    This function programs the WFW to perform the cell id searches
    on the frequency requested.

    @detail
    This request would be sent when ML1 is initiating a cell identification
    search with WCDMA.
    The WCDMA driver would
    1. Tune the RF to WCDMA
    2. Program the WFW to perform Step1 search
    3. Perform the Step1 peak elimination - eliminate peaks that are within
    32 chipx8 of the detected cells.
    4. Program the W-FW for the NASTT search based on the samples captured
    in Step1

    Note: This is a direct function call from ML1 to WCDMA Driver

    @return
    None

    Note
    The results are obtained later in an interrupt context.

*/
/*=========================================================================*/
void wl1_l2w_cell_id_srch_req(
  /*! lets us know if ue is SS, DSDS, or DSDA */
  wl1_l2w_multisim_state_e multisim_state,
  /*! Frequency on which the cell id search has to be performed */
  /*!Previously detected cells on that frequency if any. These previously
  detected cells help in WCDMA driver performing the Step1 peak elimination*/
  wl1_l2w_freq_info_s       *freq_info_ptr,
  int32 wfw_freq_err,
  sys_modem_as_id_e_type as_id);

/*===========================================================================

  FUNCTION:  wl1_l2w_pn_srch_req

===========================================================================*/
/*!
    @brief
    This request is sent when measurements have to be performed on detected
    cells.

    @detail
    This function is called in the following cases
    1. In the same DRX cycle there is both a  search command and a measure
    command is issued. This is when the search yielded some new cells and
    measurements have to be performed on them. Or possibly the
    search and measurements for the frequency coincided. In this scenario
    since the RF was already tuned when the cell id request was made the
    tune_wcdma_rf would be set to FALSE.
    2. In another scenario, this function is called in the case when only
    measurements have to be performed. The cell list will contain the list
    of cells for which  measurements have to be done. In this case the
    tune_wcdma_rf is set to TRUE indicating to the WCDMA driver that
    RF has to be tuned.

    @return

    None

    Note
    The results are obtained later in an interrupt context.
*/
/*=========================================================================*/
void wl1_l2w_pn_srch_req(
  /*! lets us know if ue is SS, DSDS, or DSDA */
  wl1_l2w_multisim_state_e multisim_state,
  /*! TRUE indicates that RF has to be tuned. It will be in the case
  when only the measurements have to be done and there was no
  search prior to this.*/
  boolean                             tune_wcdma_rf,
  /*! Cells for which measurements need to be made. */
  wl1_l2w_freq_info_s        *freq_cell_list_ptr,
  int32 wfw_freq_err,
  sys_modem_as_id_e_type as_id);

/*===========================================================================

  FUNCTION:   wl1_l2w_conn_pn_srch_req

===========================================================================*/
/*!
    @brief
    This function calFull window search method for performing
    searches in connected mode

    @detail

    On receiving this function call
    0. WFW would call the build scripts command to build the L->W scripts
    1. Provides the ref count when the WFW should tune to the W freq
    2. The W driver would maintain the internal state until the entire
    search is completed.

    @return


*/
/*=========================================================================*/
void wl1_l2w_conn_pn_srch_req(
  /*! Cells for which measurements need to be made. */
  wl1_l2w_freq_info_s  *freq_cell_list_ptr,
  int32 wfw_freq_err,
  lte_l1_stmr_w_s wstmr_rf_tune,
  sys_modem_as_id_e_type as_id);

/*===========================================================================

  FUNCTION:   wl1_l2w_initiate_conn_search

===========================================================================*/
/*!
    @brief
    This function calls the WCDMA driver to perform the Step 1search
    followed by NASTT in connected mode
    for the detected cells

    @detail

    @return
*/
/*=========================================================================*/
void wl1_l2w_initiate_conn_search(
  /*! lets us know if ue is SS, DSDS, or DSDA */
  wl1_l2w_multisim_state_e multisim_state,
  sys_modem_as_id_e_type as_id);

/*===========================================================================

  FUNCTION: wl1_l2w_cleanup

===========================================================================*/
/*!
    @brief
    This is sent from the LTE ML1 to WCDMA Driver to indicate that all the
    measurements are done and WCDMA driver should now clean up the WCDMA FW.

    @detail
    This is sent from the LTE ML1 to WCDMA Driver to indicate that all the
    measurements are done and WCDMA driver should now clean up the WCDMA FW.

    @return
    None

*/
/*=========================================================================*/
void wl1_l2w_cleanup( sys_modem_as_id_e_type as_id);

/*===========================================================================

  FUNCTION: wl1_l2w_driver_send_panic_cmd

===========================================================================*/
/*!
    @brief
    This function sends panic command to FW. It is used for breaking the fw
    to capture sample dumps.

    The fuction should only be used for debugging purpose

    @return
    None.
*/
/*=========================================================================*/
void wl1_l2w_driver_send_panic_cmd( sys_modem_as_id_e_type as_id);

/*===========================================================================

  FUNCTION: wl1_l2w_build_rf_scripts

===========================================================================*/
/*!
    @brief
    This function is called to build L2W RF scripts. If we are in connected
    mode RF script info will be saved and scripts will be built in the DPC
    task. Otherwise scripts are build in calling tasks context.

    @return
    None.
*/
/*=========================================================================*/
void wl1_l2w_build_rf_scripts(
  rfm_meas_common_param_type* common_ptr,
  boolean build_script_in_offload_thread,
  sys_modem_as_id_e_type as_id);


#endif