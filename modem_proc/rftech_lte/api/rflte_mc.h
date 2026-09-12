/*!
  @file rflte_mc.h

  @brief
  This file contains all declarations and definitions necessary to use
  the LTE Main Control.

  @details

*/

/*===========================================================================

  Copyright (c) 2009 - 2020 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/rftech_lte.mpss/3.11/api/rflte_mc.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/24/20   ssl     [CR2708896] Support to clear device Spur_db in exit mode 
04/29/20   ssl     [CR2674308] Read WTR status registers
03/30/20   ssl     [CR2634424]Handling TX disable request on non active sub_id
02/18/20   ssl     [CR2622794]Reducing Rflte rffe and grfc script sizes optimally
02/07/20   ssl     [CR2617842]heap opts: making RFLTE_MC_MAX_SSBI_SCRIPT_SIZE to 0
01/27/20   Hari    LTE minidump changes
01/23/20   ssl     [CR2609017]Removing Sub_id in rflte_mc_asdiv_cb_func
01/20/20   ssl     FR 55657: Replace RF Enter/Exit with RF Sleep/Wakeup for L+L DSDS awake
01/06/20   ssl     Clean up old rf cards 
11/08/19   ssl     FR 56310: Skip the preparation of RX Sleep scripts if configuration is not changed
10/18/19   ssl     [CR2548513]Removal of unused legacy API'S support
03/07/18   ks      QAT char offset update
03/02/18   ks      RFFE based XSW support
09/21/17   pv      New API to check particular band has Cal data or not.
08/02/17   ks      Variance id changes
08/16/16   ndb     Added band_to_carrier_idx_map in rflte state machine
05/06/16   sk       Back out alt path changes in the CR 964773
05/02/16   sg      CDRx Rx/Tx wakeup/sleep optimization
04/27/16    vc     Avoiding script building in sleep/wakeup on same config
03/11/16   am      Added rflte_mc_get_sar_forced_ant() for ASDiv
01/19/16   cv      Destroy script buffers in SM at the end of exit mode
01/12/16   cv       changes to turn eLNA on/off in TDD mode
01/11/15   sml     Phase 4 : Reading device status registers in RFLTE
12/22/15   sk      Support for A GAP tune-away for RX with Tx Intact
12/09/15   am      FTM mode support for Autopin WTR and Reflog capture
12/07/15   sg      Make TxPL APIs handle based
12/02/15   sml     Phase 2 device status read
11/26/15    vc     RF LTE timiline optimization
11/25/15   am      asm trigger update for tdd split bands.
11/25/15   vkm     Per Carrier common lock mechanism to prevent deadlocks.
11/08/15   kg      Autopin PAR Support
11/06/15    rp     Added support device to band mapping for Tx DPD loading.
11/04/15   mah     Changes to enable handle bases TxPL SM.
11/03/15   sk      Changes to do rflte_state_init in enter mode.
10/24/15   stw     MC Level Band Decision maker added to handle split bands
10/21/15   mah     WTR ICI support: add field to record the CA mode
10/06/15   sg      Remove DPD status flag from MC state machine.
                   Moved to TX handle based SM.
09/30/15   stw     Rx Tune params updated for Spur Mitigation Handling
09/28/15   ska     Adding support for DL active mask bitmask for NS ULCA
09/25/15   sb      FR30081:Apply gain offsets for ASDIV switch position change
09/24/15   pkg     Changes to Support CDRx Sleep Wakeup scenarios.
09/21/15   sk      Make AFC TxLM Handle based
09/17/15   dyc     Support for DMCS power mode
09/11/15   ska     Adding path override logic for determining IntraCA feasible
09/04/15    pv     Adding Dynamic TxLM Buffer index based on ML1 input params to support ULCA.
09/04/15   mg      Support for GRFC tuner
09/03/15   aak     Support for TP tuner
08/28/15    sg     FR28508: Add ULCA Support for MTPL Backoff due to PA thermal
08/28/15   pkg     Use Tune Bitmask for Tx Disable.
08/26/15   sk      [Phase 1] WTR hopping - Move TX MC SM variables to hadnle based DM
08/20/15   stw     FR 26008 : SCC1 SCC2 Non-Contiguous CA
08/18/15   sg      Add a flag to indicate if ASDiv critical section has been initialized
08/12/15   pkg     WTR Swap Changes for Tx.
08/07/15   sg      FR27572: Add AsDiv SAR backoff support
07/25/15    JJ     Added FBRx coupler/switch re-program support
07/20/15   pkg     Changes to support FR 24616:Algorithm to mitigate VBatt
                   droop (down to 2.5V) via TX backoff.
07/16/15   jd      WTR hopping phase2.
06/26/15   jd      Wtr hopping phase1.
06/25/15   pkg     Changes to support Intra ULCA Tune on bands with splits.
06/22/15   am      [Ported] MC common layer API to set RX config active flag for all carriers and paths per carrier
06/11/15   stw     RF-RRC Interface for Bandwidth Class Combos
06/08/15   kab     Support for FTM RF scouting for ULCA 
05/25/15   ndb     Remove the un-used API'a
05/19/15   sdu     Added an Alt_path override parameter in MC state machine
05/12/15   pkg     Code cleanup and revisit of logic for UL CA.
04/29/15   kab     Interband ULCA TQ alloc\dealloc management
04/28/15   sb      [RX/TX split] Fixes to support TX/RX cal
04/24/15   mg      FED MC layer cleanup changes
04/21/15    sk     Re-structure Tx path specific NV's per Tx NV container
04/21/15    sk     Inter-band UL CA support
03/26/15   stw     Added SRX Exit DM update API
3/23/15    JJ      ULCA FBRx Cal support with xpt override fix
03/20/15   pkg     UL CA related changes.
03/18/15   pkg     Changes to support Script based Tx Tune.
03/02/15   JJ     Added an api to check ULCA capability
03/12/15   mg      Fix to track the tdd_mode flag per carrier
03/04/15   kab     Query DTR API - to check if valid SRx tune config
03/03/15   stw     Added API for band class D support
02/23/15   gvn     Resolve bugs in Split intraband retune design
01/23/15   kab     Single Rx IRAT changes
11/26/14   ndb     Added rflte_mc_fbrx_coupler_ctl_script_setup()
12/15/14   ars     Added new parameter to LTS state machine to store original Pcell
                   DRx band before intraband split SCell is added
12/05/14   dw      Changes to support split bands based on split specified in RFC.
11/21/14   kab     Fix Single Rx concurrent SCC addition crash
11/17/14   kab     FTM RF Dynamic SCC Device allocation
11/14/14    pv     HORxD Feature Bring up changes.
11/12/14   bsh     Cleanup TQ assignment for all cells
11/07/14   kab     RRC BW Capability API Logic Fix
11/04/14   gvn     Re-tune Tx and Rx in case split band changes on intra scell add or delete
10/29/14   jf/dyc  Support for ASDIV xsw decoupling
10/22/14   shk     Added macro to define split band override default value
10/20/14   as/vb   Enable GRFC controlled ASDiv switch for FTM 
10/10/14   kab     Fix KW\CW 
10/08/14   kab     Changes to Skip PCC update when SCC exit from (PCC+SCC) SRx
10/03/14   kab     MC Layer Support for Single Rx 
10/02/14   kab     Support for Single Rx RXLM Config Mode Selection.
09/30/14   svi     Changes to split Rx Tune Script as separate Preload and 
                   Trigger events
08/20/14   ndb     Add Tuner_ID as part of the response in "FTM_SET_TUNER_TUNE_CODE_OVERRIDE"
09/18/14   bsh     Fix KW 
09/11/14   bsh     Support FTM EXIT command and SLEEP -> EXIT 
09/11/14   kab     Separate placeholders for PRx,Drx Bands in LTE State Machine
08/27/14   svi     Move Alternate Path State Variable to Path Specific
08/22/14   gvn     If alternate path is invalid, reset it to zero
08/21/14   svi     Added Support for Rx Config Script Creation
08/21/14   svi     Started Adding Time Profiling for MC Layer
08/18/14   kab     TxPL changes to support SGLTE Tx power backoff with Coex.
08/01/14    ndb     Added support for extended EARFCN for LTE
08/15/14    st     CA Alt Path Reconfiguration Support
08/14/14    pv     Added support for PA swp adjust mechanism.
08/04/14    pv     Added support for FBRx + GPS concurrency based scripts.
08/12/14    jf     MCL LTE support
08/01/14    ndb     Added support for extended EARFCN for LTE
08/01/14   gvn     Update state machine with correct pcell band if in split intraband case
07/30/14   svi     Notch Application based on Tx AFC - Phase 2
07/17/14   pv      Added support for call-back API for spur-mitigation based re-tune in CA mode.
07/08/14   gvn     Modify algorithm for split-intraband support 
07/02/14   jf      AsDiv support
06/20/14    pv     Added support for TDD PortA/B swap for ASM.
06/18/14   sbm     Added state machine variable to determine tx pwr ctrl mode.
06/13/14   svi     Notch Application based on Tx AFC
05/13/14   jf      Add CL/AOL support
05/08/14   svi     Assigned Values to the enums deciding if Tx FED DM is 
                   to be updated or not during Tx Config
05/07/14   svi     Tx Config Timeline optimization
05/05/14   svi     Depricated RFLTE_MC_MAX_PA_STATE
05/05/14   svi     Moved TXPLL update to Core Task
04/20/14   gvn     S-LTE Support
04/11/14   bsh     Split Band Support
03/27/14   svi     [ASDIV] Added callback function as part of antenna switch
3/26/14    kg      Added support for CL LM handle 
03/27/14   gvn     Add split band support for IRAT
03/18/14    pv     Added support for FBRx script population in DM
03/17/14   svi     Added support to get Intra Band CA support info
03/11/14   bsh     Adding NLIC support - add rx freq error in sm
01/28/14   jf      Append RGI_0 script to TX_ON off script, and trigger HW write 
                   RGI_0 at tx_config
02/04/14    pv     Added support for FBRx setup.
01/08/13   npi     Track current state of Intra-band CA
01/09/14   svi     Split Band Support : SGLTE concurrency
12/02/13   cri     Support for RFLM Tx override for LTE
11/12/13   npi     Moved cell definition to an enum
11/12/13   bsh     Delete the states related to Rx Dm
10/26/13   bsh     support for rx fed
10/09/13   nrk     Added initial DM population changes
09/16/13   bsh     Clean up state machine usage
09/13/13   svi     Code Reorganization
09/04/13   svi     Added support for Single Rx CA
08/29/13   svi     MC Layer Changes to support UL CA
08/14/13   svi     Added support for Single Tx CA
07/26/13   svi     Added support for Single Tx CA
07/29/13   as      Added support for relative backoff on SAR limit based on 
                   TDD-LTE ULDL configuration
07/24/13   gvn     RSB Cal updates on online mode, iqmc update and IRAT
07/19/13    pv     Added support for separate TDD Tx and Rx ASM scripts.
07/19/13   aro     Featurized ASDIV
07/18/13   pl      Update interface to match with common AsDIV driver
07/17/13   pl      Adding Antenna Switch Diversity Support
07/02/13   ndb     Added rfcommon_nv_ant_tuner_cl_ctrl_type() 
06/06/13    pv     Added support for multple TDD ASM scripts.
05/30/13   php     Pass only bands with Critical NVs to CM for Freq Scan
05/07/13   jf      Add multiple freq comp bins support for XPT  
05/14/13   jf      Support ET_EPT_CFG
05/02/13   vb      Tuner CA changes 
04/26/13   bar     Removed the rflte_mc_state.disable_powercollapse flag
04/23/13   kab     Enable FTM mode for NBEE in IRAT 
04/22/13   bar     Introduced MC level Pointer to store bb comp table for RB based FreqComp
03/26/13   cri     XPT cal deconfig support for LTE
03/22/13   jf      Add 2D lin_vs_temp_vs_freq_comp
03/21/13   pv      Added support for GNSS Blanking during Tx for LTE TDD only.
03/21/13   jf      Move tuner_config function to .h
03/15/13   gvn     Update sgsar, coex_WWAN, coex_WLAN limits using MSGR
03/06/13   php     Add flag to indicate NS mode  
03/06/13   gvn     L2L IRAT Half script support
03/06/13   kab     Removed rxlm_buffer_idx argument from mc_exit_mode function
02/14/13   aca     Tuner over rider support
02/11/13   jf      Download ET txlm buffer only if ET mode is calibrated
02/04/12   php     Add support for using On demand Task Queues for Wakeup
01/11/13   jf      Reuse txlm buffer for ET chain
01/04/13   gvn     Move interface to query RF LTE CA bc config from rfm to rflte_mc
12/26/12   gvn     LTE Max Pwr limiting support during WLAN Coex scenario 
                   (merge from Nikel CR401193)
12/21/12   gvn     Adding functionality for Upper layers to query LTE CA bc config
12/18/12   bsh     KW errors fixed
12/07/12   cri     Dynamic allocation of linearizer tables 
11/15/12   adk     Backed out Qtuner code
11/13/12   pl      added Intra-band CA support
11/09/12   pl      Upadte #define of MAX CELL to use common #define rflte_msg.h
11/07/12   sbm     GRFC script size to 10 and timing info in state mc.
10/24/12   ndb     Qtuner- Bug fixes
10/26/12   adk     Merged antenna tuner code from Nikel.
10/25/12   gvn     Featurize LTE to support LTE FW API removal on Triton
10/08/12   php     Move PA script update to mdsp
09/14/12   php     Populate shared memory with PA RFFE script
08/23/12   pl/sbm  remove obsolete function
08/17/12   vss     Add support for RFFE size 
08/12/12   pl      Update script buffer creation for per carrier
                   prepare for sleep/wakeup optimization
07/10/12   Saul    WCDMA. Added tune_with_lte_settings API.
05/23/12   as      LTE Tx power limit for SGLTE SAR and CoEx
05/16/12   pl      LTE Carrier Aggregation support
04/04/12   pl      Remove obsolete states and comamnds
04/15/12   jf      Return RFA_RF_LTE_INVALID_CAL_DATA to Ml1 if NV is missing 
03/20/12   aca     Implemented XO correction during Tx pll tune
02/16/12   pl      Remove offline DRX processing from RFSW. 
02/09/12   aca     Tx PLL error correction
11/17/11   aak     Changes to enable Quiet mode 
11/04/11   pl      Add interface for MC state machine query
10/17/11   pl      Support SMPS temp comp
10/12/11   pl      Support Offline DRX
06/23/11   pl      Remove obsolete functions
05/09/11   sar     Removed rf_cmd_dispatcher.h and added msgr_types.h.
04/28/11   pl      Adding support for PLL Adjustments
04/28/11   pl      Removed unused functions
02/17/11   tnt     TDD Rx/Tx LM migration
02/14/11   pl      RxLM RF/L1 Integration
02/09/11   pl      MC glabal variables clean up
01/18/11   pl      RxLM integration
01/07/11   bmg     Made header C++ compatible
01/03/11   pl      Adding State machine control helper functions
12/29/10   pl      Adding new Main Control state machine states
10/14/10   whc     Added LTE Reload RFNV functionality
07/01/10   jyu     Added support to power collapse  
06/30/10   qma     Added managing rx chain support
06/25/10   jyu     Added support to manage rx chain  
05/19/10   can     Remove unused rflte_mc_do_temp_comp function.
05/10/10    dw     Added support for LTE temp comp
04/06/10   can     Removed some functions and moved them to the MSM HAL layer. 
04/01/10   pl      Move FTM related function into rflte_ftm_mc.h/.c
03/12/10   tnt     Adding code to support GRFC for RF signals
02/18/10   qma     Added RX cal support for delta offset
02/16/10   qma     Added FTM LTE RX AGC
02/01/10   pl      update rflte_mc_init() interface to comply with rfm changes
11/20/09   can     Changes to enable L1 call exercise with Genesis 1.1. Some of these
                   changes are temporary and will need to be removed eventually.
11/17/09   qma     Added FTM enter mode error checking
11/06/09   mkv	    Moving MSGR registration and de-registration required for RF Task 
				       into LTE files.
10/14/09   can     Changes to support the L1 Interface.
10/13/09   qma     Added set tx waveform command
10/08/09   qma     Added RF LTE dynamic dispatch registration
09/08/09   qma     LTE MC TX config, disable, and set BW integration
08/28/09   qma     LTE MC RX config and set BW integration
08/19/09   qma     LTE MC enter mode, exit mode integration
07/21/09   mkv	    Added Dynamic Dispatcher registration for RF LTE L1 MC commands.
04/13/09   qma     Initial Check-in
===========================================================================*/

#ifndef RFLTE_MC_H
#define RFLTE_MC_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "rfcom.h"
#include "msgr_types.h"
#include "lm_types.h"
#include "rfc_common.h"
#include "rflte_msg.h"
#include "rf_hal_common.h"
#include "rfcommon_atuner_intf.h"
#include "rfcommon_asdiv_manager.h"
#include "rfcommon_core_txlin_types.h"
#include "rflte_ext_mc.h"
#include "rflte_nv.h"
#include "timetick.h"
#include "rfcommon_time_profile.h"
#include "rfdevice_lte_type_defs.h"
#include "rfcommon_asdiv_manager.h"

#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif 
#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/
#define RFLTE_MC_MAX_SSBI_SCRIPT_SIZE 0
#define RFLTE_MC_MAX_RFFE_SCRIPT_SIZE 30
#define RFLTE_MC_MAX_GRFC_SCRIPT_SIZE 10

/* Number of Max MSM Notches to be applied per cell */
#define RFLTE_MC_MSM_MAX_NUM_NOTCH    6

/* Max number of entries in RFC SS */
#define RFLTE_MC_MAX_RFC_SPLITS (RFCOM_NUM_LTE_BWS * RFLTE_MC_MAX_NUM_SPLITS)
#define RFLTE_MC_MAX_RX_PATHS_PER_CARRIER 2

/* Macro to convert us to USTMR */
#define RFLTE_MC_CONV_FACTOR_US_TO_USTMR 19.2

/* Macro to check if a particular cell index is masked in the cell mask passed */
#define RFA_LTE_IS_CARRIER_IDX_USED(idx_mask,idx) (((idx_mask & (1<<idx))>0)?TRUE:FALSE)

/* Macro to define default alternate path*/
#define RFLTE_MC_DEFAULT_ALTERNATE_PATH 0
/* Macro to define invalid Alternate Path */
#define RFLTE_MC_INVALID_ALTERNATE_PATH 255
/* Macro to define default split band path override index */
#define RFLTE_MC_DEFAULT_SPLIT_BAND_PATH_OVERRIDE 255

#define RFLTE_MC_MAX_TXLM_HANDLES_PER_CARRIER 2
#define RFLTE_MC_MAX_FBRX_HANDLES_PER_CARRIER 2
#define RFLTE_MC_TXLM_INVALID_HANDLE 65535
#define RFLTE_MC_FBRX_INVALID_HANDLE 65535

#define RF_LTE_DEVICE_STATUS_READ_REG_LIST_LENGTH 8
#define RF_LTE_DEVICE_STATUS_READ_DEVICE_SUPPORT  6

/*-----------------------------------------------------------------------*/

/*! \brief Maximum number of LTE Subscriptions supported     */

#define RFLTE_MC_MAX_SUB_ID                        2
/*-----------------------------------------------------------------------*/

/*! \brief Macro to indicate first SUB ID                                */

#define RFLTE_MC_SUB_ID_ONE                        0
/*-----------------------------------------------------------------------*/

/*! \brief Macro to indicate second SUB ID                               */

#define RFLTE_MC_SUB_ID_TWO                        1
/*-----------------------------------------------------------------------*/

/*! \brief Macro to indicate invalid SUB ID                               */

#define RFLTE_MC_SUB_INVALID                        0xFF
/*-----------------------------------------------------------------------*/


/*!
   @brief
   Enumeration for states for LTE mode.
*/
typedef enum {
  /*! @@todo Add comments here and initialization flow*/
  RF_STATE_INIT = 0,
  /*! @@todo Add comments here and Idle flow*/
  RF_STATE_LTE_IDLE = 1,
  /*! @@todo Add comments here and FDD RX flow*/
  RF_STATE_LTE_FDD_RX_WARMUP = 2,
  /*! @@todo Add comments here and FDD RXTX flow*/
  RF_STATE_LTE_FDD_RXTX = 3,
  /*! @@todo Add comments here and TDD RX flow*/
  RF_STATE_LTE_TDD_RX_WARMUP = 4,
  /*! @@todo Add comments here and TDD RXTX flow*/
  RF_STATE_LTE_TDD_RXTX = 5,
  /*! @@todo Add comments here and TX Only flow*/
  RF_STATE_LTE_TX_ONLY = 6,
  /*! @@todo Add comments here and SLEEP flow*/
  RF_STATE_LTE_SLEEP = 7,
  /*! @@todo Add comments here and invalid flow*/
  RF_STATE_INVALID = 0xFF,
} rf_lte_state_enum_type;

typedef enum
{
  RFLTE_MC_CARR_TYPE_PRX,
  RFLTE_MC_CARR_TYPE_DRX,
  RFLTE_MC_CARR_TYPE_DRX2,
  RFLTE_MC_CARR_TYPE_DRX3,
  RFLTE_MC_CARR_TYPE_TX,
  RFLTE_MC_CARR_TYPE_RX_NUM = RFLTE_MC_CARR_TYPE_TX,
  RFLTE_MC_CARR_TYPE_NUM,
  RFLTE_MC_CARR_TYPE_MAX = RFLTE_MC_CARR_TYPE_NUM,
  RFLTE_MC_CARR_TYPE_INVALID = 0xFF,
} rflte_mc_carrier_type;

/*!
   @brief
   Enumeration for whether FED STATIC DM is to be updated
   or not during Tx Config Request.

*/
typedef enum 
{

  /*!> Enable FED STATIC DM UPDATE during Tx Config */
  RFLTE_MC_TX_FED_STATIC_DM_UPDATE_DISABLE = 0,                         /*!> FALSE */

  /*!> Disable FED STATIC DM UPDATE during Tx Config */
  RFLTE_MC_TX_FED_STATIC_DM_UPDATE_ENABLE  = 1,                          /*!> TRUE */ 

  /*!> Invalid state for FED STATIC DM UPDATE during Tx Config */
  RFLTE_MC_TX_FED_STATIC_DM_UPDATE_INVALID,  

} rf_lte_update_tx_fed_static_dm_type;

/*-----------------------------------------------------------------------*/
/*!
  @brief
  This enum denotes the script buffer that is to be requested
  from RFLTE MC layer while preparing scripts for Rx Config
  or Rx Disable
  
*/
typedef enum
{

  /*!> PRx Transceiver PRELOAD */ 
  RF_LTE_MC_PRX_TUNE_PRELOAD_TRANSCEIVER,

  /*!> DRx Transceiver PRELOAD */ 
  RF_LTE_MC_DRX_TUNE_PRELOAD_TRANSCEIVER,

  /*!> PRx Transceiver TRIGGER */ 
  RF_LTE_MC_PRX_TUNE_TRIGGER_TRANSCEIVER,

  /*!> DRx Transceiver TRIGGER */ 
  RF_LTE_MC_DRX_TUNE_TRIGGER_TRANSCEIVER,
  
  /*!> PRx Config Startup */ 
  RF_LTE_MC_PRX_TUNE_NON_TRX,

  /*!> DRx Config Startup */ 
  RF_LTE_MC_DRX_TUNE_NON_TRX,

  /*!> PRx Transceiver PRELOAD */ 
  RF_LTE_MC_RX_TUNE_PRELOAD_TRANSCEIVER,

  /*!> PRx Transceiver TRIGGER */ 
  RF_LTE_MC_RX_TUNE_TRIGGER_TRANSCEIVER,

  /*!> PRx Config Startup */ 
  RF_LTE_MC_RX_TUNE_NON_TRX,

  /*!> Invalid */    
  RF_LTE_MC_RX_TUNE_INVALID,
   
} rflte_mc_rx_tune_scripts_type_s;


/*-----------------------------------------------------------------------*/
/*!
  @brief
  This enum denotes the script buffer that is to be requested
  from RFLTE MC layer while preparing scripts for Tx Config
  or Tx Disable
  
*/
typedef enum
{

  /*!> PRx Transceiver PRELOAD */ 
  RF_LTE_MC_TX_TUNE_PRELOAD_TRANSCEIVER,

  /*!> PRx Transceiver TRIGGER */ 
  RF_LTE_MC_TX_TUNE_TRIGGER_TRANSCEIVER,

  /*!> PRx Config Startup */ 
  RF_LTE_MC_TX_TUNE_NON_TRX,

  /*!> Invalid */    
  RF_LTE_MC_TX_TUNE_INVALID,
   
} rflte_mc_tx_tune_scripts_type_s;
/*!
   @brief
   Enumeration for whether the Notch applied for a particular cell
   is config 0 or config 2.
*/
typedef enum
{

  /*! Notch was programmed for config 0 */
  RFLTE_MC_NOTCH_PROGRAMMED_CFG_0 = 0,

  /*! Notch was programmed for config 2 */
  RFLTE_MC_NOTCH_PROGRAMMED_CFG_2 = 1,

  /*! Max number of Notch configs */
  RFLTE_MC_NOTCH_PROGRAMMED_CFG_MAX,

  /*! Invalid Notch config */
  RFLTE_MC_NOTCH_PROGRAMMED_CFG_INVALID = RFLTE_MC_NOTCH_PROGRAMMED_CFG_MAX,
  
}rflte_mc_notch_programmed_config_type;

/*!
   @brief
   Enumeration for which type of Spur the Notch was applied for 
   Can be a NV Spur or a WTR spur
*/
typedef enum
{
  /*! Spur type mitigated by Notch was NV Spur */
  RFLTE_MC_NV_SPUR_MITIGATED  = 0,
  
  /*! Spur type mitigated by Notch was WTR Spur */  
  RFLTE_MC_WTR_SPUR_MITIGATED = 1,
  
  /*! Max types of Spur mitigated */  
  RFLTE_MC_MAX_TYPES_OF_SPUR_MITIGATED,

  /*! Invalid type of Spur mitigated */  
  RFLTE_MC_INVALID_SPUR_MITIGATED = RFLTE_MC_MAX_TYPES_OF_SPUR_MITIGATED,
  
} rflte_mc_spur_mitigated_type;

/*!
   @brief
   Enumeration to tell which type of power control being used.
   FBrx or hkadc
*/
typedef enum
{
   RF_LTE_POWER_CTL_MODE_FBRX,
   RF_LTE_POWER_CTL_MODE_HKADC,
   RF_LTE_POWER_CTL_MODE_NONE,
}  rf_lte_power_control_mode_type;

/*!
   @brief
   Enumeration to indicate the antenna which is "forced" to be enabled from SAR NV.
*/
typedef enum
{
  RFLTE_MC_SAR_ANTENNA_NONE,             /* no SAR forced selection */
  RFLTE_MC_SAR_ANTENNA_0,                /* pass through position */
  RFLTE_MC_SAR_ANTENNA_1,                /* swapped position */
  RFLTE_MC_SAR_ANTENNA_MAX,              /* enum limit for ant conditions */
} rflte_mc_sar_forced_antenna_type;


/*!
   @brief
   The structure defines the states for Single Tx CA.
*/
typedef struct
{
  /*! @brief The Total BW the entire Single Tx CA 
    Cycle needs to tune Tx to */
  rfcom_lte_bw_type rflte_bw;

  /*! @brief The Tx channel closest to the freq the  
    Single Tx CA cycle will tune to */
  rfcom_lte_earfcn_type rflte_chan_tx;

  /*! @brief The Tx frequency corresponding to the
   channel for Single Tx CA cycle*/
  uint32 rflte_chan_freq_tx;

  /*! @brief The Tx frequency the Single Tx CA will 
    be tuned to */
  uint32 rflte_freq_tx;

  /*! @brief The Rx Freq error for the New Freq Calculated */
  int32 rx_freq_error;
}ul_ca_single_tx_type; 

/*!
   @brief
   The structure defines the states for Single Rx CA.
*/
typedef struct
{
  /*! @brief The Total BW the entire Single Rx CA 
    Cycle needs to tune Rx to */
  rfcom_lte_bw_type rflte_bw;

  /*! @brief The Rx channel closest to the freq the  
    Single Rx CA cycle will tune to */
  rfcom_lte_earfcn_type rflte_chan_rx;

  /*! @brief The Rx frequency corresponding to the
   channel for Single Rx CA cycle*/
  uint32 rflte_chan_freq_rx;

  /*! @brief The Rx frequency the Single Rx CA will 
    be tuned to */
  uint32 rflte_freq_rx;

}dl_ca_single_rx_type; 

/*!
   @brief
   The structure defines Applied Notch Freq, Notch Depth and Spur Threshold
   per notch, per chain, per cell.
*/
typedef struct
{
  /*! Frequency(Hz) at which notch was applied */
  uint32 notch_application_freq;

  /*! Notch depth (value of K0 in the notch FSM) to be used supress the spur */
  uint8 notch_depth;

  /*! master table that contains thresholds for the corresponding spurs */
  int32 spur_thresh;

  /*! type of Spur the Notch was applied for */
  rflte_mc_spur_mitigated_type spur_type;

} rflte_mc_programmed_notches_per_path_type;

/*!
   @brief
   The structure defines applied Notch details per cell.
*/
typedef struct
{
  /*! Config at which Prx notch was programmed */
  rflte_mc_notch_programmed_config_type prx_notch_config;

  /*! Notches details for PRx */
  rflte_mc_programmed_notches_per_path_type notch_programmed_prx[RFLTE_MC_MSM_MAX_NUM_NOTCH];

  /*! DL LO Rx HZ */
  uint64 prx_dl_lo_rx_hz;

  /*! Number of Notches Programmed for PRx */
  uint32 prx_num_notches;

  /*! Config at which Drx notch was programmed */
  rflte_mc_notch_programmed_config_type drx_notch_config;

  /*! Notches details for DRx */
  rflte_mc_programmed_notches_per_path_type notch_programmed_drx[RFLTE_MC_MSM_MAX_NUM_NOTCH];  

  /*! Number of Notches Programmed for DRx */
  uint32 drx_num_notches;

  /*! DL LO Rx HZ */
  uint64 drx_dl_lo_rx_hz;
  
}rflte_mc_programmed_notches_type;

/*!
   @brief
   The structure defines state/band variables for each path for LTE.
*/
typedef struct
{
  /*! @brief Maintains state of each path */
  rf_lte_state_enum_type rf_state;

  /*! @brief RX Logical Device associated with a carrier */
  rfcom_device_enum_type prx_rfm_device;
  rfcom_device_enum_type drx_rfm_device;
  rfcom_device_enum_type drx2_rfm_device;
  rfcom_device_enum_type drx3_rfm_device;

  /*! @brief Current band for each path. Default is BAND_INVALID
  Updated during tune to channel for PRX*/     
  rfcom_lte_band_type curr_band_prx;

  /*! @brief Current band for each path. Default is BAND_INVALID
  Updated during tune to channel for DRX*/     
  rfcom_lte_band_type curr_band_drx;

  /*! @brief Current RX channel for each path. Default is 0
  Updated during tune to channel for RX*/     
  rfcom_lte_earfcn_type curr_chan_rx;

  /*! @brief Current RX BW. Default is BW_INVALID */ 
  rfcom_lte_bw_type bw_rx;

  /*! @brief NV table pointer */
  void   *nv_tbl_ptr;


  /*! @brief script data pointer for PRx, DRx and Tx */
  void *rflte_script_data_ptr[RFLTE_MC_CARR_TYPE_NUM];

  /*! @brief Flag to Track if Intra-band CA is active */
  boolean intra_band_ca_active;

  /*! @brief Flag to Track current state of Intra-band CA */
  boolean intra_band_ca_active_current;

  /*! @brief Original Pcell (Tx and PRx) band used before change for split intra-band case. Default is BAND_INVALID
  * This will be same as pcell band in all cases except it may be different for certain split intra-band cases.  
  * Updated during tune to channel for RX for scell in split-intra case  
  * Made equal to original Pcell band on scell exit */ 
  rfcom_lte_band_type original_pcell_band_before_intraband_split_scell_add;

  /*! @brief Original Pcell (DRx) band used before change for split intra-band case. Default is BAND_INVALID
  * This will be same as pcell band in all cases except it may be different for certain split intra-band cases.  
  * Updated during tune to channel for RX for scell in split-intra case  
  * Made equal to original Pcell band on scell exit */ 
  rfcom_lte_band_type original_pcell_drx_band_before_intraband_split_scell_add;

  boolean olta_enable;

  /*! @brief Pointer to the next Tx Cell in Single Tx CA Cycle */
  uint32 next_tx_cell_idx_in_union;

  /*! @brief Pointer to the next Rx Cell in Single Rx CA Cycle */
  uint32 next_rx_cell_idx_in_union;  

  /*! @brief Maintains the states of Tx Cell is in Single Tx CA Cycle */
  ul_ca_single_tx_type ul_ca_single_tx;

  /*! @brief Maintains the states of Rx Cell is in Single Rx CA Cycle */
  dl_ca_single_rx_type dl_ca_single_rx;

  /*! @brief LM Buffer index For Tx, PRx, DRx, FBRx */
  lm_handle_type rxlm_prx_buf_idx;
  lm_handle_type rxlm_drx_buf_idx;
  lm_handle_type rxlm_drx2_buf_idx;
  lm_handle_type rxlm_drx3_buf_idx;

  lm_handle_type txlm_buf_idx[RFLTE_MC_MAX_TXLM_HANDLES_PER_CARRIER];
  /*Typecast as rflm_lte_dm_buf_type* */
  void *txlm_tech_data[RFLTE_MC_MAX_TXLM_HANDLES_PER_CARRIER]; 
  lm_handle_type fbrx_buf_idx[RFLTE_MC_MAX_FBRX_HANDLES_PER_CARRIER];
  uint8 active_txlm_buf_idx;
  uint8 active_fbrx_buf_idx;

  /*!@brief mutex lock to protect Per Carrier Transactions */
  rf_lock_data_type rflte_mc_tx_carrier_lock;
  /* @breif Flag indicating if "rflte_mc_tx_carrier_lock" mutex was initialized */
  boolean rflte_mc_tx_carrier_lock_init_done;

  /*! To keep the state of the coex filet in the path */
  rfc_coex_filter_status_type gsm_concurrency_path;

  /*! Path override Index to identify the unique Tx/PRx/DRx path to override 
      for each carrier */
  uint8 path_override_index[RFLTE_MC_CARR_TYPE_NUM];

  /*! @brief Flag to indicate if Rx Tune Away(TA) is in progress for this Cell */
  boolean rx_ta_in_progress;

  /*! Notches that have been programmed */
  rflte_mc_programmed_notches_type notches_programmed; 

  /*! Alternate Path for the cell */
  /* PRX = alternate_path[RFLTE_MC_CARR_TYPE_PRX] */
  /* DRX = alternate_path[RFLTE_MC_CARR_TYPE_DRX] */
  /* TX  = alternate_path[RFLTE_MC_CARR_TYPE_TX] */  
  uint8 alternate_path[RFLTE_MC_CARR_TYPE_NUM]; 

  /*! True if valid TQ is assigned to cell*/
  boolean is_tq_allocated_during_rx_tune;

  /* HORxD SM Rx path Bitmask */
  uint8 horxd_path_bitmask;

  /*! @brief Flag indicating whether LTE is in TDD mode*/
  boolean tdd_mode;

  uint8 nv_container[RF_LTE_MAX_DEVICES];

/*! @brief Mask to indicate Intraband CA between carriers */
  uint32 intraband_ca_mask;

  /*! @brief CA mode of the cell */
  rfcom_lte_ca_mode_type ca_mode;

  /*! @brief Flag to indicate if Rx wakeup script optimization is enabled in RX wakeup */
  boolean rx_wakeup_script_opt_flag;

  /*! @brief Flag to indicate if Rx sleep script optimization is enabled in RX sleep */
  boolean rx_sleep_script_opt_flag;

  /*! @brief Flag to indicate the AsDiv state for which scripts are present in handles */
  uint32 asdiv_pos_in_script;

  /*! @brief Tx tune script execution bmsk */
  uint32 tx_tune_script_bmsk;

  /*! @brief Flag to indicate if Tx wakeup script optimization is enabled in TX wakeup */
  boolean tx_wakeup_script_opt_flag;

} rflte_mc_path_state_type; 

/*! 
  @brief
  The structure has info on Device status read
*/

typedef struct 
{
  uint8   channel;            /*!< channel */
  uint8   slave_id;           /*!< slave_id*/
  uint16  reg;                /*!< register*/
  uint8   extended_type;      /*!< extended type*/
} rflte_mc_device_status_reg_list_type;

typedef struct 
{
  boolean device_enabled[RF_LTE_DEVICE_STATUS_READ_DEVICE_SUPPORT];
  uint8 reg_length[RF_LTE_DEVICE_STATUS_READ_DEVICE_SUPPORT];
  rflte_mc_device_status_reg_list_type 
      reg_list[RF_LTE_DEVICE_STATUS_READ_DEVICE_SUPPORT][RF_LTE_DEVICE_STATUS_READ_REG_LIST_LENGTH];
} rflte_mc_device_status_read_type;

/*! 
  @brief
  The structure has info on IRAT state
*/
typedef struct
{
  /*! IRAT has script in progress- flag to disable non sleep requests in MC*/
  boolean lte_mc_in_suspended_Rx_state_for_IRAT;

   uint32 last_irat_band;
   /*!<  Specify target technology band */

   rfcom_lte_earfcn_type last_irat_channel;
   /*!<  Specify source technology DL channel */  
} rflte_mc_irat_state_type; 

/*------------------------------------------------------------------------*/
#if defined(FEATURE_RF_ASDIV)
typedef struct
{
  rfcommon_asdiv_cb_handler_type cb_handler_idx;

  /*Time callback was registered*/
  rf_time_tick_type time;

  /*Number Tag that can be associated with registration*/
  uint32 registration_id;

  /*Id that can be associated with client side to distinguish between callbacks*/
  uint32 client_id;

  rfm_mode_enum_type rfm_mode;

  rfcommon_asdiv_cb_type cb_mode;

}
rflte_mc_asdiv_cb_data_type;

#endif

/*------------------------------------------------------------------------*/
/*!
   @brief
   The structure maintains state variables for LTE.

   @details
   The object of the structure gets updated when state or band changes.
   Rx channel, Tx channel are also maintained here. Tx channel is 
   maintained so that it provides clear distinction for RX and TX functionality 
   when band changes.

   Default for state = Init_state, curr_band = INVALID_BAND, primary_path = RF_PATH_0.

*/
typedef struct
{
  /*! @brief Maintains state and band of each path */
  rflte_mc_path_state_type  rf_path_state[RFA_RF_LTE_MAX_CELL_SUPPORTED];

  /*! @brief Flag indicating whether LTE enter mode has been called after power-up*/
  boolean init_once_flag;

  /*! @brief Flag indicating whether LTE is in FTM mode*/
  boolean ftm_mode;

  /*! @brief Flag indicating whether Critical LTE NVs are presented */
  boolean lte_rfnv_retrieve_success;

  /*! @brief Pointer which points to the table of rfc timing information */
  rfc_timing_info_type *rfc_msm_timing_info;

  /*! state info to track IRAT reqests*/
  rflte_mc_irat_state_type l2l_irat_state;

  /*! @brief Flag indicating whether LTE is in FTM NS mode*/
  boolean ftm_ns_mode;
    
  rfcom_lte_band_type dev_to_rfcom_band_map[RFM_MAX_WAN_DEVICES];

  /*! @brief it will store carrier idx for given band*/
  uint32 band_to_carrier_idx_map[RFCOM_NUM_LTE_BANDS]; 
  
  /*! @bried Flag indicating if dynamic scell switch is enabled or not*/
  boolean dyn_scell_switch_on;

  /*! @brief Flag indicating if Tuner is present or not */
  boolean tuner_present;

  /*! @brief Flag to indicate if TQ wake up is enabled */
  boolean wakeup_tq_enable_flag;

  /*! @brief Flag indicating if RSB cal is on */
  boolean rsb_cal_on;

  /*! @brief Current NS Flag */
  rflte_core_txpl_ns_type active_ns_flag_based_hw_ctl;

  /* ALT Path Information */
  uint8 alt_path[RFCOM_MAX_DEVICES];

  /*! @brief Flag to Track if Intra-band UL CA is active */
  boolean intra_band_ul_ca_active;

  /*Cal Tx BW*/
  rfcom_lte_bw_type cal_tx_bw;

  boolean alt_path_override;
  /*! @brief Buffer to store Band Class Combos */
  rflte_mc_bw_class_combos_s *bw_class_combos_ptr;  

 /*! @brief Active DL carrier mask for Non-Signaling AFC correction */
  uint16 active_dl_carrier_mask;
  #ifdef FEATURE_RF_ASDIV

    rflte_mc_asdiv_cb_data_type asdiv_cb_data;
    rfcommon_asdiv_position_type curr_asdiv_sw_positon;
    rfcommon_asdiv_position_type prev_asdiv_sw_positon;
    /*!@brief Flag indicating Antenna position (related to AsDIV) */
    rfcommon_asdiv_position_enum_type ant_position;

    int8 qat_char_ofst;
	
    /*!@ASD handle to register callback with ASDiv manager 
       whenever there is a change in position */
    rfcommon_asdiv_cb_handler_type asdiv_cb_handle;

    rfcommon_asdiv_cb_handler_type asdiv_gain_update_cb_handle;

    /*!@brief mutex lock to protect AsDiv status flag in SM */
    rf_lock_data_type asdiv_flag_lock;
    /* @breif Flag indicating if ASDiv mutex was initialized */
    boolean asdiv_flag_init_done;
  #endif

  boolean nv_init_done;

  rflte_mc_device_status_read_type device_status_read;

}rflte_mc_state_type;

typedef struct
{
  boolean single_rx_check_for_irat;
  dl_ca_single_rx_type settings;
} rflte_single_rx_irat_return_type;

extern rflte_mc_state_type rflte_mc_state[RFLTE_MC_MAX_SUB_ID];

typedef enum
{
  RF_LTE_MC_SUCCESS,
  RF_LTE_MC_SM_ERROR,
  RF_LTE_MC_STATUS_MAX,
} rflte_mc_status_type;

typedef enum
{
  RFLTE_MC_CMD_SET_QUIET_MODE,
  RFLTE_MC_CMD_MAX = 0xFF,
} rflte_mc_cmd_type;

typedef enum
{
  RFM_DEVICE_0_1_PAIR = 0,
  RFM_DEVICE_2_3_PAIR,
  RFM_DEVICE_4_5_PAIR, 
  RFM_DEVICE_MAX_PAIR,
  RFM_DEVICE_INVALID_PAIR
}rflte_mc_rfm_dev_pair_type;

typedef enum
{
  RFLTE_MC_CDRX_DMCS_SLEEP = 0,
  RFLTE_MC_CDRX_DMCS_WAKEUP,
  RFLTE_MC_CDRX_MODE_MAX
}rflte_mc_cdrx_mode_type;

typedef enum
{
  RFLTE_DEVICE_CFG_UNTOUCH=0,
  RFLTE_DEVICE_CFG_ADD=1,
  RFLTE_DEVICE_CFG_DROP=2,
  RFLTE_DEVICE_CFG_TUNE=3,
} rflte_device_cfg_enum_type;

typedef struct
{
  uint32 carrier_idx;
  rfcom_device_enum_type device;
  rflte_mc_carrier_type  carrier_type;
  lm_handle_type       lm_buffer_index;
  rflte_device_cfg_enum_type cell_cfg_type;
  uint8 num_ul_cells;
} rflte_mc_carrier_info_type;

typedef struct
{
  uint32 carrier_idx;
  rfcom_device_enum_type device;
  rflte_mc_carrier_type  carrier_type;
  lm_handle_type       lm_buffer_index;
  rfcom_lte_band_type band;
  rfcom_lte_earfcn_type rflte_chan;
  uint8	  alt_path;
} rflte_mc_sleep_carrier_info_type;

typedef struct
{
  uint32 carrier_idx;
  rfcom_device_enum_type device;
  rflte_mc_carrier_type  carrier_type;
  rfc_lte_bw_type rflte_sys_bw;
  rfcom_lte_band_type band;
  rfcom_lte_earfcn_type rflte_chan;
  uint8 path_override_index;
} rflte_mc_carrier_info_rfc_type;

/*-----------------------------------------------------------------------*/
/*!
  @brief
  This structure keeps the time profiling for RX Config HW Control Layer
  
*/
typedef struct
{
  /*!> Get MSM timing from RFC time */
  rf_time_tick_type msm_timing_start;
  rf_time_type msm_timing_time;
  
  /*!> Vreg Management Time */
  rf_time_tick_type vregs_start;
  rf_time_type vregs_time;

  /*!> Manage HW and Script Build time */
  rf_time_tick_type script_hw_manage_start;
  rf_time_type script_hw_manage_start_time;

  /*!> Get RSB Info from devices Time */  
  rf_time_tick_type rsb_start;
  rf_time_type rsb_time;

  /*!> TRX and NON TRX HW cleanup Script Time */  
  /* TRX Cleanup */
  rf_time_tick_type trx_cleanup_start;
  rf_time_type trx_cleanup_time;

  /* ASM Cleanup */  
  rf_time_tick_type asm_cleanup_start;
  rf_time_type asm_cleanup_time;

  /* Tuner Cleanup */    
  rf_time_tick_type tuner_cleanup_start;
  rf_time_type tuner_cleanup_time;

  /*!> TRX and NON TRX HW Config Script Time */    
  /* TRX Config*/  
  rf_time_tick_type trx_config_start;
  rf_time_type trx_config_time;

  /* ASM Config*/    
  rf_time_tick_type asm_config_start;
  rf_time_type asm_config_time;

  /* Tuner Config*/    
  rf_time_tick_type tuner_config_start;
  rf_time_type tuner_config_time;

  /*!> MSM Config Script Time */
  rf_time_tick_type msm_config_start;
  rf_time_type msm_config_time;

  /*!> MDSP CCS event creation Time */  
  rf_time_tick_type create_mdsp_event_start;
  rf_time_type create_mdsp_event_time;

  /*!> Transceiver Power vote Time */
  rf_time_tick_type power_vote_start;
  rf_time_type power_vote_time;  

  /*!> CCS Rx Config Script enqueue Time */
  rf_time_tick_type ccs_script_schedule_start;
  rf_time_type ccs_script_schedule_time;

#ifdef FEATURE_RF_ASDIV  
  /* XSW Config*/    
  rf_time_tick_type xsw_config_start;
  rf_time_type xsw_config_time;
#endif

}rflte_mc_rx_hw_ctl_time_type;

/*-----------------------------------------------------------------------*/
/*!
  @brief
  This structure keeps the time profiling for RX Config MC Layer
  
*/
typedef struct
{
  /*!> MC Rx Config time */
  rf_time_tick_type config_start;
  rf_time_type config_start_time;

  /*!> MC Rx Config HW CTL time */  
  rflte_mc_rx_hw_ctl_time_type hw_ctl;
 
}rflte_mc_rx_time_profile_type;

/*-----------------------------------------------------------------------*/
/*!
  @brief
  This structure keeps the time profiling for RX Config HW Control Layer
  
*/
typedef struct
{
  /*!> Vreg Management Time */
  rf_time_tick_type vregs_start;
  rf_time_type vregs_time;

  /*!> Get HW settings Time */
  rf_time_tick_type hw_data_start;
  rf_time_type hw_data_time;

  /*!> Create HW Scipts Time */
  rf_time_tick_type hw_script_start;
  rf_time_type hw_script_time;

  /*!> Get MSM timing from RFC time */
  rf_time_tick_type msm_timing_start;
  rf_time_type msm_timing_time;
    
  /* TRX Config*/  
  rf_time_tick_type trx_config_start;
  rf_time_type trx_config_time;

  /* ASM Config*/    
  rf_time_tick_type asm_config_start;
  rf_time_type asm_config_time;

  /* Tuner Config*/    
  rf_time_tick_type tuner_config_start;
  rf_time_type tuner_config_time;

  /* PA Config*/    
  rf_time_tick_type pa_config_start;
  rf_time_type pa_config_time;

  /* PA Init */    
  rf_time_tick_type pa_init_start;
  rf_time_type pa_init_time;

  /* HDET Config*/    
  rf_time_tick_type hdet_config_start;
  rf_time_type hdet_config_time;

  /* PAPM Config*/    
  rf_time_tick_type papm_config_start;
  rf_time_type papm_config_time;

  /* Coupler Config*/    
  rf_time_tick_type coupler_config_start;
  rf_time_type coupler_config_time;

  /*!> MDSP CCS event creation Time */  
  rf_time_tick_type create_mdsp_event_start;
  rf_time_type create_mdsp_event_time;

  /*!> CCS Rx Config Script enqueue Time */
  rf_time_tick_type ccs_script_schedule_start;
  rf_time_type ccs_script_schedule_time;
}rflte_mc_tx_hw_ctl_time_type;

/*-----------------------------------------------------------------------*/
/*!
  @brief
  This structure keeps the time profiling for RX Config MC Layer
  
*/
typedef struct
{
  /*!> MC Rx Config time */
  rf_time_tick_type config_start;
  rf_time_type config_start_time;

  /*!> MC Rx Config HW CTL time */  
  rflte_mc_tx_hw_ctl_time_type hw_ctl;
 
}rflte_mc_tx_time_profile_type;

/*-----------------------------------------------------------------------*/
/*!
  @brief
  This structure keeps the time profiling for entire MC Layer
  
*/
typedef struct
{
  /*!> MC Tx Config time */
  rflte_mc_tx_time_profile_type tx_time_profile;

  /*!> MC Rx Config time */
  rflte_mc_rx_time_profile_type rx_time_profile[RFLTE_MC_CARR_TYPE_MAX];

}rflte_mc_time_profile_type;

/*-----------------------------------------------------------------------*/
/*!
  @brief
  Time Profile Variables for logging time profile
  
*/
extern rflte_mc_time_profile_type mc_time_prof[RFA_RF_LTE_MAX_CELL_SUPPORTED];

/*-----------------------------------------------------------------------*/
/*!
  @brief
  Flag indicating if Time profiling is enabled
  
*/
extern boolean enable_mc_time_profile;

/* Event IDs for the RF Wakeup CCS Events*/
typedef enum
{
 RFLTE_MC_CCS_WAKEUP,
 RFLTE_MC_CCS_EVENT_NUM
}rflte_mc_ccs_event_type;

/*!
   @brief
   Enumeration to inform the type of RF operation
*/
typedef enum
{
   RFLTE_RX_TUNE,
   RFLTE_RX_EXIT,
   RFLTE_TX_TUNE,
   RFLTE_TX_EXIT,
   RFLTE_RXTX_INVALID,
}  rf_lte_tune_operation_type;

/* CB data type for AsDiv */
typedef struct
{
  /*! @brief  */  
  rfm_device_enum_type device;

  /*! @brief  */  
  rfcom_lte_band_type band;

  boolean immediate;

} rflte_mc_asdiv_get_script_struct_type;

/* Rx Tune params. */
typedef struct
{
  rfcom_lte_earfcn_type  rx_channel[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfcom_lte_bw_type rx_bw[RFA_RF_LTE_MAX_CELL_SUPPORTED];
} rflte_mc_rx_tune_params_struct_type;

/* Tx Tune params. */
typedef struct
{
  rfcom_lte_earfcn_type  tx_channel[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];
  rfcom_lte_bw_type tx_bw[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];
} rflte_mc_tx_tune_params_struct_type;


/* Tuner input params required for MCL */
typedef struct
{
  uint8 scenario_val;
  rfm_tuner_handle_type tuner_handle;
  boolean is_scenario_valid;
} rflte_mc_mcl_params_s;

/*-----------------------------------------------------------------------*/
/*!
  @brief
  This structure holds BW spacing info between carriers (Single Rx scenario).
 
         A     |     D
    -----------|-----------         A    --> BW of First carrier from Left 
             |B|C|                  B+C  --> BW of First carrier from Left 
    | FIRST  | | | SECOND |         D    --> BW of Second carrier from Left 
    |CARRIER | | |CARRIER |
    | FROM   | | | FROM   |
    | LEFT   | | | LEFT   |
    ------------------------------------------------>>> Freq Domain 
*/
typedef struct
{
  /*!> Number of Carriers. */
  uint8 num_of_carriers;

  /*!> BW Info of First Carrier from Left.*/
  uint32 rflte_bw_first_carrier_from_left;

  /*!> BW Gap between First Carrier from Left.*/
  uint32 rflte_bw_gap_first_vs_second_carrier;

  /*!> BW Info of Second Carrier from Left.*/
  uint32 rflte_bw_second_carrier_from_left;

}rflte_mc_carrier_bw_spacing_s_type;

/*------structure to hold input parameters to rflte_mc_common api to set Rx config active flag-------------*/
typedef struct
{
  uint32 carrier_idx_mask;
  uint16 path_bitmask[RFA_RF_LTE_MAX_CELL_SUPPORTED];  
  rfa_rf_chain_param_type prx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx2[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx3[RFA_RF_LTE_MAX_CELL_SUPPORTED];
} rflte_rx_config_active_flag_req_s;

/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/
#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR
void rf_lte_QSH_mdump_cb(void);
#endif
/*-------------------------------------------------------------------------*/
boolean rf_lte_msgr_register( msgr_client_t *client_id, msgr_id_t id );

/*----------------------------------------------------------------------------*/
boolean rf_lte_msgr_deregister( msgr_client_t *client_id );

/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_enter_mode( rflte_mc_carrier_info_type carrier_info );

/*----------------------------------------------------------------------------*/

#if 0
/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_fdd_rx_config( rflte_mc_carrier_info_type carrier_info,
                                             rfcom_lte_earfcn_type rx_channel,
                                             rfcom_lte_bw_type bw,
                                             lm_handle_type rxlm_buffer_idx,
                                             rflte_mc_mcl_params_s rflte_mc_mcl_params,
                                             rflte_mc_rx_tune_params_struct_type rx_tune_params,
                                             boolean rf_execute_tune_script);

/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_fdd_tx_config( rflte_mc_carrier_info_type carrier_info,
                                             rfcom_lte_earfcn_type tx_channel,
                                             rfcom_lte_bw_type bw,
                                             int32 rx_freq_error,
                                             lm_handle_type txlm_buffer_idx );

/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_fdd_tx_disable( rflte_mc_carrier_info_type carrier_info, 
                                              lm_handle_type txlm_buffer_idx );

/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_tdd_rx_config( rflte_mc_carrier_info_type carrier_info,
                                             rfcom_lte_earfcn_type rx_channel,
                                             rfcom_lte_bw_type bw,
                                             lm_handle_type rxlm_buffer_idx,
                                             rflte_mc_mcl_params_s rflte_mc_mcl_params,
                                             rflte_mc_rx_tune_params_struct_type rx_tune_params,
                                             boolean rf_execute_tune_script);

/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_tdd_tx_config( rflte_mc_carrier_info_type carrier_info,
                                             uint32 tx_channel,
                                             rfcom_lte_bw_type bw,
                                             rfa_rf_lte_tdd_ul_dl_cfg_type tdd_ul_dl_cfg,
                                             int32 rx_freq_error,
                                             lm_handle_type txlm_buffer_idx );
/*----------------------------------------------------------------------------*/
#endif
rflte_mc_status_type 
rflte_mc_tx_config(uint8                               sub_id,
                   rflte_mc_carrier_info_type          *carrier_info,
                   rfcom_lte_earfcn_type               *tx_channel,
                   rfcom_lte_bw_type                   *tx_bw,
                   int32                               rx_freq_error,
                   boolean                             rf_execute_tx_script,
                   rfa_rf_lte_tdd_ul_dl_cfg_type       tdd_ul_dl_cfg,
                   uint32                              *script_exec_btmsk,
                   uint8                               num_ul_cells,
                   uint8                               active_carr_idx_btmsk); 

/*----------------------------------------------------------------------------*/

#if 0
/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_tdd_tx_disable( rflte_mc_carrier_info_type carrier_info,
                                                 lm_handle_type txlm_buffer_idx );

/*----------------------------------------------------------------------------*/
#endif
void rflte_mc_tx_config_assign_tx_nv_ptr( uint8 sub_id,
	                                  rfcom_lte_band_type split_band, 
                                          rflte_mc_carrier_info_type carrier_info );

/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_sleep(uint8 sub_id, rflte_mc_carrier_info_type carrier_info );

#if 0
/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_wakeup( rflte_mc_carrier_info_type carrier_info,
                                      lm_handle_type rxlm_buffer_idx,
                                      boolean offline_wakeup_flag,
                                      rflte_mc_mcl_params_s rflte_mc_mcl_params);

/*----------------------------------------------------------------------------*/
#endif
int32 rflte_mc_cmd_dispatch( rfcom_device_enum_type device,int32 cmd, void *data);

/*----------------------------------------------------------------------------*/
void rf_lte_dispatch_init(void);

/*----------------------------------------------------------------------------*/
void rf_lte_dispatch_deinit(void);

/*----------------------------------------------------------------------------*/
uint32 rflte_mc_override_tuner_code(uint8 sub_id,
                                    rfcom_device_enum_type device,
                                    uint8 override_flag,
                                    void *data,
                                    uint8* tuner_nv_ptr,
                                    uint8 *tuner_id);
/*----------------------------------------------------------------------------*/

uint32 rflte_mc_get_cell_type_based_on_device(uint8 sub_id,
                                              rfcom_device_enum_type prx_device, 
                                              rfcom_device_enum_type drx_device );

/*----------------------------------------------------------------------------*/
/*!
  @brief

  @details

  @param common_init_success: if common initialization was successful.
  @param device: RF device
  @param caller_tcb_ptr: Calling task TCB pointer
  @param task_nv_wait_sig: Task signal to wait for when reading NV
  @param task_wait_func_ptr: Task's wait function to be called when reading NV
*/
boolean rflte_mc_init(rfcom_device_enum_type device,
                      boolean common_init_success, 
                      rex_tcb_type *caller_tcb_ptr,
                      rex_sigs_type task_nv_wait_sig,
                      void (*task_wait_func_ptr)( rex_sigs_type ));

/*----------------------------------------------------------------------------*/

int32 rflte_mc_tx_update_sar_backoff(uint8 sub_id,rfcom_device_enum_type device);

/*----------------------------------------------------------------------------*/
/*!
  @brief

  @details This API is used to populate all the needed backoffs for MTPL
  computation as passed by ML1 in the plimit_type_mask

  @param carrier_idx             : Cuurent carrier index
  @param plimit_type_mask        : Bit mask to indicate the required backoffs for MTPL computation
  @param sar_limit_dB10          : SAR backoff
  @param coex_limit_dB10         : WLAN Co-existence backoff
  @param wcn_limit_dB10          : WCN backoff
  @param subframe_coex_limit_dB10: Subframe Co-ex backoff
  @param vbatt_limit_dB10        : Vbatt backoff
  @param pa_thermal_limit_dB10   : PA thermal backoff
*/
void rflte_mc_tx_sg_set_txplim(uint8  sub_id,
                               uint32 carrier_idx,
                               lm_handle_type txlm_buffer_idx, uint32 plimit_type_mask,
                               int16 sar_limit_dB10, int16 coex_limit_dB10,
                               int16 wcn_limit_dB10, int16 subframe_coex_limit_dB10,
                               int16 vbatt_limit_dB10, int16 pa_thermal_limit_dB10);

/*----------------------------------------------------------------------------*/
boolean rflte_mc_reload_nv
(
  rfcom_device_enum_type device,
  rex_tcb_type *caller_tcb_ptr,
  rex_sigs_type task_nv_wait_sig,
  void (*task_wait_func_ptr)( rex_sigs_type )
);

/*----------------------------------------------------------------------------*/
boolean rflte_mc_set_pmic_quiet_mode(boolean enable);

/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_get_txpll_script
(
uint8  sub_id,
/* For D3925 Bring up Build, stubbing out
features dependent on other layers */
uint32 carrier_idx, 
int32 rx_freq_error_in_hz, 
lm_handle_type txlm_buffer_idx,
void *txpll_sbi_script_buf_ptr
);

boolean
rflte_mc_tx_update_pll_scripts
(
  uint8  sub_id,
  uint32 carrier_idx, 
  int32  rx_freq_error_in_hz, 
  lm_handle_type txlm_buffer_idx,
  void  *txpll_sbi_script_buf_ptr
);

/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_get_rxpll_script
(
  uint8  sub_id,
  uint32 carrier_idx, 
  void *rxpll_script_buf_ptr
);

/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_connected_mode(boolean enable);

/*----------------------------------------------------------------------------*/

rfcom_lte_band_type rflte_mc_sm_get_band( uint8 carr_index );

/*----------------------------------------------------------------------------*/
void rflte_mc_get_rfc_timing_data(uint8 sub_id,rfm_device_enum_type device, rfcom_lte_band_type rf_band);

/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_create_wakeup_events_tq(uint8 sub_id,
                                                      rflte_mc_carrier_info_type carrier_info, 
                                                      rf_hal_rfcmd_tq_type *wakeup_tq_ptr);
/*----------------------------------------------------------------------------*/
boolean 
rflte_mc_get_ftm_mode
(
  uint8 sub_id
);
/*----------------------------------------------------------------------------*/
#if 0
void 
rflte_mc_set_ftm_mode
(
boolean ftm_mode
);
#endif
/*----------------------------------------------------------------------------*/
boolean 
rflte_mc_get_tuner_present
(
  uint8 sub_id
);
/*----------------------------------------------------------------------------*/
void 
rflte_mc_set_tuner_present
(
  uint8 sub_id,
  boolean status
);
/*----------------------------------------------------------------------------*/
rfm_device_enum_type 
rflte_mc_get_rfc_remapped_device_for_scell
(
  uint8 sub_id,
  rfm_device_enum_type rfm_device,
  rfcom_lte_band_type band
);
/*----------------------------------------------------------------------------*/
boolean 
rflte_mc_validate_rfcom_band_using_nv_valid_data_flag
(
rfcom_lte_band_type rfcom_band
);
/*----------------------------------------------------------------------------*/
uint64 
rflte_mc_get_supported_band_config
(
void
);
/*----------------------------------------------------------------------------*/
boolean
rflte_mc_tx_hw_ctl_get_device_info
(
  rfm_device_enum_type device,
  rfcom_lte_band_type band,
  rfcommon_core_txlin_agc_device_info *device_info
);
/*----------------------------------------------------------------------------*/

boolean rflte_mc_trigger_asm_update(uint8 sub_id, rfa_rf_lte_dynamic_asm_update_ind_s* msg_ptr);

#ifdef FEATURE_RF_ASDIV
/*----------------------------------------------------------------------------*/
void rflte_mc_switch_ant(uint8 sub_id,
                         rfm_device_enum_type device,
                         rfcommon_asdiv_position_type to_pos,
                         rfm_l1_cb_type cb_func,
                         void *cb_data,
                         boolean immediate);
/*----------------------------------------------------------------------------*/
void rflte_mc_switch_ant_abort(uint8 sub_id,
                               rfm_device_enum_type device,
                               rfcommon_asdiv_position_type to_pos,
                               rfm_l1_cb_type cb_func,
                               void *cb_data);
/*----------------------------------------------------------------------------*/
void rflte_mc_send_switch_ant_msgr
(
   rfm_device_enum_type device,
   rfcommon_asdiv_position_type to_pos,
   rfm_l1_cb_type cb_func,
   void *cb_data,
   boolean immediate
);
/*----------------------------------------------------------------------------*/
void rflte_mc_send_switch_ant_abort_msgr
(
   uint8 sub_id,
   rfm_device_enum_type device,
   rfcommon_asdiv_position_type to_pos,
   rfm_l1_cb_type cb_func,
   void *cb_data
);
/*----------------------------------------------------------------------------*/
boolean rflte_mc_common_get_asdiv_script(rfcommon_asdiv_position_type ant_sw_pos, /*!< New Antenna Position */
                                         rf_buffer_intf *rf_buffer, /*!< pointer to buffer to store Script */
                                         void *cb_data /*!< Callback data to be passed to the function */);


/*----------------------------------------------------------------------------*/
boolean rflte_mc_common_get_xsw_sleep_script(rfcommon_asdiv_position_type ant_sw_pos, /*!< New Antenna Position */
                                         rf_buffer_intf *rf_buffer, /*!< pointer to buffer to store Script */
                                         void *cb_data /*!< Callback data to be passed to the function */);

/*----------------------------------------------------------------------------*/
void rflte_mc_asdiv_cb_func
(
 void *asdiv_cb_data,  /*!< Callback data to be passed to the function */
 rfcommon_asdiv_position_type *sw_pos /*!< New Antenna Position */
);

#endif

/*----------------------------------------------------------------------------*/
rflte_mc_status_type 
rflte_mc_ul_ca_config_update_params(uint8                                    sub_id,
                                    rfa_rf_lte_tx_script_based_config_req_s* rf_lte_msg_ptr, 
                                    uint32                                   parent_cell_index,
                                    boolean                                  *ul_ca_db_update
                                   );

/*----------------------------------------------------------------------------*/
#if 0
void 
rflte_mc_single_tx_ca_get_state
(
  uint8 sub_id,
  ul_ca_single_tx_type *ul_ca_single_tx_mc_state_ptr,
  uint32 cell_index
);
/*----------------------------------------------------------------------------*/
void 
rflte_mc_ul_ca_get_state
(
ul_ca_single_tx_type    *ul_ca_single_tx_mc_state_ptr,
uint32                   cell_index
);
/*----------------------------------------------------------------------------*/
void 
rflte_mc_reset_single_tx_ca_mc_state
(
uint32 cell_index
);
/*----------------------------------------------------------------------------*/
void 
rflte_mc_single_tx_ca_mc_reset_state
(
uint32 cell_index
);
/*----------------------------------------------------------------------------*/
void 
rflte_mc_ul_ca_mc_reset_state
(
uint32 cell_index
);

/*----------------------------------------------------------------------------*/
#endif 

boolean 
rflte_mc_is_tx_cell_member_single_tx_ca
(
  uint8 sub_id,
  uint32 cell_index
);
/*----------------------------------------------------------------------------*/
boolean rflte_mc_is_tx_cell_member_ul_ca(uint8 sub_id,uint32 cell_index);
/*----------------------------------------------------------------------------*/
#if 0
void 
rflte_mc_ul_ca_update_rf_state
(
uint32      parent_cell_index, 
uint32      umid
);
/*----------------------------------------------------------------------------*/
void 
rflte_mc_ul_ca_disable_reset_mc_state
(
uint32 parent_cell_index
);
/*----------------------------------------------------------------------------*/
#endif
uint32 
rflte_mc_single_tx_ca_get_next_cell_index
(
  uint8  sub_id,
  uint32 cell_index
);
/*----------------------------------------------------------------------------*/
uint32 
rflte_mc_ul_ca_get_next_cell_index
(
  uint8      sub_id,
  uint32     cell_index
);
/*----------------------------------------------------------------------------*/
uint32 
rflte_mc_single_rx_ca_get_next_cell_index
(
  uint8  sub_id,
  uint32 cell_index
);
/*----------------------------------------------------------------------------*/
boolean rflte_mc_is_single_rx_ca_with_cell_index(uint8 sub_id, uint32 cell_index, uint32 target_cell_index);
/*----------------------------------------------------------------------------*/
boolean 
rflte_mc_is_rx_cell_member_single_rx_ca
(
  uint8 sub_id,
  uint32 cell_index
);
/*----------------------------------------------------------------------------*/
#if 0
void 
rflte_mc_reset_single_rx_ca_mc_state
(
uint32 cell_index
);
void 
rflte_mc_single_rx_ca_reset_mc_state
(
uint32 cell_index
);
#endif
#if 0
/*----------------------------------------------------------------------------*/

\\Declared below Function in two files: rflte_mc.h, rflte_state.h : removing Here
void 
rflte_mc_single_rx_ca_exit_reset_mc_state
(
uint32 parent_cell_index
);
#endif
/*----------------------------------------------------------------------------*/
void 
rflte_mc_single_rx_ca_update_rf_state
(
  uint8 sub_id,
  uint32 parent_cell_index, 
  uint32 umid,
  boolean skip_pcell_update
);
/*----------------------------------------------------------------------------*/
void 
rflte_mc_single_rx_ca_get_state
(
  uint8 sub_id,
  dl_ca_single_rx_type *ul_ca_single_rx_mc_state_ptr,
  uint32 cell_index
);
/*----------------------------------------------------------------------------*/
void 
rflte_mc_single_rx_ca_exit_update_dm
(
  uint8 sub_id
);
/*----------------------------------------------------------------------------*/
uint32 
rflte_mc_single_rx_ca_get_num_rx_cells
(
  uint8 sub_id,
  uint32 carrier_index
);
/*----------------------------------------------------------------------------*/
uint32 
rflte_mc_single_rx_ca_get_parent_cell
(
  uint8 sub_id,
  uint32 cell_index
);
/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_single_rx_ca_config_update_params
(
uint8                                      sub_id,
rfa_rf_lte_rx_script_based_config_req_s   *rf_lte_msg_ptr,
uint32                                     parent_cell_index,
uint32                                    *carrier_idx_mask_ptr,
rflte_single_rx_irat_return_type          *single_rx_return_ptr  
);

#if 0
/*----------------------------------------------------------------------------*/
rflte_mc_status_type 
rflte_mc_single_rx_ca_tdd_config_update_params
(
rfa_rf_lte_tdd_rx_config_req_s   *rf_lte_msg_ptr,
uint32                            parent_cell_index,
uint32                           *carrier_idx_mask_ptr
);
/*----------------------------------------------------------------------------*/
#endif

rflte_mc_status_type 
rflte_mc_single_rx_ca_exit_update_params
(
  uint8                           sub_id, 
  rfa_rf_lte_exit_mode_req_s     *rf_lte_msg_ptr, 
  uint32                          parent_cell_index,
  uint32                         *carrier_idx_mask_ptr,
  boolean                         skip_pcell_update
);
/*----------------------------------------------------------------------------*/
rflte_mc_status_type 
rflte_mc_single_rx_ca_sleep_update_params
( 
  uint8                           sub_id, 
  rfa_rf_lte_sleep_req_s         *rf_lte_msg_ptr, 
  uint32                          parent_cell_index,
  uint32                         *carrier_idx_mask_ptr
);
/*----------------------------------------------------------------------------*/
rflte_mc_status_type 
rflte_mc_single_rx_ca_wakeup_update_params
(
  uint8                             sub_id,
  rfa_rf_lte_wakeup_req_s          *rf_lte_msg_ptr,
  uint32                            parent_cell_index,
  uint32                           *carrier_idx_mask_ptr
);
/*----------------------------------------------------------------------------*/
uint8 
rflte_mc_single_rx_ca_get_intra_band_ca_support
(
  rfcom_lte_band_type      band
);
/*----------------------------------------------------------------------------*/
uint8 
rflte_mc_single_rx_ca_get_intra_band_ca_support_class_d
(
  rfcom_lte_band_type      band
);
/*----------------------------------------------------------------------------*/

void rflte_mc_tx_pwr_ctl_script_setup 
(
  uint8                       sub_id,
  rflte_mc_carrier_info_type  carrier_info,
  rfcom_lte_band_type         tx_band,
  rfcom_lte_bw_type           tx_bw,
  rfcom_lte_earfcn_type       tx_channel
);

void rflte_mc_fbrx_coupler_ctl_script_setup 
(
  uint8                        sub_id,
  rflte_mc_carrier_info_type   carrier_info,
  rfcom_lte_band_type          tx_band
);

void rflte_mc_fbrx_path_ctl_script_setup
(
  uint8                       sub_id,
  rflte_mc_carrier_info_type  carrier_info,
  rfcom_lte_band_type         tx_band
);

void rflte_mc_fbrx_ldo_disable_seq 
(
  uint8                       sub_id,
  rflte_mc_carrier_info_type  carrier_info,
  rfcom_lte_band_type         tx_band,
  boolean                     slow_ramp_status_flag
);
/*----------------------------------------------------------------------------*/

rflte_nv_tbl_type* 
rflte_mc_rx_config_assign_rx_nv_ptr
(
rfcom_lte_band_type            split_band,
rflte_mc_carrier_info_type     carrier_info
);

#if 0
/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_rx_configure_chain( rflte_mc_carrier_info_type carrier_info, 
                                                   lm_handle_type rxlm_buffer_idx,
                                                  boolean enable,
                                                  rflte_mc_mcl_params_s rflte_mc_mcl_params);
/*----------------------------------------------------------------------------*/
#endif

rflte_mc_status_type 
rflte_mc_tx_disable_update_states
(
  uint8                           sub_id,
  rflte_mc_carrier_info_type     *carrier_info
);

/*----------------------------------------------------------------------------*/

rflte_mc_status_type 
rflte_mc_tx_reset_tx_fed_static_dm_update
(
  uint8                           sub_id,
  rflte_mc_carrier_info_type     *carrier_info
);

/*----------------------------------------------------------------------------*/
boolean rflte_mc_rx_spur_mitigation_cb
( 
  rflte_mc_cell_idx_type carrier_index,
  rfm_device_enum_type rfm_device,
  rfcom_lte_band_type rf_band,
  rfcom_lte_bw_type rf_bw,
  uint32 freq_khz,
  boolean intra_band_ca_active_device,
  boolean retune_done,
  rfdevice_lte_retune_reqd_event_type retune_event,
  rfdevice_lte_get_rx_gain_settings_type* gain_settings,
  rfdevice_meas_script_data_type* script_data_ptr,
  rfdevice_wtr_config_bitmask_type *wtr_config_bitmask
);


rflte_mc_status_type rflte_mc_exit_cleanup(uint8 sub_id, rflte_mc_carrier_info_type carrier_info );

boolean rflte_mc_destroy_sm_script_data_ptr(uint8 sub_id);

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Release LTE TQ pair 

  @details
  Check if the TQ pair is allocated and release if valid device

  @param
  carrier_info
*/

boolean rflte_mc_common_release_tq_pair(uint8 sub_id,rflte_mc_carrier_info_type *carrier_info);

/*----------------------------------------------------------------------------*/

rflte_mc_status_type rflte_mc_rx_tune_scripts_init_hal_buffer ( void );

/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_rx_tune_scripts_deinit_hal_buffer ( void );

/*----------------------------------------------------------------------------*/
rf_buffer_intf *rflte_mc_rx_tune_scripts_get_hal_buffer ( rflte_mc_rx_tune_scripts_type_s script_type );

/*----------------------------------------------------------------------------*/
void rflte_mc_rx_config_clean_time_profile ( uint32 carrier_idx );

/*----------------------------------------------------------------------------*/

rflte_mc_status_type rflte_mc_tx_tune_scripts_init_hal_buffer ( void );

/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_tx_tune_scripts_deinit_hal_buffer ( void );

/*----------------------------------------------------------------------------*/
rf_buffer_intf *rflte_mc_tx_tune_scripts_get_hal_buffer ( rflte_mc_tx_tune_scripts_type_s script_type );

/*----------------------------------------------------------------------------*/
void rflte_mc_tx_config_clean_time_profile ( uint32 carrier_idx );

#if 0
/*----------------------------------------------------------------------------*/
void rflte_mc_tx_internal_split_intraband_retune(rfcom_lte_band_type new_tx_band, 
                                                 rflte_mc_carrier_info_type carrier_info,
                                                 rfdevice_lte_retune_reqd_event_type retune_event);
/*----------------------------------------------------------------------------*/

void rflte_mc_rx_internal_split_intraband_retune(uint8      sub_id,
                                                 rfcom_lte_band_type new_rx_band, 
                                                 rflte_mc_carrier_info_type carrier_info,
                                                 rfcom_lte_ca_mode_type ca_mode,
                                                 lm_handle_type rxlm_buffer_idx,
                                                 rfcom_lte_earfcn_type rx_channel);

/*----------------------------------------------------------------------------*/
#endif

rflte_mc_carrier_bw_spacing_s_type rflte_mc_rx_get_bw_spacing_info(uint32 carrier_idx, 
                                                                   rflte_mc_rx_tune_params_struct_type rx_tune_params,
                                                                   uint32  single_rx_next_cell_info[RFA_RF_LTE_MAX_CELL_SUPPORTED]);

/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_common_exit_mode_reset_states
(
  uint8                           sub_id,
  rflte_mc_carrier_info_type     *carrier_info,
  boolean                         skip_pcell_update
);

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_common_reset_states( uint8                        sub_id,
                                                   rflte_mc_carrier_info_type  *carrier_info,
                                                   boolean                      skip_pcell_update);

/*----------------------------------------------------------------------------*/

#ifdef FEATURE_RF_ASDIV
void rflte_mc_update_tuner_code_imm
(
  uint8 cb_data
);

void rflte_mc_common_rx_config_set_active_flag(uint8 sub_id, rflte_rx_config_active_flag_req_s *rflte_rx_config_flag_ptr );

/*----------------------------------------------------------------------------*/
rflte_mc_sar_forced_antenna_type rflte_mc_get_sar_forced_ant(void);

/*----------------------------------------------------------------------------*/
#endif/* FEATURE_RF_ASDIV */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifdef FEATURE_RF_HAS_QTUNER
rflte_mc_status_type rflte_mc_common_tuner_config(uint8 sub_id);

/*----------------------------------------------------------------------------*/
rfcommon_atuner_carrier_id_type 
rflte_mc_common_get_atuner_carrier_id_type
(
  uint32 carrier_idx
);
/*----------------------------------------------------------------------------*/
rfcommon_atuner_device_type  
rflte_mc_common_get_atuner_device_type
(
  rflte_mc_carrier_type carrier_type
);
/*----------------------------------------------------------------------------*/
#endif /* FEATURE_RF_HAS_QTUNER */
/*----------------------------------------------------------------------------*/

boolean rflte_mc_program_atuner_scenario_async(rf_buffer_intf *tuner_script_buff);

boolean rfcommon_append_async_tp_tuner_script(rf_buffer_intf *tuner_wakeup_script_buff);

#if 0
boolean rflte_mc_is_band_intra_band_ulca_supported(rfcom_lte_band_type band);
#endif

#define RFLTE_IS_IDX_USED(idx_mask, idx) (((idx_mask & (1 << idx)) > 0) ? TRUE : FALSE)


typedef struct
{
  /*!> Carrier index to tune to*/
  uint32    carrier_index;
  /*!> Rx band to tune to*/
  rfcom_lte_band_type     rx_band;
  /*!> Rx channel to tune to*/
  rfcom_lte_earfcn_type   rx_channel;
  /*!> Rx BW to use for tunning*/
  rfcom_lte_bw_type       rx_bw;
  /*! rx frequency*/
  uint32                  rx_freq;
  /*!> path type*/
  uint32                  path_type;
  /*!> Rx device to tune*/
  rfcom_device_enum_type  device;  
  /*!> Rx LM handle */
  lm_handle_type    rxlm_buffer_handle;  
  /*!> Rx script handle */
  uint32 tune_script_handle;
  /*!> Rx tune alternate path*/
  uint32       alternate_path;
  /*!> intra CA active flag*/
  boolean     intra_band_ca_active;
  /*!> TRUE = RF to execute script*/
  boolean rf_execute_scripts;
  /*!> Calibration state */
  boolean is_calibration_state;
  /*!> TRUE = Write to HW immediately*/
  boolean write_to_hw;
  /*!> TRUE = Spur Mitigation disabled. Necessary in claibration mode*/
  boolean disable_spur_mitigation;

}rflte_mc_rx_config_params_s_type;

/*!
   @brief
   LTE component carrier enums in RFLM.

*/
typedef enum {
  RFLTE_COMPONENT_CARRIER_0 = 0,
  RFLTE_COMPONENT_CARRIER_1,
  RFLTE_COMPONENT_CARRIER_2,
  RFLTE_COMPONENT_CARRIER_3,
  RFLTE_COMPONENT_CARRIER_4,
  RFLTE_COMPONENT_CARRIER_MAX
} rflte_component_carrier_type;


typedef struct
{
  uint32                  carrier_index;
  uint8                   path_type;
  rfcom_device_enum_type  device;
  lm_handle_type          rxlm_buf_handle;                          
  rfcom_lte_band_type     rx_band;
  rfcom_lte_earfcn_type   rx_channel;
  rfcom_lte_bw_type       rx_bw;
  rfcom_lte_ca_mode_type  ca_mode;
}rflte_mc_rx_current_params_s_type;

typedef struct 
{ 
  rfdevice_lte_script_data_type trx_config_preload_script_buf;
  rfdevice_lte_script_data_type trx_config_trigger_script_buf;
  rfdevice_lte_script_data_type non_trx_script_buf;
} rflte_mc_rx_config_script_pointers_s_type;


boolean rflte_mc_allocate_tq_pair ( uint8                       sub_id,
	                           rflte_mc_carrier_info_type   carrier_info,
                                    lm_handle_type               rxlm_buffer_idx );



boolean rflte_mc_get_wb_index(lm_handle_type rxlm_handle, uint8* wb_idx);
/*----------------------------------------------------------------------------*/
/* rflte_mc_tx_disable */
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This API commits the disable state variable to the RFLTE state machine.

  @details
  This API commits the disable state variable to the RFLTE state machine.
  This API will be called by the scheduler to ensure proper SM state.
  
  @param carrier_info
  Contains carrier_index, device, and carrier type.

  @param txlm_buffer_index
  The TXLM Buffer index on which Disable has been called for.
 
  @return
  rflte_mc_status_type

*/
rflte_mc_status_type rflte_mc_tx_disable(uint8                        sub_id,
                                         rflte_mc_carrier_info_type   carrier_info,
                                         lm_handle_type               txlm_buffer_index);

#if 0
/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_asm_update_state(uint8 sub_id);

#endif

uint8 rflte_mc_get_alt_path(uint8                        sub_id,
                            uint32                       carrier_idx, 
                            rflte_device_cfg_enum_type   device_cfg_type, 
                            rfcom_device_enum_type       device);

void rflte_mc_update_alt_path_to_sm(uint8                   sub_id,
	                            uint32                  carrier_idx, 
                                    rfcom_device_enum_type  device,
                                    uint8                   alt_path);

/*----------------------------------------------------------------------------*/
const rflte_mc_rx_config_params_s_type * const  rflte_mc_config_get_rx_tune_params(uint8 sub_id, uint32 carrier_idx, uint32 path_idx);

/*----------------------------------------------------------------------------*/
const rfa_rf_config_param_type_s * const  rflte_mc_config_get_chain_cfg_ptr(uint8 sub_id,uint32 carrier_idx, uint32 path_idx);

/*----------------------------------------------------------------------------*/
rfm_device_enum_type rflte_mc_config_get_device_from_chain_cfg(uint8 sub_id, uint32 carrier_idx, uint32 path_idx);

/*----------------------------------------------------------------------------*/
lm_handle_type rflte_mc_config_get_rxlm_handle(uint8 sub_id,uint32 carrier_idx, uint32 path_idx);

/*----------------------------------------------------------------------------*/
rfcom_lte_band_type rflte_mc_config_get_rx_band_from_chain_cfg(uint8 sub_id,uint32 carrier_idx, uint32 path_idx);

/*----------------------------------------------------------------------------*/
boolean rflte_mc_update_device_status_registers(uint8 sub_id,uint8 device, rf_buffer_intf* script, int32 delay);

/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_tx_wakeup(uint8           sub_id,
                                        uint32          carrier_index,
                                        lm_handle_type  txlm_buffer_index,
                                        uint32          *script_exec_btmsk);

/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_tx_sleep(uint8           sub_id,
                                       uint32          carrier_index, 
                                       lm_handle_type  txlm_buffer_idx );

void rflte_mc_common_set_active_sub_id(uint8 sub_id);

uint8 rflte_mc_common_get_active_sub_id(void);

void rflte_mc_common_set_mismatch_sub_id(uint8 sub_id);

uint8 rflte_mc_common_get_mismatch_sub_id(void);

void rflte_mc_set_rx_exit_status_on_sub(uint8 sub_id,boolean status);

boolean rflte_mc_get_rx_exit_status_on_sub(uint8 sub_id);

void rflte_mc_common_set_tx_disable_status(boolean status);

boolean rflte_mc_common_get_tx_disable_status();

void rflte_mc_set_sub_id_mismatch_status(boolean status);

boolean rflte_mc_get_sub_id_mismatch_status(void);

void rflte_mc_wtr_status_reg_read(void);

rfcom_mode_enum_type rflte_mc_get_device_rfm_mode(rfcom_device_enum_type device);

/*----------------------------------------------------------------------------*/
boolean rflte_mc_check_if_band_is_calibrated
(
  rfcom_lte_band_type curr_band
);

#ifdef __cplusplus
}
#endif

#endif /* RFLTE_MC_H */


