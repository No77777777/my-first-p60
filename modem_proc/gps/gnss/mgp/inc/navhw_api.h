/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                          MGP API Header File

GENERAL DESCRIPTION
  This file contains contains the structure definitions and function prototypes for 
  the MGPSS Core software interface.

 
EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS
  

  Copyright (c) 2005 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2015 - 2017 Qualcomm Technologies, Inc. 
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.


Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

Version Control

  $Header: //components/rel/gnss8.mpss/11.1/gnss/mgp/inc/navhw_api.h#1 $ $DateTime: 2019/10/24 01:30:36 $ $Author: pwbldsvc $ 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/29/10   pn      Updated DmLog structs with PACKED_POST.
09/16/09   jd      Added support for Phase II RF HW Status logging.
08/05/09   mwb     Added support for MobiSens high-power mode
07/30/09   gk      Added support for On-demand positioning 2.0.
06/05/09   jlp     Added support for injecting Sparse network Region(s)
01/21/09   kinw    Added support for XTRA-T upload.
11/06/08   jlp     Added support for getting/setting the gps restriction level.
07/17/09   jd      Added mgp_SendDiagRfCmd() for HW Config cmd over QXDM
07/17/09   jd      Added mgp_RfBPStartInit() and mgp_RfBPStopInit() for 
                   RF Boot up calibration.
10/27/08   jlp     Added interface to set Sv's that we should not search for
                   or track.
07/25/08   gk      Force receiver off changes
07/02/08   ank     Added LBS support for CDG OMH R-UIM Spec
06/12/08    lt     Support for external coarse position injection.
04/25/08   gk      On-Demand feature changes
02/14/08    jw     Support for Optimistic Punc
12/18/07   lt      Added support for disabling time-alignment in periodic MSA.
07/31/07   br      Added support for 1x Allow GPS interface.
05/31/07   jw      Added Rcvr memconfig type (Hi or Lo Mem)
05/07/07   jw      Added Diag GPS Intelliciever support.
05/07/07   br      Added support for running PPM in MC task.
05/05/07   br      Added support for RF Info interface.
04/24/07    ld     Support for CELLDB (Interim Check-in)
04/23/07   jw      Flow the disable bit edge required before track flag to MC.
04/19/07    ld     Intermediate check-in for UMTS Time-tagging
04/17/07    ld     Modified MGP Tick Time to 1 second
02/20/07   br      Added support for TCXO Manager Rotator Push.
01/24/07    jw     Added new MGP Time Tick functionality
11/17/06   br      Added 1x SRCH interface.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

#include "gps_variation.h"
#ifdef WIN32
#pragma once
#endif


#ifndef NAVHW_API_H
#define NAVHW_API_H

/*
 * Include files
*/
#include "customer.h"
#include "comdef.h"
#include "gps_types.h"
#include "gnss_consts.h"
#include "gnss_common.h"
#include "aries_os_globals.h"
#include  "cgps_api.h"
#include "gnss_api.h"

#include "cm.h"

typedef struct 
{
  /* Diag Data (Used to input commands or requests from internal user) */
  uint8 u_DiagData[36]; 
} navhw_DiagCmdStruct;

/* There is a separate document which describes how these commands 
   can be used with QXDM */
typedef enum
{
  MGP_NAV_DIAG_GET_HW_VERSION_CMD = 0,
  MGP_NAV_DIAG_GET_PREAMP_CMD, /* 1 */
  MGP_NAV_DIAG_GET_PREMEAN_CMD,/* 2 */
  MGP_NAV_DIAG_GET_REGISTER_BLOCK_CMD,/* 3 */
  MGP_NAV_DIAG_GET_SINGLE_REG_VAL_CMD,/* 4 */
  MGP_NAV_DIAG_PUT_MEAS_START_STOP_CMD,/* 5 */
  MGP_NAV_DIAG_GET_GNSS_ADC_CONFIG_CMD,/* 6 */
  MGP_NAV_DIAG_PUT_MEAS_CONFIG_CMD,/* 7 */
  MGP_NAV_DIAG_PUT_SINGLE_REG_VAL_CMD,/* 8 */
  MGP_NAV_DIAG_GET_AMPL_CMD,/* 9 */
  MGP_NAV_DIAG_PUT_GNSS_ADC_CONFIG_CMD, /* 10 */
  MGP_NAV_DIAG_GET_BP_AMP, /* 11 */
  MGP_NAV_DIAG_PUT_SPAN_CFG_CMD /* 12 */
} navhw_DiagCmdEnum;

/*
 ******************************************************************************
 * Function navhw_ProcessDiagCommands
 *
 * Description:
 *
 *  This function is used to test GNSS RF APIs. 
 *
 * Parameters: 
 * 
 *  navhw_DiagCmdStruct - Struture that contains the data pertaining 
 *  to exercise RF APIs
 *
 * Dependencies:
 * 
 *  None
 *
 * Return value:
 * 
 *
 *
 ******************************************************************************
*/
void navhw_ProcessDiagCommands( navhw_DiagCmdStruct * p_NavDiagCmd );

#ifdef FEATURE_GNSS_PPS_PULSE_EXTERNAL

/*
 ******************************************************************************
 * Function: navhw_ResourcesForPps
 *
 * Description:
 *
 *  This function is used for enabling/disabling MCPM shared resources and NavBB
 *  for usage of PPS/DrSync HW
 *
 * Parameters: 
 * 
 *  b_Enable - boolean flag to request/release resources
 *
 * Dependencies:
 * 
 *  None
 *
 * Return value:
 *  TRUE - if request/release was successful,
 *  FALSE otherwise
 *
 ******************************************************************************
*/
boolean navhw_ResourcesForPps ( const boolean b_Enable );

/*
 ******************************************************************************
 * Function: navhw_IsNavHwEnabledForPps
 *
 * Description:
 *
 *  This function will return the current state of MCPM shared resources and
 *  NavBB (needed for using NAV PPS HW by external clients)
 *
 * Parameters: 
 * 
 *  None
 *
 * Dependencies:
 * 
 *  None
 *
 * Return value:
 *  TRUE - if NavBB HW is enabled for PPS use,
 *  FALSE otherwise
 *
 ******************************************************************************
*/
boolean navhw_IsNavHwEnabledForPps( void );

#endif //#ifdef FEATURE_GNSS_PPS_PULSE_EXTERNAL

#endif /* NAVHW_API_H */ 

