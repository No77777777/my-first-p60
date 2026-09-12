#ifndef TDSRRCLOGGING_H
#define TDSRRCLOGGING_H
/*===========================================================================
                          R R C L O G G I N G

DESCRIPTION
  This file defines the logging interface between RRC and the tools.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrclogging.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/15/12   gv      Added support to log new event when complete SIB is received
10/13/11   mkh     Added log version to RRC log packets
09/15/11   mkh     Putting back the mistakenly overwritten code
09/08/11   mkh     Events renamed to TDS
07/07/11   mkh     Renamed log pkts to TDSxxx. 
                   Added version filed to log pkts. 
                   Modified comments to support Doxygen style.
                   Removed usage of LOG_RECORD_DEFINE macro.
                   Removed using PACK, using reserved bits to align for 32 bits.
                   Cleaned up log pkts not in use. 
03/10/11   ttl     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"
#include "log.h"
#include "tdsrrcmm_v.h"
#include "event.h"
#include "tdscdmaselfhosting.h"
#include "log_codes_tdscdma.h"
#include "tdsl1rrcif.h"
#include "tdsrrccspdb.h"

/* Set one-byte structure packing for Windows targets. The packing for ARM
 * targets is specified by use of the PACKED directive on structure
 * definitions.
 */
#ifdef T_WINNT
  #error code not present
#endif


/*-----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* Log version for RRC */
#define TDSRRC_LOG_VERSION   1

/*! Defines the RRC states. These defines are used in log packet 
    0xD0E0 to set the rrc_state field.
 */
#define TDSRRCLOG_STATE_DISCONNECTED  0      /*!< \brief Disconnected State (Idle Mode) */
#define TDSRRCLOG_STATE_CONNECTING    1      /*!< \brief Connecting State (Idle Mode) */
#define TDSRRCLOG_STATE_CELL_FACH     2      /*!< \brief CELL_FACH State (Connected Mode) */
#define TDSRRCLOG_STATE_CELL_DCH      3      /*!< \brief CELL_DCH State (Connected Mode) */
#define TDSRRCLOG_STATE_CELL_PCH      4      /*!< \brief CELL_PCH State (Connected Mode) */
#define TDSRRCLOG_STATE_URA_PCH       5      /*!< \brief URA_PCH State (Connected Mode) */

#define TDSRRC_LOG_MAX_PFR_FREQ       9      /*!< \brief The maximum number of Pseudo Fast Return frequencies */

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

typedef enum{
  TDSRL_FAILURE_DUE_TO_L1=0,
  TDSRL_FAILURE_DUE_TO_RRC
}tdsrrc_rl_failure_cause_e_type;

typedef enum
{
  LTA,
  QTA
}tdsrrc_tuneaway_type;



typedef struct
{  
  uint8 current_rrc_state;                       /*!< \brief Current RRC state 
                                                                                      -           0 – Disconnected
                                                                                      -           1 – Connecting
                                                                                      -           2 – CELL_FACH
                                                                                      -           3 – CELL_DCH
                                                                                      -           4 – CELL_PCH
                                                                                      -           5 – URA_PCH  */

  uint8 next_rrc_state;                        /*!< \brief The next state to which RRC would transition to 
                                                                                  -           0 – Disconnected
                                                                                  -           1 – Connecting
                                                                                  -           2 – CELL_FACH
                                                                                  -           3 – CELL_DCH
                                                                                  -           4 – CELL_PCH
                                                                                  -           5 – URA_PCH  */

  uint8 new_rate;          /*!< \brief Indicates the rate */
}tdsrrc_event_state_change_info_type;

typedef enum
{
  /* Paging type 1 */
  TDSRRCLOG_PAGING_TYPE_1,
  /* Paging type 2 */
  TDSRRCLOG_PAGING_TYPE_2
}tdsrrc_event_paging_e_type;

typedef struct
{
  /*! \brief Paging type */
  tdsrrc_event_paging_e_type           paging_type;
  /*! \brief Paging cause */
  rrc_paging_cause_e_type       paging_cause;
}tdsrrc_event_paging_info_type;

typedef struct
{
  /*! \brief Current CSP state defined in tdsrrc_csp_substate_e_type*/
  uint8 prev_csp_state;
  
  /*! \brief The next CSP state defined in tdsrrc_csp_substate_e_type*/
  uint8 next_csp_state;

  /*! \brief Indicates if state changed: 0 no state change, 1 state changed */
  uint8 state_changed;

}tdsrrc_event_csp_state_info_type;

typedef struct
{
  /*! \brief TDS RRC CCM substate defined in tdsrrc_ccm_substate_e_type*/
  uint8 ccm_sub_state;
}tdsrrc_event_ccm_state_info_type;

typedef struct
{
  /*! \brief TDS RRC MCM substate defined in tdsrrcmcm_state_e_type*/
  uint8 mcm_sub_state;
}tdsrrc_event_mcm_state_info_type;


/*! \brief TDSCDMA_RRC_MODES_AND_STATES_LOG_PACKET */
typedef struct
{
  log_hdr_type    hdr;             /*!< \brief Log header */
  uint8           version;         /*!< \brief Log packet version number */ 
  uint8           rrc_state;       /*!< \brief Displays the state of the RRC
                                                                          -   0 - Disconnected
                                                                          -   1 - Connecting
                                                                          -   2 - CELL_FACH
                                                                          -   3 - CELL_DCH
                                                                          -   4 - CELL_PCH
                                                                          -   5 - URA_PCH  */
  uint16          reserved;        /*!< \brief To align for 32 bits */
}tdscdma_rrc_modes_and_states_log_packet_type;


/*--------------------------------------------------------------------------
                   LOG PACKET: TDSCDMA_RRC_PROTOCOL_ERRORS_LOG_PACKET
--------------------------------------------------------------------------*/
/* Define the RRC procedures. These defines are used to set the rrc_procedure
 * field.
 */
#define TDSRRCLOG_PROCEDURE_CSP    0   /*!< \brief Cell Selection */
#define TDSRRCLOG_PROCEDURE_SIB    1   /*!< \brief SIB Processing */
#define TDSRRCLOG_PROCEDURE_PG2    2   /*!< \brief Paging Type 2 */
#define TDSRRCLOG_PROCEDURE_MCMR   3   /*!< \brief Measurement Control Reporting */
#define TDSRRCLOG_PROCEDURE_RCE    4   /*!< \brief RRC Connection Establishment */
#define TDSRRCLOG_PROCEDURE_RCR    5   /*!< \brief RRC Connection Release */
#define TDSRRCLOG_PROCEDURE_UECI   6   /*!< \brief UE Capability Information */
#define TDSRRCLOG_PROCEDURE_UECE   7   /*!< \brief UE Capability Enquiry */
#define TDSRRCLOG_PROCEDURE_IDT    8   /*!< \brief Initial Direct Transfer */
#define TDSRRCLOG_PROCEDURE_UDT    9   /*!< \brief Uplink Direct Transfer */
#define TDSRRCLOG_PROCEDURE_DDT    10  /*!< \brief Downlink Direct Transfer */
#define TDSRRCLOG_PROCEDURE_SCR    11  /*!< \brief Signalling Connection Release */
#define TDSRRCLOG_PROCEDURE_SCRR   12  /*!< \brief Signalling Connection Release Request */
#define TDSRRCLOG_PROCEDURE_CC     13  /*!< \brief Counter Check */
#define TDSRRCLOG_PROCEDURE_RBE    14  /*!< \brief Radio Bearer Establishment */
#define TDSRRCLOG_PROCEDURE_RBRC   15  /*!< \brief Radio Bearer Re-configuration */
#define TDSRRCLOG_PROCEDURE_RBR    16  /*!< \brief Radio Bearer Release */
#define TDSRRCLOG_PROCEDURE_TCR    17  /*!< \brief Transport Channel Re-configuration */
#define TDSRRCLOG_PROCEDURE_PCR    18  /*!< \brief Physical Channel Re-configuration */
#define TDSRRCLOG_PROCEDURE_TFC    19  /*!< \brief Transport Format Combination Control */
#define TDSRRCLOG_PROCEDURE_CU     20  /*!< \brief Cell Update */
#define TDSRRCLOG_PROCEDURE_UU     21  /*!< \brief URA Update */
#define TDSRRCLOG_PROCEDURE_UMI    22  /*!< \brief UTRAN Mobility Information */
#define TDSRRCLOG_PROCEDURE_ASU    23  /*!< \brief Active Set Update */
#define TDSRRCLOG_PROCEDURE_ISHFU  24  /*!< \brief Inter-System Handover from UTRAN */
#define TDSRRCLOG_PROCEDURE_ISHTU  25  /*!< \brief Inter-System Handover to UTRAN */
#define TDSRRCLOG_PROCEDURE_ISCFU  26  /*!< \brief Inter-System Cell Reselection fm UTRAN */
#define TDSRRCLOG_PROCEDURE_ISCTU  27  /*!< \brief Inter-System Cell Reselection to UTRAN */
#define TDSRRCLOG_PROCEDURE_PG1    28  /*!< \brief Page Type 1 */
#define TDSRRCLOG_PROCEDURE_SMC    29  /*!< \brief Security Mode Control */

/* Define the failure causes. These defines are used to set the
 * failure_cause field.
 */
#define TDSRRCLOG_CONFIG_UNSUP           0 /*!< \brief Configuration unsupported */
#define TDSRRCLOG_PHY_CHAN_FAIL          1 /*!< \brief Physical channel failure */
#define TDSRRCLOG_INCOMPAT_SIM_RECONFIG  2 /*!< \brief Incompatible simultaneous reconfig */
#define TDSRRCLOG_PROTOCOL_ERROR         3 /*!< \brief Protocol error */
#define TDSRRCLOG_COMPRESSED_RT_ERR      4 /*!< \brief Compressed mode runtime error */
#define TDSRRCLOG_CELL_RESEL             5 /*!< \brief Cell reselection */
#define TDSRRCLOG_INVALID_CONFIG         6 /*!< \brief Invalid configuration */
#define TDSRRCLOG_CONFIG_INCOMPLETE      7 /*!< \brief Configuration incomplete */
#define TDSRRCLOG_UNSUP_MEAS             8 /*!< \brief Unsupported measurement */

/* Define the protocol errors. These defines are used to set the
 * protocol_error_cause field.
 */
#define TDSRRCLOG_ASN1_ENC_ERR      0 /*!< \brief ASN.1 violation or encoding error */
#define TDSRRCLOG_BAD_MSG_TYPE      1 /*!< \brief Msg type nonexistent or not implemented */
#define TDSRRCLOG_INCOMPAT_RX_STATE 2 /*!< \brief Msg not compatible with receiver state */
#define TDSRRCLOG_BAD_IE            3 /*!< \brief IE value not comprehended */
#define TDSRRCLOG_COND_IE_ERROR     4 /*!< \brief Conditional IE error */
#define TDSRRCLOG_BAD_MSG_EXTENSION 5 /*!< \brief Message extension not comprehended */

/*! TDSCDMA_RRC_PROTOCOL_ERRORS_LOG_PACKET */
typedef struct
{
  log_hdr_type   hdr;                      /*!< \brief Log header */
  uint8          version;                  /*!< \brief Log packet version number */ 
  uint8          rrc_state;                /*!< \brief Displays the state of the RRC
                                                                                          -   0 - Disconnected
                                                                                          -   1 - Connecting
                                                                                          -   2 - CELL_FACH
                                                                                          -   3 - CELL_DCH
                                                                                          -   4 - CELL_PCH
                                                                                          -   5 - URA_PCH  */
  uint8          rrc_procedure;            /*!< \brief Current RRC procedure 
                                                                                          -   0 - Cell selection/reselection procedure
                                                                                          -   1 - SIB processing procedure
                                                                                          -   2 - Paging type 2 procedure
                                                                                          -   3 - Measurement control/report procedure
                                                                                          -   4 - RRC connection establishment procedure
                                                                                          -   5 - RRC connection release procedure
                                                                                          -   6 - UE capability information
                                                                                          -   7 - UE capability enquiry
                                                                                          -   8 - Initial direct transfer
                                                                                          -   9 - Uplink direct transfer
                                                                                          -   10 - Downlink direct transfer
                                                                                          -   11 - Signaling connection release
                                                                                          -   12 - Signaling connection release request
                                                                                          -   13 - Counter check
                                                                                          -   14 - Radio bearer establishment
                                                                                          -   15 - Radio bearer reconfiguration
                                                                                          -   16 - Radio bearer release
                                                                                          -   17 - Transport channel reconfiguration
                                                                                          -   18 - Physical channel reconfiguration
                                                                                          -   19 - Transport format combination control
                                                                                          -   20 - Cell update procedure
                                                                                          -   21 - URA update procedure
                                                                                          -   22 - UTRAN mobility information
                                                                                          -   23 - Active set update in soft handover
                                                                                          -   24 - Intersystem handover from UTRAN
                                                                                          -   25 - Intersystem handover to UTRAN
                                                                                          -   26 - Intersystem cell reselection from UTRAN
                                                                                          -   27 - Intersystem cell reselection to UTRAN
                                                                                          -   28 - Paging type 1 procedure
                                                                                          -   29 - Security mode control procedure  */
  
  uint8          failure_cause;            /*!< \brief The failure cause 
                                                                                          -   0 - Configuration unsupported
                                                                                          -   1 - Physical channel failure
                                                                                          -   2 - Incompatible simultaneous reconfiguration
                                                                                          -   3 - Protocol error
                                                                                          -   4 - Compressed mode runtime error
                                                                                          -   5 - Cell reselection
                                                                                          -   6 - Invalid configuration
                                                                                          -   7 - Configuration incomplete
                                                                                          -   8 - Unsupported measurement */
  uint8          protocol_error_cause;     /*!< \brief The protocol error cause 
                                                                                          -   0 - ASN 1 violation or encoding error
                                                                                          -   1 - Msg type nonexistent or not implemented
                                                                                          -   2 - Msg not compatible with receiver state
                                                                                          -   3 - IE value not comprehended
                                                                                          -   4 - Conditional IE error
                                                                                          -   5 - Msg extension not comprehended */
}tdscdma_rrc_protocol_errors_log_packet_type;

typedef enum
{
  ACQ_DB,                        /*!< \brief ACQ frequency from ACQ_DB */
  MSET,                          /*!< \brief ACQ frequency from ACQ_DB */
  PFL,                           /*!< \brief ACQ frequency from PFL */
  Fine_Scan,                     /*!< \brief ACQ frequency from Fine_Scan */
  Freq_Redirection,              /*!< \brief ACQ frequency from Freq_Redirection */
  X2T_Redirection                /*!< \brief ACQ frequency from X2T_Redirection */
} tdsrrc_acq_freq_source_type;

/*! TDSCDMA_RRC_ACQ_INFO_LOG_PACKET */
typedef struct
{
  log_hdr_type                       hdr;         /*!< \brief Log header */                         
  uint8                              version;     /*!< \brief Log packet version number */
  uint8                              num_freq;    /*!< \brief number of uarfcn[] and source[] */
  uint16                             uarfcn[TDSL1_MAX_SCAN_FREQ];   /*!< \brief tds uarfcn for acquisition */                              
  uint8                              source[TDSL1_MAX_SCAN_FREQ];   /*!< \brief tds uarfcn source
                                                                                          -   0 -  ACQ_DB
																						  -   1 -  MSET
																						  -   2 -  PFL
																						  -   3 -  Fine_Scan
																						  -   4 -  Freq_Redirection
																						  -   5 -  X2T_Redirection*/
  uint8                              acq_mode;    /*!< \brief Current RRC procedure
                                                                                          -   0 - TDSL1_ACQ_MODE_NORMAL
																						  -   1 - TDSL1_ACQ_MODE_SHORT
																						  -   2 - TDSL1_ACQ_MODE_SHALLOW
																						  -   3 - TDSL1_ACQ_MODE_DEEP
																						  -   4 - TDSL1_ACQ_MODE_REACQ
																						  -   5 - TDSL1_ACQ_MODE_IRAT_TIMING
																						  -   6 - TDSL1_ACQ_MODE_LFS
																						  -   7 - TDSL1_ACQ_MODE_MAX*/

}tdscdma_rrc_acq_info_log_packet_type;

/*--------------------------------------------------------------------------
        LOG PACKET: TDSCDMA_RRC_DSDS_LTA_LOG_PACKET
                 (LOG CODE: 0xD0E6)
                     
This packet hold legacy tune away status for T/G + G DSDS UE, including 
its start time, end time, duration, etc 
 
This log record is generated whenever a legacy tune away happens

Size: 12 + 30 = 42 bytes
----------------------------------------------------------------------------*/

typedef PACKED struct PACKED_POST
{
  log_hdr_type                       hdr;                  /*!< \brief Log header */                         
  uint8                              version;              /*!< \brief Log packet version number */
  uint8                              rrc_state;            /*!< \brief Displays the state of the RRC 
                                                                                           -   0 - Disconnected
                                                                                           -   1 - Connecting
                                                                                           -   2 - CELL_FACH
                                                                                           -   3 - CELL_DCH
                                                                                           -   4 - CELL_PCH
                                                                                           -   5 - URA_PCH 
                                                                                           -   6 - WILDCARD*/
  uint32                             lta_duration_in_ms;   /*!< \brief LTA duration in ms*/
  int64                              lta_duration_in_clk;  /*!< \brief LTA duration in clock */
  int64                              lta_start_time;       /*!< \brief LTA start time */
  int64                              lta_end_time;         /*!< \brief LTA end time */
}tdscdma_dsds_rrc_lta_log_packet_type;




/*--------------------------------------------------------------------------
                   LOG PACKET: TDSCDMA_RRC_CELL_ID_LOG_PACKET
--------------------------------------------------------------------------*/
/* Define the cell access restrictions. These defines are used to set the
 * cell_access_rest field.
 *
 * Bit 0: 0 = Cell barred
 *        1 = Cell not barred
 *
 * Bit 1: 0 = Cell Reserved
 *        1 = Cell not reserved
 *
 * Bit 2: 0 = Cell reserved for SOLSA
 *        1 = Cell not reserved for SOLSA
 *
 * Bit 3: 0 = UE camped on cell
 *        1 = UE not camped on cell
 */
#define TDSRRCLOG_CELL_BARRED             0
#define TDSRRCLOG_CELL_NOT_BARRED         1

#define TDSRRCLOG_CELL_RESERVED           0
#define TDSRRCLOG_CELL_NOT_RESERVED       2

#define TDSRRCLOG_CELL_RESERVED_SOLSA     0
#define TDSRRCLOG_CELL_NOT_RESERVED_SOLSA 4

#define TDSRRCLOG_UE_CAMPED               0
#define TDSRRCLOG_UE_NOT_CAMPED           8

/* Define the call access. These defines are used to set the call_access field.
 *
 * Bit definitions for the call_access field:
 *
 * Bit 0: 0 = All calls
 *        1 = Emergency calls only
 */
#define RRCLOG_ALL_CALLS               0
#define RRCLOG_EMERGENCY_CALLS         1

/*! TDSCDMA_RRC_CELL_ID_LOG_PACKET */
typedef struct
{
  log_hdr_type   hdr;                     /*!< \brief Log header */
  uint8          version;                 /*!< \brief Log packet version number */ 
  uint8          cell_access_rest;        /*!< \brief Cell access restrictions 
                                                                                    -   0 - Bit 0: 0 = Cell barred
                                                                                    -   1 - Bit 0: 1 = Cell not barred
                                                                                    -   0 - Bit 1: 0 = Cell Reserved
                                                                                    -   1 - Bit 1: 1 = Cell not reserved
                                                                                    -   0 - Bit 2: 0 = Cell reserved for SOLSA
                                                                                    -   1 - Bit 2: 1 = Cell not reserved for SOLSA
                                                                                    -   0 - Bit 3: 0 = UE camped on cell
                                                                                    -   1 - Bit 3: 1 = UE not camped on cell*/
                                                                                    
  uint16         ura_id;                  /*!< \brief The URA ID */
  uint16         ul_uarfcn;               /*!< \brief The uplink UARFCN */
  uint16         dl_uarfcn;               /*!< \brief The downlink UARFCN */
  uint32         cell_id;                 /*!< \brief The cell ID */
  uint8          call_access;             /*!< \brief Allowed access 
                                                                                      -   0 - Bit 0: 0 = All calls
                                                                                      -   1 - Bit 0: 1 = Emergency calls only*/
}tdscdma_rrc_cell_id_log_packet_type;

/*--------------------------------------------------------------------------
                   LOG PACKET: TDSCDMA_RRC_SIGNALLING_MESSAGE_LOG_PACKET
--------------------------------------------------------------------------*/
/* Define the maximum size of a signalling message. This define is used to size
 * the signalling_message field.
 */
#define TDSRRCLOG_MAX_SIG_MSG_SIZE 2048

/*! Define the logical channel types. These defines are used to set the
 * channel_type field.
 */
#define TDSRRCLOG_SIG_UL_CCCH      0   /*!< \brief Uplink CCCH logical channel */
#define TDSRRCLOG_SIG_UL_DCCH      1   /*!< \brief Uplink DCCH logical channel */
#define TDSRRCLOG_SIG_DL_CCCH      2   /*!< \brief Downlink CCCH logical channel */
#define TDSRRCLOG_SIG_DL_DCCH      3   /*!< \brief Downlink DCCH logical channel */
#define TDSRRCLOG_SIG_DL_BCCH_BCH  4   /*!< \brief Downlink BCCH:BCH logical channel */
#define TDSRRCLOG_SIG_DL_BCCH_FACH 5   /*!< \brief Downlink BCCH:FACH logical channel */
#define TDSRRCLOG_SIG_DL_PCCH      6   /*!< \brief Downlink PCCH logical channel */
#define TDSRRCLOG_EXTENSION_SIB    9
#ifdef FEATURE_3GPP_CSFB
#define TDSRRCLOG_SIB_CONTAINER    10 
#endif
#define TDSRRCLOG_SIG_HANDOVERToUTRANCOMMAND      11   /*!< \brief HandoverToUTRANCommand from other RAT*/

/*! TDSCDMA_RRC_SIGNALLING_MESSAGE_LOG_PACKET */
typedef struct
{
  log_hdr_type hdr;                                  /*!< \brief Log header */
  uint8        version;                              /*!< \brief Log packet version number */ 
  uint8        channel_type;                         /*!< \brief Logical channel type 
                                                                                              -           0 – RRCLOG_SIG_UL_CCCH
                                                                                              -           1 – RRCLOG_SIG_UL_DCCH
                                                                                              -           2 – RRCLOG_SIG_DL_CCCH
                                                                                              -           3 – RRCLOG_SIG_DL_DCCH
                                                                                              -           4 – RRCLOG_SIG_DL_BCCH_BCH
                                                                                              -           5 – RRCLOG_SIG_DL_BCCH_FACH
                                                                                              -           6 – RRCLOG_SIG_DL_PCCH
                                                                                              #if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) 
                                                                                              -           9 – RRCLOG_SIG_DL_PCCH
                                                                                              #endif */
                                                         
  uint16       signalling_msg_length;                /*!< \brief signalling_message length in bytes */
  uint8        rb_id;                                /*!< \brief Radio Bearer ID */
  uint8        signalling_message[TDSRRCLOG_MAX_SIG_MSG_SIZE]; /*!< \brief Signalling message */
}tdscdma_rrc_signalling_message_log_packet_type;


/*! Define the signalling message types.  These defines are used to report
 * events when signalling messages are sent or received.
 */

#define TDSRRCLOG_ACTIVE_SET_UPDATE_MSG                            0
#define TDSRRCLOG_ACTIVE_SET_UPDATE_COMPLETE_MSG                   1
#define TDSRRCLOG_ACTIVE_SET_UPDATE_FAILURE_MSG                    2
#define TDSRRCLOG_ASSISTANCE_DATA_DELIVERY_MSG                     3
#define TDSRRCLOG_CELL_CHANGE_ORDER_FROM_UTRAN_MSG                 4
#define TDSRRCLOG_CELL_CHANGE_ORDER_FROM_UTRAN_FAILURE_MSG         5
#define TDSRRCLOG_CELL_UPDATE_MSG                                  6
#define TDSRRCLOG_CELL_UPDATE_CONFIRM_MSG                          7
#define TDSRRCLOG_COUNTER_CHECK_MSG                                8
#define TDSRRCLOG_COUNTER_CHECK_RESPONSE_MSG                       9
#define TDSRRCLOG_DOWNLINK_DIRECT_TRANSFER_MSG                     10
#define TDSRRCLOG_HANDOVER_FROM_UTRAN_COMMAND_MSG                  11
#define TDSRRCLOG_HANDOVER_FROM_UTRAN_FAILURE_MSG                  12
#define TDSRRCLOG_HANDOVER_TO_UTRAN_COMMAND_MSG                    13
#define TDSRRCLOG_HANDOVER_TO_UTRAN_COMPLETE_MSG                   14
#define TDSRRCLOG_INITIAL_DIRECT_TRANSFER_MSG                      15
#define TDSRRCLOG_INTER_RAT_HANDOVER_INFO_MSG                      16
#define TDSRRCLOG_MEASUREMENT_CONTROL_MSG                          17
#define TDSRRCLOG_MEASUREMENT_CONTROL_FAILURE_MSG                  18
#define TDSRRCLOG_MEASUREMENT_REPORT_MSG                           19
#define TDSRRCLOG_PAGING_TYPE_1_MSG                                20
#define TDSRRCLOG_PAGING_TYPE_2_MSG                                21
#define TDSRRCLOG_PHYSICAL_CHANNEL_RECONFIGURATION_MSG             22
#define TDSRRCLOG_PHYSICAL_CHANNEL_RECONFIGURATION_COMPLETE_MSG    23
#define TDSRRCLOG_PHYSICAL_CHANNEL_RECONFIGURATION_FAILURE_MSG     24
#define TDSRRCLOG_RADIO_BEARER_RECONFIGURATION_MSG                 25
#define TDSRRCLOG_RADIO_BEARER_RECONFIGURATION_COMPLETE_MSG        26
#define TDSRRCLOG_RADIO_BEARER_RECONFIGURATION_FAILURE_MSG         27
#define TDSRRCLOG_RADIO_BEARER_RELEASE_MSG                         28
#define TDSRRCLOG_RADIO_BEARER_RELEASE_COMPLETE_MSG                29
#define TDSRRCLOG_RADIO_BEARER_RELEASE_FAILURE_MSG                 30
#define TDSRRCLOG_RADIO_BEARER_SETUP_MSG                           31
#define TDSRRCLOG_RADIO_BEARER_SETUP_COMPLETE_MSG                  32
#define TDSRRCLOG_RADIO_BEARER_SETUP_FAILURE_MSG                   33
#define TDSRRCLOG_RRC_CONNECTION_REJECT_MSG                        34
#define TDSRRCLOG_RRC_CONNECTION_RELEASE_MSG                       35
#define TDSRRCLOG_RRC_CONNECTION_RELEASE_COMPLETE_MSG              36
#define TDSRRCLOG_RRC_CONNECTION_REQUEST_MSG                       37
#define TDSRRCLOG_RRC_CONNECTION_SETUP_MSG                         38
#define TDSRRCLOG_RRC_CONNECTION_SETUP_COMPLETE_MSG                39
#define TDSRRCLOG_RRC_FAILURE_INFO_MSG                             40
#define TDSRRCLOG_RRC_STATUS_MSG                                   41
#define TDSRRCLOG_SECURITY_MODE_COMMAND_MSG                        42
#define TDSRRCLOG_SECURITY_MODE_COMPLETE_MSG                       43
#define TDSRRCLOG_SECURITY_MODE_FAILURE_MSG                        44
#define TDSRRCLOG_SIGNALLING_CONNECTION_RELEASE_MSG                45
#define TDSRRCLOG_SIGNALLING_CONNECTION_RELEASE_INDICATION_MSG     46
#define TDSRRCLOG_SYSTEM_INFORMATION_MSG                           47
#define TDSRRCLOG_SYSTEM_INFORMATION_CHANGE_INDICATION_MSG         48
#define TDSRRCLOG_TRANSPORT_CHANNEL_RECONFIGURATION_MSG            49
#define TDSRRCLOG_TRANSPORT_CHANNEL_RECONFIGURATION_COMPLETE_MSG   50
#define TDSRRCLOG_TRANSPORT_CHANNEL_RECONFIGURATION_FAILURE_MSG    51
#define TDSRRCLOG_TRANSPORT_FORMAT_COMBINATION_CONTROL_MSG         52
#define TDSRRCLOG_TRANSPORT_FORMAT_COMBINATION_CONTROL_FAILURE_MSG 53
#define TDSRRCLOG_UE_CAPABILITY_ENQUIRY_MSG                        54
#define TDSRRCLOG_UE_CAPABILITY_INFORMATION_MSG                    55
#define TDSRRCLOG_UE_CAPABILITY_INFORMATION_CONFIRM_MSG            56
#define TDSRRCLOG_UPLINK_DIRECT_TRANSFER_MSG                       57
#define TDSRRCLOG_UPLINK_PHYSICAL_CHANNEL_CONTROL_MSG              58
#define TDSRRCLOG_URA_UPDATE_MSG                                   59
#define TDSRRCLOG_URA_UPDATE_CONFIRM_MSG                           60
#define TDSRRCLOG_UTRAN_MOBILITY_INFORMATION_MSG                   61
#define TDSRRCLOG_UTRAN_MOBILITY_INFORMATION_CONFIRM_MSG           62
#define TDSRRCLOG_UTRAN_MOBILITY_INFORMATION_FAILURE_MSG           63
#define TDSRRCLOG_SIB11bis_MSG                                     66
#define TDSRRCLOG_SIB19_MSG                                        67
#ifdef FEATURE_3GPP_CSFB
#define TDSRRCLOG_SIB_CONTAINER_COMPLETE_SIB                       68
#endif

/*Definitions*/

#define TDSRRCLOG_T_314_TIMER 0
#define TDSRRCLOG_T_315_TIMER 1
#define TDSRRCLOG_BPLMN_SRCH_TIMER 2
#define TDSRRCLOG_UNIFORM_OOS_SEARCH_TIMER 3

#define TDSRRCLOG_RAT_TDS 0
#define TDSRRCLOG_RAT_GSM 1

#define TDSRRCLOG_UOOS_STOP_ACQ_SUCCEED 0
#define TDSRRCLOG_UOOS_STOP_UPPER_LAYER 1
#define TDSRRCLOG_UOOS_STOP_PLMN_AVAILABLE 2

#define TDSRRCLOG_CS_CELL_UNKNOWN 0 
#define TDSRRCLOG_CS_SIB_READ_FAIL 1 
#define TDSRRCLOG_CS_CELL_BARRED 2
#define TDSRRCLOG_CS_CELL_NO_ACCESS 3
#define TDSRRCLOG_CS_CELL_BELOW_EXPECTED 4
#define TDSRRCLOG_CS_CELL_DIFFERENT_PLMN 5
#define TDSRRCLOG_CS_CELL_FORBIDDEN_LA 6
#define TDSRRCLOG_CS_S_FAIL 7

#define TDSRRCLOG_BPLMN_COMPLETED 0
#define TDSRRCLOG_BPLMN_TIMEOUT 1
#define TDSRRCLOG_BPLMN_ABORTED 2

#define TDSRRCLOG_TDTOTD_BPLMN 0
#define TDSRRCLOG_TDTOG_BPLMN 1
#define TDSRRCLOG_GTOTD_BPLMN 2

#define TDSRRCLOG_ACQ_DB_SCAN 0
#define TDSRRCLOG_FULL_FREQ_SCAN 1

#define TDSRRCLOG_MASTER_INFORMATION_BLOCK                  0
#define TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE1            1
#define TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE2            2
#define TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE3            3
#define TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE4            4
#define TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE5            5
#define TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE6            6
#define TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE7            7
#define TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE11           8
#define TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE12           9
#define TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE18           10
#define TDSRRCLOG_SCHEDULING_BLOCK_TYPE1                    11
#define TDSRRCLOG_SCHEDULING_BLOCK_TYPE2                    12
#define TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE5BIS         13
#define TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE11BIS        14
#define TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE19           15
#define TDSRRCLOG_EXTENSION_SIB_TYPE_UNSUPPORTED            16
#define TDSRRCLOG_SIB_TYPE_UNSUPPORTED                      17


#ifdef FEATURE_TDSCDMA_TO_LTE
#define TDTOL_SUCCESS 0
#define TDTOL_FAILURE 1
#define TDTOL_ABORTED 2
#define TDTOL_REDIR_CONN_RELEASE 0
#define TDTOL_REDIR_CONN_REJECT  1
#define TDTOL_REDIR_BLIND        2
#define TDTOL_REDIR_NONE         3
#endif

#define TDTOG_SUCCESS 0
#define TDTOG_FAILURE 1
#define TDTOG_ABORTED 2
#define TDTOG_REDIR_CONN_RELEASE 0
#define TDTOG_REDIR_SRCH 1
#define TDTOG_REDIR_CONN_REJECT  2
#define TDTOG_OUT_OF_SERVICE_SEARCH 3
#define TDTOG_REDIR_NONE         4

/* TDS to TDS redirection info Event Start Reason */
#define TD2TD_REDIR_RELEASE 0
#define TD2TD_REDIR_REJECT  1

/* TDS to TDS redirection info Event End Status */
#define TDTOTD_SUCCESS 1
#define TDTOTD_FAILURE 0


#define TDSRRCLOG_CELL_TYPE_ACCEPTABLE 0
#define TDSRRCLOG_CELL_TYPE_SUITABLE   1

/*--------------------------------------------------------------------------
                   LOG PACKET: TDSCDMA_RRC_RB_RATE_INFO_LOG_PACKET
--------------------------------------------------------------------------*/
/* UL MAC Configuration log packet */
/* At present keeping this under R7 feature should be __GNUC__ actually. But lint cribs */
typedef PACKED struct PACKED_POST
{
  uint32 rb_id;                 /*!< \brief Network assigned radio bearer identity */
  uint32 dl_rb_rate;            /*!< \brief DL throughput for a given radio bearer */
  uint32 ul_rb_rate;            /*!< \brief UL throughput for a given radio bearer */
} tdsrrc_rb_rate_type;

/*! TDSCDMA_RRC_RB_RATE_INFO_LOG_PACKET */
typedef struct
{
  log_hdr_type     hdr;         /*!< \brief Log header */
  uint8            version;     /*!< \brief Log packet version number */    
  uint8            num_rbs;     /*!< \brief Number of RBs */ 
  uint16           reserved;    /*!< \brief To align for 32 bits */
  tdsrrc_rb_rate_type rb_rate [1]; /*!< \brief RB rate */ 
}tdscdma_rrc_rb_rate_info_log_packet_type;

/* At present keeping this under R7 feature should be __GNUC__ actually. But lint cribs */

typedef PACKED struct {
  uint8 sib_type;
} tdsrrclog_complete_sib_rcvd_event_type;

typedef PACKED struct {
  uint64 start_band;
} tdsrrclog_da_backoff_timer_start_event_type;

typedef PACKED struct {
  uint64 stop_band;
} tdsrrclog_da_backoff_timer_stop_event_type;

typedef PACKED struct {
  uint64 expired_band;
} tdsrrclog_da_backoff_timer_expired_event_type;

typedef PACKED struct PACKED_POST
{
  uint8 channel_type;                         /*!< \brief Logical channel type 
                                                                                              -           0 – RRCLOG_SIG_UL_CCCH
                                                                                              -           1 – RRCLOG_SIG_UL_DCCH
                                                                                              -           2 – RRCLOG_SIG_DL_CCCH
                                                                                              -           3 – RRCLOG_SIG_DL_DCCH
                                                                                              -           4 – RRCLOG_SIG_DL_BCCH_BCH
                                                                                              -           5 – RRCLOG_SIG_DL_BCCH_FACH
                                                                                              -           6 – RRCLOG_SIG_DL_PCCH
                                                                                              #if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) 
                                                                                              -           9 – RRCLOG_SIG_DL_PCCH
                                                                                              #endif */
                                                                                              
  uint8 message_type;           /*!< \brief Type of message */
} tdsrrclog_sig_msg_event_type;

typedef PACKED struct PACKED_POST
{
  uint16 arfcn;                 /*!< \brief Absolute RF channel number */
  uint8  ncc;                   /*!< \brief Network Color Code */
  uint8  bcc;                   /*!< \brief Base station color code */
} tdsrrclog_td_to_gsm_reselection_start_event_type;

typedef PACKED struct PACKED_POST
{  
  uint8  status;                /*!< \brief the result of reselection td to gsm 
                                                          -           1 – SUCCESS  
                                                          -           0 – FAILURE  */
  uint8  failure_reason; /*!< \brief For failure_reason
                                                          -           0 – TDSRR_GSM_OTHER_REASON
                                                          -           1 – TDSRR_GSM_CELL_BARRED
                                                          -           2 – TDSRR_GSM_CELL_FORBIDDEN
                                                          -           3 – TDSRR_GSM_PLMN_MISMATCH
                                                          -           4 – TDSRR_GSM_CELL_LOW_C1
                                                          -           5 – TDSRR_GSM_BAND_NOT_SUPPORTED
                                                          -           6 – TDSRR_GSM_BSIC_NOT_FOUND
                                                          -           7 – TDSRR_GSM_BCCH_NOT_FOUND
                                                          -           8 – TDSRR_GSM_BCCH_DECODE_FAIL
                                                          -           9 – TDSRR_GSM_INVALID_STATE */
} tdsrrclog_td_to_gsm_reselection_end_event_type;


typedef PACKED struct PACKED_POST
{
  uint8 wait_timer_in_secs;          /*!< \brief Redirected Wait Time if Redirection Cause is due to Connection Reject */
  uint8 redir_reason;                /*!< \brief Reason for redirection: RCR or RCJ */
  uint16 freq;                       /*!< \brief Redirection cell frequency */
} tdsrrclog_tdscdma_to_tdscdma_redir_start_event_type;

typedef PACKED struct PACKED_POST
{
  uint8 status;                            /*!< \brief Redir Status
                                                                                      -     1 - SUCCESS
                                                                                      -     0 - FAILURE */
} tdsrrclog_tdscdma_to_tdscdma_redir_end_event_type;

typedef PACKED struct PACKED_POST
{
	uint8 is_emergency_call;           /* brief To verify if it’s an emergency call or not */                 
	uint8 MCC[3];                      /*!< \brief MCC code of current PLMN requested by MM */
	uint8 MNC[3];                      /*!< \brief MNC code of current PLMN requested by MM */
	uint8 wait_timer_in_secs;          /*!< \brief Redirected Wait Time if Redirection Cause is due to Connection Reject */
	uint8 cell_srch_reason;            /*!< \brief Reason for cell search */
} tdsrrclog_tdscdma_to_gsm_redir_start_event_type;

typedef PACKED struct PACKED_POST
{
uint8 status;                            /*!< \brief Redir Status
                                                                                      -     0 - SUCCESS
                                                                                      -     1 - FAILURE
                                                                                      -     2 - ABORTED */
} tdsrrclog_tdscdma_to_gsm_redir_end_event_type;

typedef PACKED struct PACKED_POST
{
  uint8  tdscdma_handover_type;           /*!< \brief Inter rat handover type
                                                                                      -     0 - GSM
                                                                                      -     1 - CDMA2000 */
  uint8  activation;                      /*!< \brief Inter rat handover activation time */
  uint8  activation_cfn;                  /*!< \brief Inter rat handover activation cfn */
  uint8  receive_cfn;                     /*!< \brief Inter rat handover receive cfn */
} tdsrrclog_inter_rat_handover_start_event_type;

typedef PACKED struct PACKED_POST
{
  uint8  tdscdma_handover_success;           /*!< \brief Inter rat handover success 
                                                                                      -     0 - Failure
                                                                                      -     1 - Success */
  uint8  tdscdma_handover_failure;           /*!< \brief Inter rat handover success 
                                                                                      -     0 - HANDOVER COMMAND message was invalid
                                                                                      -     1 - Configuration requested by network not supported 
                                                                                      -     2 - Physical channel failure during handover
                                                                                      -     3 - Handover was aborted 
                                                                                      -     0xFF - default */
} tdsrrclog_inter_rat_handover_end_event_type;


typedef PACKED struct PACKED_POST
{
  uint32 cell_id; 						/*!< \brief Selected cell id */
  uint16 mcc; 							/*!< \brief Selected plmn mcc */
  uint16 mnc; 							/*!< \brief Selected plmn mnc */
  uint16 freq;                            /*!< \brief Selected cell frequency */
  uint8 cpid;                             /*!< \brief Selected cell cpid */
  uint8  cell_type;						/*!< \brief Selected cell type
                                                                         -    0 - Accept
                                                                         -    1 - Suit */
} tdsrrclog_cell_selected_event_type;

typedef PACKED struct PACKED_POST
{
  uint16 uarfcn;                            /*!< \brief UTRA Absolute RF channel number */
  uint8 cpid;                               /*!< \brief cell parameters ID */
} tdsrrclog_handover_to_utran_start_event_type;


typedef PACKED struct PACKED_POST
{
  uint8  activation;                      /*!< \brief Inter rat cco activation time */
  uint8  activation_cfn;                  /*!< \brief Inter rat cco activation cfn */
  uint8  receive_cfn;                     /*!< \brief Inter rat cco receive cfn */
  uint8  BSIC_NCC;                        /*!< \brief Gsm BSIC NCC */
  uint8  BSIC_BCC;                        /*!< \brief Gsm BSIC BCC */
  uint8  Freq_Band;                       /*!< \brief Gsm frequency band 
                                                                            -    0 - dcs1800BandUsed
                                                                            -    1 - pcs1900BandUsed*/
  uint16 BCCH_ARFCN;                      /*!< \brief Gsm BCCH frequency number */
} tdsrrclog_inter_rat_cco_start_event_type;

typedef PACKED struct PACKED_POST
{
  uint8  tdscdma_cco_result;                   /*!< \brief Inter rat cco result 
                                                                             -   0 - Failure
                                                                             -   1 - Success*/
  uint8  tdscdma_cco_failure;                  /*!< \brief Inter rat cco failure reason 
                                                                             -   0 - CCO COMMAND message was invalid
                                                                             -   1 - Configuration requested by network not supported
                                                                             -   2 - Physical channel failure during handover
                                                                             -   0xFF - Default value*/
} tdsrrclog_inter_rat_cco_end_event_type;


typedef PACKED struct PACKED_POST
{
  uint8 phychan_cfg_status;               /*!< \brief phychan cfg status
                                                                                      -     0 - FALSE
                                                                                      -     1 - TRUE */
} tdsrrclog_phychan_cfg_status_event_type;

typedef PACKED struct PACKED_POST
{
  uint8 conn_rel_cause;/*!< \brief the causes of an RRC Connection Release 
                                          -           0 – RRC_REL_CAUSE_NORMAL
                                          -           1 – RRC_REL_CAUSE_UNSPEC
                                          -           2 – RRC_REL_CAUSE_PRE_EMPTIVE
                                          -           3 – RRC_REL_CAUSE_CONGESTION
                                          -           4 – RRC_REL_CAUSE_RE_ESTABLISH_REJECT
                                          -           5 – RRC_REL_USER_INACTIVITY
                                          -           6 – RRC_REL_DEEP_FADE
                                          -           7 – RRC_DIRECTED_SIGNALLING_REESTABLISHMENT
                                          -           8 – RRC_REL_CAUSE_OOS
                                          #if defined (FEATURE_UE_INITIATED_DORMANCY) || defined (FEATURE_DUAL_SIM)
                                          -           9 – RRC_UE_INIT_DORMANCY_UE_IN_OOS
                                          -           10 – RRC_UE_INIT_DORMANCY_ABORT_SUCCESS
                                          #endif FEATURE_UE_INITIATED_DORMANCY
                                          #ifdef FEATURE_DUAL_SIM
                                          -           11 – RRC_REL_DUAL_STANDBY_PS_ABORT_SUCCESS
                                          #endif
                                          -           12 – RRC_CONN_REL_CAUSE_MAX  */
} tdsrrclog_conn_rel_cause_event_type;

typedef PACKED struct PACKED_POST
{
  uint8 conn_req_cause;                   /*!< \brief Connection request cause
                                                                                      -     0 - RRC_EST_ORIG_CONV_CALL
                                                                                      -     1 - RRC_EST_ORIG_STREAM_CALL
                                                                                      -     2 - RRC_EST_ORIG_INTERACT_CALL
                                                                                      -     3 - RRC_EST_ORIG_BACKGND_CALL
                                                                                      -     4 - RRC_EST_ORIG_SUBSCRIBED_TRF_CALL
                                                                                      -     5 - RRC_EST_TERM_CONV_CALL
                                                                                      -     6 - RRC_EST_TERM_STREAM_CALL
                                                                                      -     7 - RRC_EST_TERM_INTERACT_CALL
                                                                                      -     8 - RRC_EST_TERM_BACKGND_CALL
                                                                                      -     9 - RRC_EST_EMERGENCY_CALL
                                                                                      -    10 - RRC_EST_INTER_RAT_CELL_RESELECT
                                                                                      -    11 - RRC_EST_INTER_RAT_CELL_CHANGE_ORDER
                                                                                      -    12 - RRC_EST_REGISTRATION
                                                                                      -    13 - RRC_EST_DETACH
                                                                                      -    14 - RRC_EST_HI_PRI_SIGNALLING
                                                                                      -    15 - RRC_EST_LOW_PRI_SIGNALLING
                                                                                      -    16 - RRC_EST_CALL_RE_ESTABLISH
                                                                                      -    17 - RRC_EST_TERM_HI_SIGNALLING
                                                                                      -    18 - RRC_EST_TERM_LOW_SIGNALLING
                                                                                      -           RRC_EST_MBMS_RECEPTION
                                                                                      -           RRC_EST_MBMS_PTP_RB_REQUEST                                                                                      
                                                                                      -    19 - RRC_EST_TERM_UNKNOWN */
} tdsrrclog_conn_req_cause_event_type;

typedef PACKED struct PACKED_POST
{
  uint8 rrc_timer_id;                   /*!< \brief RRC timer id
                                                                                      -     0 - T_314_TIMER
                                                                                      -     1 - T_315_TIMER
                                                                                      -     2 - BPLMN_SRCH_TIMER
                                                                                      -     3 - UNIFORM_OOS_SEARCH_TIMER */
} tdsrrclog_rrc_timer_expired_event_type;

typedef PACKED struct PACKED_POST
{
  uint16 wcdma_scan_time;                 /*!< \brief wcdma scan time */
  uint16 gsm_scan_time;                   /*!< \brief gsm scan time */
} tdsrrclog_UOOS_timer_used_event_type;

typedef PACKED struct PACKED_POST
{
  uint8 rat;                              /*!< \brief RAT types
                                                                                      -     0 - RRC_RAT_FDD
                                                                                      -     1 - RRC_RAT_GSM */
} tdsrrclog_UOOS_timer_start_event_type;

typedef PACKED struct PACKED_POST
{
  uint8 reason;                           /*!< \brief UOOS timer stop reason
                                                                                      -     0 - UOOS_STOP_ACQ_SUCCEED
                                                                                      -     1 - UOOS_STOP_UPPER_LAYER
                                                                                      -     2 - UOOS_STOP_PLMN_AVAILABLE */
} tdsrrclog_UOOS_timer_stop_event_type;

typedef PACKED struct PACKED_POST
{
  uint8 reason;                           /*!< \brief Cell selection fail reason
                                                                                      -     0 - RRC_CS_CELL_UNKNOWN
                                                                                      -     1 - RRC_CS_SIB_READ_FAIL
                                                                                      -     2 - RRC_CS_CELL_BARRED
                                                                                      -     3 - RRC_CS_CELL_NO_ACCESS
                                                                                      -     4 - RRC_CS_CELL_BELOW_EXPECTED
                                                                                      -     5 - RRC_CS_CELL_DIFFERENT_PLMN
                                                                                      -     6 - RRC_CS_CELL_FORBIDDEN_LA
                                                                                      -     7 - RRC_CS_S_FAIL */
} tdsrrclog_cell_selection_fail_event_type;

typedef PACKED struct PACKED_POST
{
  uint8 network_select_mode;              /*!< \brief network select mode
                                                                                      -   -1 - SYS_NETWORK_SELECTION_MODE_NONE
                                                                                      -     0 - SYS_NETWORK_SELECTION_MODE_AUTOMATIC
                                                                                      -     1 - SYS_NETWORK_SELECTION_MODE_MANUAL
                                                                                      -     2 - SYS_NETWORK_SELECTION_MODE_LIMITED
                                                                                      -     3 - SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION
                                                                                      -     4 - SYS_NETWORK_SELECTION_MODE_LIMITED_MANUAL_ACQUISITION
                                                                                      -     5 - SYS_NETWORK_SELECTION_MODE_MAX */
} tdsrrclog_bplmn_start_event_type;

typedef PACKED struct PACKED_POST
{
  uint8 bplmn_end_reason;                 /*!< \brief Bplmn end reason
                                                                                      -     0 - RC_BPLMN_COMPLETED
                                                                                      -     1 - RRC_BPLMN_TIMEOUT
                                                                                      -     2 - RRC_BPLMN_ABORTED */
} tdsrrclog_bplmn_end_event_type;

typedef PACKED struct PACKED_POST
{
  uint8 scan_type;                        /*!< \brief Bplmn scan type
                                                                                      -     0 - RRC_TDTOTD_BPLMN
                                                                                      -     1 - RRC_TDTOG_BPLMN
                                                                                      -     2 - RRC_GTOTD_BPLMN */
} tdsrrclog_bplmn_scan_start_event_type;

typedef PACKED struct PACKED_POST
{
  uint8 scan_complete;                    /*!< \brief Bplmn scan complete
                                                                                      -     0 - RRC_BPLMN_COMPLETED
                                                                                      -     1 - RRC_BPLMN_TIMEOUT
                                                                                      -     2 - RRC_BPLMN_ABORTED */
} tdsrrclog_bplmn_scan_end_event_type;

typedef PACKED struct PACKED_POST
{
  uint8 scan_type;                        /*!< \brief RRC csp scan type
                                                                                      -     0 - ACQ_DB_SCAN
                                                                                      -     1 - FULL_FREQ_SCAN */
} tdsrrclog_rrccsp_scan_start_event_type;

typedef PACKED struct PACKED_POST
{
  uint8 timer_in_secs;                    /*!< \brief eoos full scan timer */
} tdsrrclog_eoos_skip_full_scan_timer_start_event_type;

typedef PACKED struct PACKED_POST
{
  uint8   acq_mode;                       /*!< \brief RRC csp Acquisiton mode
                                                                                      -     0 - TDSL1_ACQ_MODE_NORMAL
                                                                                      -     1 - TDSL1_ACQ_MODE_SHORT
                                                                                      -     2 - TDSL1_ACQ_MODE_SHALLOW
                                                                                      -     3 - TDSL1_ACQ_MODE_DEEP
                                                                                      -     4 - TDSL1_ACQ_MODE_REACQ
                                                                                      -     5 - TDSL1_ACQ_MODE_IRAT_TIMING
                                                                                      -     6 - TDSL1_ACQ_MODE_LFS */
  uint8   num_freq;                       /*!< \brief Number of freq for Acquisiton */
  uint16  first_freq;                     /*!< \brief first frequency for Acquisiton */
  uint16  last_freq;                      /*!< \brief last frequency for Acquisiton */
} tdsrrclog_rrccsp_acq_start_event_type;

typedef PACKED struct PACKED_POST
{
  uint8   acq_success;                    /*!< \brief RRC csp Acquisiton
                                                                                      -     0 - FAILURE
                                                                                      -     1 - SUCCESS */
  uint8   num_cells_detected;             /*!< \brief Number of CPIDs detected */
  uint16  freq;                           /*!< \brief RRC csp Acquisiton frequency */
} tdsrrclog_rrccsp_acq_event_type;

typedef PACKED struct PACKED_POST
{
  uint8  num_pfr_freq;                            /*!< \brief Number of Pseudo Fast Return frequency */
  uint16 pfr_freq[TDSRRC_LOG_MAX_PFR_FREQ];       /*!< \brief Pseudo Fast Return frequency list */
} tdsrrclog_rrccsp_interrat_pfr_start_event_type;


typedef PACKED struct PACKED_POST
{
  uint8  success;                         /*!< \brief Pseudo Fast Return Status
                                                                                      -     0 - FAILURE
                                                                                      -     1 - SUCCESS */
} tdsrrclog_rrccsp_interrat_pfr_end_event_type;


#ifdef FEATURE_TDSCDMA_TO_LTE
typedef PACKED struct PACKED_POST 
{
  uint32 earfcn;                         /*!< \brief EARFCN of the downlink carrier frequency 
                                                                                         Range: (0..65535) */
  uint16 cell_id;                        /*!< \brief Physical cell id of a cell from LTE EARFCN 
												     Range 0-503 */
  uint8 MCC[3];                          /*!< \brief MCC code of current PLMN requested by MM */
  uint8 MNC[3];                          /*!< \brief MNC code of current PLMN requested by MM */
} tdsrrclog_tdscdma_to_lte_resel_start_event_type;

typedef PACKED struct PACKED_POST 
{
  uint8 status;                          /*!< \brief Resel Status
                                                                                      -     0 - SUCCESS
                                                                                      -     1 - FAILURE
                                                                                      -     2 - ABORTED */
  uint8 failure_cause;                   /*!< \brief Resel Failure Cause
                                                                                         see lte_rrc_irat_to_lte_failure_cause_type_e
                                                                                         in lte_rrc_irat_msg.h */
} tdsrrclog_tdscdma_to_lte_resel_end_event_type;

typedef PACKED struct PACKED_POST
{
uint8 redir_cause;                       /*!< \brief Resel Status
                                                                                      -     0 - Connection Release
                                                                                      -     1 - Connection Reject
                                                                                      -     2 - None */
uint8 MCC[3];                            /*!< \brief MCC code of current PLMN requested by MM */
uint8 MNC[3];                            /*!< \brief MNC code of current PLMN requested by MM */
uint8 wait_timer_in_secs;                /*!< \brief Redirected Wait Time if Redirection Cause is due to Connection Reject */
} tdsrrclog_tdscdma_to_lte_redir_start_event_type;

typedef PACKED struct PACKED_POST
{
uint8 status;                            /*!< \brief Redir Status
                                                                                      -     0 - SUCCESS
                                                                                      -     1 - FAILURE
                                                                                      -     2 - ABORTED */
} tdsrrclog_tdscdma_to_lte_redir_end_event_type;
#endif

#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
typedef PACKED struct PACKED_POST
{
  uint64 validity_timer_in_ms;           /*!< \brief Dedicated priority validity timer value in ms */
} tdsrrclog_tdscdma_ded_pri_val_tmr_start_event_type;
#endif

typedef PACKED struct PACKED_POST
{
  uint8  handover_type;               /*!< \brief Indicates the handover type is HHO or BHO
                                                                                      0 - HHO
                                                                                      1 - BHO */
  uint8  source_cpid;                 /*!< \brief Source Cell CPID 0~127 */
  uint8  target_cpid;                 /*!< \brief Target Cell CPID 0~127 */
  uint16  source_freq;                /*!< \brief Source Cell Freq 0~65535 */
  uint16  target_freq;                /*!< \brief Target Cell Freq 0~65535 */
} tdsrrclog_handover_start_event_type;

typedef PACKED struct PACKED_POST
{
  uint8  tdscdma_handover_result;           /*!< \brief TDSCDMA CCO result 
                                                                                      -     0 - Failure
                                                                                      -     1 - Success */
  uint8  tdscdma_handover_failure_type;           /*!< \brief If failure, give reason; otherwise 0xFF
                                                                                      -     0 - Channel setup failure
                                                                                      -     1 - Downlink sync failure
                                                                                      -     0xFF - default */
} tdsrrclog_handover_end_event_type;

typedef PACKED struct PACKED_POST
{
  uint16  primary_freq;          /*!< \brief Primary frequency of current cell */
  uint8  cpid;                   /*!< \brief CPID of current cell */
  uint8  upa_indicator;          /*!< \brief UPA indicator
                                                                                      0 - In Idle mode, NW do not support UPA. In connected  mode, UPA is inactive.
                                                                                      1 - In Idle mode, NW support UPA. In connected mode, UPA is active */
} tdsrrclog_hsupa_support_status_type;

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
typedef PACKED struct PACKED_POST
{
  uint8  deferred_sib_11;           /*!< \brief sib_11 deferred 
                                                                                      -     0 - FALSE
                                                                                      -     1 - TRUE */
  uint8  deferred_sib_11_bis;       /*!< \brief sib_11_bis deferred 
                                                                                      -     0 - FALSE
                                                                                      -     1 - TRUE */
  uint8  deferred_sib_12;           /*!< \brief sib_12 deferred 
                                                                                      -     0 - FALSE
                                                                                      -     1 - TRUE */
} tdsrrclog_dmcr_enabled_event_type;
#endif
typedef PACKED struct PACKED_POST
{
  uint8 ta_type;                               /*!< \brief Block Reason
                                                                       0 - LTA
                                                                       1 - QTA
                                                */
  uint8 block_reason;                          /*!< \brief Block Reason
                                                                       0 - TUNEAWAY_DISABLED,                  
                                                                       1 - TUNEAWAY_ALREADY_IN_PROGRESS,       
                                                                       2 - TIME_NOT_ENOUGH,                    
                                                                       3 - RRC_TASK_DELAY,                     
                                                                       4 - STATE_INCORRECT,                    
                                                                       5 - RESELECTION_IN_PROGRESS,            
                                                                       6 - OC_SETTING_IN_PROGRESS,             
                                                                       7 - NOT_CAMP_ON,                        
                                                                       8 - CELL_UPDATE_IN_PROGRESS,            
                                                                       9 - RCR_IN_PROGRESS,                    
                                                                      10 - TD2G_CCO_IN_PROGRESS,               
                                                                      11 - G2TD_CCO_IN_PROGRESS,               
                                                                      12 - CS_DOMAIN_OPEN,                     
                                                                      13 - PEDNING_DL_SRB_DATA_EXIST,          
                                                                      14 - PENDING_L2_ACK_EXIST,               
                                                                      15 - LOOP_BACK_MODE_OPEN,                
                                                                      16 - CPC_DRX_ACTIVE,                     
                                                                      17 - SUSPEND_TD_MODE_IN_PROGRESS,        
                                                                      18 - RRC_CONN_NOT_EST,                  
                                                                      19 - CS_RAB_PRESENT,                     
                                                                      20 - CS_IDT_IN_PROGRESS,                 
                                                                      21 - PS_IDT_IN_PROGRESS,                 
                                                                      22 - CONVERSATION_STREAMING_PS_PRESENT,  
                                                                      23 - BLOCK_REASON_MAX  */
} tdsrrclog_ta_block_event_type;



#ifdef T_WINNT
  #error code not present
#endif

#endif /* TDSRRCLOGGING_H */
