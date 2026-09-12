/*! \file geran_nv.c 
 
  @brief This module contains functionality related to GERAN NV items
    Create /nv/item_files/conf/geran.conf in EFS.
    The geran.conf file lists the EFS-NV items owned by GERAN.
    This file is used by tools such as QXDM to access the items, 
    and determines which items are included in QCN back-ups.
 
                Copyright (c) 2012-2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


$Header: //components/rel/geran.mpss/7.4.0/gcommon/src/geran_nv.c#3 $ 
$DateTime: 2021/02/07 22:55:02 $ 

when       who      what, where, why
--------   -------- ---------------------------------------------
06/06/16   sp       CR1024701: Featurize GSM3 Clients
08/09/17   sn       CR1061730 FR43082: TA during PS Procedures- GL1 changes
29/03/16   mn       CR995137 Access tune away support: Allowing other tech's paging at GPRS access
23/10/15   nn       CR923517: GERAN F3 reduction
08/24/15   msa    CR894019: Compiler warnings in GCOMMON files
10/03/15   sjv      CR805970: SCell RSSI Threshold set through a new NV for DR to SR Fallback
23/02/15   sjv      CR799313: Using new mcfg APIs replacing efs_get for Geran

===========================================================================*/



/*----------------------------------------------------------------------------
 * Include Files
 *--------------------------------------------------------------------------*/

#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"

#include "mcfg_fs.h"

#ifdef FEATURE_MODEM_CONFIG_REFRESH
#include "mcfg_refresh.h"
#include "geran_dual_sim.h"
#include "rr_nv_g.h"
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

#include "geran_nv.h"
#include "geran_efs_nv_items.h"
#include "string.h"
#include <stringl.h>
#include "fs_public.h"
#include "msg.h"
#include "gprs_mem.h"
#include "geran_multi_sim.h"
#include "gl1_hw_g.h"
#include "mmgsdilib.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Type Declarations
 *--------------------------------------------------------------------------*/

typedef struct
{
  const char* str_ptr;
  uint8 str_len;
} item_info_t;

#define G2W_DSDS_TA_SUPPORT  0x01
#define G2W_TSTS_TA_SUPPORT  0x02
#define G2T_DSDS_TA_SUPPORT  0x04
#define G2T_TSTS_TA_SUPPORT  0x08
#define G2C_DSDS_TA_SUPPORT  0x10
#define G2C_TSTS_TA_SUPPORT  0x20
#define G2L_DSDS_TA_SUPPORT  0x40
#define G2L_TSTS_TA_SUPPORT  0x80

/*----------------------------------------------------------------------------
 * Global Data Definitions
 *--------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
 * Static Variable Definitions
 *--------------------------------------------------------------------------*/

struct
{
  boolean  valid;
#ifdef FEATURE_DUAL_SIM
  uint8    default_rf_chain;
  uint8    g2x_ta_support;
  /*Store the RSSI Threshold for Serving Cell for DR to SR Fallback*/
  int16    gsm_dr_to_sr_falback_rssi_thresh;
  uint8    dsc_threshold_for_slow_irat;
#endif // FEATURE_DUAL_SIM
#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP)
  boolean  qsh_debug_enabled;
#endif /* FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_DUMP */
#ifdef FEATURE_DATA_PDU_DEBUG
  uint8    data_pdu_debug_mask;
#endif /* FEATURE_DATA_PDU_DEBUG */
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
  boolean  ptm_err_recovery;
#endif /* FEATURE_GERAN_PTM_ERR_RECOVERY */
  boolean  allow_ta_dl_unack;
} geran_nv_data;

#ifdef FEATURE_MODEM_CONFIG_REFRESH
typedef struct
{
  /* order registerd with MCFG */
  mcfg_refresh_order_e_type    reg_order;
  /* slots registerd with MCFG to receive refresh update */
  mcfg_refresh_index_mask_type reg_slot_mask;
  /* subs registerd with MCFG to receive refresh update */
  mcfg_refresh_index_mask_type reg_sub_mask;
  /* groups registerd with MCFG to receive refresh update */
  mcfg_refresh_group_type      reg_group;
  /* Type of refresh (slot/sub) registered with MCFG */
  mcfg_refresh_type_e_type     reg_type;
  /* Reference ID received from MCFG in call back function */
  uint32                       ref_ids[MCFG_REFRESH_INDEX_MAX + 1];
} geran_nv_refresh_data_t;

static geran_nv_refresh_data_t geran_nv_refresh_data;
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

/*----------------------------------------------------------------------------
 * Static Function Declarations
 *--------------------------------------------------------------------------*/

#ifdef FEATURE_MODEM_CONFIG_REFRESH
static geran_nv_refresh_data_t *geran_nv_refresh_get_data_ptr(void);
static gas_id_t geran_nv_refresh_map_sub_index_to_gas_id(mcfg_refresh_index_type sub_index);
static mcfg_refresh_index_type geran_nv_refresh_map_gas_id_to_sub_index(gas_id_t gas_id);
static boolean geran_nv_refresh_validate_and_store_cb_params(mcfg_refresh_info_s_type *p_info);
static boolean geran_nv_refresh_cb(mcfg_refresh_info_s_type *p_info);
static void geran_nv_refresh_register_cb(void);
static void geran_nv_refresh_init(void);
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

/*---------------------------------------------------------------------------
 * Function Definitions
 *--------------------------------------------------------------------------*/

/*!
 * \brief Maps sys_modem_as_id_e_type enum to mcfg_fs_sub_id_e_type enum
 *
 * \return mcfg_fs_sub_id_e_type enum value
 */
static mcfg_fs_sub_id_e_type geran_map_as_id_to_mcfg_fs_sub_id(const sys_modem_as_id_e_type as_id)
{
  mcfg_fs_sub_id_e_type mcfg_fs_sub_id;

  switch(as_id)
  {
    case SYS_MODEM_AS_ID_2:
      mcfg_fs_sub_id = MCFG_FS_SUBID_1;
      break;

    case SYS_MODEM_AS_ID_3:
      mcfg_fs_sub_id = MCFG_FS_SUBID_2;
      break;

    default:
      mcfg_fs_sub_id = MCFG_FS_SUBID_0;
  }

  return mcfg_fs_sub_id;
}

/*!
 * \brief Wrapper API for mcfg_fs_read for reading EFS Items for primary subscription
 * - consistent with legacy efs_get API usage
 *
 * \return length of data read if successful else -1
 */
int geran_efs_read_primary(const char *path, void *data, uint32 length)
{
  mcfg_fs_status_e_type efs_status;

  efs_status = mcfg_fs_read(path,
                            data,
                            length,
                            MCFG_FS_TYPE_EFS,
                            MCFG_FS_SUBID_0); /* primary subscription */

  if (MCFG_FS_STATUS_OK == efs_status)
  {
    return (int)length;
  }
  else
  {
    MSG_GERAN_LOW_1("EFS READ ERROR: status: %d ", (int)efs_status);
    return(-1);
  }
} /* geran_efs_read_primary */

/*!
 * \brief Wrapper API for mcfg_fs_read for reading EFS Items per subscription
 *
 * \return length of data read if successful else -1
 */
int geran_efs_read_per_sub(const char *path, void *data, uint32 length, const sys_modem_as_id_e_type as_id)
{
  mcfg_fs_status_e_type efs_status;

  efs_status = mcfg_fs_read(path,
                            data,
                            length,
                            MCFG_FS_TYPE_EFS,
                            geran_map_as_id_to_mcfg_fs_sub_id(as_id));

  if (MCFG_FS_STATUS_OK == efs_status)
  {
    return (int)length;
  }
  else
  {
    MSG_GERAN_LOW_1("EFS READ ERROR: status: %d ", (int)efs_status);
    return(-1);
  }
}

/*!
 * \brief Wrapper API for mcfg_fs_stat for getting the EFS item stats per subscription
 *
 * \return boolean - TRUE if successful, FALSE otherwise
 */
boolean geran_efs_stat_per_sub(const char *path, struct fs_stat *data, const sys_modem_as_id_e_type as_id)
{
  mcfg_fs_status_e_type efs_status;

  efs_status = mcfg_fs_stat(path,
                            data,
                            MCFG_FS_TYPE_EFS,
                            geran_map_as_id_to_mcfg_fs_sub_id(as_id));

  if (MCFG_FS_STATUS_OK == efs_status)
  {
    return(TRUE);
  }
  else
  {
    MSG_GERAN_LOW_1("EFS STAT ERROR: status: %d ", (int)efs_status);
  }

  return(FALSE);
}

/*----------------------------------------------------------------------------
 * Static Function Definitions
 *--------------------------------------------------------------------------*/

/*!
 * \brief Creates /nv/item_files/conf/geran.conf
 */
static void create_efs_nv_conf_file(void)
{
  int fd;     // File Descriptor

  // Create geran.conf if it doesn't exist
  fd = efs_open("/nv/item_files/conf/geran.conf",
                O_WRONLY | O_CREAT | O_EXCL,
                S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

  // A return of 0 or greater means success
  if (fd >= 0)
  {
    // Limit string lengths to 127
    #define MAX_STR_LEN 127

    // Limit number of items to 255
    #define MAX_NUM_OF_ITEMS 255

    // Number of items defined in geran_efs_nv_items.h
    #define NUM_OF_ITEMS (sizeof(geran_efs_nv_items) / sizeof(const char*))

    item_info_t *items_ptr;
    uint8 num_of_items;

    // Cap the number of items at MAX_NUM_OF_ITEMS
    num_of_items = MIN(NUM_OF_ITEMS, MAX_NUM_OF_ITEMS);

    // Allocate memory to store the information about the items
    items_ptr = GPRS_MEM_MALLOC(sizeof(item_info_t) * num_of_items);

    if (items_ptr != NULL)
    {
      int i;
      item_info_t *item_ptr;
      int result; // result of file operation
      uint16 file_len = 0;
      char *file_buffer_ptr;

      // Start at the beginning of the list of items
      item_ptr = items_ptr;

      for (i = 0; i < num_of_items; i++)
      {
        const char* str_ptr = geran_efs_nv_items[i]; // see geran_efs_nv_items.h
        uint8 str_len = strlen(str_ptr);  // find the length of the string

        // Cap the string length at MAX_STR_LEN
        str_len = MIN(str_len, MAX_STR_LEN);

        // Store a pointer to the text string and its length
        item_ptr->str_ptr = str_ptr;
        item_ptr->str_len = str_len;

        // Increase the total file length count by the size of the string
        file_len += str_len;

        // Add 2 because \r\n will be added to each string
        file_len += 2;

        // Point to next item
        ++item_ptr;
      }

      // Allocate a buffer to store the file contents
      file_buffer_ptr = (char*)GPRS_MEM_MALLOC(file_len);

      if (file_buffer_ptr != NULL)
      {
        char* file_buffer_write_ptr = file_buffer_ptr;

        // Start at the beginning of the list of items
        item_ptr = items_ptr;

        for (i = 0; i < num_of_items; i++)
        {
          // Copy the string in to the file contents buffer
          // Buffer overflow cannot occur as we have worked out above how much we're going
          // to write but use memscpy anyway so all use of memcpy is expunged
          memscpy(file_buffer_write_ptr, file_len, item_ptr->str_ptr, item_ptr->str_len);

          // Increase the file buffer write pointer by the length of the string just copied
          file_buffer_write_ptr += item_ptr->str_len;

          // Add \r\n and increase the file buffer write pointer by 2
          memscpy(file_buffer_write_ptr, 2, "\r\n", 2);
          file_buffer_write_ptr += 2;

          // Point to next item
          ++item_ptr;
        }

        // Write to the file
        result = efs_write(fd, (const void*)file_buffer_ptr, file_len);

        if (result <= 0)
        {
          MSG_GERAN_ERROR_1("Error (%d) writing geran.conf",result);
        }

        // Free allocated memory
        GPRS_MEM_FREE(file_buffer_ptr);
      }

      // Free allocated memory
      GPRS_MEM_FREE(items_ptr);
    }

    // Close the file
    (void) efs_close(fd);
  }

  return;
}

/*!
 * \brief Reads general GERAN EFS-NV items
 */
void geran_read_efs_nv_items(void)
{
#ifdef FEATURE_DUAL_SIM
  uint8 default_rf_chain;
  uint8 g2x_ta_support;
  int16 gsm_dr_to_sr_falback_rssi_thresh;
  uint8 dsc_threshold_for_slow_irat;

  /* -1 indicates file not present */
  geran_nv_data.default_rf_chain = -1;

  /* If no NV item file is present, G2W/G2T/G21x/G2L TA is enabled for DSDS */
  geran_nv_data.g2x_ta_support = (G2W_DSDS_TA_SUPPORT | G2T_DSDS_TA_SUPPORT | G2C_DSDS_TA_SUPPORT | G2L_DSDS_TA_SUPPORT);

  /* Threshold defaults to -102 dBm */
  geran_nv_data.gsm_dr_to_sr_falback_rssi_thresh = GSM_DR_TO_SR_FALLBACK_THRESH_DEF;

  /* DSC control is set to 100% by default for Slow IRAT mode */
  geran_nv_data.dsc_threshold_for_slow_irat = GERAN_MAX_DSC_PERCENT;

  if (geran_efs_read_primary(GERAN_EFS_DEF_RF_CHAIN,
              &default_rf_chain,
              sizeof(default_rf_chain)) > 0)
  {
    if ((default_rf_chain == 0) || (default_rf_chain == 1))
    {
      MSG_GERAN_HIGH_1("default_rf_chain=%d", default_rf_chain);
      geran_nv_data.default_rf_chain = default_rf_chain;
    }
  }

  if (geran_efs_read_primary(GERAN_EFS_G2X_TA_SUPPORT,
                             &g2x_ta_support,
                             sizeof(g2x_ta_support)) > 0)
  {
    MSG_GERAN_HIGH_1("G2X tuneaway support 0x%02X", g2x_ta_support);
    geran_nv_data.g2x_ta_support = g2x_ta_support;
  }

  if (geran_efs_read_primary(GERAN_EFS_DR_TO_SR_FALLBACK_RSSI_THRESH,
                             &gsm_dr_to_sr_falback_rssi_thresh,
                             sizeof(gsm_dr_to_sr_falback_rssi_thresh)) >0)
  {
    MSG_GERAN_HIGH_1("EFS Read: gsm_dr_to_sr_falback_rssi_thresh: %d" ,gsm_dr_to_sr_falback_rssi_thresh);

    /* Check to see if the NV Set is within range */
    if (((gsm_dr_to_sr_falback_rssi_thresh << 4) >= BSP_RX_POWER_LOWER_LIMIT_dBm_x16) &&
        ((gsm_dr_to_sr_falback_rssi_thresh << 4) <= BSP_RX_POWER_UPPER_LIMIT_dBm_x16))
    {
      geran_nv_data.gsm_dr_to_sr_falback_rssi_thresh = gsm_dr_to_sr_falback_rssi_thresh;
    }
    else
    {
      MSG_GERAN_ERROR_2("gsm_dr_to_sr_falback_rssi_thresh: Invalid Value:%d Value set to default:%d",
                        gsm_dr_to_sr_falback_rssi_thresh,
                        geran_nv_data.gsm_dr_to_sr_falback_rssi_thresh);
    }
  }
  else
  {
    MSG_GERAN_HIGH_1("EFS Read: NV/EFS Not Set/Found so gsm_dr_to_sr_falback_rssi_thresh: %d",
                     geran_nv_data.gsm_dr_to_sr_falback_rssi_thresh);
  }

  if (geran_efs_read_primary(GERAN_EFS_DSC_THRESHOLD_FOR_SLOW_IRAT,
                             &dsc_threshold_for_slow_irat,
                             sizeof(dsc_threshold_for_slow_irat)) >0)
  {
    MSG_GERAN_HIGH_1("EFS Read: dsc_threshold_for_slow_irat: %d" , dsc_threshold_for_slow_irat);

    /* Check to see if the NV Set is within range 0-100 %*/
    if (dsc_threshold_for_slow_irat <= GERAN_MAX_DSC_PERCENT)
    {
      geran_nv_data.dsc_threshold_for_slow_irat = dsc_threshold_for_slow_irat;
    }
    else
    {
      MSG_GERAN_ERROR_2("dsc_threshold_for_slow_irat: Invalid Value:%d Value set to default:%d",
                        dsc_threshold_for_slow_irat,
                        geran_nv_data.dsc_threshold_for_slow_irat);
    }
  }
  else
  {
    MSG_GERAN_HIGH_1("EFS Read: NV/EFS Not Set/Found so dsc_threshold_for_slow_irat: %d",
                     geran_nv_data.dsc_threshold_for_slow_irat);
  }

#endif /* FEATURE_DUAL_SIM */

#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP)
  {
    // Read /nv/item_files/modem/geran/qsh_debug_enabled
    int8 efs_val;
  
    if (geran_efs_read_primary(GERAN_EFS_QSH_DEBUG_ENABLED,
          &efs_val,
          sizeof(efs_val)) < 0)
    {
      MSG_GERAN_HIGH_0("EFS-NV qsh_debug_enabled not present");
      efs_val = -1;
    }
  
    // Sanity-check the result is 0 or 1
    if ((efs_val != 0) && (efs_val != 1))
    {
      efs_val = 0;  // Default is DISABLED for this EFS-NV item
    }
  
    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("GERAN QSH debug is ENABLED");
      geran_nv_data.qsh_debug_enabled = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_0("GERAN QSH debug is DISABLED");
      geran_nv_data.qsh_debug_enabled = FALSE;
    }
  }
#endif /* FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_DUMP */

#ifdef FEATURE_DATA_PDU_DEBUG
  // Read /nv/item_files/modem/geran/data_pdu_debug_mask
  {
    int8 efs_val;

    if (geran_efs_read_primary(GERAN_EFS_DATA_PDU_DEBUG_MASK, &efs_val, sizeof(efs_val)) > 0)
    {
      MSG_GERAN_HIGH_1("EFS Read: data_pdu_debug_mask = 0x%x", efs_val);
    }
    else
    {
      MSG_GERAN_HIGH_0("EFS Read: data_pdu_debug_mask not present");
      efs_val = 0;
    }
    
    geran_nv_data.data_pdu_debug_mask = (uint8) efs_val;
  }
#endif /* FEATURE_DATA_PDU_DEBUG */

  {
    /* 
     *  Read /nv/item_files/modem/geran/allow_ta_dl_unack
     *  If NV is enabled, allow TA in RLC DL UNACK mode, otherwise block
     */
    int8 efs_val;

    if (geran_efs_read_primary(GERAN_EFS_ALLOW_TA_DL_UNACK,
          &efs_val,
          sizeof(efs_val)) < 0)
    {
      MSG_GERAN_HIGH_0("EFS-NV allow_ta_dl_unack not present");
      efs_val = -1;
    }
  
    // Update the module store
    if (efs_val == 1)
    {
      MSG_GERAN_HIGH_0("TA is allowed for DL RLC UNACK mode");
      geran_nv_data.allow_ta_dl_unack = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_1("TA is blocked for DL RLC UNACK mode[efs_val = %d]", efs_val);
      geran_nv_data.allow_ta_dl_unack = FALSE;
    }
  }

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
  {
    /* 
     *  Read /nv/item_files/modem/geran/ptm_err_recovery
     *  If EFS is 0, NV is disabled and hence ERR recovery is disabled.
     *  Otherwise, ERR recovery is considered enabled
     */
    int8 efs_val;

    if (geran_efs_read_primary(GERAN_EFS_PTM_ERR_RECOVERY,
          &efs_val,
          sizeof(efs_val)) < 0)
    {
      MSG_GERAN_HIGH_0("EFS-NV ptm_err_recovery not present");
      efs_val = -1;
    }
  
    // Update the module store
    if (efs_val == 0)
    {
      MSG_GERAN_HIGH_0("GMAC-L1 PTM_ERR_RECOVERY is Disabled");
      geran_nv_data.ptm_err_recovery = FALSE;
    }
    else
    {
      MSG_GERAN_HIGH_1("GMAC-L1 PTM_ERR_RECOVERY is Enabled [efs_val = %d]", efs_val);
      geran_nv_data.ptm_err_recovery = TRUE;
    }
  }
#endif /* FEATURE_GERAN_PTM_ERR_RECOVERY */

  geran_nv_data.valid = TRUE;

  return;
}

#ifdef FEATURE_DUAL_SIM
/*!
 * \brief Returns the value of EFS-NV item default_rf_chain.
 *
 * \return int - {0,1} or -1 if not defined
 */
int geran_nv_get_default_rf_chain(void)
{
  if (geran_nv_data.valid)
  {
    return geran_nv_data.default_rf_chain;
  }

  return -1;
}
#endif /* FEATURE_DUAL_SIM */

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
/*!
* \brief Returns the GPRS->WCDMA tuneaway support based on the values of EFS-NV items.
 *
 * \return int - {GERAN_TA_SUPPORTED, GERAN_TA_NOT_SUPPORTED, GERAN_NV_MEM_NOT_VALID}
 */
int geran_nv_g2w_ta_supported(void)
{
  sys_modem_dual_standby_pref_e_type mode_pref = geran_get_sys_multi_sim_pref();

  if (geran_nv_data.valid)
  {
    if (SYS_MODEM_DS_PREF_DUAL_STANDBY == mode_pref)
    {
      return((geran_nv_data.g2x_ta_support & G2W_DSDS_TA_SUPPORT)?GERAN_TA_SUPPORTED:GERAN_TA_NOT_SUPPORTED);
    }
    else if (SYS_MODEM_DS_PREF_TRIPLE_STANDBY == mode_pref)
    {
      return((geran_nv_data.g2x_ta_support & G2W_TSTS_TA_SUPPORT)?GERAN_TA_SUPPORTED:GERAN_TA_NOT_SUPPORTED);
    }
    else
    {
      return(GERAN_TA_NOT_SUPPORTED);
    }
  }

  return(GERAN_NV_MEM_NOT_VALID);
}


#if defined(FEATURE_GTA_DURING_PS_RACH) || defined(FEATURE_G2X_IDLE_TUNEAWAY)
/*!
 * \brief Returns the G->X tech tuneaway support based on the values of EFS-NV items.
 * 
 * \return int - {GERAN_TA_SUPPORTED, GERAN_TA_NOT_SUPPORTED, GERAN_NV_MEM_NOT_VALID} 
 */

int geran_nv_g2x_ta_supported(grm_client_enum_t client)
{
  sys_modem_dual_standby_pref_e_type mode_pref = geran_get_sys_multi_sim_pref();
    
  if (geran_nv_data.valid)
  {
    if (SYS_MODEM_DS_PREF_DUAL_STANDBY == mode_pref)
    {
      MSG_GERAN_HIGH_1("G2X tuneaway support %d", geran_nv_data.g2x_ta_support);
      switch(client)
      {
         case  GRM_GSM1  :
         case  GRM_GSM2  :
         case  GRM_GPRS1 :
         case  GRM_GPRS2 :
         return(GERAN_TA_SUPPORTED);//supported by default

         case  GRM_UMTS :
         case  GRM_UMTS_SECONDARY:
         case  GRM_UMTS2 :
         case  GRM_UMTS_SECONDARY2:
         return((geran_nv_data.g2x_ta_support & G2W_DSDS_TA_SUPPORT)?GERAN_TA_SUPPORTED:GERAN_TA_NOT_SUPPORTED);

         case  GRM_TDSCDMA :
         case  GRM_TDSCDMA_SECONDARY:
         return((geran_nv_data.g2x_ta_support & G2T_DSDS_TA_SUPPORT)?GERAN_TA_SUPPORTED:GERAN_TA_NOT_SUPPORTED);

         case  GRM_1X :
         case  GRM_1X_SECONDARY:
         return((geran_nv_data.g2x_ta_support & G2C_DSDS_TA_SUPPORT)?GERAN_TA_SUPPORTED:GERAN_TA_NOT_SUPPORTED);

         case  GRM_LTE :
         case  GRM_LTE_SECONDARY:
         case  GRM_LTE_SUB2 :
         case  GRM_LTE_SUB2_SECONDARY:
         return((geran_nv_data.g2x_ta_support & G2L_DSDS_TA_SUPPORT)?GERAN_TA_SUPPORTED:GERAN_TA_NOT_SUPPORTED);

         default :
         return(GERAN_TA_NOT_SUPPORTED);
       }

   }
    else
    {
      return(GERAN_TA_NOT_SUPPORTED);
    }
  }

  return(GERAN_NV_MEM_NOT_VALID);
}

#endif /* FEATURE_GTA_DURING_PS_RACH/FEATURE_G2X_IDLE_TUNEAWAY */


/*!
 * \brief Returns the GPRS->TDSCDMA tuneaway support based on the values of EFS-NV items.
 *
 * \return int - {GERAN_TA_SUPPORTED, GERAN_TA_NOT_SUPPORTED, GERAN_NV_MEM_NOT_VALID}
 */
int geran_nv_g2t_ta_supported(void)
{
  sys_modem_dual_standby_pref_e_type mode_pref = geran_get_sys_multi_sim_pref();

  if (geran_nv_data.valid)
  {
    if (SYS_MODEM_DS_PREF_DUAL_STANDBY == mode_pref)
    {
      return((geran_nv_data.g2x_ta_support & G2T_DSDS_TA_SUPPORT)?GERAN_TA_SUPPORTED:GERAN_TA_NOT_SUPPORTED);
    }
    else if (SYS_MODEM_DS_PREF_TRIPLE_STANDBY == mode_pref)
    {
      return((geran_nv_data.g2x_ta_support & G2T_TSTS_TA_SUPPORT)?GERAN_TA_SUPPORTED:GERAN_TA_NOT_SUPPORTED);
    }
    else
    {
      return(GERAN_TA_NOT_SUPPORTED);
    }
  }

  return(GERAN_NV_MEM_NOT_VALID);
}

/*!
 * \brief Returns the GPRS->CDMA tuneaway support based on the values of EFS-NV items.
 *
 * \return int - {GERAN_TA_SUPPORTED, GERAN_TA_NOT_SUPPORTED, GERAN_NV_MEM_NOT_VALID}
 */
int geran_nv_g21x_ta_supported(void)
{
  sys_modem_dual_standby_pref_e_type mode_pref = geran_get_sys_multi_sim_pref();

  if (geran_nv_data.valid)
  {
    if (SYS_MODEM_DS_PREF_DUAL_STANDBY == mode_pref)
    {
      return((geran_nv_data.g2x_ta_support & G2C_DSDS_TA_SUPPORT)?GERAN_TA_SUPPORTED:GERAN_TA_NOT_SUPPORTED);
    }
    else if (SYS_MODEM_DS_PREF_TRIPLE_STANDBY == mode_pref)
    {
      return((geran_nv_data.g2x_ta_support & G2C_TSTS_TA_SUPPORT)?GERAN_TA_SUPPORTED:GERAN_TA_NOT_SUPPORTED);
    }
    else
    {
      return(GERAN_TA_NOT_SUPPORTED);
    }
  }

  return(GERAN_NV_MEM_NOT_VALID);
}

/*!
 * \brief Returns the GPRS->LTE tuneaway support based on the values of EFS-NV items.
 *
 * \return int - {GERAN_TA_SUPPORTED, GERAN_TA_NOT_SUPPORTED, GERAN_NV_MEM_NOT_VALID}
 */
int geran_nv_g2l_ta_supported(void)
{
  sys_modem_dual_standby_pref_e_type mode_pref = geran_get_sys_multi_sim_pref();

  if (geran_nv_data.valid)
  {
    if (SYS_MODEM_DS_PREF_DUAL_STANDBY == mode_pref)
    {
      return((geran_nv_data.g2x_ta_support & G2L_DSDS_TA_SUPPORT)?GERAN_TA_SUPPORTED:GERAN_TA_NOT_SUPPORTED);
    }
    else if (SYS_MODEM_DS_PREF_TRIPLE_STANDBY == mode_pref)
    {
      return((geran_nv_data.g2x_ta_support & G2L_TSTS_TA_SUPPORT)?GERAN_TA_SUPPORTED:GERAN_TA_NOT_SUPPORTED);
    }
    else
    {
      return(GERAN_TA_NOT_SUPPORTED);
    }
  }

  return(GERAN_NV_MEM_NOT_VALID);
}
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

#ifdef FEATURE_DATA_PDU_DEBUG
/*!
 * \brief Indicates if data PDU debug print is enabled for a given mask.
 *        
 * \return boolean - TRUE if enabled, FALSE otherwise
 */
boolean geran_nv_data_pdu_debug_enabled(const uint8 mask)
{
  if (geran_nv_data.valid)
  {
    return ((geran_nv_data.data_pdu_debug_mask & mask) != 0);
  }

  return FALSE;
}
#endif /* FEATURE_DATA_PDU_DEBUG */

#ifdef FEATURE_MODEM_CONFIG_REFRESH
/*!
 * \brief return pointer to geran_nv_refresh_data.
 */
static geran_nv_refresh_data_t *geran_nv_refresh_get_data_ptr(void)
{
  return &geran_nv_refresh_data;
}


/*!
 * \brief maps SUB index to GAS-ID.
 */
static gas_id_t geran_nv_refresh_map_sub_index_to_gas_id(mcfg_refresh_index_type sub_index)
{
  sys_modem_as_id_e_type as_id = SYS_MODEM_AS_ID_NONE;
  gas_id_t               gas_id;

  switch (sub_index)
  {
    case MCFG_REFRESH_INDEX_0:
      as_id = SYS_MODEM_AS_ID_1;
      break;

    case MCFG_REFRESH_INDEX_1:
      as_id = SYS_MODEM_AS_ID_2;
      break;

    case MCFG_REFRESH_INDEX_2:
      as_id = SYS_MODEM_AS_ID_3;
      break;

    default:
      MSG_GERAN_ERROR_0("Invalid SUB Index received");
      break;
  }

  gas_id = geran_map_nas_id_to_gas_id(as_id);
  MSG_GERAN_HIGH_2("Mapping:: sub_index=%d ==> gas_id=%d", sub_index, gas_id);

  return gas_id;
}


/*!
 * \brief maps GAS-ID to SUB index.
 */
static mcfg_refresh_index_type geran_nv_refresh_map_gas_id_to_sub_index(gas_id_t gas_id)
{
  sys_modem_as_id_e_type  as_id;
  mcfg_refresh_index_type sub_index = MCFG_REFRESH_INDEX_INVALID;

  as_id = geran_map_gas_id_to_nas_id(gas_id);

  switch (as_id)
  {
    case SYS_MODEM_AS_ID_1:
      sub_index = MCFG_REFRESH_INDEX_0;
      break;

    case SYS_MODEM_AS_ID_2:
      sub_index = MCFG_REFRESH_INDEX_1;
      break;

    case SYS_MODEM_AS_ID_3:
      sub_index = MCFG_REFRESH_INDEX_2;
      break;

    default:
      MSG_GERAN_ERROR_0("Invalid GAS ID received");
      break;
  }

  MSG_GERAN_HIGH_2("Mapping:: gas_id=%d ==> sub_index=%d", gas_id, sub_index);

  return sub_index;
}


/*!
 * \brief validates params received from MCFG in call back function.
 * It store the params if they are valid.
 *
 *\param p_info(in) -contains sub to be refreshed and reference id
 *
 *\return boolean - TRUE: Params are valid and stored
 *                          FALSE: Invalid params received
 */
static boolean geran_nv_refresh_validate_and_store_cb_params(mcfg_refresh_info_s_type *p_info)
{
  geran_nv_refresh_data_t *geran_nv_refresh_data_ptr = geran_nv_refresh_get_data_ptr();
  boolean                  param_valid = TRUE;

  MSG_GERAN_HIGH_4("Rcvd params:: type=%d, slot=%d, sub=%d, ref_id=%d",
    p_info->type,
    p_info->slot_index,
    p_info->sub_index,
    p_info->reference_id);

  if ((p_info->type & geran_nv_refresh_data_ptr->reg_type) != 0)
  {
    switch (p_info->type)
    {
      case MCFG_REFRESH_TYPE_SUBS:
      {
        if ((p_info->sub_index <= MCFG_REFRESH_INDEX_MAX) &&
            (((1 << p_info->sub_index) & geran_nv_refresh_data_ptr->reg_sub_mask) != 0))
        {
          MSG_GERAN_HIGH_2("Storing params:: ref_id=%d for sub_index=%d",
            p_info->reference_id,
            p_info->sub_index);
          geran_nv_refresh_data_ptr->ref_ids[p_info->sub_index] = p_info->reference_id;
        }
        else
        {
          param_valid = FALSE;
        }

        break;
      }

      /* Refresh based on sub_id is only supported in GERAN */
      default:
        param_valid = FALSE;
        break;
    }
  }
  else
  {
    param_valid = FALSE;
  }

  return param_valid;
}


/*!
 * \brief Call back function for handling NV refresh.
 *
 *\param p_info(in) -contains sub to be refreshed and reference id
 *
 *\return boolean - TRUE: if refresh can be done
 *                          FALSE: error in received parameters
 */
static boolean geran_nv_refresh_cb(mcfg_refresh_info_s_type *p_info)
{
  gas_id_t gas_id;

  if (p_info == NULL)
  {
    MSG_GERAN_ERROR_0("p_info is NULL in call back");
    return FALSE;
  }

  MSG_GERAN_HIGH_1("NV Refresh Call Back:: sub_index=%d", p_info->sub_index);

  /* Validate and store received parameters */
  if (!geran_nv_refresh_validate_and_store_cb_params(p_info))
  {
    MSG_GERAN_ERROR_0("Invalid params received in call back");
    return FALSE;
  }

  /* Post a message to GRR to refresh NV items */
  gas_id = geran_nv_refresh_map_sub_index_to_gas_id(p_info->sub_index);
  rr_nv_send_nv_refresh_ind(gas_id);

  return TRUE;
}


/*!
 * \brief this function registers call back with MCFG for NV refresh.
 */
static void geran_nv_refresh_register_cb(void)
{
  mcfg_refresh_registration_s_type p_reg;
  geran_nv_refresh_data_t         *geran_nv_refresh_data_ptr = geran_nv_refresh_get_data_ptr();

  p_reg.order = geran_nv_refresh_data_ptr->reg_order;
  p_reg.slot_mask = geran_nv_refresh_data_ptr->reg_slot_mask;
  p_reg.sub_mask = geran_nv_refresh_data_ptr->reg_sub_mask;
  p_reg.group = geran_nv_refresh_data_ptr->reg_group;
  p_reg.cb = geran_nv_refresh_cb;

  MSG_GERAN_HIGH_4("NV Refresh Register cb:: order=%d, slot=%d, sub=%d, group=%d",
    p_reg.order,
    p_reg.slot_mask,
    p_reg.sub_mask,
    p_reg.group);

  /* register call back with MCFG for NV refresh */
  mcfg_refresh_register(&p_reg);
}


/*!
 * \brief Initialisation function for NV refresh.
 */
static void geran_nv_refresh_init(void)
{
  MSG_GERAN_HIGH_0("NV Refresh Init");
  memset(&geran_nv_refresh_data, 0, sizeof(geran_nv_refresh_data));

  /* order: Default order (i.e. 50) */
  geran_nv_refresh_data.reg_order = MCFG_REFRESH_ORDER_50;

  /* Our refresh will be based on SUB and will not be based on SLOT.  */
  geran_nv_refresh_data.reg_type = MCFG_REFRESH_TYPE_SUBS;
  geran_nv_refresh_data.reg_slot_mask = MCFG_REFRESH_INDEX_MASK_NONE;
  geran_nv_refresh_data.reg_sub_mask = MCFG_REFRESH_INDEX_MASK_ALL;

  /* group: reserved*/
  geran_nv_refresh_data.reg_group = 0;

  /* register call back with MCFG for NV refresh */
  geran_nv_refresh_register_cb();
}
#endif /* FEATURE_MODEM_CONFIG_REFRESH */


/*-----------------------------------------------------------------------------
 * Externalised Function Definitions
 *---------------------------------------------------------------------------*/

#ifdef FEATURE_MODEM_CONFIG_REFRESH
/*!
 * \brief function for informing MCFG about refresh done. Called from RR task.
 */
void geran_nv_refresh_done(const boolean done, const gas_id_t gas_id)
{
  geran_nv_refresh_data_t *geran_nv_refresh_data_ptr = geran_nv_refresh_get_data_ptr();
  mcfg_refresh_index_type  sub_index = geran_nv_refresh_map_gas_id_to_sub_index(gas_id);

  if (geran_nv_refresh_data_ptr == NULL)
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN, QSH_CLT_GRR, QSH_CLT_GRR)
#endif /* FEATURE_QSH_MDUMP */
    ERR_GERAN_FATAL_0_G("geran_nv_refresh_data_ptr == NULL");
  }

  if (sub_index <= MCFG_REFRESH_INDEX_MAX)
  {
    uint32 reference_id = geran_nv_refresh_data_ptr->ref_ids[sub_index];

    MSG_GERAN_HIGH_3_G("NV Refresh Done:: sub_index=%d, ref_id=%d, done=%d", sub_index, reference_id, done);

    /* NV refresh is not done if SUB is ACTIVE */
    if (done)
    {
      mcfg_refresh_done_w_status(reference_id, MCFG_REFRESH_SUCCESS);
    }
    else
    {
      mcfg_refresh_done_w_status(reference_id, MCFG_REFRESH_STATUS_SUB_IS_ACTIVE);
    }
  }
}
#endif /* FEATURE_MODEM_CONFIG_REFRESH */


/*!
 * \brief Module initialisation function. Called during RR task initialisation.
 */
void geran_nv_init(
  void
)
{
  // Zero the NV data structure
  memset(&geran_nv_data, 0, sizeof(geran_nv_data));

#ifdef FEATURE_MODEM_CONFIG_REFRESH
  geran_nv_refresh_init();
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

  // Create /nv/item_files/conf/geran.conf if it doesn't exist
  create_efs_nv_conf_file();
}

/*!
 *\brief Function to return the NV value set for the default RSSI threshold for SCell for DR-> SR Fallback
 * Returns in dBmx16
 */
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
int16 gsm_dr_to_sr_falback_rssi_thresh_dBmx16 (gas_id_t gas_id)
{
   MSG_GERAN_MED_1_G("geran_nv_dr_to_sr_fallback_rssi_thresh Read: %d",geran_nv_data.gsm_dr_to_sr_falback_rssi_thresh);
   return ((geran_nv_data.gsm_dr_to_sr_falback_rssi_thresh << 4 ));
}

/*!
 *\brief Function to return the NV value set for the DSC threshold for slow irat mode in terms of percentage to its max value
 * Returns in uint8 - 0-100%
 */
uint8 geran_nv_get_dsc_threshold_for_slow_irat(void)
{
   MSG_GERAN_MED_1("dsc_threshold_for_slow_irat Read: %d", geran_nv_data.dsc_threshold_for_slow_irat);
   return (geran_nv_data.dsc_threshold_for_slow_irat);
}

#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP)
/*!
 * \Function to check if the qsh debug EFS is enabled so that all GERAN modules can use it
 * 
 * \return boolean - TRUE if enabled, FALSE otherwise
 */
boolean geran_nv_qsh_debug_enabled(void)
{
  return geran_nv_data.qsh_debug_enabled;
}
#endif /* FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_DUMP */

/*!
 * \Function to check the EFS allow_ta_dl_unack, to allow/block tuneaway in DL RLC UNACK mode
 * 
 * \return boolean - TRUE if tuneaway allowed, FALSE otherwise
 */
boolean grlc_nv_allow_ta_dl_unack(void)
{
  return geran_nv_data.allow_ta_dl_unack;
}

/*!
 * \Function to check Ciphering logging enabled or not. 
 * 
 * \return boolean - By default TRUE if Test sim is configured, FALSE otherwise.
 */
boolean geran_ciphering_logging_enabled(gas_id_t gas_id)
{
  sys_modem_as_id_e_type  as_id = SYS_MODEM_AS_ID_NONE;

  as_id = geran_map_gas_id_to_nas_id(gas_id);

  /* Call UIM API "mmgsdi_is_test_mode_logging_enabled_for_subscription" is used to get whether security key logging is enabled. 
   * If enabled, logging of keys is allowed in the caller.
   * If disabled, If a test card is in use,it will always return TRUE.If it is a commercial card,
   * it will take into account the value in the protected NV item (/nv/item_files/modem/uim/mmgsdi/test_mode_logging_enabled".
   * Based on above efs value, it act accordingly. 
   */
  if (TRUE == mmgsdi_is_test_mode_logging_enabled_for_subscription(as_id))
  {
    return TRUE;
  }

  MSG_GERAN_HIGH_0("Ciphering related logging is disabled at UIM Level.");
  return FALSE;
}

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
/*!
 * \Function to check if the EFS ptm_err_recovery is enabled
 *  so that GMAC-GL1 error recovery mechanism can be executed
 * 
 * \return boolean - TRUE if ERR recovery is enabled, otherwise FALSE
 */
boolean geran_nv_ptm_err_recovery(void)
{
  return geran_nv_data.ptm_err_recovery;
}
#endif /* FEATURE_GERAN_PTM_ERR_RECOVERY */

/* EOF */

