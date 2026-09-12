#ifndef LOC_LTE_OTDOA_GNSS_API_H
#define LOC_LTE_OTDOA_GNSS_API_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  Location LTE OTDOA - GNSS API Header File

GENERAL DESCRIPTION
This file contains API definitions between Location LTE OTDOA module and
other GNSS modules.

===========================================================================
 
  Copyright (c) 2011 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2015 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2015 - 2017 Qualcomm Technologies, Inc. 
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
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

===========================================================================

Version Control

$Header: //components/rel/gnss8.mpss/11.1/gnss/wwanme/lte_otdoa/5_0/inc/loc_lte_otdoa_gnss_api.h#1 $
$DateTime: 2019/10/24 01:30:36 $
$Author: pwbldsvc $

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*--------------------------------------------------------------------------
 * Include Files
 *-----------------------------------------------------------------------*/

#include "comdef.h"
#include "sm_loc_lte_otdoa.h"

/*--------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -----------------------------------------------------------------------*/


/*--------------------------------------------------------------------------
 * Type Declarations
 *-----------------------------------------------------------------------*/
/* Different session types. Enum value defined by SM layer */
#define LOC_LTE_OTDOA_UP_SESSION 8 /* user plane */
#define LOC_LTE_OTDOA_CP_SESSION 0x200  /* control plane */

/* Different operation modes. Enum value defined by SM layer */
#define LOC_LTE_OTDOA_OP_MODE_OTDOA_GPS_HYBRID 2 /* hybrid mode */
#define LOC_LTE_OTDOA_OP_MODE_OTDOA_ONLY_UP 7        /* OTDOA only */
#define LOC_LTE_OTDOA_OP_MODE_OTDOA_ONLY_CP 16   /* OTDOA only for CP */

typedef struct
{
  uint8 u_Reason;
  uint16 e_SessType;
  /* LTE Subscription ID as defined in structure 'sys_modem_as_id_e_type' in
   * file 'modem_proc\mmcp\api\public\sys.h' */
  int8 b_SubId;
}
loc_lte_otdoa_StopSessCmdType;

typedef struct
{
  /* LTE Subscription ID as defined in structure 'sys_modem_as_id_e_type' in
   * file 'modem_proc\mmcp\api\public\sys.h' */
  int8 b_SubId;
}
loc_lte_otdoa_OtaResetCmdType;

typedef struct
{
  gnss_ClockStructType *p_Clock;
  uint16 e_SessType;
  boolean b_IsEarlyRptReq;
}
loc_lte_otdoa_ReqMeasRptCmdType;


/* */
#define LOC_LTE_OTDOA_MAX_DIAG_MSG   4000
#define LOC_LTE_OTDOA_MAX_DIAG_MSG_DATA_LENGTH   2000

#ifdef T_WINNT
#error code not present
#endif /* T_WINNT */

typedef PACKED struct PACKED_POST
{
  /* Command code */
  uint8   u_CmdCode;

  /* Subcommand code (overload of u_SubsysId) */
  uint8   u_GpsControl;

  /* Length of data payload (overload of w_SubsysCmdCode) */
  uint16  w_DataLength;

  /* CGPS Command Code (overload of u_CgpsCmdCode) */
  uint8   u_GpsSubcmd;

  /* Request Message Version Number */
  uint8   u_Version;
  int32   l_ClientId;

  /* Complete data set length that needs to be transfered in all packets
   * (irrelevant if q_TotalPacketsNumber == 1) */
  uint32  q_TotalDataLength;

  /* Current packet number (irrelevant if q_TotalPacketsNumber == 1);
   * first packet number == 0 */
  uint32  q_PacketNumber;

  /* Total numer of packtes needed to transfer the complete data set */
  uint32  q_TotalNumberOfPackets;

  /* Data length of the u_PacketData field */
  uint32  q_PacketDataLength;

  uint32  q_PacketData[ LOC_LTE_OTDOA_MAX_DIAG_MSG_DATA_LENGTH / 4 ];
}
loc_lte_otdoa_DiagCmdType;

typedef PACKED struct PACKED_POST
{
  /* Command code */
  uint8   u_CmdCode;

  /* Subcommand code (overload of u_SubsysId) */
  uint8   u_GpsControl;

  /* Length of data payload (overload of w_SubsysCmdCode) */
  uint16  w_DataLength;

  /* CGPS Command Code (overload of u_CgpsCmdCode) */
  uint8   u_GpsSubcmd;

  /* Command that has to be injected */
  uint8   u_InjectCmdCode;

  /* Boolean to indicate to use Internal Data or not */
  uint8   b_ExternalData;

  uint8   q_Data[LOC_LTE_OTDOA_MAX_DIAG_MSG_DATA_LENGTH / 4 ];
}
loc_lte_otdoa_DiagCmdSmTestType;

#ifdef T_WINNT
#error code not present
#endif /* T_WINNT */

typedef struct
{
  sm_OTDOA_AssistanceData *p_OtdoaAssistanceData;
  uint8 e_OpMode;
  uint16 e_SessType;

  /* LTE Subscription ID as defined in structure 'sys_modem_as_id_e_type' in
   * file 'modem_proc\mmcp\api\public\sys.h' */
  int8 b_SubId;
}
loc_lte_otdoa_SmStartCmdInfoType;

typedef struct
{
  /* Timeout in milli seconds */
  uint32 q_Timeout;
  uint16 e_SessType;
}
loc_lte_otdoa_UpdateTimoutInfoType;

typedef struct
{
  sm_OTDOA_FTA_Information *p_FtaAidingData;
  uint16 e_SessType;
}
loc_lte_otdoa_FtaAidingDataInfoType;

/* Command to indicate whether the feature is enabled
   in SM NV #73710 or not */
typedef struct
{
  uint8 b_Enable;
}
loc_lte_otdoa_FeatureStatusType;


/*--------------------------------------------------------------------------
 * Function Declarations
 *-----------------------------------------------------------------------*/

/*
 ******************************************************************************
 * Function: loc_lte_otdoa_StartSessCmd
 *
 * Description:
 *  This function will start the OTDOA meas. process. It will be sent by SM
 *
 * Parameters:
 *  Assitance data, operation mode and session type.
 *
 * Dependencies:
 *  None.
 *
 * Return value:
 *  None.
 *
 ******************************************************************************
 */
extern void loc_lte_otdoa_StartSessCmd
(
  loc_lte_otdoa_SmStartCmdInfoType *p_SmStartCmdInfo
);

/*
 ******************************************************************************
 * Function: loc_lte_otdoa_StopSessCmd
 *
 * Description:
 *  This function will stop the OTDOA meas. process. It will be sent by SM
 *
 * Parameters:
 *  Reason for stop command.
 *
 * Dependencies:
 *  None.
 *
 * Return value:
 *  None.
 *
 ******************************************************************************
 */
extern void loc_lte_otdoa_StopSessCmd
(
  loc_lte_otdoa_StopSessCmdType *p_StopSessCmd
);

/*
 ******************************************************************************
 * Function: loc_lte_otdoa_ReqMeasRptCmd
 *
 * Description:
 *  This function is used by SM to request OTDOA measurements from GPS SW
 *
 * Parameters:
 *  Meas. type
 *
 * Dependencies:
 *  None.
 *
 * Return value:
 *  None.
 *
 ******************************************************************************
 */
extern void loc_lte_otdoa_ReqMeasRptCmd
(
  loc_lte_otdoa_ReqMeasRptCmdType * p_ReqMeasRptCmd
);

/*
 ******************************************************************************
 * Function: loc_lte_otdoa_ProcDiagCmd
 *
 * Description:
 *  This function is used by DIAG task to pass received Loc LTE OTDOA packet
 *
 * Parameters:
 *  DIAG packet
 *
 * Dependencies:
 *  None.
 *
 * Return value:
 *  None.
 *
 ******************************************************************************
 */
extern void loc_lte_otdoa_ProcDiagCmd
(
  uint8 * p_DiagPkt, uint16 w_PktLength
);

/*
 ******************************************************************************
 * Function: loc_lte_otdoa_UpdateTimeout
 *
 * Description:
 *  This function is used by SM to send the timeout info
 *
 * Parameters:
 *  Timeout value and session type
 *
 * Dependencies:
 *  None.
 *
 * Return value:
 *  None.
 *
 ******************************************************************************
 */
extern void loc_lte_otdoa_UpdateTimeout
(
  loc_lte_otdoa_UpdateTimoutInfoType *p_UpdateTimoutInfo
);

/*
 ******************************************************************************
 * Function: loc_lte_otdoa_UpdateFtaAidingData
 *
 * Description:
 *  This function is used by SM to send the fine time assistance info
 *
 * Parameters:
 *  Fine time assitance and session type
 *
 * Dependencies:
 *  None.
 *
 * Return value:
 *  None.
 *
 ******************************************************************************
 */
extern void loc_lte_otdoa_UpdateFtaAidingData
(
  loc_lte_otdoa_FtaAidingDataInfoType *p_FtaAidingDataInfo
);

/*
 ******************************************************************************
 * Function: loc_lte_otdoa_OtaReset
 *
 * Description:
 *  This function is used by SM to send the OTA reset to LOC_LTE_OTDOA
 *
 * Parameters:
 *  None
 *
 * Dependencies:
 *  None.
 *
 * Return value:
 *  None.
 *
 ******************************************************************************
 */
extern void loc_lte_otdoa_OtaReset
(
   loc_lte_otdoa_OtaResetCmdType *p_OtaResetInfo
);

/*
 ******************************************************************************
 * Function: lloc_StopTimer
 *
 * Description:
 *  Stop a timer
 *
 * Parameters:
 *  None
 *
 * Dependencies:
 *  None
 *
 * Return value:
 *  FALSE in case of any error
 *
 ******************************************************************************
 */
boolean lloc_StopTimer( uint32 q_TimerId );

#endif /* LOC_LTE_OTDOA_GNSS_API_H */
