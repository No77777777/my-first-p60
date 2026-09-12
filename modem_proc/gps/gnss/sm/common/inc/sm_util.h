#ifndef SM_UTIL_H
#define SM_UTIL_H
/*----------------------------------------------------------------------------
 *  Copyright (c) 2014 Qualcomm Atheros, Inc..
 *  All Rights Reserved.
 *  Qualcomm Atheros Confidential and Proprietary.
 *----------------------------------------------------------------------------
 */
/*============================================================================
  FILE:         sm_util.h

  OVERVIEW:     


  DEPENDENCIES: If the code in this file has any notable dependencies,
                describe them here.  Any initialization and sequencing
                requirements, or assumptions about the overall state of
                the system belong here.
 
  Copyright (c) 2009 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2015 - 2019 Qualcomm Technologies, Inc. 
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
============================================================================*/

/*============================================================================
  EDIT HISTORY FOR MODULE

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/common/inc/sm_util.h#1 $
  $DateTime: 2019/10/24 01:30:36 $
  $Author: pwbldsvc $

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------

============================================================================*/

/*------------------------------------------------------------------------------
File comment
------------------------------------------------------------------------------*/

/**
* @file sm_util.h
    
* file containing useful functions for SM module, e.g.
* translating between Gen7 and Gen8 structs
 
*/
#include "gps_variation.h"
#include <gnss_common.h>
#include "mgp_api.h"
#include "sm_api.h"

/* converts the given argument to boolean */
#define SM_UTIL_MAKE_BOOL(arg) (!!(arg))

/**
* @brief  Retrieves a the supported constellation information
* 
* @return  bitmask of supported GNSS constellations
* @side effects None
* 
*/
uint32 sm_get_supported_constellation_mask(void);

/**
* @brief  converts GNSS time to gps time
* 
* @return  boolean. false on failure
* @side effects None
* @see
* 
*/
boolean 
sm_translate_gnss_time_to_gps_time(
   sm_ReportGpsTimeStructType *p_Dest, 
   const sm_ReportGnssTimeStructType *p_Src);

/**
* @brief  converts GNSS assistance data to internal assistance
*         data type. this function is used to help bring about
*         changes in internal code in phases.
* 
* @return  boolean. false on failure
* @sideeffects None
* @see
* 
*/
boolean 
sm_translate_gnss_assist_data_to_internal_assist_data( sm_InternalAssistDataStatusStructType* p_Dest,
                                                       const sm_GnssAssistDataStatusStructType *p_Src);

boolean
sm_translate_sm_fix_report_to_gnss_nav_solution(
   gnss_NavSolutionStructType* p_dest, 
   const sm_FixInfoStructType* p_src,
   boolean b_FinalFix,
   boolean b_backgroundFix);


/**
 * Translates sm's new SVInfo structure into the old array which
 * is defined in gnss nav solution structure so 
 * that it may be used to generate old-style log 
 * @param p_destSVInfo
 * @param p_srcSVInfo 
 * 
 * @return uint32  number of SVs populated in destSVInfo array
 */
uint32
sm_generate_sv_array_from_svinfo_struct(
   uint16 *p_destSV,   
   uint32 q_destMaxNum,
   const sm_GnssSvInfoType *p_srcSVInfo,
   uint32 q_srcMaxNum);

/**
 * Translates sm's new SVInfo structure into the old 8-bit array which
 * is defined in gnss nav solution structure so 
 * that it may be used to generate old-style log 
 * @param p_destSVInfo
 * @param p_srcSVInfo 
 * @param q_validBitmask  all bits must be present (if more than 
 *                        one bit)
 * 
 * @return uint32  number of SVs populated in destSVInfo array
 */
uint32
sm_generate_sv_array8_from_svinfo_struct(
   uint8 *p_destSV,
   uint32 q_destMaxNum,
   const sm_GnssSvInfoType *p_srcSVInfo,
   uint32 q_srcMaxNum,
   uint32 q_validBitmask);

/**
* Translates MeasBlk Src type to Master COnstellation
* One constellation can have multiple MeasBlk Src depending
* on the Band Type. This function will return the primary constellation
*
* @param[in] gnss_MeasBlkSrcEnumType
*
* @return gnss_MeasBlkSrcEnumType  MeaskBlk Src for Primary Constellation
*/
gnss_MeasBlkSrcEnumType sm_util_get_master_constellation(gnss_MeasBlkSrcEnumType e_MeasBlkSrc);

/**
* Queries ME for Multi-band enablement status
*
* @param None
*
* @return TRUE if Multiband is enabled
*/
boolean sm_is_multiband_supported();

#ifdef FEATURE_XTRA3_DYN_URL_SELECTION
/**
* Queries ME for Multi-band constellation config
*
* @param None
*
* @return Multi-band config 
*/
uint32 sm_util_get_multiband_config();

#endif

/**
* Wrapper function to do a memcpy of GNSS measurement report.
* Measurement report is dynamically sized based on the number of SVs,
* so care has to be taken when copying the measurement report so as to
* not read memory beyond what has been allocated
*
* @param[out] pointer to destination memory
* @param[in]  max number of bytes to copy to the destination memory
* @param[in]  pointer to measurement report to be copied
*
* @return None
*/
void sm_GnssMeas_memscpy(void* p_Dest, size_t t_DestSize, const sm_GpsMeasRptStructType* pz_SrcMeasReport);

#endif /* SM_UTIL_H */
