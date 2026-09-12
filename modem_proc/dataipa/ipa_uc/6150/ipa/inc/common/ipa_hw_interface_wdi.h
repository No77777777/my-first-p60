/*=========================================================================*//**
    @file  ipa_hw_interface_wdi.h

    @brief Define the IPA HW to IPA driver WDI interface
*//****************************************************************************/
/*------------------------------------------------------------------------------
    Copyright (c) 2014 Qualcomm Technologies Incorporated.
    All Rights Reserved.
    Qualcomm Confidential and Proprietary
 
    $Header: //components/rel/dataipa.mpss/3.2/ipa_uc/6150/ipa/inc/common/ipa_hw_interface_wdi.h#1 $
------------------------------------------------------------------------------*/

#ifndef IPA_HW_INTERFACE_WDI_H_
#define IPA_HW_INTERFACE_WDI_H_
#include "ipa_hw_interface.h"

#define IPA_HW_INTERFACE_WDI_VERSION            0x0004     /**< IPA HW to IPA Driver WDI interface version*/

#define IPA_HW_MAX_WDI_TX_CHANNELS              1
#define IPA_HW_MAX_WDI_RX_CHANNELS              1

#define IPA_HW_WDI_MAX_CMD_SIZE                 sizeof(IpaHwWdiTxSetUpCmdData_t)

#define IPA_HW_WDI_MBOX_GROUP                   3
/*
  This is used to allocate the mailbox register that will be used for WDI Rx path
  In terms of IPA registers, IPA_UC_MAILBOX_m_n, m=[0..3], n=[0..31], this will correspond
  to m = 1, n = 16.
 
  Currently we can support 2 Rx channels. It is assumed that we will only have to support
  32 bit addresses. If the DDR location is greater than 32 bits, we will need two mailbox
  register for each doorbell
 
  The physical address of the mailbox registers will be
  IPA_WRAPPER_BASE_PHYS + 0x00072000 + 4*(32 + n) where n = 16 or n = 17
*/
#define IPA_HW_WDI_RX_MBOX_START_INDEX          48

/*
  This is used to allocate the mailbox register that will be used for WDI Tx path (CE ring)
  In terms of IPA registers, IPA_UC_MAILBOX_m_n, m=[0..3], n=[0..31], this will correspond
  to m = 1, n = 18
 
  Currently we can support 2 Tx channels. It is assumed that we will only have to support
  32 bit addresses. If the DDR location is greater than 32 bits, we will need two mailbox
  register for each doorbell
 
  The physical address of the mailbox registers will be
  IPA_WRAPPER_BASE_PHYS + 0x00072000 + 4*(32 + n) where n = 18 or n = 19
*/
#define IPA_HW_WDI_TX_MBOX_START_INDEX          50

/**
 * @brief   Values that represent WDI channel state machine. 
 *          These states apply to both Tx and Rx paths. These do
 *          not reflect the sub-state the state machine may be
 *          in.
 *          
*/
typedef enum
{
	IPA_HW_WDI_CHANNEL_STATE_INITED_DISABLED = 1,  /**< Channel is initialized but disabled       */
   IPA_HW_WDI_CHANNEL_STATE_ENABLED_SUSPEND = 2,  /**< Channel is enabled but in suspended state */
	IPA_HW_WDI_CHANNEL_STATE_RUNNING         = 3,  /**< Channel is running. Entered after SET_UP_COMMAND is processed successfully */
	IPA_HW_WDI_CHANNEL_STATE_ERROR           = 4,  /**< Channel is in error state */
	IPA_HW_WDI_CHANNEL_STATE_INVALID         = 0xFF /**< Invalid state. Shall not be in use in operational scenario  */
}IPA_HW_WDI_CHANNEL_STATES;

/**
 * @brief   Values that represent the WDI commands from CPU (IPA
 *          Driver) to IPA HW. Note that there is no WDI
 *          specific response at this time. Response to the
 *          commands listed here are listed in the type
 *          IPA_HW_2_CPU_RESPONSES in the file
 *          ipa_hw_interface.h
*/

typedef enum
{
   IPA_CPU_2_HW_CMD_WDI_TX_SET_UP             = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI, 0),     /**< Command to set up WDI Tx Path        */
   IPA_CPU_2_HW_CMD_WDI_RX_SET_UP             = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI, 1),     /**< Command to set up WDI Rx Path        */
   IPA_CPU_2_HW_CMD_WDI_RX_EXT_CFG            = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI, 2),     /**< Provide extended config info for 
                                                                                                                               Rx path */

   //Following commands are common for Tx and Rx path
   IPA_CPU_2_HW_CMD_WDI_CH_ENABLE             = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI, 3),     /**< Command to enable a channel          */
   IPA_CPU_2_HW_CMD_WDI_CH_DISABLE            = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI, 4),     /**< Command to disable a channel         */
   IPA_CPU_2_HW_CMD_WDI_CH_SUSPEND            = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI, 5),     /**< Command to suspend a channel         */
   IPA_CPU_2_HW_CMD_WDI_CH_RESUME             = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI, 6),     /**< Command to resume a channel          */
   IPA_CPU_2_HW_CMD_WDI_TEAR_DOWN             = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI, 7),     /**< Command to tear down WDI Tx/ Rx Path */

}IPA_CPU_2_HW_WDI_COMMANDS;

/**
 * @brief   Values that represent WDI related command response 
 *          status to be sent to CPU.
*/
typedef enum
{
   IPA_HW_2_CPU_WDI_CMD_STATUS_SUCCESS                = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI,0),
   IPA_HW_2_CPU_MAX_WDI_TX_CHANNELS                   = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI,1),
   IPA_HW_2_CPU_WDI_CE_RING_OVERRUN_POSSIBILITY       = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI,2),
   IPA_HW_2_CPU_WDI_CE_RING_SET_UP_FAILURE            = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI,3),
   IPA_HW_2_CPU_WDI_CE_RING_PARAMS_UNALIGNED          = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI,4),
   IPA_HW_2_CPU_WDI_COMP_RING_OVERRUN_POSSIBILITY     = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI,5),
   IPA_HW_2_CPU_WDI_COMP_RING_SET_UP_FAILURE          = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI,6),
   IPA_HW_2_CPU_WDI_COMP_RING_PARAMS_UNALIGNED        = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI,7),
   IPA_HW_2_CPU_WDI_UNKNOWN_TX_CHANNEL                = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI,8),
   IPA_HW_2_CPU_WDI_TX_INVALID_FSM_TRANSITION         = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI,9),
   IPA_HW_2_CPU_WDI_TX_FSM_TRANSITION_ERROR           = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI,10),
   IPA_HW_2_CPU_MAX_WDI_RX_CHANNELS                   = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI,11),
   IPA_HW_2_CPU_WDI_RX_RING_PARAMS_UNALIGNED          = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI,12),
   IPA_HW_2_CPU_WDI_RX_RING_SET_UP_FAILURE            = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI,13),
   IPA_HW_2_CPU_WDI_UNKNOWN_RX_CHANNEL                = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI,14),
   IPA_HW_2_CPU_WDI_RX_INVALID_FSM_TRANSITION         = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI,15),
   IPA_HW_2_CPU_WDI_RX_FSM_TRANSITION_ERROR           = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI,16),
}IPA_HW_2_CPU_CMD_RESP_STATUS;

/**
 * @brief   Values that represent WDI related HW event to be 
 *          sent to CPU.
*/
typedef enum
{
   IPA_HW_2_CPU_EVENT_WDI_ERROR       = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI, 0),   /**< Event to specify that HW detected an error in WDI */
}IPA_HW_2_CPU_WDI_EVENTS;

/**
 * @brief   List of WDI error types. This is present in the 
 *          event param.
*/
typedef enum
{
   IPA_HW_WDI_ERROR_NONE              = 0, /**< No error persists.  */
   IPA_HW_WDI_CHANNEL_ERROR           = 1  /**< Error is spcific to channel */
}IPA_HW_WDI_ERRORS;

/**
 * @brief   List of WDI Channel error types. This is present in 
 *          the event param.
*/
typedef enum
{
   IPA_HW_WDI_CH_ERR_NONE                             = 0,   /**< No error persists.  */
   IPA_HW_WDI_TX_COMP_RING_WP_UPDATE_FAIL             = 1,   /**< Write pointer update failed in Tx Completion ring */
   IPA_HW_WDI_TX_FSM_ERROR                            = 2,   /**< Error in the state machine transition */
   IPA_HW_WDI_TX_COMP_RE_FETCH_FAIL                   = 3,   /**< Error while calculating num RE to bring */
   IPA_HW_WDI_RX_RING_WP_UPDATE_FAIL                  = 4,   /**< Write pointer update failed in Rx ring */
   IPA_HW_WDI_RX_FSM_ERROR                            = 5,   /**< Error in the state machine transition */
   IPA_HW_WDI_CH_ERR_RESERVED                         = 0xFF /**< Reserved - Not available for CPU to use */
}IPA_HW_WDI_CH_ERRORS;


/**
 * @brief   Values that represent WDI related HW responses to 
 *          CPU commands.
*/
typedef enum
{
   IPA_HW_2_CPU_RESPONSE_WDI_CMD_COMPLETE_RESP  = FEATURE_ENUM_VAL(IPA_HW_FEATURE_WDI, 1),   /**< Response to IPA_CPU_2_HW_CMD_WDI_TX_SET_UP,
                                                                                                              IPA_CPU_2_HW_CMD_WDI_RX_SET_UP,
                                                                                                              IPA_CPU_2_HW_CMD_WDI_TEAR_DOWN */
}IPA_HW_2_CPU_WDI_RESPONSES;


#pragma pack(push,1) /*Save previous, and turn on 1 byte alignment*/

/**
 * @brief   Structure referring to the common and WDI section of 128B shared memory located 
 *          in offset zero of SW Partition in IPA SRAM. 
 * @note    The shared memory is used for communication between IPA HW and CPU. 
 *                                                                                        
*/
typedef PACK(struct) IpaHwSharedMemWdiMapping_t
{
   /*word 0 - word 9 */
   IpaHwSharedMemCommonMapping_t common;
   /*word 10 */
   uint32_t reserved_2B_28;   /* Bytes 0x28 - 0x2B reserved */
   /*word 11 */
   uint32_t reserved_2F_2C;   
   /*word 12 */
   uint32_t reserved_33_30;   
   /*word 13 */
   uint32_t reserved_37_34;
   /*word 14 */
   uint32_t reserved_3B_38;
   /*word 15 */
   uint32_t reserved_3F_3C;

   /*word 16 */
   uint16_t interfaceVersionWdi;
   uint16_t reserved_43_42;

   /*word 17*/
   uint8_t  wdi_tx_ch_0_state; //wdi_tx_ch_0_state
   uint8_t  wdi_rx_ch_0_state;
   uint16_t reserved_47_46;
}IpaHwSharedMemWdiMapping_t;

/* Data structures for CPU->HW WDI commands*/
/**
   * @brief   Structure holding the parameters for IPA_CPU_2_HW_CMD_WDI_TX_SET_UP command.
   *          Parameters are sent as pointer thus should be reside in address accessible to HW
*/
typedef PACK(struct) IpaHwWdiTxSetUpCmdData_t
{
   uint32_t comp_ring_base_pa;   /**< This is the physical address of the base of the Tx completion ring */
   uint16_t comp_ring_size;      /**< This is the size of the Tx completion ring */
   uint16_t reserved_comp_ring;  /**< Reserved field for expansion of Completion ring params */
   uint32_t ce_ring_base_pa;     /**< This is the physical address of the base of the Copy Engine Source Ring */
   uint16_t ce_ring_size;        /**< Copy Engine Ring size */
   uint16_t reserved_ce_ring;    /**< Reserved field for expansion of CE ring params*/
   uint32_t ce_ring_doorbell_pa; /**< This is the physical address of the doorbell that the IPA uC has to write into to trigger the copy engine */
   uint16_t num_tx_buffers;      /**< Number of pkt buffers allocated. The size of the CE ring and the Tx completion ring has to be atleast ( num_tx_buffers + 1) */
   uint8_t  ipa_pipe_number;  	/**< This is the IPA pipe number that has to be used for the Tx path */
   uint8_t  reserved;            /**< Reserved field */
}IpaHwWdiTxSetUpCmdData_t;

/**
    * @brief   Structure holding the parameters for 
    *          IPA_CPU_2_HW_CMD_WDI_RX_SET_UP command.
    *  
    *          Parameters are sent as pointer thus should be reside
    *          in address accessible to HW
*/
typedef PACK(struct) IpaHwWdiRxSetUpCmdData_t
{
   uint32_t rx_ring_base_pa;  /**< This is the physical address of the base of the Rx ring (containing Rx buffers) */
   uint32_t rx_ring_size;     /**< This is the size of the Rx ring */
   uint32_t rx_ring_rp_pa;    /**< This is the physical address of the location through which IPA uc is expected to communicate about the Read pointer into the Rx Ring */
   uint8_t  ipa_pipe_number; 	/**< This is the IPA pipe number that has to be used for the Rx path */
}IpaHwWdiRxSetUpCmdData_t;


/**
 * @brief   Structure holding the parameters for 
 *          IPA_CPU_2_HW_CMD_WDI_RX_EXT_CFG command. The
 *          parameters are passed as immediate params in the
 *          shared memory
*/
typedef PACK(union) IpaHwWdiRxExtCfgCmdData_t
{
   PACK(struct) IpaHwWdiRxExtCfgCmdParams_t
   {
      uint32_t  ipa_pipe_number :8;  /**< The IPA pipe number for which this config is passed */
      uint32_t  qmap_id         :8;  /**< QMAP ID to be set in the metadata register          */
      uint32_t  reserved        :16;
   }params;
   uint32_t raw32b;
}IpaHwWdiRxExtCfgCmdData_t;

/**
 * @brief   Structure holding the parameters for 
 *          IPA_CPU_2_HW_CMD_WDI_TEAR_DOWN,
 *          IPA_CPU_2_HW_CMD_WDI_CH_ENABLE,
 *          IPA_CPU_2_HW_CMD_WDI_CH_DISABLE,
 *          IPA_CPU_2_HW_CMD_WDI_CH_SUSPEND,
 *          IPA_CPU_2_HW_CMD_WDI_CH_RESUME command. The
 *          parameters are passed as immediate params in the
 *          shared memory
*/
typedef PACK(union) IpaHwWdiCommonChCmdData_t
{
   PACK(struct) IpaHwWdiCommonChCmdParams_t
   {
      uint32_t  ipa_pipe_number :8;  /**< The IPA pipe number. This could be Tx or an Rx pipe */      
      uint32_t  reserved        :24;
   }params;
   uint32_t raw32b;
}IpaHwWdiCommonChCmdData_t;

typedef IpaHwWdiCommonChCmdData_t IpaHwWdiTearDownCmdData_t;
typedef IpaHwWdiCommonChCmdData_t IpaHwWdiEnableChCmdData_t;
typedef IpaHwWdiCommonChCmdData_t IpaHwWdiDisableChCmdData_t;
typedef IpaHwWdiCommonChCmdData_t IpaHwWdiSuspendChCmdData_t;
typedef IpaHwWdiCommonChCmdData_t IpaHwWdiResumeChCmdData_t;


/**
 * @brief   Structure holding the parameters for 
 *          IPA_HW_2_CPU_EVENT_WDI_ERROR event. The
 *          parameters are passed as immediate params in the
 *          shared memory
*/
typedef PACK(union) IpaHwWdiErrorEventData_t
{
   PACK(struct) IpaHwWdiErrorEventParams_t
   {
      uint32_t  wdi_error_type  :8;  /**< The type of WDI error (IPA_HW_WDI_ERRORS) */      
      uint32_t  reserved        :8;  /**< Reserved */
      uint32_t  ipa_pipe_number :8;  /**< IPA pipe number on which error has happened. Applicable only if error type indicates channel error */
      uint32_t  wdi_ch_err_type :8;  /**< Information about the channel error (if available) */

   }params;
   uint32_t raw32b;
}IpaHwWdiErrorEventData_t;

#pragma pack(pop) /* Revert alignment to what it was previously*/

/******************************************************************************
               Data Types related to debug information to be added here
******************************************************************************/
/**
 * @brief   Structure holding the WDI Rx channel structures 
 *          Ensure that this is always word aligned 
*/
typedef struct IpaHwStatsWDIRxInfoData_t
{
   //Packet related statistics
   uint32_t  max_outstanding_pkts; /**< Number of outstanding packets in Rx Ring    */
   uint32_t  num_pkts_processed;   /**< Number of packets processed - cumulative    */

   //Ring related info
   uint32_t  rx_ring_rp_value;     /**< Read pointer last advertized to the WLAN FW */

   //Ring related statistics
   IpaHwRingStats_t rx_ind_ring_stats;

   // GSI related stats
   IpaHwGsiStats_t gsi_stats;

   //FW related stats
   uint32_t  num_bam_int_handled;  /**< Number of Bam Interrupts handled by FW      */
   uint32_t  num_db;               /**< Number of times the doorbell was rung       */
   uint32_t  num_unexpected_db;    /**< Number of unexpected doorbells              */
   uint32_t  num_pkts_in_dis_uninit_state;
   uint32_t  num_ic_inj_vdev_change; /**< Number of times the Imm Cmd is injected due to vdev_id change */
   uint32_t  num_ic_inj_fw_desc_change; /**< Number of times the Imm Cmd is injected due to fw_desc change */


   uint32_t  reserved0;
   uint32_t  reserved1;
   uint32_t  reserved2;

}IpaHwStatsWDIRxInfoData_t;

/**
 * @brief   Structure holding the WDI Tx channel structures 
 *          Ensure that this is always word aligned 
*/
typedef struct IpaHwStatsWDITxInfoData_t
{
   //Packet related statistics
   uint32_t  num_pkts_processed;   /**< Number of packets processed - cumulative        */
   //Copy Engine related info
   uint32_t  copy_engine_doorbell_value;
                                   /**< Latest value of doorbell written to copy engine */
   uint32_t  num_db_fired;         /**< Number of DB from uC FW to Copy engine          */

   //Ring related statistics
   IpaHwRingStats_t tx_comp_ring_stats;//5 + 3 = 8 

   // GSI related stats
   IpaHwGsiStats_t gsi_stats;

   //FW related stats   
   uint32_t  num_db;               /**< Number of times the doorbell was rung       */
   uint32_t  num_unexpected_db;    /**< Number of unexpected doorbells              */
   uint32_t  num_bam_int_handled;  /**< Number of Bam Interrupts handled by FW      */   
   uint32_t  num_bam_int_in_non_runnning_state;
                                   /**< Number of Bam interrupts while not in Running state */
   uint32_t  num_qmb_int_handled;  /**< Number of QMB interrupts handled            */
   uint32_t  num_bam_int_handled_while_wait_for_bam;

}IpaHwStatsWDITxInfoData_t;

/**
 * @brief   Structure holding the WDI channel structures 
 *           
*/
typedef struct IpaHwStatsWDIInfoData_t
{
   IpaHwStatsWDIRxInfoData_t rx_ch_stats[IPA_HW_MAX_WDI_RX_CHANNELS];
   IpaHwStatsWDITxInfoData_t tx_ch_stats[IPA_HW_MAX_WDI_TX_CHANNELS];
}IpaHwStatsWDIInfoData_t;



#endif /* IPA_HW_INTERFACE_WDI_H_ */
