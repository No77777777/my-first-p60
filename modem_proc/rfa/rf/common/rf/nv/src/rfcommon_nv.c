
/*!
   @file
   rfcommon_nv.c

   @brief
    leveraged from rfnv.c

   @details
   Functions to read and write NV items. There are two set of functions rfcommon_nv_*
   and rfcommon_rfnv_* the difference is that the rfcommon_nv_* functions are
   used to read old NV items store in NV manager and rfcommon_rfnv_* functions are
   used to read both, old NV items and new RFNV items.
 
   RFNV items are RF rela  ted items with item id greater or equal to 20,000 and are stored
   in a public EFS folder /nv/item_files/rfnv/. See @ref rfnv_items.h for RFNV id numbers.
*/

/*===========================================================================
Copyright (c) 2009 - 2020 by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/rfa.mpss/4.11/rf/common/rf/nv/src/rfcommon_nv.c#1 $
$DateTime: 2021/03/09 05:11:02 $

when       who     what, where, why
--------   ---     --------------------------------------------------------------------------------
01/06/20   akm     rf rumi featuization
03/17/15   rp      KW Fix.
03/05/18   rp      Code changes to read and store AsDiv offset data from EFS file.
10/26/17   sk      Enable LTE B71 support
03/30/16   gvn     Add support to ignore legacy switchpoint NVs for LTE if new datatype is present
03/01/16   mah     Added LTE B66
11/23/15   aa      Fix FBRx C1 compressed nv loading 
10/13/15   Saul    GNSS Blanking. NV renamed to CONTROL. Listed bands are for disable of blanking.
10/13/15   Saul    GNSS Blanking
09/02/15   yb      Added support for new version of Tuner CL NVs
06/29/14   sd      FR 27572 Add support for AsDiv SAR backoff NV
02/12/14   cpm     call robust API to fetch combo RUMI RFNV items 
01/21/15   kg      Added support for Rx RSB calibration
10/28/14   ac      function to retreive nv tuner items for DBDC
10/16/14   jmf     [selftest] Implement multi-gain stage switch for ACLR improvement
10/14/14   jmf     [selftest] Implement FBRx Noise Floor Cal
10/08/14   cri     Increase uncompression size for Rx V2 NV
10/07/14   weiz    Support PA static compressed NV and fix the compile warning
08/21/14   ndb     KW warning fix
05/27/14   pl      Adding support for Common RX NV
12/16/13   aa      Move fbrx related api's to rfcommon_fbrx_nv.c file  
10/16/13   vs      Added support for tuner LTE CA NV
11/21/13   aa      Moved FBRX Cal NV retrieve api to rfcommon_fbrx_nv.h 
11/19/13   aa      Added api to retrieve fbrx_gain_nv_item per tech per band
11/12/13   aa      Added support to retrieve RFNV_DATA_TYPE_FBRX_GAIN_V2 data
10/24/13   vv      Added a runtime flag(use_rumi_nv_read) to switch between EFS read and rumi NV read(hardcoded values) and removing RUMI featurization
09/30/13   vs      Added changes for tuner nv item code fail case to avoid errors
09/19/13   vs      Adding fix for handling TUNER NV error case of array initialization 
08/27/13   vs      Added api for retrieving tuner NV item code per band per tech
08/26/13   bsh     Use the correct RUMI Flag for NVs 
08/13/13   ndb     Replace memcpy with memscpy
08/05/13   vbh     Update the RUMI feature flag 
08/02/13   jl      NV Buffer Cleared before Reuse
07/30/13   bsh     Added EFS bypass for RUMI 
07/29/13   aca     RL NV version update
07/23/13   dbz     Add FBRX gain switch point NV support
07/18/13   dbz     Add support for common API to intepret FBRX gain v.s. freq NV
07/02/13   ndb     Bug fixes in rfcommon_load_ant_tuner_cl_ctrl_info_from_nv()
06/28/13   ndb     Added rfcommon_load_ant_tuner_cl_ctrl_info_from_nv()
04/11/13   ems     Added support for NV items larger than 4K 
04/10/12   kg      Adding support for rfc.bl creation
10/11/12   aki     Create RFNV BL file and RFNV folder only if missing
11/21/11   sr      Only critical NVs missing will be F3 Error Msgs.
09/01/11   sar     Fixed typo, changing std.h to stdio.h.
08/26/11   sar     Replaced AEE lib with std lib.
07/06/11   aki     And back to: modem heap, fix kw, etc
06/30/11   aki     Reverting back to previous version, for now
06/22/11   aki     Allocate RFNV buffer from modem heap, fix kw and compiler 
                   warnings, misc fixes
03/24/11   pl      Fix warning
07/28/10   aki     Reduced the stack size requirement for rfnv functions, 
                   init update
07/09/10   aki     All RFNV files are now 'regular' files. Init will create 
                   config and folders. 
07/01/10   aki     Merging RFNV FTM changes from dev/rfnv_dev
06/23/10   aki     RFNV modifications  
04/02/10   adk     Merged branch scmm_mdm9k_merge_final
03/19/10   lcl     Pull in fixes from QSC6295
02/10/10   sar     Clocwork fixes.
02/02/10   sar     Clocwork fixes.
01/26/10   sar     Lint fixes.
02/27/09   sr      Initial version
===========================================================================*/
#include "stringl.h" /* for memscpy */
#include "task.h"
#include "rfcommon_nv.h"
#include "msg.h"
#include "diag.h"

#include "modem_mem.h"
#include "rfcommon_msg.h"
#include "stringl.h"

#include "fs_lib.h"
#include "fs_public.h"
#include <stdio.h>  
#include <memory.h> 
#include "zlib.h"
#include "rfcommon_nv_mm.h"
#include "rfca_defs.h"
#include "rfcommon_efs.h"
#include <math.h>



/*Bypass EFS read; will be replaced by RAMDISK Solution*/

#include "rfrumi.h"
 

#define RFNV_CONFIG_FILE_PATH "/nv/item_store/rfnv"         // Path of RFNV config file for QPST backup
#define RFNV_CONFIG_FILE RFNV_CONFIG_FILE_PATH "/rfnv.bl"   // Path and name of RFNV config file
#define RFC_BL_FILE RFNV_CONFIG_FILE_PATH "/rfc.bl"         // Path and name of RFC bl file
#define RFC_FILE_PATH "/rfc/"

// A runtime flag to switch between EFS read and rumi NV read(hardcoded values)
static volatile uint8 use_rumi_nv_read =1;

boolean rfcommon_nv_parse_offset_data_from_efs(const char *data,   int32 data_size);

FTM_RF_Mode_Enum 
rfcommon_convert_band_tech_to_ftm_rf_mode
(
  rfcom_mode_enum_type	mode,
  rfcom_band_type_u		  band
);


boolean
rfcommon_nv_load_gnss_blanking_control
(
  rex_tcb_type           *task_ptr,
  rex_sigs_type          task_wait_sig,
  void                   (*task_wait_handler)( rex_sigs_type )
);


boolean rfcommon_nv_band_has_rx_static
(
   const rfcommon_rfnv_items_list_type *rfnv_item_list, 
   uint16 rfnv_item_list_siz, 
   rfnv_item_id_enum_type *rfnv_item_id
);

boolean rfcommon_nv_band_has_rx_switch_pt_datatype
(
   rfnv_item_id_enum_type local_rfnv_item_id, 
   rfnv_item_type* rfnv_item_buf,
   rfnv_item_size_type nv_item_size
);

boolean band_has_rx_switchpt_datatype = FALSE;

FTM_RF_Mode_Enum wcdma_band_to_phone_mode_converter[RFCOM_NUM_WCDMA_BANDS] = 
{
  PHONE_MODE_WCDMA_IMT, //RFCOM_BAND_IMT
  PHONE_MODE_WCDMA_1900A, //RFCOM_BAND_1900
  PHONE_MODE_WCDMA_1800,//RFCOM_BAND_BC3
  PHONE_MODE_WCDMA_BC4,//RFCOM_BAND_BC4
  PHONE_MODE_WCDMA_800, //RFCOM_BAND_800,
  PHONE_MODE_WCDMA_BC8, //RFCOM_BAND_BC8,
  PHONE_MODE_WCDMA_BC9, //RFCOM_BAND_BC9,
  PHONE_MODE_WCDMA_1500, //RFCOM_BAND_BC11,
  PHONE_MODE_WCDMA_BC19 //RFCOM_BAND_BC19,
};

FTM_RF_Mode_Enum gsm_band_to_phone_mode_converter[RFCOM_NUM_GSM_BANDS] = 
{
  PHONE_MODE_GSM_850,//RFCOM_BAND_GSM850
  PHONE_MODE_GSM_900,//RFCOM_BAND_GSM900
  PHONE_MODE_GSM_1800,//RFCOM_BAND_GSM1800
  PHONE_MODE_GSM_1900,//RFCOM_BAND_GSM1900
  PHONE_MODE_GSM_1800//RFCOM_BAND_GSM1800_B  
};

FTM_RF_Mode_Enum lte_band_to_phone_mode_converter[RFCOM_NUM_LTE_BANDS] =
{
  PHONE_MODE_LTE_B1,//RFCOM_BAND_LTE_B1,
  PHONE_MODE_LTE_B2,//RFCOM_BAND_LTE_B2,
  PHONE_MODE_LTE_B3,//RFCOM_BAND_LTE_B3,
  PHONE_MODE_LTE_B4,//RFCOM_BAND_LTE_B4,
  PHONE_MODE_LTE_B5,//RFCOM_BAND_LTE_B5,
  PHONE_MODE_LTE_B6,//RFCOM_BAND_LTE_B6,
  PHONE_MODE_LTE_B7,//RFCOM_BAND_LTE_B7,
  PHONE_MODE_LTE_B8,//RFCOM_BAND_LTE_B8,
  PHONE_MODE_LTE_B9,//RFCOM_BAND_LTE_B9,
  PHONE_MODE_LTE_B10,//RFCOM_BAND_LTE_B10,
  PHONE_MODE_LTE_B11,//RFCOM_BAND_LTE_B11,
  PHONE_MODE_LTE_B12,//RFCOM_BAND_LTE_B12,
  PHONE_MODE_LTE_B13,//RFCOM_BAND_LTE_B13,
  PHONE_MODE_LTE_B14,//RFCOM_BAND_LTE_B14,
  PHONE_MODE_LTE_B15,//RFCOM_BAND_LTE_B15,
  PHONE_MODE_LTE_B16,//RFCOM_BAND_LTE_B16,
  PHONE_MODE_LTE_B17,//RFCOM_BAND_LTE_B17,
  PHONE_MODE_LTE_B18,//RFCOM_BAND_LTE_B18,
  PHONE_MODE_LTE_B19,//RFCOM_BAND_LTE_B19,
  PHONE_MODE_LTE_B20,//RFCOM_BAND_LTE_B20,
  PHONE_MODE_LTE_B21,//RFCOM_BAND_LTE_B21,
  PHONE_MODE_LTE_B22,//RFCOM_BAND_LTE_B22,
  PHONE_MODE_LTE_B23,//RFCOM_BAND_LTE_B23,
  PHONE_MODE_LTE_B24,//RFCOM_BAND_LTE_B24,
  PHONE_MODE_LTE_B25,//RFCOM_BAND_LTE_B25,
  PHONE_MODE_LTE_B26,//RFCOM_BAND_LTE_B26,
  PHONE_MODE_LTE_B27,//RFCOM_BAND_LTE_B27,
  PHONE_MODE_LTE_B28,//RFCOM_BAND_LTE_B28,
  PHONE_MODE_LTE_B29,//RFCOM_BAND_LTE_B29,
  PHONE_MODE_LTE_B30,//RFCOM_BAND_LTE_B30,
  PHONE_MODE_LTE_B31,//RFCOM_BAND_LTE_B31,
  PHONE_MODE_LTE_B32,//RFCOM_BAND_LTE_B32,
  PHONE_MODE_LTE_B33,//RFCOM_BAND_LTE_B33,
  PHONE_MODE_LTE_B34,//RFCOM_BAND_LTE_B34,
  PHONE_MODE_LTE_B35,//RFCOM_BAND_LTE_B35,
  PHONE_MODE_LTE_B36,//RFCOM_BAND_LTE_B36,
  PHONE_MODE_LTE_B37,//RFCOM_BAND_LTE_B37,
  PHONE_MODE_LTE_B38,//RFCOM_BAND_LTE_B38,	/* This is a full band configuration */ 
  PHONE_MODE_LTE_B39,//RFCOM_BAND_LTE_B39,
  PHONE_MODE_LTE_B40,//RFCOM_BAND_LTE_B40,	/* This is a full band configuration */ 
  PHONE_MODE_LTE_B41,//RFCOM_BAND_LTE_B41,	/* This is a full band configuration */
  PHONE_MODE_LTE_B42,//RFCOM_BAND_LTE_B42,
  PHONE_MODE_LTE_B43,//RFCOM_BAND_LTE_B43,
  PHONE_MODE_MAX,//RFCOM_BAND_LTE_B44,
  PHONE_MODE_LTE_B28,//RFCOM_BAND_LTE_B28_B = 44, /* This is a split band configuration */
  PHONE_MODE_LTE_B40,//RFCOM_BAND_LTE_B40_B = 45, /* This is a split band configuration */
  PHONE_MODE_LTE_B41,//RFCOM_BAND_LTE_B41_B = 46, /* This is a split band configuration */
  PHONE_MODE_LTE_B41,//RFCOM_BAND_LTE_B41_C = 47, /* This is a split band configuration */
  PHONE_MODE_LTE_B39_B,//RFCOM_BAND_LTE_B39_B = 48, /* This is a split band configuration */
  
  /* LTE-U Bands */
  PHONE_MODE_MAX,//RFCOM_BAND_LTE_B125 = 49,
  PHONE_MODE_MAX,//RFCOM_BAND_LTE_B126 = 50,
  PHONE_MODE_MAX,//RFCOM_BAND_LTE_B127 = 51,
  
  /* Tentative definition for new LTE-U bands */
  PHONE_MODE_MAX,//RFCOM_BAND_LTE_B253 = 52, 
  PHONE_MODE_MAX,//RFCOM_BAND_LTE_B254 = 53,
  PHONE_MODE_LTE_B66,//RFCOM_BAND_LTE_B66 = 54,
  PHONE_MODE_MAX,
  PHONE_MODE_MAX,
  PHONE_MODE_MAX,
  PHONE_MODE_LTE_B71 //RFCOM_BAND_LTE_B71 = 58, /*To keep In line with ATLAS definition*/
};

FTM_RF_Mode_Enum cdma_band_to_phone_mode_converter[RFM_CDMA_MAX_BAND] =
{
  PHONE_MODE_CDMA_800,//RFM_CDMA_BC0  = 0,  /*!< 800 MHz Band                             */
  PHONE_MODE_CDMA_1900,//RFM_CDMA_BC1  = 1,  /*!< 1900 MHz Band                            */
  PHONE_MODE_MAX,//RFM_CDMA_BC2  = 2,  /*!< TACS Band                                */
  PHONE_MODE_J_CDMA,//RFM_CDMA_BC3  = 3,  /*!< JTACS Band                               */
  PHONE_MODE_MAX,//RFM_CDMA_BC4  = 4,  /*!< Korean PCS Band                          */
  PHONE_MODE_CDMA_450,//RFM_CDMA_BC5  = 5,  /*!< 450 MHz Band                             */
  PHONE_MODE_MAX,//RFM_CDMA_BC6  = 6,  /*!< 2 GHz Band                               */
  PHONE_MODE_MAX,//RFM_CDMA_BC7  = 7,  /*!< Upper 700 MHz Band                       */
  PHONE_MODE_CDMA_1800,//RFM_CDMA_BC8  = 8,  /*!< 1800 MHz Band                            */
  PHONE_MODE_MAX,//RFM_CDMA_BC9  = 9,  /*!< 900 MHz Band                             */
  PHONE_MODE_CDMA_800_SEC,//RFM_CDMA_BC10 = 10, /*!< Secondary 800 MHz Band                   */
  PHONE_MODE_CDMA_450_EXT,//RFM_CDMA_BC11 = 11, /*!< 400 MHz European PAMR Band               */
  PHONE_MODE_MAX,//RFM_CDMA_BC12 = 12, /*!< 800 MHz PAMR Band                        */
  PHONE_MODE_CDMA_IMT,//RFM_CDMA_BC13 = 13, /*!< 2.5 GHz IMT-2000 Extension Band          */
  PHONE_MODE_CDMA_1900_EXT,//RFM_CDMA_BC14 = 14, /*!< US PCS 1.9GHz Band                       */
  PHONE_MODE_CDMA_BC15,//RFM_CDMA_BC15 = 15, /*!< AWS Band                                 */
  PHONE_MODE_MAX,//RFM_CDMA_BC16 = 16, /*!< US 2.5GHz Band                           */
  PHONE_MODE_MAX,//RFM_CDMA_BC17 = 17, /*!< US 2.5GHz Forward Link Only Band         */
  PHONE_MODE_MAX,//RFM_CDMA_BC18 = 18, /*!< 700 MHz Public Safety Band               */
  PHONE_MODE_MAX,//RFM_CDMA_BC19 = 19, /*!< Lower 700 MHz Band                       */
  PHONE_MODE_MAX //RFM_CDMA_BC20 = 20, /*!< L-Band                                   */
};

FTM_RF_Mode_Enum tdscdma_band_to_phone_mode_converter[RFCOM_NUM_TDSCDMA_BANDS] =
{
  PHONE_MODE_TDSCDMA_B34,//RFCOM_BAND_TDSCDMA_B34,
  PHONE_MODE_TDSCDMA_B39,//RFCOM_BAND_TDSCDMA_B39,
  PHONE_MODE_TDSCDMA_B40,//RFCOM_BAND_TDSCDMA_B40,  /* This is a full band configuration */
  /* Partial band configuration begins after Band B40  */
  PHONE_MODE_TDSCDMA_B40,//RFCOM_BAND_TDSCDMA_B40_B = 3, /* This is a split band configuration */
  PHONE_MODE_TDSCDMA_B34,//RFCOM_BAND_TDSCDMA_B34_B,
  PHONE_MODE_TDSCDMA_B39//RFCOM_BAND_TDSCDMA_B39_B
};

FTM_RF_Mode_Enum *rf_tech_band_to_phone_mode_converter[RFM_NUM_MODES] =
{
  NULL,//RFM_PARKED_MODE,   /*!< Parked/sleep mode */
  wcdma_band_to_phone_mode_converter,//RFM_IMT_MODE,      /*!< WCDMA @todo depricate this enum @deprecated  */
  gsm_band_to_phone_mode_converter,//RFM_EGSM_MODE,     /*!< GSM @todo depricate this enum @deprecated  */
  cdma_band_to_phone_mode_converter,//RFM_1X_MODE,       /*!< 1X Mode */
  NULL,//RFM_GPS_MODE,      /*!< GPS Mode @todo depricate this enum @deprecated  */
  cdma_band_to_phone_mode_converter,//RFM_1XEVDO_MODE,   /*!< HDR Rev 0, A, and B modes */
  cdma_band_to_phone_mode_converter,//RFM_SVDO_MODE,     /*!< Mode to support simultaneous 1x and HDR transmit on a single device. */
  NULL,//RFM_GNSS_MODE,     /*!< Global navigation systems mode. */
  lte_band_to_phone_mode_converter,//RFM_LTE_MODE,      /*!< UMTS Long Term Evolution mode. */
  lte_band_to_phone_mode_converter,//RFM_LTE_FTM_MODE,  /*!< UMTS Long Term Evolution FTM Mode */
  tdscdma_band_to_phone_mode_converter,//RFM_TDSCDMA_MODE,  /*!< TDSCDA mode */
  tdscdma_band_to_phone_mode_converter,//RFM_TDSCDMA_MODE_FTM=14,/*!< TDSCDMA FTM NSmode */
  gsm_band_to_phone_mode_converter,//RFM_EGSM_MODE_2,   /*!< Second GSM mode for DSDA case for RF-MCPM interface */
  gsm_band_to_phone_mode_converter,//RFM_EGSM_MODE_3,   /*!< Third GSM mode for TSTS/TSDA case for RF-MCPM interface */
  NULL,//RFM_WLAN_MODE,     /*!< WLAN Mode */ 
  wcdma_band_to_phone_mode_converter,//RFM_IMT_MODE_2,    /*!< Second WCDMA in W+W mode*/
  lte_band_to_phone_mode_converter,//RFM_LTE_MODE_2,    /*!< Second LTE in L+L mode*/
};

rfcommon_asdiv_offset_char_data *asdiv_offset_char_data = NULL;
uint16 asdiv_offset_char_data_size = 0;

// Copied from nvdiag.c to add basic security checks which are not exposed via public APIS from NV/DIAG
/*===========================================================================

FUNCTION RFCOMMON_SP_NV_WRITE_ITEM

DESCRIPTION
  This procedure checks the given nv item type to see if it's a Service
  Programming item, which should not be written until the SPC
  has been entered, or security unlocked.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if it is a Service Programming item,
  FALSE if not

SIDE EFFECTS
  None.

============================================================================*/
static boolean rfcommon_sp_nv_write_item (
  nv_items_enum_type item
)
{
  switch (item) {
    case NV_SLOT_CYCLE_INDEX_I:
    case NV_CDMA_SID_LOCK_I:
    case NV_CDMA_SID_ACQ_I:
    case NV_ANALOG_SID_LOCK_I:
    case NV_ANALOG_SID_ACQ_I:
    case NV_ANALOG_FIRSTCHP_I:
    case NV_ANALOG_HOME_SID_I:
    case NV_ANALOG_REG_I:
    case NV_PCDMACH_I:
    case NV_SCDMACH_I:
    case NV_NAM_CHKSUM_I:
    case NV_MIN1_I:
    case NV_MIN2_I:
    case NV_MOB_TERM_HOME_I:
    case NV_MOB_TERM_FOR_SID_I:
    case NV_MOB_TERM_FOR_NID_I:
    case NV_ACCOLC_I:
    case NV_HOME_SID_NID_I:
    case NV_SID_NID_I:
    case NV_MIN_CHKSUM_I:
    case NV_SEC_CODE_I:
    case NV_IMSI_MCC_I:
    case NV_IMSI_11_12_I:
    case NV_DIR_NUMBER_I:
    case NV_FSC_I:
    case NV_WDC_I:
    case NV_FSC2_I:
    case NV_FSC2_CHKSUM_I:
    case NV_IMSI_ADDR_NUM_I:
    case NV_DIR_NUMBER_PCS_I:
    case NV_OTKSL_I:
    case NV_RTRE_CONFIG_I:
    case NV_UIM_PREF_PROTOCOL_I:
    case NV_PAP_USER_ID_I:
    case NV_PAP_PASSWORD_I:
    case NV_A_KEY_I:
    case NV_A_KEY_CHKSUM_I:
    case NV_SSD_A_I:
    case NV_SSD_B_I:
    case NV_UP_KEY_I:
    case NV_ROAMING_LIST_I:
    case NV_ROAMING_LIST_683_I:   
    case NV_HDR_AN_AUTH_PASSWORD_I:
    case NV_HDR_AN_AUTH_PASSWD_LONG_I:
    case NV_HDR_AN_PPP_PASSWORD_I:
    case NV_PPP_PASSWORD_I:
    case NV_DS_MIP_SS_USER_PROF_I:
    case NV_DS_MIP_DMU_MN_AUTH_I:
    case NV_DEVICE_SERIAL_NO_I:
    case NV_HDRSCMDB_SECURE_CONFIG_I:
    case NV_GPSONE_PASSWORD_I:
    case NV_GPS1_SEC_UPDATE_RATE_I:

      return (TRUE);

    default:
      return (FALSE);
  }
} /* sp_nv_write_item */


/*===========================================================================

FUNCTION RFCOMMON_SP_NV_READ_ITEM

DESCRIPTION
  This procedure checks the given nv item type to see if it's a Service
  Programming item, which should not be read until the SPC has been
  entered, or security unlocked.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if it is a Service Programming item,
  FALSE if not

SIDE EFFECTS
  None.

============================================================================*/
static boolean rfcommon_sp_nv_read_item (
  nv_items_enum_type item
)
{
  switch (item) {
    case NV_SEC_CODE_I:
    case NV_FSC_I:
    case NV_FSC2_I:
    case NV_FSC2_CHKSUM_I:
    case NV_OTKSL_I:
    case NV_PAP_USER_ID_I:
    case NV_PPP_USER_ID_I:
    case NV_HDR_AN_AUTH_NAI_I:
    case NV_DEVICE_SERIAL_NO_I:

      return (TRUE);

    default:
      return (FALSE);
  }
} /* sp_nv_read_item */

/*===========================================================================

FUNCTION RFCOMMON_UNREADABLE_NV_ITEM

DESCRIPTION
  This procedure checks the given nv item type to see if it's one of
  the few items which are never allowed to be read.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if it is unreadable.
  FALSE if it's not.

SIDE EFFECTS
  None.

============================================================================*/
boolean rfcommon_unreadable_nv_item (
  nv_items_enum_type item
)
{
  switch (item) {

    /* Keys, secrets, and passwords are unreadable */
    case NV_A_KEY_I:
    case NV_A_KEY_CHKSUM_I:
    case NV_SSD_A_I:
    case NV_SSD_B_I:
    case NV_UP_KEY_I:
#ifndef FEATURE_FFA_DEBUG_KEYS
    case NV_HDR_AN_AUTH_PASSWORD_I:
    case NV_HDR_AN_AUTH_PASSWD_LONG_I:
    case NV_HDR_AN_PPP_PASSWORD_I:
    case NV_PPP_PASSWORD_I:
    case NV_PAP_PASSWORD_I:
    case NV_DS_MIP_SS_USER_PROF_I:
    case NV_DS_MIP_DMU_MN_AUTH_I:
#endif
    case NV_SEC_DEVICE_KEY_I :

    /* We can't read the roaming lists using the usual nv_read command */
    case NV_ROAMING_LIST_I:
    case NV_ROAMING_LIST_683_I:

    /* GPS passwords are unreadable */
    case NV_GPSONE_PASSWORD_I:
    case NV_GPS1_SEC_UPDATE_RATE_I:
      
      return (TRUE);

    default:
      return (FALSE);
  }
} /* unreadable_nv_item */

/*===========================================================================

FUNCTION RFCOMMON_UNWRITABLE_NV_ITEM

DESCRIPTION
  This procedure checks the given nv item type to see if it's one of
  the few items which are never allowed to be written. 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if it is unreadable.
  FALSE if it's not.

SIDE EFFECTS
  None.

============================================================================*/
boolean rfcommon_unwritable_nv_item (
  nv_items_enum_type item
)
{
  switch (item) {

    /* Keys that are not unwritable */
    case NV_SEC_DEVICE_KEY_I :
    case NV_DIAG_SPC_UNLOCK_TTL_I:
    case NV_DIAG_SPC_TIMEOUT_I:
    case NV_SECTIME_TIME_OFFSETS_I:
       return (TRUE);

    default:
      return (FALSE);
  }
} /* unwritable_nv_item */

/*===========================================================================

FUNCTION RFCOMMON_CHECK_NV_WRITE_IS_OK 

DESCRIPTION
  This procedure checks if NV item can be written.

DEPENDENCIES
  None.

RETURN VALUE
  Success: TRUE
  Fail: FALSE

SIDE EFFECTS
  None.

============================================================================*/
boolean rfcommon_check_nv_write_is_ok (
  nv_items_enum_type item
)
{   
  /*-----------------------------------------------------------------------
    First check to see if this item is not allowed to be read.  If it isn't,
    we will return an error packet.  NOTE:  These items are unwritable,
    even if security is unlocked!
  -----------------------------------------------------------------------*/
  if ( rfcommon_unwritable_nv_item(item) ) {
    return FALSE;
  }

  /*-----------------------------------------------------------------------
    Next check to see if this item we're writing is a SP item, and if
    it is, make sure the SPC has been entered properly first, or that
    security is unlocked.  If it hasn't, we will return an error packet.
    Also make sure the security password is not unwritten.  If the security
    password is unwritten, then we rely on the SPC state flag.
  -----------------------------------------------------------------------*/
  else if ( rfcommon_sp_nv_write_item(item) && (diag_get_sp_state() == DIAG_SPC_LOCKED ) &&
       ((diag_get_security_state() == DIAG_SEC_LOCKED) || diag_check_password (NULL) == TRUE))
  {
    return FALSE;
  }
  /*-----------------------------------------------------------------------
    Next, see if the item being written is the ESN.
    We can't ever write the ESN if security is locked.
    Return the error packet for invalid security mode.
  -----------------------------------------------------------------------*/
  else if ((item == NV_ESN_I || item == NV_MEID_I) && diag_get_security_state() == DIAG_SEC_LOCKED)
  {
    return FALSE;
  }
  
  return TRUE;
}


/*===========================================================================

FUNCTION RFCOMMON_CHECK_NV_READ_IS_OK 

DESCRIPTION
  This procedure checks if NV item can be read.

DEPENDENCIES
  None.

RETURN VALUE
  Success: TRUE
  Fail: FALSE

SIDE EFFECTS
  None.

============================================================================*/
boolean rfcommon_check_nv_read_is_ok (
  nv_items_enum_type item
)
{
  /*-----------------------------------------------------------------------
    First check to see if this item is not allowed to be read.  If it isn't,
    we will return an error packet.  NOTE:  These items are unreadable,
    even if security is unlocked!
  -----------------------------------------------------------------------*/
  if ( rfcommon_unreadable_nv_item(item) ) {
    return FALSE;
  }
   
  /*-----------------------------------------------------------------------
    Next check to see if this item we're requesting is a SP item, and if
    it is, make sure the SPC has been entered properly first, or that
    security is unlocked.  If it hasn't, we will return an error packet.
    Also make sure the security password is not unwritten.  If the security
    password is unwritten, then we rely on the SPC state flag.
  -----------------------------------------------------------------------*/
  else  if (rfcommon_sp_nv_read_item(item) &&
       (diag_get_sp_state() == DIAG_SPC_LOCKED) &&
       ((diag_get_security_state() == DIAG_SEC_LOCKED) ||
        diag_check_password (NULL) == TRUE))
  {
    return FALSE;
  }

  return TRUE;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function set a specific NV item to NV.

  @details

  @param item_code : Item to set
  @param item_ptr  : Pointer to the item
  @param task_ptr  : Calling task's TCB pointer
  @param task_wait_sig : Task signal to wait for when reading NV.
  @param task_wait_handler : Task's wait function to be called when reading NV
 
  @retval
  NV_DONE_S       - request done
  NV_NOTACTIVE_S  - item was not active
  NV_BADPARM_S    - invalid parameter 
*/
nv_stat_enum_type rfcommon_nv_set_item(nv_items_enum_type item_code, 
                             nv_item_type *item_ptr, rex_tcb_type *task_ptr, 
                             rex_sigs_type task_wait_sig,
                             void (*task_wait_handler)( rex_sigs_type ))
{
  /* Command buffer to NV */
  static nv_cmd_type  nv_cmd_buf;

  if(FALSE == rfcommon_check_nv_write_is_ok(item_code))
  {
    return NV_FAIL_S;
  }

  /* Prepare command buffer to NV. */
  nv_cmd_buf.cmd = NV_WRITE_F;               /* Write request        */
  nv_cmd_buf.tcb_ptr = task_ptr;             /* Notify back to me    */
  nv_cmd_buf.sigs = task_wait_sig;           /* With this signal     */
  nv_cmd_buf.done_q_ptr = NULL;              /* No buffer to return  */
  nv_cmd_buf.item = item_code;               /* Item to set          */
  nv_cmd_buf.data_ptr = item_ptr;            /* Where to return it   */

  /* Clear signal, issue the command, and wait for the respone. */
  /* Clear signal for NV  */
  (void) rex_clr_sigs( task_ptr, task_wait_sig );

  /* Issue the request    */
  nv_cmd( &nv_cmd_buf );                       

  /* Wait for completion  */
  if(task_wait_handler!=NULL)
  {
    task_wait_handler( task_wait_sig );                
  }

  /* clear the signal again */
  (void) rex_clr_sigs( task_ptr, task_wait_sig );

  /* return status */
   return (nv_cmd_buf.status);
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function performs a Single NV Read operation to retrieve the requested 
  item.

  @details

  @param item_code : Item to get
  @param item_ptr  : Pointer where to put the item 
  @param task_ptr  : Calling task's TCB pointer
  @param task_wait_sig : Task signal to wait for when reading NV.
  @param task_wait_handler : Task's wait function to be called when reading NV 

  @retval
    NV_DONE_S       - request done
    NV_NOTACTIVE_S  - item was not active
    NV_BADPARM_S    - invalid parameter
*/


LOCAL nv_stat_enum_type rfcommon_nv_get_item(nv_items_enum_type item_code,
                                                nv_item_type *item_ptr,
                                                rex_tcb_type *task_ptr,
                                                rex_sigs_type task_wait_sig,
                                                void (*task_wait_handler)( rex_sigs_type ))
{
   /* Command buffer to NV */
   static nv_cmd_type  nv_cmd_buf;
   if(FALSE == rfcommon_check_nv_read_is_ok(item_code))
   {
      return NV_FAIL_S;
   }
   
   #ifndef T_RUMI_EMULATION
   use_rumi_nv_read = 0; /* if full-stack build, use the EFS operation by default */
   #endif

   #ifdef T_RUMI_EMULATION
   
   if( use_rumi_nv_read == 1 )
   {
    /*Bypass EFS read */
      boolean rfrumi_nv_get_success = FALSE;
      rfrumi_nv_get_success = rfrumi_nv_get_item((rfnv_item_id_enum_type)item_code, item_ptr);
      if (rfrumi_nv_get_success)
      {
         MSG_1(MSG_SSID_RF, MSG_LEGACY_MED, "RF NV item %d found in rumi nv list", item_code);
         return(NV_DONE_S);
      }
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "RF NV item %d is not found in rumi nv list", item_code);
      return(NV_BADTG_S);
    }

    #endif
  

   /* Prepare command buffer to NV. */
   nv_cmd_buf.cmd = NV_READ_F;                /* Read request         */
   nv_cmd_buf.tcb_ptr = task_ptr;             /* Notify back to me    */
   nv_cmd_buf.sigs = task_wait_sig;           /* With this signal     */
   nv_cmd_buf.done_q_ptr = NULL;              /* No buffer to return  */
   nv_cmd_buf.item = item_code;               /* Item to get          */
   nv_cmd_buf.data_ptr = item_ptr;            /* Where to return it   */

   /* Clear signal, issue the command, and wait for the respone. */
   /* Clear signal for NV  */
   (void) rex_clr_sigs(task_ptr, task_wait_sig);

   /* Issue the request    */
   nv_cmd(&nv_cmd_buf);

   /* Wait for completion  */
   if(task_wait_handler != NULL)
   {
     task_wait_handler(task_wait_sig);
   }
   /* clear the signal again */
   (void) rex_clr_sigs( task_ptr, task_wait_sig );

   /* return status */
   return (nv_cmd_buf.status);
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  

  @details

  @param task_ptr  : Calling task's TCB pointer
  @param task_wait_sig : Task signal to wait for when reading NV.
  @param task_wait_handler : Task's wait function to be called when reading NV 

  @retval
    NV_DONE_S       - request done
    NV_NOTACTIVE_S  - item was not active

*/

rfcommon_nv_status_type 
rfcommon_nv_retrieve_items(rfcommon_nv_info_type *nv_info_table, 
                           int16 nv_info_table_size, rex_tcb_type *task_ptr, 
                           rex_sigs_type task_wait_sig,
                           void (*task_wait_handler)( rex_sigs_type ))
{
   nv_item_type nv_item_buf; /* NV item buffer */
   nv_stat_enum_type nv_read_status; /* NV read status */
   int16 nv_item_cnt; /* loop counter for RF NV item fetches */

   /* Pointer to the list of RF NV items */
   const rfcommon_nv_items_list_type *nv_item_list;  
   int16 nv_item_list_siz; /* Number of items in RF NV items list */
   int16 table_index; /* index to each mode */
   rfcommon_nv_status_type status = RFCOMMON_NV_READ_SUCCESS;

   for (table_index = 0; table_index < nv_info_table_size; table_index++) 
   {
     if (nv_info_table[table_index].nv_data_valid_flag == TRUE)
     {
       /* get the pointer to NV items list for each mode */
       nv_item_list =  nv_info_table[table_index].nv_item_list_ptr;

       /* get the list size */
       if( nv_info_table[table_index].get_nv_item_list_size != NULL)
         nv_item_list_siz = (nv_info_table[table_index].get_nv_item_list_size)();
       else
         nv_item_list_siz = 0;

       /* for each NV item in the list, read from NV and copy into local buffer. */
       /* then copy from local buffer into RF NV table for each mode */
       for (nv_item_cnt = 0; nv_item_cnt < nv_item_list_siz; nv_item_cnt++)
       {
         /* read from NV into local buffer */
         nv_read_status = rfcommon_nv_get_item(nv_item_list[nv_item_cnt].nv_item_id, 
                                               &nv_item_buf,
                                               task_ptr,
                                               task_wait_sig,
                                               task_wait_handler );

         /* if read status is good */
         if (nv_read_status == NV_DONE_S)
         {
            /* copy from local buffer into RF NV table */
            nv_info_table[table_index].copy_nv_item( nv_item_list[nv_item_cnt].nv_item_id, &nv_item_buf ); 
         }
         else if (nv_read_status == NV_NOTACTIVE_S)
         {
            MSG_ERROR( "RF NV item %d for mode %d is not available, attr=0x%x",
                       nv_item_list[nv_item_cnt].nv_item_id, table_index, 
                       nv_item_list[nv_item_cnt].nv_item_attrib );

            if (nv_item_list[nv_item_cnt].nv_item_attrib & NV_ITEM_ATTRIB_CRTCL_BMSK)
            {
               nv_info_table[table_index].nv_data_valid_flag = FALSE;
               status = RFCOMMON_NV_INACTIVE_ITEM;
            }
         }
         else
         {
            MSG_ERROR( "NV read failed on RF NV item %d for mode %d, , attr=0x%x", 
                       nv_item_list[nv_item_cnt].nv_item_id, table_index, 
                       nv_item_list[nv_item_cnt].nv_item_attrib );
            
            if (nv_item_list[nv_item_cnt].nv_item_attrib & NV_ITEM_ATTRIB_CRTCL_BMSK)
            {
               nv_info_table[table_index].nv_data_valid_flag = FALSE;
               status = RFCOMMON_NV_READ_FAILED;
            }
         }

       } /*for (nv_item_cnt = 0; nv_item_cnt < nv_item_list_siz; nv_item_cnt++)*/
     } /* rfnv_is_nv_data_valid */
   } /*for (mode_idx = 0; mode_idx < RFNV_SUPPORTED_MODE_NUM; mode_idx++)*/

   return status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Function creates a file and writes data_size bytes to it from data buffer.
 
  @details
  Support function for rfnv EFS operations
 
  @retval
    Returns 0 if the item is written successfully, or -1 for error. 
*/
static int32 rfcommon_rfnv_create_file(const char *file, const byte *data, int32 data_size)
{
  int fd = -1;

  fd = efs_open (file, O_TRUNC | O_WRONLY | O_CREAT, 0777);
  if (fd < 0)
  {
    return -1;
  }

  if (data_size != efs_write (fd, data, (fs_size_t)data_size))
  {
    (void) efs_close (fd);
    return -1;
  }
  
  return efs_close (fd);
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Prepares EFS for RFNV

  @details
  Function creates config file for RFNV items back from EFS. Config file will
  inidicate the folder from where all RFNV items should be backed up.
 
  - File to be created: "/nv/item_store/rfnv/rfnv.bl"
  - File content: "/nv/item_files/rfnv/"

 
  @retval
    Returns >=0 if the item is written successfully, or <0 for error. 
*/
int32 rfcommon_rfnv_init(void)
{
  int status = 0;
  struct fs_stat temp_buf;

  if(efs_stat(RFNV_CONFIG_FILE, &temp_buf) == -1)
  {
    // Force path creation for RFNV.BL file
    status |= efs_put (RFNV_CONFIG_FILE, RFNV_FILE_PATH, strlen(RFNV_FILE_PATH), O_CREAT | O_AUTODIR, 0777);
    status |= efs_unlink(RFNV_CONFIG_FILE);
    // We want the RFNV.BL file be a normal file
    status |= rfcommon_rfnv_create_file(RFNV_CONFIG_FILE, (byte*)RFNV_FILE_PATH, (int32)strlen(RFNV_FILE_PATH));
  }

  if(efs_stat(RFNV_FILE_PATH, &temp_buf) == -1)
  {
    // Lets do the same for RFNV file path
    char fn[RFNV_MAX_PATH] = "";    
    snprintf(fn, RFNV_MAX_PATH, "%s%s", RFNV_FILE_PATH, "BAADFOOD"); 
    status |= efs_put (fn, RFNV_FILE_PATH, strlen(RFNV_FILE_PATH), O_CREAT | O_AUTODIR, 0777);
    status |= efs_unlink(fn);
  }

  if(efs_stat(RFC_BL_FILE, &temp_buf) == -1)
  {
  // Force path creation for RFC.BL file
    status |= efs_put (RFC_BL_FILE,RFC_FILE_PATH, strlen(RFC_FILE_PATH), O_CREAT | O_AUTODIR, 0777);
    status |= efs_unlink(RFC_BL_FILE);
    // We want the RFC.BL file be a normal file
    status |= rfcommon_rfnv_create_file(RFC_BL_FILE,(byte *)RFC_FILE_PATH, (int32)strlen(RFC_FILE_PATH));
  }

  return status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Writes RFNV item
 
  @details
  This function sets a specific RFNV item to NV or EFS. If item_code is
  greater than RFNV_USE_EFS_NV_CUTOFF then item is written to EFS, otherwise
  the old NV storage is used.
 
  - Check if item id is belowe cutoff and if it is writes NV item to old NV.
  - Creates path to EFS item by appending RFNV_FILE_PATH and item_code.
    File names are right-padded with zeros if needed to make the lenght to be 8.
  - Writes item to EFS and returns write status

 
  @param item_code          : Item to set
  @param item_ptr           : Pointer to the item
  @param item_size          : Size of RFNV item in bytes
  @param task_ptr           : Calling task's TCB pointer
  @param task_wait_sig      : Task signal to wait for when reading NV.
  @param task_wait_handler  : Task's wait function to be called when reading NV
 
  @retval
    NV_DONE_S       - request done
    NV_NOTACTIVE_S  - item was not active
    NV_BADPARM_S    - invalid parameter
*/
nv_stat_enum_type rfcommon_rfnv_set_item
(
  rfnv_item_id_enum_type  item_code,       /* Item to set */
  rfnv_item_type          *item_ptr,       /* Pointer to the item */
  rfnv_item_size_type     item_size,       /* Size of item in bytes */  
  rex_tcb_type            *task_ptr,       /* Calling task's TCB pointer */
  rex_sigs_type           task_wait_sig,   /* Task signal to wait for when reading NV */
  void                    (*task_wait_handler)( rex_sigs_type ) /* Task's wait function to be called when reading NV */
)
{

  if( (int)item_code < RFNV_USE_EFS_NV_CUTOFF )
  {
    return rfcommon_nv_set_item((nv_items_enum_type)item_code, (nv_item_type*)item_ptr, task_ptr, task_wait_sig, task_wait_handler);      
  }
  else
  {
    char path[RFNV_MAX_PATH] = "";
      
    if (RFNV_DATA_SIZE_MAX < item_size)
    {
      return NV_FAIL_S;
    }
  
    (void) snprintf(path, RFNV_MAX_PATH, "%s%08d", RFNV_FILE_PATH, item_code); 
    path[RFNV_MAX_PATH-1] = 0;
      
    if (-1 == efs_put (path, item_ptr, item_size, O_CREAT | O_AUTODIR, 0777))
    {
      return NV_FAIL_S;
    }

    return NV_DONE_S;
  }
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Reads RFNV item

  @details
  This function performs a Single RFNV Read operation to retrieve the requested 
  item. If item_code is greater than RFNV_USE_EFS_NV_CUTOFF then item is read from EFS,
  otherwise the old NV storage is used.
 
  - Creates path to EFS item by appending RFNV_FILE_PATH and item_code.
    File names are right-padded with zeros if needed to make the lenght to be 8.
  - Reads at most RFNV_DATA_SIZE_MAX bytes to item_ptr
  - Return the status of RFNV read.

  @param item_code
  Item to get

  @param item_ptr
  Pointer where to put the item

  @param item_size
  An in/out parameter specifying the maximum size in bytes of the buffer to
  load the NV item into as the input, and then the actual size of the item
  in bytes after the item has been read.

  @param task_ptr
  Calling task's TCB pointer

  @param task_wait_sig
  Task signal to wait for when reading NV.

  @param task_wait_handler
  Task's wait function to be called when reading NV 

  @retval
    NV_DONE_S       - request done
    NV_NOTACTIVE_S  - item was not active
    NV_FAIL_S       - failed to read existing item from EFS
    NV_BADPARM_S    - invalid parameter

*/
nv_stat_enum_type
rfcommon_rfnv_get_item
(
  rfnv_item_id_enum_type item_code,
  rfnv_item_type         *item_ptr,
  rfnv_item_size_type    *item_size,       
  rex_tcb_type           *task_ptr,
  rex_sigs_type          task_wait_sig,
  void                   (*task_wait_handler)( rex_sigs_type )
)
{
   
   #ifndef T_RUMI_EMULATION
   use_rumi_nv_read = 0; /* if full-stack build, use the EFS operation by default */
   #endif
   
   #ifdef T_RUMI_EMULATION
   if(use_rumi_nv_read == 1)
  {
  /*Bypass EFS read */
      boolean rfrumi_nv_get_success = FALSE;
      rfrumi_nv_get_success = rfrumi_rfnv_get_item(item_code, item_ptr, item_size);
      if (rfrumi_nv_get_success)
      {
         MSG_1(MSG_SSID_RF, MSG_LEGACY_MED, "RF NV item %d found in rumi nv list", item_code);
         return(NV_DONE_S);
      }
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "RF NV item %d is not found in rumi nv list", item_code);
      return(NV_BADTG_S);
    }
    #endif
   
   
  if( (int)item_code < RFNV_USE_EFS_NV_CUTOFF )
  {
    if ( *item_size >= 128 )
    {
      nv_stat_enum_type status;
      status = rfcommon_nv_get_item((nv_items_enum_type)item_code, (nv_item_type*)item_ptr, task_ptr, task_wait_sig, task_wait_handler);
      *item_size = 128;
      return status;
    }
    else
    {
      *item_size = 0;
      return NV_FAIL_S;
    }
  } /* if( item_code < RFNV_USE_EFS_NV_CUTOFF ) */
  else
  {
    char path[RFNV_MAX_PATH];
    int data_size = -1;
    struct fs_stat temp_buf;

    (void) snprintf(path, RFNV_MAX_PATH, "%s%08d", RFNV_FILE_PATH, item_code);
    path[RFNV_MAX_PATH-1] = 0;

    if(efs_stat(path,&temp_buf) == -1) 
    {
      *item_size = 0;
      return NV_NOTACTIVE_S;
    }

    if (*item_size < temp_buf.st_size)
    {
      *item_size = 0;
      return NV_FAIL_S;
    }

    data_size = efs_get(path, item_ptr, temp_buf.st_size);

    if (-1 == data_size)
    {
      *item_size = 0;
      return NV_FAIL_S;
    }

    *item_size = (rfnv_item_size_type)data_size;
    return NV_DONE_S;
  } /* else if( item_code < RFNV_USE_EFS_NV_CUTOFF ) */
} /* rfcommon_rfnv_get_item() */



/*----------------------------------------------------------------------------*/
/*!
  @brief
  Reads all RFNV items for entries listed in rfnv_info_table.
  See rfcommon_rfnv_info_type and rfcommon_rfnv_items_list_type.
 
  - Iterates through the rfnv_info_table array.
  - Checks if current rfnv_info_table array item is valid, only valid items are processed.
  - Gets pointer to the RFNV item table using callback function.
  - Get the size of RFNV item table using callback function.
  - Tries to read RFNV item using rfcommon_rfnv_get_item.
  - If read was success then copy callback function is called.
  - If read failed and RFNV item was flagged with NV_ITEM_ATTRIB_CRTCL_BMSK
    then current rfnv_info_table entry declared as invalid. 

  @details

  @param rfnv_info_table      : Array of rfnv items to retrieve
  @param rfnv_info_table_size  : Size of nv_info_table
  @param task_ptr              : Calling task's TCB pointer
  @param task_wait_sig        : Task signal to wait for when reading NV.
  @param task_wait_handler    : Task's wait function to be called when reading NV 

  @retval
    RFCOMMON_NV_READ_SUCCESS   - request done
    RFCOMMON_NV_INACTIVE_ITEM  - item was not active
    RFCOMMON_NV_READ_FAILED    - read faild

*/
rfcommon_nv_status_type rfcommon_rfnv_retrieve_items(rfcommon_rfnv_info_type *rfnv_info_table, 
                           int16 rfnv_info_table_size, 
                           rex_tcb_type *task_ptr, 
                           rex_sigs_type task_wait_sig,
                           void (*task_wait_handler)( rex_sigs_type ))

{
  rfnv_item_type* rfnv_item_buf = NULL; /* RFNV item buffer */
  nv_stat_enum_type nv_read_status; /* RFNV read status */
  int16 rfnv_item_count; /* loop counter for RFNV item fetches */

  /* SAR AsDiv NV status*/
  boolean sar_asdiv_status = FALSE;
  boolean has_rx_static_nv = FALSE;
  rfnv_item_id_enum_type local_rfnv_item_id = 0;

  /* Pointer to the list of RFNV items */
  const rfcommon_rfnv_items_list_type *rfnv_item_list;  
  uint16 rfnv_item_list_siz; /* Number of items in RFNV items list */
  uint16 table_index; /* index to each mode */
  rfcommon_nv_status_type status = RFCOMMON_NV_READ_SUCCESS;
  rfnv_item_size_type rx_static_item_size = sizeof(rfnv_item_type);

  // Allocate memory from modem heap for RFNV items
  rfnv_item_buf = modem_mem_alloc(sizeof(rfnv_item_type), MODEM_MEM_CLIENT_RFA);
  if(NULL == rfnv_item_buf)
  {
    return RFCOMMON_NV_READ_FAILED;
  }

  for (table_index = 0; table_index < rfnv_info_table_size; table_index++) 
  {
    if (rfnv_info_table[table_index].rfnv_data_valid_flag == TRUE)
    {
      /* get the pointer to NV items list for each mode */
      rfnv_item_list =  rfnv_info_table[table_index].rfnv_item_list_ptr;

      /* get the list size */
      if( rfnv_info_table[table_index].get_rfnv_item_list_size != NULL)
        rfnv_item_list_siz = (rfnv_info_table[table_index].get_rfnv_item_list_size)();
      else
        rfnv_item_list_siz = 0;

      /* loop over all items in rfnv_item_list_ptr
       * if rx_static is present, go inside this nv and see if type 203 is present*/
      if (rfnv_item_list_siz != 0)
      {
        has_rx_static_nv = rfcommon_nv_band_has_rx_static(rfnv_item_list, rfnv_item_list_siz, &local_rfnv_item_id);

        if (has_rx_static_nv == TRUE)
        {
          /* Re-initialize item size before calling rfnv_get_item function */
          rx_static_item_size = sizeof(rfnv_item_type);
          
          /* read from NV into local buffer */
          nv_read_status = rfcommon_rfnv_get_item(local_rfnv_item_id, 
                                                  rfnv_item_buf,
                                                  &rx_static_item_size,
                                                  task_ptr,
                                                  task_wait_sig,
                                                  task_wait_handler );

          /* if read status is good */
          if (nv_read_status == NV_DONE_S)
          {                
            /* process this nv and find out if elements of type 203 are present */
            band_has_rx_switchpt_datatype = rfcommon_nv_band_has_rx_switch_pt_datatype(local_rfnv_item_id, 
                                                                                       rfnv_item_buf, 
                                                                                       rx_static_item_size);

          }
          else if (nv_read_status == NV_NOTACTIVE_S)
          {
              MSG_1(MSG_SSID_RF, MSG_LEGACY_LOW,"NV_203_DBG: Rx static NV %d not active ", local_rfnv_item_id);

          }
          else
          {

            MSG_2(MSG_SSID_RF, MSG_LEGACY_LOW, "NV_203_DBG: nv read failed for RF NV item %d and mode %d: ",
                  local_rfnv_item_id, table_index);
          }
        }
      }    

      /* Re-Initialize Buffer in case there was any data that was copied */
      if(rx_static_item_size > 0 && rx_static_item_size <= sizeof(rfnv_item_type))
      {
        memset((void*)rfnv_item_buf,0,rx_static_item_size);
      }

      /* for each NV item in the list, read from NV and copy into local buffer. */
      /* then copy from local buffer into RF NV table for each mode */
      for (rfnv_item_count = 0; rfnv_item_count < rfnv_item_list_siz; rfnv_item_count++)
      {
        rfnv_item_size_type item_size = sizeof(rfnv_item_type);

        /*Check if the NV item is WCDMA SAR AsDiv backoff NV*/ 
        if (rfnv_item_list[rfnv_item_count].rfnv_item_id == RFNV_WCDMA_SAR_BACK_OFF_LIMIT_ASDIV_POS)
     
        {
          sar_asdiv_status = rfcommon_load_wcdma_sar_asdiv_nv(RFM_IMT_MODE, rfnv_item_buf,rfnv_item_list[rfnv_item_count].rfnv_item_id,
                                       task_ptr,task_wait_sig,task_wait_handler);
     
          if (sar_asdiv_status == TRUE)
          {
            /* copy from local buffer into RF NV table */        
            rfnv_info_table[table_index].copy_rfnv_item( rfnv_item_list[rfnv_item_count].rfnv_item_id, rfnv_item_buf ); 
          }
          else
          {
            /*Non critical NV item, do nothing*/
            MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR, "RF NV item %d for mode %d is not available, attr=0x%x",
                  rfnv_item_list[rfnv_item_count].rfnv_item_id, table_index, 
                  rfnv_item_list[rfnv_item_count].rfnv_item_attrib );
          }
        }
        else if( rfnv_item_list[rfnv_item_count].rfnv_item_id == RFNV_GNSS_BLANKING_CONTROL_I )
        {
          rfnv_info_table[table_index].rfnv_data_valid_flag = 
            rfcommon_nv_load_gnss_blanking_control( task_ptr, task_wait_sig, task_wait_handler );
        }
        else
        {
          /* read from NV into local buffer */
          nv_read_status = rfcommon_rfnv_get_item(rfnv_item_list[rfnv_item_count].rfnv_item_id, 
                                                  rfnv_item_buf,
                                                  &item_size,
                                                  task_ptr,
                                                  task_wait_sig,
                                                  task_wait_handler );

          /* if read status is good */
          if (nv_read_status == NV_DONE_S)
          {
            /* copy from local buffer into RF NV table */        
            rfnv_info_table[table_index].copy_rfnv_item( rfnv_item_list[rfnv_item_count].rfnv_item_id, rfnv_item_buf ); 
          }
          else if (nv_read_status == NV_NOTACTIVE_S)
          {
            MSG_3(MSG_SSID_RF, MSG_LEGACY_LOW, "RF NV item %d for mode %d is not available, attr=0x%x",
                  rfnv_item_list[rfnv_item_count].rfnv_item_id, table_index, 
                  rfnv_item_list[rfnv_item_count].rfnv_item_attrib );

            if (rfnv_item_list[rfnv_item_count].rfnv_item_attrib & NV_ITEM_ATTRIB_CRTCL_BMSK)
            {
              rfnv_info_table[table_index].rfnv_data_valid_flag = FALSE;
              status = RFCOMMON_NV_INACTIVE_ITEM;
              MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR, "RF NV item %d for mode %d is not available, attr=0x%x",
                    rfnv_item_list[rfnv_item_count].rfnv_item_id, table_index, 
                    rfnv_item_list[rfnv_item_count].rfnv_item_attrib );

            }
          }
          else
          {
            MSG_3(MSG_SSID_RF, MSG_LEGACY_LOW, "NV read failed on RF NV item %d for mode %d, , attr=0x%x", 
                  rfnv_item_list[rfnv_item_count].rfnv_item_id, table_index, 
                  rfnv_item_list[rfnv_item_count].rfnv_item_attrib );
             
             if (rfnv_item_list[rfnv_item_count].rfnv_item_attrib & NV_ITEM_ATTRIB_CRTCL_BMSK)
             {
               rfnv_info_table[table_index].rfnv_data_valid_flag = FALSE;
               status = RFCOMMON_NV_READ_FAILED;
               MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR, "NV read failed on RF NV item %d for mode %d, , attr=0x%x", 
                     rfnv_item_list[rfnv_item_count].rfnv_item_id, table_index, 
                     rfnv_item_list[rfnv_item_count].rfnv_item_attrib );

            }
          }
        }
    
        /* Re-Initialize Buffer in case there was any data that was copied */
        if(item_size > 0 && item_size <= sizeof(rfnv_item_type))
        {
          memset((void*)rfnv_item_buf,0,item_size);
        }
      } /*for (rfnv_item_count = 0; rfnv_item_count < rfnv_item_list_siz; rfnv_item_count++)*/
    } /* rfnv_is_nv_data_valid */
    /* Reset the global rx_static flag */
    band_has_rx_switchpt_datatype = FALSE;
  } /*for (mode_idx = 0; mode_idx < RFNV_SUPPORTED_MODE_NUM; mode_idx++)*/

  // Free RFNV buffer
  modem_mem_free(rfnv_item_buf, MODEM_MEM_CLIENT_RFA);
  rfnv_item_buf = NULL;

  return status;
}


/*--------------------------------------------------------------------------*/
/*!
  @brief
  This function is used to load the CL Control info from the CL NV table
  based on the version

  @details

  @param rfnv_item_ptr
  Pointer to the NV buffer where data loaded from NV currently is stored

  @ant_tuner_cl_ctrl_info 
  Pointer to which the CL Control info  will be copied
  
  @return
  boolean TRUE or FALSE 
  TRUE - Memory Allocation & Loading NV table -> success
  FALSE -Failure in Memory Allocation  or loading 
*/

boolean rfcommon_load_ant_tuner_cl_ctrl_info_from_nv
(
  rfnv_item_type *rfnv_item_ptr,
  rfcommon_nv_ant_tuner_cl_ctrl_type *ant_tuner_cl_ctrl_info
)
{
  boolean status = TRUE;

  /*Sanity check */
  if(rfnv_item_ptr == NULL || ant_tuner_cl_ctrl_info == NULL )
  {
    return FALSE;
  }
  
  /*Fill the version */
  ant_tuner_cl_ctrl_info->version = ((rfcommon_nv_ant_tuner_cl_ctrl_type *)rfnv_item_ptr)->version;

  /*Allocate the memory based on the version*/
  switch(ant_tuner_cl_ctrl_info->version)
  {
    case RFCOMMON_NV_ATUNER_CL_CTRL_INFO_V_A_ID:
      /* Allocate the memory only once */
      if (ant_tuner_cl_ctrl_info->payload == NULL )
      {
        ant_tuner_cl_ctrl_info->payload = modem_mem_alloc(sizeof(rfcommon_nv_ant_tuner_cl_ctrl_data_type_v_a), 
                                            MODEM_MEM_CLIENT_RFA);
      }
      if(ant_tuner_cl_ctrl_info->payload != NULL )
      {
        /*Copy the data  after the version.*/
        memscpy((void*)ant_tuner_cl_ctrl_info->payload,
                 sizeof(rfcommon_nv_ant_tuner_cl_ctrl_data_type_v_a),
                 (void*)&(((rfcommon_nv_ant_tuner_cl_ctrl_type *)rfnv_item_ptr)->payload),
                 sizeof(rfcommon_nv_ant_tuner_cl_ctrl_data_type_v_a));    
      }
      break;

    case RFCOMMON_NV_ATUNER_CL_CTRL_INFO_V_B_ID:
      /* Allocate the memory only once */
      if (ant_tuner_cl_ctrl_info->payload == NULL )
      {
        ant_tuner_cl_ctrl_info->payload =
          modem_mem_alloc(sizeof(rfcommon_nv_ant_tuner_cl_ctrl_data_type_v_b),
                                 MODEM_MEM_CLIENT_RFA);
      }

      if(ant_tuner_cl_ctrl_info->payload != NULL )
      {
        /*Copy the data  after the version.*/
        memscpy((void*)ant_tuner_cl_ctrl_info->payload,
                 sizeof(rfcommon_nv_ant_tuner_cl_ctrl_data_type_v_b),
                 (void*)&(((rfcommon_nv_ant_tuner_cl_ctrl_type *)rfnv_item_ptr)->payload),
                 sizeof(rfcommon_nv_ant_tuner_cl_ctrl_data_type_v_b));
      }
      break;

    default:
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfcommon_load_ant_tuner_cl_ctrl_info_from_nv() Invalid version %d",
                                           ant_tuner_cl_ctrl_info->version);
      break;
  }
  return status; 
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Enum list for NVIDs of third party tuner per tech per band 
  To be updated by customer after defining appropriate NV item code
  */

rfnv_item_id_enum_type rfcommon_nv_wcdma_tuner_id_list[RFCOM_NUM_WCDMA_BANDS] =
{
   RFNV_ITEMS_ENUM_MAX,      // RFCOM_BAND_IMT                          
   RFNV_ITEMS_ENUM_MAX,      // RFCOM_BAND_1900, 
   RFNV_ITEMS_ENUM_MAX,      // RFCOM_BAND_BC3,  
   RFNV_ITEMS_ENUM_MAX,      // RFCOM_BAND_BC4,  
   RFNV_ITEMS_ENUM_MAX,      // RFCOM_BAND_800,  
   RFNV_ITEMS_ENUM_MAX,      // RFCOM_BAND_BC8,  
   RFNV_ITEMS_ENUM_MAX,      // RFCOM_BAND_BC9,  
   RFNV_ITEMS_ENUM_MAX,      // RFCOM_BAND_BC11, 
   RFNV_ITEMS_ENUM_MAX       // RFCOM_BAND_BC19, 
};

rfnv_item_id_enum_type rfcommon_nv_cdma_tuner_id_list[RFM_CDMA_MAX_BAND] =
{
   RFNV_ITEMS_ENUM_MAX,    // All band NVID should be updated as needed  
   RFNV_ITEMS_ENUM_MAX,     
   RFNV_ITEMS_ENUM_MAX,     
   RFNV_ITEMS_ENUM_MAX,
   RFNV_ITEMS_ENUM_MAX,
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX     
};


rfnv_item_id_enum_type rfcommon_nv_lte_tuner_id_list[RFCOM_NUM_LTE_BANDS] =
{
   RFNV_ITEMS_ENUM_MAX,   // All band NVID should be updated as needed  
   RFNV_ITEMS_ENUM_MAX,     
   RFNV_ITEMS_ENUM_MAX,     
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX,    
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX,    
   RFNV_ITEMS_ENUM_MAX,
   RFNV_ITEMS_ENUM_MAX,
   RFNV_ITEMS_ENUM_MAX,
   RFNV_ITEMS_ENUM_MAX,
   RFNV_ITEMS_ENUM_MAX,
   RFNV_ITEMS_ENUM_MAX,   
   RFNV_ITEMS_ENUM_MAX,
   RFNV_ITEMS_ENUM_MAX,
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX, 
   RFNV_ITEMS_ENUM_MAX,  
   RFNV_ITEMS_ENUM_MAX,  
   RFNV_ITEMS_ENUM_MAX,  
   RFNV_ITEMS_ENUM_MAX,  
   RFNV_ITEMS_ENUM_MAX,  
   RFNV_ITEMS_ENUM_MAX,  
   RFNV_ITEMS_ENUM_MAX,  
   RFNV_ITEMS_ENUM_MAX,  
   RFNV_ITEMS_ENUM_MAX,  
   RFNV_ITEMS_ENUM_MAX,  
   RFNV_ITEMS_ENUM_MAX,  
   RFNV_ITEMS_ENUM_MAX,  
   RFNV_ITEMS_ENUM_MAX,  
   RFNV_ITEMS_ENUM_MAX,  
   RFNV_ITEMS_ENUM_MAX,  
   RFNV_ITEMS_ENUM_MAX   
};

rfnv_item_id_enum_type rfcommon_nv_tdscdma_tuner_id_list[RFCOM_NUM_TDSCDMA_BANDS] =
{
   RFNV_ITEMS_ENUM_MAX,   // All band information should be updated as needed  
   RFNV_ITEMS_ENUM_MAX,     
   RFNV_ITEMS_ENUM_MAX,     
   RFNV_ITEMS_ENUM_MAX,     
};

rfnv_item_id_enum_type rfcommon_nv_gsm_tx_tuner_id_list[RFCOM_NUM_GSM_BANDS] =
{
  RFNV_ITEMS_ENUM_MAX,   // All band NVID should be updated as needed 
  RFNV_ITEMS_ENUM_MAX, 
  RFNV_ITEMS_ENUM_MAX, 
  RFNV_ITEMS_ENUM_MAX, 
};

rfnv_item_id_enum_type rfcommon_nv_gsm_rx_tuner_id_list[RFCOM_NUM_GSM_BANDS] =
{
  RFNV_ITEMS_ENUM_MAX,  // All band NVID should be updated as needed 
  RFNV_ITEMS_ENUM_MAX, 
  RFNV_ITEMS_ENUM_MAX, 
  RFNV_ITEMS_ENUM_MAX, 
};

const rfcommon_nv_lte_ca_info rfcommon_nv_lte_ca_data[] =
{
  /*To be filled as per LTE CA requirement*/
  { RFCOM_NUM_LTE_BANDS, RFCOM_NUM_LTE_BANDS, RFNV_ITEMS_ENUM_MAX },
  { RFCOM_NUM_LTE_BANDS, RFCOM_NUM_LTE_BANDS, RFNV_ITEMS_ENUM_MAX },
  /*Last element of the array is forced to a default and should be as defined here*/
};

const rfcommon_nv_wcdma_dbdc_info rfcommon_nv_wcdma_dbdc_data[] =
{
  /*To be filled as per wcdma DBDC requirement*/
  { RFCOM_NUM_WCDMA_BANDS, RFCOM_NUM_WCDMA_BANDS, RFNV_ITEMS_ENUM_MAX },
  { RFCOM_NUM_WCDMA_BANDS, RFCOM_NUM_WCDMA_BANDS, RFNV_ITEMS_ENUM_MAX },
  /*Last element of the array is forced to a default and should be as defined here*/
};
uint16 ca_nv_struc_size = sizeof(rfcommon_nv_lte_ca_data)
                          /sizeof(rfcommon_nv_lte_ca_info); 
                          /* Find size of the nv structure array this is added to 
                            avoid use of macro for size that can lead to error 
                            The variable is global to avoid recomputation if used 
                            in function.*/

uint16 dbdc_nv_struc_size = sizeof(rfcommon_nv_wcdma_dbdc_data)
                          /sizeof(rfcommon_nv_wcdma_dbdc_info); 
                          /* Find size of the nv structure array this is added to 
                            avoid use of macro for size that can lead to error 
                            The variable is global to avoid recomputation if used 
                            in function.*/

/*----------------------------------------------------------------------------*/
/*!
  @brief
  API for tuner NV item code retrieve based on band, tech and tx/rx
  API returns true or false and updates the item code.
  For now only no band information is updated for wcdma as it
  should be completed after addition of NV items.

  @details
  -The code returns the item code based on tech band and tx rx
  -The customer should take care of populating the NV item code.
  -The API returns fail if item code read is invalid.
 
  @param mode
  tech information
 
  @param band
  band of each tech
 
  @param is_rx
  is this tx or rx
  Needed for GSM case of tune code
 
  @param item_code
  pointer to return item code information
 
  @return
  True or False

*/

boolean rfcommon_nv_get_tuner_item_code
( 
   rfcom_mode_enum_type mode,
   rfcom_band_type_u band,
   boolean is_this_rx_operation,
   rfnv_item_id_enum_type *item_code  
)
{
   boolean status = FALSE;

   if (item_code == NULL)
   {
      return FALSE;
   }

   *item_code = RFNV_ITEMS_ENUM_MAX;

   switch (mode)
   {
   case RFM_IMT_MODE:
     if (band.wcdma_band < RFCOM_NUM_WCDMA_BANDS)
     {
        *item_code = rfcommon_nv_wcdma_tuner_id_list[band.wcdma_band];
     }
     break;

   case RFM_EGSM_MODE:
     if (band.gsm_band < RFCOM_NUM_GSM_BANDS)
     {
        if (TRUE == is_this_rx_operation)
        {
           *item_code = rfcommon_nv_gsm_rx_tuner_id_list[band.gsm_band];
        }

        else
        {
           *item_code = rfcommon_nv_gsm_tx_tuner_id_list[band.gsm_band];
        }

     }
     break;

   case RFM_1X_MODE:
   case RFM_1XEVDO_MODE:
   case RFM_SVDO_MODE:
     if (band.cdma_band < RFM_CDMA_MAX_BAND)
     {
        *item_code = rfcommon_nv_cdma_tuner_id_list[band.cdma_band];
     }
     break;

   case RFM_LTE_MODE:
     if (band.lte_band < RFCOM_NUM_LTE_BANDS)
     {
        *item_code = rfcommon_nv_lte_tuner_id_list[band.lte_band];
     }
     break;

   case RFM_TDSCDMA_MODE:
     if (band.tdscdma_band < RFCOM_NUM_TDSCDMA_BANDS)
     {
        *item_code = rfcommon_nv_tdscdma_tuner_id_list[band.tdscdma_band];
     }
     break;

   default:
     break;
   }

   if ( *item_code>0 && *item_code<=RFNV_USE_EFS_NV_CUTOFF )
   {
     *item_code = RFNV_ITEMS_ENUM_MAX;
     status = FALSE;
   }

   /*Condition to check if the item code is not within the NV EFS range
     This should be modified as per range of tuner NV items of custormer*/
   if ( (*item_code > RFNV_USE_EFS_NV_CUTOFF) && (*item_code < RFNV_ITEMS_ENUM_MAX) )
   {
      status = TRUE;
   }
   return (status); 
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  API for tuner NV item code retrieve based on CA bands of LTE
  API returns true or false and updates the item code.

  @details
  -The code returns the item code based on CA combination defined in the CA item
   structure. 
  -The customer should take care of populating the NV item code and its bands in
   rfcommon_nv_lte_ca_data structure.
  -The API returns fail if item code read is invalid.
  -In regards to the structure rfcommon_nv_lte_ca_data[] that stores band combo
   and associated NV, care should be taken to have last element of this array
   as { RFCOM_NUM_LTE_BANDS, RFCOM_NUM_LTE_BANDS, RFNV_ITEMS_ENUM_MAX }
   The while loop in the code looks for this value to terminate the search of
   band combo.
 
  @param band_1
  One of the LTE CA band
 
  @param band_2
  Second LTE CA band
 
  @param item_code
  pointer to return item code information
 
  @return
  True or False

*/
boolean rfcommon_nv_get_tuner_ca_item_code
( 
   rfcom_lte_band_type band_1,
   rfcom_lte_band_type band_2,
   rfnv_item_id_enum_type *item_code  
)
{
   boolean status = FALSE;
   uint8 index = 0;

   if (item_code == NULL)
   {
      return FALSE;
   }

   if (band_1 >= RFCOM_NUM_LTE_BANDS || band_2 >= RFCOM_NUM_LTE_BANDS)/*Validate band information*/
   {
      return FALSE;
   }

   *item_code = RFNV_ITEMS_ENUM_MAX;  /*Re-init the value for case where garbage is read by pointer*/

   for (index = 0; index < ca_nv_struc_size; index++ )
   {
      /*Check for the last element pattern*/
      if (  (rfcommon_nv_lte_ca_data[index].band_x == RFCOM_NUM_LTE_BANDS)
          &&(rfcommon_nv_lte_ca_data[index].band_y == RFCOM_NUM_LTE_BANDS)
          &&(rfcommon_nv_lte_ca_data[index].ca_item_code == RFNV_ITEMS_ENUM_MAX)) 
      {
         break;
      }

      if (rfcommon_nv_lte_ca_data[index].band_x == band_1 
          && rfcommon_nv_lte_ca_data[index].band_y == band_2)
      {
         *item_code = rfcommon_nv_lte_ca_data[index].ca_item_code;
         break;
      }

   }/*End of for*/

   /*To handle cases where the NV entry is wrong*/
   if ( *item_code>0 && *item_code<=RFNV_USE_EFS_NV_CUTOFF ) 
   { 
     *item_code = RFNV_ITEMS_ENUM_MAX;
     status = FALSE;
   }

   /*Condition to check if the item code is not within the NV EFS range 
     This should be modified as per range of tuner NV items of custormer*/
   if ( (*item_code > RFNV_USE_EFS_NV_CUTOFF) && (*item_code < RFNV_ITEMS_ENUM_MAX) )
   {
      status = TRUE;
   }

   return status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  API for tuner NV item code retrieve based on DBDC bands of WCDMA
  API returns true or false and updates the item code.

  @details
  -The code returns the item code based on DBDC combination defined in the 
   structure. 
  -The customer should take care of populating the NV item code and its bands in
   rfcommon_nv_wcdma_DBDC_data structure.
  -The API returns fail if item code read is invalid.
  -In regards to the structure rfcommon_nv_wcdma_dbdc_data[] that stores band combo
   and associated NV, care should be taken to have last element of this array
   as { RFCOM_NUM_WCDMA_BANDS, RFCOM_NUM_WCDMA_BANDS, RFNV_ITEMS_ENUM_MAX }
   The while loop in the code looks for this value to terminate the search of
   band combo.
 
  @param band_1
  One of the WCDMA DBDC band
 
  @param band_2
  Second WCDMA DBDC band
 
  @param item_code
  pointer to return item code information
 
  @return
  True or False

*/
boolean rfcommon_nv_get_tuner_dbdc_item_code
( 
   rfcom_wcdma_band_type band_1,
   rfcom_wcdma_band_type band_2,
   rfnv_item_id_enum_type *item_code  
)
{
   boolean status = FALSE;
   uint8 index = 0;

   if (item_code == NULL)
   {
      return FALSE;
   }

   if (band_1 >= RFCOM_NUM_WCDMA_BANDS || band_2 >= RFCOM_NUM_WCDMA_BANDS)/*Validate band information*/
   {
      return FALSE;
   }

   *item_code = RFNV_ITEMS_ENUM_MAX;  /*Re-init the value for case where garbage is read by pointer*/

   for (index = 0; index < dbdc_nv_struc_size; index++ )
   {
      /*Check for the last element pattern*/
      if (  (rfcommon_nv_wcdma_dbdc_data[index].band_1 == RFCOM_NUM_WCDMA_BANDS)
          &&(rfcommon_nv_wcdma_dbdc_data[index].band_2 == RFCOM_NUM_WCDMA_BANDS)
          &&(rfcommon_nv_wcdma_dbdc_data[index].dbdc_item_code == RFNV_ITEMS_ENUM_MAX)) 
      {
         break;
      }

      if (rfcommon_nv_wcdma_dbdc_data[index].band_1 == band_1 
          && rfcommon_nv_wcdma_dbdc_data[index].band_2 == band_2)
      {
         *item_code = rfcommon_nv_wcdma_dbdc_data[index].dbdc_item_code;
         break;
      }

   }/*End of for*/

   /*To handle cases where the NV entry is wrong*/
   if ( *item_code>0 && *item_code<=RFNV_USE_EFS_NV_CUTOFF ) 
   { 
     *item_code = RFNV_ITEMS_ENUM_MAX;
     status = FALSE;
   }

   /*Condition to check if the item code is not within the NV EFS range 
     This should be modified as per range of tuner NV items of custormer*/
   if ( (*item_code > RFNV_USE_EFS_NV_CUTOFF) && (*item_code < RFNV_ITEMS_ENUM_MAX) )
   {
      status = TRUE;
   }

   return status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Reads RFNV PA Static strucutre item

  @details
  API retries giant PA static NV item based on the MODE passed.

  @param rfcom_mode_enum_type mode
  RFCOM_1X_MODE : Returns the entire structure (rfcommon_nv_pa_static_data_type)
  For other 3G and 4G tech 
  ie., RFCOM_WCDMA_MODE, RFCOM_TDSCDMA_MODE, RFCOM_LTE_MODE
  Returns the common structure only (rfcommon_nv_cmn_pa_static_type)
  
  @param void *pa_static_nv_data : Pointer where to put the item 
  @param rfnv_item_id_enum_type pa_static_nv_id : NV Item ID to get
  @param task_ptr  : Calling task's TCB pointer
  @param task_wait_sig : Task signal to wait for when reading NV.
  @param task_wait_handler : Task's wait function to be called when reading NV 

  @retval 
    TRUE - API success
    FALSE - API Failure
*/


boolean 
rfcommon_load_pa_static_nv
(
  rfcom_mode_enum_type mode,
  void *pa_static_nv_data,
  rfnv_item_id_enum_type pa_static_nv_id,
  rex_tcb_type* caller_tcb_ptr,
  rex_sigs_type task_nv_wait_sig,
  void (* task_wait_func_ptr)( rex_sigs_type )
)
{
  boolean status = TRUE;
  nv_stat_enum_type  nv_read_status = NV_DONE_S;

  /* Pointer to NV data*/
  rfnv_item_type *nv_data_ptr = NULL;
  /* Pointer to NV decompressed data, Note using the same data type for decompression data. */
  rfnv_item_type *nv_dc_data_ptr = NULL;

  rfnv_item_size_type nv_read_size = sizeof(rfnv_item_type);
  uint32 uncompress_size = sizeof(rfnv_item_type);
  uint32 nv_element_size;
  /* NV decompressino status */
  int8 uncompress_status = Z_ERRNO;

  /* Allocate memory for raw NV data */
  nv_data_ptr = (rfnv_item_type *)modem_mem_alloc( sizeof(rfnv_item_type), MODEM_MEM_CLIENT_RFA );

  RF_MSG_2(MSG_LEGACY_LOW, "Starting rfcommon_load_pa_static_nv: Mode:%d | NV_ID:%d",mode,pa_static_nv_id);
  
  /* Null pointer check */
  if (nv_data_ptr == NULL)
  {
    RF_MSG_2(MSG_LEGACY_ERROR, "rfcommon_load_pa_static_nv %d: Failed to allocate memory for %d bytes.", pa_static_nv_id, nv_read_size);
    status = FALSE;
  }
  else
  {
    /* default to all 0's */
    memset( nv_data_ptr, 0, sizeof(rfnv_item_type));

    nv_read_status = rfcommon_rfnv_get_item( pa_static_nv_id,
                                             nv_data_ptr,
                                             &nv_read_size,
                                             caller_tcb_ptr,
                                             task_nv_wait_sig,
                                             task_wait_func_ptr );
    if( nv_read_status == NV_DONE_S )
    {
      RF_MSG_2(MSG_LEGACY_HIGH, "rfcommon_load_pa_static_nv id#: %d NV read successful, size = %d", pa_static_nv_id, nv_read_size);
      status = TRUE;
      // check for NV Id is compressed or not
      if (rfcommon_rfnv_chk_item_compressed(pa_static_nv_id))
      {
        /* if it is compressed data, need to uncompressed data */
        /* Allocate memory for NV data decompression. Allocate memory with nv_item_type for PA static data, and using PA static data type (smaller) causes a problem */
        nv_dc_data_ptr = (rfnv_item_type *)modem_mem_alloc(sizeof(rfnv_item_type), MODEM_MEM_CLIENT_RFA);

        if (nv_dc_data_ptr != NULL)
        {
          /* Decompress NV data */
          uncompress_status = uncompress((uint8 *)nv_dc_data_ptr,   
                                       &uncompress_size,
                                       (const uint8 *)nv_data_ptr, 
                                       (uint32)nv_read_size);

          if (uncompress_status != Z_OK)
          {
            RF_MSG_2( RF_ERROR, "decompression of PA static NV %d failed!status:%d", pa_static_nv_id, uncompress_status ); 
            /* Free the memory for decompressed NV data */
            modem_mem_free((void*)nv_dc_data_ptr, MODEM_MEM_CLIENT_RFA);
            status = FALSE;  
          }
          else
          {
            RF_MSG_2( MSG_LEGACY_HIGH, "rfcommon_load_pa_static_nv id#:%d NV uncompressed successful, size = %d", pa_static_nv_id, uncompress_size );
            /* Free the memory for raw NV data*/
            modem_mem_free((void*)nv_data_ptr, MODEM_MEM_CLIENT_RFA);

            /* Reuse the nv_data_ptr for the uncompress nv data */
            nv_data_ptr = nv_dc_data_ptr;
            status = TRUE;
          }
      } /* nv_dc_data_ptr */
      else
      {
          RF_MSG_1( RF_ERROR, "Fail to allocate memory for decompress PA static NV %d!", pa_static_nv_id); 
          status = FALSE;
        } /* nv_dc_data_ptr - else */
      } /* compressed NV */
    } /* nv_read_status */
    else
    {
      RF_MSG_1(MSG_LEGACY_ERROR, "rfcommon_load_pa_static_nv id#: %d NV Read Failed", pa_static_nv_id);
      status = FALSE;
    }

    if( status==TRUE )
    {
      switch(mode)
      {
        case RFCOM_1X_MODE:
        case RFCOM_1XEVDO_MODE:
          nv_element_size = sizeof(rfcommon_nv_pa_static_data_type);
          memscpy((void *)pa_static_nv_data, nv_element_size, (void*)nv_data_ptr, nv_element_size );
          status = TRUE;
          break;

        case RFCOM_WCDMA_MODE:
        case RFCOM_TDSCDMA_MODE:
        case RFCOM_LTE_MODE:
        case RFM_LTE_FTM_MODE:
          nv_element_size = sizeof(rfcommon_nv_cmn_pa_static_type);
          memscpy((void *)pa_static_nv_data, nv_element_size, (void*)nv_data_ptr, nv_element_size );        
          break;

        default:
          MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfcommon_load_pa_static_nv: Unsupported Mode", mode);
          status = FALSE;
          break;
      }
    }
  } /* else nv_data_ptr */

  /* Cleanup */
  if( nv_data_ptr != NULL )
  {
    modem_mem_free( nv_data_ptr, MODEM_MEM_CLIENT_RFA );
  }
  return status;
}

/*--------------------------------------------------------------------------*/
/* rflte_nv_populate_rx_gain_vs_freq_v2_nv                                  */
/*--------------------------------------------------------------------------*/
/*!
  @brief
  This API populates the RX gain vs freq V2 NV

  @details
  This API populates the RX gain vs freq V2 NV

  @param nv_tbl_type_ptr
  Pointer to the band-specific NV table
 
  @param rfnv_item_ptr
  Pointer to the NV

*/
boolean
rfcommon_populate_rx_gain_vs_freq_v2_nv
(
  rfnv_rx_cal_data_type        *rx_cal_data_ptr,
  void                         *rfnv_item_ptr,
  uint8                        num_gain_states
)
{
  /*---------------------------------------------------------------*/
  /*                        LOCAL VARIABLES                        */
  /*---------------------------------------------------------------*/
  /* NV Data related Variables */
  uint32 bytes_parsed = 0,  total_elements = 0,element_index = 0;
  uint32 uncompress_size = 0, total_nv_size = 0, path_cal_data_size = 0;
  rfnv_data_variant_marker_type *variant_marker_ptr = NULL;
  rfnv_rx_cal_data_typeid_118_type *rx_cal_data_typeid_118_ptr = NULL;
  rfnv_rx_cal_data_typeid_124_type *rx_cal_data_typeid_124_ptr = NULL;
  uint8 *current_pos_ptr = NULL, *uncompressed_nv_data_ptr = NULL;
  uint16 type_id = 0;
  int8 uncompress_status = Z_ERRNO;
  boolean success = FALSE ;
  uint32 num_elements_typeid118 = 0;
  uint32 num_elements_typeid124 = 0;
  uint8 *init_pass_to_get_num = NULL;
  uint32 typeid118_total_nv_size = 0;
  uint32 typeid118_path_cal_data_size = 0;
  uint32 typeid124_total_nv_size = 0;
  uint32 typeid124_path_cal_data_size = 0;
  uint32 typeid118_element_index = 0;
  uint32 typeid124_element_index = 0;

  /*---------------------------------------------------------------*/
  /*                    END OF LOCAL VARIABLES                     */
  /*---------------------------------------------------------------*/
  
  /*---------------------------------------------------------------*/
  /*                         VALIDATION                            */
  /*---------------------------------------------------------------*/
  if ( NULL == rx_cal_data_ptr )
  {
    MSG ( MSG_SSID_RF, 
          MSG_LEGACY_ERROR, 
             "rfcommon_populate_rx_gain_vs_freq_v2_nv: ERROR: "
             "NV Table Pointer is NULL ! "
             "Skiping NV Load "  );
    return success ;       
  }

  if ( NULL == rfnv_item_ptr )
  {
    MSG ( MSG_SSID_RF, 
          MSG_LEGACY_ERROR, 
          "rfcommon_populate_rx_gain_vs_freq_v2_nv: ERROR: "
          "rx_gain_vs_freq_v2 data Pointer is NULL ! "
          "Skiping NV Load "  );
    return success ;
  }

  if ((num_gain_states > RFNV_MAX_LNA_GAIN_STATES) || 
      (num_gain_states == 0))
  {
    MSG_2 ( MSG_SSID_RF, 
            MSG_LEGACY_ERROR, 
            "rfcommon_populate_rx_gain_vs_freq_v2_nv: ERROR: "
            "num_gain_state %d exceeds MAX %d allowed, or no gain states "
            "Skiping NV Load ",
            num_gain_states, 
            RFNV_MAX_LNA_GAIN_STATES);
    return success ;
  }

  /*---------------------------------------------------------------*/
  /*                      END OF VALIDATION                        */
  /*---------------------------------------------------------------*/

  /*---------------------------------------------------------------*/
  /*                      Decompress the NV                        */
  /*---------------------------------------------------------------*/
  /* Allocate worst-case memory size for NV data decompression. */
  uncompress_size = sizeof(rfnv_data_variant_marker_type);
  uncompress_size += (sizeof(rfnv_rx_cal_data_typeid_118_type) * RFCOMMON_NV_RX_GAIN_V2_NV_MAX_CONTAINERS);
  uncompress_size += (sizeof(rfnv_rx_cal_data_typeid_124_type) * RFCOMMON_NV_RX_GAIN_V2_NV_MAX_CONTAINERS);
  uncompressed_nv_data_ptr = modem_mem_alloc(uncompress_size, MODEM_MEM_CLIENT_RFA);
  if (uncompressed_nv_data_ptr == NULL)
  {
     MSG_1 ( MSG_SSID_RF, 
             MSG_LEGACY_ERROR,
             "RX CAL Data ERROR - failed to allocate memory for %d bytes.", 
             uncompress_size );
    return success ;
  }
  /* Decompress NV data */
  uncompress_status = uncompress( (uint8 *)uncompressed_nv_data_ptr,   
                                  &uncompress_size,
                                  (const uint8 *)rfnv_item_ptr, 
                                  sizeof(rfnv_item_type) );
  
  if (uncompress_status != Z_OK)
  {
    MSG ( MSG_SSID_RF, 
          MSG_LEGACY_ERROR,
          "RX CAL Data ERROR - decompression of NV ID failed.");
    modem_mem_free(uncompressed_nv_data_ptr, MODEM_MEM_CLIENT_RFA);
    return success ;
  }

  /*---------------------------------------------------------------*/
  /*                      END OF Decompression                     */
  /*---------------------------------------------------------------*/
  

  /*---------------------------------------------------------------*/
  /*                        NV Population                          */
  /*---------------------------------------------------------------*/
  /* ----- Extract the variant_marker to check how items to parse ----- */
  variant_marker_ptr = ( rfnv_data_variant_marker_type * )uncompressed_nv_data_ptr;
  total_elements = variant_marker_ptr->num_elements;
  bytes_parsed = sizeof(rfnv_data_variant_marker_type);
  current_pos_ptr = ((uint8 *)uncompressed_nv_data_ptr) + bytes_parsed;
  memscpy(&(rx_cal_data_ptr->variant_marker),
          sizeof(rfnv_data_variant_marker_type),
          uncompressed_nv_data_ptr,
          sizeof(rfnv_data_variant_marker_type));

  /* First element must be cal_data_info */
  type_id = *((uint16 *)current_pos_ptr);

  /* copy over the current position to temp location for initial 
     pass to gather the number of each individual type of nv */
  init_pass_to_get_num = current_pos_ptr;
  for ( element_index = 0 ;
        element_index < total_elements 
        ; element_index++)
  {
    type_id = *((uint16 *)init_pass_to_get_num);
    init_pass_to_get_num += sizeof(uint16);
    switch (type_id)
    {
      case RFNV_DATA_TYPE_RX_CAL_OFFSET_V2:
        init_pass_to_get_num += sizeof(rfnv_rx_cal_data_typeid_118_type);
        num_elements_typeid118++;
      break;
      case RFNV_DATA_TYPE_RXRSB_V1:
        init_pass_to_get_num += sizeof(rfnv_rx_cal_data_typeid_124_type);
        num_elements_typeid124++;
      break;
      default:
        return success;
      break;
    }
  }    

  rx_cal_data_ptr->num_of_cal_118_elements = num_elements_typeid118;
  rx_cal_data_ptr->num_of_cal_124_elements = num_elements_typeid124;

  typeid118_total_nv_size = ( (sizeof(rfnv_rx_cal_data_typeid_118_type)*num_elements_typeid118) - 
                    (sizeof(int16)*RFNV_NUMBER_OF_PCLS*(RFNV_MAX_LNA_GAIN_STATES-num_gain_states)) );
  typeid118_path_cal_data_size = ((sizeof(int16)*RFNV_NUMBER_OF_PCLS*(num_gain_states+1)) + sizeof(uint16)*2);
                     
  if(rx_cal_data_ptr->cal_118_data != NULL)
  {
    modem_mem_free(rx_cal_data_ptr->cal_118_data, 
                   MODEM_MEM_CLIENT_RFA);
  }

  if (num_elements_typeid118 != 0)
  {
    rx_cal_data_ptr->cal_118_data = modem_mem_alloc(typeid118_total_nv_size,
                                                    MODEM_MEM_CLIENT_RFA);
    if (rx_cal_data_ptr->cal_118_data == NULL)
    {
        MSG ( MSG_SSID_RF, 
              MSG_LEGACY_ERROR, 
              "rfcommon_populate_rx_gain_vs_freq_v2_nv: ERROR: "
              "cannot allocate memory for cal data"
              "Skiping NV Load ");
        modem_mem_free(uncompressed_nv_data_ptr, MODEM_MEM_CLIENT_RFA);
        return success ;
    }
  }

  /* Calculate memory size to be allocated for the rx rsb data */
  typeid124_total_nv_size = ( (sizeof(rfnv_rx_cal_data_typeid_124_type)*num_elements_typeid124) );
  /* Calculate memory size for each element of rx rsb data */
  typeid124_path_cal_data_size = sizeof(rfnv_rx_cal_data_typeid_124_type);
                     
  if(rx_cal_data_ptr->cal_124_data != NULL)
  {
    modem_mem_free(rx_cal_data_ptr->cal_124_data, 
                   MODEM_MEM_CLIENT_RFA);
  }
  
  if (num_elements_typeid124 != 0)
  {
    rx_cal_data_ptr->cal_124_data = modem_mem_alloc(typeid124_total_nv_size,
                                                    MODEM_MEM_CLIENT_RFA);
    if (rx_cal_data_ptr->cal_124_data == NULL)
    {
        MSG ( MSG_SSID_RF, 
              MSG_LEGACY_ERROR, 
              "rfcommon_populate_rx_gain_vs_freq_v2_nv: ERROR: "
              "cannot allocate memory for cal data"
              "Skiping NV Load ");
        modem_mem_free(uncompressed_nv_data_ptr, MODEM_MEM_CLIENT_RFA);
        return success ;
    }
  }

  /* Load the rest of the NV data */
  for ( element_index = 0 , 
        /* Capture success of all loops and if even one element fails return FALSE */
        success = TRUE,
        typeid118_element_index = 0 , 
        typeid124_element_index = 0 ; 
        element_index < total_elements 
        ; element_index++)
  {
    type_id = *((uint16 *)current_pos_ptr);
    bytes_parsed += sizeof(uint16);
    current_pos_ptr = ((uint8 *)uncompressed_nv_data_ptr) + bytes_parsed;
    switch (type_id)
    {
      /* Make sure to update the "success" in all the case
         statements everytime */
      case RFNV_DATA_TYPE_RX_CAL_OFFSET_V2:
	  	
        if((NULL != rx_cal_data_ptr->cal_118_data)&&(num_elements_typeid118 != 0))
        {
          rx_cal_data_typeid_118_ptr = (rfnv_rx_cal_data_typeid_118_type *)current_pos_ptr;
          memscpy(&(rx_cal_data_ptr->cal_118_data[typeid118_element_index]),
                    typeid118_path_cal_data_size,
                    rx_cal_data_typeid_118_ptr,
                    sizeof(rfnv_rx_cal_data_typeid_118_type));
          bytes_parsed += sizeof(rfnv_rx_cal_data_typeid_118_type);
          current_pos_ptr = ((uint8 *)uncompressed_nv_data_ptr) + bytes_parsed;
          success &= TRUE ;
          typeid118_element_index++;
        }
        else
        {
          MSG(MSG_SSID_RF,MSG_LEGACY_ERROR, "rfcommon_populate_rx_gain_vs_freq_v2_nv: ERROR: Null 118 Data");
        }
        break;

      case RFNV_DATA_TYPE_RXRSB_V1:

        if((NULL != rx_cal_data_ptr->cal_124_data)&&(num_elements_typeid124 != 0))
        {
          rx_cal_data_typeid_124_ptr = (rfnv_rx_cal_data_typeid_124_type *)current_pos_ptr;
          memscpy(&(rx_cal_data_ptr->cal_124_data[typeid124_element_index]),
                  typeid124_path_cal_data_size,
                  rx_cal_data_typeid_124_ptr,
                  sizeof(rfnv_rx_cal_data_typeid_124_type));
          bytes_parsed += sizeof(rfnv_rx_cal_data_typeid_124_type);
          current_pos_ptr = ((uint8 *)uncompressed_nv_data_ptr) + bytes_parsed;
          success &= TRUE ;
          typeid124_element_index++;
        }
        else
        {
          MSG(MSG_SSID_RF,MSG_LEGACY_ERROR, "rfcommon_populate_rx_gain_vs_freq_v2_nv: ERROR: Null 124 Data");
        }
        break;

      default:
        MSG_2 ( MSG_SSID_RF, 
                MSG_LEGACY_ERROR, 
                "rfcommon_populate_rx_gain_vs_freq_v2_nv: ERROR: "
                "unexpected cal info type id %d"
                "Skiping NV Load at index %d", 
                type_id,
                element_index );
        if((NULL != rx_cal_data_ptr->cal_118_data)&&(num_elements_typeid118 != 0))
        {
          modem_mem_free(rx_cal_data_ptr->cal_118_data, 
                         MODEM_MEM_CLIENT_RFA);
        }
        if((NULL != rx_cal_data_ptr->cal_124_data)&&(num_elements_typeid124 != 0))
        {
          modem_mem_free(rx_cal_data_ptr->cal_124_data, 
                         MODEM_MEM_CLIENT_RFA);            
        }
        if(NULL != uncompressed_nv_data_ptr)
        {
          modem_mem_free(uncompressed_nv_data_ptr, MODEM_MEM_CLIENT_RFA);
        }
        return success ;
    }
  }

  /*---------------------------------------------------------------*/
  /*                    END OF POPULATION                          */
  /*---------------------------------------------------------------*/
  modem_mem_free(uncompressed_nv_data_ptr, MODEM_MEM_CLIENT_RFA);

  return success ;
}/* rfcommon_nv_populate_rx_gain_vs_freq_v2_nv */

/*===========================================================================

FUNCTION RFNV_CHK_NV_ITEM_COMPRESSED

DESCRIPTION
  This function check a specific NV item whether compressed with the following.
  TRUE:    if the input nv_item is compressed
  FALSE:   if the input nv_item is uncompressed

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  Harded checking based on the nv defination enum

===========================================================================*/
boolean rfcommon_rfnv_chk_item_compressed
(
  rfnv_item_id_enum_type  item_code              /* Item to check */
)
{
   boolean is_rfnv_item_compressed = FALSE;

   switch (item_code) 
   {
      case RFNV_LTE_C1_B1_FBRX_MODE_VS_GAIN_VS_TEMP_VS_FREQ_Z_I:
      case RFNV_LTE_C1_B1_FBRX_DROOP_FIR_FILTER_COEFF_Z_I:
      case RFNV_LTE_C1_B2_FBRX_MODE_VS_GAIN_VS_TEMP_VS_FREQ_Z_I:
      case RFNV_LTE_C1_B2_FBRX_DROOP_FIR_FILTER_COEFF_Z_I:
      case RFNV_LTE_C1_B3_FBRX_MODE_VS_GAIN_VS_TEMP_VS_FREQ_Z_I:
      case RFNV_LTE_C1_B3_FBRX_DROOP_FIR_FILTER_COEFF_Z_I:
      case RFNV_LTE_C1_B4_FBRX_MODE_VS_GAIN_VS_TEMP_VS_FREQ_Z_I:
      case RFNV_LTE_C1_B4_FBRX_DROOP_FIR_FILTER_COEFF_Z_I:
      case RFNV_LTE_C1_B21_FBRX_MODE_VS_GAIN_VS_TEMP_VS_FREQ_Z_I:
      case RFNV_LTE_C1_B21_FBRX_DROOP_FIR_FILTER_COEFF_Z_I:
      case RFNV_LTE_C1_B25_FBRX_MODE_VS_GAIN_VS_TEMP_VS_FREQ_Z_I:
      case RFNV_LTE_C1_B25_FBRX_DROOP_FIR_FILTER_COEFF_Z_I:
      case RFNV_LTE_C1_B39_FBRX_MODE_VS_GAIN_VS_TEMP_VS_FREQ_Z_I:
      case RFNV_LTE_C1_B39_FBRX_DROOP_FIR_FILTER_COEFF_Z_I:
      case RFNV_LTE_C1_B5_FBRX_MODE_VS_GAIN_VS_TEMP_VS_FREQ_Z_I:
      case RFNV_LTE_C1_B5_FBRX_DROOP_FIR_FILTER_COEFF_Z_I:
      case RFNV_LTE_C0_B66_FBRX_MODE_VS_GAIN_VS_TEMP_VS_FREQ_Z_I:
      case RFNV_LTE_C1_B66_FBRX_MODE_VS_GAIN_VS_TEMP_VS_FREQ_Z_I:
      case RFNV_LTE_C0_B66_FBRX_DROOP_FIR_FILTER_COEFF_Z_I:
      case RFNV_LTE_C1_B66_FBRX_DROOP_FIR_FILTER_COEFF_Z_I:
      case RFNV_LTE_C0_B66_PA_STATIC_Z_I:
      case RFNV_LTE_C1_B66_PA_STATIC_Z_I:
      case RFNV_LTE_C0_B71_FBRX_MODE_VS_GAIN_VS_TEMP_VS_FREQ_Z_I:
      case RFNV_LTE_C0_B71_FBRX_DROOP_FIR_FILTER_COEFF_Z_I:
      case RFNV_LTE_C0_B71_PA_STATIC_Z_I:
         is_rfnv_item_compressed = TRUE;
         break;

      default:
         is_rfnv_item_compressed = FALSE;
   }
  
  if ( (((int32) item_code >= (int32) RFNV_CDMA_C0_BC0_PA_STATIC_Z_I) &&
        ((int32) item_code <= (int32) RFNV_WCDMA_B19_FBRX_MODE_VS_GAIN_VS_TEMP_VS_FREQ_Z_I)) || 
       ( (((int32) item_code >= (int32) RFNV_LTE_C0_B1_FBRX_DROOP_FIR_FILTER_COEFF_Z_I) &&
        ((int32) item_code <= (int32) RFNV_LTE_C0_B44_FBRX_DROOP_FIR_FILTER_COEFF_Z_I)) ) ||
        is_rfnv_item_compressed == TRUE )    
  {
     return TRUE;
  }
  else 
  { 
     return FALSE;
  }
}

#define RF_COMMON_FBRX_NOISE_FIXED_SIZE  (sizeof(rfcommon_nv_fbrx_noise_data_type))

boolean
rfcommon_nv_load_fbrx_noise_data_from_nv
(
  rfcommon_nv_fbrx_noise_data_type *fbrx_noise_nv_data,
  rfnv_item_id_enum_type fbrx_noise_data_item,
  rex_tcb_type* caller_tcb_ptr,
  rex_sigs_type task_nv_wait_sig,
  void (* task_wait_func_ptr)( rex_sigs_type )
)
{
  /* Overall status for this API */
  boolean api_status = TRUE;
  /* NV item size (maximum) */
  uint16 nv_item_size;
  /* Pointer to NV data*/
  rfnv_item_type *nv_data_ptr = NULL;
  /* Helper ptr to hold moving ptr location during data copy*/
  uint8     *temp_data_ptr = NULL;
  /* actual size of memory read from the NV*/
  rfnv_item_size_type   fbrx_noise_nv_read_size;
  /* Contains the number of bytes for an element in NV */
  uint32 nv_element_size;

  /* NV read status */
  nv_stat_enum_type  fbrx_noise_nv_read_status = NV_DONE_S;

  nv_item_size = sizeof(rfnv_item_type); 
  fbrx_noise_nv_read_size = sizeof(rfnv_item_type);

  /* Allocate memory for raw NV data */
  nv_data_ptr = (rfnv_item_type *)modem_mem_alloc( sizeof(rfnv_item_type),
                                                   MODEM_MEM_CLIENT_RFA );
  RF_MSG( RF_HIGH, "Enter rfcommon_nv_load_fbrx_noise_data_from_nv");

  /* Null pointer check */
  if (nv_data_ptr == NULL)
  {
    RF_MSG_1( RF_ERROR, "rfcommon_nv_load_fbrx_noise_data_from_nv - failed to allocate memory for %d bytes.", nv_item_size);
    return FALSE;
  }

  fbrx_noise_nv_read_status = rfcommon_rfnv_get_item( fbrx_noise_data_item,
                                                  nv_data_ptr,
                                                  &fbrx_noise_nv_read_size,
                                                  caller_tcb_ptr,
                                                  task_nv_wait_sig,
                                                  task_wait_func_ptr );

  if ( fbrx_noise_nv_read_status == NV_DONE_S )
  {
    RF_MSG_1( RF_HIGH, "rfcommon_nv_load_fbrx_noise_data_from_nv: NV read successful, size = %d", fbrx_noise_nv_read_size);
  } /* if (fbrx_noise_nv_read_status == NV_DONE_S ) */
  else
  {
    /* initialize the structure with default values accordlingly*/
    RF_MSG_1( RF_ERROR, "rfcommon_nv_load_fbrx_noise_data_from_nv: Initialized the fbrx gain data to default", 0);  
    api_status = FALSE;
    if( nv_data_ptr != NULL )
    {
      modem_mem_free( nv_data_ptr, MODEM_MEM_CLIENT_RFA );
    }
    return api_status;
  } /* else: if (fbrx_noise_nv_read_status == NV_DONE_S ) */


  /* If NV data has been successfully retrieved proceed to populating the data structure */
  if (api_status == TRUE) 
  {
    temp_data_ptr = (uint8 *)nv_data_ptr;
    /* copy data for fixed part and variant header of the NV, need to adjust size*/
    nv_element_size = RF_COMMON_FBRX_NOISE_FIXED_SIZE;
    memscpy((void *)fbrx_noise_nv_data, sizeof(*fbrx_noise_nv_data), (void*)temp_data_ptr, nv_element_size );
  }

  /* We are done, do the clean-up  */
  if( nv_data_ptr != NULL )
  {
    modem_mem_free( nv_data_ptr, MODEM_MEM_CLIENT_RFA );
  }

  return TRUE;
}





/*----------------------------------------------------------------------------*/
/*!
  @brief
  Generic function to load the FBRX gain switch points data for any band.


  @param rfcommon_nv_selftest_fbrx_gain_switch_pt_type
  The rfcommon_nv_selftest_fbrx_gain_switch_pt_type NV data structure where the cal data has to be loaded into

  @param selftest_fbrx_gain_switch_pts_data_item
  The NV item that contains the cal value.

  @param caller_tcb_ptr
  The TCB of the task that is requesting the RF driver to load the
  configuration data.  This must match rex_self() or the NV loading
  will fail.
  
  @param task_nv_wait_sig
  The signal the requesting task has allocated to the RF driver to wait
  on to indicate NV completion.
  
  @param task_wait_func_ptr
  A function provided by the task that is requesting the RF driver to
  load the configuration data which performs the correct idle-waiting
  behavior for the task.

  @return
  TRUE if the FBRX attn switch points value is loaded
  FALSE if NV did not load, and some more action is required by caller
*/

/* Size of NV */ 
#define RF_COMMON_SELFTEST_FBRX_GAIN_SWITCH_PTS_FIXED_SIZE  (sizeof(rfcommon_nv_selftest_fbrx_gain_switch_pt_type))

boolean
rfcommon_nv_load_selftest_fbrx_gain_switch_pts_from_nv
(
  rfcommon_nv_selftest_fbrx_gain_switch_pt_type *selftest_fbrx_gain_switch_pts_nv_data,
  rfnv_item_id_enum_type selftest_fbrx_gain_switch_pts_data_item,
  rex_tcb_type* caller_tcb_ptr,
  rex_sigs_type task_nv_wait_sig,
  void (* task_wait_func_ptr)( rex_sigs_type )
)
{

  /* Overall status for this API */
  boolean api_status = TRUE;
  /* NV read status */
  nv_stat_enum_type  selftest_fbrx_gain_switch_pts_nv_read_status = NV_DONE_S;
  /* NV item size (maximum) */
  uint16 nv_item_size = sizeof(rfnv_item_type); 
  /* Pointer to NV data*/
  rfnv_item_type *nv_data_ptr = NULL;
  /* Helper ptr to hold moving ptr location during data copy*/
  uint8     *temp_data_ptr = NULL;
  /* Contains the number of bytes for an element in NV */
  uint32 nv_element_size;
  /* actual size of memory read from the NV*/
  rfnv_item_size_type   selftest_fbrx_gain_switch_pts_nv_read_size;

  selftest_fbrx_gain_switch_pts_nv_read_size = sizeof(rfnv_item_type);
  /* Allocate memory for raw NV data */
  nv_data_ptr = (rfnv_item_type *)modem_mem_alloc( sizeof(rfnv_item_type),
                                                   MODEM_MEM_CLIENT_RFA );

  /* Null pointer check */
  if (nv_data_ptr == NULL)
  {
    RF_MSG_1( RF_ERROR, "rfcommon_nv_load_selftest_fbrx_gain_switch_pts_from_nv - failed to allocate memory for %d bytes.", nv_item_size);
    return FALSE;
  }
 
  selftest_fbrx_gain_switch_pts_nv_read_status = rfcommon_rfnv_get_item( selftest_fbrx_gain_switch_pts_data_item,
                                                  nv_data_ptr,
                                                  &selftest_fbrx_gain_switch_pts_nv_read_size,
                                                  caller_tcb_ptr,
                                                  task_nv_wait_sig,
                                                  task_wait_func_ptr );

  if ( selftest_fbrx_gain_switch_pts_nv_read_status == NV_DONE_S )
  {
    RF_MSG_1( RF_HIGH, "rfcommon_nv_load_selftest_fbrx_gain_switch_pts_from_nv: NV read successful, size = %d", selftest_fbrx_gain_switch_pts_nv_read_size);
  } /* if (selftest_fbrx_gain_switch_pts_nv_read_status == NV_DONE_S ) */
  else
  {
    /* initialize the structure with default values accordlingly*/
    RF_MSG_1( RF_ERROR, "rfcommon_nv_load_selftest_fbrx_gain_switch_pts_from_nv: Initialized the fbrx gain data to default", 0);  
    api_status = FALSE;
    if( nv_data_ptr != NULL )
    {
      modem_mem_free( nv_data_ptr, MODEM_MEM_CLIENT_RFA );
    }

    return FALSE;
  } /* else: if (fbrx_attn_switch_pts_nv_status == NV_DONE_S ) */

  /* If NV data has been successfully retrieved proceed to populating the data structure */
  if (api_status == TRUE) 
  {
    temp_data_ptr = (uint8 *)nv_data_ptr;
    /* copy data for fixed part and variant header of the NV, need to adjust size*/
    nv_element_size = RF_COMMON_SELFTEST_FBRX_GAIN_SWITCH_PTS_FIXED_SIZE;

    memscpy((void *)selftest_fbrx_gain_switch_pts_nv_data, sizeof(rfcommon_nv_selftest_fbrx_gain_switch_pt_type), (void*)temp_data_ptr, nv_element_size );

    /* print some debug - to be removed*/
    RF_MSG_1( RF_HIGH, "rfcommon_nv_load_selftest_fbrx_gain_switch_pts_from_nv - No. of switch points = %d",selftest_fbrx_gain_switch_pts_nv_data->num_switch_pts);
  }

  /* We are done, do the clean-up  */
  if( nv_data_ptr != NULL )
  {
    modem_mem_free( nv_data_ptr, MODEM_MEM_CLIENT_RFA );
  }

  return TRUE;

} /* rfcommon_nv_load_fbrx_attn_switch_pts_from_nv() */

boolean 
rfcommon_load_wcdma_sar_asdiv_nv
(
  rfcom_mode_enum_type mode,
  void *sar_static_nv_data,
  rfnv_item_id_enum_type sar_asdiv_nv_id,
  rex_tcb_type* sar_task_ptr,
  rex_sigs_type sar_task_wait_sig,
  void (* sar_task_wait_handler)( rex_sigs_type )
)
{
  boolean status = FALSE;
  nv_stat_enum_type  nv_read_status = NV_DONE_S;
   /* Pointer to NV data*/
  rfnv_item_type *nv_data_ptr = NULL;
  rfnv_item_size_type nv_read_size = sizeof(rfnv_item_type);
  uint32 nv_element_size;
    
  /* Pointer to NV decompressed data, Note using the same data type for decompression data. */
  rfnv_item_type *nv_dc_data_ptr = NULL;
   /* NV decompressino status */
  int8 uncompress_status = Z_ERRNO;
   uint32 uncompress_size = sizeof(rfnv_item_type);
  
  RF_MSG_2(RF_HIGH, "Starting rfcommon_load_wcdma_sar_asdiv_nv: Mode:%d | NV_ID:%d",mode,sar_asdiv_nv_id);
  
  /* Allocate memory for raw NV data */
  nv_data_ptr = (rfnv_item_type *)modem_mem_alloc( sizeof(rfnv_item_type), MODEM_MEM_CLIENT_RFA );

  /* Null pointer check */
  if (nv_data_ptr == NULL)
  {
    RF_MSG_2(RF_ERROR, "rfcommon_load_wcdma_sar_asdiv_nv %d: Failed to allocate memory for %d bytes.", sar_asdiv_nv_id, nv_read_size);
    status = FALSE;   
    return status;
  }
  
  /* default nv_data_ptr  to all 0's */
  memset( nv_data_ptr, 0, sizeof(rfnv_item_type));
  
  nv_read_status = rfcommon_rfnv_get_item( sar_asdiv_nv_id,
                                             nv_data_ptr,
                                             &nv_read_size,
                                             sar_task_ptr,
                                             sar_task_wait_sig,
                                             sar_task_wait_handler );
  
  if( nv_read_status == NV_DONE_S )
  {
    
      RF_MSG_2(RF_HIGH, "rfcommon_load_wcdma_sar_asdiv_nv id: %d NV read successful, size = %d", sar_asdiv_nv_id, nv_read_size);       
  
      /*Skipping changes for uncompress NV. Will be added when compressed NV is checked in*/
      
      #if 0
      /* Allocate memory for NV data decompression */
    nv_dc_data_ptr = (rfnv_item_type *)modem_mem_alloc(sizeof(rfnv_item_type), MODEM_MEM_CLIENT_RFA);

    if (nv_dc_data_ptr != NULL)
    {
         /* Decompress NV data */
          uncompress_status = uncompress((uint8 *)nv_dc_data_ptr,   
                                       &uncompress_size,
                                       (const uint8 *)nv_data_ptr, 
                                       (uint32)nv_read_size);
     
          if (uncompress_status == Z_OK)
          {
            RF_MSG_2( RF_HIGH, "rfcommon_load_wcdma_sar_asdiv_nv id:%d NV uncompressed successful, size = %d", sar_asdiv_nv_id, uncompress_size );
            /* Free the memory for raw NV data*/
            modem_mem_free((void*)nv_data_ptr, MODEM_MEM_CLIENT_RFA);

            /* Reuse the nv_data_ptr for the uncompress nv data */
            nv_data_ptr = nv_dc_data_ptr;
            status = TRUE;
          }
      else
          {
            RF_MSG_2( RF_ERROR, "rfcommon_load_wcdma_sar_asdiv_nv: decompression of SAR AsDiV NV %d failed!status:%d", sar_asdiv_nv_id, uncompress_status ); 
            /* Free the memory for decompressed NV data */
      ASSERT(0);

      modem_mem_free((void*)nv_dc_data_ptr, MODEM_MEM_CLIENT_RFA);
      
            status = FALSE; 
      return status;
          }
    }
      else
    { 
      RF_MSG_1( RF_ERROR, "rfcommon_load_wcdma_sar_asdiv_nv: Fail to allocate memory for decompress SAR AsDiv static NV %d!", sar_asdiv_nv_id); 
    status = FALSE;
    return status;
      }
      #endif
  status = TRUE; /* REmove when compressed NV is checked in*/
  }
  else
  {
      RF_MSG_2(RF_ERROR, "rfcommon_load_wcdma_sar_asdiv_nv nv id: %d NV Read Failed status: %d", sar_asdiv_nv_id, nv_read_status );
    status = FALSE;
    return status;
  }
  
  nv_element_size = RFCOM_NUM_WCDMA_BANDS*sizeof(rfwcdma_nv_sar_asdiv_type);
  memscpy((void *)sar_static_nv_data, nv_element_size, (void*)nv_data_ptr, nv_element_size );  
  return status;
  
  
}

boolean
rfcommon_nv_load_gnss_blanking_control
(
  rex_tcb_type           *task_ptr,
  rex_sigs_type          task_wait_sig,
  void                   (*task_wait_handler)( rex_sigs_type )
)
{
  boolean api_status = TRUE; 

  rfnv_item_id_enum_type     nv_id = RFNV_GNSS_BLANKING_CONTROL_I;
  nv_stat_enum_type          nv_status;
  rfnv_item_type *           nv_data_raw_ptr;
  rfnv_item_size_type        nv_data_raw_sz = sizeof(rfnv_item_type);
  uint8 *                    nv_data_uncompressed_ptr = NULL;
  uint32                     nv_data_uncompress_size = 0;
  int8                       uncompress_status = Z_ERRNO;
  rfcommon_nv_tbl_type *     rfcommon_nv_tbl = rfcommon_nv_get_tbl_ptr();

  nv_data_uncompress_size = sizeof(rfcommon_nv_tbl->gnss_blanking_control);

  // NV defaults
  memset( &rfcommon_nv_tbl->gnss_blanking_control, 0x0, sizeof(rfcommon_nv_tbl->gnss_blanking_control) );

  nv_data_raw_ptr          = modem_mem_alloc( sizeof(rfnv_item_type), MODEM_MEM_CLIENT_RFA );
  nv_data_uncompressed_ptr = modem_mem_alloc( sizeof(rfnv_item_type), MODEM_MEM_CLIENT_RFA );
  
  if ( (nv_data_raw_ptr == NULL) || (nv_data_uncompressed_ptr == NULL) )
  {
    RF_MSG( RF_ERROR, "rfcommon_nv_load_gnss_blanking_control: Memory Alloc failed" );
    api_status = FALSE;
  }

  if( api_status == TRUE )
  {
    // Read raw compressed NV data
    nv_status = 
      rfcommon_rfnv_get_item( nv_id,
                              nv_data_raw_ptr,
                              &nv_data_raw_sz,
                              task_ptr,
                              task_wait_sig,
                              task_wait_handler );

    if ( nv_status == NV_DONE_S )
    {
      /* Decompress NV data */
      uncompress_status = 
        uncompress( (uint8 *)nv_data_uncompressed_ptr,   
                    &nv_data_uncompress_size,
                    (const uint8 *)nv_data_raw_ptr, 
                    (uint32)nv_data_raw_sz );

      if (uncompress_status != Z_OK)
      {
        RF_MSG_1( RF_ERROR, "rfcommon_nv_load_gnss_blanking_control: NV ID %d uncompress failed.", 
                  nv_id );
        api_status = FALSE;
      }

      if ( nv_data_uncompress_size > sizeof(rfcommon_nv_tbl->gnss_blanking_control) )
      {
        RF_MSG_3( RF_ERROR, 
                  "rfcommon_nv_load_gnss_blanking_control: NV ID %d data size %d > %d size available.",
                  nv_id, nv_data_uncompress_size, sizeof(rfcommon_nv_tbl->gnss_blanking_control) );
        api_status = FALSE;
      }
    }
  }

  if( api_status == TRUE )
  {
    if ( nv_status == NV_DONE_S )
    {
      //Init to 0xFF to be used during operation as a terminator check
      memset(&rfcommon_nv_tbl->gnss_blanking_control.rf_tech_bands, 0xFF, 
             sizeof(rfcommon_nv_tbl->gnss_blanking_control.rf_tech_bands) );

      /* Load the NV data into the pre-defined structure */
      memscpy( &rfcommon_nv_tbl->gnss_blanking_control, 
               sizeof(rfcommon_nv_tbl->gnss_blanking_control),
               nv_data_uncompressed_ptr, 
               nv_data_uncompress_size );

      api_status = TRUE;
    } 
    else if ( nv_status == NV_NOTACTIVE_S )
    {
      RF_MSG_1( RF_HIGH,
                "rfcommon_nv_load_gnss_blanking_control: NV ID %d inactive",
                nv_id );

      api_status = TRUE;
    } 
    else
    {
      RF_MSG_1 ( RF_ERROR,
                 "rfcommon_nv_load_gnss_blanking_control: NV ID %d read error.",
                 nv_id );

      api_status = FALSE;
    }
  }

  /* Cleanup */

  if (nv_data_raw_ptr != NULL)
  {
    modem_mem_free( nv_data_raw_ptr, MODEM_MEM_CLIENT_RFA );
  }

  if (nv_data_uncompressed_ptr != NULL)
  {
    modem_mem_free( nv_data_uncompressed_ptr, MODEM_MEM_CLIENT_RFA );
  }

  return api_status;
}
/*--------------------------------------------------------------------------*/
/* rfcommon_nv_band_has_rx_static                                  */
/*--------------------------------------------------------------------------*/
/*!
  @brief
  This function determines if rx_static variant NV is present in the nv_list

  @details
  Loops over rfnv_list and returns true if rx_static_nv is present.
  It also populates rfnv_item_id if rx_static_nv is present

  @param rfnv_item_list
  The NV item id of rx_static_nv
 
  @param rfnv_item_list_siz
  size of rfnv_item_list
 
  @return rfnv_item_id
  rfnv_item_id if rx_static_nv is present
 
  @return boolean: true if rx_static_nv is present
 
*/
boolean rfcommon_nv_band_has_rx_static
(
   const rfcommon_rfnv_items_list_type *rfnv_item_list, 
   uint16 rfnv_item_list_siz, 
   rfnv_item_id_enum_type *rfnv_item_id
)
{
  int16 rfnv_item_count = 0;
  boolean has_rx_static_item = FALSE;

  for (rfnv_item_count = 0; rfnv_item_count < rfnv_item_list_siz; rfnv_item_count++)
  {
    switch (rfnv_item_list[rfnv_item_count].rfnv_item_id)
    {
      case RFNV_LTE_B1_RX_STATIC_DATA_I:
      case RFNV_LTE_B2_RX_STATIC_DATA_I:
      case RFNV_LTE_B3_RX_STATIC_DATA_I:
      case RFNV_LTE_B4_RX_STATIC_DATA_I:
      case RFNV_LTE_B5_RX_STATIC_DATA_I:
      case RFNV_LTE_B6_RX_STATIC_DATA_I:
      case RFNV_LTE_B7_RX_STATIC_DATA_I:
      case RFNV_LTE_B8_RX_STATIC_DATA_I:
      case RFNV_LTE_B9_RX_STATIC_DATA_I:
      case RFNV_LTE_B10_RX_STATIC_DATA_I:
      case RFNV_LTE_B11_RX_STATIC_DATA_I:
      case RFNV_LTE_B12_RX_STATIC_DATA_I:
      case RFNV_LTE_B13_RX_STATIC_DATA_I:
      case RFNV_LTE_B14_RX_STATIC_DATA_I:
      case RFNV_LTE_B17_RX_STATIC_DATA_I:
      case RFNV_LTE_B18_RX_STATIC_DATA_I:
      case RFNV_LTE_B19_RX_STATIC_DATA_I:
      case RFNV_LTE_B20_RX_STATIC_DATA_I:
      case RFNV_LTE_B21_RX_STATIC_DATA_I:
      case RFNV_LTE_B23_RX_STATIC_DATA_I:                         
      case RFNV_LTE_B25_RX_STATIC_DATA_I:                           
      case RFNV_LTE_B26_RX_STATIC_DATA_I:                           
      case RFNV_LTE_B27_RX_STATIC_DATA_I:                           
      case RFNV_LTE_B28_RX_STATIC_DATA_I:                           
      case RFNV_LTE_B28B_RX_STATIC_DATA_I:                          
      case RFNV_LTE_B29_RX_STATIC_DATA_I:                           
      case RFNV_LTE_B30_RX_STATIC_DATA_I:                           
      case RFNV_LTE_B34_RX_STATIC_DATA_I:                           
      case RFNV_LTE_B38_RX_STATIC_DATA_I:                           
      case RFNV_LTE_B39_RX_STATIC_DATA_I:                           
      case RFNV_LTE_B40_RX_STATIC_DATA_I:                           
      case RFNV_LTE_B40B_RX_STATIC_DATA_I:                          
      case RFNV_LTE_B41_RX_STATIC_DATA_I:                           
      case RFNV_LTE_B41B_RX_STATIC_DATA_I:                          
      case RFNV_LTE_B41C_RX_STATIC_DATA_I:                          
      case RFNV_LTE_B42_RX_STATIC_DATA_I:                           
      case RFNV_LTE_B252_RX_STATIC_DATA_I:                          
      case RFNV_LTE_B255_RX_STATIC_DATA_I:                          
        has_rx_static_item = TRUE;
        *rfnv_item_id = rfnv_item_list[rfnv_item_count].rfnv_item_id;
        break;
      default:
        break;
    }
    /* Break out of for loop if rx_static item is found */
    if (has_rx_static_item == TRUE)
    {
      break;
    }
  }

  return has_rx_static_item;
}

/*--------------------------------------------------------------------------*/
/* rfcommon_nv_band_has_rx_switch_pt_datatype                                  */
/*--------------------------------------------------------------------------*/
/*!
  @brief
  This function determines if rx_static NV has RX_switchpoint_type datatype (203)

  @details
  Loops over num of elements in rx_static NV and if type 203 is found, sets return value to true

  @param rfnv_item_id
  The NV item id of rx_static_nv
 
  @param nv_data_ptr
  Pointer to the NV
 
  @param nv_item_size
  Size of static nv
 
*/
boolean rfcommon_nv_band_has_rx_switch_pt_datatype
(
   rfnv_item_id_enum_type local_rfnv_item_id,
   rfnv_item_type *nv_data_ptr,
   rfnv_item_size_type nv_item_size
)
{

  /* Return value for this function */
  boolean rx_switch_pt_datatype_detected = FALSE;
  /* variables that store data extracted from NV*/  
  rf_nv_data_variant_marker_type variant_marker;
  uint16 type_id = 0;
  /* Variables to assist with algorithm steps */
  uint8 *temp_data_ptr = NULL;   
  uint32 num_bytes_parsed; /* Keeps track of the running total of number of bytes parsed */
  uint32 nv_element_size;   /* Contains the number of bytes for an element in NV */

  /* Null pointer check */
  if (nv_data_ptr == NULL)
  {
    RF_MSG(RF_ERROR,
           "NV_203_DBG: rfcommon_nv_band_has_rx_switch_pt_datatype: NULL NV ptr passed in!!");

    return FALSE;
  }

  RF_MSG_2(RF_LOW,
           "NV_203_DBG: rfcommon_nv_band_has_rx_switch_pt_datatype - item code %d, item size %d",
           local_rfnv_item_id, nv_item_size);


  /* Initialize running total of bytes already parsed */
  num_bytes_parsed = 0;

  /* Extract the variant marker. First element in NV item must be the number of type IDs it contains */
  nv_element_size = sizeof(variant_marker);

  /* Ensure there is enough memory left before actually extracting the element */
  if (nv_item_size < nv_element_size)
  {
    MSG_ERROR("NV_203_DBG: rflte_nv_process_rx_static_data() nv_item_size passed in not correct: nv_item_size %d, nv_element_size %d ",
              nv_item_size, nv_element_size, 0);
    rx_switch_pt_datatype_detected = FALSE;
  }
  else 
  {
    /* EXTRACT variant marker, which has Number of type IDs present */
    temp_data_ptr = (uint8 *)nv_data_ptr + num_bytes_parsed;
    memscpy((void *)&variant_marker, nv_element_size, (void *)temp_data_ptr, nv_element_size);

    /* Update running total of bytes parsed with num bytes from last element extracted */
    num_bytes_parsed += nv_element_size;
    /* Ensure there is enough memory left before actually extracting the element */
    if (variant_marker.num_elements == 0)
    {
      RF_MSG(RF_ERROR, "NV_203_DBG: rfcommon_nv_band_has_rx_switch_pt_datatype - Number of elements value of 0 "
             "is invalid.");
    }
    else /* Variant marker extraction successful */
    {

      for (uint16 type_id_idx = 0; type_id_idx < variant_marker.num_elements; type_id_idx++)
      {
        /* Get type ID of the subsequent element*/
        nv_element_size = RFNV_DATA_TYPE_ID_SIZE;

        /* Ensure there is enough memory left before actually extracting the element */
        if ((nv_item_size - num_bytes_parsed) < nv_element_size)
        {
          RF_MSG_3(RF_ERROR,
                   "NV_203_DBG: rfcommon_nv_band_has_rx_switch_pt_datatype - Not enough elements left to extract "
                   "item for type index %d, nv_item_size %d num_bytes_parsed %d", type_id_idx, nv_item_size, num_bytes_parsed);
          rx_switch_pt_datatype_detected = FALSE;
          break;
        }

        /* EXTRACT ELEMENT: Type ID */
        temp_data_ptr = (uint8 *)nv_data_ptr + num_bytes_parsed;
        memscpy((void *)&type_id, sizeof(type_id), (void *)temp_data_ptr, nv_element_size);
        /* Update running total of bytes parsed with num bytes from last element extracted */
        num_bytes_parsed += nv_element_size;

        if (type_id == RFNV_DATA_TYPE_LTE_RX_SWITCH_POINT_CONTAINER)
        {
          rx_switch_pt_datatype_detected = TRUE;
          break;
        }
      }
    }
  }

  return rx_switch_pt_datatype_detected;
}

FTM_RF_Mode_Enum 
rfcommon_convert_band_tech_to_ftm_rf_mode
(
  rfcom_mode_enum_type	mode,
  rfcom_band_type_u  band
)
{
  FTM_RF_Mode_Enum phone_mode = PHONE_MODE_MAX;
  
  switch(mode)
  {
	case RFM_IMT_MODE:
	case RFM_IMT_MODE_2:

          if(band.wcdma_band < RFCOM_NUM_WCDMA_BANDS)
          {
            phone_mode = rf_tech_band_to_phone_mode_converter[mode][band.wcdma_band];
          }
          else
          {
            RF_MSG_2(RF_ERROR, "rfcommon_convert_band_tech_to_ftm_rf_mode:"
                                "Wrong Band = %d Passed for tech = %d",
                                 band.wcdma_band, mode);		   
          }
          break;
		
	case RFM_EGSM_MODE:
	case RFM_EGSM_MODE_2:	
	case RFM_EGSM_MODE_3:

           if(band.gsm_band < RFCOM_NUM_GSM_BANDS)
           {
              phone_mode = rf_tech_band_to_phone_mode_converter[mode][band.gsm_band];
           }
           else
           {
             RF_MSG_2(RF_ERROR, "rfcommon_convert_band_tech_to_ftm_rf_mode:"
                                "Wrong Band = %d Passed for tech = %d",
                                 band.gsm_band, mode); 		 
           }
           break;

	case RFM_1X_MODE:
	case RFM_1XEVDO_MODE:
	case RFM_SVDO_MODE:
		
           if(band.cdma_band < RFM_CDMA_MAX_BAND)
           {
             phone_mode = rf_tech_band_to_phone_mode_converter[mode][band.cdma_band];
           }
           else
           {
             RF_MSG_2(RF_ERROR, "rfcommon_convert_band_tech_to_ftm_rf_mode:"
                                "Wrong Band = %d Passed for tech = %d",
                                 band.wcdma_band, mode);		   
           }
           break;

	case RFM_LTE_MODE:
	case RFM_LTE_FTM_MODE:
	case RFM_LTE_MODE_2:
		
            if(band.lte_band < RFCOM_NUM_LTE_BANDS)
            {
              phone_mode = rf_tech_band_to_phone_mode_converter[mode][band.lte_band];
            }
            else
            {
               RF_MSG_2(RF_ERROR, "rfcommon_convert_band_tech_to_ftm_rf_mode:"
                                  "Wrong Band = %d Passed for tech = %d",
                                   band.wcdma_band, mode);		   
            }
            break;

	case RFM_TDSCDMA_MODE:
	case RFM_TDSCDMA_MODE_FTM:
		
            if(band.tdscdma_band < RFCOM_NUM_TDSCDMA_BANDS)
            {
              phone_mode = rf_tech_band_to_phone_mode_converter[mode][band.tdscdma_band];
            }
            else
            {
               RF_MSG_2(RF_ERROR, "rfcommon_convert_band_tech_to_ftm_rf_mode:"
                                  "Wrong Band = %d Passed for tech = %d",
                                   band.wcdma_band, mode);		   
            }
            break;

	case RFM_PARKED_MODE:
	case RFM_GPS_MODE:
	case RFM_GNSS_MODE:
	case RFM_WLAN_MODE:
	case RFM_INVALID_MODE:
	case RFM_ALL_MODES:
	  default:

	  RF_MSG_2(RF_ERROR, "rfcommon_convert_band_tech_to_ftm_rf_mode:"
                             "Wrong Band = %d and tech = %d",
                              band.lte_band, mode); 	   
	  break;
  }

  return phone_mode;
}


int8 rfcommon_nv_retrieve_asdiv_offset
(
  FTM_RF_Mode_Enum rf_mode
)
{
  uint8 i = 0;
  int8 asdiv_offset = 0;

  if(NULL == asdiv_offset_char_data)
  {
     RF_MSG(RF_ERROR, "rfcommon_nv_retrieve_asdiv_offset: Char data is not present." 
                      "Returning 0 offset");
	 
     return asdiv_offset;
  }
  
  for(i = 0; i<asdiv_offset_char_data_size; i++)
  {
    if(asdiv_offset_char_data[i].rf_mode == (uint8)rf_mode) break;
  }
  
  asdiv_offset =  asdiv_offset_char_data[i].asdiv_offset_db10;

  return asdiv_offset;
}

/*--------------------------------------------------------------------------*/
/* rfcommon_nv_get_asdiv_offset_data                                  */
/*--------------------------------------------------------------------------*/
/*!
  @brief
  This function fetches the Asdiv offset char data for given tech and band.
   
*/
int8 rfcommon_nv_get_asdiv_offset_data
(
  rfcom_mode_enum_type  mode,
  rfcom_band_type_u  band,
  uint8 ant_index
)
{
  int8 asdiv_offset = 0;
  FTM_RF_Mode_Enum rfm_mode = PHONE_MODE_MAX;

  if(ant_index == 1)
  {
    /*convert mode and band passed from tech layer to FTM RF mode
      because offset data is stored per FTM RF mode from EFS*/
    rfm_mode = rfcommon_convert_band_tech_to_ftm_rf_mode(mode, band);

    if(rfm_mode != PHONE_MODE_MAX)
    {
      asdiv_offset = rfcommon_nv_retrieve_asdiv_offset(rfm_mode);

      RF_MSG_4(RF_ERROR, "rfcommon_nv_get_asdiv_offset_data:"
                         "Got Asdiv Offset = %d, for Tech = %d, band = %d, antenna index = %d",
                          asdiv_offset, mode, band.lte_band, ant_index);
    }
    else
    {
      RF_MSG_2(RF_ERROR, "rfcommon_nv_get_asdiv_offset_data:"
                         "Wrong Tech = %d Band = %d combo passed for getting offset data."
                         "Returning 0 offset",
                          mode, band.lte_band);
    }	
  }
  return asdiv_offset;
}

boolean rfcommon_nv_parse_offset_data_from_efs(const char *data,   int32 data_size)
{
  char rf_mode[8] = "rf_mode";
  uint8 rf_mode_char_count = 7;
  char offset[4] = "pwr";
  uint8 offset_char_count = 3;
  char start_of_rf_mode[1] = {34}; //ASCII of "
  char end_of_rf_mode = 34; //ASCII of "
  char start_of_offset[1] = {34}; //ASCII of "
  char  end_of_offset = 34; //ASCII of "
  uint16 char_count = 0;
  uint16 efs_entries_count = 0;
  uint8 data_place_holder[10] = {0,0,0,0,0,0,0,0,0,0}; //temp array to hold the extracted values from EFS. 
  const char *data_parser = data;
  const char *data_parser_2 = data;
  uint16 rf_mode_temp = 0;
  uint16 offset_temp = 0;
  int8 i = 0;
  boolean status = TRUE;
  uint16 watchdog_loop_count_1 = 0; //watch dog loop count to break the loops
  uint16 watchdog_loop_count_2 = 0; //watch dog loop count to break the loops
  uint16 max_efs_entries = 255; //max allowed entries in EFS. (mode, pwr) is counted as one entry 
  boolean offset_is_negative = FALSE;

  if(NULL == data)
  {
    RF_MSG(RF_ERROR,"rfcommon_nv_parse_offset_data_from_efs:"
                    "NULL pointer passed");
    return FALSE;
  }

  if(NULL == asdiv_offset_char_data)
  {
    /* allocate memory to hold the EFS data*/
    asdiv_offset_char_data = 
      (rfcommon_asdiv_offset_char_data *)modem_mem_alloc((PHONE_MODE_MAX*sizeof(rfcommon_asdiv_offset_char_data)),MODEM_MEM_CLIENT_RFA);

  }

  /* check if memory is allocated properlt*/
  if(NULL == asdiv_offset_char_data)
  {
    RF_MSG(RF_ERROR,"rfcommon_nv_parse_offset_data_from_efs:"
                    "Failed to allocate memory to offset char datastructure");
    return FALSE;
  }
  else
  {
    //Initialize the structure to hold AsDiv Offset data*/
    memset(asdiv_offset_char_data, 0,(PHONE_MODE_MAX*sizeof(rfcommon_asdiv_offset_char_data)));
    asdiv_offset_char_data_size = 0;
  }

  /* Run the loop to cover entire file*/
  while((data_parser != NULL)&&(watchdog_loop_count_1 < max_efs_entries))
  {
    data_parser_2 = data_parser;
    /* Check the occurance of rf_mode first */
    data_parser = strstr(data_parser_2, rf_mode);

    if (NULL != data_parser)
    {  
      /*increment the data pointer to go pass "rf_mode"
        data_ptr_temp = data_parser + rf_mode_char_count; */

      data_parser_2 = data_parser;
 
      /* move data parser to start of mode value which is expected to wrapped in ""*/
	  data_parser = strstr(data_parser_2, start_of_rf_mode);

      if (NULL != data_parser)
      {
        /* increment data parser by 1 to point to actual RF Mode value*/	 
        data_parser++;

        /*reset char count*/
        char_count = 0;
        /*reset loop count*/
        watchdog_loop_count_2 = 0;
        /*reset temp rfmode variable*/
        rf_mode_temp = 0;
        
        /* loop to extract the RF Mode value*/
        while(((*data_parser) != end_of_rf_mode)&&(watchdog_loop_count_2 < 10))
        {
          /* extract the mode value in ASCII if it is not space value*/
          if(*data_parser != 32)
          {
            data_place_holder[char_count++] = (*data_parser);
          }
          data_parser++;
          watchdog_loop_count_2++;
        }
        if((0 < char_count)&&(char_count < 8)&&(watchdog_loop_count_2 < 10))
        {
          /* place holder now have rf_mode value in ascii format. Combine it to make actual number*/
          for(i = (char_count-1) ; i>=0 ; i--)
          {
            rf_mode_temp += (data_place_holder[i] - 48)*(uint16)pow(10,(char_count-1-i));
            /* reset data place holder for further use*/
            data_place_holder[i] = 0;
          }
          RF_MSG_3(RF_ERROR, "rfcommon_nv_parse_offset_data_from_efs:"
                             "RF Mode Extracted %d, with char count %d, Num Entries: %d",
                              rf_mode_temp, char_count, efs_entries_count);

          /* assign RF Mode to global structure*/
          asdiv_offset_char_data[efs_entries_count].rf_mode = rf_mode_temp;
		  
          /*reset char count*/
          //char_count = 0;
          /*reset loop count*/
          //watchdog_loop_count_2 = 0;
        }
        else
        {
          RF_MSG_3(RF_ERROR,"No RF Mode Value extracted. Skipping this iteration"
                            "Char Count =%d, Loop Count = %d. Num Entries: %d",
                             char_count, watchdog_loop_count_2, efs_entries_count);
          /*reset loop count*/
          // watchdog_loop_count_2 = 0;	
          /*reset char count*/
          // char_count = 0;
          /* bring back data_parser to from where it started searching for RF Mode*/
          data_parser = ++data_parser_2;
          continue;
        }
      }
      else
      {
        RF_MSG_1(RF_ERROR,"No start marker found for RF Mode. Skipping this iteration %d", efs_entries_count);
        /* increment data_parser pointer by at least one count for next itteration of RF Mode Search
        data_parser_2++;*/
        /* initialize data_parser to data_parser_2 to start for next rf mode search itteration*/
        data_parser = ++data_parser_2;
        continue;
      }	 
    }
    else
    {
      RF_MSG(RF_ERROR,"No 'rf_mode' indicator found in rest of the file. Breaking the loop");
      break;
    }

    /* next search for 'pwr' in EFS file corresponding to given rf mode found earlier*/
    data_parser_2 = data_parser;
    /* Check the occurance of rf_mode first */
    data_parser = strstr(data_parser_2, offset);

    if (NULL != data_parser)
    {  
      /*increment the data pointer to go pass "rf_mode"
      data_ptr_temp = data_parser + rf_mode_char_count; */

      data_parser_2 = data_parser;
 
      /* move data parser to start of mode value which is expected to wrapped in ""*/
      data_parser = strstr(data_parser_2, start_of_offset);

      if (NULL != data_parser)
      {
        /* increment data parser by 1 to point to actual offset value*/	 
        data_parser++;
		
        /*reset char count*/
        char_count = 0;
        /*reset loop count*/
        watchdog_loop_count_2 = 0;
        /*reset temp offset variable*/
        offset_temp = 0;
        /* reset the negative offset flag*/
        offset_is_negative = FALSE;
        
        /* loop to extract the RF Mode value*/
        while(((*data_parser) != end_of_offset)&&(watchdog_loop_count_2 < 10))
        {
          /* extract the mode value in ASCII if it is not space value*/
          if(*data_parser != 32)
          {
            /* check if the offset is negative*/
            if(*data_parser == 45)
            {
              offset_is_negative = TRUE;
            }
            else
            {
              data_place_holder[char_count++] = (*data_parser);
            }
          }
          data_parser++;
          watchdog_loop_count_2++;
        }

        if((0 < char_count)&&(char_count < 8)&&(watchdog_loop_count_2 < 10))
        {
          /* place holder now have rf_mode value in ascii format. Combine it to make actual number*/
          for(i = (char_count-1) ; i>=0 ; i--)
          {
            offset_temp += (data_place_holder[i]- 48)*(uint16)pow(10,(char_count-1-i));
            /* reset data place holder for further use*/
            data_place_holder[i] = 0;
          }
		  /* assign RF Mode to global structure*/
          asdiv_offset_char_data[efs_entries_count].asdiv_offset_db10 = 
                             (offset_is_negative)?((-1)*offset_temp):offset_temp;

          RF_MSG_3(RF_ERROR, "rfcommon_nv_parse_offset_data_from_efs:"
                             "offset Extracted %d, with char count %d, Num Entries: %d",
                              asdiv_offset_char_data[efs_entries_count].asdiv_offset_db10, char_count, efs_entries_count);
          /* increment the EFS entry count*/
          efs_entries_count++;
		  
          /*reset char count
            char_count = 0;
          /*reset loop count
           watchdog_loop_count_2 = 0;*/
        }
        else
        {
          RF_MSG_3(RF_ERROR,"No RF Mode Value extracted. Skipping this iteration"
                            "Char Count =%d, Loop Count = %d, Num Entries : %d",
                             char_count, watchdog_loop_count_2, efs_entries_count);
          /*reset loop count*/
          //watchdog_loop_count_2 = 0;	
          //char_count = 0;
          /* bring back data_parser to from where it started searching for RF Mode*/
          data_parser = ++data_parser_2;
          /* values should be stored in next entry so increase the nm entry counter*/   
          efs_entries_count++;		  
          continue;
        }
      }
      else
      {
        RF_MSG_1(RF_ERROR,"No start marker found for offset. Skipping this iteration %d", efs_entries_count);
        /* increment data_parser pointer by at least one count for next itteration of RF Mode Search*/
        data_parser = ++data_parser_2;
        /* values should be stored in next entry so increase the nm entry counter*/   
        efs_entries_count++;				
        continue;
      }	 
    }
    else
    {
      RF_MSG(RF_ERROR,"No 'pwr' indicator found in rest of the file. Breaking the loop");
      break;
    }
    watchdog_loop_count_1++;
  }

  /* update the EFS char data size*/
  if(0 != efs_entries_count)
  {
    asdiv_offset_char_data_size =  efs_entries_count;
  }
  else
  {
    status = FALSE;
  }
  return status;
};

/*--------------------------------------------------------------------------*/
/* rfcommon_asdiv_offset_char_efs_get_data                                  */
/*--------------------------------------------------------------------------*/
/*!
 
*/
boolean rfcommon_asdiv_offset_char_efs_get_data
(
  void
)
{
  char file_path[ASDIV_EFS_MAX_PATH_SIZE] = "/nv/item_store/rfnv/asdiv_char.dat"; // full file path
  uint8* data = NULL;							   // ptr to EFS data
  int32 data_size = 0;			
  struct fs_stat temp_buf;						   // fs buffer
  boolean status = FALSE;
  int i;
  
  /* EFS file will be read only once */
  //if (autopin_efs_data_read_once == FALSE)
  {
	/* Check if the EFS file exists */
	if( efs_stat( file_path, &temp_buf) == -1)
	{
	  /* file not found */
	  MSG_SPRINTF_1(MSG_SSID_RF, MSG_LEGACY_MED,
					"rfcommon_asdiv_offset_char_efs_get_data: FILE not found: %s ",
					file_path);
          return status;
	}

	data_size = (int32)temp_buf.st_size;

	/* Allocate memory for data */
	data = (uint8 *)modem_mem_alloc( data_size, MODEM_MEM_CLIENT_RFA);

	if( data == NULL)
	{
	  MSG_SPRINTF_2(MSG_SSID_RF, MSG_LEGACY_MED,
					"rfcommon_asdiv_offset_char_efs_get_data: Mem Alloc failed"
					" for scenario data. FILE %s, size=%d",
					file_path, data_size);
	  return status;
	}

	/* Obtain file data */
	data_size = rfcommon_efs_get( file_path, data, temp_buf.st_size);

	MSG_SPRINTF_2(MSG_SSID_RF, MSG_LEGACY_MED,
				  "rfcommon_asdiv_offset_char_efs_get_data: File %s found. File size(%d)",
				  file_path, data_size);

	status = rfcommon_nv_parse_offset_data_from_efs((const char*)data, data_size);   		   	
  }
  
  return status;
	  
} /* rfcommon_autopin_efs_get_data */


