/*!
  @file
  ipa_hw_interface.h

  @brief
  Define the IPA HW to IPA driver common interface
 
  @note
  Struct packing is not in use. These structs are aligned along 4-byte 
  boundaries, other archs (ex: 8-byte aligned) must make necessary changes 
  for compatibility.

*/
/*===========================================================================

  Copyright (c) 2017-19 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

  $Header: //components/rel/dataipa.mpss/3.2/ipa_uc/6150/ipa/inc/common/ipa_hw_interface.h#1 $
==============================================================================*/

#ifndef IPA_HW_INTERFACE_H_
#define IPA_HW_INTERFACE_H_

#define IPA_HW_FIRST_ERROR_ADDRESS_RESET_VALUE  0x7EEDBEAF /**< Reset value for firstErrorAddress field in shared memory  */

#define IPA_HW_NUM_FEATURES                     0x8        /**< Maximum number of features that HW can support */  

#define IPA_HW_SRAM_CPU_COMM_SIZE               128         /**< 128B shared memory located in offset zero of SW Partition in IPA SRAM.*/
#define IPA_HW_SRAM_DEBUG_EVENT_ADDR            (HWIO_ADDRI(IPA_SRAM_DIRECT_ACCESS_n, 0) + IPA_HW_SRAM_CPU_COMM_SIZE) /**< Location of the structure where the debug info will be advertised */

//TBD
#define IPA_HW_COMMON_MAX_CMD_SIZE              4

/**
 * @brief   Mailbox register to Interrupt HWP for CPU Comm info 
 *          In terms of IPA registers, IPA_UC_MAILBOX_m_n,
 *          m=[0..3H], n=[0..31], this will correspond to m = 0,
 *          n = 23. The physical address of the mailbox
 *          registers will be IPA_WRAPPER_BASE_PHYS + 0x00072000
 *          + 4*(32*m + n) where m = 0 and n = 23
 *  
*/
#define IPA_CPU_2_HW_CMD_MBOX_m          0

#define IPA_CPU_2_HW_CMD_MBOX_n         23 //CPU updates the shared mem 
                                           //with the command info
                                           //and triggers this doorbell

/**
 * @brief   Values that represent the features supported in IPA HW
*/
typedef enum
{
   IPA_HW_FEATURE_COMMON                    = 0x0,    /**< Feature related to common operation of IPA HW */
   IPA_HW_FEATURE_POWER_COLLAPSE           = 0x2,    /**< Feature related to IPA Power collapse */
   IPA_HW_FEATURE_WDI                       = 0x3, /** */
   IPA_HW_FEATURE_ETH						= 0x5,	  /**< Feature related to ETH operation in IPA HW	  */   
   IPA_HW_FEATURE_OFFLOAD					= 0x6,	  /**< Feature related to Offload operation in IPA HW	  */      
   IPA_HW_FEATURE_UNIT_TEST                =  0x7,   /**< Feature related to Unit test framework in IPA HW */
   IPA_HW_FEATURE_MAX                       = IPA_HW_NUM_FEATURES     
}IPA_HW_FEATURES;
///Need to ensure that the IPA_HW_FEATURES is backward compatible. Basically, leave only common and power collapse 
//in the enum but reuse the same number from the older chipsets

/**
 * @brief   Bitmask representation for the features supported in
 *          IPA HW
*/
typedef enum
{
   IPA_HW_FEATURE_BMASK_COMMON              = 1 << IPA_HW_FEATURE_COMMON,            /**< Feature related to common operation of IPA HW  */
   IPA_HW_FEATURE_BMASK_POWER_COLLAPSE      = 1 << IPA_HW_FEATURE_POWER_COLLAPSE,    /**< Feature related to IPA Power Collapse  */ 
   IPA_HW_FEATURE_BMASK_WDI                 = 1 << IPA_HW_FEATURE_WDI,               /**< Feature related to WDI  */
   IPA_HW_FEATURE_BMASK_ETH                 = 1 << IPA_HW_FEATURE_ETH,               /**< Feature related to ETH  */
   IPA_HW_FEATURE_BMASK_OFFLOAD             = 1 << IPA_HW_FEATURE_OFFLOAD,           /**< Feature related to Offload  */
}IPA_HW_FEATURE_MASK;

/**
 *  @brief   Enum value determined based on the feature it
 *           corresponds to
 *  +----------------+----------------+
 *  |    3 bits      |     5 bits     |
 *  +----------------+----------------+
 *  |   HW_FEATURE   |     OPCODE     |
 *  +----------------+----------------+
 *  
 */
#define FEATURE_ENUM_VAL(feature,opcode) ((feature << 5) | opcode)

/**
 * @brief   Values that represent the common commands from CPU 
 *          to IPA HW.
*/
typedef enum
{
   IPA_CPU_2_HW_CMD_NO_OP                     = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 0),    /**< No operation is required.  */
   IPA_CPU_2_HW_CMD_UPDATE_FLAGS              = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 1),    /**< Update SW flags which defines the behavior of HW . Once operation was completed HW shall respond with IPA_HW_2_CPU_RESPONSE_CMD_COMPLETED */
   IPA_CPU_2_HW_CMD_DEBUG_RUN_TEST            = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 2),    /**< Launch predefined test over HW  */
   IPA_CPU_2_HW_CMD_DEBUG_GET_INFO            = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 3),    /**< Read HW internal debug information  */
   IPA_CPU_2_HW_CMD_ERR_FATAL                 = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 4),    /**< CPU instructs HW to perform error fatal handling  */
   IPA_CPU_2_HW_CMD_CLK_GATE                  = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 5),    /**< CPU instructs HW to goto Clock Gated state  */ 
   IPA_CPU_2_HW_CMD_CLK_UNGATE                = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 6),    /**< CPU instructs HW to goto Clock Ungated state  */ 
   IPA_CPU_2_HW_CMD_GSI_CH_EMPTY              = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 10),   /**< Command to check for GSI channel emptiness */
   IPA_CPU_2_HW_CMD_STORE_ADD                 = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 12),   /**< Command to store address shared by CPU */
}IPA_CPU_2_HW_COMMANDS;

/**
 * @brief   Values that represent common HW responses to CPU 
 *          commands.
*/
typedef enum
{
   IPA_HW_2_CPU_RESPONSE_NO_OP                    = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 0),
   IPA_HW_2_CPU_RESPONSE_INIT_COMPLETED           = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 1),  /**< HW shall send this command once boot sequence is completed and HW is ready to serve commands from CPU */
   IPA_HW_2_CPU_RESPONSE_CMD_COMPLETED            = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 2),  /**< Response to CPU commands */
   IPA_HW_2_CPU_RESPONSE_DEBUG_GET_INFO           = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 3),  /**< Response to IPA_CPU_2_HW_CMD_DEBUG_GET_INFO command */
}IPA_HW_2_CPU_RESPONSES;

/**
 * @brief   Values that represent common HW events to be sent to
 *          CPU.
*/
typedef enum
{
   IPA_HW_2_CPU_EVENT_NO_OP     = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 0),   /**< No event present  */
   IPA_HW_2_CPU_EVENT_ERROR     = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 1),   /**< Event specify a system error is detected by the device */
   IPA_HW_2_CPU_EVENT_LOG_INFO  = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 2),   /**< Event providing logging specific information */
}IPA_HW_2_CPU_EVENTS;

/**
 * @brief   SW flags define the behavior of HW.
*/
typedef enum
{
   IPA_HW_FLAG_HALT_SYSTEM_ON_ASSERT_FAILURE          = 0x01,   /**< Halt system in case of assert failure */
}IPA_HW_FLAGS;

/**
 * @brief   Common error types.
*/
typedef enum
{
   IPA_HW_ERROR_NONE              = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 0),  /**< No error persists  */
   IPA_HW_INVALID_DOORBELL_ERROR  = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 1),  /**< Invalid data read from doorbell */
   IPA_HW_DMA_ERROR               = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 2),  /**< Unexpected DMA error */
   IPA_HW_FATAL_SYSTEM_ERROR      = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 3),  /**< HW has crashed and requires reset. */
   IPA_HW_INVALID_OPCODE          = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 4),  /**< Invalid opcode sent */
   IPA_HW_INVALID_PARAMS          = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON, 5),  /**< Invalid params for the requested command */

   IPA_HW_CONS_DISABLE_CMD_GSI_STOP_FAILURE = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON,6),
   IPA_HW_PROD_DISABLE_CMD_GSI_STOP_FAILURE = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON,7),
   IPA_HW_GSI_CH_NOT_EMPTY_FAILURE          = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON,8),

   IPA_HW_CONS_STOP_FAILURE       = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON,9),   /**< Failed to stop a consumer pipe */
   IPA_HW_PROD_STOP_FAILURE       = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON,10),   /**< Failed to stop a producer pipe */
   IPA_HW_INVALID_QUEUE_OVERFLOW  = FEATURE_ENUM_VAL(IPA_HW_FEATURE_COMMON,11),   /**< Internal Queue overflow detected */
}IPA_HW_ERRORS;

//#if UNIT_TEST_UC_IMAGE
typedef enum
{
  IPA_CPU_2_HW_CMD_UNIT_TEST                 = FEATURE_ENUM_VAL(IPA_HW_FEATURE_UNIT_TEST, 0),
}IPA_HW_2_CPU_UNIT_TEST_CMDS;


/**
 * @brief   Values that represent unit test framework related HW responses to 
 *          CPU commands.
*/
 typedef enum
 {
    IPA_HW_2_CPU_RESPONSE_DOORBELL_COMPLETED        = FEATURE_ENUM_VAL(IPA_HW_FEATURE_UNIT_TEST, 0),
    IPA_HW_2_CPU_RESPONSE_QMB_READ_COMPLETED        = FEATURE_ENUM_VAL(IPA_HW_FEATURE_UNIT_TEST, 1),
    IPA_HW_2_CPU_RESPONSE_QMB_WRITE_COMPLETED       = FEATURE_ENUM_VAL(IPA_HW_FEATURE_UNIT_TEST, 2),  /**< HW shall send this command once boot sequence is completed and HW is ready to serve commands from CPU */
    IPA_HW_2_CPU_RESPONSE_GSI_DATA_PATH_COMPLETED   = FEATURE_ENUM_VAL(IPA_HW_FEATURE_UNIT_TEST, 3),  /**< Response to CPU commands */
    IPA_HW_2_CPU_RESPONSE_IC_VERIFY_COMPLETED       = FEATURE_ENUM_VAL(IPA_HW_FEATURE_UNIT_TEST, 4),  /**< Response to IPA_CPU_2_HW_CMD_DEBUG_GET_INFO command */
    IPA_HW_2_CPU_RESPONSE_TIMER_COMPLETED           = FEATURE_ENUM_VAL(IPA_HW_FEATURE_UNIT_TEST, 5),  /**< Response to IPA_CPU_2_HW_CMD_DEBUG_GET_INFO command */
   IPA_HW_2_CPU_RESPONSE_GSI_STOP_IN_PROG_COMPLETED = FEATURE_ENUM_VAL(IPA_HW_FEATURE_UNIT_TEST, 6),  /**< Response to IPA_CPU_2_HW_CMD_DEBUG_GET_INFO command */
   IPA_HW_2_CPU_RESPONSE_QMB_MULTI_DMA_READ_COMPLETED = FEATURE_ENUM_VAL(IPA_HW_FEATURE_UNIT_TEST, 7),
   IPA_HW_2_CPU_RESPONSE_QMB_MULTI_DMA_WRITE_COMPLETED = FEATURE_ENUM_VAL(IPA_HW_FEATURE_UNIT_TEST, 8),
   IPA_HW_2_CPU_RESPONSE_QMB_MAX_DMA_READ_COMPLETED    = FEATURE_ENUM_VAL(IPA_HW_FEATURE_UNIT_TEST, 9),   
   IPA_HW_2_CPU_RESPONSE_QMB_MAX_DMA_WRITE_COMPLETED    = FEATURE_ENUM_VAL(IPA_HW_FEATURE_UNIT_TEST, 10),      
 }IPA_HW_2_CPU_UNIT_TEST_RESPONSES;


//#endif /* UNIT_TEST_UC_IMAGE */
/**
 * @brief   Strucuture referring to the common section in 128B shared memory located in 
 *          offset zero of SW Partition in IPA SRAM. 
 * @note    The shared memory is used for communication between IPA HW and CPU. 
 *                                                                                        
*/
typedef struct IpaHwSharedMemCommonMapping_t
{
   /*word 0 */
   uint8_t  cmdOp;           /**< CPU->HW command opcode. See IPA_CPU_2_HW_COMMANDS */
   uint8_t  reserved_01;
   uint16_t reserved_03_02;
   /*word 1 & 2 */
   uint32_t cmdParams_lo;    /**< CPU->HW command parameter. The parameter filed can hold 32 bits of parameters (immediate parameters) or pointer to structure in system memory (in such case the address must be accessible for HW) */
   uint32_t cmdParams_hi;    /**< CPU->HW command parameter. The parameter filed can hold upper 32 bits of parameters (immediate parameters) or pointer to structure in system memory (in such case the address must be accessible for HW) */
   /*word 3 */
   uint8_t  responseOp;      /**< HW->CPU response opcode. See IPA_HW_2_CPU_RESPONSES */
   uint8_t  reserved_0D;
   uint16_t reserved_0F_0E;
   /*word 4 */
   uint32_t responseParams;  /**< HW->CPU response parameter. The parameter filed can hold 32 bits of parameters (immediate parameters) or pointer to structure in system memory*/
   /*word 5 */
   uint8_t  eventOp;         /**< HW->CPU event opcode. See IPA_HW_2_CPU_EVENTS */
   uint8_t  reserved_15;
   uint16_t reserved_17_16;   
   /*word 6 */
   uint32_t eventParams;     /**< HW->CPU event parameter. The parameter filed can hold 32 bits of parameters (immediate parameters) or pointer to structure in system memory*/
   /*word 7 */
   uint32_t firstErrorAddress;  /**< Contains the address of first error-source on SNOC */
   /*word 8 */
   uint8_t  hwState;            /**< State of HW. The state carries information regarding the error type. See IPA_HW_ERRORS */
   uint8_t  reserved_21;
   uint16_t reserved_23_22;
   /*word 9 */
   uint16_t uCFwVersion;        /**< The Fw version as reported by HW*/
   uint16_t reserved_27_26;
}IpaHwSharedMemCommonMapping_t;

/*CPU->HW Common commands*/

/**
 * @brief   Structure holding the parameters for IPA_CPU_2_HW_CMD_UPDATE_FLAGS command.
 *          Parameters are sent as 32b immediate parameters.
*/
typedef union IpaHwUpdateFlagsCmdData_t
{
   struct IpaHwUpdateFlagsCmdParams_t
   {
      uint32_t     newFlags;      /**< SW flags defined the behavior of HW. This field is expected to be used as bitmask for IPA_HW_FLAGS*/
   }params;
   uint32_t raw32b;
}IpaHwUpdateFlagsCmdData_t;

/**
 * @brief   Structure holding the parameters for 
 *          IPA_CPU_2_HW_CMD_GSI_CH_EMPTY command. Parameters are
 *          sent as 32b immediate parameters.
*/
typedef union IpaHwChkChEmptyCmdData_t
{
   struct IpaHwChkChEmptyCmdParams_t
   {
      uint8_t     EE_n;
      uint8_t     vir_ch_id;
      uint16_t    reserved_02_04;
   }params;
   uint32_t raw32b;
}IpaHwChkChEmptyCmdData_t;

/*HW->CPU Common responses*/

/**
 * @brief   Structure holding the parameters for IPA_HW_2_CPU_RESPONSE_CMD_COMPLETED response.
 *          Parameters are sent as 32b immediate parameters.
*/
typedef union IpaHwCpuCmdCompletedResponseData_t
{
   struct IpaHwCpuCmdCompletedResponseParams_t
   {
      uint32_t     originalCmdOp     :8;  /**< The original command opcode */
      uint32_t     status            :8;  /**< 0 for success indication, otherwise failure */
      uint32_t     reserved          :16;
   }params;
   uint32_t raw32b;
}IpaHwCpuCmdCompletedResponseData_t;

/*HW->CPU Common Events */

typedef union IpaHwErrorEventData_t
{
   struct IpaHwErrorEventParams_t
   {
      uint32_t     errorType :8;        /**< Entered when a system error is detected by the HW. Type of error is specified by IPA_HW_ERRORS*/
      uint32_t     reserved  :24;
   }params;
   uint32_t raw32b;
}IpaHwErrorEventData_t;


/******************************************************************************
               Data Types related to debug logging 
******************************************************************************/
typedef union IpaHwFeatureInfoData_t
{
   struct IpaHwFeatureInfoParams_t
   {
      uint32_t     offset  :16;     /**< Location of a feature within the EventInfoData */
      uint32_t     size    :16;     /**< Size of the feature */
   }params;
   uint32_t raw32b;
}IpaHwFeatureInfoData_t;

/**
 * @brief   Structure holding the parameters for 
 *          statistics and config info
 * @note    Information about each feature in the featureInfo[]
 *          array is populated at predefined indices per the
 *          IPA_HW_FEATURES enum definition
*/
typedef struct IpaHwEventInfoData_t
{
   uint32_t                 baseAddrOffset;                      /**< Base Address Offset of the statistics or config structure from IPA_WRAPPER_BASE */
   IpaHwFeatureInfoData_t   featureInfo[IPA_HW_NUM_FEATURES];    /**< Location and size of each feature within the statistics or config structure */
}IpaHwEventInfoData_t;


/**
 * @brief   Structure holding the parameters for 
 *          IPA_HW_2_CPU_EVENT_LOG_INFO Event
 * @note    The offset location of this structure from 
 *          IPA_WRAPPER_BASE will be provided as Event Params
 *          for the IPA_HW_2_CPU_EVENT_LOG_INFO Event
*/
typedef struct IpaHwEventLogInfoData_t
{
   uint32_t                 featureMask;             /**< Mask indicating the features enabled in HW. Refer IPA_HW_FEATURE_MASK */
   uint32_t                 circBuffBaseAddrOffset;  /**< Base Address Offset of the Circular Event Log Buffer structure */
   IpaHwEventInfoData_t     statsInfo;               /**< Statistics related information */
   IpaHwEventInfoData_t     configInfo;              /**< Configuration related information */

}IpaHwEventLogInfoData_t;

/******************************************************************************
               Data Types related to Statistics Collection 
******************************************************************************/
/**
 * @brief   Structure holding the Gsi statistics
*/
typedef struct IpaHwGsiStats_t
{
   uint32_t  gsiFifoFull;          /**< Number of times Gsi Fifo got full - For In Ch: Good, For Out Ch: Bad */
   uint32_t  gsiFifoEmpty;         /**< Number of times GsiFifo got empty - For In Ch: Bad, For Out Ch: Good */
   uint32_t  gsiFifoUsageHigh;     /**< Number of times Gsi fifo usage went above 75% - For In Ch: Good, For Out Ch: Bad */
   uint32_t  gsiFifoUsageLow;      /**< Number of times Gsi fifo usage went below 25% - For In Ch: Bad, For Out Ch: Good */
   uint32_t  gsiUtilCount;         /**< Number of times we sample the Gsi Fifo utilization  */
} IpaHwGsiStats_t;

/**
 * @brief   Structure holding the Protocol Ring statistics
*/
typedef struct IpaHwRingStats_t
{
   uint32_t  ringFull;             /**< Number of times Transfer Ring got full - For In Ch: Good, For Out Ch: Bad */
   uint32_t  ringEmpty;            /**< Number of times Transfer Ring got empty - For In Ch: Bad, For Out Ch: Good */
   uint32_t  ringUsageHigh;        /**< Number of times Transfer Ring usage went above 75% - For In Ch: Good, For Out Ch: Bad */
   uint32_t  ringUsageLow;         /**< Number of times Transfer Ring usage went below 25% - For In Ch: Bad, For Out Ch: Good */
   uint32_t  RingUtilCount;        /**< Number of times we sample the Transfer Ring utilization  */
} IpaHwRingStats_t;

/**
 * @brief   Structure holding the common statistics info of hw 
*/
typedef struct IpaHwStatsCommonInfoData_t
{
   uint64_t hw_usage_time;    /**< hw usage time counted in IPA clock ticks */
   uint32_t NumClkGateReq;    /**< Number of Clock Gate request from CPU */
   uint32_t NumClkUngateReq;  /**< Number of Clock Ungate request from CPU */  
}IpaHwStatsCommonInfoData_t;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_STATE 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 rx_wait : 1;
  uint32 rx_idle : 1;
  uint32 tx_idle : 1;
  uint32 reserved0 : 1;
  uint32 bam_gsi_idle : 1;
  uint32 ipa_status_sniffer_idle : 1;
  uint32 ipa_noc_idle : 1;
  uint32 aggr_idle : 1;
  uint32 mbim_aggr_idle : 1;
  uint32 ipa_rsrc_mngr_db_empty : 1;
  uint32 ipa_rsrc_state_idle : 1;
  uint32 ipa_ackmngr_db_empty : 1;
  uint32 ipa_ackmngr_state_idle : 1;
  uint32 ipa_tx_ackq_full : 1;
  uint32 ipa_prod_ackmngr_db_empty : 1;
  uint32 ipa_prod_ackmngr_state_idle : 1;
  uint32 ipa_prod_bresp_idle : 1;
  uint32 reserved1 : 1;
  uint32 ipa_ntf_tx_empty : 1;
  uint32 ipa_tx_ackq_empty : 1;
  uint32 ipa_uc_ackq_empty : 1;
  uint32 ipa_rx_ackq_empty : 1;
  uint32 ipa_tx_commander_cmdq_empty : 1;
  uint32 ipa_rx_splt_cmdq_empty : 1;
  uint32 reserved2 : 4;
  uint32 ipa_rx_hps_empty : 1;
  uint32 ipa_hps_dps_empty : 1;
  uint32 ipa_dps_tx_empty : 1;
  uint32 ipa_uc_rx_hnd_cmdq_empty : 1;
} IpaHw_def_ipa_state_s;

/* Union definition of register */
typedef union 
{
  IpaHw_def_ipa_state_s def;
  uint32 value;
} IpaHw_def_ipa_state_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_TESTBUS_SEL 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 testbus_en : 1;
  uint32 reserved0 : 3;
  uint32 external_block_select : 8;
  uint32 internal_block_select : 8;
  uint32 pipe_select : 5;
  uint32 reserved1 : 7;
} IpaHw_def_ipa_testbus_sel_s;

/* Union definition of register */
typedef union 
{
  IpaHw_def_ipa_testbus_sel_s def;
  uint32 value;
} IpaHw_def_testbus_sel_u;

/*===========================================================================*/
/*!
  @brief Bit Field definition of register: IPA_QSB_MAX_WRITES 
*/
/*===========================================================================*/
/* Structure definition of register */
typedef struct 
{
  uint32 gen_qmb_0_max_writes : 4;
  uint32 gen_qmb_1_max_writes : 4;
  uint32 reserved0 : 24;
} ipa_hwio_def_ipa_qsb_max_writes_s;

/* Union definition of register */
typedef union 
{
  ipa_hwio_def_ipa_qsb_max_writes_s def;
  uint32 value;
} ipa_hwio_def_ipa_qsb_max_writes_u;

#endif /* IPA_HW_INTERFACE_H_ */
