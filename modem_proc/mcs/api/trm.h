#ifndef TRM_H
#define TRM_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*=

           T R A N S C E I V E R   R E S O U R C E   M A N A G E R

                   Transceiver Resource Manager Header File

GENERAL DESCRIPTION

  This file provides management of the RF Rx/Tx resources


EXTERNALIZED FUNCTIONS

  trm_reserve_at() - Registers a client's need to access the RF at a specific
    moment in the future for the indicated duration, such as for a paging
    slot.  trm_request() must be used at the specified moment, to actually
    obtain the RF resource.  Implicitly performs a trm_release().

  trm_request() - Request access to an RF chain immediately, for the indicated
    duration.  Implicitly performs a trm_release().

  trm_request_and_notify() - Request access to an RF chain, when it becomes
    available.  Implicitly performs a trm_release().


  trm_extend_duration() - Requests an increase in the duration the RF is
    held for by the client.  If the increase cannot be granted, the original
    grant duration remains in effect.

  trm_change_duration() - Requests a change in the duration the RF is held for
    by the client.  If the full extension cannot be granted, a partial
    extension may result.

  trm_change_priority() - Specifies a change in the reason (and consequently
    priority) for a client's hold on an RF resource.

  trm_exchange() - Swaps the resources that two clients hold.  This is useful
    when the primary client holds the secondary RF chain, and the secondary
    client acquires the primary RF chain.  After this function is called, the
    primary client will hold the primary RF chain, and the secondary client
    will hold the secondary RF chain.

  trm_release() - Releases the clients hold on an RF resource.  If no
    client is waiting for the RF resource, the RF will be turned off.


  trm_get_granted() - This function indicates if a client holds an RF
    resource.  Ideally, the client should already know and maintain knowledge
    of whether or not it holds an RF resource.

  trm_get_rf_device() - Returns the identifier associated with the granted
    RF chain.

  trm_get_1x_mode() - Return the 1X mode

  trm_get_device_mapping() - Returns the device mapping for certain (band
     resource and client) combination.
 
  trm_get_device_mapping_multiple() - Returns the device mapping for certain
    (band resource and client) combination for multiple requests.

REGIONAL FUNCTIONS

  None


INITIALIZATION AND SEQUENCING REQUIREMENTS

  trm_task() must be called before any other function.

  Before using any RF functions, a client must be granted permission via
  trm_request( ) or trm_request_and_notify().

  When the client is finished using the RF, it must release its hold on the
  RF chain either directly via a call to trm_release(), or indirectly by
  calling trm_reserve_at(), trm_request_and_notify(), or trm_request().


  Copyright (c) 2004-2021 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by this document
  are confidential and proprietary information of Qualcomm Technologies
  Incorporated and all rights therein are expressly reserved.  By accepting
  this material the recipient agrees that this material and the information
  contained therein are held in confidence and in trust and will not be used,
  copied, reproduced in whole or in part, nor its contents revealed in any
  manner to others without the express written permission of Qualcomm
  Technologies Incorporated.


=*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*=



==============================================================================

                           EDIT HISTORY FOR MODULE


This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     ---------------------------------------------------------
05/05/2016   ag      Added new api to query upper layer information
11/04/2016   md      Add support for L+L DSDS
12/04/2015   sk      Support for LTE query to proceed with QTA(941699)
09/24/2015   pap     Add changes to make L+W DR only when appropriate NV is set.
08/31/2015   mn      Changes for LTE CA device hopping (CR: 894455).
08/24/2015   rj      Added support for CTCH (Broadcast MultiCast) channel.
08/06/2015   sg      RAT access mode info support.
07/17/2015   sr      Support for RF device handler (CR:  874205)
07/06/2015   mn      Handle data tech passing all devices in START QTA (CR: 864477)
07/02/2015   jm      Support to get RF HWID in trm_get_info (CR: 862682)
06/17/2015   sp      Added ACQ_CELL_SELECT AND ACQ_CELL_SELECT_INV DR reasons 
05/19/2015   sp      Removed reasons and made other TRM-U fixes/cleanup (CR: 827122)
05/01/2015   sg      QXTA support
04/18/2015   sk      Support for grouped APIs/async callbacks
04/16/2015   sp      Added DR_TRAFFIC reason + sub-reason ID as input to requests 
                     for TRM-U (CR: 812193)
04/10/2015   sr      Added new UMTS client for W+W
03/30/2015   sk      Mechanism to check if DTA required with currently active
                     techs (812760)
03/17/2015   tl      Add API to register for ASDiv events by non-clients
02/18/2015   sk      Update log v8 for eMBMS logging(796676)
01/21/2015   jm      Adding TRM metric query support (CR: 778020)
01/06/2014   jm      Adding TRM metric query support -- header file only (CR: 778020)
12/31/2014   mn      TRM should internally register for connected mode 
                      hop reasons (CR: 775727).
12/11/2014   mn      LTE EMBMS functional changes (CR: 769538).
12/10/2014   jm      Remove deprecated APIs not referenced in CRM build
11/14/2014   sr      Add a SLEEP Diversity reason for LTE to use for diversity
                     to avoid sending wrong unlocks due to PAM (CR: 751597)
10/24/2014   mn      Support for LTE HO RxD (CR: 744539)
10/18/2014   sk      support for DRDSDS ASDiv(707105)
10/13/2014   mn      Increasing MAX_BAND_INFO to 7 (CR: 737579).
10/10/2014   ag      Added interface for ASDIV to get tech state indication
09/17/2014   sk      Changes for tx sharing DSDA
09/29/2014   mn      Added a new TRM_DIVERSITY_IRAT_MEASUREMENT reason (CR: 712921).
09/25/2014   sr      Change Reason with min duration (CR: 715526)
09/16/2014   sr      Option to change reason to IRAT during IRAT (CR:724649)
08/17/2014   sr      Support for multiple chain release(710705)
08/14/2014   sk      wlan IRAT conflict support(708790)
08/12/2014   mn      Support for a unified IRAT mechanism which works across all 
                      TRM modes (CR: 705286).
08/08/2014   mn      Added okay_to_shorten flag for MODIFY_DURATION (CR: 703746).
08/04/2014   sk      Support to enable disable hopping for L+G coex issue(678827)
07/23/2014   sr      GERAN using deprecated priority causing L2G resel/redir/CCO 
                     failure in DR-DSDS mode (700576)
07/23/2014   mn      Adding new GSM secondary clients (CR: 696634).
07/22/2014   mn      Expose sp_trm_mode_override() API for RF usage in 
                      FTM mode (CR: 689879).
07/18/2014   mn      1xSRLTE:- Requirement to default PBR bias values fro 1x 
                      and SLTE in SW code (CR: 681568).
07/02/2014   mn      Enable IFS and IRAT for DR-DSDS. (CR: 688066).
06/30/2014   mn      Corrected critical section handling in trm_request_and_notify
                      (CR: 687261).
06/26/2014   mn      TRM should bypass the SHDR dual receive chain logic when in 
                      DR-DSDS mode (CR: 685907).
06/24/2014   mn      Adding support for num pending callbacks in req and not,
                      req and not enhanced and unlock callbacks. (CR: 684638).
06/15/2014   sk      WLAN antenna sharing changes (615092)
06/18/2014   mn      Changes for DR-DSDS (CR: 651800)
05/15/2014   sk      TRM support for Adaptive PAge Skip
03/25/2014   sk      New Reason for LTE high priority signalling(653561)
04/07/2014   sk      TRM support for QTA
04/03/2014   sk      New Reason for LTE emergency SIB Reads
03/28/2014   sr      Changes for QTA gap issue in SGLTE+G (CR:640167)
03/20/2014   sk      SVLTE support for ASDiv, new api to block switching
03/19/2014   jm      WWAN coex based off of RF device assignment
02/27/2014   sr      SLTE Changes
03/12/2014   mn      Adding support for connected mode WTR hopping.
02/13/2014   sk      Added channel_maintenance_inv reason 
02/10/2014   sk      Changes for Bolt SHDR 
01/10/2014   sk      Changes for SGLTE+G WTR hopping requirement
01/05/2014   sr      In PBR, increments within 100 ms are redundant(CR:591479)
11/26/2013   mn      Adding support for Opp SGLTE/SGTDS/SVLTE + G.
11/25/2013   sr      PBR Algorithm Implementation
11/21/2013   jm      trm_update_tech_state, update G/W/TDS state in single SIM
10/02/2013   jm      Adding API for querying multiple device mappings  
09/19/2013   rj      Adding TRM support GSM to ASID mapping
09/19/2013   rj      Adding TRM support for G2W TRM_LTA_SUPPORTED
09/12/2013   sk      L+G DSDS support
06/13/2013   sk      ASDiv Enhancement Changes
05/21/2013   mn      Adding support for TSTS.
05/31/2013   rj      trm_get_device_mapping API support 
05/16/2013   sk      ASDiv feature porting
05/14/2013   rj      Added support for SGLTE in TRM
02/04/2012   sr      Add a new priority TRM_ACQ_DIVERSITY for HDR(CR:439864)
05/14/2012   sr      Changes to REVERSE_ANY and REVERSE_BP.
12/07/2012   ag      Remove trm_init header
11/22/2012   mn      Triton DSDA changes.
11/19/2012   rj      Added a new client TRM_CM and new reason TRM_SUBS_CAP_CHANGE.
10/21/2012   sr      TRM Logging fix changes (CR: 412848).
10/03/2012   mn      NikeL DSDS merge
08/13/2012   ag      Added new reason for non-urgent 1x rude wakeup
08/09/2012   sr      Modified the order of trm clients to avoid parser change.
08/07/2012   sr      Dime Changes: Added new clients and api for Dime.
04/20/2012   ag      Added new api - trm_change_reason
02/10/2012   ag      Support for SVLTE+noSVDO mode.
02/08/2012   sr      new methods for rc-init task initialization and startup.
01/10/2012   ag      Adding two new diversity reasons.
09/19/2011   sr      Adding API for querying supported modem technologies.
08/15/2011   sg      Added support for TDSCDMA Layer1
07/07/2011   ag      Support for wakeup manager.
04/22/2011   ag      Single Api that returns SV and SHDR capability
04/21/2011   ag      Api that checks whether shdr mode is enabled.
02/10/2011   sg      Added new reasons for ET and HDR IRAT measurement
11/22/2010   ag      Added new reason SMALL_SCI_CONTINUATION for HDR. 
10/22/2010   sg      Fixed a compilation issue.
10/18/2010   ag      Merged DSDS functionality.
08/23/2010   ag      Merged GSM/WCDMA reasons.
06/17/2010   ag      Added new reason TRM_MC_TRAFFIC for HDR to do a 'hybrid 
                     like' traffic in SHDR mode. 
06/01/2010   hh      Added new reason TRM_SYSTEM_MEASUREMENT for 1x system 
                     measurement changes.
03/09/2010   ag      Removed trm_init() function declaration.
10/28/2009   ag      Moved TRM_NO_CLIENT to trm_client_enum_t
08/31/09     ag      Removing customer.h
02/27/09     ns      Enable trm_get_rf_device for Off Target builds 
02/17/09     cpk     Added support for Off Target builds
09/18/2008   rkc     Added TRM_DEMOD_PAGE_CONTINUATION to trm_reason_enum_t
08/28/2008   adw     Added FEATURE_MCS_TRM to featurize TRM for ULC.
06/06/2008   sg      Added TRM_HP_BROADCAST in trm_reason_enum_t
05/20/2008   sg      Consolidated new NV items into new 1x_mode enum/API
03/18/2008   sg      Remove FTS-3 Featurization from num_antennas/1x SecondaryOnly 
                     functions
02/11/2008   sg      Added NV fields for num antennas and 1x SecondaryOnly mode 
01/24/2008   sg      Added Full Time SHDR-3 support
07/11/2007   cab     Added trm_timestamp_t
10/17/2006   cab     Added TRM_DEMOD_SMALL_SCI_PAGE reason.
09/20/2006   cab     Integrated UMTS
09/20/2006   cab     Added include for customer.h
07/28/2006   jwob    Added #error for !FEATURE_TRM_API_V2
07/13/2006   jwob    Tag parameter to trm_request_and_notify and callback
07/12/2006   jwob    Removed trm_cancel_request_and_notify
05/26/2006   cab     Updated trm_extend_duration comment
05/02/2006   cab     Added ifdef's for offline compilation and testing
12/16/2005   cab     Added some comments 
12/12/2005   grl     Corrected enum to preserve backward compatibility with 
                     TRM log parsers.
11/02/2005   cab     Added BEST_POSSIBLE resource option - pre-empts lower
                     pri clients from chain 0, but takes chain 1 if higher 
                     pri clients have chain 0
09/20/2005   grl     Made the last supported reason TRM_LOW_LAT_TRAFFIC.
09/10/2005   grl     Added broadcast access/traffic reasons.
08/29/2005   grl     Added support for dynamic compatibility masks and unlock
                     immediate in the unlock callback.
06/29/2005   ajn     Added typedef for trm_init's wait callback.
05/26/2005   ajn     Code review comment changes
03/28/2005   ajn     Added GPS_TDM reason.
03/16/2005   ajn     Added get_rf_config.
03/01/2005   ajn     Timestamp to unlock callback.
01/07/2005   ajn     Removed GRANTED_0_GIVE_1.  Use trm_exchange() instead.
12/29/2004   ajn     Moved RFM_* helpers to trm_rf.h
11/12/2004   ajn     Initial AMSS version

============================================================================*/


/*============================================================================

                           INCLUDE FILES FOR MODULE

============================================================================*/

#include <comdef.h>
#include <timetick.h>
#include "hwio_cap.h"
#include "rfm_device_types.h"
#include "rfm_path_types.h"
#include "sys.h"
#include "rfmeas_types.h"
#include "sys_v.h"

/*============================================================================

                   DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

============================================================================*/

/* Don't shorten the lock duration - only extend it, if possible */
#define TRM_DONT_SHORTEN          0xFFFFFFFFuL

/* This driver can only support 8 Rx + 2Tx + 1GPS + 1 WLAN chains */
#define TRM_MAX_VALID_CHAINS              ((uint32)RFM_MAX_WAN_DEVICES)
#define TRM_MAX_CHAINS                    (TRM_MAX_VALID_CHAINS + 2)

#define TRM_MAX_EVENT_REG_NUM             10

/* Max chains that could be released simultaneously in multiple release api  */
#define TRM_MAX_RELEASE_CLIENTS           4

/* maximum concurrent techs, 
   value same as CXM_MAX_CONCURRENT_TECHS */
#define TRM_ASDIV_MAX_CONCURRENT_TECHS    4

/* Max Number of Primary and Diversity for a Client  */
#define TRM_NUM_PRI_DIV_CLIENTS_PER_TECH  2

/* Max Number of devices a Tech's Clients can hold, which will be used during QTA  */
#define TRM_MAX_DEVICE_TECH_HOLD          TRM_MAX_VALID_CHAINS


/* multi techs group max entry */
#define TRM_GROUPED_CLIENTS_MAX           15

/* SRLTE and DSDS mode bit values (not position) for upper layer mode detection */
#define TRM_DSDS_MODE_BIT     (uint8)0x1
#define TRM_SRLTE_MODE_BIT    (uint8)0x2
#define TRM_DUAL_MULTI_MODE_BIT (uint8)0x4

#define TRM_MAX_ELS_IDLE_TECHS  8
/*----------------------------------------------------------------------------
  Transceiver resource clients (who?)
    These represent the possible owners of the Rx/Tx chains.
 
    TRM_xxx_SECONDARY clients are intended for use with the secondary RF chain
    for diversity or for off-frequency searching without tuning the primary
    RF chain away from the active frequency.

    Note: Always add to the end in order to preserve backward compatibility
          with the TRM log parsers.

----------------------------------------------------------------------------*/

typedef enum
{
  TRM_1X = 0,
  TRM_1X_SECONDARY = 1,

  TRM_HDR = 2,
  TRM_HDR_SECONDARY = 3, 

  TRM_GPS = 4,

  TRM_UMTS = 5,
  TRM_UMTS_SECONDARY = 6,

  TRM_GSM1 = 7,

  TRM_GSM2 = 8,

  TRM_WCDMA = 9,

  TRM_TDSCDMA = 10,
  TRM_TDSCDMA_SECONDARY = 11,

  TRM_UMTS_CA = 12,
  TRM_UMTS_CA_SECONDARY = 13,

  TRM_LTE = 14,
  TRM_LTE_SECONDARY = 15,

  TRM_LTE_SUB2 = 16,
  TRM_LTE_SUB2_SECONDARY = 17,

  TRM_CM = 18,

  TRM_GSM3 = 19,

  TRM_GSM_SECONDARY = 20,


  TRM_RF = 21,

  /* Dummy client for L+G DSDS */
  TRM_IRAT = 22,

  /* WLAN client */
  TRM_WLAN = 23,

  TRM_GPRS1 = 24,

  TRM_GPRS2 = 25,

  TRM_GSM1_SECONDARY = 26,

  TRM_GSM2_SECONDARY = 27,

  TRM_GSM3_SECONDARY = 28,

  /* Client for SUB2 Reselection Exchange */
  TRM_IRAT2 = 29,

  /* SUB2 WCDMA PRx Client */
  TRM_UMTS2 = 30,

  /* SUB2 WCDMA DRx Client */
  TRM_UMTS2_SECONDARY = 31,

  TRM_UMTS_SECONDARY2 = TRM_UMTS2_SECONDARY,

  /* Tx clients: rxtx split */
  TRM_1X_TX = 32,

  TRM_HDR_TX = 33,

  TRM_UMTS_TX = 34,

  TRM_UMTS2_TX = 35,

  TRM_UMTS_CA_TX = 36,

  TRM_GSM1_TX = 37,
  
  TRM_GSM2_TX = 38,

  TRM_TDSCDMA_TX = 39,

  TRM_LTE_TX = 40,

  TRM_LTE_SUB2_TX = 41,

  TRM_GPRS1_TX = 42,

  TRM_GPRS2_TX = 43,

  TRM_IRAT_TX = 44,

  TRM_IRAT2_TX = 45,

  TRM_WLAN_SECONDARY = 46,

  TRM_WLAN_5G = 47, 

  TRM_WLAN_5G_SECONDARY = 48,

  TRM_LAST_STATIC_CLIENT = TRM_WLAN_5G_SECONDARY,

  /* There is a gap 12 clients between static cand dynamic client ids
     New static clients can be added here without affecting Dynamic client ids 
     if new static  cleint id are  <= 60. */

  /* dynamic clients */

  TRM_LTE_CA = 61,

  TRM_LTE_CA_SECONDARY = 62,

  TRM_LTE_CA_TX = 63,

  TRM_LTE_CA1 = 64,

  TRM_LTE_CA1_SECONDARY = 65,

  TRM_LTE_CA1_TX = 66,

  TRM_LTE_CA2 = 67,

  TRM_LTE_CA2_SECONDARY = 68,

  TRM_LTE_CA2_TX = 69,

  TRM_LTE_HO_SECONDARY1 = 70,

  TRM_LTE_4_RX0 = TRM_LTE_HO_SECONDARY1,

  TRM_LTE_HO_SECONDARY2 = 71,

  TRM_LTE_4_RX1 = TRM_LTE_HO_SECONDARY2,

  TRM_LTE_CA_4_RX0 = 72,

  TRM_LTE_CA_4_RX1 = 73,

  TRM_LTE_CA1_4_RX0 = 74,

  TRM_LTE_CA1_4_RX1 = 75,

  TRM_LTE_CA2_4_RX0 = 76,

  TRM_LTE_CA2_4_RX1 = 77,

  TRM_LTE_EMBMS1 = 78,
  
  TRM_LTE_EMBMS2 = 79,

  TRM_LTED = 80,
  TRM_LTED_SECONDARY = 81,

  TRM_LTED_TX = 82,

  TRM_LTE_CGI = 83,

  TRM_LTE_CGI_SECONDARY = 84,

  TRM_LTE_SUB2_CA = 85,

  TRM_LTE_SUB2_CA_SECONDARY = 86,

  TRM_LTE_SUB2_CA_TX = 87,

  TRM_LTE_SUB2_CA1 = 88,

  TRM_LTE_SUB2_CA1_SECONDARY = 89,

  TRM_LTE_SUB2_CA1_TX = 90,

  TRM_LTE_SUB2_CA2 = 91,

  TRM_LTE_SUB2_CA2_SECONDARY = 92,

  TRM_LTE_SUB2_CA2_TX = 93,

  TRM_LTE_SUB2_4_RX0 = 94,

  TRM_LTE_SUB2_4_RX1 = 95,

  TRM_LTE_SUB2_CA_4_RX0 = 96,

  TRM_LTE_SUB2_CA_4_RX1 = 97,

  TRM_LTE_SUB2_CA1_4_RX0 = 98,

  TRM_LTE_SUB2_CA1_4_RX1 = 99,

  TRM_LTE_SUB2_CA2_4_RX0 = 100,

  TRM_LTE_SUB2_CA2_4_RX1 = 101,

  TRM_LTE_SUB2_EMBMS1 = 102,

  TRM_LTE_SUB2_EMBMS2 = 103,

  TRM_LTE_SUB2_LTED = 104,

  TRM_LTE_SUB2_LTED_SECONDARY = 105,

  TRM_LTE_SUB2_LTED_TX = 106,

  TRM_LTE_SUB2_CGI = 107,

  TRM_LTE_SUB2_CGI_SECONDARY = 108,
  
  TRM_LTE_CA3 = 109,
  TRM_NR5G_CA3 =  TRM_LTE_CA3,

  TRM_LTE_CA3_SECONDARY = 110,
  TRM_NR5G_CA3_SECONDARY =  TRM_LTE_CA3_SECONDARY,

  TRM_LTE_CA3_TX = 111,
  TRM_NR5G_CA3_TX = TRM_LTE_CA3_TX,

  TRM_LTE_CA3_4_RX0 = 112,
  TRM_NR5G_CA3_4_RX0 = TRM_LTE_CA3_4_RX0,

  TRM_LTE_CA3_4_RX1 = 113,
  TRM_NR5G_CA3_4_RX1 = TRM_LTE_CA3_4_RX1,

  TRM_LTE_SUB2_CA3 = 114,

  TRM_LTE_SUB2_CA3_SECONDARY = 115,

  TRM_LTE_SUB2_CA3_TX = 116,

  TRM_LTE_SUB2_CA3_4_RX0 = 117,

  TRM_LTE_SUB2_CA3_4_RX1 = 118,

  /* For internal bound-checking and array-sizing */
  TRM_LAST_CLIENT = TRM_LTE_SUB2_CA3_4_RX1,

  /* Should always be TRM_LAST_CLIENT + 1 */
  TRM_MAX_CLIENTS,

  TRM_NO_CLIENT    = 0xFF
}
trm_client_enum_t;

/*----------------------------------------------------------------------------
  Transceiver resource requests (what?)
    These represent what the clients can ask for.

    Note: Always add to the end in order to preserve backward compatibility
          with the TRM log parsers.

    Note: Always update trm_chains array when modifying this enum

----------------------------------------------------------------------------*/

typedef enum
{
  /* Any available Rx chain */
  TRM_RX_ANY,

  /* ... with the highest sensitivity, nothing if not available */
  TRM_RX_BEST,

  /* The secondary receive chain */
  TRM_RX_SECONDARY,

  /* The receive chain associated with GPS antenna */
  TRM_RX_GPS,

  /* Any RF chain with both Rx and Tx capability */
  TRM_RXTX_ANY,

  /* The RXTX RF chain the best sensitivity, nothing if not available */
  TRM_RXTX_BEST,

  /* ... with the highest sensitivity, or lower if not available */
  TRM_RX_BEST_POSSIBLE,

  /* The RXTX RF chain the best sensitivity, or lower if not available */
  TRM_RXTX_BEST_POSSIBLE,

  /* The RXTX SV RF chain */
  TRM_RXTX_SV,

  /* ... with the highest sensitivity, or lower if its already locked */
  TRM_RX_BEST_POSSIBLE_2,


  TRM_RX_CA_PRIMARY,

  TRM_RX_CA_SECONDARY,

  /* Preferred resource for a single mode subscription under DSDA context */
  TRM_RXTX_BEST_SMODE,

  TRM_REVERSE_RX_BEST_POSSIBLE,

  TRM_REVERSE_RX_ANY,

  TRM_RX_BEST_POSSIBLE_MOD,

  TRM_RX_BEST_POSSIBLE_MOD_2,

  TRM_RX_SECOND_PREFERRED,

  TRM_RX_DIVERSITY,

  TRM_RX_DIVERSITY_SECONDARY,

  /* tx resources: rxtx split */
  TRM_TX_ANY,

  TRM_TX_BEST,

  /* For internal bound-checking */
  TRM_LAST_RESOURCE = TRM_TX_BEST,

  TRM_NUM_RESOURCES
}
trm_resource_enum_t;


/*----------------------------------------------------------------------------
  Enum indicating Type of TRM client at a high level
----------------------------------------------------------------------------*/
typedef enum
{
  TRM_CLIENT_PRIMARY,
  TRM_CLIENT_DIVERSITY,
  TRM_CLIENT_CA,
  TRM_CLIENT_CA_DIVERSITY,
  TRM_CLIENT_CA1,
  TRM_CLIENT_CA1_DIVERSITY,
  TRM_CLIENT_CA2,
  TRM_CLIENT_CA2_DIVERSITY,
  TRM_CLIENT_CA3,
  TRM_CLIENT_CA3_DIVERSITY,
  TRM_CLIENT_HO_DIVERSITY,
  TRM_CLIENT_4_LAYER,
  TRM_CLIENT_CA_4_LAYER,
  TRM_CLIENT_CA1_4_LAYER,
  TRM_CLIENT_CA2_4_LAYER,
  TRM_CLIENT_CA3_4_LAYER,
  TRM_CLIENT_PRIMARY_TX,
  TRM_CLIENT_MAX
} trm_client_type;


/*----------------------------------------------------------------------------
  Transceiver resource request times (when?)
    When a client needs a resource is composed of two parts:
    1) A start time, when the resource is needed (could be ASAP).
    2) The duration the resource is needed for.
----------------------------------------------------------------------------*/

/* Timestamp type used by TRM (sclks)  See: timetick_get() */
typedef timetick_type             trm_time_t;

/*----------------------------------------------------------------------------
  Define Tag type to be uint32
----------------------------------------------------------------------------*/
typedef uint32                    trm_tag_type;

/* Time duration type used by TRM (sclks) */
typedef timetick_type             trm_duration_t;

/* 64 bit timestamp for increased range*/
typedef uint64                    trm_timestamp_t;

/*----------------------------------------------------------------------------
  Transceiver resource request reasons (why?)
    Is used, with the requesting client, to determine the priority of the
    request.

    Note: Always add to the end in order to preserve backward compatibility
          with the TRM log parsers.

----------------------------------------------------------------------------*/

typedef enum
{
  /* Perform general access attempt */
   TRM_ACCESS = 0,

  /* Perform access as soon as possible. TRM asks retain lock holders to
     release the lock immediately. */
   TRM_ACCESS_URGENT = 1,

   /* Determining the mobile location */
   TRM_LOCATION_FIX = 2,

   /* Deprecated reason */
   TRM_DEMOD_GPS = TRM_LOCATION_FIX,

  /* In a traffic call */
   TRM_TRAFFIC = 3,

  /* Monitoring paging channel */
   TRM_DEMOD_PAGE = 4,

  /* Acquiring or reacquiring the system (DR Reason) */
   TRM_ACQUISITION = 5,

  /* Diversity */
   TRM_DIVERSITY = 6,

  /* Monitoring paging channel with a small SCI */
   TRM_SMALL_SCI_PAGE = 7,

  /**/
   TRM_CHANNEL_MAINTENANCE = 8,

  /* For priority inversion  */
   TRM_ACQUISITION_INV = 9,

  /* For Background Traffic */
   TRM_BG_TRAFFIC = 10,

  /* Small SCI Page Continuation */
   TRM_SMALL_SCI_PAGE_CONTINUATION = 11,

  /* For IRAT measurements */
   TRM_IRAT_MEASUREMENT = 12,

  /* Sleep */
   TRM_SLEEP = 13,

  /* inverted priority for channel maintenance */
   TRM_CHANNEL_MAINTENANCE_INV = 14,

  /* Inter-RAT measurment with diversity */
   TRM_DIVERSITY_IRAT_MEASUREMENT = 15,

   TRM_SLEEP_DIVERSITY = 16,

  /* Low priority EMBMS */
   TRM_EMBMS = 17,

  /* Inverted EMBMS priority */
   TRM_EMBMS_INV = 18,

   TRM_EMBMS_LOW = 19,

  /* For LTE Single chain exit */
   TRM_DR_TRAFFIC = 20,

  /* Acquiring or reacquiring the system (SR Reason)  */
   TRM_ACQUISITION_CELL_SELECT = 21,

  /* For priority inversion  */
   TRM_ACQUISITION_CELL_SELECT_INV = 22,

  /* CTCH (BroadCast MultiCast) */
   TRM_CTCH = 23,

  /* Inverted CTCH (BroadCast MultiCast) priority */
   TRM_CTCH_INV = 24,

   /* Used to prevent single chain exit in LTE */
   TRM_HIGH_PRIORITY_IDLE = 25,

   /*Used for special priority mapping*/
   TRM_CHANNEL_MAINTENANCE_HIGH = 26,

  /*Used for CM_HIGH to ACCESS_LOW switching*/
   TRM_ACCESS_LOW = 27,

  /* For internal bound-checking and array-sizing */
   TRM_REASON_MAX = 28,
  
  /*default value*/
  TRM_NO_REASON = 0xFF,

  /* for backward compatibility as techs use NUM_REASON enum */
  TRM_NUM_REASONS = TRM_NO_REASON

}
trm_reason_enum_t;


/*----------------------------------------------------------------------------
  Transceiver resource request sub-reasons (more specific than trm_reason_enum_t)
    With TRM Unification reasons are more general so sub-reason is used to specify
    the activity being performed for logging purposes.

    Note: Always add to the end in order to preserve backward compatibility
          with the TRM log parsers.

----------------------------------------------------------------------------*/

typedef enum
{
  /* In a circuit switched voice call */
  TRM_SUBREASON_CS_CALL,

  /* In a packet switched data call */
  TRM_SUBREASON_PS_CALL,

  /* In a Voice over LTE call */
  TRM_SUBREASON_VOLTE,

  /*  */
  TRM_SUBREASON_EMBMS,

  /*  */
  TRM_SUBREASON_SRB,

  /* Perform Intra-F cell reslection */
  TRM_SUBREASON_INTRA_F_RESELECT,

  /* Perform Inter-F cell reslection  */
  TRM_SUBREASON_INTER_F_RESELECT,

  /* Perform Inter-RAT reselection  */
  TRM_SUBREASON_IRAT_RESELECT,

  /* Perform Intra-F measurement */
  TRM_SUBREASON_INTRA_F_MEASUREMENT,

  /* Perform Inter-F measurement */
  TRM_SUBREASON_INTER_F_MEASUREMENT,

  /* Perform Inter-RAT measurement */
  TRM_SUBREASON_IRAT_MEASUREMENT,

  /* Perform System Info Reading */
  TRM_SUBREASON_SYSINFO_READ,

  /* Perform CMAS/PWS/ETWS Emergency alert */
  TRM_SUBREASON_EMERGENCY_ALERT,

  /* Perform Neighbor Cell Monitoring  */
  TRM_SUBREASON_NEIGHBOR_CELL_MONITOR,

  /* Demod a page */
  TRM_SUBREASON_PAGE,

  /* Demod page extended*/
  TRM_SUBREASON_PAGE_EXTENDED,

  /* Perform Location area update*/
  TRM_SUBREASON_LAU,

  /* Perform Routing area update */
  TRM_SUBREASON_RAU,

  /*   */
  TRM_SUBREASON_TU,

  /* */
  TRM_SUBREASON_CS_NAS_SIGNALLING,

  /*  */
  TRM_SUBREASON_PS_NAS_SIGNALLING,

  /*  */
  TRM_SUBREASON_REDIRECTION,

  /* Perform Cell change order  */
  TRM_SUBREASON_CCO,    

  /*  */
  TRM_SUBREASON_PS_HANDOVER,

  /* Single Radio Voice Call Continuity */
  TRM_SUBREASON_SRVCC,

  /*  */
  TRM_SUBREASON_CS_FALLBACK,

  /*  */
  TRM_SUBREASON_ACQUISITION,

  /*  */
  TRM_SUBREASON_OOS_SCAN,

  /* Perform Better PLMN search */
  TRM_SUBREASON_BPLMN_SEARCH,

  /* Perform Manual PLMN */
  TRM_SUBREASON_MANUAL_PLMN,

  /* Perform closed subscriber group search */
  TRM_SUBREASON_CSG_SEARCH,

  /* Perform Radio Link Failure recovery */
  TRM_SUBREASON_RLF_RECOVERY,

  /*  */
  TRM_SUBREASON_PANIC_SEARCH,

  /* Diversity Activities */
  TRM_SUBREASON_DIVERSITY,

  /* SCELL Activities */
  TRM_SUBREASON_SCELL,  

  /* For internal bound-checking and array-sizing */
  TRM_SUBREASON_MAX,
  
  /*default value*/
  TRM_SUBREASON_NONE = 0xFF,

  /* */
  TRM_NUM_SUBREASONS = TRM_SUBREASON_NONE

}
trm_sub_reason_enum_t;

/*----------------------------------------------------------------------------
  Request results (what happened?)
    Returned by trm_request or passed as an argument to the
    trm_request_and_notify callback.

    Note: Always add to the end in order to preserve backward compatibility
          with the TRM log parsers.

----------------------------------------------------------------------------*/

typedef enum
{
  TRM_DENIED,

  TRM_GRANTED_CHAIN0,

  TRM_GRANTED_CHAIN1,

  TRM_GRANTED_CHAIN2,

  TRM_GRANTED_CHAIN3,

  TRM_GRANTED_CHAIN4,

  TRM_GRANTED_CHAIN5,

  TRM_GRANTED_CHAIN6,

  TRM_GRANTED_CHAIN7,

  TRM_GRANTED_CHAIN8,
  
  TRM_GRANTED_CHAIN9,

  TRM_GRANTED_CHAIN_GPS,

  TRM_GRANTED_CHAIN_WLAN  
}
trm_grant_event_enum_t;



/*----------------------------------------------------------------------------
  Unlock request events
    A higher priority client needs the resource you hold ...

    Note: Always add to the end in order to preserve backward compatibility
          with the TRM log parsers.

----------------------------------------------------------------------------*/

typedef enum
{
  /* Unlock request cancelled.  You no-longer need to unlock. */
  TRM_UNLOCK_CANCELLED,

  /* ... at the indicated time.  You can hold the lock until then. */
  TRM_UNLOCK_BY,

  /* ... as soon as possible.  Release the lock when convenient, but soon. */
  TRM_UNLOCK_REQUIRED,

  /* ... immediately for a page response!  Give it up ASAP! */
  TRM_UNLOCK_IMMEDIATELY,

  /*. . . as soon as possible for a pending band retune or handoff */
  TRM_UNLOCK_BAND_INCOMPATIBLE
}
trm_unlock_event_enum_t;



/*----------------------------------------------------------------------------
  RF device corresponding to RF chain owned by TRM lock
----------------------------------------------------------------------------*/

/* Identifer representing "no RF chain is owned" */
#define TRM_NO_DEVICE             RFM_MAX_DEVICES



/*----------------------------------------------------------------------------
  RF configurations

    Note: Always add to the end in order to preserve backward compatibility
          with the TRM log parsers.

----------------------------------------------------------------------------*/

typedef enum
{
  /* Single RF chain */
  TRM_RF_SINGLE,

  /* Dual RF chain - both must tune to the same frequency */
  TRM_RF_DIVERSITY,

  /* Dual RF chain - independently tunable - unequal sensitivity */
  TRM_RF_UNEQUAL_SENSITIVITY,

  /* Dual RF chain - independently tunable - equal sensitivity */
  TRM_RF_EQUAL_SENSITIVITY,

  /* Two Tx chains with an exclusive chain for 1x */
  TRM_RF_DUAL_TX
}
trm_rf_enum_t;



/*----------------------------------------------------------------------------
  TRM Modes which can be enabled/disabled
----------------------------------------------------------------------------*/

typedef enum
{
  /* Simultaneous 1x idle with HDR traffic mode */
  TRM_MODE_SIMUL_1XIDLE_HDRTC
}
trm_mode_enum_t;

/*----------------------------------------------------------------------------
  Set Input Types
----------------------------------------------------------------------------*/
typedef enum
{
  /* IRAT State */
  TRM_IRAT_STATE,

  /* IFS State */
  TRM_IFS_STATE,

  TRM_QTA,

  TRM_APS,

  /* Whether DR-DSDS is supported or not. By default it is supported */
  TRM_DUAL_RECEIVE,

  /* INTERNAL: This is internal to TRM */
  TRM_HOP_STATE,

  TRM_LTED_STATE,

  /* For updating Extended LTE Signaling feature registration or state information */
  TRM_ELS_STATE, 
  
  /* To vote/devote for various LTE Modes needed for APS handling */
  TRM_TECH_MODE_STATE,

  /* Enum Max */
  TRM_MAX_CLIENT_STATE

} trm_client_state_enum_type;


/*----------------------------------------------------------------------------
  Set State Operation
----------------------------------------------------------------------------*/
typedef enum
{
  /* End of the state */
  TRM_END_STATE,

  /* Start of the state */
  TRM_START_STATE,

  /* Enum Max */
  TRM_MAX_STATE_OPERATION

} trm_state_oper_enum_type;

/*----------------------------------------------------------------------------
TRM SYS_MODE_SUB ID STURCTURE
----------------------------------------------------------------------------*/
typedef struct
{
  sys_sys_mode_e_type      rat_group;
  
  sys_modem_as_id_e_type   as_id;
 
}trm_sys_mode_sub_id_struct_type;

/*----------------------------------------------------------------------------
TRM QSH DATA
----------------------------------------------------------------------------*/
typedef struct
{
  boolean                is_reliable;

  uint32                  modes_enabled;

  uint32                 rf_config;

  /*Indicate which rat is active on specific chain*/
  trm_sys_mode_sub_id_struct_type chain_owner[TRM_MAX_CHAINS];

  /*If entry  is SYS_SYS_MODE_MAX that RAT is not active*/
  trm_sys_mode_sub_id_struct_type order_list[SYS_SYS_MODE_MAX];
	
}trm_qsh_data_struct_type;


/*===========================================================================
FUNCTION trm_qsh_data_for_crash
  
DESCRIPTION
  This function collects Active RAT info while UE crash


DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
void trm_qsh_data_for_crash(trm_qsh_data_struct_type *trm_qsh_data_struct);

/*----------------------------------------------------------------------------
TRM RAT Group
----------------------------------------------------------------------------*/
typedef enum
{
  TRM_RAT_GROUP_1X,

  TRM_RAT_GROUP_HDR,

  TRM_RAT_GROUP_GERAN_SUB1,

  TRM_RAT_GROUP_GERAN_SUB2,

  TRM_RAT_GROUP_UMTS_SUB1,

  TRM_RAT_GROUP_UMTS_SUB2,

  TRM_RAT_GROUP_TDSCDMA,

  TRM_RAT_GROUP_LTE,

   TRM_RAT_GROUP_LTE_SUB1 = TRM_RAT_GROUP_LTE,
     
   TRM_RAT_GROUP_LTE_SUB2,
   
   TRM_RAT_GROUP_WLAN,

  TRM_RAT_GROUP_NONE,

  TRM_RAT_GROUP_MAX = TRM_RAT_GROUP_NONE,

  TRM_RAT_GROUP_ABSENT,

} trm_rat_group_type;

typedef enum
{
   /* To register that UE is ELS capable */
   TRM_ELS_REGISTRATION,

   /* To activate ELS tracking and callbacks */
   TRM_ELS_ACTIVATION,

   /* For idle tech to update its ELS state to send to DDS */
   TRM_ELS_STATE_UPDATE,

   /* Enum Max */
   TRM_MAX_ELS_OPERATION

}trm_els_request_enum_type;

/* Macros used for TRM operating mode */
#define TRM_SHDR_IS_ENABLED  (0x1)
#define TRM_SVDO_IS_ENABLED  (0x2)
#define TRM_SVLTE_IS_ENABLED (0x4)
#define TRM_DSDA_IS_ENABLED (0x10)
#define TRM_SGLTE_SGTDS_IS_ENABLED (0x20)
#define TRM_1X_SLTE_IS_ENABLED (0x40)
#define TRM_GSM_SLTE_IS_ENABLED (0x80)
#define TRM_DR_DSDS_IS_ENABLED  (0x100)
#define TRM_TX_SHARING_IS_ENABLED (0x200) 
#define TRM_DR_W_AND_L_IS_ENABLED (0X400)

/* Below macros are used by TRM and its clients for tune away */
/* Extension flag bitmaps and masks */
#define TRM_EXTENSION_FLAG_DEFAULT (0x00)

/* these masks should correspond to single bit positions in an 8-bit map */
#define TRM_PRIORITY_INV_ENABLED   (0x01)
#define TRM_QUICK_TA_SUPPORTED     (0x02)
#define TRM_ALTERNATE_PAGE_ENABLED (0x04)
#define TRM_LTA_SUPPORTED          (0x08)

/* Number of subscriptions that are possible & supported */
#define MAX_NUM_OF_SUBSCRIPTIONS       MAX_AS_IDS

/* rfm device capability */
#define TRM_RF_DEVICE_RX_CAPABLE      RFM_DEVICE_RX_SUPPORTED
#define TRM_RF_DEVICE_TX_CAPABLE      RFM_DEVICE_TX_SUPPORTED
#define TRM_RF_DEVICE_DR_CAPABLE      0x04

/*----------------------------------------------------------------------------
  Enum indicating Type of TuneAway for any TRM transaction
----------------------------------------------------------------------------*/
typedef enum
{
  /* No Tune Away preference */
  TRM_TUNEAWAY_NONE,

  /* QTA Indication */
  TRM_QUICK_TUNEAWAY,

  /* LTA Indication */
  TRM_LONG_TUNEAWAY,

  /* QCTA Indication - will be used by TRM-CXM interface */
  TRM_QUICK_CA_TUNEAWAY,

  /* QDTA Indication - will be used by TRM-CXM interface */
  TRM_QUICK_DIV_TUNEAWAY,

  /* FallBack Indication - will be used by TRM-CXM interface */
  TRM_TUNEAWAY_FALLBACK,

  /* Enum Max */
  TRM_TUNEAWAY_MAX,

} trm_tuneaway_enum_type;

/*----------------------------------------------------------------------------
  Enum indicating Type of Restriction causing Unlock Callback
----------------------------------------------------------------------------*/
typedef enum
{
  /* No Restriction (Default state) */
  TRM_RESTRICTION_NONE = -1,

  /* RF (Base Band) Resctriction */
  TRM_RF_RESTRICTION,

  /* FW (CCS Memory or VPE and Demback) Resctriction */
  TRM_FW_RESTRICTION,

  /* Enum Max */
  TRM_RESTRICTION_MAX,

} trm_restriction_enum_type;

/*----------------------------------------------------------------------------
  TRM 1X Modes
----------------------------------------------------------------------------*/

typedef enum
{
  /* Standard 1X operation */
  TRM_1X_MODE_NORMAL,

  /* Require 1x to operate on primary antenna in SOODA operation mode  */
  TRM_1X_MODE_FORCE_SOODA,

  /* 1X should only run on the secondary chain */
  TRM_1X_MODE_SECONDARY_ONLY,

  /* Undefined operation */
  TRM_1X_MODE_UNDEFINED
}
trm_1x_mode_enum_t;

/*  Band Request Grant Type  */
typedef enum
{

  /* Band tune or cancel request is denied */
  TRM_BAND_REQUEST_DENIED,

  /* Band tune request is pending */
  TRM_BAND_REQUEST_PENDING,

  /* Band tune request is granted */
  TRM_BAND_REQUEST_GRANTED

} trm_band_request_grant_type;


/*----------------------------------------------------------------------------
  Result of a change request e.g request to change reason for holding chain
----------------------------------------------------------------------------*/

typedef enum
{
  /* Change request is denied */
  TRM_CHANGE_DENIED,

  /* Change request is accepted. This can still result in an unlock callback
     to the requestor */
  TRM_CHANGE_ACCEPTED,

  /* Change request is pending. expect a callback... */
  TRM_CHANGE_PENDING

} trm_change_result_enum_t;

/*----------------------------------------------------------------------------
  SR Mode Tech Index
----------------------------------------------------------------------------*/
typedef enum
{
  TRM_SR_TECH_1X = 0,
  TRM_SR_TECH_HDR = 1,
  TRM_SR_TECH_GSM = 2,
  TRM_SR_TECH_LTE = 3,
  TRM_SR_TECH_WCDMA = 4,
  TRM_SR_TECH_TDSCDMA = 5,
  /* 
  Reserved  = 6,

  Reserved  = 7,
  */
  TRM_MAX_SR_TECH = 8, 
}trm_sr_tech_enum_t;
/*----------------------------------------------------------------------------
  Operating mode enum type
----------------------------------------------------------------------------*/
typedef enum
{
  /* Hybrid fallback mode */
  TRM_OPERATING_MODE_FALLBACK,

  /* Full concurrency mode */
  TRM_OPERATING_MODE_FULL_CONCURRENCY,

  /* Diversity sharing mode */
  TRM_OPERATING_MODE_DIV_SHARING,

  /* operating mode limit */
  TRM_OPERATING_MODE_MAX

}trm_operating_modes_enum_type;

/*----------------------------------------------------------------------------
  Fallback reason enum type
----------------------------------------------------------------------------*/
typedef enum
{
  /* No Fallback */
  TRM_FALLBACK_TYPE_NONE,

  /* Fallback triggered by a client*/
  TRM_FALLBACK_TYPE_CLIENT_VOTE,

  /* Fallback due to non-DR activity */
  TRM_FALLBACK_TYPE_NON_DR_ACTIVITY,

  TRM_FALLBACK_TYPE_MAX = 0xFFFF, 

}trm_fallback_mode_enum_type;

/*----------------------------------------------------------------------------
  TRM G Client Priority
----------------------------------------------------------------------------*/
typedef enum
{
  /* Low Priority */
  TRM_PRIO_LOW,

  /* High Priority */
  TRM_PRIO_HIGH,

  /* Undefined Priority */
  TRM_PRIO_UNDEFINE
}
trm_client_priority_enum_t;

/*----------------------------------------------------------------------------
  TRM Performance Metrics
----------------------------------------------------------------------------*/
typedef enum
{

  /* Total page misses due to eMBMS */
  TRM_METRIC_PAGE_MISS_EMBMS_AGGR,

  /* Total page misses */
  TRM_METRIC_TOTAL_PAGE_MISS,

  /* Number of eMBMS denials due to paging */
  TRM_METRIC_EMBMS_DENIAL_PAGING,

  /* Total number of eMBMS denials */
  TRM_METRIC_EMBMS_DENIAL_TOTAL,
  
  /* Total number of paging requests */
  TRM_METRIC_TOTAL_PAGE_REQ,

  TRM_METRIC_MAX

} trm_metrics_enum_t;

/* Size of band-chain mapping information */
#define TRM_DEVICE_MAPPING_INPUT_SIZE    10

/*-------------------------------------------------------------------------------- 
  Bitmask of currently active and suspended protocol stacks 
  --------------------------------------------------------------------------------*/
typedef struct
{
  uint32 active_tech_mask;  /* bitmask with each bit representing a tech which is active.*/
  uint32 suspend_tech_mask; /* bitmask with each bit representing a tech which is suspended */
}trm_protocol_stack_mask_type;

/*----------------------------------------------------------------------------
  Payload for trm_request_and_notify() / (*trm_grant_callback_t)() pair
----------------------------------------------------------------------------*/

typedef uint32                    trm_request_tag_t;

/*----------------------------------------------------------------------------
  Band type used to communicate with tech and RF.
----------------------------------------------------------------------------*/

typedef int32                    trm_band_type;

/*----------------------------------------------------------------------------
  Band type
----------------------------------------------------------------------------*/
typedef  sys_band_class_e_type trm_band_t;

/*----------------------------------------------------------------------------
  Channel Type
----------------------------------------------------------------------------*/
typedef  uint32 trm_channel_t;

/*----------------------------------------------------------------------------
  Frequency Type
----------------------------------------------------------------------------*/
typedef struct
{
  trm_band_t band;
  uint32     num_channels;
  trm_channel_t* channels;
} trm_frequency_type_t;


/*----------------------------------------------------------------------------
  Grant Return Type
----------------------------------------------------------------------------*/
typedef struct
{
  trm_grant_event_enum_t grant;
} trm_chain_grant_return_type;

typedef struct
{
  trm_band_request_grant_type grant;
} trm_band_grant_return_type;

/*----------------------------------------------------------------------------
  Structure for Multimode AS-ID mapping with tech clients 
----------------------------------------------------------------------------*/

typedef struct
{
  sys_modem_as_id_e_type multimode_as_id;
  trm_client_enum_t      gsm_id_list[MAX_NUM_OF_SUBSCRIPTIONS];
} trm_tech_map_table_t;

/*----------------------------------------------------------------------------
  Device Sharing Info
----------------------------------------------------------------------------*/
typedef struct
{

  /* client ID with which device sharing needs to happen
     if no sharing is required use TRM_NO_CLIENT 
     if sharing is required use the client ID with which sharing is needed  */
  trm_client_enum_t     client_id;

}trm_device_sharing_info;

/*----------------------------------------------------------------------------
  Structure to hold the current priority of the TRM GSM clients.
----------------------------------------------------------------------------*/
typedef struct trm_client_priority_struct
{
  /* Client Id */
  trm_client_enum_t           client_id;

  /* Priority of the client */
  trm_client_priority_enum_t  client_prio;

  /* Subscription capability tech mapping as per multimode subs */
  trm_tech_map_table_t         tech_asid_map;
} trm_client_priority_type;

/*----------------------------------------------------------------------------
  Structure to capture client data
----------------------------------------------------------------------------*/
typedef struct
{
  /* Client Id */
  trm_client_enum_t       client_id;

  /* Reason for which the client is requesting for chain */
  trm_reason_enum_t       reason;

  /*Duration for which client wants to hold the chain*/
  trm_duration_t          duration;
} trm_client_info_t;



/*----------------------------------------------------------------------------
  Struct for DRX Update.
----------------------------------------------------------------------------*/
typedef struct
{
  trm_client_enum_t client; /* client setting drx input */
  uint32            drx_cycle; /* current drx cycle in milli-seconds */
  uint32            default_drx_cycle; /* default drx cycle in milli-seconds */

} trm_drx_cycle_input_type;




/*----------------------------------------------------------------------------
  Struct for PBR Mode Registration.
----------------------------------------------------------------------------*/
typedef struct
{
  trm_client_enum_t client;    /* client setting Page Block Rate Modes */
  uint64            pbr_modes; /* Modes for which PBR modes are set */

} trm_pbr_modes_input_type;

/*----------------------------------------------------------------------------
  Denotes the result of an ASYNC event.
----------------------------------------------------------------------------*/
typedef enum
{
  TRM_ASYNC_EVENT_RESPONSE_SUCCESS,

  TRM_ASYNC_EVENT_RESPONSE_FAIL_NOT_READY,

  TRM_ASYNC_EVENT_RESPONSE_FAIL_NO_RETRY
} trm_async_event_response_result_enum_type;


/*----------------------------------------------------------------------------
  Struct to represent RF Handler.
----------------------------------------------------------------------------*/
typedef rfm_path_type trm_rf_handler_type;


/*----------------------------------------------------------------------------
  Data for the Device hopping request event.
----------------------------------------------------------------------------*/
typedef struct
{
  /* The grant enum type for the device to hop to*/
  trm_grant_event_enum_t    grant;

  /* RF device for the device to hop to */
  rfm_device_enum_type      rf_device;

  trm_rf_handler_type       rf_handle;

} trm_async_event_device_hop_req_struct_type;

/*----------------------------------------------------------------------------
  Data for Unlock Notify Async Callback
----------------------------------------------------------------------------*/
typedef struct 
{
  /* The event being sent to the client */
  trm_unlock_event_enum_t unlock_event;

  /* Sclk timestamp for TRM_UNLOCK_BY */
  uint32                  unlock_by_sclk;

  /* Information about the winning client.
     Higher priority client who's request is 
     causing the unlock notification */
  trm_client_info_t       winning_client;

  /* Unlock tag provided in the retain_lock
     or request and notify enhanced */
  trm_tag_type            unlock_tag;
} trm_async_event_unlock_notify_data_type;

/*----------------------------------------------------------------------------
  Data for Grant notify callback
----------------------------------------------------------------------------*/
typedef struct 
{
  /* The event being sent to the client */
  trm_chain_grant_return_type     grant;

  /* Anonymous payload echoed from trm_request_and_notify() */
  trm_request_tag_t               tag;
}trm_async_event_grant_notify_data_type;

/*----------------------------------------------------------------------------
  Data for Grant notify enhanced callback
----------------------------------------------------------------------------*/
typedef struct 
{
  /* The event being sent to the client */
  trm_chain_grant_return_type     grant;

  /* Anonymous payload echoed from trm_request_and_notify() */
  trm_request_tag_t               tag;

  /* Duration by which the lock is extended */
  trm_duration_t                  duration;
}trm_async_event_grant_notify_enhanced_data_type;

/*----------------------------------------------------------------------------
  Union which contains the data for different async events.
----------------------------------------------------------------------------*/
typedef union
{
  trm_async_event_device_hop_req_struct_type      dev_hop_req;
  trm_async_event_unlock_notify_data_type         unlock_notify;
  trm_async_event_grant_notify_data_type          grant_notify;
  trm_async_event_grant_notify_enhanced_data_type grant_notify_enhanced;
} trm_async_event_union_type;

/*----------------------------------------------------------------------------
  Data for the Device hopping request event.
----------------------------------------------------------------------------*/
typedef struct
{
  trm_async_event_response_result_enum_type result;
} trm_async_event_response_device_hop_req_struct_type;

/*----------------------------------------------------------------------------
  Union which contains the data for different async responses.
----------------------------------------------------------------------------*/
typedef struct
{
  trm_async_event_response_device_hop_req_struct_type dev_hop_req;
} trm_async_event_response_union_type;

/*----------------------------------------------------------------------------
  Enum to indicate type of async call-back
----------------------------------------------------------------------------*/
typedef enum
{
  TRM_CONNECTED_MODE_HOP_ASYNC_INFO,

  TRM_REQUEST_AND_NOTIFY_ASYNC_INFO,

  TRM_REQUEST_AND_NOTIFY_ENH_ASYNC_INFO,

  TRM_MODIFY_CHAIN_STATE_ASYNC_INFO,

  TRM_UNLOCK_CHAIN_ASYNC_INFO,

  TRM_DUAL_RECEIVE_INFO,

  /* async callback to WLAN for IRAT event release info */
  TRM_IRAT_EVENT_ASYNC_INFO,

  /* async callback to indicate QTA Start/End Info */
  TRM_QTA_ASYNC_INFO,

  /* async callback to notify data tech of ELS state changes and
		 idle techs of ELS on/off */
  TRM_ELS_STATE_ASYNC_INFO,

  /* async event to notify tech about idle collision events */
  TRM_IDLE_COLL_ASYNC_INFO,

  /* async event to notify active and suspended protocol stacks on all subs*/
  TRM_PROTOCOL_STACK_STATUS_ASYNC_INFO,

  TRM_ASYNC_EVENT_MAX,

}trm_async_callback_type_enum_t;

/*----------------------------------------------------------------------------
  Data type passed as input to the ASYNC event callback
----------------------------------------------------------------------------*/
typedef struct
{
  trm_client_enum_t               client_id;

  trm_async_callback_type_enum_t  event;

  trm_async_event_union_type      data;

} trm_async_event_cb_data_type;

/* rxtx split */
/*----------------------------------------------------------------------------
  Data type passed as input to the ASYNC event callback
----------------------------------------------------------------------------*/
typedef struct
{
  /* number of valid entries */
  uint32                           num_of_entries;

  /* list of grouped async data */
  trm_async_event_cb_data_type     grouped_data[TRM_GROUPED_CLIENTS_MAX];

} trm_grouped_async_event_cb_data_type;

/*----------------------------------------------------------------------------
  Data type passed as input to the ASYNC response API
----------------------------------------------------------------------------*/
typedef struct
{
  trm_client_enum_t                   client_id;

  trm_async_callback_type_enum_t      event;

  trm_async_event_response_union_type data;

} trm_async_event_response_data_type;

/* rxtx split */
/*----------------------------------------------------------------------------
  Data type passed as input to the ASYNC response API
----------------------------------------------------------------------------*/
typedef struct
{
  /* number of valid entries */
  uint32                                num_of_entries;

  /* list of response info */
  trm_async_event_response_data_type    resp_data[TRM_GROUPED_CLIENTS_MAX];

} trm_grouped_async_event_response_data_type;

/*----------------------------------------------------------------------------
  Data to denote the HW capability
----------------------------------------------------------------------------*/
typedef struct 
{
  /*  This is an array of bitmasks to be indexed using sys_sys_mode_e_type. It denotes the 
      technologies on a multimode subscription that are compatible with each other. That is,
      they can hold different chains simultaneously.

      Each bit in the bitmask corresponds to a tech the index tech is simultaneously
      compatible with. 

      Also, the values of the bitmasks should show symmetry. That is, if 1x is compatible
      with LTE, LTE should be made compatible with 1x.
      
      For Example, to enable SVLTE and SGLTE: 
      simult_cap[SYS_SYS_MODE_LTE] =  SP_CONV_ENUM_TO_BIT_MASK(SYS_SYS_MODE_HDR)  | 
                                              SP_CONV_ENUM_TO_BIT_MASK(SYS_SYS_MODE_CDMA) | 
                                              SP_CONV_ENUM_TO_BIT_MASK(SYS_SYS_MODE_GSM); 
      simult_cap[SYS_SYS_MODE_CDMA] = SP_CONV_ENUM_TO_BIT_MASK(SYS_SYS_MODE_LTE);
      simult_cap[SYS_SYS_MODE_HDR]  = SP_CONV_ENUM_TO_BIT_MASK(SYS_SYS_MODE_LTE);
      simult_cap[SYS_SYS_MODE_GSM]  = SP_CONV_ENUM_TO_BIT_MASK(SYS_SYS_MODE_LTE); */
  uint32 simult_cap[SYS_SYS_MODE_MAX];

  /* Denotes the maximum number of concurrently active subscriptions supported. 
     It is also the number of subscriptions that can transmit at the same time
     For example: For SSSS, max_concurrent_active_subscriptions = 1,
                  For DSDS, max_concurrent_active_subscriptions = 1,
                  For DSDA, max_concurrent_active_subscriptions = 2,
                  For TSTS, max_concurrent_active_subscriptions = 1 and so on. */
  uint32 max_concurrent_active_subscriptions;
}trm_hw_cap_info;


/* Chain owner and request information callback for clients interested in
   state update for particular client */
typedef struct
{
  boolean   generate_rnum;
} trm_state_info_type;

/*============================================================================
CALLBACK TRM_TECH_STATE_UPDATE_CALLBACK_T

DESCRIPTION
  The prototype for callback functions, used by the Transceiver manager
  to inform the clients of client's state for each tech.
  
DEPENDENCIES
  The callback will be called by the Transceiver Manager.  It may be called
  from a task context of another client, or from interrupt context.

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
typedef void (* trm_tech_state_update_callback_type)
(
trm_state_info_type trm_state
);

/*----------------------------------------------------------------------------
  Antenna Switch Configuration Enum Type.
----------------------------------------------------------------------------*/
typedef enum
{

  /* PRx --> Ant0; DRx --> Ant1 */
  TRM_ANT_SWITCH_DIV_CONFIG_0,

  /* PRx --> Ant1; DRx --> Ant0 */
  TRM_ANT_SWITCH_DIV_CONFIG_1,

  /* Default configuration */
  TRM_ANT_SWITCH_DIV_CONFIG_DEFAULT,

  /* Current Settings */
  TRM_ANT_SWITCH_DIV_CONFIG_CURRENT,

  /* Preferred Config*/
  TRM_ANT_SWITCH_DIV_CONFIG_PREFERRED,

  /* The switch is in transition */
  TRM_ANT_SWITCH_DIV_CONFIG_IN_TRANSITION,

  /* Invalid configuration */
  TRM_ANT_SWITCH_DIV_CONFIG_INVALID = TRM_ANT_SWITCH_DIV_CONFIG_IN_TRANSITION,

  /* Maximum configuration, can be cleaned */
  TRM_ANT_SWITCH_DIV_CONFIG_MAX,

} trm_ant_switch_div_config_type;


/*----------------------------------------------------------------------------
  Antenna Switch Configuration Input Enum Type.
----------------------------------------------------------------------------*/
typedef enum
{
  /* return the default antenna configuration */
  TRM_ANT_SWITCH_CONFIG_DEFAULT,

  /* return the current antenna configuration */
  TRM_ANT_SWITCH_CONFIG_CURRENT,

  TRM_ANT_SWITCH_CONFIG_MAX,
} trm_ant_switch_get_input_type;



typedef uint64 trm_ant_switch_div_dwelling_time_type;

/*----------------------------------------------------------------------------
  Feature Override Enum Type. This is used to indicate if the feature On/Off
  settings in NV has been explicitly over ridden or not through QMI
----------------------------------------------------------------------------*/
typedef enum
{
  /* Feature Override is On */
  TRM_ANT_SWITCH_FEATURE_OVERRIDE_ON,

  /* Feature Override is Off */
  TRM_ANT_SWITCH_FEATURE_OVERRIDE_OFF,

  /* Feature Override is Unset */
  TRM_ANT_SWITCH_FEATURE_OVERRIDE_UNSET,
} trm_ant_switch_feature_override_type;

/*----------------------------------------------------------------------------
  Antenna Switch Div Test Mode Ctrl Type.
----------------------------------------------------------------------------*/
typedef enum
{
  TYPICAL,
  ALTERNATING_ON_OFF,
  ALTERNATING_OFF_FORCED_SWITCH,
  ALTERNATING_ON_OFF_FORCED_SWITCH,
  ANT_SWITCH_DIV_TEST_MODE_CTRL_MAX,
} trm_ant_switch_div_test_mode_ctrl_type;


/*----------------------------------------------------------------------------
  Antenna Switch Client Mode Types, In Ascending order of priority
----------------------------------------------------------------------------*/
typedef enum
{

  /* INACTIVE: Default/OOS */
  TRM_CLIENT_MODE_INACTIVE,

  /* INTERNAL */
  TRM_CLIENT_MODE_RXTX_LOW,

  /* RXACQ: acquisition mode */
  TRM_CLIENT_MODE_RXACQ,

  /* SLEEP: IDLE sleep */
  TRM_CLIENT_MODE_SLEEP,

  /* IDLE WAKEUP: Idle wakeup */
  TRM_CLIENT_MODE_IDLE_WAKEUP,

  /* client is entering Transmit mode */
  TRM_CLIENT_MODE_RXTX,


  /*! following modes are decprecated */

  /* client is entering receive mode */
  TRM_CLIENT_MODE_RX_ONLY,

  /* client is entering receive mode */
  TRM_CLIENT_MODE_DEF_ENFORCED_RX,

  TRM_CLIENT_MODE_LOCK_SWITCH,

  TRM_CLIENT_MODE_LISTEN,

  TRM_CLIENT_MODE_MAX

} trm_client_mode_type;



typedef enum
{
  /* sv keep, idle default */
  TRM_ANT_SWITCH_SV_KEEP_IDLE_DEF,
  /* default in SV and idle mode */
  TRM_ANT_SWITCH_SV_DEF_IDLE_DEF,
  /* keep in SV and idle mode */
  TRM_ANT_SWITCH_SV_KEEP_IDLE_KEEP,
  /* default in SV and keep in idle */
  TRM_ANT_SWITCH_SV_DEF_IDLE_KEEP

} trm_ant_switch_div_sv_idle_behavior;

/*----------------------------------------------------------------------------
  Non-tech Clients interested in idle collision event
----------------------------------------------------------------------------*/
typedef enum 
{
  TRM_IDLE_COLL_CLIENT_NAS,

  TRM_IDLE_COLL_CLIENT_MAX 
}trm_idle_coll_ind_client_e;

/*----------------------------------------------------------------------------
  Antenna Switch Configuration Data Type. This data is initialized in NV.
----------------------------------------------------------------------------*/
typedef PACK(struct) 
{

  /* default/current configuration */
  trm_ant_switch_div_config_type  configuration;

  /* test mode control */
  trm_ant_switch_div_test_mode_ctrl_type test_mode_ctrl;

  /* dwelling time */
  trm_ant_switch_div_dwelling_time_type dwelling_time;

  /* default configuration when in SV transmit mode */
  trm_ant_switch_div_config_type      sv_default_configuration;

  /* ant switch diversity behavior
     0: SV KEEP, Idle Default
     1: SV Default, Idle Default
     2: SV KEEP, Idle KEEP
     3: SV Default, Idle KEEP */
  // To Do: rename this variable 
  trm_ant_switch_div_sv_idle_behavior     sv_behavior;

  /* limit for idle_idle collision before the denied client
     is given the preferred antenna */
  uint8 idle_idle_col_limit;

  /* reserved for future use */
  uint16 reserved1;

} trm_ant_switch_div_data_type;


/*----------------------------------------------------------------------------
  Antenna Switch callback info type. This enum is sent from TRM to the L1s.
  This indicates if any config completes occurred or if any config action has
  to be initiated from L1s.
----------------------------------------------------------------------------*/
typedef enum
{
  /* Represents action:
      TECH needs to do switching to config specified by config parm,
      TRM will pass this action in case of PASSIVE SWITCHING */
  TRM_ANT_SWITCH_INITIATE_SET_CONFIG,

  /* Represents info:
      Switching was complete. When a tech subscribes to switch
      change during registering mode with TRM, TRM will call cb
      with this action everytime the switching is completed */
  TRM_ANT_SWITCH_SET_CONFIG_DONE,

  /* Represents info:
     Switching has started by other tech in tx. When a tech subscribes
     to switch change during registering mode with TRM, TRM will call cb
     with this action everytime the switching is initiated */
  TRM_ANT_SWITCH_SET_CONFIG_STARTED,

  /* Represents info: NOT USED */
  TRM_ANT_SWITCH_ENABLED,

  /* represents info: NOT USED */
  TRM_ANT_SWITCH_DISABLED,

  /* no action/info: INTERNAL */
  TRM_ANT_SWITCH_ACTION_NONE

} trm_ant_switch_action_type;


/*----------------------------------------------------------------------------
  Input struct for fall-back to default config cb.
----------------------------------------------------------------------------*/
typedef struct 
{
  /* feedback of action taken or to be taken */
  trm_ant_switch_action_type            action;

  /* configuration which was set or needs to be set */
  trm_ant_switch_div_config_type       config;

  /* feature override info */
  trm_ant_switch_feature_override_type feature_override;

  /* Client_id: used by G to diff betwn G1, G2 or G3 */
  trm_client_enum_t                    client;

} trm_ant_switch_cb_info_type;


/*----------------------------------------------------------------------------
  Callback input for calling to force fallback to default config.
----------------------------------------------------------------------------*/
typedef void (*trm_ant_switch_div_callback_t)
(
/* ant config info type */
trm_ant_switch_cb_info_type ant_cfg_info
);


/*----------------------------------------------------------------------------
  Ant Switch Set Mode Return Type.
----------------------------------------------------------------------------*/
typedef enum
{
  /* Invalid client provided as input */
  TRM_SET_MODE_DENIED_INVALID_PARAMETERS,

  /* Set Mode Granted */
  TRM_SET_MODE_GRANTED,

  /* Set the switch to the mentioned state in config and continue */
  TRM_SET_SWITCH_AND_CONTINUE,

  /* feature is not supported */
  TRM_SET_MODE_DENIED_FEATURE_UNSUPPORTED,

  /* set mode request denied */
  TRM_SET_MODE_DENIED    

} trm_ant_switch_set_mode_return_type;

/*----------------------------------------------------------------------------
  Ant Switch Set Mode Return Type.
----------------------------------------------------------------------------*/
typedef struct
{
  /* set mode return type */
  trm_ant_switch_set_mode_return_type set_mode_grant;

  /* configuration to be set if feedback is set rf and continue */
  trm_ant_switch_div_config_type       config;

  /* Anonymous payload to be echoed through set mode grant callback */
  trm_request_tag_t                     tag;

  /* Client_id to derive instance*/
  trm_client_enum_t					 client;

} trm_ant_switch_set_mode_type;

/*----------------------------------------------------------------------------
  Ant Switch Set Mode Call Back Type.
----------------------------------------------------------------------------*/
typedef void (*trm_ant_switch_set_mode_callback_t)
(
/* set mode call-back info */
trm_ant_switch_set_mode_type set_mode_info
);

/*----------------------------------------------------------------------------
  Ant Switch subscribe switch info
----------------------------------------------------------------------------*/
typedef enum
{
  /* subscribe to none */
  TRM_SUBS_NONE=0x00,
  /* subscribe to switch change */
  TRM_SUBS_SW_COMPLETE,
  /* subscribe to permission change */
  TRM_SUBS_SW_PERM_CHANGE,
  /* invalid subscription */
  TRM_SUBS_SW_INVALID
}trm_ant_switch_subcribe_switch_info;

/*----------------------------------------------------------------------------
  Ant Switch Set Mode Input Type.
----------------------------------------------------------------------------*/
typedef struct
{
  /* client requesting the mode set*/
  trm_client_enum_t client;

  /* Mode to be set */
  trm_client_mode_type mode;

  /* set mode call-back info. This is called when set mode request is
     granted or denied. */
  trm_ant_switch_set_mode_callback_t set_mode_cb;

  /* ant switch call-back info. This is used when an explicit set swith
     has to be initiated or if feed-back has to be provided about any other
     set info*/
  trm_ant_switch_div_callback_t      ant_switch_cb;

  /* to subscribe to switch completion, below value should be set to TRUE
     TRM will call the callback function with actiong: SET_CONFIG_DONE to
     inform the client that switching is complete */
  boolean                            inform_switch_change;

  /* Deprecated... NOT USED*/
  trm_ant_switch_subcribe_switch_info   subscribe_switch_info;

  /* Anonymous payload to be echoed through set mode grant callback */
  trm_request_tag_t                     tag;

} trm_set_mode_input_type;

/*----------------------------------------------------------------------------
  Antenna  Switch Return Type.
----------------------------------------------------------------------------*/
typedef enum
{
  /* Set Switch completed successfully */
  TRM_ANT_SWITCH_DIV_SET_SUCCESS,

  /* set switch failed */
  TRM_ANT_SWITCH_DIV_SET_CFG_DENIED,

  /* Set Switch failed. Feature Disabled */
  TRM_ANT_SWITCH_DIV_SET_FAIL_FEAT_DISABLED,

  /* Failed: Modem in SV transmit mode */
  TRM_ANT_SWITCH_DIV_SET_FAIL_IN_SV_TRANSMIT,

  /* Failed: In Enforced Default mode */
  TRM_ANT_SWITCH_DIV_SET_FAIL_IN_ENFORCED_DEFAULT,

  /* Failed: Invalid input provided */
  TRM_ANT_SWITCH_DIV_SET_FAIL_INVALID_INPUT,

  /* Failed: In transition */
  TRM_ANT_SWITCH_DIV_SET_FAIL_IN_TRANSITION,

  /* Failed: Switch is locked */
  TRM_ANT_SWITCH_DIV_SET_FAIL_IN_LOCK,
  /* failed: switch is in privilege mode */
  TRM_ANT_SWITCH_IN_PRIVILEGE_CONTROL,
  /* failed: swithcing not allowed for current mode */
  TRM_ANT_SWITCH_SET_FAIL_CURR_MODE,
  /* success: switching requested to LWT */
  TRM_ANT_SWITCH_DIV_SET_DONE_BY_LWT,

  /* success: PASSIVE SWITCHING done by tech in tx */
  TRM_ANT_SWITCH_SET_CFG_BY_TX_TECH

}trm_set_ant_switch_return_type;

/*----------------------------------------------------------------------------
  Ant Switch Set Mode and Config Type.
----------------------------------------------------------------------------*/
typedef struct
{
  /* set mode return type */
  trm_ant_switch_set_mode_return_type   set_mode_grant;

  /* set config return type */
  trm_set_ant_switch_return_type  set_config_grant;

  /* configuration to be set if feedback is set rf and continue */
  trm_ant_switch_div_config_type       config;

  /* Anonymous payload to be echoed through set mode grant callback */
  trm_request_tag_t                     tag;

  /* Client_id to derive instance*/
  trm_client_enum_t					 client;

} trm_asd_set_mode_and_config_type;

/*----------------------------------------------------------------------------
  Ant Switch Set Mode and Config Call Back Type.
----------------------------------------------------------------------------*/
typedef void (*trm_asd_set_mode_and_config_cb_t)
(
/* set mode call-back info */
trm_asd_set_mode_and_config_type  set_mode_cfg_info
);

/*----------------------------------------------------------------------------
  Ant Switch Set Mode and Config Input Type.
  This structure should be used in set mode and switch api
----------------------------------------------------------------------------*/
typedef struct
{
  /* client requesting the mode set*/
  trm_client_enum_t client;

  /* Mode to be set */
  trm_client_mode_type mode;

  /* configuration tech want to switch to */
  trm_ant_switch_div_config_type  config;

  /* set mode call-back info. This is called when set mode request is
     granted or denied. */
  trm_asd_set_mode_and_config_cb_t   set_mode_cb;

  /* ant switch call-back info. This is used when an explicit set swith
     has to be initiated or if feed-back has to be provided about any other
     set info*/
  trm_ant_switch_div_callback_t      ant_switch_cb;

  /* to subscribe to switch completion, below value should be set to TRUE
    TRM will call the callback function with actiong: SET_CONFIG_DONE to
    inform the client that switching is complete */
  boolean                            inform_switch_change;

  /* Deprecated.. NOT USED. */
  trm_ant_switch_subcribe_switch_info   subscribe_switch_info;

  /* Anonymous payload to be echoed through set mode grant callback */
  trm_request_tag_t                     tag;

} trm_set_mode_and_config_input_t;

/*----------------------------------------------------------------------------
  Ant Switch Input Data for Settng Configuration
----------------------------------------------------------------------------*/
typedef struct
{
  /* client setting the switch configuration */
  trm_client_enum_t              client;

  /* configuration to which the switch is being set */
  trm_ant_switch_div_config_type config;

  /* request type: indicates crisis/Normal mode */
  boolean   is_crisis_mode;

} trm_ant_switch_set_input_type;


/*----------------------------------------------------------------------------
  Ant Switch Complete input type
----------------------------------------------------------------------------*/
typedef struct
{
  /* client completing the switch */
  trm_client_enum_t client;

  /* configuration to switch to */
  trm_ant_switch_div_config_type config;
} trm_switch_complete_input_type;


/*----------------------------------------------------------------------------
  Ant Switch State Enum Type
----------------------------------------------------------------------------*/
typedef enum
{
  TRM_ANT_SWITCH_DIV_ENABLED,

  TRM_ANT_SWITCH_DIV_DISABLED,
} trm_ant_switch_state_type;


/*----------------------------------------------------------------------------
  Set Ant Switch Diversity State Input Struct Type
----------------------------------------------------------------------------*/
typedef struct
{
  trm_client_enum_t         client; /* client */
  trm_ant_switch_state_type state;  /* state */
} trm_set_asd_state_input_type;

/*----------------------------------------------------------------------------
  Ant Switch Mode Notification Info Type.
----------------------------------------------------------------------------*/
typedef struct
{
  /* client completing the mode set*/
  trm_client_enum_t client;

  /* Mode to which it transitioned */
  trm_client_mode_type mode;
} trm_ant_switch_mode_notification_info_type;

/*----------------------------------------------------------------------------
  Ant Switch Mode Notification call-back type.
----------------------------------------------------------------------------*/
typedef void (*trm_ant_switch_mode_notification_callback_t)
(
/* set mode call-back info */
trm_ant_switch_mode_notification_info_type set_mode_info
);

/*----------------------------------------------------------------------------
  Ant Switch Mode Notification Input Type.
----------------------------------------------------------------------------*/
typedef struct
{
  /* client requesting the mode set*/
  trm_client_enum_t client;

  /* indicates whether notification should be turned on or not */
  boolean should_notify;

  /* if notification is turned on, the call-back to call for notification */
  trm_ant_switch_mode_notification_callback_t mode_change_cb;
} trm_ant_switch_mode_notification_input_type;

/*----------------------------------------------------------------------------
  Ant Switch Set Control Type enum.
----------------------------------------------------------------------------*/
typedef enum
{
  /* release the switch control
      :translates to tech being in SAFE state */
  TRM_ANT_SWITCH_CTRL_RELEASE,

  /* request for switch control 
      :translates to tech being in NOT SAFE state */
  TRM_ANT_SWITCH_CTRL_REQUEST,

  /* invalid request */
  TRM_ANT_SWITCH_CTRL_INVALID

}trm_ant_switch_set_ctrl_enum;

/*----------------------------------------------------------------------------
  Ant Switch Set Control Type.
----------------------------------------------------------------------------*/
typedef struct
{
  /* client */
  trm_client_enum_t               client;

  /* control request/release */
  trm_ant_switch_set_ctrl_enum    req_type;

  /* allow critical switching even if unsafe and higher priority */
  boolean                         allow_crisis_mode_switching;

}trm_ant_switch_set_ctrl_type;

/*----------------------------------------------------------------------------
  TRM Asdiv Switch State Update Result Type
----------------------------------------------------------------------------*/
typedef enum
{

  /* SWITCH STATE UPDATE LOCK */
  TRM_ASDIV_SWITCH_STATE_UPDATE_FAILED,

  /* SWITCH STATE UPDATE UNLOCK */
  TRM_ASDIV_SWITCH_STATE_UPDATE_SUCCESS,

}trm_asdiv_switch_state_update_result_t;

/*----------------------------------------------------------------------------
  TRM Asdiv Switch State Update Fail Reason Type
----------------------------------------------------------------------------*/
typedef enum
{

  /* SWITCH STATE UPDATE LOCK */
  TRM_ASDIV_STATE_UPDATE_FAIL_NONE,

  /* SWITCH STATE UPDATE LOCK */
  TRM_ASDIV_STATE_UPDATE_FAIL_FEAT_NOT_SUPPORTED,

  /* SWITCH STATE UPDATE LOCK */
  TRM_ASDIV_STATE_UPDATE_FAIL_INVALID_INFO,

  /* SWITCH STATE UPDATE UNLOCK */
  TRM_ASDIV_STATE_UPDATE_FAIL_SWITCH_IN_PROGRESS,

}trm_asdiv_switch_state_update_fail_reason_t;

/*----------------------------------------------------------------------------
  TRM Asdiv Switch State Update Return Info Type
----------------------------------------------------------------------------*/
typedef struct
{

  /* SWITCH STATE UPDATE LOCK */
  trm_asdiv_switch_state_update_result_t  update_result;

  /* SWITCH STATE UPDATE UNLOCK */
  trm_asdiv_switch_state_update_fail_reason_t  fail_reason;

}trm_asdiv_switch_state_update_return_info_t;


/*----------------------------------------------------------------------------
  TRM Asdiv Switch State Update Request Type
----------------------------------------------------------------------------*/
typedef enum
{

  /* SWITCH STATE UPDATE LOCK */
  TRM_ASDIV_SWITCH_UNLOCK_REQUEST,

  /* SWITCH STATE UPDATE UNLOCK */
  TRM_ASDIV_SWITCH_LOCK_REQUEST,

  /* invalid request */
  TRM_ASDIV_UPDATE_REQUEST_INVALID

}trm_asdiv_switch_state_update_request_t;

/*----------------------------------------------------------------------------
  TRM Asdiv Switch State Update Reason Type
----------------------------------------------------------------------------*/
typedef enum
{

  /* QTA PROCESS */
  TRM_ASDIV_STATE_UPDATE_REASON_QTA,

  /* IRAT PROCESS */
  TRM_ASDIV_STATE_UPDATE_REASON_IRAT,

  /* RF INTERACTION */
  TRM_ASDIV_STATE_UPDATE_REASON_RF_INTERACT,

  /* OTHER CRITICAL PROCESS */
  TRM_ASDIV_STATE_UPDATE_REASON_CRIT_PROCESS,

  /* INVALID REASON */
  TRM_ASDIV_STATE_UPDATE_REASON_MAX

}trm_asdiv_switch_state_update_reason_t;

/*----------------------------------------------------------------------------
  TRM Asdiv Switch State Update Request Info
----------------------------------------------------------------------------*/
typedef struct
{

  /* CLIENT ID */
  trm_client_enum_t   client;

  /* REQUEST TYPE */
  trm_asdiv_switch_state_update_request_t    request_type;

  /* REQUEST REASON */
  trm_asdiv_switch_state_update_reason_t     update_reason;

  /* SWITCH CONFIGURATION- for future purpose */
  trm_ant_switch_div_config_type             switch_config;

  /* If, TRUE- update can happen even if switch is in progress
               and update result is always returned SUCCESS. 
         FALSE- update cannot happen if switch is in progress
               and update result is returned as FAILED */
  boolean     update_if_switch_in_progress;

}trm_asdiv_switch_state_update_request_info_t;


/*----------------------------------------------------------------------------
  TRM Request Criticality Type
----------------------------------------------------------------------------*/
typedef enum
{
  /* indicates low priority, triggers non-quick hopping */
  TRM_REQUEST_CRITICALITY_LOW,

  /* indicates high priority request, triggers quick hopping */
  TRM_REQUEST_CRITICALITY_HIGH,

  TRM_REQUEST_CRITICALITY_MAX,

} trm_request_criticality_type;


/*----------------------------------------------------------------------------
  TRM Conn Mode Hop Type
----------------------------------------------------------------------------*/
typedef enum
{
  /* indicates Quick, critical hop required */ 
  TRM_CONNECTED_HOP_IMMEDIATE_QUICK,

  /* indicates non-quick normal hop required */
  TRM_CONNECTED_HOP_IMMEDIATE_NORMAL,

  /* reserved for future purpose */
  TRM_CONNECTED_HOP_FUTURE_NORMAL,

  TRM_CONNECTED_HOP_MAX,

} trm_conn_mode_hop_type;

typedef struct
{
  /*! client id */
  trm_client_enum_t          client_id;

  /*! Macro priority SUB state from subs_prio.h e.g Voice High for active voice call,
      Idle and Acq cannot be distinguished. for both, this state will be BASELINE.
      LIMITSMGR sends uint8 value that needs to be casted to sp_subs_states_enum_t*/
  uint8                      macro_state;

  /*! Subscription ID */
  sys_modem_as_id_e_type     as_id;

}trm_asdiv_tech_state_data;

typedef struct
{
  /*! Number of active techs */
  uint8                        num_active_techs;
  
  /*! State of active techs in order of macro priority */
  trm_asdiv_tech_state_data    tech_state[TRM_ASDIV_MAX_CONCURRENT_TECHS];

}trm_asdiv_tech_state_info_type;



/*============================================================================

                            FUNCTION DECLARATIONS

============================================================================*/



/*============================================================================

FUNCTION TRM_GET_RF_CONFIG

DESCRIPTION
  Return the RF configuration
  
DEPENDENCIES
  trm_init( ) must have been called.

RETURN VALUE
  The RF configuration in use

SIDE EFFECTS
  None

============================================================================*/

trm_rf_enum_t trm_get_rf_config( void );


/*============================================================================

FUNCTION TRM_GET_EXTENSION_FLAG

DESCRIPTION
  This function returns a boolean value indicating the status of the specified
  extension flag bit for the given TRM reason code
  
DEPENDENCIES
  None

RETURN VALUE
  TRUE if the extension flag is enabled for the specified reason.
  FALSE otherwise

SIDE EFFECTS
  None whatsoever.

============================================================================*/

boolean trm_get_extension_flag 
(
/* Cleint Id */
trm_client_enum_t       client,

/* Reason for which the inversion status to be checked */
trm_reason_enum_t       reason,

/* extension flag mask to be checked */
uint8                   ext_flag_mask
);

/*============================================================================

FUNCTION TRM_SET_EXTENSION_FLAG

DESCRIPTION
  This function sets the specified extension flag for the given reason
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

void trm_set_extension_flag 
(
/* Client Id */
trm_client_enum_t       client,

/* Reason for which the extension flag is to be set */
trm_reason_enum_t       reason,

/* extension flag mask to be updated */
uint8                   ext_flag_mask,

/* value to be set against the extension flags */
boolean                 status
);

/*============================================================================

FUNCTION TRM_GET_EXTENSION_MASK

DESCRIPTION
  This function returns full mask information set against a given TRM reason 
  code.
  
DEPENDENCIES
  None

RETURN VALUE
  Full mask information set against a given reason code.

SIDE EFFECTS
  None

============================================================================*/
uint8 trm_get_extension_mask
(
/* Client Id */
trm_client_enum_t       client,

/* Reason whose complete mask value needs to be read */
trm_reason_enum_t       reason
);

/*============================================================================

FUNCTION TRM_SET_EXTENSION_MASK

DESCRIPTION
  This function sets full mask information against a given TRM reason code
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_set_extension_mask
(
/* Client Id */
trm_client_enum_t       client,

/* Reason for which the mask needs to be set */
trm_reason_enum_t       reason,

/* Mask having combined set of flags */
uint8                   mask
);



/*============================================================================

FUNCTION TRM_TASKINIT

DESCRIPTION
  Initializes the data required for task start-up. Please note that no
  NV initialization should happen in task init.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_taskinit (void);


/*============================================================================

FUNCTION TRM_TASK

DESCRIPTION
  Task Start-up function where all the NV initialization amongst others is 
  done.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_task ( dword dummy );



/*============================================================================

FUNCTION TRM_ANT_SWITCH_DIV_SET_CONFIG

DESCRIPTION
  Sets the tx diversity switch configuration to given configuration.
  
DEPENDENCIES
  None

RETURN VALUE
  Set Config Success or Error.

SIDE EFFECTS
  None

============================================================================*/
trm_set_ant_switch_return_type trm_ant_switch_div_set_config(
trm_ant_switch_set_input_type newsettings /* new settings info */
);


/*============================================================================

FUNCTION TRM_ANT_SWITCH_DIV_GET_CONFIG

DESCRIPTION
  Get the current tx diversity switch configuration.
  
DEPENDENCIES
  None

RETURN VALUE
  Current tx diversity switch configuration or error due to switching.

SIDE EFFECTS
  None

============================================================================*/
trm_ant_switch_div_config_type trm_ant_switch_div_get_config(
trm_ant_switch_get_input_type input_type /* get config input type */
);


/*============================================================================

FUNCTION TRM_ANT_SWITCH_DIV_GET_TEST_MODE_CTRL

DESCRIPTION
  Get the current tx diversity test mode control.
  
DEPENDENCIES
  None

RETURN VALUE
  Current tx diversity test mode control

SIDE EFFECTS
  None

============================================================================*/
trm_ant_switch_div_test_mode_ctrl_type trm_ant_switch_div_get_test_mode_ctrl(void);

/*============================================================================

FUNCTION TRM_ANT_SWITCH_DIV_GET_DWELLING_TIME

DESCRIPTION
  Get the current tx diversity dwelling time.
  
DEPENDENCIES
  None

RETURN VALUE
  Current tx diversity dwelling time.

SIDE EFFECTS
  None

============================================================================*/
trm_ant_switch_div_dwelling_time_type trm_get_ant_switch_div_dwelling_time(void);


/*============================================================================

FUNCTION TRM_ANT_SWITCH_DIV_SET_MODE

DESCRIPTION
  L1s update the mode which they are entering: SLEEP, RX or TX. Depending on
  the mode, set mode logic could initiate call-back to other techs.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_ant_switch_div_set_mode(
trm_set_mode_input_type set_mode_data /* Set mode Input type */
);

/*============================================================================

FUNCTION TRM_ASD_SET_MODE_AND_CONFIG

DESCRIPTION
  L1s use this function to register modes and request switching permission
  This should be used for idle wakeup/sleep modes only
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_asd_set_mode_and_config(
const trm_set_mode_and_config_input_t  *input_data
);


/*============================================================================

FUNCTION TRM_ANT_SWITCH_DIV_SWITCH_COMPLETE

DESCRIPTION
  L1s update trm about switch complete once they have done the switch following
  set_config or set_mode.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_ant_switch_div_switch_complete(
trm_switch_complete_input_type switch_input /* switch complete input type */
);

/*============================================================================

FUNCTION TRM_ANT_SWITCH_DIV_SWITCH_CANCEL

DESCRIPTION
  L1s update trm about switch incomplete if for any reason the switching
  couldnt go through.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_ant_switch_div_switch_cancel
(
trm_switch_complete_input_type req_data 
);

/*============================================================================

FUNCTION TRM_GET_CLIENT_ANT_SWITCH_MODE

DESCRIPTION
  Returns the mode of the requested client.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
trm_client_mode_type trm_get_client_ant_switch_mode(
trm_client_enum_t clientid
);

/*============================================================================

FUNCTION TRM_SET_ANT_SWITCH_DIV_STATE

DESCRIPTION
  Updates the ant switch diversity state(enable or disable) for a given client.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_set_ant_switch_div_state(trm_set_asd_state_input_type input);


/*============================================================================

FUNCTION TRM_SET_ANT_SWITCH_DIV_STATE

DESCRIPTION
  Updates the ant switch diversity state(enable or disable) for a given client.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_set_ant_switch_mode_change_notify(
trm_ant_switch_mode_notification_input_type input
);



/*============================================================================

FUNCTION TRM_ANT_SWITCH_IS_SWITCHING_ALLOWED

DESCRIPTION
  Returns whether switching is allowed or not.
  
DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None

============================================================================*/
boolean trm_ant_switch_is_switching_allowed(void);


/*============================================================================

FUNCTION TRM_ANT_SWITCH_SET_CONTROL

DESCRIPTION
  Updates the ant switch diversity switch control to privilege mode,
  it is called when privileged clients request/release the switch
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_ant_switch_set_control
(
trm_ant_switch_set_ctrl_type   ctrl_req 
);


/*============================================================================

FUNCTION TRM_ASDIV_REGISTER_ANT_SWITCH_CALLBACK

DESCRIPTION
  Registers a callback to be called when the antenna configuration changes.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if antenna switching is enabled and the callback was succesfully
  registered; FALSE otherwise.

SIDE EFFECTS
  None

============================================================================*/
boolean trm_asdiv_register_ant_switch_callback
(
  trm_ant_switch_div_callback_t   ant_switch_cb
);


/*============================================================================

FUNCTION TRM_ASDIV_DEREGISTER_ANT_SWITCH_CALLBACK

DESCRIPTION
  Deregisters a callback to be called when the antenna configuration changes.

DEPENDENCIES
  The callback must have been registered with TRM using
  trm_ant_switch_register_ant_switch_callback().

RETURN VALUE
  TRUE if antenna switching is enabled and the callback was successfully
  deregistered; FALSE otherwise.

SIDE EFFECTS
  None

============================================================================*/
boolean trm_asdiv_deregister_ant_switch_callback
(
  trm_ant_switch_div_callback_t   ant_switch_cb
);


/*============================================================================
FUNCTION TRM_UPDATE_TECH_STATE

DESCRIPTION
  Techs (GSM/WCDMA/TDSCDMA) will call this API during Single SIM scenario 
  to update the state. This will NOT be used by TRM for scheduling or arbitrating
  RF devices or any kind of chain management.

DEPENDENCIES
  Device should be in Single SIM mode so chain should not be held by any other tech

RETURN VALUE
  None

SIDE EFFECTS
  None
============================================================================*/

void trm_update_tech_state
(
/* The client which needs the RF resource */
trm_client_enum_t               client,

/* Why the resource is needed (used for priority decisions) */
trm_reason_enum_t               reason,

/* Band which is currently used by tech */
trm_band_t                      band
);

/*============================================================================
FUNCTION TRM_REGISTER_LMTSMGR_TECH_STATE

DESCRIPTION
  This function registers Limitsmgr callback to TRM for getting tech's state info

DEPENDENCIES
  TRM should be intialized

RETURN VALUE
  None

SIDE EFFECTS
  None
============================================================================*/

void trm_register_lmtsmgr_client
(
/* The Limitsmgr callback */
trm_tech_state_update_callback_type   callback
);

/*----------------------------------------------------------------------------
  Trm Rfm Device Info Type.
----------------------------------------------------------------------------*/

typedef struct
{

  /* Device type */
  rfm_device_enum_type  device;

  /* Handler info */
  trm_rf_handler_type       rf_handle;

  /* capability bitmask */
  uint32                 capability;

} trm_rfm_device_info_type;

/*----------------------------------------------------------------------------
  trm Rfm Device Support Input Type.
----------------------------------------------------------------------------*/
typedef struct
{
  /* client id */
  trm_client_enum_t         client;

  /* band info */
  trm_band_t                band;

  /* number of devices supporting the band */
  uint8                     num_of_device;

  /* rfm device info supporting the band requested */
  trm_rfm_device_info_type  rfm_device[TRM_MAX_CHAINS];

}trm_rfm_device_support_input_type;

/*============================================================================

FUNCTION TRM_GET_DEVICE_SUPPORT_INFO

DESCRIPTION
  Checks the RFM devices that support the band provided

RETURN VALUE
  List of rfm devices that support the band provided and the capability of
  each device.

SIDE EFFECTS
  following fields in the passed structure gets modified
    num_of_devices- indicates how many device support the band
    rfm_device- represents the list of device with capability

============================================================================*/
void trm_get_device_support_info(

trm_rfm_device_support_input_type *info
);

/*============================================================================
FUNCTION TRM ASDIV REQUEST SWITCH_STATE UPDATE 

DESCRIPTION
  This function allows tech to update the switch state to LOCK or UNLOCK.
 
  If updated to LOCK state, TRM will deny any request for switching irrespective
  of priority, till the tech updates it to UNLOCK state.
  Reason used for locking should be used to unlock as well.

DEPENDENCIES
  Switch denials if this api is called to lock the switch

RETURN VALUE
  None

SIDE EFFECTS
  None
============================================================================*/
trm_asdiv_switch_state_update_return_info_t trm_asdiv_request_switch_state_update
(
trm_asdiv_switch_state_update_request_info_t*  update_info
);

/*============================================================================

FUNCTION TRM_ASDIV_SET_PREF_CONFIG_TEST

DESCRIPTION
  OFFTARGET USE ONLY API:
      to set the preferred config of a client
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_asdiv_set_pref_config_test
(
trm_client_enum_t                cl, 
trm_ant_switch_div_config_type   cfg
);

/*============================================================================

FUNCTION TRM_ASDIV_GET_PREF_CONFIG_INFO_TEST

DESCRIPTION
  OFFTARGET USE ONLY API:
      to get the preferred config of a client
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_asdiv_get_pref_config_info_test
(
   trm_client_enum_t                cl, 
   trm_ant_switch_div_config_type  *cfg,
   rfm_device_enum_type            *dev
);


/* BOLT 2.0 APIs */

/*----------------------------------------------------------------------------
  Maximum Bands Supported
----------------------------------------------------------------------------*/
#define MAX_BAND_INFO 7

typedef uint8  trm_tech_info_mask_type;

/*----------------------------------------------------------------------------
  TRM Tech Info mask Enum Type
----------------------------------------------------------------------------*/
typedef enum
{
  /* mask bit position 0
      Specific to LTE as of now to indicate if it is TDD or FDD */
  TRM_TDD_TECH_INFO = 0,

  TRM_TECH_INFO_MAX

}trm_tech_info_mask_enum_t;

/* utility macros for L1s */
#define TRM_SET_TDD_TECH_INFO(mask) ( (mask) |= 1<<TRM_TDD_TECH_INFO )

/*----------------------------------------------------------------------------
  Frequency Information
----------------------------------------------------------------------------*/
typedef struct
{
  /* Band Information */
  trm_band_t band;

} trm_frequency_info;


/*----------------------------------------------------------------------------
  Frequency Input Information
----------------------------------------------------------------------------*/
typedef struct
{
  /* Number of bands */
  uint32             num_bands;

  /* Band Information */
  trm_frequency_info    bands[MAX_BAND_INFO];
} trm_freq_input_type;


/*----------------------------------------------------------------------------
  Structure to log band information of a client
----------------------------------------------------------------------------*/
typedef PACK( struct )
{  
  /* Band Information */
  int32    bands[MAX_BAND_INFO];
  
  /* Number of bands */
  uint32                num_bands;
} trm_log_band_info_type;

/*----------------------------------------------------------------------------
  TRM Operating mode info struct
----------------------------------------------------------------------------*/
typedef struct
{
  /* operating mode information */
  trm_operating_modes_enum_type        mode;

  /* fallback mode type information */
  trm_fallback_mode_enum_type          fallback_type;

  /* Current clients shown in operating mode log */
  trm_client_enum_t                    client1;
  trm_client_enum_t                    client2;

  /* Current client bands shown in operating mode log */
  trm_log_band_info_type               client1_bands;
  trm_log_band_info_type               client2_bands;

}trm_operating_mode_info_type;

/*----------------------------------------------------------------------------
  Client Information Structure
----------------------------------------------------------------------------*/
typedef struct
{

  /* The client which is releasing the resource */
  trm_client_enum_t               client;

  trm_tag_type                    tag;

}  trm_client_info;

/*----------------------------------------------------------------------------
  Input information for TRM Chain Request
----------------------------------------------------------------------------*/
typedef struct
{
  /* The client which needs the transceiver resource */
  trm_client_enum_t               client_id;

  /* The transceiver resource which is being requested */
  trm_resource_enum_t             resource;

  /* How long the resource will be needed for (in sclks) */
  trm_duration_t                  duration;

  /* Why the resource is needed (used for priority decisions) */
  trm_reason_enum_t               reason;

  /* Activity the resource is being requested for */
  sys_proc_type_e_type           sub_reason;

  /* Band/s that would be used */
  trm_freq_input_type             freq_info;

  /* Unique Identifier to indicate the wakeup for which reservation is done */
  uint64                          wakeup_identifier;

  /* Tag */
  trm_tag_type                    tag;

  /* Indicates if the request is critical or not. This information is used
     in Single Tx DSDA to qualify the hop criticality. */
  trm_request_criticality_type    criticality_info;

  /* If the client is LTE, this field is used to indicate if it is LTE TDD
     or LTE FDD. Other techs can ignore this field. This field is used in
     Single Tx DSDA for device allocation */
  trm_tech_info_mask_type         tech_info_mask;

  /* Indicates Client's TuneAway prefence when reserving for a chain 
     if client doesn't have preference it should be set to TRM_TUNEAWAY_NONE */
  trm_tuneaway_enum_type          ta_info;

  /* Indicates Client with which device needs to be shared */
  trm_device_sharing_info         dev_sharing_info;

} trm_request_input_info;


/*----------------------------------------------------------------------------
  Enum indicating Grant/Pending/Denial for any trm transaction
----------------------------------------------------------------------------*/
typedef enum
{
  /* The transaction is kept pending */
  TRM_PENDING,

  /* The transaction is granted */
  TRM_GRANTED,

/* The transaction is denied */
  TRM_DENIAL,

  /* Added for IRAT */
  TRM_PARTIAL_GRANT,

  /* Enum Max */
  TRM_RETURN_MAX,

} trm_grant_return_enum_type;

/*----------------------------------------------------------------------------
  Enum indicating Grant/Pending/Denial for any trm transaction
----------------------------------------------------------------------------*/
typedef enum
{
  /* RF needs be tuned */
  TRM_RF_TUNE_REQUIRED,

  /* RF need not be tuned */
  TRM_RF_TUNE_NOT_REQUIRED,

  /* Enum Max */
  TRM_RF_TUNE_MAX,

} rfm_tune_information;


/*----------------------------------------------------------------------------
  Band-Device allocation mapping structure
----------------------------------------------------------------------------*/
typedef struct
{
  trm_frequency_info    band;
  rfm_device_enum_type     dev_allocated;
  trm_rf_handler_type      rf_handle_allocated;

} trm_band_dev_alloc_type;


/*----------------------------------------------------------------------------
  Return Type for TRM Request
----------------------------------------------------------------------------*/
typedef struct
{
  trm_grant_return_enum_type  grant;
  uint32                      num_bands;
  trm_band_dev_alloc_type     band_alloc[MAX_BAND_INFO];
  rfm_tune_information        tune_info;
  trm_tag_type                tag;

} trm_request_return_data;


/*============================================================================

FUNCTION TRM_REQUEST

DESCRIPTION
  Specifies the given client needs the given transciever resource, for the
  given duration, for the supplied reason.

  The resource request is immediately evaluated, and the result returned.

  This may be used in conjunction with trm_reserve_at(). The band information
  is used to check if the band is valid and also if components mapped to the
  band don't have any concurrency issues.
  
DEPENDENCIES
  None

RETURN VALUE
  Chain Grant Information.

SIDE EFFECTS
  If the client currently holds an RF chain, that chain is released before
  the request is evaluated.

============================================================================*/
void trm_request
(
/* Input Structure for Request */
trm_request_input_info  *req_info,

/* Output Structure from Request Processing */
trm_request_return_data *ret_data
);

/*----------------------------------------------------------------------------
  Input information for TRM INFORM GRANT EVENT
----------------------------------------------------------------------------*/
typedef struct
{
  /* The client which needs the transceiver resource */
  trm_client_enum_t               client_id;

  /* The transceiver resource which is being requested */
  trm_resource_enum_t             resource;

  /* How long the resource will be needed for (in sclks) */
  trm_duration_t                  duration;

  /* Why the resource is needed (used for priority decisions) */
  trm_reason_enum_t               reason;

  /* Band/s that would be used */
  trm_freq_input_type             freq_info;

  /* Unique Identifier to indicate the wakeup for which reservation is done */
  uint64                          wakeup_identifier;

  /* Tag */
  trm_tag_type                    tag;

  /* Grant event. Signifies which chain was used or if
     there was a denial that needs to be notified */
  trm_grant_event_enum_t          grant_event;

} trm_inform_grant_event_input_info;

/*----------------------------------------------------------------------------
  Return Type for TRM INFORM GRANT EVENT
----------------------------------------------------------------------------*/
typedef struct
{
  trm_grant_return_enum_type  grant;
  trm_tag_type                tag;

} trm_inform_grant_event_return_data;

/*============================================================================

FUNCTION TRM_INFORM_GRANT_EVENT

DESCRIPTION
  Notifies TRM that a particular client got granted or denied a TRM lock. This
  shall be used by TRM for book keeping page collision results and also for 
  having a unique indicator for a wakeup. 
  This should be called only once for a particular wakeup after that wakeup 
  is complete and before reserving for next wakeup.

  Note: This API does not lock a chain and also does not cause any chain
        release.
  
DEPENDENCIES
  None

RETURN VALUE
  Chain Grant Information.

SIDE EFFECTS
  None

============================================================================*/
void trm_inform_grant_event
(
/* Input Structure for Request */
trm_inform_grant_event_input_info  *input,

/* Output Structure from Request Processing */
trm_inform_grant_event_return_data *output
);


/*----------------------------------------------------------------------------
  Input Information for Chain reservation
----------------------------------------------------------------------------*/
typedef struct
{
  /* The client which needs the transceiver resource */
  trm_client_enum_t               client_id;

  /* The transceiver resource which is being requested */
  trm_resource_enum_t             resource;

  /* When the resource will be needed (sclks timestamp) */
  trm_time_t                      when;

  /* How long the resource will be needed for (in sclks) */
  trm_duration_t                  duration;

  /* Why the resource is needed (used for priority decisions) */
  trm_reason_enum_t               reason;

  /* Activity the resource is being requested for */
  sys_proc_type_e_type           sub_reason;

  /* Band/s that would be used */
  trm_freq_input_type             freq_info;

  /* Unique Identifier to indicate the wakeup for which reservation is done */
  uint64                          wakeup_identifier;

  /* Tag Information */
  trm_tag_type                    tag;

  /* Indicates if the request is critical or not. This information is used
     in Single Tx DSDA to qualify the hop criticality. */
  trm_request_criticality_type     criticality_info;

    /* If the client is LTE, this field is used to indicate if it is LTE TDD
     or LTE FDD. Other techs can ignore this field. This field is used in
     Single Tx DSDA for device allocation */
  trm_tech_info_mask_type         tech_info_mask;

  /* Indicates Client's TuneAway prefence when reserving for a chain 
     if client doesn't have preference it should be set to TRM_TUNEAWAY_NONE */
  trm_tuneaway_enum_type          ta_info;

  /* Indicates Client with which device needs to be shared */
  trm_device_sharing_info         dev_sharing_info;

   /* unlock release time Tag Information */
   trm_tag_type                    time_tag;


} trm_reserve_at_input_info;



/*============================================================================

FUNCTION TRM_RESERVE_AT

DESCRIPTION
  Specifies the given client needs the given transceiver resource at the given
  time, for the given duration, for the supplied reason.

  This would be mostly used with the "trm_request()" function.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  If the client currently holds an RF chain, the chain is released.

============================================================================*/
void trm_reserve_at
(
/* Input Information for reserve at */
trm_reserve_at_input_info* res_at_info
);


/*----------------------------------------------------------------------------
  Input Information for Chain request and notification
----------------------------------------------------------------------------*/
typedef trm_request_input_info trm_request_notify_input_info;


/*============================================================================

FUNCTION TRM_REQUEST_AND_NOTIFY

DESCRIPTION
  Specifies the given client needs the given transceiver resource, for the
  given duration, for the supplied reason.

  When the resource can be granted to the client, the event callback for the
  client will be called with the result of the lock request.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  If the client currently holds an RF chain, the chain is released.

============================================================================*/
void trm_request_and_notify
(
/* Input Information for Request and Notify */
trm_request_notify_input_info *req_not_info
);


/*----------------------------------------------------------------------------
  Input Information for Enhanced Chain request and notification
----------------------------------------------------------------------------*/
typedef struct 
{
  /* The client which needs the transceiver resource */
  trm_client_enum_t               client_id;

  /* The transceiver resource which is being requested */
  trm_resource_enum_t             resource;

  /* How long the resource will be needed for (in sclks) */
  trm_duration_t                  duration;

  /* Why the resource is needed (used for priority decisions) */
  trm_reason_enum_t               reason;

  /* Activity the resource is being requested for */
  sys_proc_type_e_type           sub_reason;

  /* Band/s that would be used */
  trm_freq_input_type             freq_info;

  /* Unique Identifier to indicate the wakeup for which reservation is done */
  uint64                          wakeup_identifier;

  /* Tag */
  trm_tag_type                    tag;

  /* Tag to use with the unlock notification */
  trm_tag_type                    unlock_tag;

  /* Indicates if the request is critical or not. This information is used
     in Single Tx DSDA to qualify the hop criticality. */
  trm_request_criticality_type     criticality_info;

  /* If the client is LTE, this field is used to indicate if it is LTE TDD
     or LTE FDD. Other techs can ignore this field. This field is used in
     Single Tx DSDA for device allocation */
  trm_tech_info_mask_type         tech_info_mask;
  
  /* Indicates Client's TuneAway prefence when reserving for a chain 
     if client doesn't have preference it should be set to TRM_TUNEAWAY_NONE */
  trm_tuneaway_enum_type          ta_info;

  /* Indicates Client with which device needs to be shared */
  trm_device_sharing_info         dev_sharing_info;

}trm_request_notify_enh_input_info;


/*============================================================================

FUNCTION TRM_REQUEST_AND_NOTIFY_ENHANCED

DESCRIPTION
  Specifies the given client needs the given transceiver resource, for a 
  minimum of the given duration, for the supplied reason. At the time of the
  grant, the duration is extended to the maximium possible time that the lock
  can be made available to the client without colliding with any other client.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  If the client currently holds an RF chain, the chain is released.
  Also the time value returned via a grant_enh_callback is subject to change
  depending on the nature and timing of other requests from other clients
  after this function call.

============================================================================*/
void trm_request_and_notify_enhanced
(
/* Input Information for Request and Notify Enh */
trm_request_notify_enh_input_info *req_not_info
);




/*----------------------------------------------------------------------------
  Input Information for TRM Release
----------------------------------------------------------------------------*/
typedef trm_client_info trm_release_chain_input_info;


/*----------------------------------------------------------------------------
  Input Information to release multiple chains
----------------------------------------------------------------------------*/
typedef struct
{
  /* Num of releases */
  uint8 num_releases;

  /* release info */
  trm_release_chain_input_info release_info[TRM_MAX_RELEASE_CLIENTS];
 } trm_release_multiple_chains_input_type;
 


/*============================================================================
FUNCTION TRM_RELEASE_CHAIN

DESCRIPTION
  Releases the chain held by a client.
  
DEPENDENCIES
  NONE

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_release_chain
(
trm_release_chain_input_info  *rel_info
);

/*============================================================================
FUNCTION TRM_RELEASE_MULTIPLE_CHAINS

DESCRIPTION
  Allows the client to releases chains belonging to multiple clients 
  simultaneously.
  
DEPENDENCIES
  NONE

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_release_multiple_chains(trm_release_multiple_chains_input_type* input);

/*----------------------------------------------------------------------------
  Input Information for Retain Lock
----------------------------------------------------------------------------*/
typedef struct 
{
  trm_client_enum_t               client;

  trm_tag_type                    tag;

  trm_tag_type                    unlock_tag;
} trm_retain_chain_lock_input_info;

/*============================================================================
FUNCTION TRM_RETAIN_CHAIN_LOCK

DESCRIPTION
  Indicates that the client wants to hold the chain indefinitely - as long as
  there is no other client requesting the chain with higher priority.
  
DEPENDENCIES
  NONE

RETURN VALUE
  None

SIDE EFFECTS
  Unlock call-back could be called to release the chain immediately or in the
  future.

============================================================================*/
void trm_retain_chain_lock
(
trm_retain_chain_lock_input_info *retain_lock_info
);


/*----------------------------------------------------------------------------
  Input Information for TRM Exchange
----------------------------------------------------------------------------*/
typedef struct
{
  /* The first client */
  trm_client_enum_t               client_1;

  /* The second client */
  trm_client_enum_t               client_2;

} trm_client_exchange_input_info;


/*============================================================================
FUNCTION TRM_CLIENT_EXCHANGE

DESCRIPTION
  Exchange of state between two clients.
  
DEPENDENCIES
  NONE

RETURN VALUE
  None

SIDE EFFECTS
  Reason and Chain State could change.

============================================================================*/
void trm_client_exchange
(
trm_client_exchange_input_info *exchange_info
);


/*----------------------------------------------------------------------------
  Dual Recieve Enum Information
----------------------------------------------------------------------------*/
typedef enum
{
  /* Dual Receive Enabled */
  TRM_DUAL_RECEIVE_ENABLED,

  /* Dual Receive Disabled */
  TRM_DUAL_RECEIVE_DISABLED,

  /* Enum Max */
  TRM_DUAL_RECEIVE_MAX
} trm_dual_receive_enum_type;

/*----------------------------------------------------------------------------
  Dual Recieve Output Struct
----------------------------------------------------------------------------*/
typedef struct
{
  /* Dual Receive Information*/
  trm_dual_receive_enum_type dual_rec_info;

}trm_dual_rec_info_return_data;

/*----------------------------------------------------------------------------
  Dual Recieve Enum Information
----------------------------------------------------------------------------*/
typedef enum
{
  /* Tx Sharing DSDA Enabled */
  TRM_SINGLE_TX_DSDA_ENABLED,

  /* Tx Sharing DSDA Disabled */
  TRM_SINGLE_TX_DSDA_DISABLED,

  /* Enum Max */
  TRM_SINGLE_TX_DSDA_MAX

} trm_single_tx_dsda_enum_type;

/*----------------------------------------------------------------------------
  Dual Recieve Output Struct
----------------------------------------------------------------------------*/
typedef struct
{
  /* Single tx DSDA */
  trm_single_tx_dsda_enum_type  info;

} trm_single_tx_dsda_return_data;



/*----------------------------------------------------------------------------
  Return Information for TRM Request and Notify
----------------------------------------------------------------------------*/
typedef struct
{
  trm_grant_return_enum_type  grant;
  uint32                      num_bands;
  trm_band_dev_alloc_type     band_alloc[MAX_BAND_INFO];
  rfm_tune_information        tune_info;
  /* Number of pending request and notify callbacks 
     for other clients belonging to the same RAT. 
     This number is valid for callbacks being 
     called under the current task context. */
  uint8                       num_pending_cbs_for_rat;
} trm_req_n_not_return_data;


/*----------------------------------------------------------------------------
  Return Information for Enhanced TRM Request and Notify
----------------------------------------------------------------------------*/
typedef struct
{
  trm_grant_return_enum_type  grant;
  uint32                      num_bands;
  trm_band_dev_alloc_type     band_alloc[MAX_BAND_INFO];
  rfm_tune_information        tune_info;

  /* Duration by which the lock is extended */
  trm_duration_t              duration;

  /* Unlock tag information */
  trm_tag_type                unlock_tag;

  /* Number of pending request and notify enhanced callbacks 
     for other clients belonging to the same RAT. 
     This number is valid for callbacks being 
     called under the current task context. */
  uint8                       num_pending_cbs_for_rat;
} trm_req_n_not_enh_return_data;

/*----------------------------------------------------------------------------
  Unlock Call-back Information
----------------------------------------------------------------------------*/
typedef struct
{

  /* The event being sent to the client */
  trm_unlock_event_enum_t event;

  /* Client Id of the client being 
     asked to unlock the chain */
  trm_client_enum_t       unlock_client;

  /* Sclk timestamp for TRM_UNLOCK_BY */
  trm_timestamp_t         unlock_by_sclk;

  /* Information about the winning client.
     Higher priority client who's request is 
     causing the unlock notification */
  trm_client_info_t       winning_client_info;

  /* Unlock tag information */
  trm_tag_type            unlock_tag;

  /* Number of pending unlock callbacks 
     for other clients belonging to the same RAT. 
     This number is valid for callbacks being 
     called under the current task context. */
  uint8                   num_pending_cbs_for_rat;

  /* NOTE: QTA/LTA Event (ta_info) should be IGNORED for (event != TRM_UNLOCK_BY)
      Indicates Client's TuneAway prefence when reserving for a chain 
      if client doesn't have preference it should be set to TRM_TUNEAWAY_NONE */
  trm_tuneaway_enum_type    ta_info;

  /* Indicates Client's Restriction info that is causing current Unlock Callback to client 
      For Example, 
      If both RF and FW restrictions are present: 
          restrict_info =  ( 1 << TRM_RF_RESTRICTION  ) | ( 1 << TRM_FW_RESTRICTION  ); 
      If only RF restrictions is valid: 
          restrict_info =  ( 1 << TRM_RF_RESTRICTION  ) ; 
  */
  uint32                    restrict_info;

   /* Tag */
   trm_tag_type                    time_tag;
   
   /* measurment client_id if IRAT is going on this infor needs to be passed to tech */
   trm_client_enum_t       measurment_client_id;

} trm_unlock_callback_data;


/*----------------------------------------------------------------------------
  Typedef grant return to modify return just in case modify enum needs to
  change in the future.
----------------------------------------------------------------------------*/
typedef trm_grant_return_enum_type trm_modify_return_enum_type;


/*----------------------------------------------------------------------------
  Return information for change reason
----------------------------------------------------------------------------*/
typedef struct
{
  /* Result of modify reason operation */
  trm_modify_return_enum_type result;

  /* New reason that was requested */
  trm_reason_enum_t               reason_requested;

} trm_modify_reason_return_type;

/*----------------------------------------------------------------------------
  Enum to indicate what band operation is requested
----------------------------------------------------------------------------*/
typedef enum
{
  /* Request Tune to new band */
  TRM_BAND_TUNE_REQUEST,

  /* Confirm Tune to new band */
  TRM_BAND_TUNE_CONFIRM,

  /* Cancel Tune to new band */
  TRM_BAND_TUNE_CANCEL,

  /* Enum Max */
  TRM_BAND_TUNE_MAX,

} trm_band_tune_action_type;


/*----------------------------------------------------------------------------
  Return information for modify band
----------------------------------------------------------------------------*/
typedef struct
{
  /* Band Modification Result */
  trm_modify_return_enum_type  result;

  /* Band Tune Type for which result is applicable */
  trm_band_tune_action_type    band_tune_type;

  /* To Be DEPRECATED: Frequency Information
     This will be part of trm_band_dev_alloc_type */
  trm_freq_input_type          freq_info;

  /* Split band architecture can result in different device during Band
     Handover, Added parameter to accomodate device hopping due to such
     Band split support */
  uint32                      num_bands;
  trm_band_dev_alloc_type     band_alloc[MAX_BAND_INFO];

  /* on DR-DSDS this will always be TRUE */
  rfm_tune_information         tune_info;

} trm_modify_band_return_type;


/*----------------------------------------------------------------------------
  Return information for modify duration
----------------------------------------------------------------------------*/
typedef struct
{
  /* Duration Modification Result */
  trm_modify_return_enum_type  result;

  /* New Duration that was requested */
  trm_duration_t               new_duration;

} trm_modify_duration_return_type;

/*----------------------------------------------------------------------------
  Union for returning result of any modify operation.
----------------------------------------------------------------------------*/
typedef union
{
  /* Modify Reason Information */
  trm_modify_reason_return_type   mod_reason_info;

  /* Modify Band Information */
  trm_modify_band_return_type     mod_band_info;

  /* Modify Duration Information */
  trm_modify_duration_return_type mod_duration_info;

} trm_modify_chain_state_return_info;

/*----------------------------------------------------------------------------
  Enum to indicate modify operation type
----------------------------------------------------------------------------*/
typedef enum
{
  /* Modify Band */
  TRM_MODIFY_BAND,

  /* Modify Duration */
  TRM_MODIFY_DURATION,

  /* Modify Reason */
  TRM_MODIFY_REASON,

  /* Enum Max */
  TRM_MODIFY_MAX

} trm_modify_state_enum_t;

/*----------------------------------------------------------------------------
  Structure for returning result of any modify operation.
----------------------------------------------------------------------------*/
typedef struct
{
  trm_modify_state_enum_t              modify_type;

  trm_modify_chain_state_return_info   modify_info;

} trm_modify_chain_state_return_data;

/*----------------------------------------------------------------------------
  Enum to indicate Connected Mode Hopping information
----------------------------------------------------------------------------*/
typedef enum
{
  /* Connected Mode Hopping is needed */
  TRM_CONN_MODE_HOP_DEVICE_HOP_REQ,

  /* Enum Max */
  TRM_CONN_MODE_HOP_MAX

} trm_conn_mode_hop_enum_type;


/*----------------------------------------------------------------------------
  Return Information for connected Mode Hopping
----------------------------------------------------------------------------*/
typedef struct
{
  /* what is the hop action requested */
  trm_conn_mode_hop_enum_type   hop_info;

  /* Device for which the hop is requested */
  rfm_device_enum_type          rf_device_hop_to;

  trm_rf_handler_type          rf_handle_hop_to;

  /* Device from which the hop is requested */
  rfm_device_enum_type          rf_device_hop_from;

  trm_rf_handler_type          rf_handle_hop_from;

  /* indicates type of hop- quick vs non-quick */    
  trm_conn_mode_hop_type        hop_type;
  
  /* indicates tech causing the hop */
  trm_client_enum_t             tech_causing_hop;

  /* time by when the hop is expected. 
     Reserved for future purpose */
  trm_duration_t                hop_by_time;

} trm_connected_mode_hop_callback_data;

/*----------------------------------------------------------------------------
  Information for IRAT conflict to WLAN 
----------------------------------------------------------------------------*/
typedef struct
{

  /* information regarding the conflicting client */
  trm_client_enum_t      irat_client;

  /* Sclk timestamp for irat duration */
  trm_duration_t         irat_duration;

}trm_irat_event_conflict_data;

/*----------------------------------------------------------------------------
  Return Structure for QTA data only in case of END QTA Gap
----------------------------------------------------------------------------*/
typedef struct
{
  /* Flag to indicate whether previous QTA Gap was Empty */
  boolean qta_gap_empty;

}trm_qta_return_data_type;

/*----------------------------------------------------------------------------
  Information for QTA to Target client
----------------------------------------------------------------------------*/
typedef struct
{

  /* Operation Start/End QTA*/
  trm_state_oper_enum_type  operation;

  /* Information regarding the Source client (X tech) in X->Y QTA*/
  trm_client_enum_t         source_client;

  /* START_QTA/END_QTA state - RF Source (X tech) tech data for Script Building in */
  rfm_meas_common_param_type* rf_params;

  /* Array of devices allocated for both Primary ([0]) & Secondary ([1]) 
      If device is not allocated, it will be marked as RFM_INVALID_DEVICE*/
  rfm_device_enum_type     dev_allocated[TRM_NUM_PRI_DIV_CLIENTS_PER_TECH];

  trm_rf_handler_type      rf_handle_allocated[TRM_NUM_PRI_DIV_CLIENTS_PER_TECH];

  /* START_QTA state - Set to NULL
     END_QTA state - Target tech (Y in X->Y QTA) updates QTA GAP empty info */
  trm_qta_return_data_type*     qta_result;

}trm_qta_event_callback_data;


/*----------------------------------------------------------------------------
  Idle collision action enum
----------------------------------------------------------------------------*/
typedef enum
{
  /* Fallback for colliding idle tech */
  TRM_IDLE_COLL_ACTION_FALLBACK,

  /* Change in DRx cycle for colliding idle tech */
  TRM_IDLE_COLL_ACTION_DRX_CHANGE,

  TRM_IDLE_COLL_ACTION_MAX

} trm_idle_coll_action_e_type;

/*----------------------------------------------------------------------------
  Fallback tech enum
----------------------------------------------------------------------------*/
typedef enum
{
  /* No fallback */
  TRM_FALLBACK_NONE,

  /* Fallback to GSM */
  TRM_FALLBACK_GSM_PREF,

  /* Fallback to LTE */
  TRM_FALLBACK_LTE_PREF,

  /* Fallback to UMTS */
  TRM_FALLBACK_UMTS_PREF,

  TRM_FALLBACK_MAX

} trm_idle_coll_fallback_enum_t;

/*----------------------------------------------------------------------------
  Structure for fallback information
----------------------------------------------------------------------------*/
typedef struct
{
  /* Fallback tech */
  trm_idle_coll_fallback_enum_t           fallback_type;

  /* 
   * Rat which will have least priority of reselection
   * SYS_SYS_MODE_NONE, no rat
   * SYS_SYS_MODE_WCDMA,
   * SYS_SYS_MODE_LTE,
   */
  sys_sys_mode_e_type                     least_priority_rat;

} trm_idle_coll_fallback_data;

/*----------------------------------------------------------------------------
  Union for types of idle collision action
----------------------------------------------------------------------------*/
typedef union
{
  /* Idle tech fallback info */
  trm_idle_coll_fallback_data             idle_fallback_data;

} trm_idle_coll_action_data_u;

/*----------------------------------------------------------------------------
  Structure for idle collision action information
----------------------------------------------------------------------------*/
typedef struct
{
  /* Type of action
   * TRM_PBR_ACTION_FALLBACK,   Fallback
   * TRM_PBR_ACTION_DRX_CHANGE, DRx change
   */
  trm_idle_coll_action_e_type             action;

  /* ASID for colliding idle tech which will perform the action */
  sys_modem_as_id_e_type                  asid;

} trm_idle_coll_action_info;

/*----------------------------------------------------------------------------
  Idle collison async event info
----------------------------------------------------------------------------*/
typedef struct
{
  /* Information about type of action and involved sub */
  trm_idle_coll_action_info               action_info;

  /* Information about the action to be performed */
  trm_idle_coll_action_data_u             data;

} trm_idle_coll_event_data;

typedef trm_idle_coll_event_data trm_idle_coll_return_data;


/*---------------------------------------------------------------------------- 
  Protocol Stack Status info return data
  ----------------------------------------------------------------------------*/
typedef struct
{
  trm_protocol_stack_mask_type trm_protocol_stack_info[MAX_NUM_OF_SUBSCRIPTIONS];
}trm_protocol_stack_status_data;

typedef enum
{
  TRM_ELS_PERIODIC_GAP = 0,

  TRM_ELS_APERIODIC_GAP = 1,

  TRM_ELS_GAP_MAX_TYPE = 2
}trm_els_gap_enum_t;

typedef enum
{
  TRM_ELS_PAGE = 0,

  TRM_ELS_EXTEND_PAGE = 1,

  TRM_ELS_MEASUREMENT = 2,

  TRM_ELS_PARAM_CHANGE = 3,

  TRM_ELS_RESELECTION = 4,

  TRM_ELS_IRAT = 5,

  TRM_ELS_OOS = 6,

  TRM_ELS_OTHER = 7,

  TRM_ELS_ACTIVITY_MAX_TYPE = 8
}trm_els_activity_enum_t;

typedef enum
{
  TRM_ELS_1X_QPCH = 0,

  TRM_ELS_1X_PCH  = 1, 

  TRM_ELS_1X_OTHER = 2,

  TRM_ELS_SUB_ACTIVITY_MAX_TYPE = 3
}trm_els_sub_activity_enum_t;

/*----------------------------------------------------------------------------
  ELS state info of a client
----------------------------------------------------------------------------*/
typedef struct
{
  /* Specify whether gap is aperiodic or periodic */
  trm_els_gap_enum_t        gap_type; 

  /* RAT group of ELS idle client */
  trm_client_enum_t         client_id;

  /* TRM reason and activity for which lock is held or requested */
  trm_reason_enum_t reason; 
  trm_els_activity_enum_t   activity;
  trm_els_sub_activity_enum_t sub_activity;

  /* Time for which idle ELS client has requested a lock.
     In sclks (should match DDS sub unlock by time)*/
  trm_time_t                gap_start; 

  /* Idle ELS client duration scheduled, in sclks */
  trm_duration_t            duration;

  /* Current idle client DRX cycle for periodic gaps, in milliseconds*/
  uint32                    drx_cycle;

  /* Idle client frame info for periodic gaps */
  trm_time_t                page_frame_start;
  trm_time_t                page_frame_position;

  /* APS registered flag*/
  boolean                   is_aps_registered;
}trm_els_state_info;

/*----------------------------------------------------------------------------
  Information for ELS state change for data and idle techs
----------------------------------------------------------------------------*/
typedef struct
{
    /* Two types of information can be sent in this callback 
      (reusing enum from trm_set_client_state)
      1. ELS on/off notification to idle tech (TRM_ELS_ACTIVATION)
      2. new ELS state update to data tech (TRM_ELS_STATE_UPDATE) 
    */
    trm_els_request_enum_type   els_cb_activity;

    /* Info used for ELS on/off notification from data->idle tech
        1. Data tech ID 
        2. Indication whether DDS has turned ELS On (TRUE) or Off (FALSE)
        3. Number of pages before tech should resend ELS paging info.
           Value of -1 means only send if the ELS info has changed.
    */
    trm_client_enum_t       data_tech;
    boolean                 els_active; 
    int8                    repetition_interval;

    /* ELS state info update idle->data tech
        1. Array to hold els info of all pending idle techs
        2. Number of idle techs with new els info
    */
    trm_els_state_info      idle_tech[TRM_MAX_ELS_IDLE_TECHS];
    uint8                   num_idle_techs;
}trm_els_state_callback_data;


/*----------------------------------------------------------------------------
  Union to return any aysnc call-back information
----------------------------------------------------------------------------*/
typedef union
{
  /* Req and Notify Return Information */
  trm_req_n_not_return_data             chain_req_n_not_info;

  /* Enh Req and Notify Return Information */
  trm_req_n_not_enh_return_data         chain_req_n_not_enh_info;

  /* Modify Chain State Return Information */
  trm_modify_chain_state_return_data    chain_modify_state_info;

  /* Unlock Call-back Return Information */
  trm_unlock_callback_data              unlock_info;

  /* Connected Mode Hop Return Information */
  trm_connected_mode_hop_callback_data  conn_mode_hop_info;

  trm_dual_rec_info_return_data         dual_receive_info;

  /* IRAT lock release info to WLAN */
  trm_irat_event_conflict_data          irat_event_info;

  /* QTA data info for a client */
  trm_qta_event_callback_data           qta_event_info;

  /* ELS feature and state info for client */
  trm_els_state_callback_data           els_event_info;

  /* Idle collison async event info */
  trm_idle_coll_event_data              idle_coll_event_info;

  /*Protocol stack status information*/
  trm_protocol_stack_status_data        protocol_stack_status_info;

} trm_async_callback_data;


typedef struct
{
  trm_client_enum_t               client;
  trm_async_callback_type_enum_t  cb_type;
  trm_async_callback_data         data;
  trm_request_tag_t               tag;
} trm_async_callback_information_type;



/*----------------------------------------------------------------------------
  Callback function for Async events.
----------------------------------------------------------------------------*/
typedef void (* trm_async_event_cb_type)
(
trm_async_callback_information_type* event_data
);

/* rxtx split */
/*----------------------------------------------------------------------------
  Callback data for grouped Async events callback.
----------------------------------------------------------------------------*/
typedef struct
{
  /* number of valid entries */
  uint32                                    num_of_entries;

  /* list of async callback info */
  trm_async_callback_information_type       data[TRM_GROUPED_CLIENTS_MAX];

}trm_grouped_async_callback_information_type;
  
/*----------------------------------------------------------------------------
  Callback function for grouped Async events.
----------------------------------------------------------------------------*/
typedef void (* trm_grouped_async_event_cb_type)
(
  trm_grouped_async_callback_information_type*  event_data
);

/*----------------------------------------------------------------------------
  Modify Duration Input Information
----------------------------------------------------------------------------*/
typedef struct
{
  /* The required sclk extension, from "now" */
  trm_duration_t                  duration;

  /* Flag to mention whether shortening of the duration is fine or not.
     If this flag is true, TRM will shorten the duration if the new duration 
     is less than the already remaining duration of the client. */
  boolean                         okay_to_shorten;

} trm_modify_duration_info;


/*----------------------------------------------------------------------------
  Modify Reason Input Information
----------------------------------------------------------------------------*/
typedef struct
{
  /* The new reason why the RF lock is held (used for priority decisions) */
  trm_reason_enum_t               new_reason;

  /* New activity the RF lock is held for */
  sys_proc_type_e_type           sub_reason;

  /* minimum duration for immediate unlock-by time (in sclks) */
  trm_duration_t                  min_duration;

   /* Unique Identifier to indicate the wakeup */
   uint64                          wakeup_identifier;

} trm_modify_reason_info;

/*----------------------------------------------------------------------------
  Modify Band Input Information
----------------------------------------------------------------------------*/
typedef struct
{
  /* Modify Band Type */
  trm_band_tune_action_type       action;

  /* Band/s that would be used for band request. This is applicable only for
     Band Tune action */
  trm_freq_input_type             new_freq_info;

  /* Duration required to tune and honor the unlock if any */
  trm_duration_t                  duration;

}  trm_modify_band_info;

/*----------------------------------------------------------------------------
  Union for any modify chain owner data operation
----------------------------------------------------------------------------*/
typedef union
{
  /* Input for modify duration */
  trm_modify_duration_info modify_duration;

  /* Input for modify reason */
  trm_modify_reason_info   modify_reason;

  /* Input for modify band */
  trm_modify_band_info     modify_band;

} trm_modify_data;


/*----------------------------------------------------------------------------
  Modify Chain state input information
----------------------------------------------------------------------------*/
typedef struct
{
  /* Client modifying chain state */
  trm_client_enum_t client_id;

  /* Modification Type */
  trm_modify_state_enum_t  modify_type;

  /* Modify Information */
  trm_modify_data   modify_info;

  /* Tag */
  trm_tag_type      tag;

} trm_modify_chain_state_input_info;

/*============================================================================

FUNCTION TRM_MODIFY_CHAIN_STATE

DESCRIPTION
  Allows the client to modify various attributes. For example: Band, duration,
  reason etc.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_modify_chain_state(trm_modify_chain_state_input_info *modify_state_info);

/*----------------------------------------------------------------------------
  Enum for different feature information
----------------------------------------------------------------------------*/
typedef enum
{
  /* Client state None */
  TRM_FEATURE_NONE,

  /* Feature QTA */
  TRM_FEATURE_QTA,

  /* APS */
  TRM_FEATURE_APS,

  /* PBR */
  TRM_FEATURE_PBR,

  /* Device Hopping */
  TRM_FEATURE_HOPPING,

  /* Enum Max */
  TRM_FEATURE_MAX,

} trm_feature_enum_t;


/*----------------------------------------------------------------------------
  Enum for PBR Feature Modification
----------------------------------------------------------------------------*/
typedef enum
{
  /* Modify PBR modes */
  TRM_MODIFY_PBR_MODES,

  /* Modify DRx Cycle */
  TRM_MODIFY_PBR_DRX_CYCLE,

  /* Enum Max */
  TRM_MODIFY_PBR_MAX,

} trm_pbr_modify_enum_t;


/*----------------------------------------------------------------------------
  Structure for PBR Modification
----------------------------------------------------------------------------*/
typedef struct
{

  /* PBR Modification Type */
  trm_pbr_modify_enum_t    type;

  union
  {
    /* New Mode */
    trm_pbr_modes_input_type new_modes;

    /* New DRx Cycle */
    trm_drx_cycle_input_type new_drx_cycle;
  };

} trm_modify_pbr_data;


typedef uint64 trm_aps_modes_input_type;

/*----------------------------------------------------------------------------
  Structure for APS Modification
----------------------------------------------------------------------------*/
typedef struct
{
  trm_aps_modes_input_type reason_mask;
  /* yet to be filled */
} trm_modify_aps_data;

/*----------------------------------------------------------------------------
  Structure for Async cb modification
----------------------------------------------------------------------------*/
typedef struct
{
  /* Async call-back */
  trm_async_event_cb_type    async_cb;

} trm_modify_async_cb_data;

/*----------------------------------------------------------------------------
  trm hopping type.
----------------------------------------------------------------------------*/
typedef enum
{
  /* TRM idle mode hopping type */
  TRM_HOPPING_TYPE_IDLE_MODE,

  /* TRM connected mode hopping type */
  TRM_HOPPING_TYPE_CONNECTED_MODE,

  /* TRM all hopping types */
  TRM_HOPPING_TYPE_ALL

}trm_hopping_type;

/*----------------------------------------------------------------------------
  trm modify hop behavior data
----------------------------------------------------------------------------*/
typedef struct
{
  /* flag to disable/enable hopping */
  boolean   disable_hop;

  /* specifies which type of hopping to disable */
  trm_hopping_type  hop_type;

}trm_modify_hop_behavior_data;

/*---------------------------------------------------------------------------- 
  Bitmask of QTA reasons
------------------------------------------------------------------------------*/
typedef uint64 trm_qta_modes_input_type;

/*----------------------------------------------------------------------------
  Structure for QTA Modification
----------------------------------------------------------------------------*/
typedef struct
{
  trm_qta_modes_input_type qta_reasons;
}trm_modify_qta_data;

/*----------------------------------------------------------------------------
  Union for all modification type
----------------------------------------------------------------------------*/
typedef union
{
  /* PBR Modification Data */
  trm_modify_pbr_data  new_pbr_info;

  /* APS Modification Data */
  trm_modify_aps_data  new_aps_info;

  /* Hopping behavior Data */
  trm_modify_hop_behavior_data new_hop_info;
/* QTA Modification Data */
   trm_modify_qta_data new_qta_info;

} trm_modify_feature_data;

/*----------------------------------------------------------------------------
  Typedef feature modification to feature registration
----------------------------------------------------------------------------*/
typedef trm_modify_feature_data trm_register_feature_data;

/*----------------------------------------------------------------------------
  Input Structure for feature data modification
----------------------------------------------------------------------------*/
typedef struct
{
  /* Client Id */
  trm_client_enum_t        client_id;

  /* Feature to modify */
  trm_feature_enum_t       feature;

  /* Modification Information */
  trm_modify_feature_data  data;

} trm_modify_feature_data_input_info;


/*----------------------------------------------------------------------------
  Input Structure for feature data modification
----------------------------------------------------------------------------*/
typedef struct
{
  /* Client registering for the feature */
  trm_client_enum_t            client_id;

  /* Feature Information */
  trm_feature_enum_t           feature_info;  

  /* Freature registeration Information */
  trm_register_feature_data    register_info;
} trm_register_feature_input_info;

void trm_register_feature(trm_register_feature_input_info *register_info);

/*----------------------------------------------------------------------------
  Feature Deregister Information
----------------------------------------------------------------------------*/
typedef struct
{
  /* Client Id */
  trm_client_enum_t            client_id;

  /* Feature to deregister */
  trm_feature_enum_t           feature_info;  

} trm_deregister_feature_input_info;

void trm_deregister_feature(trm_deregister_feature_input_info* deregister_info);


void trm_modify_feature_info(trm_modify_feature_data_input_info* modify_info);

/*----------------------------------------------------------------------------
  Band Device Mapping Information
----------------------------------------------------------------------------*/
typedef struct
{
  /* Num devices. Number of valid entries in the device array */
  uint32                num_devices;

  /* rfm device info supporting the band requested */
  rfm_device_enum_type     device[TRM_DEVICE_MAPPING_INPUT_SIZE];

  trm_rf_handler_type      rf_device_handle[TRM_DEVICE_MAPPING_INPUT_SIZE];

} trm_band_dev_map_return_data;

/*----------------------------------------------------------------------------
  Simultaneous Capability Information
----------------------------------------------------------------------------*/
typedef uint32 trm_simul_cap_return_data;

/*----------------------------------------------------------------------------
  Client Information
----------------------------------------------------------------------------*/
typedef struct
{
  /* Client Id */
  trm_client_enum_t       client_id;

  /* Device Held */
  rfm_device_enum_type    dev;

  trm_rf_handler_type     rf_handle;

  /* Reason with which device held */
  trm_reason_enum_t       reason;

} trm_client_info_return_data;

/*----------------------------------------------------------------------------
  ASID Mapping Information
----------------------------------------------------------------------------*/
typedef sys_modem_as_id_e_type trm_asid_mapping_return_data;


/*----------------------------------------------------------------------------
  H/W Capability information 
----------------------------------------------------------------------------*/
typedef trm_hw_cap_info   trm_hw_cap_return_data;

/*----------------------------------------------------------------------------
  Device Mapping Information
----------------------------------------------------------------------------*/
typedef struct
{
  /* Device owner information */
  trm_client_enum_t     owners[RFM_MAX_DEVICES];

  /* Device owner reasons information */
  trm_reason_enum_t     owner_reasons[RFM_MAX_DEVICES];

} trm_dev_mapping_return_data;


typedef struct
{
  /* number of devices supporting the band */
  uint8                     num_of_device;

  /* rfm device info supporting the band requested */
  trm_rfm_device_info_type  rfm_device[TRM_MAX_CHAINS];

}trm_device_support_return_data;

/*----------------------------------------------------------------------------
  Enum type to return priority comparison information
----------------------------------------------------------------------------*/
typedef enum
{
  /* If both the priorities are equal */
  TRM_PRIORITY_COMP_PRIORITIES_EQUAL,

  /* If the compared with client priority is lesser */
  TRM_PRIORITY_COMP_PRIORITY_LESSER,

  /* If the compared with client priority is greater */
  TRM_PRIORITY_COMP_PRIORITY_GREATER,

  TRM_PRIORITY_COMP_MAX
} trm_priority_comparison_enum_type;

/*----------------------------------------------------------------------------
  Structure to encapsulate priority comparison information
----------------------------------------------------------------------------*/
typedef struct
{
  trm_priority_comparison_enum_type result;
} trm_priority_comparison_return_data;


/*----------------------------------------------------------------------------
  Structure to encapsulate PBR information
----------------------------------------------------------------------------*/
typedef struct
{
  uint32            drx_cycle; /* drx cycle in milli-seconds */
} trm_pbr_info_return_data;


/*----------------------------------------------------------------------------
  Structure to encapsulate TRM Performance Metric Info
----------------------------------------------------------------------------*/
typedef struct
{
  uint16            trm_metric[TRM_METRIC_MAX];
} trm_metrics_return_data;

/* rxtx split */
/*----------------------------------------------------------------------------
  Enum for connected mode transition Rx device change info 
----------------------------------------------------------------------------*/
typedef enum
{
  /* This indicates that Rx device should be changed to enter connected mode
     Techs will need to put new Request for both Tx and Rx devices */
  TRM_CONN_MODE_RX_DEV_CHANGE,

  /* This indicates that Rx device that tech is currently holding can be used
     to transition to connected mode, Techs can simply put request for tx dev */
  TRM_CONN_MODE_RX_DEV_CURRENT

}trm_conn_mode_rx_dev_info_enum_type;

/*----------------------------------------------------------------------------
  Structure to return connected mode transition Rx device change info 
----------------------------------------------------------------------------*/
typedef struct
{
  trm_conn_mode_rx_dev_info_enum_type  info;

}trm_conn_mode_rx_dev_return_data;/* rxtx split */

/*----------------------------------------------------------------------------
  Structure to return RF configuration info
----------------------------------------------------------------------------*/
typedef struct
{
  uint32    rf_hwid;
}trm_rf_info_return_data;

/*----------------------------------------------------------------------------
  Structure to return if DTA band combo exist with any active tech 
----------------------------------------------------------------------------*/
typedef struct
{

  boolean   is_div_sharing_case;

} trm_div_sharing_return_data;

/*----------------------------------------------------------------------------
  Structure to return if a RAT is connected mode or 
  having a pending RnN for connected mode
----------------------------------------------------------------------------*/
typedef struct
{

  uint32   rat_bit_mask;

} trm_rat_access_return_data;

/*----------------------------------------------------------------------------
 Structure for QTA proceed check return data
----------------------------------------------------------------------------*/
typedef struct
{
  /* indicates if data tech can proceed with QTA */
  boolean   proceed_with_qta;

} trm_qta_proceed_check_return_data;

typedef struct
{
  /* for different bit values, check macros :
     TRM_DSDS_MODE_BIT
     TRM_SRLTE_MODE_BIT
     TRM_DUAL_MULTI_MODE_BIT

     so for checking SRLTE, do (mask & TRM_SRLTE_MODE_BIT) ? TRUE : FALSE;
  */
  uint8  mask;
}trm_upper_layer_mode_info;

/*----------------------------------------------------------------------------
  Structure to return if a RAT has voted for
  APS_REQUESTED
----------------------------------------------------------------------------*/
typedef struct
{

  uint32 aps_req_bit_mask;

} trm_aps_requested_return_data;

/*----------------------------------------------------------------------------
  Union for all get request output information
----------------------------------------------------------------------------*/
typedef union
{
  /* Band Device Map Output Information */
  trm_band_dev_map_return_data band_mapping_info;

  /* Simultaneous Capability Output Information */
  trm_simul_cap_return_data    simul_cap_info;

  /* Client Information Output Information */
  trm_client_info_return_data  client_info;

  /* Hardware Capability Output Information */
  trm_hw_cap_return_data       hw_cap_info;

  /* ASID Mapping Output Information */
  trm_asid_mapping_return_data asid_mapping_info;

  /* Device Mapping Output Information */
  trm_dev_mapping_return_data  dev_mapping_info;

  /* Dual Receive Output Information */
  trm_dual_rec_info_return_data  dual_rec_info;

  trm_device_support_return_data  dev_support_info;

  /* priority comparison return information */
  trm_priority_comparison_return_data  prio_comp_info;

  /* PBR information */
  trm_pbr_info_return_data             pbr_info;

  /* single tx dsda information */
  trm_single_tx_dsda_return_data       single_tx_info;

  /* TRM Performance Metrics Information */
  trm_metrics_return_data              metrics_info;

  trm_div_sharing_return_data          div_sharing_info;

  /* rxtx split */
  /* Return data for techs querying for Rx device change possibility when
     transitioning to connected mode */
  trm_conn_mode_rx_dev_return_data     rx_dev_info;

  /* Information shared by RF to TRM */
  trm_rf_info_return_data              rf_info;

  trm_rat_access_return_data           rat_access;

  trm_qta_proceed_check_return_data    qta_proceed_info;

  /* Send the upper layer mode information */
  trm_upper_layer_mode_info             ul_mode_info;
  
  /* Send the idle tech ELS state information */
  trm_els_state_info                    els_state_info;

  /* Information about idle collison actions */
  trm_idle_coll_return_data             idle_coll_info;

   /*APS info return data*/
  trm_aps_requested_return_data         aps_info;


} trm_get_info_return_data;


/*----------------------------------------------------------------------------
  Enum for get information
----------------------------------------------------------------------------*/
typedef enum
{
  /* Device Information */
  TRM_DEVICE_ALLOCATION_INFO, 

  /* Device for a given set of bands */
  TRM_DEVICE_MAPPING_INFO,

  TRM_DEVICE_SUPPORT_INFO,

  /* Client Information */
  TRM_CLIENT_INFO,

  /* Hardware Capability Information */
  TRM_HW_CAPABILITY,

  /* Simultaneous Capability Information */
  TRM_SIMUL_CAPABILITY,

  /* ASID Mapping Information */
  TRM_ASID_MAPPING,

  /* Dual Receive Capability Information */
  TRM_DR_CAPABILITY,

  /* Priority comparison */
  TRM_PRIORITY_COMPARISION,

 /* Tx sharing DSDA information */
  TRM_SINGLE_TX_CAPABILITY,

  /* PBR info */
  TRM_PBR_INFO,

  /* Performance Metrics */
  TRM_PERF_METRICS,

  /* Diversity Sharing case */
  TRM_DIVERSITY_SHARING_INFO,

  /* rxtx split */
  TRM_CONN_MODE_RX_DEV_INFO,

  /* RF information shared to TRM */
  TRM_RF_CONFIG_INFO,

  /* Get RATs that are in connected state */
  TRM_RAT_ACCESS_INFO,

  /* Check if QTA can be started */
  TRM_QTA_PROCEED_CHECK_INFO,

/* To query upper layer mode information... TRM is pass through */
   TRM_UPPER_LAYER_MODE_INFO,

  /* To query particular idle techs ELS state */
  TRM_ELS_INFO,

  /* Get info about idle collsion actions, GSM fallback or DRx change */
  TRM_IDLE_COLL_INFO,

   /*aps info*/
   TRM_APS_INFO,

  /* Enum Max */
  TRM_GET_INFO_MAX,

} trm_get_info_type;


/*----------------------------------------------------------------------------
  Get Information Output Type
----------------------------------------------------------------------------*/
typedef struct
{
  trm_get_info_type         info_type;

  trm_get_info_return_data  info;

} trm_get_info_return_type;


/*----------------------------------------------------------------------------
  Struct input for band mapping information
----------------------------------------------------------------------------*/
typedef struct
{
  /* resource type being requested */
  trm_resource_enum_t   resource;

  /* Number of bands */
  uint32 num_bands;

  /* Band information */
  trm_band_t            band[TRM_DEVICE_MAPPING_INPUT_SIZE];

}trm_get_band_mapping_input_info;

/*----------------------------------------------------------------------------
  Structure input for client information
----------------------------------------------------------------------------*/
typedef struct
{
  /* client whose information is requested */
  trm_client_enum_t     requested_client_id;

} trm_get_client_input_info;

/*----------------------------------------------------------------------------
  Structure input for ASID mapping query
----------------------------------------------------------------------------*/
typedef struct
{
  /* client whose information is requested */
  trm_client_enum_t     requested_client_id;
} trm_get_as_id_mapping_info;

/*----------------------------------------------------------------------------
  trm Rfm Device Support Input Type.
----------------------------------------------------------------------------*/
typedef struct
{
  /* band info */
  trm_band_t                band;

}trm_get_device_support_input_info;


/*----------------------------------------------------------------------------
  trm priority comparison input info.
----------------------------------------------------------------------------*/
typedef struct
{
  trm_client_enum_t         compared_with_client;
  trm_reason_enum_t         compared_with_reason;
  trm_client_enum_t         compared_client;
  trm_reason_enum_t         compared_reason;
} trm_priority_comparison_input_info;

typedef trm_get_client_input_info trm_pbr_input_info;

/* rxtx split */
/*----------------------------------------------------------------------------
  trm connected mode rx dev check input for band info
----------------------------------------------------------------------------*/
typedef struct
{
  trm_freq_input_type       freq_info;
}trm_conn_mode_rx_dev_input_info;

/*----------------------------------------------------------------------------
  Structure to query if a RAT is connected mode or 
  having a pending RnN for connected mode
----------------------------------------------------------------------------*/

typedef struct
{
  trm_client_enum_t   client_id;
}trm_rat_access_input_info;


typedef struct
{
  trm_client_enum_t   idle_tech;

}trm_qta_proceed_check_input_info;

typedef struct
{
  trm_client_enum_t   idle_tech;
}trm_els_query_info; 

/*----------------------------------------------------------------------------
  Union for get information input
----------------------------------------------------------------------------*/
typedef union
{
  /* band mapping input information */
  trm_get_band_mapping_input_info    band_mapping_info;

  /* client information input */
  trm_get_client_input_info          client_info;

  /* Device support query input */
  trm_get_device_support_input_info  dev_support_info;

  /* Get AS_ID mapping input */
  trm_get_as_id_mapping_info         as_id_mapping_info;

  /* priority comparison input info */
  trm_priority_comparison_input_info prio_comp_info;

  /* PBR input info */
  trm_pbr_input_info                 pbr_info;

  /* connected mode transition rx dev check input info */
  trm_conn_mode_rx_dev_input_info    conn_mode_info;

  /* Query for RATs that are in connected mode */
  trm_rat_access_input_info          rat_access;

  /* Query for checking if QTA can be proceeded with */
  trm_qta_proceed_check_input_info   qta_proceed_check_info;

  /* Query to get idle tech ELS info */
  trm_els_query_info					  els_info;

  /* Query for getting status of idle collison actions */
  trm_idle_coll_action_info             idle_coll_info;

} trm_get_info_data;


/*----------------------------------------------------------------------------
  Get API input structure
----------------------------------------------------------------------------*/
typedef struct
{
  trm_client_enum_t  client_id;

  trm_get_info_type  info_type;

  trm_get_info_data  input_data;

} trm_get_info_input_type;

void trm_get_info(trm_get_info_input_type*, trm_get_info_return_type*);


/*----------------------------------------------------------------------------
  IRAT Identifier
----------------------------------------------------------------------------*/
typedef uint32  trm_irat_identifier;

/*----------------------------------------------------------------------------
  IRAT Operation Input Data
----------------------------------------------------------------------------*/
typedef struct
{
  /* Operation */
  trm_state_oper_enum_type  operation;

  /* Measured Client: Needed only for Start Operation */
  trm_client_enum_t         measured_client;

  /* Frequency Input: Needed only for Start Operation */
  trm_freq_input_type       freq_info;

  /* IRAT Identifier for start or end of operation */
  trm_irat_identifier       transaction_id;

  /* Duration: Needed only for Start Operation */
  trm_time_t                duration;

  /* Sync Result: Set to TRUE for now on DR-DSDS. Might be needed  */
  boolean                   sync_result;

  /* Indicates if diversity chain needs to allocated for IRAT or not  */
  boolean                   is_diversity_needed;

  /* Indicates if the reason should be changed to IRAT  */
  boolean                   should_change_reason_to_irat;

} trm_irat_data_type;


/*----------------------------------------------------------------------------
  Typedef IRAT Data Type to IFS Data Type.
----------------------------------------------------------------------------*/
typedef trm_irat_data_type trm_ifs_data_type;


typedef enum
{
  TRM_APS_REQUESTED,

  TRM_APS_SUPPORTED,

} trm_aps_enum_type;

typedef struct
{
  /* Start or End */
  trm_state_oper_enum_type  operation;

  /* applicable for APS End */
  trm_aps_enum_type aps_type;

} trm_aps_data_type;


/*----------------------------------------------------------------------------
  enum TRM_APS_MODE_ENUM_TYPE
----------------------------------------------------------------------------*/
typedef enum 
{
  /*EMBMS mode where for L and W both needs to conidered for idle client for APS*/
  TRM_TECH_MODE_EMBMS,
  
  TRM_TECH_MODE_MAX
  
}trm_tech_mode_enum_type;

/*----------------------------------------------------------------------------
  structure TRM_TECH_MODE_STATE_TYPE
----------------------------------------------------------------------------*/
typedef struct
{
  /* Start or End */
  trm_state_oper_enum_type  operation;

  /* bitmask of trm_aps_mode_enum_type 
     should be set with both start operation and end operation 
     to indicate which mode/modes are started/ended */
  uint32 tech_mode_bitmask;
   
}trm_tech_mode_state_type;

typedef struct
{
  /* Start or End */
  trm_state_oper_enum_type  operation;

  /* Number of device to be protected */
  uint8                     num_dev_protect;
  
  /* This is an array of devices which will be protected by TRM during QTA */
  rfm_device_enum_type      dev_protect[TRM_MAX_DEVICE_TECH_HOLD];

  trm_rf_handler_type      rf_device_handle_protect[TRM_DEVICE_MAPPING_INPUT_SIZE];

  /* client with which gap is opened or closed */
  trm_client_enum_t         target_client;

  /* RF Source tech data for Script Building */
  rfm_meas_common_param_type* rf_params;

}trm_qta_data_type;

typedef struct
{
  /* State voted by the client */
  trm_dual_receive_enum_type dr_vote;

} trm_dr_data_type;



typedef enum
{
   TRM_LTED_ACTIVE,

   TRM_LTED_INACTIVE

} trm_lted_state_type;



typedef struct
{
  trm_lted_state_type  lted_state;

} trm_lted_state_data_type;

/*----------------------------------------------------------------------------
  Structure to input info for ELS State update
----------------------------------------------------------------------------*/
typedef struct
{
   trm_els_request_enum_type      request;

   /* Operation - only relevant for feature registration/activation request types */
   trm_state_oper_enum_type  operation;

   /* Repetition interval DDS passes TRM (not used in initial bring-up, hardcoded to 1) */
   int8                         repetition_interval;

   /* New state info - only relevant for idle tech */
   trm_els_state_info        new_els_state;
}trm_els_data_type;

/*----------------------------------------------------------------------------
  Union for all set state input data
----------------------------------------------------------------------------*/
typedef union
{
  trm_irat_data_type   irat_data;
  trm_ifs_data_type    ifs_data;
  trm_qta_data_type    qta_data;
  trm_aps_data_type    aps_data;
  trm_dr_data_type     dr_data;
  trm_lted_state_data_type      lted_active_state_data;
  trm_els_data_type            els_data; 
   
   /* tech mode setting */
  trm_tech_mode_state_type      tech_mode_data;
    
} trm_client_state_data;


/*----------------------------------------------------------------------------
  Input Structure for set client state
----------------------------------------------------------------------------*/
typedef struct
{
  /* Client doing the set state*/
  trm_client_enum_t           client_id;

  /* Type of state being set */
  trm_client_state_enum_type  client_state;

  /* Information for set state */
  trm_client_state_data       data;

} trm_set_client_state_input_type;


typedef enum
{
  /* IRAT is denied because there is another concurrent IRAT operation */
  TRM_IRAT_DENIAL_CONCURRENT_IRAT,

  /* IRAT is denied because of concurrency limitation.
     Example: Diversity is needed for IRAT but band is in DTA mode
     with concurrent tech */
  TRM_IRAT_DENIAL_CONCURRENCY_CONFLICT,

  /* IRAT is denied because target device is occupied. */
  TRM_IRAT_DENIAL_DEVICE_CONFLICT,

} trm_irat_denial_enum_type;

typedef struct
{
  /* Band information */
  trm_band_t            band;

  trm_grant_return_enum_type  grant;

  /* This field is valid only if grant field is TRM_DENIAL */
  trm_irat_denial_enum_type  denial_info;

  /* Primary Device */
  rfm_device_enum_type  primary_device;

  trm_rf_handler_type   primary_handle;

  /* Diversity Device */
  rfm_device_enum_type  diversity_device;

  trm_rf_handler_type   diversity_handle;

} trm_set_irat_band_device_info;

/*----------------------------------------------------------------------------
  Input Structure for set client state
----------------------------------------------------------------------------*/
typedef struct
{
  /* Band information */
  trm_band_t                      band[TRM_DEVICE_MAPPING_INPUT_SIZE];

  /* Device type */
  rfm_device_enum_type            device[TRM_DEVICE_MAPPING_INPUT_SIZE];

  trm_rf_handler_type             rf_handle[TRM_DEVICE_MAPPING_INPUT_SIZE];

  /* Grant info for each band */
  trm_set_irat_band_device_info   band_dev_info[TRM_DEVICE_MAPPING_INPUT_SIZE];

} trm_set_irat_return_data;

/*----------------------------------------------------------------------------
  Define IRAT return to IFS return
----------------------------------------------------------------------------*/
typedef trm_set_irat_return_data  trm_set_ifs_return_data;




/*----------------------------------------------------------------------------
  Union for all set state result
----------------------------------------------------------------------------*/
typedef union
{
  /* IRAT return data */
  trm_set_irat_return_data     irat_result;

  /* IFS return data */
  trm_set_ifs_return_data      ifs_result;

  /* QTA return data ONLY in END_QTA State */
  trm_qta_return_data_type     qta_result;

} trm_set_state_result;


/*----------------------------------------------------------------------------
  Output structure for set state
----------------------------------------------------------------------------*/
typedef struct
{
  /* Set state type */
  trm_client_state_enum_type  set_state_type;

  /* Operation requested */
  trm_state_oper_enum_type    state_oper;

  /* Transaction Id */
  trm_irat_identifier         transaction_id;

  /* The overall result */
  trm_grant_return_enum_type  grant;

  /* Operation result */
  trm_set_state_result        result;

} trm_set_client_state_output_data;

void trm_set_client_state(trm_set_client_state_input_type*, trm_set_client_state_output_data*);


/*----------------------------------------------------------------------------
  Enum for different async operations
----------------------------------------------------------------------------*/
typedef enum
{
  TRM_ASYNC_CB_REGISTER,

  TRM_ASYNC_CB_MODIFY,

  TRM_ASYNC_CB_DEREGISTER,

  /* rxtx split */
  /* Registration for grouped async callbacks */
  TRM_ASYNC_GROUPED_CB_REGISTER,

  TRM_ASYNC_GROUPED_CB_MODIFY,

  TRM_ASYNC_GROUPED_CB_DEREGISTER,

  TRM_ASYNC_MIXED_CB_REGISTER,

  TRM_ASYNC_MIXED_CB_MODIFY,

  TRM_ASYNC_MIXED_CB_DEREGISTER,

  TRM_ASYNC_CB_MAX,

} trm_async_operation_enum_type;

/*----------------------------------------------------------------------------
  Enum for optional async event bitmask
----------------------------------------------------------------------------*/
typedef enum
{
  TRM_AYSNC_DR_INFO_REQ       = 1,

  TRM_AYSNC_CONN_HOP_INFO_REQ = 2,

  /* optional async callback
      WLAN subscribes to this to know if unlock was due to conflicting IRAT event */
  TRM_ASYNC_IRAT_EVENT_INFO = 4,

  /* optional idle tech fallback event */
  TRM_ASYNC_IDLE_COLL_EVENT_INFO  = 8,
    
  /* optional protocol stack status info event*/
  TRM_ASYNC_PROTOCOL_STACK_STATUS_INFO = 0x10, 

  TRM_AYSNC_MAX_INFO_REQ

} trm_async_optional_event_bitmask;

/*----------------------------------------------------------------------------
  Data associated with async call-back registration
----------------------------------------------------------------------------*/
typedef struct
{
  /* Client identifier */
  trm_client_enum_t             client_id;

  /* Bitmask of optional events the client is registering for.
     Bit 0: DR-DSDS <--> DSDS notification. 
     Techs can use trm_async_optional_event_bitmask for bit shifting */
  uint32                        events_bitmask;

  /* Function pointer of the function to be called to nofify the
     client of the event */
  trm_async_event_cb_type       callback_ptr;
  
} trm_modify_async_event_cb_data_type;

typedef trm_modify_async_event_cb_data_type trm_register_async_event_cb_data_type;

/* rxtx split */
/*----------------------------------------------------------------------------
  Data associated with grouped async call-back registration/modification
----------------------------------------------------------------------------*/
typedef struct
{
  /* number of clients being grouped */
  uint32                        num_of_clients;

  /* list of Client identifiers to be grouped,
     Techs should provide the list of clients for which they intend to
     receive a single async callback for same type */
  trm_client_enum_t             client_ids[TRM_GROUPED_CLIENTS_MAX];

  /* Bitmask of optional events the client is registering for.
     Bit 0: DR-DSDS <--> DSDS notification. 
     Techs can use trm_async_optional_event_bitmask for bit shifting */
  uint32                        events_bitmask;

  /* Function pointer of the function to be called to nofify the
     clients of the event. For same callback type, all techs from the group
     will be informed with a single async callback.For different types of
     async callback, separate callback will be done */
  trm_grouped_async_event_cb_type callback_ptr;
  
} trm_grouped_modify_async_event_cb_data_type;

typedef trm_grouped_modify_async_event_cb_data_type trm_grouped_register_async_event_cb_data_type;

/*----------------------------------------------------------------------------
  Union for data passed for different async call-back handling operations
----------------------------------------------------------------------------*/
typedef union
{
  /* Info for registration */
  trm_register_async_event_cb_data_type reg_info;

  /* Info for modification */
  trm_modify_async_event_cb_data_type   modify_info;


  /* following to be used for registering/modifying for group callback */
  trm_grouped_register_async_event_cb_data_type       grouped_reg_info;

  trm_grouped_modify_async_event_cb_data_type         grouped_modify_info;

} trm_async_operation_input_type;

/*----------------------------------------------------------------------------
  Data type passed as input for async cb operations
----------------------------------------------------------------------------*/
typedef struct
{
  /* Identifier of the client registering for the event callbacks */
  trm_client_enum_t                client_id;

  /* Operation */
  trm_async_operation_enum_type   operation;

  /* Data associated with operation(not valid for deregister) */
  trm_async_operation_input_type        data;

} trm_async_event_cb_input_type;

/*============================================================================
FUNCTION TRM_HANDLE_ASYNC_EVENT_CB

DESCRIPTION
  This function registers an asynchronous event callback function with TRM.
  TRM will call the callback function when a particular even occurs.

DEPENDENCIES
  TRM should be intialized

RETURN VALUE
  TRUE when successful.
  FALSE when failed.

SIDE EFFECTS
  None
============================================================================*/

void trm_handle_async_event_cb
(
trm_async_event_cb_input_type* input
);

/*============================================================================
CALLBACK TRM_ASYNC_RESEPONSE_TYPE

DESCRIPTION
  The prototype for callback functions, used by the Transceiver manager
  to inform the clients of a particular event.

DEPENDENCIES
  The callback will be called by the Transceiver Manager.  It may be called
  from a task context of another client, or from interrupt context.

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_async_event_response
(
/* Currently only connected mode hop needs this response */
trm_async_event_response_data_type* resp_data
);


/*============================================================================

FUNCTION TRM_GET_CLIENT_PRIORITY

DESCRIPTION
  Gets the client priority.
  Client priority is used in case of multiple subscriptions.
  
DEPENDENCIES
  None

RETURN VALUE
  trm_client_priority_enum_t

SIDE EFFECTS
 None.  

============================================================================*/

trm_client_priority_enum_t trm_get_client_priority
(

/* client_id */
trm_client_enum_t                client_id

);  /* trm_get_client_priority */

/*============================================================================
TRM_ASDIV_UPDATE_TECH_STATE_INFO

DESCRIPTION
  API called by LIMITSMGR to update the tech state info

DEPENDENCIES
 None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_asdiv_update_tech_state_info(trm_asdiv_tech_state_info_type* state_info);

/*============================================================================

FUNCTION TRM_ASDIV_IS_ANTENNA_SWITCH_SUPPORTED

DESCRIPTION
  Indicate if RF HW supports antenna switching or not
  
DEPENDENCIES
  None

RETURN VALUE
  True  : if HW supports antenna switching. 
  False : otherwise. 

SIDE EFFECTS
  None

============================================================================*/
boolean trm_asdiv_is_antenna_switch_supported(void);





/*============================================================================

CALLBACK TRM_GRANT_CALLBACK_TYPE

DESCRIPTION
  The prototype for event callback functions, used by the Transceiver manager
  to inform the clients of transceiver management events.
  
DEPENDENCIES
  The callback will be called by the Transceiver Manager.  It may be called
  from a task context of another client, or from interrupt context.

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

typedef void (*trm_grant_callback_type)
(
/* The client which is being informed of an event */
trm_client_enum_t               client,

/* The event being sent to the client */
trm_chain_grant_return_type     grant,

/* Anonymous payload echoed from trm_request_and_notify() */
trm_request_tag_t               tag
);


/*----------------------------------------------------------------------------
  Data for unlock callback advanced. To be used with trm_retain_lock_advanced.
----------------------------------------------------------------------------*/
typedef struct 
{
  /* Client Id of the client being 
     asked to unlock the chain */
  trm_client_enum_t       unlock_client;

  /* The event being sent to the client */
  trm_unlock_event_enum_t event;

  /* Sclk timestamp for TRM_UNLOCK_BY */
  uint32                  unlock_by_sclk;

  /* Information about the winning client.
     Higher priority client who's request is 
     causing the unlock notification */
  trm_client_info_t       winning_client;
} trm_unlock_cb_advanced_data_t;


/*============================================================================

CALLBACK TRM_GRANT_ENH_CALLBACK_TYPE

DESCRIPTION
  The prototype for event callback functions, used by the Transceiver manager
  to inform the clients of transceiver management events.
  
DEPENDENCIES
  The callback will be called by the Transceiver Manager.  It may be called
  from a task context of another client, or from interrupt context.

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

typedef void (*trm_grant_enh_callback_type)
(
/* The client which is being informed of an event */
trm_client_enum_t               client,

/* The event being sent to the client */
trm_chain_grant_return_type           grant,

/* Anonymous payload echoed from trm_request_and_notify() */
trm_request_tag_t               tag,

/* Duration by which the lock is extended */
trm_duration_t                  duration

);


/*----------------------------------------------------------------------------
  Enum for set info
----------------------------------------------------------------------------*/
typedef enum
{
  /* Set the device preference information */
  TRM_DEVICE_PREFERENCE,

  TRM_SET_INFO_MAX,

} trm_set_info_type;

/*----------------------------------------------------------------------------
  Enum for device affinity
----------------------------------------------------------------------------*/
typedef enum
{
  /* Highest affinity towards the device */
  TRM_DEVICE_AFFINITY_HIGH,

  /* No specific affinity. Init state */
  TRM_DEVICE_AFFINITY_NORMAL,

  /* Affinity is low due to poor performance */
  TRM_DEVICE_AFFINITY_LOW,

} trm_device_affinity_enum_type;


/*----------------------------------------------------------------------------
  Device Preference Input Type for set info.
----------------------------------------------------------------------------*/
typedef struct
{
  /* client for which the device preference is being set*/
  trm_client_enum_t                client;

  /* Device for which the preference is being set */
  rfm_device_enum_type             device;

  trm_rf_handler_type             rf_handle;

  /* affinity level */
  trm_device_affinity_enum_type    affinity;
  
} trm_set_info_dev_pref_type;


#define TRM_MAX_DEV_AFFINITY_INPUT 6

/*----------------------------------------------------------------------------
  Device Preference Input Type for set info.
----------------------------------------------------------------------------*/
typedef struct
{
  /* Number of Preferences */
  uint32                           num_preferences;

  /* Preferences */
  trm_set_info_dev_pref_type       preferences[TRM_MAX_DEV_AFFINITY_INPUT];
  
} trm_set_info_dev_pref_input;


/*----------------------------------------------------------------------------
  Union for all set info types
----------------------------------------------------------------------------*/
typedef union
{
  /* Device Preference Information */
  trm_set_info_dev_pref_input dev_pref_info;
}trm_set_info_input_data;


/*----------------------------------------------------------------------------
  Input information for set info.
----------------------------------------------------------------------------*/
typedef struct
{
  /* Set info type */
  trm_set_info_type  info_type;
  
  /* Input data related to the set info type */
  trm_set_info_input_data  input_data;

}trm_set_info_input_type;

/*----------------------------------------------------------------------------
  Output information for set info.
----------------------------------------------------------------------------*/
typedef struct
{
  /* Set Info type */
  trm_set_info_type  info_type;

  /* Result of Set info */
  trm_grant_return_enum_type  set_info_result;
  
  /* there is no output info at this point. When we need to add output data;
     we should define a new union */

} trm_set_info_output_type;

/*============================================================================

FUNCTION TRM_SET_INFO

DESCRIPTION
   Used to set certain client related information. 
 
DEPENDENCIES
  None

RETURN VALUE
  Set Info Result.

SIDE EFFECTS
  None.

============================================================================*/
void trm_set_info(
   trm_set_info_input_type *input, /* input info */
   trm_set_info_output_type *output /* output info */
);

/*! Grouped APIs: rxtx split */

/*----------------------------------------------------------------------------
  Data for grouped reserve at call
----------------------------------------------------------------------------*/
typedef struct
{
  /* Number of entries in the list */
  uint32                        num_of_entries;

  trm_reserve_at_input_info     reserve_at_info[TRM_GROUPED_CLIENTS_MAX];

}trm_grouped_reserve_at_input_info;


/*============================================================================

FUNCTION TRM_GROUPED_RESERVE_AT

DESCRIPTION
  Specifies the given client needs the given transceiver resource at the given
  time, for the given duration, for the supplied reason.

  This would be mostly used with the "trm_request()" function.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  If the client currently holds an RF chain, the chain is released.

============================================================================*/
void trm_grouped_reserve_at
(
  /* Input Information for reserve at */ 
  trm_grouped_reserve_at_input_info* input_info
);

/*----------------------------------------------------------------------------
  Input information for Grouped TRM Requests
----------------------------------------------------------------------------*/
typedef struct 
{
  /* number of valid requests in the list */
  uint32                        num_of_entries;

  /* list of request attributes */
  trm_request_input_info        request_info[TRM_GROUPED_CLIENTS_MAX];

}trm_grouped_request_input_info;

/*----------------------------------------------------------------------------
  Return Info for grouped TRM Requests
----------------------------------------------------------------------------*/
typedef struct
{
  trm_client_enum_t           client_id;
  trm_grant_return_enum_type  grant;
  uint32                      num_bands;
  trm_band_dev_alloc_type     band_alloc[MAX_BAND_INFO];
  rfm_tune_information        tune_info;
  trm_tag_type                tag;

} trm_grouped_request_return_info;
/*----------------------------------------------------------------------------
  Return Type for grouped TRM Request
----------------------------------------------------------------------------*/
typedef struct
{
  /* number of entries */
  uint32                              num_of_entries;

  /* return info for multiple */
  trm_grouped_request_return_info     return_info[TRM_GROUPED_CLIENTS_MAX];
 
} trm_grouped_request_return_data;

/*============================================================================

FUNCTION TRM_GROUPED_REQUEST

DESCRIPTION
  Specifies the given client needs the given transciever resource, for the
  given duration, for the supplied reason.

  The resource request is immediately evaluated, and the result returned.

  This may be used in conjunction with trm_reserve_at(). The band information
  is used to check if the band is valid and also if components mapped to the
  band don't have any concurrency issues.
  
DEPENDENCIES
  None

RETURN VALUE
  Chain Grant Information.

SIDE EFFECTS
  If the client currently holds an RF chain, that chain is released before
  the request is evaluated.

============================================================================*/
void trm_grouped_request
(
  /* Input Structure for Request */
  trm_grouped_request_input_info  *input_info,

  /* Output Structure from Request Processing */
  trm_grouped_request_return_data *ret_data
);

/*----------------------------------------------------------------------------
  Input for grouped Request and Notify
----------------------------------------------------------------------------*/
typedef trm_grouped_request_input_info      trm_grouped_request_notify_input_info;

/*============================================================================

FUNCTION TRM_GROUPED_REQUEST_AND_NOTIFY

DESCRIPTION
  Specifies the given client needs the given transceiver resource, for the
  given duration, for the supplied reason.

  When the resource can be granted to the client, the event callback for the
  client will be called with the result of the lock request.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  If the client currently holds an RF chain, the chain is released.

============================================================================*/
void trm_grouped_request_and_notify
(
  /* Input Information for Request and Notify */
  trm_grouped_request_notify_input_info *input_info
);

/*----------------------------------------------------------------------------
 Input data type for grouped request and notify enhanced
----------------------------------------------------------------------------*/
typedef struct
{
  /* number of valid entries */
  uint32                                    num_of_entries;

  /* list for req and notify info */
  trm_request_notify_enh_input_info         req_not_info[TRM_GROUPED_CLIENTS_MAX];

}trm_grouped_request_notify_enh_input_info;

/*============================================================================

FUNCTION TRM_GROUPED_REQUEST_AND_NOTIFY_ENHANCED

DESCRIPTION
  Specifies the given client needs the given transceiver resource, for a 
  minimum of the given duration, for the supplied reason. At the time of the
  grant, the duration is extended to the maximium possible time that the lock
  can be made available to the client without colliding with any other client.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  If the client currently holds an RF chain, the chain is released.
  Also the time value returned via a grant_enh_callback is subject to change
  depending on the nature and timing of other requests from other clients
  after this function call.

============================================================================*/
void trm_grouped_request_and_notify_enhanced
(
/* Input Information for Request and Notify Enh */
trm_grouped_request_notify_enh_input_info *input_info
);

/*----------------------------------------------------------------------------
  Input Information for grouped Retain Lock
----------------------------------------------------------------------------*/
typedef struct 
{
  /* number of valid entries */
  uint32                                num_of_entries;

  /* list for retain lock info */
  trm_retain_chain_lock_input_info      retain_info[TRM_GROUPED_CLIENTS_MAX];

} trm_grouped_retain_lock_input_info;

/*============================================================================
FUNCTION TRM_GROUPED_RETAIN_LOCK

DESCRIPTION
  Indicates that the client wants to hold the chain indefinitely - as long as
  there is no other client requesting the chain with higher priority.
  
DEPENDENCIES
  NONE

RETURN VALUE
  None

SIDE EFFECTS
  Unlock call-back could be called to release the chain immediately or in the
  future.

============================================================================*/
void trm_grouped_retain_lock
(
 trm_grouped_retain_lock_input_info     *input_info
);

/*----------------------------------------------------------------------------
  Input Information to grouped chain release
----------------------------------------------------------------------------*/
typedef struct
{
  /* number of valid entries */
  uint32                                  num_of_entries;

  /* release info */
  trm_release_chain_input_info            rel_info[TRM_GROUPED_CLIENTS_MAX];

} trm_grouped_release_input_type;

 /*============================================================================
FUNCTION TRM_GROUPED_RELEASE

DESCRIPTION
  Releases the chain held by a client.
  
DEPENDENCIES
  NONE

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_grouped_release
(
  trm_grouped_release_input_type  *input_info
);

/*----------------------------------------------------------------------------
  Grouped modify Chain state input information
----------------------------------------------------------------------------*/
typedef struct
{
  /* Client modifying chain state */
  trm_client_enum_t client_id;

  /* Modify Information */
  trm_modify_data   modify_data;

  /* Tag */
  trm_tag_type      tag;

}trm_grouped_modify_state_data;

/*----------------------------------------------------------------------------
  Modify Grouped state input information
----------------------------------------------------------------------------*/
typedef struct
{
  /* number of entries */
  uint32                          num_of_entries;
 
  /* Modification Type:
     Currently TRM_MODIFY_BAND is only supported type */
  trm_modify_state_enum_t         modify_type;

  /* List of Modify Information */
  trm_grouped_modify_state_data   modify_info[TRM_GROUPED_CLIENTS_MAX];

} trm_grouped_modify_state_input_info;

/*============================================================================

FUNCTION TRM_GROUPED_MODIFY_CHAIN_STATE

DESCRIPTION
  Allows the client to modify various attributes for multiple clients.
  Currently this API only supports combined call for modify band.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_grouped_modify_chain_state(trm_grouped_modify_state_input_info *input_info);

/*============================================================================
CALLBACK TRM_GROUPED_ASYNC_RESEPONSE_TYPE

DESCRIPTION
  This API is called by techs as a response of async callback they got
  Currently, only Connected mode hopping request expects a response all
  other async callbacks have no response 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_grouped_async_event_response
(
  /* Currently only connected mode hop needs this response */
  trm_grouped_async_event_response_data_type* resp_data
);

/* combined API/SRx/Device hopping changes */
#define TRM_SRX_MAX_GROUPS             5
#define TRM_SRX_MAX_GROUP_ENTRY        10
#define TRM_SRX_MAX_TECHS_GROUP_ENTRY  2

/*----------------------------------------------------------------------------
  SRx Group Entry Type,
  Contains:
    List of techs forming a possible SRx combo
    Bandwidth associated
    Band associated
----------------------------------------------------------------------------*/
typedef struct
{
  /* number of techs in the entry */
  uint8                  num_techs;
  /* list of client IDs*/
  trm_client_enum_t      techs_in_srx_group_entry[TRM_SRX_MAX_TECHS_GROUP_ENTRY];
  /* Associated Bandwidth for this Combo */
  uint32                 max_band_width;
  /* associated band for this combo */
  trm_band_t             band;

} trm_srx_group_entry_type;

/*----------------------------------------------------------------------------
  SRx Group Type,
  Contains:
    List of Entries of srx_group_entry_type
----------------------------------------------------------------------------*/
typedef struct
{

  uint8                       num_of_entries;
  trm_srx_group_entry_type    group_entry[TRM_SRX_MAX_GROUP_ENTRY];

}trm_srx_group_type;

/*----------------------------------------------------------------------------
  SRx GInfo Type,
  Contains:
    List of possible SRx/non-SRx combo
    RF info pointer
----------------------------------------------------------------------------*/
typedef struct
{
  /* Info for device ordering table*/
  uint8                 num_groups;
  trm_srx_group_type    group_info[TRM_SRX_MAX_GROUPS]; 

  /* Pass through information for RF */
  void                  *rf_info;

}trm_srx_info_type;

/*----------------------------------------------------------------------------
  TRM MULTI REQUEST ENUM
----------------------------------------------------------------------------*/
typedef enum
{
  /* Request and notify type */
  TRM_REQUEST_N_NOTIFY_TYPE,

  /* Request Type */
  TRM_REQUEST_TYPE,
	
  /* Reserve at Type */
	TRM_RESERVE_AT_TYPE,

  /* Release type */
  TRM_RELEASE_TYPE,
	
  /* Pre Release Type */	
  TRM_PRE_RELEASE_TYPE,

  /* Modify Chain State Type */
  TRM_MODIFY_CHAIN_STATE_TYPE,
	
  /* Async Event Response Type */	
  TRM_ASYNC_EVENT_RESP_TYPE,

}trm_multi_request_enum_type; 

typedef trm_client_info trm_pre_release_input_info;

/*----------------------------------------------------------------------------
  union MULTI REQUEST INFO TYPE,
  Contains:
    All possible data type that can be passed in multi request 
----------------------------------------------------------------------------*/
typedef union
{
	/* req and notify input info */
  trm_request_notify_input_info      req_not_info;

  trm_request_input_info             req_info;
	
	/* reserve at info */
	trm_reserve_at_input_info			    res_at_info;
	
	/* release info */
	trm_release_chain_input_info		  rel_info;
	
	/* future release info */
	trm_pre_release_input_info		    pre_rel_info;

	/* modify chain state info for band tune */
	trm_modify_chain_state_input_info	  mod_info;
	
	/* connected mode hop response indication */
	trm_async_event_response_data_type	  resp_info;
  
} trm_multi_request_info_type;

/*----------------------------------------------------------------------------
  TRM MULTI REQUEST STRUCT TYPE
  Contains:
    Request type
    Data associated to the request 
----------------------------------------------------------------------------*/
typedef struct
{

  trm_client_enum_t                 client_id;

  trm_multi_request_enum_type		    request_type;
  
  trm_multi_request_info_type       request_data;

}trm_multi_request_struct_type;

/*----------------------------------------------------------------------------
  TRM_MULTI_REQUEST_INPUT_TYPE
  Contains:
    List of requests being made
    SRx group information
----------------------------------------------------------------------------*/
typedef struct
{
  /* number of entries in the request*/
  uint8 			                num_of_entries;
  
  /* list of request information */
  trm_multi_request_struct_type		request_info[TRM_GROUPED_CLIENTS_MAX];

  /* SRx Group information */
  trm_srx_info_type                 srx_info;

}trm_multi_request_input_type;


typedef union
{
  trm_grouped_request_return_data   req_data;

}trm_multi_request_output_data;

typedef struct
{
  trm_multi_request_output_data    return_data;

}trm_multi_request_output_type;


/*============================================================================

FUNCTION TRM_MULTI_REQUEST

DESCRIPTION
  allows clients to make multiple requests in a single call
  Request types supported:
    1. Request and Notify
    2. Release
    3. Pre-Release
    4. Modify Chain State(band tune only)
    5. ASync event response
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
 
  ============================================================================*/
void trm_multi_request( trm_multi_request_input_type* input_info, 
                        trm_multi_request_output_type* output_info
); 


/*----------------------------------------------------------------------------
  TRM_PROTOCOL_STACK_STATUS_E_TYPE
  Contains:
    Enum used to indicate the protocl stack status
----------------------------------------------------------------------------*/
typedef enum
{
  TRM_PROTOCOL_STACK_NO_STATUS,  /* Uninialized value */
  TRM_PROTOCOL_STACK_ACTIVE,     /* Protocol stack is enabled */
  TRM_PROTOCOL_STACK_SUSPEND,    /* Protocol stack is suspended. Ex: IRAT */
  TRM_PROTOCOL_STACK_INACTIVE,   /* Protocol stack is disabled */
  TRM_PROTOCOL_STACK_MAX,        /* MAX state */
  
} trm_protocol_stack_status_e_type;

/*----------------------------------------------------------------------------
  TRM_PROTOCOL_STACK_IND_INPUT_TYPE
  Contains:
    Information about tech stack status
----------------------------------------------------------------------------*/
typedef struct
{
  /* Client ID */
  trm_client_enum_t                  client_id;

  /* ASID  */
  sys_modem_as_id_e_type             as_id;

  /* status of stack */
  trm_protocol_stack_status_e_type   status;

}trm_protocol_stack_ind_input_type;

/*----------------------------------------------------------------------------
  TRM_PROTOCOL_STACK_IND_OUTPUT_TYPE
  Contains:
    Information about tech stack status output
----------------------------------------------------------------------------*/
typedef struct
{
  /* Client ID */
  trm_client_enum_t                  client_id;
  
  /* Result */
  boolean                            result;

}trm_protocol_stack_ind_output_type;


/*============================================================================

FUNCTION TRM_PROTOCOL_STACK_INDICATION

DESCRIPTION
  Allows clients to vote about the status of the protocol stack
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
 
  ============================================================================*/
void trm_protocol_stack_ind
( 
  trm_protocol_stack_ind_input_type*   input_info, 
  trm_protocol_stack_ind_output_type*  output_info
); 

/*============================================================================

FUNCTION TRM_LOG_SUMMARY_LOG_PACKET

DESCRIPTION
  Log TRM log packet with summary of the current state

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_log_summary_log_packet(void);



/*----------------------------------------------------------------------------
trm_register_client_type			   
----------------------------------------------------------------------------*/
typedef enum
{
  TRM_PCELL,
  TRM_CA,
  TRM_CA1,
  TRM_CA2,
  TRM_CA3,
  TRM_HORX,
  TRM_CA_HORX,
  TRM_CA1_HORX,
  TRM_CA2_HORX,
  TRM_CA3_HORX,
  TRM_NO_TYPE =255
}
trm_register_client_type;

/*----------------------------------------------------------------------------
trm_register_rx_index			   
----------------------------------------------------------------------------*/

typedef enum
{
  TRM_INDEX_TX,
  TRM_INDEX_PRX,
  TRM_INDEX_DRX,
  TRM_INDEX_INVALID
}
trm_register_rx_index;

/*----------------------------------------------------------------------------
trm_exeption_client_type			   
----------------------------------------------------------------------------*/
typedef enum
{
TRM_EXP_NONE,
TRM_EXP_EMBMS1,
TRM_EXP_EMBMS2,
TRM_EXP_LTED,
TRM_EXP_LTECGI
}
trm_exeption_client_type;


/*----------------------------------------------------------------------------
LTE_CLIENT_REGISTRATION_DATA
----------------------------------------------------------------------------*/
typedef struct
{
  trm_rat_group_type RAT;
  trm_asid_mapping_return_data as_id;
  trm_register_client_type  sec_client;
  trm_register_rx_index  rx_index;
  trm_exeption_client_type exp_client_type;
  trm_client_enum_t client_id;
}
trm_client_registration_data; 


/*----------------------------------------------------------------------------
trm_grouped_client_registration_data		   
----------------------------------------------------------------------------*/
/* enable this once dynamic clients changes are in 
#define TRM_MAX_CLIENT_REGISTRATION (TRM_MAX_CLIENTS-TRM_LAST_STATIC_CLIENT)
*/
/*Remove this macro and use above one after dynamic client main code changes*/
#define TRM_MAX_CLIENT_REGISTRATION TRM_MAX_CLIENTS

typedef struct
{
  trm_client_registration_data client_details[TRM_MAX_CLIENT_REGISTRATION];
  uint8  num_clients;
}
trm_grouped_client_registration_data; 

/*----------------------------------------------------------------------------
trm_grouped_client_registration_output	   
----------------------------------------------------------------------------*/
typedef struct
{
  trm_client_enum_t clients[TRM_MAX_CLIENT_REGISTRATION];
  uint8  num_clients;
}
trm_grouped_client_registration_output;
/*============================================================================

FUNCTION trm_register_clients

DESCRIPTION
  updates the ant state info subscription
 
DEPENDENCIES
  None

RETURN VALUE
  boolean
    TRUE if successful
    FALSE otherwise
 
SIDE EFFECTS
  None
 
  ============================================================================*/
void trm_register_clients(trm_grouped_client_registration_data *reg_data,
                          trm_grouped_client_registration_output *out);

/*============================================================================

FUNCTION trm_de_register_lte_clients

DESCRIPTION
  updates the ant state info subscription
 
DEPENDENCIES
  None

RETURN VALUE
  boolean
    TRUE if successful
    FALSE otherwise
 
SIDE EFFECTS
  None
 
  ============================================================================*/
void trm_de_register_clients(trm_grouped_client_registration_data *reg_data,
                             trm_grouped_client_registration_output *out);


/*============================================================================

CALLBACK trm_idle_collision_cb_type

DESCRIPTION
  This callback is for non-tech clients to know about the idle collision
  fallback events
 
DEPENDENCIES
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  None
 
  ============================================================================*/
typedef void (* trm_idle_collision_cb_type)
(
  trm_idle_coll_event_data* event_data
);

/*============================================================================

FUNCTION trm_register_fallback_ind_cb

DESCRIPTION
  This is used by non-tech clients to register callback function with TRM for
  listening to the idle collision fallback indication.
  Passing NULL pointer will deregister the fallback indication.
 
DEPENDENCIES
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  None
 
  ============================================================================*/
void trm_register_fallback_ind_cb(trm_idle_coll_ind_client_e idle_coll_client,
                                  trm_idle_collision_cb_type idle_coll_cb);

/*============================================================================

FUNCTION trm_update_random_num

DESCRIPTION
This function updates the newly generated random number with TRM

DEPENDENCIES
None

RETURN VALUE
uint32

SIDE EFFECTS
None

============================================================================*/
void trm_update_random_num (uint32 random_num);


/*============================================================================

FUNCTION trm_send_page_merge_indication

DESCRIPTION
LTE will use this api to indicate when page merge starts/ends.
TRM needs to turn off/handle drx reduction/fallback accordingly

DEPENDENCIES
None

RETURN VALUE
void

SIDE EFFECTS
None

============================================================================*/
void trm_send_page_merge_indication (boolean page_merge);

/*============================================================================

FUNCTION trm_set_asdiv_switch

DESCRIPTION
 API to set value to disable/enable AsDiv switch.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void trm_set_asdiv_switch(boolean state);

/*============================================================================

FUNCTION trm_get_asdiv_switch

DESCRIPTION
 API to get value to disable/enable AsDiv switch.
 The default value for return is TRUE if no diag command is set.

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - If switch is enabled
  FALSE - If switch is disabled

SIDE EFFECTS
  None

============================================================================*/
boolean trm_get_asdiv_switch(void);

#include "trm_deprecated_apis.h"

#endif /* TRM_H */
