/*! @brief!
  @file

  @brief
  WCDMA FW AGC interface definitions header file
*/

/*! @brief===========================================================================

  Copyright (c) 2007-2013 Qualcomm Technologies Incorporated. All Rights Reserved

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

/*! @brief===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //source/qcom/qct/modem/fw/components/fw_wcdma.mpss/rel/17.0/api/wfw_rxagc_intf.h#1 $ $DateTime: 2019/06/16 13:38:34 $ $Author: deepemm $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
04/11/11   WX      Add DVGA gain offset async parameter
04/07/11   WX      RxAGC async write/read interface update for NikeL
06/04/10   WB      Moved RxAGC cmd word constant defintions from wfw_rxagc.h
05/27/10   WB      Add rxAgcOn flag to interface 
10/20/09   NC      Remove extra LNA SBI interface parameter
09/01/09   EM      New interface change to support 6 LNA gain states
03/30/09   EM      Split SWI to LNA processing and dual carrier process
===========================================================================*/

#ifndef WFW_AGC_INTF_H
#define WFW_AGC_INTF_H

/*! @brief===========================================================================

                           INCLUDE FILES

===========================================================================*/

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/* Public constants and enums */
/* -------------------------- */
#define WFW_RXAGC_MAX_ANTENNAS 2
#define WFW_RXAGC_NUM_BAND     2
#define WFW_RXAGC_MAX_LNAS_PER_ANTENNA 2
#define WFW_RXAGC_MAX_CARRIERS 4
#define WFW_RXAGC_MAX_CARRIERS_PER_SUB 2

/* Structure definitions */
/* --------------------- */

/*! @name Structure definition for AGC SW-FW interface */
/*!@{*/
/*! @brief WCDMA FW AGC information update */

/*! @brief RxAGC Async Read/Write variables*/
typedef struct
{

  /*! @brief Specifies control flag for enabling the jammer detection */
  UWord8 rxJammerDetectEnable;

  /*! @brief Specifies the accumulated log energy of reference
     signal during steady state.*/
  UWord16 rxLogReference;

  /*! @brief Specifies the scale factor for AGC Adjust Value in
     steady state. */
  UWord16 rxGainConstant;

  /*! @brief Initial RSSI value for the LNA state 0-5 */
  Word16 rxAgcValueCmInit[6];

  /*! @brief Specifies the accumulated log energy of reference signal
     during compress mode. */
  Word16 rxCmLogReference;

  /*! @brief RxAGC DVGA gain offset setting */
  Word16 rxDvgaOffset;

}WfwRxAgcAsyncWrStruct;

typedef struct
{
  /*! NBR tunable parameters */
  /*! rotator frequency (in KHz) to be programmed to RXFE_NB_CSR_FREQ_OFFSET_NB0; mainly used by RFSW */
  UWord32 rotatorFreq;           
  /*! num of bpgs to accumulate for NBEE */
  UWord8  accumBpg;              
  /*! threshold1 - for checking against Gmax_Gmin_Power_Diff which is log2(GmaxPwr/GminPwr) */
  Word8   threshold1;            
  /*! threshold2 - for checking against W_G_Power_Diff which is log2(WPwr/GmaxPwr) */
  Word8   threshold2;            
} WfwNbrAsyncWrStruct;

typedef struct
{
  /*! @brief This variable indicates if RxAGC is turned ON (1) for 
      this chain or turned OFF (0) */
  UWord8 rxAgcOn;

  /*! @brief This variable indicates if RxAGC is enabled (1) for 
      this chain or disabled (0) */
  UWord8 rxAgcEnable;

  /*! @brief Specifies internal accumulation of AGC Adjust Value.
     RSSI = rxAgcValue .12 + constant. */
  Word16 rxAgcValue;

  /*! @brief Accumulated received signal. This quantity models the
     received power at the antenna. It is the value which is
     sent back to the adder in the feedback loop */
  Word16 rxAccumOut;

  /*! @brief This variable contains value of the Rx AGC RSSI in
     db10 units, in a compressed mode gap. */
  Word16 cmSrchRxAgcVal;

  /*!@}*/
}WfwRxAgcAsyncRdStruct;

typedef enum
{
  WFW_NBR_RESULT_WCDMA_SIGNAL_DETECTED = 0,
  WFW_NBR_RESULT_GSM_SIGNAL_DETECTED = 1,
  WFW_NBR_RESULT_MAX
} WfwNbrResultType;

#define WFW_NBR_NUM_GSM_NB     3

/*! max bpg count for NBR accummulation */
#define WFW_NBR_LOG_2_MAX_ACCUM_BPGS        5
#define WFW_NBR_MAX_ACCUM_BPGS              (1 << WFW_NBR_LOG_2_MAX_ACCUM_BPGS)

typedef struct
{
  /*! NBR result */
  WfwNbrResultType nbrResult;                 
  /*! accummulated nbr power */
  UWord32 i2q2NbrAccPwr[WFW_NBR_NUM_GSM_NB];    
  /*! per bpg nbr power */
  UWord32 i2q2NbrPwrs[WFW_NBR_MAX_ACCUM_BPGS][WFW_NBR_NUM_GSM_NB];
  /*! accummulated prim NB power */
  UWord32 i2q2PrimNbAccPwr;                     
  /*! per bpg nbr power */
  UWord32 i2q2PrimNbPwrs[WFW_NBR_MAX_ACCUM_BPGS];                     
  /*! P_Gsm min */
  UWord32 minG;
  /*! P_Gsm max */
  UWord32 maxG;
  /*! P_Gsm_diff */
  Word32  pwrGsmDiff;                           
  /*! P_W_G_diff */
  Word32  pwrWGDiff;                            
  /*! rxagc power in dB10 */
  Word32  rxAgcPwrdB10;                         
  /*! latch of rxagc->rxAccumOutEval */ 
  Word32  rxAccumOutEval;                       

} WfwNbrAsyncRdStruct;

typedef struct
{
  /*! @brief WB power estimator result */
  UWord32 wbPwrAccumResult;

  /*! @brief LNA state for the WB pwr result */
  UWord16 wbPwrLnaDecision;
}WfwRxAgcWbPwrRdStruct;

typedef struct
{

  /*! @brief Its a read only variable which gives the values of LNA
     state. */
  UWord16 rxLnaDecision;

  /*! @brief This variable will log the LNA state in Compressed 
      mode or Interrat GAP */
  UWord16 cmRxLnaDecision;

  /*! @brief WB power estimator report to L1 */
  WfwRxAgcWbPwrRdStruct wbPwr;

  /*!@}*/
}WfwRxAgcLnaAsyncRdStruct;
 
/*! @brief RxAGC Command Processing Bmsk Struct */
typedef union
{
  struct
  {
    /*! @brief Whether AGC cmd is pending on Antenna 0 Carrier 0 */
    UWord32 a0c0 : 1;  /* Bits 0 */
    /*! @brief Whether AGC cmd is pending on Antenna 1 Carrier 0 */
    UWord32 a1c0 : 1;  /* Bits 1 */
    /*! @brief Whether AGC cmd is pending on Antenna 0 Carrier 1 */
    UWord32 a0c1 : 1;  /* Bits 2 */
    /*! @brief Whether AGC cmd is pending on Antenna 1 Carrier 1 */
    UWord32 a1c1 : 1;  /* Bits 3 */
    /*! @brief Whether AGC cmd is pending on Antenna 0 Carrier 2 */
    UWord32 a0c2 : 1;  /* Bits 4 */
    /*! @brief Whether AGC cmd is pending on Antenna 1 Carrier 2 */
    UWord32 a1c2 : 1;  /* Bits 5 */
    /*! @brief Whether AGC cmd is pending on Antenna 0 Carrier 3 */
    UWord32 a0c3 : 1;  /* Bits 6 */
    /*! @brief Whether AGC cmd is pending on Antenna 1 Carrier 3 */
    UWord32 a1c3 : 1;  /* Bits 7 */
    UWord32      : 24; /* Unused Bits  */
  };
  /*! @brief RxAGC Command Processing Bmsk Word */
  UWord32 word;
} WfwRxAgcCmdProcBmskStruct;
#define WFW_RXAGC_CMDPENDING_NUM_BITS_PER_SUB 4

/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

#endif /* WFW_AGC_INTF_H */

