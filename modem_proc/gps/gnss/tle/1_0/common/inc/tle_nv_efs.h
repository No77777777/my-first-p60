#ifndef TLE_EFS_H
#define TLE_EFS_H

#ifdef __cplusplus
extern "C" {
#endif

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                      Terrestrial Location Engine (TLE) Internal header file

GENERAL DESCRIPTION
  This file contains data-structure/function prototypes for EFS access in
  TLE module
     
EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS
  

  Copyright (c) 2010 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2015 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2015 - 2017 Qualcomm Technologies, Inc. 
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/tle/1_0/common/inc/tle_nv_efs.h#1 $
  
when       who   what, where, why
--------   ---   ----------------------------------------------------------
08/25/10   atien  GNSS_NV_EFS_TLE_XTRAT_TIME_INJECTION_CONTROL_DEFAULT is enabled
07/22/10   ah     File created
===========================================================================*/

/*=============================================================================

                                INCLUDE FILES

=============================================================================*/
#include "comdef.h"
#include "gps_nv_efs.h"



/*===========================================================================
=============================================================================

     Macros
   
=============================================================================
===========================================================================*/
#define TLE_TEST_CONTROL_GEN_IPC_MASK                           (0x0001)
#define TLE_TEST_CONTROL_GEN_UPLOAD_CONTROL_MSG                 (0x0002)  
#define TLE_TEST_CONTROL_USE_TDP_CANNED_DATA                    (0x0004) 
#define TLE_TEST_CONTROL_USE_TDP_TEST_LOOPBACK                  (0x0008)
#define TLE_TEST_CONTROL_ENABLE_ASN1_LOGGING                    (0x0010)
#define TLE_TEST_CONTROL_ENABLE_TOUCH_LOGGING                   (0x0020) 
#define TLE_MAX_SERVER_ADDRESS_SIZE  (50)
#define TLE_NUM_NV_RADII             ( 5)   /* Number of radii in the cellid default radius nv item */

#define GNSS_NV_EFS_TLE_XTRAT_PRIMARY_SVR_ADDRESS_DEFAULT                  "gtp1.izatcloud.net"
#define GNSS_NV_EFS_TLE_XTRAT_PRIMARY_SVR_PORT_DEFAULT                     (   443)  /* HTTPS */
#define GNSS_NV_EFS_TLE_XTRAT_SECONDARY_SVR_ADDRESS_DEFAULT                "gtp2.izatcloud.net"
#define GNSS_NV_EFS_TLE_XTRAT_SECONDARY_SVR_PORT_DEFAULT                   (   443)  /* HTTPS */
#define GNSS_NV_EFS_TLE_XTRAT_USER_SESSION_CONTROL_DEFAULT                 (     2)
#define GNSS_NV_EFS_TLE_XTRAT_TIME_INJECTION_CONTROL_DEFAULT               (     1) 
#define GNSS_NV_EFS_TLE_XTRAT_CLIENT_TOKEN_DEFAULT                         (     0)
#define GNSS_NV_EFS_TLE_XTRAT_DATAXFR_MAX_ELAPSED_THRESHOLD_DEFAULT        (    24) 
#define GNSS_NV_EFS_TLE_XTRAT_PERIODIC_UPLOAD_LIVECOUNT_DEFAULT            (     0)
#define GNSS_NV_EFS_TLE_XTRAT_DATAXFR_NUM_MAX_ELAPSED_RETRY_DEFAULT        (     0)
#define GNSS_NV_EFS_TLE_XTRAT_LAST_DOWNLOAD_GPS_WEEK_DEFAULT               (     0)
#define GNSS_NV_EFS_TLE_XTRAT_LAST_DOWNLOAD_GPS_SEC_DEFAULT                (     0)
#define GNSS_NV_EFS_TLE_XTRAT_TIME_INJECT_TUNC_DEFAULT                     ( 10000) /* 10000 [msec] */   
#define GNSS_NV_EFS_TLE_XTRAT_MIN_TIME_SERVER_ACCESS_DEFAULT               (   180) /* [Sec] */
#define GNSS_NV_EFS_TLE_XTRAT_UPLOAD_SESS_CONTROL_DEFAULT                  (     0) /* Bit 0 - 0:Disabled, 1:Enabled */ 
                                                                                    /* Bit 1 (screening) - 0:Disabled, 1:Enabled */
#define GNSS_NV_EFS_TLE_XTRAT_FEATURE_CONTROL_DEFAULT                      (     1) /* 0:Disabled, 1: Basic Enabled */ 
#define GNSS_NV_EFS_TLE_XTRAT_SIMULTANEOUS_XTRA_XTRAT_CONTROL_DEFAULT      (     0) /* 0:Disabled, 1:Enabled */ 
#define GNSS_NV_EFS_TLE_XTRAT_CONN_FAILURE_RETRY_PERIOD_DEFAULT            ( 30000) /* [msec] */

#define GNSS_NV_EFS_TLE_TLM_DATABASE_AGE_THRESHOLD_DEFAULT                 (  8760) /* 8760 Hours = 1 Year */
#define GNSS_NV_EFS_TLE_TLM_DATABASE_AGE_THRESHOLD_UNIT_DEFAULT            (     1) /* 1:Hour, 2:Minutes */  
#define GNSS_NV_EFS_TLE_TLM_CELLID_L2_DFT_RADIUS_DEFAULT                   (500000) /* meters */
#define GNSS_NV_EFS_TLE_TLM_CELLID_L3_DFT_RADIUS_DEFAULT                   (200000) /* meters */
#define GNSS_NV_EFS_TLE_TLM_CELLID_L4_DFT_RADIUS_DEFAULT                   ( 10000) /* meters */
#define GNSS_NV_EFS_TLE_XTRAT_PRIMARY_REQUEST_URI_DEFAULT                  "/uds/v1"    /* Uniform Resource Identifier */
#define GNSS_NV_EFS_TLE_XTRAT_SECONDARY_REQUEST_URI_DEFAULT                "/uds/v1"    /* Uniform Resource Identifier */
#define GNSS_NV_EFS_TLE_XTRAT_USE_TRANSPORT_SECURITY_DEFAULT               (     1) /* 0 - HTTP   1 - HTTPS        */
#define GNSS_NV_EFS_TLE_XTRAT_KEEP_WARM_TIMEOUT_DEFAULT                    (7200000) /* 2 hours = 7200000 [msec]   */
#define GNSS_NV_EFS_TLE_TLM_ENABLE_DB_WRITE_TO_STORAGE_DEFAULT             (     0) /* 0:Disabled, 1:Enabled */
#define GNSS_NV_EFS_TLE_TLM_TEST_CONTROL_DEFAULT                           (TLE_TEST_CONTROL_GEN_IPC_MASK)
#define GNSS_NV_EFS_TLE_TLM_UPLOAD_CHECK_TIMER_PERIOD_DEFAULT_SEC          (  7200) /* [sec] */
#define GNSS_NV_EFS_TLE_TLM_UPLOAD_THRESHOLD_DEFAULT_CELLCOUNT             (    20)
#define GNSS_NV_EFS_TLE_TLM_UPLOAD_FORCE_TIMER_PERIOD_DEFAULT_SEC          ( 28800) /* 28800 Seconds = 8 hours*/
#define GNSS_NV_EFS_TLE_TLM_TDP_MEM_CHECK_THRESHOLD_DEFAULT                (     1) /* 1 meas block */
#define GNSS_NV_EFS_TLE_TLM_OPERATIONAL_USER_CELL_HISTORY_SIZE_DEFAULT     (   650)
/*===========================================================================
=============================================================================

   TLE data structures and declarations
   
=============================================================================
===========================================================================*/
typedef uint32 tle_CellIdRadiusType[TLE_NUM_NV_RADII];

typedef struct
{
  const cgps_nv_efs_reg_item_struct * cpz_Table;
  cgps_nv_efs_reg_item_type e_MinItem;
  cgps_nv_efs_reg_item_type e_MaxItem;
} tle_NvEfsItemTableType;
	
/*==============================================================================

       Functions

==============================================================================*/

/*==============================================================================
  FUNCTION
    tle_nv_efs_reg_get_default

  DESCRIPTION
    Retrieve the specified item and store it in 'dest'. Note that the size of
    'dest' is implicit via the item type. 
 
  RETURN VALUE
    TRUE : If Successful
    FALSE: Otherwise
 
DEPENDENCIES
  
==============================================================================*/
boolean tle_nv_efs_reg_get_default(cgps_nv_efs_reg_item_type const item, uint8 data_size,
                                    void *data_ptr) ;

/*===========================================================================

FUNCTION tle_nv_efs_init

DESCRIPTION
  This function is used to initialize internal TLE-NV data efs structure.
  
RETURN VALUE
  None
  
DEPENDENCIES
 
===========================================================================*/
void tle_nv_efs_init(const tle_NvEfsItemTableType * cpz_NvEfs,
                     const cgps_nv_efs_subsys_type e_SubSystem);

/*===========================================================================

FUNCTION tle_nv_efs_reg_item_read

DESCRIPTION
  This function is used to read NV item. If the read fails, it sets it to
  default value .
  
RETURN VALUE
  TRUE : If read operation is successful
  FALSE: Otherwise
  
DEPENDENCIES
 
===========================================================================*/
boolean tle_nv_efs_reg_item_read(const tle_NvEfsItemTableType * cpz_NvEfs,
                                 cgps_nv_efs_reg_item_type const item, 
                                 uint8 data_size, void *data_ptr);

/*===========================================================================

FUNCTION tle_nv_efs_reg_item_write

DESCRIPTION
  This function is used to write an NV item. 

  
RETURN VALUE
  TRUE : If write operation is successful
  FALSE: Otherwise

  
DEPENDENCIES
 
===========================================================================*/
boolean tle_nv_efs_reg_item_write(const tle_NvEfsItemTableType * cpz_NvEfs,
                                  cgps_nv_efs_reg_item_type const item, 
                                  uint8 data_size, const void *data_ptr);



#ifdef __cplusplus
}
#endif

#endif /* TLE_EFS_H */

