#ifndef CMEFS_H
#define CMEFS_H
/*===========================================================================

                         C A L L   M A N A G E R

                       CM EFS  I N T E R F A C E

DESCRIPTION
  This header file contains all the definitions necessary to interface
  with CMEFS.C (i.e. the CM EFS components).

Copyright (c) 2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmefs.h#5 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/22/14   xs      hVOLTE SSAC support
03/06/14   xs      Per RAT SMS domain sel
10/07/13   jvo     Added CMEFS_ENS_SUB_FEATURES to cmefs_id_e_type
07/24/13   jh      cmefs_read to return fail for size 0 file and reorganize
                   the cmefs enum so only internal files to be created.
07/19/13   jvo     Remove all MDM/MSM Fusion features
03/26/12   fj      Add EFS item for RAT-based no srv/limited srv hold up time.
02/03/13   cl      FR1975: Dynamic Switch SVLTE <-> CSFB
01/07/12   xs      Added PS supplemetary service support
10/09/12   sr      Support  to derive MCC of an EvDo system from eqprl.
09/12/12   skk     Do not send internal service lost or limited service during uptime.
06/05/12   sg      CM changes for SGLTE requirement
03/23/12   gm      DIME integration fix: TMC deprecation and others.
11/30/11   jh      Added feature for dynamic rat acq order change
12/01/11   vk      PS voice domain selection migration from CNE to CM
09/14/11   rk      NV items N_min_MO_call_soft_retry and N_maxSIB8
09/02/11   rk      S102 less 1XCSFB
07/05/11   rk      Support for sending voice domain preference to NAS
05/18/11   rn      WCDMA Channel locking feature
05/10/11   sv      Make GSM Auxilary Speech value EFS configurable
04/08/11   sv      Add EFS item for DRS bit value for Voice calls
03/24/11   cl      FUSION2 MMSS-Arbitration centralization rework
03/22/11   cl      Add GW-Throttle Mechanism during HDR/LTE Idle for FUSION2
03/09/11   cl      Add support for SVLTE2 MMSS arbitration
02/22/11   rk      SMS only registation
02/16/11   xs      Added JCDMA EFS control
02/07/11   xs      Mainline FEATURE_JCDMA features: call throttle parameter need to be
                   created by user instead of CM
01/25/11   ak      Added support for periodic GWL scans in DO only coverage.
01/19/11   fj      Added IDM timer value in EFS.
12/14/10   xs      Added support for call throttle parameter configurable in EFS for JCDMA
09/15/10   rm      Initial revision

===========================================================================*/


#include "cm.h"       /* External interface to CM */
#include "cm_v.h"     /* Interface to local CM defintions */
#include "cmi.h"      /* Internal interface to cm.c */
#include "comdef.h"   /* Definition for basic types and macros */

#define C2K_SXLTE_OPTI_ARRAY_SIZE 50 /* Array size of 1xsxlte_optimization EFS */
#define C2K_SXLTE_OPTI_ARRAY_USED 4 /* Number of elements used in 1xsxlte_optimization array */

#define CMEFS_UE_BASED_SUPS_ARRAY_USED 8 /* Number of elements used in UE based sups setting */
#define CMEFS_UE_BASED_SUPS_ARRAY_SIZE 20 /* Array size of UE based sups setting */
#define CMEFS_CW_SUPS_CONFIG_ARRAY_SIZE 20 /* Array size for CW config */
/*===========================================================================
=============================================================================
=============================================================================
================================== TYPES ====================================
=============================================================================
=============================================================================
===========================================================================*/

/* Features controlled by c2k_sxlte_opti_array, one feature per index
** When you add a new feature you must:
** - increment C2K_SXLTE_OPTI_ARRAY_USED
** - update cmefs_init_c2k_sxlte_opti() to set default value
*/
typedef enum
{

  C2K_SXLTE_OPTI_PWRUP_FROM_LAST_SUBMODE = 0,

  C2K_SXLTE_OPTI_PS_DETACH,
  /**< This item is no longer used */

  C2K_OPTI_ESR_E911,

  C2K_OPTI_ESR_E911_3RD_PARTY

} c2k_sxlte_opti_e_type;

/** EFS id */
typedef enum
{

  CMEFS_NONE = -1,
  /**< Not in EFS id - internal use only */


  CMEFS_USER_NET_SEL_MODE_PREF_NAM_1,
  /**< User network selection mode_pref for NAM - 1 */

  CMEFS_USER_NET_SEL_MODE_PREF_NAM_2,
  /**< User network selection mode_pref for NAM - 2 */

  CMEFS_USER_NET_SEL_MODE_PREF_NAM_3,
  /**< User network selection mode_pref for NAM - 3 */

  /* 3 */
  CMEFS_USER_NET_SEL_MODE_PREF_NAM_4,
  /**< User network selection mode_pref for NAM - 4 */

  CMEFS_HICPS_GWL_AVOIDANCE_PARAM,
  /**< Parameters for HICPS enhancement for GWL avoidance */

  CMEFS_SGLTE_DEVICE,
  /**< Parameters to identify SGLTE target */

  CMEFS_IDM_TIMER_VALUE,
  /**< IDM timer */

  /* 7 */
  CMEFS_SRV_IND_TIMER_VALUES,
  /**< srv ind uptime timers */

  CMEFS_RAT_SRV_IND_TIMER_VALUES,
  /**< Rat-based srv ind uptime timers - NV73551 */

  CMEFS_MM_ARBITRATION_ENABLE,
  /**< Flag to enable/disable intermodem arbitration on SVLTE II */

  CMEFS_SOA_SVLTE_PLMN_ID_LIST,
  /**< Plmn ID list store the plmns which support SVLTE */

  /* 11 */
  CMEFS_SOA_CONFIG,
  /**< Configuration for CMSOA */

  CMEFS_JCDMA_CALL_THROTTLE_TIME,
  /**< JCDMA call throttle timer */

  CMEFS_JCDMA_CALL_THROTTLE_NUM,
  /**< JCDMA call throttle times */

  CMEFS_DOMAIN_SELECTION_BEGIN,

  CMEFS_SMS_ONLY,
  /**< Is CS registation is for SMS only - NV65776 */

  CMEFS_UE_USAGE_SETTING,
  /**< Voice centric or data centric - NV65777 */

  /* 17 */
  CMEFS_VOICE_DOMAIN_PREF,
  /**< Voice domain preference - NV66048 */

  CMEFS_LTE_DISABLE_DURATION,
  /**< Duration for how long to diable LTE - NV65778 */

  CMEFS_N_MIN_MO_CALL_SOFT_RETRY,
  /**< Min number of soft retry attempts - NV67212 */

  CMEFS_N_MAX_SIB8_READ_ATTEMPTS,
  /**< Max number of sib8 read attempts - NV67213 */

  /* 21 */
  CMEFS_SMS_DOMAIN_PREF,
  /**< SMS domainpreferance - NV66472 */

  CMEFS_RAT_ACQ_ORDER,
  /**< RAT Acquisition order - NV67249 */

  CMEFS_SMS_MANDATORY,
  /**< SMS mandatory - NV66473 */

  /* 24 */
  CMEFS_SUPP_SRV_DOMAIN_PREF,
  /**< PS supplementary service domain preference - NV70239*/

  CMEFS_QMSS_ENABLED,
  /** QRD Features Enabled Flag - NV70301 */

  CMEFS_OPERATOR_NAME,
  /** Operator Name required for QRD Features - NV70302 */

  CMEFS_SMS_OVER_S102,
  /**< SMS Over S102 support - NV70362 */

  /* 28 */
  CMEFS_GET_NET_AUTO_MODE,
  /**< Support GET NET in Auto Mode - NV71524 */

  CMEFS_CUSTOM_EMERG_INFO,
  /**< Emergency call customization information - NV71522 */

  CMEFS_CU_IMSI,
  /* China Unicom IMSI Information */

  CMEFS_CMCC_IMSI,
  /* China Unicom IMSI Information */

  /* 32 */
  CMEFS_IMSI_MCC,

  CMEFS_IMSI_MIN1,

  CMEFS_IMSI_MIN2,

  CMEFS_IMSI_11_12,

  CMEFS_MANU_CODE,

  /* 37 */
  CMEFS_MANU_NAME,

  CMEFS_DEV_MODEL,

  CMEFS_SW_VER,

  CMEFS_LOCAL_REG_IND,

  CMEFS_MID_SRVCC,
  /* NV71554 */

  CMEFS_LTE_DO_IRAT_DURATION,
  /* NV71579 */

  /* 43 */
  CMEFS_EXTEND_LTE_DISABLE_DURATION,
  /**< Extended LTE disable duration - NV70286 */

  CMEFS_CDMA_VOICE_CALL_COLLISION,
  /* NV72531 */
  
  CMEFS_3GPP2_PS_CALL_OPTIMIZATION,
  /* NV72540 */

  CMEFS_SMS_DOMAIN_PREF_LIST,
  /* NV72573 */

  CMEFS_CSG_AUTO_MODE,
  /**<Support CSG in Auto Mode - NV72541 */
  CMEFS_SSAC_HVOLTE,
  /* NV72610 */

  /* 49 */
  CMEFS_ALLOW_CSFB_UPON_IMS_REG,
  /* NV73512 */

  CMEFS_UE_BASED_CW,
  /*UE based call waiting setting - NV73538 */
  CMEFS_ALLOW_SMS_IN_ECBM,
  /**<Support for allowing SMS in ECBM for Sprint - NV72618 */

  CMEFS_C2KSWITCH2SRLTE,
  /* Enable/disable CM support to switch to SRLTE mode to enable
  ** background BSR for LTE when camped on 1x - NV73561
  */

  CMEFS_MMODE_1XSXLTE_OPTIMIZATION,
  /**< mmode optimization flags - NV73596 */

  CMEFS_IMS_REG_STATUS_WAIT_TIMER,
  /**< IMS registration wait timer - NV72607 */

  CMEFS_CUSTOM_VOICE_SR_TIMER,
  /** EFS item to increase the silent redial timer - NV73582 */

  CMEFS_SKIP_GWT_SCAN_DURING_RLF,
  /** EFS item to skip GWT scan when VOLTE is active
  ** during RLF - NV73631 
  */

  CMEFS_ALLOW_CS_TO_PS_EMERG_REDIAL,
  /* NV73655 */

  CMEFS_ATTACH_COMPLETE_TIMER,
  /** ATTCH complete timer - NV73666 */

  CMEFS_CMREGPRX_DBG_MSGS,
  /** Enables CMREGPRX dbg msgs in the CM dbg buf */

  /* 59 */
  CMEFS_ID_LTE_BANDPREF,
  /**< User selected LTE band pref - NV65633 */

  CMEFS_ID_LTE_BANDPREF_65_256,
  /**< User selected LTE band pref bands 65-256 - NV73680 */

  CMEFS_ID_TDS_BANDPREF,
  /**< User selected TD-SCDMA band pref - NV67311*/

  CMEFS_BUFFER_SYS_SEL_PREF,
  /**< Enable/disable buffering of sys_sel_pref_cmds - NV73702 */

  CMEFS_CDMA_HOLD_SRV_TIMER,
  /**< Timer to hold 1x service reported to clients - NV73698 */

  CMEFS_HOLD_HY2_OPER_FLAG_TIMER,
  /**< Timer to hold is_oper flag reported to clients - NV73712 */

  CMEFS_VOLTE_SR_CONTROL,
  /**< Volte SR Control - NV72512 */

  CMEFS_IS_PLMN_BLOCK_REQ_IN_LTE_ONLY_MODE,
  /**< Inidcates if PLMN blocking is needed in LTE only mode 
  ** or not - NV73704
  */

  CMEFS_MANUAL_SEARCH_PERIODIC_TIMER,
  /**< Timer to trigger Full Band Manual Search - NV73738 */

  CMEFS_CDMA_LESS_DEVICE,
  /**< CDMA less device config, should not be written 
  ** on the fly - NV73732 
  */

  CMEFS_MTCSFB_CALL_REDIAL_OVER_1X,
  /**< MTCSFB call to be redialled over 1x - NV73572 */

  CMEFS_SELF_ACTIVATION_SUPPORT,
  /** If CSIM is not activated then do not switch to 
  ** SRLTE mode - NV73766 
  */

  CMEFS_PS_SIGNALLING_WAIT_TIMER,
  /* At the end of last Packet call for GSM/WCDMA, we would
  ** wait for this time. This allows network to teardown signaling
  ** during this time. By introducing wait, we will avoid the
  ** race condition when a PDP CONTEXT ACTIVE request is sent
  ** while network is tearing down the PS signaling.
  */

  CMEFS_SDS_PLMN_BLOCKING_TYPE,
  /** For L+L, whether to block plmn if IMS reg fail - NV73895 */

  CMEFS_REG_SMS_RETRY_TIMER_VALUE,
  /** Timer value for AUTO REG SMS retry - NV73924
   */

  CMEFS_LTE_SOFT_FAILURE_RETRY_COUNT,
  /* If this count is exceeded, the VOLTE call will be attempted over CS
   ** This count defines the maximum LTE soft failures during a call 
   ** - NV73945
   */

  CMEFS_ENABLE_VOLTE_ON_STACK2,
  /* Stay in SRLTE sub mode in HOME and MO/MT VOLTE call to go on 
  ** stack 2 - NV73959
  */

  CMEFS_SDS_PLMN_BLOCKING_SYNC,
  /** always Wait for IMS registration status then do PLMN blocking 
  ** - NV73983
  */

  CMEFS_ALLOW_CS_PAGE_DURING_MT_VOLTE_CALL,
  /* Allow MT CS Page if MT VOLTE Call cannot be connected/alerted
    **within 7 (configurable) seconds - NV73988
   */

  CMEFS_E911_PRIORITIZE_PS,
  /* Enable/disable CM to route emergency calls over VOLTE as long as
  ** LTE network has e911 capability regardless of csfb availability 
  **- NV73953
  */

  CMEFS_POWERDOWN_TIMESTAMP,
  /* Timestamp when operation mode changes to LPM
  ** Read at initial power up to get timestamp when UE last powers down.
  ** Write when operation mode changes to LPM and has been online at least once
  ** to ensure EFS is not written when AP sends LPM->online at initial power up
  */

  CMEFS_NON_DDS_LTE_REMOVAL_TIMER,
  /* Remove LTE on non DDS sub when voice over IMS is not supported for a duration.
  ** Two timers to differentiate whether it's NW indicates no vops or IMS reg failure case
  ** - NV74038
  */

  CMEFS_E911_PREF_OVER_PS_DURING_WLAN,
  /* Prefer PS for emergency call when the current voice domain is wifi
  */

  CMEFS_EMERG_CALL_REJ_IN_ACQ_HOLD,
  /* Control the decision of emergency call rejection, if acquisition is held on that perticular sub.
  */

  CMEFS_AFFCN_LIST_INFO,
  /* Controls sending modem frequency info reports to AP
  */

  CMEFS_REG_SMS_SW_VER,
  /*SW Version informed in AUTO REG SMS*/

  CMEFS_MCC_LIST_EMERG_CALL_END_NO_SIM, 
  /**<EFS id for MCC list on which emergency call needs to be ended without
    SIM when rejected with cause #5 - NV 74183 */

  CMEFS_DISABLE_MCC_LIST_EMERG_CALL_END_NO_SIM,
  /**<EFS id for MCC list on which emergency call should not be ended without
     SIM when rejected with cause #5 - NV 74259 */

  CMEFS_ALLOW_MT_PAGE_CONCURRENCY,
  /* Enable/Disable allowing MT page while there is a VoWLAN call on the other
  ** sub - NV 74275
  */

  CMEFS_ONLINE_EMERG_PENDING_SUPP,
  /* NV to enable pending emergency support system selection upon online.
  ** No NV.
  */

  CMEFS_ADDTNL_CHAR_FOR_VOICE_CC,
  /* Controls whether additional characters need to be
  ** added for international roaming numbers.
  ** NV 74789
  */

  CMEFS_INT_MAX,
  /* Enter all EFS items that can be read but not written to by CM
  ** (owned by other modules) below
  */

  CMEFS_FEATURE_JCDMA,
  /**< JCDMA global config - NV65810 */

  CMEFS_WCDMA_FREQ_LOCK,
  /**< WCDMA channel locking - NV65818 */

  CMEFS_ENS_SUB_FEATURES,
  /**< ENS sub features, including RAT balancing support 
  **- NV71568 
  */

  CMEFS_MODE_DISABLED_IN_TSTS,
  /**<Disable LTE from multiomode sub
   ** when moving to tsts from dsds - NV72591 */

  CMEFS_TAU_RADIO_CAP_UPDATE,
  /**< Indicates if the TAU capability update feature is enabled/disabled */

  /* !!IMPORTANT NOTE!!
  ** If it's MMODE owned item, add the new EFS item above the CMEFS_INT_MAX
  ** If it's other module owned item, add the new EFS item above here
  */
  CMEFS_SXLTE_TIMERS,
  /**< SXLTE timers - NV72538 */

  CMEFS_SUPPORT_DUAL_RX_UE,
  /**< SXLTE timers - NV72539 */

  CMEFS_MSIM_STANDBY_ESR_SUPPORT,
  /**< Indicates if ESR should be done in case of LTE sharing RF 
  **- NV72614 
  */

  CMEFS_1xSRVCC_CALLED_PARTY_NUM,
  /**< STN-SR Number for 1xSRVCC orig - NV72617 */

  CMEFS_CUSTOM_VOWIFI_CONFIG,
  /**< Wifi configuration details - NV73545 */

  CMEFS_ID_RPM_SYS_TIME,
  /*<System time till last power down - NV73559 >*/

  CMEFS_ID_LTE_RPM_INFO,
  /**< LTE RPM configuration details - NV73673 */

  CMEFS_EMCALL_TO_NORMAL_VOICECALL,
  /*<Convert emergency call to normal voice call - NV73720 >*/

  CMEFS_LTE_3GPP_RELEASE_VER,
  /**< EFS file owned by LTE team -NV65538 */

  CMEFS_IMS_ENABLE,
  /**< EFS file owned by IMS team - NV67218 */

  CMEFS_REJECT_OIR_CLIR_INTERROGATION,
  /**< Reject OIR and CLIR Interrogation request - NV73824 */

  CMEFS_LTE_RPM_INFO_2,
  /** LTE RPM configuration details - NV73854 */

  CMEFS_CLEAR_MANUAL_PLMN,
  /* Clears MPLMN */

  CMEFS_PS_CALL_ORIG_HOLD_TIMER,
  /* PS call hold timer if other sub is waiting for srv cnf - NV73944 */

  CMEFS_RPM_FEATURE_ENABLED,
  /** RPM feature enabled - NV73989 */

  CMEFS_E911_WLAN_TO_LTE_HANDOVER,
  /** Enable feature to start LTE scans so that e911 call over wlan 
  **  can be handed over to LTE
  */

  CMEFS_MSIM_VOLTE_ESR_SUPPORT,
  /**< Indicates if ESR should be done in case of LTE in VoLTE mode sharing RF 
  **- NV72614 
  */

  CMEFS_UE_BASED_SUPS_MODE,
    /*For configuring UE based supplementary mode - NV 74229*/

  CMEFS_UE_BASED_SUPS,
  /*For configuring UE based supplementary service - NV 74221*/

  CMEFS_SYNC_UE_BASED_CW_WITH_CS,
  /** Enable feature to sync terminal based CW between CS and IMS - NV 74182
  */

  CMEFS_IMMEDIATE_SUPS_SYNC_WITH_CS,
  /* Enable it for immediate sups sync to CS network through CSFB. IMS - NV 74232*/

  CMEFS_CW_EXT_CONFIG_INFO,
  /* COntains the CW extended config info NV74339 */

  CMEFS_SUPS_ENABLE_STATUS_NW,
  /*UE BASED SUPS informed to NW*/

  CMEFS_GPP_WIFI_CELLULAR_SCAN_TIMER,  
  /* Timer for which emergency call would be attempted on Cellular domain when the UE is attached over a WLAN access 
  **NV74320
  */
  CMEFS_SKIP_NO_SRV_BUFFERING_ON_RAT_REMV,
  /* Buffer no service when this NV is enabled NV74350*/

  CMEFS_PREV_UE_BASED_CW,
  /*
   ** EFS to store CW setting which will be retained across power cycle / MCFG refresh.
  */

  CMEFS_MAX
  /**< Maximum number of CM EFS items */
} cmefs_id_e_type;

typedef struct
{
  uint8 version;
  /* version number of the NV item */
  boolean opti[50];
  /* optimization flags for mmode 1xsxlte optimizations*/
} mmode_1xsxlte_optimization_s_type;


/*===========================================================================
=============================================================================
=============================================================================
============================== FUNCTIONS ====================================
=============================================================================
=============================================================================
===========================================================================*/


/*===========================================================================

FUNCTION cmefs_read

DESCRIPTION
Read EFS item denoted by efs_id and copy to data_ptr

  MODE
DEPENDENCIES
  None

RETURN VALUE
  TRUE if read is successful,
  FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

extern  boolean                  cmefs_read(

  cmefs_id_e_type           efs_id,
  /* passing the efs Id to read.
  */
  byte                     *data_ptr,
  /* Pointer to a data buffer where to store the data that is
  ** associated with the EFS Data.
  */
  int32                    *size_ptr
  /* On input this parameter is the  maximum number
  ** of bytes to be copied into the buffer pointed by data_ptr.
  ** This parameter must not be nil.
  ** On output it is set *size_ptr to the actual number of bytes
  ** that were copied into the buffer pointed by data_ptr.
  */
);


/*===========================================================================

FUNCTION cmefs_write

DESCRIPTION
Write contents of data_ptr in EFS item denoted by efs_id

  MODE
DEPENDENCIES
  None

RETURN VALUE
  TRUE if write is successful,
  FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

extern    boolean                  cmefs_write(

  cmefs_id_e_type           efs_id,
  /* passing the efs Id to read.
  */
  const byte                *data_ptr,
  /* Pointer to a data buffer to write to WLAN MRU.
  */
  int32                     size
  /* Size of the data_ptr buffer.
  */
);

/*===========================================================================

FUNCTION cmefs_fs_write

DESCRIPTION
  Write contents of data_ptr to EFS item denoted by efs_id

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean                  cmefs_fs_write(

  cmefs_id_e_type          efs_id,
  /* passing the efs Id to write.
  */
  const byte                  *data_ptr,
  /* Pointer to a data buffer to write to EFS.
  */
  int32                       size,
  /* Size of the data_ptr buffer.
  */
  sys_modem_as_id_e_type      sub_id
  /* A different EFS file is read based on the sub_id
  */
);

/*===========================================================================

FUNCTION cmefs_init

DESCRIPTION
  Initialize CM EFS items.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void cmefs_init(void);

/*===========================================================================

FUNCTION cmefs_get_c2k_sxlte_opti

DESCRIPTION
  Check if the specified optimization is enabled

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the optimization is enabled
  FALSE otherwise

SIDE EFFECTS
  None

==========================================================================*/
boolean cmefs_get_c2k_sxlte_opti(
  c2k_sxlte_opti_e_type opti_index,
  sys_modem_as_id_e_type asubs_id
);


/*===========================================================================

FUNCTION cmefs_get_c2k_sxlte_opti

DESCRIPTION
  Check if the specified optimization is enabled

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the optimization is enabled
  FALSE otherwise

SIDE EFFECTS
  None

==========================================================================*/
boolean* cmefs_get_addr_c2k_sxlte_opti(
  c2k_sxlte_opti_e_type opti_index,
  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

FUNCTION cmefs_init_c2k_sxlte_opti

DESCRIPTION
  Read the efs item for 1XSXLTE optimization and set default value if read fails

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void cmefs_init_c2k_sxlte_opti(void);

/*===========================================================================

FUNCTION cmefs_get_efs_id_for_curr_nam

DESCRIPTION
  Returns the EFS id for the given NAM

DEPENDENCIES
  None

RETURN VALUE
  cmefs_id_e_type.

SIDE EFFECTS
  None.

===========================================================================*/
extern cmefs_id_e_type cmefs_get_efs_id_for_curr_nam (cm_nam_e_type);


/*===========================================================================

FUNCTION cm_is_jcdma_enable

DESCRIPTION
  If a specified feature is enabled return TRUE, otherwise return FALSE


DEPENDENCIES
  None

RETURN VALUE
  TRUE
  FALSE
SIDE EFFECTS
  None.

===========================================================================*/
boolean cm_is_jcdma_enable(void);


/*===========================================================================

FUNCTION cmefs_get_itc_aux_val

DESCRIPTION
  Get ALS ITC value to use

DEPENDENCIES
  None

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/

byte cmefs_get_itc_aux_val(void);


/*===========================================================================

FUNCTION cmefs_get

DESCRIPTION
Get EFS item denoted by efs_id and copy to data_ptr

  MODE
DEPENDENCIES
  None

RETURN VALUE
  TRUE if read is successful,
  FALSE otherwise.

SIDE EFFECTS
  None.

==========================================================================*/
boolean                  cmefs_get(

  cmefs_id_e_type          efs_id,
  /* passing the efs Id to read.
  */
  byte                     *data_ptr,
  /* Pointer to a data buffer where to store the data that is
  ** associated with the EFS item.
  */
  int32                    size,
  /* On input this parameter is the  maximum number
  ** of bytes to be copied into the buffer pointed by data_ptr.
  ** This parameter must not be nil.
  ** On output it is set *size_ptr to the actual number of bytes
  ** that were copied into the buffer pointed by data_ptr.
  */
  sys_modem_as_id_e_type  sub_id
  /* A different EFS file is read based on the sub_id
  */
);

/*===========================================================================

FUNCTION cmefs_get2

DESCRIPTION
Get EFS item denoted by filename and copy to data_ptr

  MODE
DEPENDENCIES
  None

RETURN VALUE
  TRUE if read is successful and size read matches the size passed in,
  FALSE otherwise.

SIDE EFFECTS
  None.

==========================================================================*/
boolean                  cmefs_get2(

  const char              *filename,
  /* passing the filename to read.
  */
  byte                     *data_ptr,
  /* Pointer to a data buffer where to store the data that is
  ** associated with the EFS item.
  */
  int32                    size,
  /* The  maximum number of bytes to be copied into the
  ** buffer pointed by data_ptr.
  */
  sys_modem_as_id_e_type  sub_id
  /* A different EFS file is read based on the sub_id
  */
);


/*===========================================================================

FUNCTION cmefs_map_efs_id_to_filename

DESCRIPTION
  Map EFS id to EFS filename


DEPENDENCIES
  None

RETURN VALUE
  Return TRUE if mapping was successful, else FALSE

SIDE EFFECTS
  None.

===========================================================================*/

const char *const  cmefs_map_efs_id_to_filename(

  cmefs_id_e_type          efs_id
  /* passing the efs Id to map.
  */
);
/*===========================================================================

FUNCTION cmefs_refresh

DESCRIPTION
  Refresh NV items

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void cmefs_refresh(void);

/*===========================================================================

FUNCTION cmefs_fs_write_no_create

DESCRIPTION
  Write contents of data_ptr to EFS item denoted by efs_id with oflag = MCFG_FS_O_RDWR.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean                  cmefs_fs_write_no_create(

  cmefs_id_e_type          efs_id,
  /* passing the efs Id to write.
  */
  const byte                  *data_ptr,
  /* Pointer to a data buffer to write to EFS.
  */
  int32                       size,
  /* Size of the data_ptr buffer.
  */
  sys_modem_as_id_e_type      sub_id
  /* A different EFS file is read based on the sub_id
  */
);

#endif

