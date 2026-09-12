#ifndef _GTS_TTR_H_
#define _GTS_TTR_H_

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                     Global Time Services TTR Module

GENERAL DESCRIPTION
This file is the main file of Global Time Service module

Copyright (c) 2014,2015 by Qualcomm Technologies INCORPORATED. All Rights Reserved.
Copyright (c) 2015 - 2016 Qualcomm Technologies, Inc. 
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

Version Control

$Header: //components/rel/gnss8.mpss/11.1/gnss/wwanme/gts/inc/gts_ttr.h#2 $
$DateTime: 2020/07/21 22:54:52 $
$Author: pwbldsvc $

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*--------------------------------------------------------------------------
 * Include Files
 *-----------------------------------------------------------------------*/
#include "cgps_api.h"
#include "gnss_common.h"

/*--------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -----------------------------------------------------------------------*/

/* Conversion factor for CDMA RTC phase (2^32) */
#define GTS_CDMA_RTC_PHASE_CONV  4294967296LL

/* CDMA RTC rollover value in Cx8. CDMA RTC rolls over every 10.24secs or
 * 10.24 * 1.2288e6 * 8 = 100663296 (0x6000000) */
#define GTS_CDMA_RTC_ROLLOVER_CX8   100663296

/* Num of CDMA RTC Cx8 in 1ms - 8*1228.8 */
#define GTS_CDMA_RTC_CX8_PER_MS  (9830.4)

/* Max tolerable diff between CDMA RTC and USTMR, in ms */
#define GTS_MAX_CDMA_RTC_USTMR_DIFF   (0.1)

/* Macro to extract lower 24bits of USTMR value */
#define GTS_USTMR_LOWER_24BIT_MASK           (0xFFFFFF)

/* Max. num of LTE subscriptions. Same as 'LTE_MEM_MAX_INST' defined in
 * 'modem_proc\lte\cust\inc\custlte.h' */
#define GTS_MAX_LTE_SUBSCRIPTIONS   2

/*--------------------------------------------------------------------------
 * Type Declarations
 *-----------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * Function Prototype Declarations
 *-----------------------------------------------------------------------*/

/*
 ******************************************************************************
 *
 * Function
 *
 *   gts_CdmaTtrReqTime
 *
 * Description:
 *
 *  This function is called to request Time Transfer from M1X SRCH
 *  If the previous Time Transefe request is still pending and is the same
 *  TTr type as this request, this function will return immediately. Otherwise,
 *  it will send Request CDMA Time to M1X SRCH module.
 *
 * Parameters:
 *
 *  Requested LTE Time Transfer parameters
 *
 * Return value:
 *
 *  None
 *
  ******************************************************************************
*/
boolean gts_CdmaTtrReqTime( gts_CdmaTtrReqType * p_ReqParams );

/*
 ******************************************************************************
 *
 * Function:
 *
 *   gts_CdmaTtrProcWaitTimerExpired
 *
 * Description:
 *
 *
 * Parameters:
 *
 *
 * Return value:
 *
 *  None
 *
  ******************************************************************************
*/
void gts_CdmaTtrProcWaitTimerExpired( void );

/*
 ******************************************************************************
 * Function:
 *
 *   gts_CdmaTtrProcStartFTCal
 *
 * Description:
 *
 *  This functions restarts collecting Fast TCal Time Transfer information.
 *
 * Parameters:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_CdmaTtrProcStartFTCal( void );

/*
 ******************************************************************************
 * Function:
 *
 *   gts_CdmaTtrStopFTCal
 *
 * Description:
 *
 *  This functions stop collecting Fast TCal Time Transfer information.
 *
 * Parameters:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_CdmaTtrProcStopFTCal( void );

/*
 ******************************************************************************
 * Function:
 *
 *   gts_CdmaTtrReqFTCal
 *
 * Description:
 *
 *  This functions Requests Fast TCal Time Transfer information from 1x
 *
 * Parameters:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_CdmaTtrReqFTCal( void );

/*
 ******************************************************************************
 *
 * Function:
 *
 *   gts_CdmaTtrProcTime
 *
 * Description:
 *
 *  This function processes the Time-Transfer data. It computes GPS TimeEstimate
 *  in terms of FCount, GPS_Week and GPS_MsecofWeek and sends the estimate to MC.
 *
 * Parameters:
 *
 *  1x Time Transfer Data structure.
 *
 * Return value:
 *
 *  None
 *
  ******************************************************************************
*/
void gts_CdmaTtrProcTime( const gts_CdmaTimeInfoType *p_CdmaTtrData );

/*
 ******************************************************************************
 *
 * Function:
 *
 *   gts_LteTtrProcRfDevCmd
 *
 * Description:
 *
 *  This function will set/reset the flag for enabling/disabling LTE RF dev test.
 *  Also increase the number of TT samples required to 8.
 *
 * Parameters:
 *
 *  Sub command
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LteTtrProcRfDevCmd( uint8 b_SubCmd );

/*
 ******************************************************************************
 *
 * Function
 *
 *   gts_LteTtrProcAbortTimer
 *
 * Description:
 *
 *  This function is called when LTE Time Transfer Abort timer expires.
 *  This would happen if LTE module does not acknowledge in 30seconds that it
 *  received and processed Request Time Transfer Abort message.
 *
 * Parameters:
 *
 *  LTE Time Transfer Request ID of the Request that is aborted.
 *
 * Return value:
 *
 *  None
 *
  ******************************************************************************
*/
void gts_LteTtrProcAbortTimer( uint32 q_AbortedReqID );

/*
 ******************************************************************************
 *
 * Function
 *
 *   gts_LteTtrReqTime
 *
 * Description:
 *
 *  This function is called to request Time Transfer from LTE module.
 *  If the previous Time Transefe request is still pending and is the same
 *  TTr type as this request, this function will return immediately. Otherwise,
 *  it will send Request LTE Time Transfer message to LTE ML1 module.
 *  This function can be called indirectly by MC or directly by PGI tasks.
 *
 * Parameters:
 *
 *  Requested LTE Time Transfer parameters
 *
 * Return value:
 *
 *  TRUE if successful; FALSE otherwise
 *
  ******************************************************************************
*/
boolean gts_LteTtrReqTime( gts_LteTtrReqType * p_ReqParams );

/*
 ******************************************************************************
 *
 * Function:
 *
 *   gts_LteTtrProcTime
 *
 * Description:
 *
 *  This function is called when LTE module sends Time Transfer Report packet.
 *
 * Parameters:
 *
 *  LTE Time Transfer Report structure.
 *
 * Return value:
 *
 *  None
 *
  ******************************************************************************
*/
void gts_LteTtrProcTime( const gts_LteTimeInfoType * p_TTRpt );

/*
 ******************************************************************************
 *
 * Function:
 *
 *   gts_LteTtrProcWaitTimerExpired
 *
 * Description:
 *   
 *   This function processes the timer expiry message for LTE TTR
 *
 * Parameters:
 *
 *  None
 
 * Return value:
 *
 *  None
 *
  ******************************************************************************
*/
void gts_LteTtrProcWaitTimerExpired( void );

/*
 ******************************************************************************
 *
 * Function
 *
 *   gts_LteTtrProcAbortReq
 *
 * Description:
 *
 *  This function aborts LTE Time Transfer Request if necessary and starts
 *  sanity timer.
 *
 * Parameters:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
  ******************************************************************************
*/
void gts_LteTtrProcAbortReq( void );

/*
 ******************************************************************************
 * gts_InitTtr
 *
 * Function description:
 *
 *  This function initializes the variables for GTS Time transfer operation
 *
 * Parameters:
 *  None
 *
 * Return value:
 *  None
 *
 ******************************************************************************
*/
void gts_InitTtr( void );

#endif //#ifndef _GTS_TTR_H_