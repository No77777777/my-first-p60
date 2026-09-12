 /*!
  @file
  rflte_msg.h

  @brief
  Message router UMIDs header for RF LTE subsystem. This file defines
  interfaces in the RF task.

  @note
  This file must be named with a _msg.h suffix for the build environment
  to correctly process the messages defined within.

*/

/*===========================================================================

  Copyright (c) 2009 - 2015 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header  : //source/qcom/qct/modem/rfa3/main/3.0/api/lte/rflte_msg.h $
$DateTime: 2018/04/15 02:22:08 $
$Author  : $

when       who     what, where, why
--------   ---     ------------------------------------------------------------------
04/14/16   sk      FBRX Tune-away support Phase 2
12/22/15   sk      Add Command to do RX Only disable
12/02/15   sg      [ML1->RF Interface] Make TxPL APIs handle based
11/25/15   am      asm trigger update for tdd split bands.
11/04/15   stw     Packing removed from Structures that include non-pack msgr_hdr_struct_type
11/04/15   mah     Changes to enable handle bases TxPL SM.
11/04/15   mah     Added RFA_RF_LTE_MSG_ID_CONFIG_COMPLETE Confirm msg.
10/30/15   sg      [ML1->RF Interface] Make TxPL APIs handle based
10/15/15   vc      change max Number of chains configure input tune based on modem 
09/21/15   sk      Make AFC TxLM Handle based
09/17/15   dyc     Support for DMCS power mode
08/28/15    sg     FR28508: Add ULCA Support for MTPL Backoff due to PA thermal
08/25/15   jz      Add non-trx message for device hopping
08/17/15   pkg     Changes to handle ULCA while accessing DM.
08/14/15   ska     Increase number of Max Tx Cells to 3
08/12/15   pkg     WTR Swap Changes for Tx.
08/11/15   ska     Adding support for ULCA SCC2
08/12/15   vc      WTR Hopping changes
08/04/15   vc      WTR hopping changes
07/20/15   pkg     Changes to support FR 24616:Algorithm to mitigate VBatt
                   droop (down to 2.5V) via TX backoff.
07/18/15   bmg     Added ML1-RF check config version 2 interface
06/30/15   jd      Wtr hopping phase1 changes.
06/25/15   cj      L2L Interface change
05/20/15   pkg     Interface Changes for FR 24616 Algorithm to mitigate 
                   VBatt droop (down to 2.5V) via Tx backoff for THOR/MAV16.
05/19/15   sdu     Added MSG router support for FTM_SET_PATH_SEL_VERIFICATION_INDEX command 
05/12/15   pkg     Changes to support Freq Comp for Intra Band UL CA.
04/23/15   pkg     Changes to enable independent scouting for Rx and Tx.
04/16/15   kab     Cleanup ununsed MSGs
03/23/15   stw     USTMR timestamp added to rfa_rf_lte_rx_script_based_config_cnf_s
01/14/15   pkg     Interface changes for Script based Tx Tune.
01/12/15   pkg     Scouting API changes for UL CA Feature.
12/10/14   kab     ML1-RF interface changes for IRAT to support Single Rx
11/20/14   php     ML1-RF interface changes for IRAT to support HORxD/other fixes
10/20/14   vb      ASDiv immediate script support
09/30/14   kab     RF-ML1 Interface changes for Scouting APIs, FW Triggered RF Tune
09/09/14   svi     Interface changes for supporting Single Rx Tune and
                   higher order diversity.
08/18/14   kab     TxPL changes to support SGLTE Tx power backoff with Coex.
08/15/14   st      Alt Path Override Support
08/11/14   jf      Return timestamp at end of RX_tune/wakeup to ML1
08/12/14   jf      MCL LTE support
07/31/14   ndb     Added support for extended EARFCN for LTE
06/13/14   kab     Add 3xCA support
05/05/14   svi     Moved TXPLL update to Core Task
05/05/14   svi     Re-Organized the file structure
04/20/14   gvn     S-LTE Support
02/04/14   bsh     Porting CL 5493095
03/12/14   bsh     Adding NLIC support
02/26/14   kab     Added support for Slot1/SRS Scheduling thru RF FED LTE Task
02/04/14    pv     Removed support for FBRx Gain error from RFA and moved to RFLM.
02/04/14    pv     Added Support for FBRx Gain error update MSG Router command
01/23/14   sbm     MCS msgr interface changes to support SGLTE Tx power backoff
07/29/13   as      Added support for relative backoff on SAR limit based on 
                   TDD-LTE ULDL configuration
07/26/13   pl      Added support for setting ant switch position at enter mode and wakeup
07/18/13    pv     Added support for separate TDD Tx and Rx ASM scripts.
07/18/13   pl      Update interface to match with common AsDIV driver
07/17/13   pl      Added MSG support for Asdiv abort
06/06/13   pv      Added support for multple TDD ASM scripts.
06/05/13   php     Add MSG support for Ant Switch Diversity
04/08/13   pl      L2L IRAT abort support
03/15/13   gvn     Update sgsar, coex_WWAN, coex_WLAN limits using MSGR
01/29/13   aca     Tuner update message support
01/15/10   pl      redefine L2L IRAT message to support multiple SCELL
12/18/12   gvn     Change the structure passed to L1 during request to get Min Power. 
                   Only pass Min Power instead of entire Tx params structure.
12/12/12   gvn     Band Specific Min power NV support (Merge from Nikel)
11/15/12   adk     Backed out Qtuner code
11/09/12   pl      moved L2L IRAT interface to msg file
11/09/12   pl      Add CA L2L IRAT interface
11/08/12   adk     Featurized Qtuner code (FEATURE_RF_HAS_QTUNER)
10/31/12   pl      Change MAX CELL support to 2
09/14/12   jf      Qtuner LTE added flag for TDD bands
09/14/12   jf      Qtuner LTE added flag for TDD bands
08/27/12   jf      Add Qtuner interface msg between ML1 and RFSW 
08/27/12   jf      Add Qtuner interface msg between ML1 and RFSW 
05/23/12   as      LTE Tx power limit for SGLTE SAR and CoEx
05/14/12   pl      Revise interface to support Carrier Aggregation
04/17/12   jf      Fix to remove ML1 to call RFSW for Quiet Mode 
03/19/12   aca     API change to support XO correction during Tx PLL tune
03/13/12   can     Added FA_RF_LTE_INVALID_CAL_DATA.
10/12/11   pl      Support Offline DRX
04/28/11   pl      Adding support for RX/TX PLL adjustments
02/18/11   pl      remove duplicated  txlm buffer field, make naming consistent
02/09/11   pl      Update to add RXLM and TXLM support
01/13/11   aca	   L2L Message routing interface
07/01/10   jyu     Added support to power collapse   
06/23/10   jyu     Added support to manage rx chain 
05/03/10   can     HDET optimization.
10/14/09   can     Changes to support the L1 Interface.
10/13/09   mkv     Added MAX RF command ID to set length to uint16
10/07/09   can     Bug fix. 
04/02/09   can     Initial Checkin

===========================================================================*/

#ifndef RF_LTE_MSG_H
#define RF_LTE_MSG_H

/*===========================================================================

                  INCLUDE FILES

===========================================================================*/

#include "appmgr_msg.h"
#include "msgr.h"
#include "msgr_types.h"
#include "rfa_msgr.h"
#include "rfcom.h"
#include "lm_types.h"


/*===========================================================================

                           CONSTANT DEFINITIONS

===========================================================================*/


/*---------------------------------------------------------------*/
/*                    MSGR SUPERVISORY ID'S                      */
/*---------------------------------------------------------------*/  

/*!>
  @brief
  This module contains MSG IDS for the different MSGR
  messages and commands to be dispatched and
  received by RFLTE
  @{
*/

/*-----------------------------------------------------------------------*/
/*! @brief ID for RF to enter LTE mode of operation CMD and RSP
 */
#define RFA_RF_LTE_MSG_ID_ENTER_MODE                0xA1

/*-----------------------------------------------------------------------*/
/*! @brief ID for RF to exit LTE mode of operation CMD and RSP
 */
#define RFA_RF_LTE_MSG_ID_EXIT_MODE                 0xA2

/*-----------------------------------------------------------------------*/
/*! @brief ID for RF to configure RX CMD and RSP (this includes tuning RX device)
 */
#define RFA_RF_LTE_MSG_ID_FDD_RX_CONFIG             0xA3

/*-----------------------------------------------------------------------*/
/*! @brief ID for RF to configure TX CMD and RSP (this includes tuning TX device)
 */
#define RFA_RF_LTE_MSG_ID_FDD_TX_CONFIG             0xA4

/*-----------------------------------------------------------------------*/
/*! @brief ID for RF to configure TX CMD and RSP (this includes tuning TX device)
 */
#define RFA_RF_LTE_MSG_ID_TDD_RX_CONFIG             0xA5

/*-----------------------------------------------------------------------*/
/*! @brief ID for RF to disable TX CMD and RSP
 */
#define RFA_RF_LTE_MSG_ID_TDD_TX_CONFIG             0xA6

/*-----------------------------------------------------------------------*/
/*! @brief ID for RF to disable TX CMD and RSP
 */
#define RFA_RF_LTE_MSG_ID_FDD_TX_DISABLE            0xA7

/*-----------------------------------------------------------------------*/
/*! @brief ID for RF to disable TX CMD and RSP
 */
#define RFA_RF_LTE_MSG_ID_TDD_TX_DISABLE            0xA8

/*-----------------------------------------------------------------------*/
/*! @brief ID for RF to go to sleep CMD and RSP
 */
#define RFA_RF_LTE_MSG_ID_SLEEP                     0xA9

/*-----------------------------------------------------------------------*/
/*! @brief ID for RF to wake up CMD and RSP
 */ 
#define RFA_RF_LTE_MSG_ID_WAKEUP                    0xAA

/*-----------------------------------------------------------------------*/
/*! @brief ID to querry whether RF is sleeping CMD and RSP
 */
#define RFA_RF_LTE_MSG_ID_RF_IS_SLEEPING            0xAB

/*-----------------------------------------------------------------------*/
/*! @brief ID to indicate TX is ON CMD
 */
#define RFA_RF_LTE_MSG_ID_TX_ON                     0xAD

/*-----------------------------------------------------------------------*/
/*! @brief ID to request L2L measurement
 */
#define RFA_RF_LTE_MSG_ID_L2L_BUILD_SCRIPTS         0xAE

/*-----------------------------------------------------------------------*/
/*! @brief ID to request L2L measurement
 */
#define RFA_RF_LTE_MSG_ID_PLL_ADJUST                0xAF

/*-----------------------------------------------------------------------*/
/*! @brief ID to request connection mode status
 */
#define RFA_RF_LTE_MSG_ID_CONNECT_MODE              0xB0

/*-----------------------------------------------------------------------*/
/*! @brief ID to Update TXPL parameters
 */
#define RFA_RF_LTE_MSG_ID_TXPL_PARAM_UPDATE         0xB1

/*-----------------------------------------------------------------------*/
/*! @brief ID to set TX power limit
 */
#define RFA_RF_LTE_MSG_ID_SET_TX_PLIMIT	            0xB2

/*-----------------------------------------------------------------------*/
/*! @brief ID to get UE min Tx power limit
 */
#define RFA_RF_LTE_MSG_ID_GET_UE_MIN_POWER          0xB3

/*-----------------------------------------------------------------------*/
/*! @brief ID for ML1 ticks for ant tuning 
*/
#define RFA_RF_LTE_MSG_ID_ANT_TUNER_TICK            0xB4

/*-----------------------------------------------------------------------*/
/*! @brief ID for Ant SW Req from ML1 
*/
#define RFA_RF_LTE_MSG_ID_ANT_SW                    0xB5

/*-----------------------------------------------------------------------*/
/*! @brief ID for Ant SW Req from ML1 
*/
#define RFA_RF_LTE_MSG_ID_ANT_SW_ABORT              0xB6

/*-----------------------------------------------------------------------*/
/*! @brief 
*/
#define  RFA_RF_LTE_MSG_ID_TDD_ASM_SCRIPT_UPDATE    0xB7

/*-----------------------------------------------------------------------*/
/*! @brief RESERVED_0
*/
#define RFA_RF_LTE_MSG_ID_RESERVED_0             0xB8

/*-----------------------------------------------------------------------*/
/*! @brief 
*/
#define  RFA_RF_LTE_MSG_ID_FBRX_GAIN_ERR_UPDATE     0xB9

/*-----------------------------------------------------------------------*/
/*! @brief RESERVED_1
*/
#define RFA_RF_LTE_MSG_ID_RESERVED_1      0xBA

/*-----------------------------------------------------------------------*/
/*! @brief RESERVED_2
*/
#define RFA_RF_LTE_MSG_ID_RESERVED_2          0xBB

/*-----------------------------------------------------------------------*/
/*! @brief Schedue PCMAX Update to Core Task
*/
#define RFA_RF_LTE_MSG_ID_P_CMAX_UPDATE             0xBC

/*-----------------------------------------------------------------------*/
/*! @brief 
*/
#define RFA_RF_LTE_MSG_ID_UL_POWER_UPDATE           0xBD

/*-----------------------------------------------------------------------*/
/*! @brief 
*/
#define  RFA_RF_LTE_MSG_ID_PATH_SEL_OVERRIDE        0xBE

/*-----------------------------------------------------------------------*/
/*! @brief ID for RF to do Rx1 Chain config
 */
#define RFA_RF_LTE_MSG_ID_RX1_CHAIN_CONFIG          0xBF

/*-----------------------------------------------------------------------*/
/*! @brief Schedule TxPLL Script Update to DM to Core Task
*/
#define RFA_RF_LTE_MSG_ID_TXPLL_SCRIPTS_UPDATE      0xC0

/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
/*! @brief 
*/
#define  RFA_RF_LTE_MSG_ID_ALT_PATH_OVERRIDE        0xC1

/*-----------------------------------------------------------------------*/
/*! @brief ID for Script Based Rx Tune 
*/
#define RFA_RF_LTE_MSG_ID_RX_SCRIPT_BASED_CONFIG    0xC2

/*-----------------------------------------------------------------------*/
/*! @brief ID for Script Based Tx Tune 
*/
#define RFA_RF_LTE_MSG_ID_TX_SCRIPT_BASED_CONFIG    0xC3

/*-----------------------------------------------------------------------*/
/*! @brief ID for getting Single Chain Support
*/
#define RFA_RF_LTE_MSG_ID_GET_SINGLE_CHAIN_SUPPORT  0xC4

/*-----------------------------------------------------------------------*/
/*! @brief ID for determining Re-Tune requirement of already tuned Cells
*/
#define RFA_RF_LTE_MSG_ID_CHECK_CONFIG              0xC5

/*-----------------------------------------------------------------------*/
/*! @brief ID for verifying Alt path in FTM mode 
*/
#define RFA_RF_LTE_MSG_ID_ALT_PATH_VERIFICATION     0xC6

/*-----------------------------------------------------------------------*/
/*! @brief ID for determining Tune requirement of already tuned Cells
*/
#define RFA_RF_LTE_MSG_ID_CONFIG                    0xC7

/*-----------------------------------------------------------------------*/
/*! @brief ID for confirmation after cells already tuned
*/
#define RFA_RF_LTE_MSG_ID_CONFIG_COMPLETE           0xC8

/*-----------------------------------------------------------------------*/
/*! @brief ID for determining Tune requirement of already tuned Cells
*/
#define RFA_RF_LTE_MSG_ID_CHECK_CONFIG_V2           0xC9

/*-----------------------------------------------------------------------*/
/*! @brief ID for RF to disable TX CMD and RSP
 */
#define RFA_RF_LTE_MSG_ID_TX_DISABLE                0xCA

/*-----------------------------------------------------------------------*/
/*! @brief ID for RF for CDRX sleep CMD and RSP
 */
#define RFA_RF_LTE_MSG_ID_CDRX_SLEEP                0xCB

/*-----------------------------------------------------------------------*/
/*! @brief ID for RF for CDRX wake up CMD and RSP
 */
#define RFA_RF_LTE_MSG_ID_CDRX_WAKEUP               0xCC

/*-----------------------------------------------------------------------*/
/*! @brief ID for RF to disable RX CMD and RSP
 */
#define RFA_RF_LTE_MSG_ID_RX_DISABLE                0xCD

/*-----------------------------------------------------------------------*/
/*! @brief ID for RF to determine FBRX params to pass to ML1
 */
#define RFA_RF_LTE_MSG_ID_GET_FBRX_PARAMS           0xCE

/*-----------------------------------------------------------------------*/

/*! @brief Max Number of MSG ID
*/
#define RFA_RF_LTE_MSG_MAX_ID                       0xFF

/*! @} */

/*-----------------------------------------------------------------------*/


/*---------------------------------------------------------------*/
/*                 OTHER CONSTANT DEFINITIONS                    */
/*---------------------------------------------------------------*/

/*!>
  @brief
  This module contains the constant definitions used in RFLTE
  @{
*/

/*-----------------------------------------------------------------------*/
/*! @brief Max Number of Cells Supported
 */
#define RFA_RF_LTE_MAX_CELL_SUPPORTED               3

/*-----------------------------------------------------------------------*/
/*! @brief Max Number of Uplink Tx Cells Supported
 */
#define RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED               3

/*-----------------------------------------------------------------------*/
/*! @brief applies to Simultaneous Transmit SAR power limit
*/
#define RFA_RF_LTE_ST_SAR_PLIMIT_MASK               0x01

/*-----------------------------------------------------------------------*/
/*! @brief applies to WWAN Coexistence power limit
*/
#define RFA_RF_LTE_WWAN_COEX_PLIMIT_MASK            0x02 

/*-----------------------------------------------------------------------*/
/*! @brief applies to WCN (WLAN & BT) Coexistence power limit
*/
#define RFA_RF_LTE_WCN_COEX_PLIMIT_MASK             0x04

/*-----------------------------------------------------------------------*/
/*! @brief applies to WWAN Coexistence subframe level power limit
*/
#define RFA_RF_LTE_SUBFRAME_COEX_PLIMIT_MASK        0x08

/*-----------------------------------------------------------------------*/
/*! @brief applies to LTE power limit
*/
#define RFA_RF_LTE_VBATT_BACKOFF_MASK               0x10

/*-----------------------------------------------------------------------*/
/*! @brief applies to PA thermal backoff power limit
*/
#define RFA_RF_LTE_PA_THERMAL_BACKOFF_MASK          0x20

/*-----------------------------------------------------------------------*/
/*! @brief defines the bit mask on the path bit mask.
*/

/* Bit Mask per Path 
   <Reserved 15:8> | DRx3 | DRx2 | DRx | PRx | <Reserved 3:1> | Tx | 
*/
#define RFA_RF_LTE_TX_PATH_BIT_MASK                 0x01
#define RFA_RF_LTE_RX_PATH_BIT_MASK                 0xF0
#define RFA_RF_LTE_PRX_PATH_BIT_MASK                0x10
#define RFA_RF_LTE_DRX_PATH_BIT_MASK                0x20
#define RFA_RF_LTE_DRX2_PATH_BIT_MASK               0x40
#define RFA_RF_LTE_DRX3_PATH_BIT_MASK               0x80

/*! @} */

/*-----------------------------------------------------------------------*/
/*! @brief Defines 
*/
/*  Single Chain Support Bit Mask           
    Rx_SC_Bit_Mask [8 bits] | Tx_SC_Bit_Mask [8 bits]
*/
#define RFA_RF_LTE_SINGLE_CHAIN_TX_BM_START_BIT  0
#define RFA_RF_LTE_SINGLE_CHAIN_RX_BM_START_BIT  8

/* Single chain support bit mask for Tx */
#define RFA_RF_LTE_SINGLE_CHAIN_BMSK             0x00FF

/* Clear single chain support bit mask for Tx */
#define RFA_RF_LTE_SINGLE_CHAIN_TX_BM_CLEAR      0xFF00

/*-----------------------------------------------------------------------*/
/*! @brief Max Number of Slots in a Subframe
*/
#define RFA_RF_LTE_MAX_SLOTS                     0x4
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/*! @brief Max Number of chains configure input tune
    2 sets of 6 rx + 2 tx  
*/

#ifdef FEATURE_TABASCO_MODEM
#define RFA_RF_MAX_NUM_CHAIN_CONFIGS 6 * 2
#else
#define RFA_RF_MAX_NUM_CHAIN_CONFIGS 8 * 2
#endif 


/*-----------------------------------------------------------------------*/

/*===========================================================================

                  EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*---------------------------------------------------------------*/

/*!>
  @brief
  This module contains structures and enums that define the
  payloads of the different MSGR commands to be received
  and dispatched back by the LTE SUBSYSTEM

  @{
*/

/*-----------------------------------------------------------------------*/
/*! @todo: This might need to be moved to a common file so that L1 can see it 
           and it might be shared with other targets.*/

/*-----------------------------------------------------------------------*/
typedef enum
{
  RFA_RF_LTE_SUCCESS=1,
  RFA_RF_LTE_STATE_MACHINE_ERROR=2,
  RFA_RF_LTE_EXECUTION_ERROR=3,
  RFA_RF_LTE_INVALID_CAL_DATA=4,
  RFA_RF_LTE_CMD_OUTCOME_MAX=5,
} rfa_rf_lte_cmd_outcome_t;

/*---------------------------------------------------------------*/
/*                    RF Task LTE messages                       */
/*---------------------------------------------------------------*/

/*!>
  @brief
  This module contains the constant definitions of the symbols
  of the different MSGR commands and the associated Payloads.

  MSGR Commands in the LTE SUBSYSTEM can be of three types:
  1. Messages received by the LTE SUBSYSTEM as REQUESTS
  2. Messages sent back by the LTE SUBSYSTEM as CONFIRMATION to REQUESTS
  3. Messages received by the LTE SUBSYSTEM as INDICATORS

  The enums defined below are never called by name.
  They just define the symbols.
  
  @{
*/

/*-----------------------------------------------------------------------*/
/*!
  @brief
  This enum defines the symbols of the MSGR Message ID's to be sent back
  by the LTE Task (LTE SUBSYSTEM) as confirmation responses

  The enum defined below is never called by name.
  It just define the confirmation symbols.
  
*/
enum
{
   /*! Enter LTE mode message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, ENTER_MODE, RFA_RF_LTE_MSG_ID_ENTER_MODE, rfa_rf_lte_enter_mode_cnf_s),

   /*! Exit LTE mode message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, EXIT_MODE, RFA_RF_LTE_MSG_ID_EXIT_MODE, rfa_rf_lte_exit_mode_cnf_s),

   /*! Configure FDD RX message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, FDD_RX_CONFIG, RFA_RF_LTE_MSG_ID_FDD_RX_CONFIG, rfa_rf_lte_fdd_rx_config_cnf_s),

   /*! Configure FDD TX message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, FDD_TX_CONFIG, RFA_RF_LTE_MSG_ID_FDD_TX_CONFIG, rfa_rf_lte_fdd_tx_config_cnf_s),

   /*! Configure TDD RX message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, TDD_RX_CONFIG, RFA_RF_LTE_MSG_ID_TDD_RX_CONFIG, rfa_rf_lte_tdd_rx_config_cnf_s),

   /*! Config TDD TX  message*/
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, TDD_TX_CONFIG, RFA_RF_LTE_MSG_ID_TDD_TX_CONFIG, rfa_rf_lte_tdd_tx_config_cnf_s),

   /*! Disable TDD TX disable message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, FDD_TX_DISABLE, RFA_RF_LTE_MSG_ID_FDD_TX_DISABLE, rfa_rf_lte_fdd_tx_disable_cnf_s),

   /*! Disable TDD TX disable message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, TDD_TX_DISABLE, RFA_RF_LTE_MSG_ID_TDD_TX_DISABLE, rfa_rf_lte_tdd_tx_disable_cnf_s),

   /*! RF sleep message*/
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, SLEEP, RFA_RF_LTE_MSG_ID_SLEEP, rfa_rf_lte_sleep_cnf_s),

   /*! RF wakeup message*/
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, WAKEUP, RFA_RF_LTE_MSG_ID_WAKEUP, rfa_rf_lte_wakeup_cnf_s),

   /*! RF CDRX sleep message*/
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, CDRX_SLEEP, RFA_RF_LTE_MSG_ID_CDRX_SLEEP, rfa_rf_lte_cdrx_sleep_cnf_s),

   /*! RF CDRX wakeup message*/
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, CDRX_WAKEUP, RFA_RF_LTE_MSG_ID_CDRX_WAKEUP, rfa_rf_lte_cdrx_wakeup_cnf_s),

   /*! RF is sleeping querry*/
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, RF_IS_SLEEPING, RFA_RF_LTE_MSG_ID_RF_IS_SLEEPING, rfa_rf_lte_rf_is_sleeping_cnf_s),

   /*! RF TX_ON confirmation message*/
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, TX_ON, RFA_RF_LTE_MSG_ID_TX_ON, rfa_rf_lte_tx_on_cfn_s),   
   
   /*! RF L2L measurement request message confirmation*/
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, L2L_BUILD_SCRIPTS, RFA_RF_LTE_MSG_ID_L2L_BUILD_SCRIPTS, rfa_rf_lte_l2l_build_scripts_cnf_s),

   /*! RF PLL adjustment request message confirmation*/
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, PLL_ADJUST, RFA_RF_LTE_MSG_ID_PLL_ADJUST, rfa_rf_lte_pll_adjust_cnf_s),

   /*! UE min Tx power limit message confirmation */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, GET_UE_MIN_POWER, RFA_RF_LTE_MSG_ID_GET_UE_MIN_POWER, rfa_rf_lte_get_ue_min_power_cnf_s),

   /*! Antenna switch request confirmation */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, ANT_SW, RFA_RF_LTE_MSG_ID_ANT_SW, rfa_rf_lte_ant_sw_cnf_s),

   /*! Rx1 chain config LTE mode message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, RX1_CHAIN_CONFIG, RFA_RF_LTE_MSG_ID_RX1_CHAIN_CONFIG, rfa_rf_lte_rx1_chain_config_cnf_s),
   
   /*! ID for Script Based Rx Tune  */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, RX_SCRIPT_BASED_CONFIG, RFA_RF_LTE_MSG_ID_RX_SCRIPT_BASED_CONFIG, rfa_rf_lte_rx_script_based_config_cnf_s),

   /*! ID for Script Based Tx Tune. */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, TX_SCRIPT_BASED_CONFIG, RFA_RF_LTE_MSG_ID_TX_SCRIPT_BASED_CONFIG, rfa_rf_lte_tx_script_based_config_cnf_s),

   /*! ID to obtain Single Chain Support */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, GET_SINGLE_CHAIN_SUPPORT, RFA_RF_LTE_MSG_ID_GET_SINGLE_CHAIN_SUPPORT, rfa_rf_lte_get_single_chain_support_cnf_s),

   /*! Identify Re-Tune requirement for already tuned Cells. */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, CHECK_CONFIG, RFA_RF_LTE_MSG_ID_CHECK_CONFIG, rfa_rf_lte_check_config_cnf_s),

   /*! ID to obtain required tune scripts. */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, CONFIG, RFA_RF_LTE_MSG_ID_CONFIG, rfa_rf_lte_config_cnf_s),

   /*! Identify Re-Tune requirement for already tuned cells, input interface matching the CONFIG input. */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, CHECK_CONFIG_V2, RFA_RF_LTE_MSG_ID_CHECK_CONFIG_V2, rfa_rf_lte_check_config_v2_cnf_s),

   /*! ID to perform Tx DISABLE */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, TX_DISABLE, RFA_RF_LTE_MSG_ID_TX_DISABLE, rfa_rf_lte_tx_disable_cnf_s),

   /*! Identify rf Tune completed on requested Cells. */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, CONFIG_COMPLETE, RFA_RF_LTE_MSG_ID_CONFIG_COMPLETE, rfa_rf_lte_config_complete_s),

   /*! UE FBRX Params message confirmation */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, GET_FBRX_PARAMS, RFA_RF_LTE_MSG_ID_GET_FBRX_PARAMS, rfa_rf_lte_get_fbrx_params_cnf_s),
};/* Confirmation Resposes */


/*-----------------------------------------------------------------------*/
/*!
  @brief
  This enum defines the symbols of the MSGR Message ID's to be received
  by the LTE Task (LTE SUBSYSTEM) as requests or indicators

  The enum defined below is never called by name.
  It just define the request / indicator symbols.
  
*/
enum
{
   /*! Enter LTE mode message, payload is the device type, sent by L1 */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, ENTER_MODE, RFA_RF_LTE_MSG_ID_ENTER_MODE, rfa_rf_lte_enter_mode_req_s),

   /*! Exit LTE mode message, payload is the device type, sent by L1 */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, EXIT_MODE, RFA_RF_LTE_MSG_ID_EXIT_MODE, rfa_rf_lte_exit_mode_req_s),

   /*! Configure FDD RX message, payload is defined in lte_config_s, sent by L1 */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, FDD_RX_CONFIG, RFA_RF_LTE_MSG_ID_FDD_RX_CONFIG, rfa_rf_lte_fdd_rx_config_req_s),

   /*! Configure FDD TX message, payload is defined in lte_config_s, sent by L1 */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, FDD_TX_CONFIG, RFA_RF_LTE_MSG_ID_FDD_TX_CONFIG, rfa_rf_lte_fdd_tx_config_req_s),

   /*! Configure TDD message, payload is defined in lte_config_s, sent by L1 */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, TDD_RX_CONFIG, RFA_RF_LTE_MSG_ID_TDD_RX_CONFIG, rfa_rf_lte_tdd_rx_config_req_s),

   /*! Disable TDD TX Enable message*/
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, TDD_TX_CONFIG, RFA_RF_LTE_MSG_ID_TDD_TX_CONFIG, rfa_rf_lte_tdd_tx_config_req_s),

   /*! Disable TDD TX disable message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, FDD_TX_DISABLE, RFA_RF_LTE_MSG_ID_FDD_TX_DISABLE, rfa_rf_lte_fdd_tx_disable_req_s),

   /*! Disable TDD TX disable message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, TDD_TX_DISABLE, RFA_RF_LTE_MSG_ID_TDD_TX_DISABLE, rfa_rf_lte_tdd_tx_disable_req_s),

   /*! Sleep message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, SLEEP, RFA_RF_LTE_MSG_ID_SLEEP, rfa_rf_lte_sleep_req_s),

   /*! Wakeup message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, WAKEUP, RFA_RF_LTE_MSG_ID_WAKEUP, rfa_rf_lte_wakeup_req_s),

   /*! CDRX Sleep message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, CDRX_SLEEP, RFA_RF_LTE_MSG_ID_CDRX_SLEEP, rfa_rf_lte_cdrx_sleep_req_s),

   /*! CDRX Wakeup message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, CDRX_WAKEUP, RFA_RF_LTE_MSG_ID_CDRX_WAKEUP, rfa_rf_lte_cdrx_wakeup_req_s),

   /*! Sleep querry message, */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, RF_IS_SLEEPING, RFA_RF_LTE_MSG_ID_RF_IS_SLEEPING, rfa_rf_lte_rf_is_sleeping_req_s),

   /*! TX ON Indicator message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, IND, TX_ON, RFA_RF_LTE_MSG_ID_TX_ON, rfa_rf_lte_tx_on_ind_s),

   /*! L2L Measurement request message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, L2L_BUILD_SCRIPTS, RFA_RF_LTE_MSG_ID_L2L_BUILD_SCRIPTS, rfa_rf_lte_l2l_build_scripts_req_s),

   /*! PLL adjustment request message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, PLL_ADJUST, RFA_RF_LTE_MSG_ID_PLL_ADJUST, rfa_rf_lte_pll_adjust_req_s),

    /*! Notify connection mode request message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, IND, CONNECT_MODE, RFA_RF_LTE_MSG_ID_CONNECT_MODE, rfa_rf_lte_connect_mode_ind_s),

   /*! Notify connection mode request message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, IND, TXPL_UPDATE, RFA_RF_LTE_MSG_ID_TXPL_PARAM_UPDATE, rfa_rf_lte_txpl_update_ind_s),

   /*! Set TX power limit for SGLTE, payload is TX power limit in dB10, sent by MCS */
   MSGR_DEFINE_UMID(RFA, RF_LTE, IND, SET_TX_PLIMIT, RFA_RF_LTE_MSG_ID_SET_TX_PLIMIT, rfa_rf_lte_set_tx_plimit_ind_s),

   /*! UE min Tx power limit request message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, GET_UE_MIN_POWER, RFA_RF_LTE_MSG_ID_GET_UE_MIN_POWER, rfa_rf_lte_get_ue_min_power_s),

   /*! ML1 provide the RFSW ticks for ant tuning */
   MSGR_DEFINE_UMID(RFA, RF_LTE, IND, ANT_TUNER_TICK, RFA_RF_LTE_MSG_ID_ANT_TUNER_TICK, rfa_rf_lte_ant_tuner_tick_ind_s),

   /*! Antenna switch request message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, ANT_SW, RFA_RF_LTE_MSG_ID_ANT_SW, rfa_rf_lte_ant_sw_s),

   /*! Antenna switch abort message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, IND, ANT_SW_ABORT, RFA_RF_LTE_MSG_ID_ANT_SW_ABORT, rfa_rf_lte_ant_sw_abort_ind_s),

   /*! Update asm script dynamically request for tdd bands */
   MSGR_DEFINE_UMID(RFA, RF_LTE, IND, TDD_ASM_SCRIPT_UPDATE, RFA_RF_LTE_MSG_ID_TDD_ASM_SCRIPT_UPDATE, rfa_rf_lte_dynamic_asm_update_ind_s),

   /*! Update asm script dynamically confirmation for tdd bands */
   MSGR_DEFINE_UMID(RFA, RF_LTE, CNF, TDD_ASM_SCRIPT_UPDATE, RFA_RF_LTE_MSG_ID_TDD_ASM_SCRIPT_UPDATE, rfa_rf_lte_dynamic_asm_update_cnf_s),

   /*! PC Max Update, sent by LTE ML1 Task */
   MSGR_DEFINE_UMID(RFA, RF_LTE, IND, P_CMAX_UPDATE, RFA_RF_LTE_MSG_ID_P_CMAX_UPDATE, rfa_rf_lte_p_cmax_update_ind_s),

    /*UL power every SF*/
   MSGR_DEFINE_UMID(RFA, RF_LTE, IND, UL_POWER_UPDATE, RFA_RF_LTE_MSG_ID_UL_POWER_UPDATE, rfa_rf_lte_ul_power_update_s),

   /*! Force TX/Rx Path for a given LTE Band */
   MSGR_DEFINE_UMID(RFA, RF_LTE, IND, PATH_SEL_OVERRIDE, RFA_RF_LTE_MSG_ID_PATH_SEL_OVERRIDE, rfa_rf_lte_path_sel_override_update_ind_s),
   
   /*! Rx1 chain config LTE mode message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, RX1_CHAIN_CONFIG, RFA_RF_LTE_MSG_ID_RX1_CHAIN_CONFIG, rfa_rf_lte_rx1_chain_config_req_s),

   /*! TxPLL Script generation and update to DM */
   MSGR_DEFINE_UMID(RFA, RF_LTE, IND, TXPLL_SCRIPTS_UPDATE, RFA_RF_LTE_MSG_ID_TXPLL_SCRIPTS_UPDATE, rfa_rf_lte_txpll_scripts_update_s), 

   /*! ID for Alt path override in Calibration mode  */
   MSGR_DEFINE_UMID(RFA, RF_LTE, IND, ALT_PATH_OVERRIDE, RFA_RF_LTE_MSG_ID_ALT_PATH_OVERRIDE, rfa_rf_lte_ftm_alt_path_oride_ind_s),
   
   /*! ID for Script Based Rx Tune  */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, RX_SCRIPT_BASED_CONFIG, RFA_RF_LTE_MSG_ID_RX_SCRIPT_BASED_CONFIG, rfa_rf_lte_rx_script_based_config_req_s),

   /*! ID for Script Based Tx Tune. */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, TX_SCRIPT_BASED_CONFIG, RFA_RF_LTE_MSG_ID_TX_SCRIPT_BASED_CONFIG, rfa_rf_lte_tx_script_based_config_req_s),

   /*! ID to obtain Single Chain Support */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, GET_SINGLE_CHAIN_SUPPORT, RFA_RF_LTE_MSG_ID_GET_SINGLE_CHAIN_SUPPORT, rfa_rf_lte_get_single_chain_support_req_s),
   
   /*! Identify Re-Tune requirement for already tuned Cells. */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, CHECK_CONFIG, RFA_RF_LTE_MSG_ID_CHECK_CONFIG, rfa_rf_lte_check_config_req_s),

   /*! ID for Alt path override in FTM verification mode  */
   MSGR_DEFINE_UMID(RFA, RF_LTE, IND, ALT_PATH_VERIFICATION, RFA_RF_LTE_MSG_ID_ALT_PATH_VERIFICATION, rfa_rf_lte_ftm_alt_path_ver_ind_s),

   /*! Identify RF config request to tune Cells. */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, CONFIG, RFA_RF_LTE_MSG_ID_CONFIG, rfa_rf_lte_config_req_s),

   /*! Identify rf Tune completed on requested Cells. */
   MSGR_DEFINE_UMID(RFA, RF_LTE, IND, CONFIG_COMPLETE, RFA_RF_LTE_MSG_ID_CONFIG_COMPLETE, rfa_rf_lte_config_complete_s),

   /*! Identify Re-Tune requirement for already tuned cells, input interface matching the CONFIG input. */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, CHECK_CONFIG_V2, RFA_RF_LTE_MSG_ID_CHECK_CONFIG_V2, rfa_rf_lte_check_config_v2_req_s),

   /*! ID to Perform TX DISABLE */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, TX_DISABLE, RFA_RF_LTE_MSG_ID_TX_DISABLE, rfa_rf_lte_tx_disable_req_s),

   /*! ID to Perform RX DISABLE State */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, RX_DISABLE, RFA_RF_LTE_MSG_ID_RX_DISABLE, rfa_rf_lte_rx_disable_req_s),

   /*! UE FBRX Params request message */
   MSGR_DEFINE_UMID(RFA, RF_LTE, REQ, GET_FBRX_PARAMS, RFA_RF_LTE_MSG_ID_GET_FBRX_PARAMS, rfa_rf_lte_get_fbrx_parmas_s),   
};/* Requests and Indicators */


/*! @} */

/*-----------------------------------------------------------------------*/


/*---------------------------------------------------------------*/
/*                EXTERNAL STRUCTURES AND ENUMS                  */
/*-----------------------------------------------------------------------*/
/* This data structure can be used for holding the RF 
Tx related parameters to be updated to ML1*/
typedef struct
{  
  int16   rf_min_tx_power;
} rfa_rf_lte_tx_config_cnf_params_type;

/*-----------------------------------------------------------------------*/
/*!> Structure is 32 bit alligned to save memory */
typedef struct
{
   rfm_device_enum_type device;
   uint8                 scenario_val;
   boolean               is_scenario_valid;
   lm_handle_type       lm_buffer_index;
   rfm_tuner_handle_type tuner_handle;
} rfa_rf_chain_param_type;

/*-----------------------------------------------------------------------*/
typedef struct
{
  rfcom_lte_earfcn_type            channel;
  rfcom_lte_bw_type bw;
} rfa_rf_rx_param_type;

/*-----------------------------------------------------------------------*/
/* Indicates TDD UL/DL configurations as specified in 
the 3GPP 36.211 V11.3.0, Table 4.2-2 */
typedef enum
{
  RFA_RF_LTE_TDD_UL_DL_CFG_0 = 0,
  RFA_RF_LTE_TDD_UL_DL_CFG_1 = 1,
  RFA_RF_LTE_TDD_UL_DL_CFG_2 = 2,
  RFA_RF_LTE_TDD_UL_DL_CFG_3 = 3,
  RFA_RF_LTE_TDD_UL_DL_CFG_4 = 4,
  RFA_RF_LTE_TDD_UL_DL_CFG_5 = 5,
  RFA_RF_LTE_TDD_UL_DL_CFG_6 = 6,
  RFA_RF_LTE_TDD_UL_DL_CFG_INVALID = 0xFF,  
} rfa_rf_lte_tdd_ul_dl_cfg_type;

/*-----------------------------------------------------------------------*/
typedef struct
{
  rfcom_lte_earfcn_type            channel;
  rfcom_lte_bw_type bw;
  rfa_rf_lte_tdd_ul_dl_cfg_type tdd_ul_dl_cfg;  /* Cfg#0, Cfg#1....Cfg#6*/
  int32             rx_freq_error;  
} rfa_rf_tx_param_type;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  uint32 carrier_idx_mask;
  uint16 path_bitmask[RFA_RF_LTE_MAX_CELL_SUPPORTED];  
  rfa_rf_chain_param_type prx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx2[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx3[RFA_RF_LTE_MAX_CELL_SUPPORTED];  
  uint8 ant_pos;
} rfa_rf_lte_enter_mode_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type     hdr;
  uint32 carrier_idx_mask;
  uint16 path_bitmask[RFA_RF_LTE_MAX_CELL_SUPPORTED];  
  rfa_rf_chain_param_type prx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx2[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx3[RFA_RF_LTE_MAX_CELL_SUPPORTED];  
  uint8 ant_pos;
  rfa_rf_lte_cmd_outcome_t req_result;
} rfa_rf_lte_enter_mode_cnf_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  uint32 carrier_idx_mask;
  uint16 path_bitmask[RFA_RF_LTE_MAX_CELL_SUPPORTED];  
  rfa_rf_chain_param_type prx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx2[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx3[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_rx_param_type rx_param[RFA_RF_LTE_MAX_CELL_SUPPORTED];
} rfa_rf_lte_fdd_rx_config_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type     hdr;
  uint32 carrier_idx_mask;
  uint16 path_bitmask[RFA_RF_LTE_MAX_CELL_SUPPORTED];  
  rfa_rf_chain_param_type prx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx2[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx3[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_rx_param_type rx_param[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_lte_cmd_outcome_t req_result;
  boolean                  antenna_tuner_present;
  uint64 ustmr_timestamp;
} rfa_rf_lte_fdd_rx_config_cnf_s;


typedef struct
{
  rfcom_lte_earfcn_type       channel;
  rfcom_lte_bw_type           bw;

  /*!> @brief rx path bit mask to be tuned. 
    This is per carrier. bit0 - RX0, bit1 - RX1, bit2 - RX2 ... */
  uint8                    path_bitmask;  
  /*!> @brief params for each path. This is per path per carrier  */
  rfa_rf_chain_param_type rx_path_params[4];


} rfa_rf_rx_tune_param_type;


/*!> @brief This structure contains params necessary for rx config */
typedef struct
{
  /*!> @brief request type from ML1 to RFSW LTE */
  msgr_hdr_struct_type   hdr;  
  /*!> @brief carrier index mask to be tuned from ML1 to RFSW */
  uint32 carrier_idx_mask;
  /*!> @brief params for each cell. This is per carrier */
  rfa_rf_rx_tune_param_type rx_cell_params[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  /*!> @brief TRUE = RF to execute scripts */
  boolean rf_to_execute_scripts;

} rfa_rf_lte_rx_config_req_s;


/*-----------------------------------------------------------------------*/

typedef struct
{
  msgr_hdr_struct_type    hdr;
} rfa_rf_lte_rx_disable_req_s;


typedef struct
{
   /*!> @brief request type from ML1 to RFSW LTE */
  msgr_hdr_struct_type   hdr;  
  /*!> @brief carrier index mask to be tuned from ML1 to RFSW */
  uint32 carrier_idx_mask;
  /*!> @brief params for each cell. This is per carrier */
  rfa_rf_rx_param_type rx_cell_params[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  /*!> @brief TRUE = RF to execute scripts */
  rfa_rf_lte_cmd_outcome_t req_result;

  boolean                  antenna_tuner_present;

  uint64 ustmr_timestamp;

} rfa_rf_lte_rx_config_cnf_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  uint32 carrier_idx_mask;
  uint16 path_bitmask[RFA_RF_LTE_MAX_CELL_SUPPORTED];  
  rfa_rf_chain_param_type prx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx2[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx3[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_rx_param_type rx_param[RFA_RF_LTE_MAX_CELL_SUPPORTED];
} rfa_rf_lte_tdd_rx_config_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type     hdr;
  uint32 carrier_idx_mask;
  uint16 path_bitmask[RFA_RF_LTE_MAX_CELL_SUPPORTED];  
  rfa_rf_chain_param_type prx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx2[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx3[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_rx_param_type rx_param[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_lte_cmd_outcome_t req_result;
  boolean                  antenna_tuner_present;
  uint64 ustmr_timestamp;
} rfa_rf_lte_tdd_rx_config_cnf_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  uint32 carrier_idx_mask;
  rfa_rf_chain_param_type tx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_tx_param_type tx_param[RFA_RF_LTE_MAX_CELL_SUPPORTED];
} rfa_rf_lte_fdd_tx_config_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type     hdr;
  uint32 carrier_idx_mask;
  rfa_rf_chain_param_type tx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_tx_param_type tx_param[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_lte_tx_config_cnf_params_type tx_config_cnf_params;
  rfa_rf_lte_cmd_outcome_t req_result;
} rfa_rf_lte_fdd_tx_config_cnf_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  uint32 carrier_idx_mask;
  rfa_rf_chain_param_type tx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_tx_param_type tx_param[RFA_RF_LTE_MAX_CELL_SUPPORTED];
} rfa_rf_lte_tdd_tx_config_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type     hdr;
  uint32 carrier_idx_mask;
  rfa_rf_chain_param_type tx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_tx_param_type tx_param[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_lte_tx_config_cnf_params_type tx_config_cnf_params;
  rfa_rf_lte_cmd_outcome_t req_result;
} rfa_rf_lte_tdd_tx_config_cnf_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  uint32 carrier_idx_mask;
  rfa_rf_chain_param_type tx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
} rfa_rf_lte_fdd_tx_disable_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type     hdr;
  uint32 carrier_idx_mask;
  rfa_rf_chain_param_type tx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_lte_cmd_outcome_t req_result;
} rfa_rf_lte_fdd_tx_disable_cnf_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  uint32 carrier_idx_mask;
  rfa_rf_chain_param_type tx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
} rfa_rf_lte_tdd_tx_disable_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type     hdr;
  uint32 carrier_idx_mask;
  rfa_rf_chain_param_type tx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_lte_cmd_outcome_t req_result;
} rfa_rf_lte_tdd_tx_disable_cnf_s;

/*-----------------------------------------------------------------------*/

typedef struct
{
  msgr_hdr_struct_type    hdr;
  uint32                  carrier_idx_mask;
  rfa_rf_chain_param_type tx[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];
} rfa_rf_lte_tx_disable_req_s;

/*-----------------------------------------------------------------------*/

typedef struct
{
  msgr_hdr_struct_type     hdr;
  uint32                   carrier_idx_mask;
  rfa_rf_chain_param_type  tx[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];
  rfa_rf_lte_cmd_outcome_t req_result;
} rfa_rf_lte_tx_disable_cnf_s;

/*-----------------------------------------------------------------------*/

typedef struct
{
  msgr_hdr_struct_type   hdr;
  uint32 carrier_idx_mask;
  uint16 path_bitmask[RFA_RF_LTE_MAX_CELL_SUPPORTED];   
  rfa_rf_chain_param_type prx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx2[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx3[RFA_RF_LTE_MAX_CELL_SUPPORTED];
} rfa_rf_lte_sleep_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type     hdr;
  uint32 carrier_idx_mask;
  uint16 path_bitmask[RFA_RF_LTE_MAX_CELL_SUPPORTED];   
  rfa_rf_chain_param_type prx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx2[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx3[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_lte_cmd_outcome_t req_result;
} rfa_rf_lte_sleep_cnf_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  uint32 carrier_idx_mask;
  uint16 path_bitmask[RFA_RF_LTE_MAX_CELL_SUPPORTED];  
  rfa_rf_chain_param_type prx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx2[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx3[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  uint8 ant_pos;
} rfa_rf_lte_wakeup_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type     hdr;
  uint32 carrier_idx_mask;
  uint16 path_bitmask[RFA_RF_LTE_MAX_CELL_SUPPORTED];  
  rfa_rf_chain_param_type prx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx2[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx3[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  uint8 ant_pos;
  rfa_rf_lte_cmd_outcome_t req_result;
  uint64 ustmr_timestamp;
} rfa_rf_lte_wakeup_cnf_s;


/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  uint32 carrier_idx_mask;
  uint32 l1_sleep_duration_ms;
  uint16 path_bitmask[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type prx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx2[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx3[RFA_RF_LTE_MAX_CELL_SUPPORTED];
} rfa_rf_lte_cdrx_sleep_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type     hdr;
  uint32 carrier_idx_mask;
  uint32 l1_sleep_duration_ms;
  uint16 path_bitmask[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type prx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx2[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx3[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_lte_cmd_outcome_t req_result;
} rfa_rf_lte_cdrx_sleep_cnf_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  uint32 carrier_idx_mask;
  uint16 path_bitmask[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type prx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx2[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx3[RFA_RF_LTE_MAX_CELL_SUPPORTED];
} rfa_rf_lte_cdrx_wakeup_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type     hdr;
  uint32 carrier_idx_mask;
  uint16 path_bitmask[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type prx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx2[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx3[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_lte_cmd_outcome_t req_result;
} rfa_rf_lte_cdrx_wakeup_cnf_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  uint32 carrier_idx_mask;
  uint16 path_bitmask[RFA_RF_LTE_MAX_CELL_SUPPORTED];  
  rfa_rf_chain_param_type prx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx2[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx3[RFA_RF_LTE_MAX_CELL_SUPPORTED];
} rfa_rf_lte_exit_mode_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type     hdr;
  uint32 carrier_idx_mask;
  uint16 path_bitmask[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type prx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx2[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx3[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_lte_cmd_outcome_t req_result;
} rfa_rf_lte_exit_mode_cnf_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  uint32                 carrier_idx;
} rfa_rf_lte_rf_is_sleeping_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type     hdr;
  boolean                  rf_is_sleeping_val;
  /*echo back the input*/
  uint32                 carrier_idx;
  rfa_rf_lte_cmd_outcome_t req_result;
} rfa_rf_lte_rf_is_sleeping_cnf_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  uint32                 carrier_idx;
  rfcom_lte_earfcn_type channel;
} rfa_rf_lte_tx_on_ind_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  /*echo back the input*/
  uint32                 carrier_idx;
  rfcom_lte_earfcn_type channel;
  rfa_rf_lte_cmd_outcome_t req_result;
} rfa_rf_lte_tx_on_cfn_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type  hdr;
  uint8                 sub_frame;
  int16                 tx_power;
  uint8                 num_rbs;
  uint8                 rb_start_index;

} rfa_rf_lte_ul_power_update_s;

/*-----------------------------------------------------------------------*/
/*!
  @brief
  This structure defines the Payload for the MSGR MDS ID
  RFA_RF_LTE_MSG_ID_TXPLL_SCRIPTS_UPDATE
  
*/

typedef struct
{

  /*!> Carrier Index of the Tx Cell */
  uint32 carrier_idx; 

  /*!> Rx Freq Error for the Tx Cell */  
  int32 rx_freq_error_in_hz;

  /*!> LM Handle for the Tx Cell */ 
  lm_handle_type txlm_buffer_idx;

  /*!> TxPPL SBI Script Buffer Ptr for the Tx Cell */  
  void *txpll_sbi_script_buf_ptr;

} rfa_rf_lte_txpll_scripts_update_params_s;

/*-----------------------------------------------------------------------*/
/*!
  @brief
  This structure defines the Header and Payload for the MSGR MDS ID
  RFA_RF_LTE_MSG_ID_TXPLL_SCRIPTS_UPDATE
  
*/
typedef struct
{

  /*!> MSG header */
  msgr_hdr_struct_type  hdr;

  /*!> MSG Params */  
  rfa_rf_lte_txpll_scripts_update_params_s txpll_params;

} rfa_rf_lte_txpll_scripts_update_s;

/*-----------------------------------------------------------------------*/

#if 0

typedef struct
{
  msgr_hdr_struct_type   hdr;
  rfm_l2l_setup_and_build_scripts_param_type l2l_build_script_request_params;
} rfa_rf_lte_l2l_build_scripts_req_s;


#endif


/*-----------------------------------------------------------------------*/
typedef struct
{
  boolean tx_pll_adjust_flag;
  int64   pll_freq_adjust;
} rfa_rf_lte_pll_adjust_params_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  rfa_rf_lte_pll_adjust_params_s pll_adjust_params;
} rfa_rf_lte_pll_adjust_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  rfa_rf_lte_pll_adjust_params_s pll_adjust_params;
  // result of the request
  rfa_rf_lte_cmd_outcome_t req_result;
} rfa_rf_lte_pll_adjust_cnf_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  rfcom_device_enum_type device;
  boolean switch_init_mode;
  boolean param_update;
  boolean srs_update;
  uint8 mpr;
  uint8 ampr;
  uint8 ns_x;
  uint8 mod;
  uint8 rb_block_start_1;
  uint8 rb_block_start_2;
  uint8 rb_block_num;
  uint16 power_pcmax;
  uint16 power_pemax;
  int16  dtpl;
  int16  mtpl;
  int16  dtpl_coex;
  int16  mtpl_coex;
  int16  combined_tpl;
} rfa_rf_lte_txpl_params_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  rfa_rf_lte_txpl_params_s txpl_params;
} rfa_rf_lte_txpl_update_ind_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  rfcom_device_enum_type device;
  boolean enable;
} rfa_rf_lte_connect_mode_params_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  rfa_rf_lte_connect_mode_params_s connect_mode_params;
} rfa_rf_lte_connect_mode_ind_s;
/*-----------------------------------------------------------------------*/
/*!
  @brief
  This structure defines the bit mask and the values of the backoffs 
  applied in the calculation of DTPL. They are applied for each carrier
  based on the bit mask populated by ML1.
  
  @param plimit_type_mask
  Bit 0: Indicates sar_plimit backoff needs to be applied
  Bit 1: Indicates coex_limit backoff needs to be applied
  Bit 2: Indicates wcn_coex_plimit backoff needs to be applied
  Bit 3: Indicates subframe_coex_plimit backoff needs to be applied
  Bit 4: Indicates vbatt_plimit backoff needs to be applied
  Bit 5: Indicates pa_thermal_plimit backoff needs to be applied
*/
/*-----------------------------------------------------------------------*/
typedef struct
{
  /* Bit mask which tells which limits are being updated*/
  uint32 plimit_type_mask;
  /* SAR Tx Power limit */
  int16 sar_plimit;
  /* Coex desense Tx Power limit */
  int16 coex_plimit;
  /* WLAN and BT coex tx power liimit*/
  int16 wcn_coex_plimit;
  /* this new limit is for subframe level backoff… so MCS will use this for 
  ** SGLTE, G-SLTE and 1X-SLTE  and any other combinations in future that
  ** require subframe level backoff */
  int16 subframe_coex_plimit;
  /* vbatt power limit */
  int16 vbatt_plimit; 
  /* PA thermal mitigation backoff */
  int16 pa_thermal_plimit;
  /* TxLM buffer handle */
  lm_handle_type txlm_buffer_idx;
} rfa_rf_lte_sg_txpl_params_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  uint8                  carrier_index_bmsk;
  lm_handle_type         txlm_buffer_idx[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];
} rfa_rf_lte_fbrx_input_params_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  /*Indicates if ML1 needs to run FBRX*/
  boolean                  fbrx_enable;
  /*Indicates if PCC Drx needs to be tune-away for FBRX*/
  boolean                  pcc_drx_disruption_flag;
  /*FBRX trigger power threshold for single carrier*/
  int16                    fbrx_trig_pwr_threshold_sc;
  /*FBRX trigger power threshold for Intra-ULCA*/
  int16                    fbrx_trig_pwr_threshold_ulca;
  /*FBRX timer periodicity for Tune-away cases*/
  uint8                    fbrx_timer;
  /*FBRX timer periodicity for no Tune-away cases*/
  uint8                    fbrx_timer_no_tune_away;
} rfa_rf_lte_fbrx_output_params_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  rfa_rf_lte_sg_txpl_params_s sg_txpl_ulca_params[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];
} rfa_rf_lte_set_tx_plimit_ind_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
} rfa_rf_lte_get_ue_min_power_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type     hdr;
  int16   rf_min_tx_power; /* Units are in db10 */
  rfa_rf_lte_cmd_outcome_t req_result;
} rfa_rf_lte_get_ue_min_power_cnf_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type             hdr;
  rfa_rf_lte_fbrx_input_params_s   fbrx_params_input;
} rfa_rf_lte_get_fbrx_params_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type             hdr;
  rfa_rf_lte_fbrx_output_params_s  fbrx_params_output;
  rfa_rf_lte_cmd_outcome_t         req_result;
} rfa_rf_lte_get_fbrx_params_cnf_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  uint64 timestamp; 
  uint32 transaction_id; 
} rfa_rf_lte_transaction_hdr_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
	/* this transaction_id should be reset only during LTE exit and is maintained by ML1 */
  rfa_rf_lte_transaction_hdr_s tick_ind_params;
} rfa_rf_lte_ant_tuner_tick_ind_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  rfm_device_enum_type device;
  /* Antenna switch position */
  uint32 pos;
  void* cb_func;
  void* cb_data;
  boolean immediate;
} rfa_rf_lte_ant_sw_params_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  rfa_rf_lte_ant_sw_params_s ant_sw_params;
} rfa_rf_lte_ant_sw_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  rfa_rf_lte_ant_sw_params_s ant_sw_params;
  /* Result of the request */
  rfa_rf_lte_cmd_outcome_t req_result;
} rfa_rf_lte_ant_sw_cnf_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  uint32 carrier_idx_mask;
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  boolean config;
} rfa_rf_lte_rx1_chain_config_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type     hdr;
  uint32 carrier_idx_mask;
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  boolean config;
  rfa_rf_lte_cmd_outcome_t req_result;
} rfa_rf_lte_rx1_chain_config_cnf_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  rfa_rf_lte_ant_sw_params_s ant_sw_params;
} rfa_rf_lte_ant_sw_abort_ind_s;

/*-----------------------------------------------------------------------*/
typedef enum
{
  PORT_A = 0,/* this is the default setting */
  PORT_B = 1,/* this is the port B setting */
  PORT_MAX = 2,/* this is the Max allowed setting */
} rfa_rf_lte_port_state_type;

/*-----------------------------------------------------------------------*/
typedef enum
{
  RX_PORT = 0,/* this enables the Rx Port Update */
  TX_PORT = 1,/* this enables the Tx Port Update */
  RXTX_PORT = 2,/* this enables both Rx and Tx Port Update */
  RXTX_PORT_MAX = 3,
} rfa_rf_lte_rx_tx_chain_type;

/*-----------------------------------------------------------------------*/
typedef struct
{
  /* All active Tx carriers to be provided to comprehend Single carrier/Multi-carrier */
  uint8 active_carrier_mask; 
  uint32 txlm_handle[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];
} rfa_rf_lte_asm_update_params_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  rfa_rf_lte_asm_update_params_s lte_tdd_asm_params; 
} rfa_rf_lte_dynamic_asm_update_ind_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  rfa_rf_lte_asm_update_params_s lte_tdd_asm_params; 
  rfa_rf_lte_cmd_outcome_t asm_req_result; 
} rfa_rf_lte_dynamic_asm_update_cnf_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  /*! */
  uint32 carrier_idx;
  /*! */
  uint8 subframe_num;
  /*! */
  rfa_rf_lte_txpl_params_s tpl_params;
  /*! */
  uint8 pa_swpt_backoff;

} rfa_rf_lte_p_cmax_params_s;

/* ENUM to indicate current Uplink band configuration */
typedef enum
{
  /* Default scenario. One carrier Component */
  RFA_RF_LTE_SINGLE_UL_CARRIER = 0,

  /* MULTI CARRIER ULCA scenario */
  RFA_RF_LTE_MULTI_UL_CARRIER = 1,

  MAX_UPLINK_CARRIERS = 2,
}rfa_rf_lte_ul_carrier_config_type_e;


/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  /* Active Carrier Index Bitmask
  ** Ex: If PCC and SCC are active
  **     Bit Pos:| 7| 6| 5| 4| 3| 2| 1| 0|
  **               x  x  x  x  x  x  1  1
  **     active_carr_idx_bmsk = 0x03
  */
  uint8 active_carr_idx_bmsk;

  /* Flag to indicate if INTRA ULCA is active/inactive */
  boolean intra_ulca_active_flag;

  /*  Current carrier configuration */
  rfa_rf_lte_ul_carrier_config_type_e curr_ul_carrier_config;

  /* Current TxLM buffer Indexes for PCC and SCC */
  lm_handle_type txlm_buffer_idx[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED];

  /* Indicate active slots in current subframe */
  boolean comb_slot_active[RFA_RF_LTE_MAX_SLOTS];

  rfa_rf_lte_p_cmax_params_s tx_params[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED]; 

} rfa_rf_lte_p_cmax_update_ind_s;

/*-----------------------------------------------------------------------*/
/* This structure is used to support forcing a Tx/Rx path on a given
   band irrespective of its Channel/BW mapping
   We support upto 254 splits on a given band */
typedef PACK(struct)
{
  /*! Defines the Carrier_index/Cell_idx */
  uint8 carrier_id;
  /*! Defines the LTE Band. Always provide the full-band value */
  uint8 band;
  /*! Defines the Tx Path index */
  uint8 tx_path_index;
  /*! Defines the PRx Path index */
  uint8 prx_path_index;

  /*! Defines the PRx Path index */
  uint8 drx_path_index;

} rfa_rf_lte_path_sel_override_params_s;

/*-----------------------------------------------------------------------*/
typedef struct
{ 
  msgr_hdr_struct_type hdr;
  rfa_rf_lte_path_sel_override_params_s lte_path_sel_override_params;
} rfa_rf_lte_path_sel_override_update_ind_s; 

/*-----------------------------------------------------------------------*/
/* This structure is used to configure (tune\disable) single rx chain. */
typedef PACK(struct)
{
  /*! Defines the Carrier_info */
  //rflte_mc_carrier_info_type carrier_info;
  uint8 carrier_idx;
  rfa_rf_chain_param_type rx_chain_info;
  uint8 carrier_type;
  /*! Specifies chain to be enabled or disabled */
  boolean enable_chain;

} rfa_rf_lte_config_rx_chain_params_s;

/*-----------------------------------------------------------------------*/
typedef struct
{ 
  msgr_hdr_struct_type hdr;
  rfa_rf_lte_config_rx_chain_params_s lte_rx_chain_params;
} rfa_rf_lte_config_rx_chain_update_ind_s; 

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  uint32 carrier_idx_mask;
  uint32 exit_carrier_idx_mask;

  /*  Single Chain Support Bit Mask           
      RX_SC_BIT_MASK [8 bits] | TX_SC_BIT_MASK [8 bits]
   */
  uint16 single_chain_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];  

  /*  RF Cell Config Bit Mask                                                                                               *
   *  <Reserved 15:8> | DRX3 | DRX2 | DRX | PRX | <Reserved 3:1> | TX 
   */
  uint16 rf_cell_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];

  /* Boolean to indicate if RF or FW should execute Trigger\Non-TRx writes. */
  boolean exectue_script;

  rfa_rf_chain_param_type prx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx2[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx3[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_rx_param_type rx_param[RFA_RF_LTE_MAX_CELL_SUPPORTED];

}rfa_rf_lte_rx_script_based_config_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type     hdr;
  uint32 carrier_idx_mask;
  uint32 exit_carrier_idx_mask;

  /*  Single Chain Support Bit Mask           
      RX_SC_BIT_MASK [8 bits] | TX_SC_BIT_MASK [8 bits]
   */
  uint16 single_chain_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];  

  /*  RF Cell config Bit Mask                                                                                               *
   *  <Reserved 15:8> | DRX3 | DRX2 | DRX | PRX | <Reserved 3:1> | TX 
   */
  uint16 rf_cell_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];

  /* Boolean to indicate if RF or FW should execute Trigger\Non-TRx writes. */
  boolean exectue_script;
  
  rfa_rf_chain_param_type prx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx2[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_chain_param_type drx3[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_rx_param_type rx_param[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_lte_cmd_outcome_t req_result;
  /* USTMR time stamp */
  uint64 ustmr_timestamp;

}rfa_rf_lte_rx_script_based_config_cnf_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  uint32 carrier_idx_mask;
  uint32 exit_carrier_idx_mask;
  /*  Single Chain Support Bit Mask           
      RX_SC_BIT_MASK [8 bits] | TX_SC_BIT_MASK [8 bits]
   */
  uint16 single_chain_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];

  /*  RF Cell config Bit Mask                                                                                               *
   *  <Reserved 15:8> | DRX3 | DRX2 | DRX | PRX | <Reserved 3:1> | TX 
   */
  uint16 rf_cell_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  
  /* Boolean to indicate if RF or FW should execute Trigger\Non-TRx writes. */
  boolean execute_script;  

  rfa_rf_chain_param_type tx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_tx_param_type tx_param[RFA_RF_LTE_MAX_CELL_SUPPORTED];

}rfa_rf_lte_tx_script_based_config_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type     hdr;
  uint32 carrier_idx_mask;
  uint32 exit_carrier_idx_mask;
  /*  Single Chain Support Bit Mask           
      RX_SC_BIT_MASK [8 bits] | TX_SC_BIT_MASK [8 bits]
   */
  uint16 single_chain_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];  

  /*  RF Cell config Bit Mask                                                                                               *
   *  <Reserved 15:8> | DRX3 | DRX2 | DRX | PRX | <Reserved 3:1> | TX 
   */
  uint16 rf_cell_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];  
 
  /* Boolean to indicate if RF or FW should execute Trigger\Non-TRx writes. */
  boolean execute_script;
 
  rfa_rf_chain_param_type tx[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_tx_param_type tx_param[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  rfa_rf_lte_cmd_outcome_t req_result;

}rfa_rf_lte_tx_script_based_config_cnf_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  /*   Bit Mask per Path 
   *   <Reserved 15:8> | DRx3 | DRx2 | DRx | PRx | <Reserved 3:1> | Tx | 
   */
  uint16                       path_bitmask;
  rfa_rf_chain_param_type      tx;
  rfa_rf_chain_param_type      prx;
  rfa_rf_chain_param_type      drx;
  rfa_rf_chain_param_type      drx2;
  rfa_rf_chain_param_type      drx3;
  rfa_rf_tx_param_type         tx_param;
  rfa_rf_rx_param_type         rx_param;

}rfa_rf_lte_get_single_chain_support_params_s;
/*-----------------------------------------------------------------------*/
typedef struct
{

  msgr_hdr_struct_type   hdr;

  /* Current RF configuration info.*/
  rfa_rf_lte_get_single_chain_support_params_s current_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];

  /* Requested RF configuration info.*/ 
  rfa_rf_lte_get_single_chain_support_params_s requested_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];  

}rfa_rf_lte_get_single_chain_support_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{

  msgr_hdr_struct_type   hdr;

  /* Current RF configuration info.*/
  rfa_rf_lte_get_single_chain_support_params_s current_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];

  /* Requested RF configuration info.*/
  rfa_rf_lte_get_single_chain_support_params_s requested_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];  

  /*  Single Chain Support Bit Mask           
      RX_SC_BIT_MASK [8 bits] | TX_SC_BIT_MASK [8 bits]
   */
  uint16 single_chain_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];  
  
  rfa_rf_lte_cmd_outcome_t req_result;

}rfa_rf_lte_get_single_chain_support_cnf_s;


/*-----------------------------------------------------------------------*/
typedef struct
{
  /*   Bit Mask per Path 
   *   <Reserved 15:8> | DRx3 | DRx2 | DRx | PRx | <Reserved 3:1> | Tx | 
   */
  uint16                       path_bitmask;
  rfa_rf_chain_param_type      tx;
  rfa_rf_chain_param_type      prx;
  rfa_rf_chain_param_type      drx;
  rfa_rf_chain_param_type      drx2;
  rfa_rf_chain_param_type      drx3;
  rfa_rf_tx_param_type         tx_param;
  rfa_rf_rx_param_type         rx_param;

}rfa_rf_lte_check_config_params_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  
  /* Current RF configuration info.*/
  rfa_rf_lte_check_config_params_s current_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];

  /* Requested RF configuration info.*/
  rfa_rf_lte_check_config_params_s requested_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];

  /*  Single Chain Support Bit Mask           
      RX_SC_BIT_MASK [8 bits] | TX_SC_BIT_MASK [8 bits]
   */
  uint16 single_chain_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];  

} rfa_rf_lte_check_config_req_s;

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type     hdr;
  uint32 carrier_idx_mask; 
  uint32 exit_carrier_idx_mask;

  /* Current RF configuration info.*/
  rfa_rf_lte_check_config_params_s current_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];

  /* Requested RF configuration info.*/
  rfa_rf_lte_check_config_params_s requested_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];

  /*  Single Chain Support Bit Mask           
      RX_SC_BIT_MASK [8 bits] | TX_SC_BIT_MASK [8 bits]
   */
  uint16 single_chain_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];  

  /*  RF Cell config Bit Mask                                                                                               *
   *  <Reserved 15:8> | DRX3 | DRX2 | DRX | PRX | <Reserved 3:1> | TX 
   */
  uint16 rf_cell_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];

  rfa_rf_lte_cmd_outcome_t req_result;

} rfa_rf_lte_check_config_cnf_s;


/*-----------------------------------------------------------------------*/
typedef struct
{
   uint8   scenario_val;
   boolean   is_scenario_valid;
   rfm_tuner_handle_type   tuner_handle;
} rfa_rf_tuner_config_param_type;


/*-----------------------------------------------------------------------*/
typedef struct
{
  uint32 carrier_idx;                   /* rflte_mc_cell_idx_type */ 
  uint32 path_idx;                      /* rflte_path_type */
  rfm_device_enum_type  device;
  lm_handle_type   handle_id ;          /*lm_buffer_idx */
  rfcom_lte_earfcn_type   channel;
  rfcom_lte_bw_type    bw;
  rfa_rf_tuner_config_param_type  tuner_config_param;
  boolean rf_execute_script;

} rfa_rf_config_param_type_s ;  


/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  int num_current_chains;  // number of chains configured
  int num_requested_chains; // number of chains requested to be configured
  /* Current RF configuration followed by Final RF configuration info */
  rfa_rf_config_param_type_s chain_config[RFA_RF_MAX_NUM_CHAIN_CONFIGS];
} rfa_rf_lte_config_req_s;


/*-----------------------------------------------------------------------*/
typedef struct
{
/*lm_buffer_idx for the chain*/
lm_handle_type handle_id;
/*Indicate if preload script need to be executed  */
boolean preload_script_execute;
/*! Specifies preload script execution time in us units  */
uint16 preload_exec_time:16;
/*! Specifies trigger script execution time in us units  */
uint16 trig_exec_time:16;
/*! Specifies trigger script execution start time in us units  */
int16 trig_offset_time:16;
/*Indicate if trigger script need to be executed  */
boolean trigger_script_execute;
/*! Specifies non trx script execution start time in us units  */
int16 non_trx_offset_time:16;
/*Indicate if non-trx script need to be executed  */
boolean non_trx_script_execute;
/*! Specifies non-trx script execution time in us units  */
uint16 non_trx_exec_time:16;
/*Indicate if blanking is required  */
boolean blanking_required;
/*Indicate if freeze is required which is starting preloading  */
boolean freeze_required;
/*Indicate if handle is in use  */
boolean is_used;

} rfa_rf_lte_script_exec_info_s;


/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  int num_current_chains;  // number of chains configured
  int num_requested_chains; // number of chains requested to be configured
  /* Current RF configuration followed by Final RF configuration info */
  rfa_rf_config_param_type_s chain_config[RFA_RF_MAX_NUM_CHAIN_CONFIGS];
  /* Handle for scripts */
  rfa_rf_lte_script_exec_info_s script_exec_info[RFA_RF_MAX_NUM_CHAIN_CONFIGS];
  boolean                  antenna_tuner_present;
  /* Used in MSIM wakeup for ML1*/  
  uint64 ustmr_timestamp;

} rfa_rf_lte_config_cnf_s;


/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  int status; //status = -1 (error ), status = 0 (cancelled), status =1 (done)

  int num_current_chains;  // number of chains configured
  int num_requested_chains; // number of chains requested to be configured
  /* Current RF configuration followed by Final RF configuration info */
  rfa_rf_config_param_type_s chain_config[RFA_RF_MAX_NUM_CHAIN_CONFIGS];
  rfa_rf_lte_script_exec_info_s  script_exec_info [RFA_RF_MAX_NUM_CHAIN_CONFIGS]  ;

} rfa_rf_lte_config_complete_s;


/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type   hdr;
  
  int num_current_chains;  // number of chains configured
  int num_requested_chains; // number of chains requested to be configured
  /* Current RF configuration followed by Final RF configuration info */
  rfa_rf_config_param_type_s chain_config[RFA_RF_MAX_NUM_CHAIN_CONFIGS];
} rfa_rf_lte_check_config_v2_req_s;


#define RFA_RF_LTE_CC0_CONFIG_MASK      (1U<<0)
#define RFA_RF_LTE_CC1_CONFIG_MASK      (1U<<1)
#define RFA_RF_LTE_CC2_CONFIG_MASK      (1U<<2)

#define RFA_RF_LTE_TX0_CELL_CONFIG_MASK (1U<<0)
#define RFA_RF_LTE_TX1_CELL_CONFIG_MASK (1U<<1)
#define RFA_RF_LTE_RX0_CELL_CONFIG_MASK (1U<<4)
#define RFA_RF_LTE_RX1_CELL_CONFIG_MASK (1U<<5)
#define RFA_RF_LTE_RX2_CELL_CONFIG_MASK (1U<<6)
#define RFA_RF_LTE_RX3_CELL_CONFIG_MASK (1U<<7)

/*-----------------------------------------------------------------------*/
typedef struct
{
  msgr_hdr_struct_type     hdr;

  int num_current_chains;  // number of chains configured
  int num_requested_chains; // number of chains requested to be configured
  /* Current RF configuration followed by Final RF configuration info */
  rfa_rf_config_param_type_s chain_config[RFA_RF_MAX_NUM_CHAIN_CONFIGS];

  /*! @brief
      If any path of a current component carrier is affected by the retune 
      operation, the corresponding bit in this mask will be set.
   
      @details
      These masks may be used to access this member in an abstracted manner:
      * RFA_RF_LTE_CC0_CONFIG_MASK
      * RFA_RF_LTE_CC1_CONFIG_MASK
      * RFA_RF_LTE_CC2_CONFIG_MASK
      */
  uint32 carrier_idx_mask; 

  /*! Unused in version 2 of CHECK_CONFIG. */
  uint32 exit_carrier_idx_mask;

  /*! @brief
      If a carrier in the current configuration set will be affected 
      by the tuning operation of any of the carriers in the requested
      configuration set, the corresponding bit will be set in this
      array.
   
      @details
      These masks may be used to access this member in an abstracted manner:
      * RFA_RF_LTE_TX0_CELL_CONFIG_MASK
      * RFA_RF_LTE_TX1_CELL_CONFIG_MASK
      * RFA_RF_LTE_RX0_CELL_CONFIG_MASK
      * RFA_RF_LTE_RX1_CELL_CONFIG_MASK
      * RFA_RF_LTE_RX2_CELL_CONFIG_MASK
      * RFA_RF_LTE_RX3_CELL_CONFIG_MASK
      */
  uint16 rf_cell_config[RFA_RF_LTE_MAX_CELL_SUPPORTED];

  rfa_rf_lte_cmd_outcome_t req_result;

} rfa_rf_lte_check_config_v2_cnf_s;

/*! @} */

/*-----------------------------------------------------------------------*/

#endif /* RF_LTE_MSG_H */
