/*=========================================================================*//**
    @file  ipa_uc_interface.h

    @brief Define the uC to IPA driver interface
*//****************************************************************************/
/*------------------------------------------------------------------------------
    Copyright (c) 2013 Qualcomm Technologies Incorporated.
    All Rights Reserved.
    Qualcomm Confidential and Proprietary
------------------------------------------------------------------------------*/

#ifndef IPA_UC_INTERFACE_H_
#define IPA_UC_INTERFACE_H_

#define IPA_UC_INTREFACE_VERSION                0x010B     /**< A7 to MHI engine interface version*/
#define IPA_UC_FIRST_ERROR_ADDRESS_RESET_VALUE  0x7EEDBEAF /**< Reset value for firstErrorAddress field in shared memory  */

#define IPA_UC_MAX_NUMBER_OF_CHANNELS           2
#define IPA_UC_MAX_CHANNEL_HANDLE               (IPA_UC_MAX_NUMBER_OF_CHANNELS-1)

/**
 * @brief   ZIP Shared Mem Region in DRAM to share ZIP related info between CPU and HWP 
*/
#define IPA_HW_ZIP_SHAREDMEM_ADDR_OFFSET     0x07A00

#define IPA_HW_ZIP_SHAREDMEM_SIZE_IN_BYTES   512

/**
 * @brief   ZIP write index of descriptor fifo of high priority consumer
*/
#define IPA_HW_ZIP_HPQ_CNSMR_WI_OFFSET       IPA_HW_ZIP_SHAREDMEM_ADDR_OFFSET

/**
 * @brief   ZIP write index of descriptor fifo of low priority consumer
*/
#define IPA_HW_ZIP_LPQ_CNSMR_WI_OFFSET       (IPA_HW_ZIP_HPQ_CNSMR_WI_OFFSET + 4)

/**
 * @brief   ZIP write index of descriptor fifo of high priority producer
*/
#define IPA_HW_ZIP_HPQ_PRDCR_WI_OFFSET       (IPA_HW_ZIP_LPQ_CNSMR_WI_OFFSET + 4)

/**
 * @brief   ZIP write index of descriptor fifo of low priority producer
*/
#define IPA_HW_ZIP_LPQ_PRDCR_WI_OFFSET       (IPA_HW_ZIP_HPQ_PRDCR_WI_OFFSET + 4)

/**
 * @brief   ZIP read index of descriptor fifo of high priority producer
*/
#define IPA_HW_ZIP_HPQ_PRDCR_RI_OFFSET       (IPA_HW_ZIP_LPQ_PRDCR_WI_OFFSET + 4)

/**
 * @brief   ZIP read index of descriptor fifo of low priority producer
*/
#define IPA_HW_ZIP_LPQ_PRDCR_RI_OFFSET       (IPA_HW_ZIP_HPQ_PRDCR_RI_OFFSET + 4)

/**
 * @brief   The offset of IpaHwZipInitCmdData_t struct in uC DRAM for ZIP initialization
*/
#define IPA_HW_ZIP_INIT_CMD_ADDR_OFFSET      (IPA_HW_ZIP_LPQ_PRDCR_RI_OFFSET + 4)

/**
 * @brief   The mailbox group index of uC ZIP commands doorbells
*/
#define IPA_HW_ZIP_MBOX_GROUP           0

/**
 * @brief   The mailbox index of uC ZIP init command
*/
#define IPA_HW_ZIP_INIT_MBOX_n         19

/**
 * @brief   The mailbox index of uC ZIP doorbell for high priority queue zip fifo
*/
#define IPA_HW_ZIP_HPQ_MBOX_n          20

/**
 * @brief   The mailbox index of uC ZIP doorbell for low priority queue zip fifo
*/
#define IPA_HW_ZIP_LPQ_MBOX_n          21

/**
 * @brief   The size of uC generated status
*/
#define IPA_UC_ZIP_STATUS_SIZE         0x20

/**
 * @brief   The interrupt which uC uses to send 'transfer is finished' signal to Q6
*/
#define IPA_UC_ZIP_TRANSFER_FINISHED_IRQ HAL_IPA_UC_IRQ_2

/**
 * @brief   Values that represent MHI channel state machine.
 * Values are according to MHI specification
*/
typedef enum
{
	IPA_UC_MHI_CHANNEL_STATE_DISABLE = 0,  /**< Channel is disabled and not processed by the host or device.  */
	IPA_UC_MHI_CHANNEL_STATE_ENABLE  = 1,  /**< A channel is enabled after being initialized and configured by host, including its channel context and associated transfer ring. While this state, the channel is not active and the device does not process transfer.  */
	IPA_UC_MHI_CHANNEL_STATE_RUN     = 2,  /**< The device processes transfers and doorbell for channels.  */
	IPA_UC_MHI_CHANNEL_STATE_SUSPEND = 3,  /**< Used to halt operations on the channel. The device does not process transfers for the channel in this state. This state is typically used to synchronize the transition to low power modes.  */
	IPA_UC_MHI_CHANNEL_STATE_STOP	   = 4,  /**< Used to halt operations on the channel. The device does not process transfers for the channel in this state. */
	IPA_UC_MHI_CHANNEL_STATE_ERROR   = 5   /**< The device detected an error in an element from the transfer ring associated with the channel.  */
}IPA_UC_MHI_CHANNEL_STATES;

/**
 * @brief   Values that represent the command from CPU to uC.
*/
typedef enum
{
   IPA_CPU_2_UC_CMD_NO_OP                     = 0x0,  	 /**< No operation is required.  */
   IPA_CPU_2_UC_CMD_MHI_INIT                  = 0x1,     /**< Initialize uC to be ready for MHI processing. Once operation was completed uC shall respond with IPA_UC_2_CPU_RESPONSE_CMD_COMPLETED */
   IPA_CPU_2_UC_CMD_MHI_INIT_CHANNEL          = 0x2,     /**< Initialize specific channel to be ready to serve MHI transfers. Once initialization was completed uC shall respond with IPA_UC_2_CPU_RESPONSE_MHI_CHANGE_CHANNEL_STATE.IPA_UC_MHI_CHANNEL_STATE_ENABLE  */
   IPA_CPU_2_UC_CMD_MHI_UPDATE_MSI            = 0x3,     /**< Update MHI MSI interrupts data. Once operation was completed uC shall respond with IPA_UC_2_CPU_RESPONSE_CMD_COMPLETED */
   IPA_CPU_2_UC_CMD_MHI_CHANGE_CHANNEL_STATE  = 0x4,     /**< Change specific channel processing state following host request. Once operation was completed uC shall respond with IPA_UC_2_CPU_RESPONSE_MHI_CHANGE_CHANNEL_STATE */
   IPA_CPU_2_UC_CMD_UPDATE_FLAGS              = 0x10,    /**< Update SW flags which defines the behavior of uC . Once operation was completed uC shall respond with IPA_UC_2_CPU_RESPONSE_CMD_COMPLETED */
   IPA_CPU_2_UC_CMD_DEBUG_RUN_TEST            = 0x20,    /**< Launch predefined test over uC  */
   IPA_CPU_2_UC_CMD_DEBUG_GET_INFO            = 0x21,    /**< Read uC internal debug information  */
   IPA_CPU_2_UC_CMD_INIT_RPMH_HANDSHAKE_TEST  = 0x22,    /**< Initial uC for testing handshake with RPMh for power collapse and restore */
   IPA_CPU_2_UC_CMD_INVALID                   = 0xFF     /**< Invalid command. Shall not be in use in operational scenario  */
}IPA_CPU_2_UC_COMMANDS;

/**
 * @brief   Values that represent uC responses to CPU commands.
*/
typedef enum
{
   IPA_UC_2_CPU_RESPONSE_NO_OP                        = 0x0,
   IPA_UC_2_CPU_RESPONSE_MHI_CHANGE_CHANNEL_STATE     = 0x1,   /**< Response to IPA_CPU_2_UC_CMD_MHI_INIT_CHANNEL or IPA_CPU_2_UC_CMD_MHI_CHANGE_CHANNEL_STATE commands. */
   IPA_UC_2_CPU_RESPONSE_INIT_COMPLETED               = 0x2,   /**< uC shall send this command once boot sequence is completed and uC is ready to serve commands from CPU */
   IPA_UC_2_CPU_RESPONSE_CMD_COMPLETED                = 0x3,   /**< Response to CPU commands */
   IPA_UC_2_CPU_RESPONSE_DEBUG_GET_INFO               = 0x10,  /**< Response to IPA_CPU_2_UC_CMD_DEBUG_GET_INFO command */
   IPA_UC_2_CPU_RESPONSE_INVALID                      = 0xFF   /**< Invalid respond. Shall not be in use in operational scenario  */
}IPA_UC_2_CPU_RESPONSES;

/**
 * @brief   Values that represent uC event to be sent to CPU.
*/
typedef enum
{
   IPA_UC_2_CPU_EVENT_NO_OP                           = 0x0,   /**< No event present  */
   IPA_UC_2_CPU_EVENT_ERROR                           = 0x1,   /**< Event specify a system error is detected by the device */
   IPA_UC_2_CPU_EVENT_MHI_CHANNEL_ERROR               = 0x2,   /**< Event specify the device detected an error in an element from the transfer ring associated with the channel */
   IPA_UC_2_CPU_EVENT_MHI_CHANNEL_WAKE_UP_REQUEST     = 0x3,   /**< Event specify a bam interrupt was asserted when MHI engine is suspended*/
   IPA_UC_2_CPU_EVENT_INVALID                         = 0xFF   /**< Invalid event. Shall not be in use in operational scenario  */
}IPA_UC_2_CPU_EVENTS;

/**
 * @brief   SW flags define the behavior of uC.
*/
typedef enum
{
   IPA_UC_FLAG_HALT_SYSTEM_ON_ASSERT_FAILURE          = 0x01,  /**< Halt system in case of assert failure */
   IPA_UC_FLAG_NO_REPORT_MHI_CHANNEL_ERORR            = 0x02,	/**< Channel error would be reported in the event ring only. No event to CPU */
   IPA_UC_FLAG_NO_REPORT_MHI_CHANNEL_WAKE_UP          = 0x04, 	/**< No need to report event IPA_UC_2_CPU_EVENT_MHI_WAKE_UP_REQUEST.  */
   IPA_UC_FLAG_WORK_OVER_DDR				          = 0x08 	/**< Perform all transaction to external addresses by QMB (avoid memcpy)  */
}IPA_UC_FLAGS;


/**
 * @brief   Channel error types.
*/
typedef enum
{
   IPA_UC_CHANNEL_ERROR_NONE, 	         /**< No error persists.  */
   IPA_UC_CHANNEL_INVALID_RE_ERROR 	      /**< Invalid Ring Element was detected  */
}IPA_UC_CHANNEL_ERRORS;


/**
 * @brief   MHI error types.
*/
typedef enum
{
   IPA_UC_ERROR_NONE, 	            /**< No error persists  */
   IPA_UC_INVALID_DOORBELL_ERROR,	/**< Invalid data read from doorbell */
   IPA_UC_INVALID_MMIO_ERROR,	      /**< Invalid data read from MMIO space */
   IPA_UC_INVALID_CHANNEL_ERROR,	   /**< Invalid data read from channel context array */
   IPA_UC_INVALID_EVENT_ERROR,	   /**< Invalid data read from event ring context array */
   IPA_UC_DMA_ERROR,	               /**< Unexpected DMA error */
   IPA_UC_NO_ED_IN_RING_ERROR,	   /**< No event descriptors are available to report on secondary event ring */
   IPA_UC_FATAL_SYSTEM_ERROR,	      /**< uC SW has crashed and requires reset. */
   IPA_UC_LINK_ERROR, 	            /**< Link error */
   IPA_UC_INVALID_OPCODE 	            /**< Invalid opcode sent */
}IPA_UC_ERRORS;

/**
 * @brief   uC warning types. Use prime number as enumerator values to allow extracting information from warning counter
*/
typedef enum
{
   IPA_UC_WARN_NONE, 	                     /**< No warning persists  */
   IPA_UC_INVALID_CPU_REQUEST_WARN    = 3,   /**< Invalid cpu command or wrong timing for the command*/
   IPA_UC_INVALID_FSM_TRANSATION_WARN = 5,	/**< Unexpected transaction in internal final state machine */
   IPA_UC_INVALID_QUEUE_OVERFLOW_WARN = 7 	/**< Unexpected overflow in internal queue lead to discarding an element */
}IPA_UC_WARNINGS;

#pragma pack(push,1) /*Save previous, and turn on 1 byte alignment*/

/**
 * @brief   128B shared memory located in offset zero of uC DRAM. The shared memory is used for communication between uC and CPU
*/
typedef PACK(struct) IpaUcSharedMemeMapping_t
{
   /*word 0 */
   uint8_t  cmdOp;         	/**< CPU->uC command opcode. See IPA_CPU_2_UC_COMMANDS */
   uint8_t  reserved_02_01;
   uint16_t reserved_04_02;
   /*word 1 */
   uint32_t cmdParams;        /**< CPU->uC command parameter. The parameter field can hold 32 bits of parameters (immediate parameters) and point on structure in system memory (in such case the address must be accessible for uC) */
   /*word 2 */
   uint8_t  responseOp;       /**< uC->CPU response opcode. See IPA_UC_2_CPU_RESPONSES */
   uint8_t  reserved_0A_09;
   uint16_t reserved_0C_0A;
   /*word 3 */
   uint32_t responseParams;   /**< uC->CPU response parameter. The parameter field can hold 32 bits of parameters (immediate parameters) and point on structure in system memory*/
   /*word 4 */
   uint8_t  eventOp;          /**< uC->CPU event opcode. See IPA_UC_2_CPU_EVENTS */
   uint8_t  reserved_12_11;
   uint16_t reserved_14_12;
   /*word 5 */
   uint32_t eventParams;      /**< uC->CPU event parameter. The parameter field can hold 32 bits of parameters (immediate parameters) and point on structure in system memory*/
   /*word 6 */
   uint32_t reserved_1C_18;
   /*word 7 */
   uint16_t interfaceVersion; /**< The uc/CPU interface version as reported by uC*/
   uint16_t ucVersion;     	/**< The uc FW version */
   /*word 8 */
   uint8_t mhiCnl0State;      /**< State of MHI channel X. The state carries information regarding the error type. See IPA_UC_MHI_CHANNEL_STATES */
   uint8_t mhiCnl1State;
   uint8_t mhiCnl2State;
   uint8_t mhiCnl3State;
   /*word 9 */
   uint8_t mhiCnl4State;
   uint8_t mhiCnl5State;
   uint8_t mhiCnl6State;
   uint8_t mhiCnl7State;
   /*word 10 */
   uint32_t firstErrorAddress; 	/**< Contains the address of first error-source on SNOC */
   /*word 11 */
   uint8_t  ucState;            /**< State of uC. The state carries information regarding the error type. See IPA_UC_MHI_ERRORS */
   uint8_t  warningCounter;	  /**< The warnings counter. The counter carries information regarding non fatal errors in uC */
   uint16_t reserved_30_2E;
   /*word 12 */
   uint32_t reserved_34_30;
   /*word 13 */
   uint32_t reserved_38_34;
   /*word 14 */
   uint32_t reserved_3C_38;
   /*word 15 */
   uint32_t reserved_40_3C;
}IpaUcSharedMemeMapping_t;


/*CPU->uC commands*/

/**
 * @brief   Structure holding the parameters for IPA_CPU_2_UC_CMD_MHI_INIT command.
 *          Parameters are sent as pointer thus should be reside in address accessible to uC
*/
typedef PACK(struct) IpaUcMhiInitCmdData_t
{
   uint32_t msiAddress;          	   /**< The MSI base (in device space) used for asserting the interrupt (MSI) associated with the event ring.*/
   uint32_t mmioBaseAddress;           /**< The address   (in device space) of MMIO structure in host space*/
   uint32_t deviceMhiCtrlBaseAddress;  /**< Base address of the memory region in the device address space where the MHI control data structures are allocated by the host, including channel context array, event context array, and rings. This value is used for host/device address translation */
   uint32_t deviceMhiDataBaseAddress;  /**< Base address of the memory region in the device address space where the MHI data buffers are allocated by the host. This value is used for host/device address translation */
   uint32_t firstChannelIndex;  	   /**< First channel ID. Doorbell 0 is mapped to this channel. */
   uint32_t firstEventRingIndex;  	   /**< First event ring ID. Doorbell 16 is mapped to this event ring. */
}IpaUcMhiInitCmdData_t;

/**
 * @brief   Structure holding the parameters for IPA_CPU_2_UC_CMD_MHI_INIT_CHANNEL command.
 *          Parameters are sent as 32b immediate parameters.
*/
typedef PACK(union) IpaUcMhiInitChannelCmdData_t
{
   PACK(struct) IpaUcMhiInitChannelCmdParams_t
   {
      uint32_t channelHandle     :8;   	/**< The channel identifier as allocated by driver. value is within the range 0 to IPA_UC_MAX_CHANNEL_HANDLE*/
      uint32_t contexArrayIndex  :8;      /**< Unique index for channels, between 0 and 255. The index is used as an index in channel context array structures. */
      uint32_t bamPipeId         :6;      /**< The BAM pipe number for pipe dedicated for this channel */
      uint32_t channelDirection  :2; 	   /**< The direction of the channel as defined in the channel type field (CHTYPE) in the channel context data structure.  */
      uint32_t reserved          :8;
   }params;
   uint32_t raw32b;
}IpaUcMhiInitChannelCmdData_t;

/**
 * @brief   Structure holding the parameters for IPA_CPU_2_UC_CMD_MHI_UPDATE_MSI command.
 *          Parameters are sent as 32b immediate parameters.
*/
typedef PACK(union) IpaUcMhiUpdateMsiCmdData_t
{
   PACK(struct) IpaUcMhiUpdateMsiCmdParams_t
   {
      uint32_t  msiAddress    :32;     /**< The MSI address (in device space) used for asserting the interrupt (MSI) associated with the event ring.*/
   }params;
   uint32_t raw32b;
}IpaUcMhiUpdateMsiCmdData_t;

/**
 * @brief   Structure holding the parameters for IPA_CPU_2_UC_CMD_MHI_CHANGE_CHANNEL_STATE command.
 *          Parameters are sent as 32b immediate parameters.
*/
typedef PACK(union) IpaUcMhiChangeChannelStateCmdData_t
{
   PACK(struct) IpaUcMhiChangeChannelStateCmdParams_t
   {
      uint32_t  requestedState    :8;  /**< The requested channel state as was indicated from Host. Use IPA_UC_MHI_CHANNEL_STATES to specify the requested state */
      uint32_t  channelHandle     :8;  /**< The channel identifier as allocated by driver. value is within the range 0 to IPA_UC_MAX_CHANNEL_HANDLE*/
      uint32_t  reserved          :16;
   }params;
   uint32_t raw32b;
}IpaUcMhiChangeChannelStateCmdData_t;

/**
 * @brief   Structure holding the parameters for IPA_CPU_2_UC_CMD_UPDATE_FLAGS command.
 *          Parameters are sent as 32b immediate parameters.
*/
typedef PACK(union) IpaUcUpdateFlagsCmdData_t
{
   PACK(struct) IpaUcUpdateFlagsCmdParams_t
   {
      uint32_t     newFlags;      /**< SW flags defined the behavior of uC. This field is expected to be used as bitmask for IPA_UC_FLAGS*/
   }params;
   uint32_t raw32b;
}IpaUcUpdateFlagsCmdData_t;

/*uC->Events responses*/

/**
 * @brief   Structure holding the parameters for IPA_UC_2_CPU_RESPONSE_MHI_CHANGE_CHANNEL_STATE response.
 *          Parameters are sent as 32b immediate parameters.
*/
typedef PACK(union) IpaUcMhiChangeChannelStateResponseData_t
{
   PACK(struct) IpaUcMhiChangeChannelStateResponseParams_t
   {
      uint32_t     state             :8;  /**< The new channel state. In case state is not as requested this is error indication for the last command*/
      uint32_t     channelHandle     :8;  /**< The channel identifier */
      uint32_t     additonalParams   :16; /**< For stop: the number of pending bam descriptors currently queued */
   }params;
   uint32_t raw32b;
}IpaUcMhiChangeChannelStateResponseData_t;

/**
 * @brief   Structure holding the parameters for IPA_UC_2_CPU_RESPONSE_CMD_COMPLETED response.
 *          Parameters are sent as 32b immediate parameters.
*/
typedef PACK(union) IpaUcCpuCmdCompletedResponseData_t
{
   PACK(struct) IpaUcCpuCmdCompletedResponseParams_t
   {
      uint32_t     originalCmdOp     :8;  /**< The original command opcode */
      uint32_t     success           :8;  /**< 0 for success indication, otherwise failure */
      uint32_t     reserved          :16;
   }params;
   uint32_t raw32b;
}IpaUcCpuCmdCompletedResponseData_t;

/*uC->Events commands*/

typedef PACK(union) IpaUcErrorEventData_t
{
   PACK(struct) IpaUcErrorEventParams_t
   {
      uint32_t     errorType :8;        /**< Entered when a system error is detected by the uC. Type of error is specified by IPA_UC_ERRORS*/
      uint32_t     reserved  :24;
   }params;
   uint32_t raw32b;
}IpaUcErrorEventData_t;

typedef PACK(union) IpaUcMhiChannelErrorEventData_t
{
   PACK(struct) IpaUcMhiChannelErrorEventParams_t
   {
      uint32_t     errorType      :8;      /**< Type of error - IPA_UC_CHANNEL_ERRORS*/
      uint32_t     channelHandle  :8;      /**< The channel identifier as allocated by driver. value is within the range 0 to IPA_UC_MAX_CHANNEL_HANDLE*/
      uint32_t     reserved       :16;
   }params;
   uint32_t raw32b;
}IpaUcMhiChannelErrorEventData_t;

typedef PACK(union) IpaUcMhiChannelWakeupEventData_t
{
   PACK(struct) IpaUcMhiChannelWakeupEventParams_t
   {
      uint32_t     channelHandle  :8;     /**< The channel identifier as allocated by driver. value is within the range 0 to IPA_UC_MAX_CHANNEL_HANDLE*/
      uint32_t     reserved       :24;
   }params;
   uint32_t raw32b;
}IpaUcMhiChannelWakeupEventData_t;

/**
 * @brief   Values that represent the common commands from CPU 
 *          to IPA HW. (for mhi3x image)
*/
typedef enum
{
   IPA_CPU_2_HW_CMD_NO_OP                     = 0,  	 /**< No operation is required.  */
   IPA_CPU_2_HW_CMD_UPDATE_FLAGS              = 1,    /**< Update SW flags which defines the behavior of HW . Once operation was completed HW shall respond with IPA_HW_2_CPU_RESPONSE_CMD_COMPLETED */
   IPA_CPU_2_HW_CMD_DEBUG_RUN_TEST            = 2,    /**< Launch predefined test over HW  */
   IPA_CPU_2_HW_CMD_DEBUG_GET_INFO            = 3,    /**< Read HW internal debug information  */
   IPA_CPU_2_HW_CMD_ERR_FATAL                 = 4,    /**< CPU instructs HW to perform error fatal handling  */
   IPA_CPU_2_HW_CMD_CLK_GATE                  = 5,    /**< CPU instructs HW to goto Clock Gated state  */ 
   IPA_CPU_2_HW_CMD_CLK_UNGATE                = 6,    /**< CPU instructs HW to goto Clock Ungated state  */ 
   IPA_CPU_2_HW_CMD_MEMCPY                    = 7,     /**< CPU instructs HW to do memcopy using QMB  */    
   IPA_CPU_2_HW_CMD_RESET_PIPE                = 8,    /**< Command to reset a pipe - SW WA for a HW bug */
   IPA_CPU_2_HW_CMD_REG_WRITE                 = 9,    /**< Command to write to a IPA register - SW WA for a HW bug */
}IPA_CPU_2_HW_COMMANDS;

/**
 * @brief   Values that represent common HW responses to CPU 
 *          commands. (for mhi3x image)
*/
typedef enum
{
   IPA_HW_2_CPU_RESPONSE_NO_OP                    = 0,
   IPA_HW_2_CPU_RESPONSE_INIT_COMPLETED           = 1,   /**< HW shall send this command once boot sequence is completed and HW is ready to serve commands from CPU */
   IPA_HW_2_CPU_RESPONSE_CMD_COMPLETED            = 2,   /**< Response to CPU commands */
   IPA_HW_2_CPU_RESPONSE_DEBUG_GET_INFO           = 3,  /**< Response to IPA_CPU_2_HW_CMD_DEBUG_GET_INFO command */
}IPA_HW_2_CPU_RESPONSES;

/**
 * @brief   128B shared memory located in offset zero of SW Partition in IPA SRAM. 
 * @note    The shared memory is used for communication between IPA HW and CPU. 
 *          This is an internal structure as seen by uC (for mhi3x image)
 *                                                                              
*/
typedef PACK(struct) IpaHwSharedMemeMapping_t
{
   /*word 0 */
   uint8_t  cmdOp;          /**< CPU->HW command opcode. See IPA_CPU_2_HW_COMMANDS */
   uint8_t  reserved_01;
   uint16_t reserved_03_02;
   /*word 1 & 2 */
   uint32_t cmdParams_lo;        /**< CPU->HW command parameter. The parameter filed can hold 32 bits of parameters (immediate parameters) and point on structure in system memory (in such case the address must be accessible for HW) */
   uint32_t cmdParams_hi;        /**< CPU->HW command parameter. The parameter filed can hold 32 bits of parameters (immediate parameters) and point on structure in system memory (in such case the address must be accessible for HW) */
   /*word 3 */
   uint8_t  responseOp;       /**< HW->CPU response opcode. See IPA_HW_2_CPU_RESPONSES */
   uint8_t  reserved_0D;
   uint16_t reserved_0F_0E;
   /*word 4 */
   uint32_t responseParams;   /**< HW->CPU response parameter. The parameter filed can hold 32 bits of parameters (immediate parameters) and point on structure in system memory*/
   /*word 5 */
   uint8_t  eventOp;          /**< HW->CPU event opcode. See IPA_HW_2_CPU_EVENTS */
   uint8_t  reserved_15;
   uint16_t reserved_17_16;   
   /*word 6 */
   uint32_t eventParams;      /**< HW->CPU event parameter. The parameter filed can hold 32 bits of parameters (immediate parameters) and point on structure in system memory*/
   /*word 7 */
   uint32_t firstErrorAddress;  /**< Contains the address of first error-source on SNOC */
   /*word 8 */
   uint8_t  hwState;            /**< State of HW. The state carries information regarding the error type. See IPA_HW_MHI_ERRORS */
   uint8_t  warningCounter;     /**< The warnings counter. The counter carries information regarding non fatal errors in HW */
   uint16_t reserved_23_22;
   /*word 9 */
   uint16_t interfaceVersionCommon; /**< The Common interface version as reported by HW*/
   uint16_t reserved_27_26; 	  
   /*word 10 */
   uint16_t interfaceVersionMhi;   /**< The MHI interface version as reported by HW*/  	  
   uint8_t  mhiState;              /**< Overall MHI state */  	  
   uint8_t  reserved_2B;
   /*word 11 */
   uint8_t mhiCnl0State;      /**< State of MHI channel X. The state carries information regarding the error type. See IPA_HW_MHI_CHANNEL_STATES */
   uint8_t mhiCnl1State;
   uint8_t mhiCnl2State;
   uint8_t mhiCnl3State;
   /*word 12 */
   uint8_t mhiCnl4State;
   uint8_t mhiCnl5State;
   uint8_t mhiCnl6State;
   uint8_t mhiCnl7State;
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
   uint8_t  wdi_tx_ch_0_state;
   uint8_t  wdi_rx_ch_0_state;
   uint16_t reserved_47_46;
}IpaHwSharedMemeMapping_t;

/**
 * @brief   Structure holding the ZIP Init parameters passed through doorbell_0_18.
 *          Parameters are sent as pointer thus should reside in address accessible to HW
*/
typedef struct
{
   uint8_t  hpq_cnsmr_pipe_number;    /**< IPA pipe # to be used for imm cmds injected from High priority Q */
   uint8_t  lpq_cnsmr_pipe_number;    /**< IPA pipe # to be used for imm cmds injected from Low priority Q */
   uint8_t  hpq_prdcr_pipe_number;        /**< IPA pipe # where ZIP requests from hpq cnsmr pipe will be sent */
   uint8_t  lpq_prdcr_pipe_number;        /**< IPA pipe # where ZIP requests from hpq cnsmr pipe will be sent */

   uint32_t hpq_cnsmr_bam_fifo_base_addr_ofst;  /**< Address offset of HPQ consumer Bam Fifo base in the ZIP Shared Mem Region */
   uint32_t lpq_cnsmr_bam_fifo_base_addr_ofst;  /**< Address offset of LPQ consumer Bam Fifo base in the ZIP Shared Mem Region */

   uint32_t hpq_prdcr_bam_fifo_base_addr_ofst;  /**< Address offset of HPQ producer Bam Fifo base in the ZIP Shared Mem Region */
   uint32_t lpq_prdcr_bam_fifo_base_addr_ofst;  /**< Address offset of LPQ producer Bam Fifo base in the ZIP Shared Mem Region */

   uint16_t hpq_cnsmr_bam_fifo_size;        /**< HPQ consumer Bam Fifo Size in elements */
   uint16_t lpq_cnsmr_bam_fifo_size;        /**< LPQ consumer Bam fifo Size in elements */

   uint16_t hpq_prdcr_bam_fifo_size;        /**< HPQ producer Bam Fifo Size in elements */
   uint16_t lpq_prdcr_bam_fifo_size;        /**< LPQ producer Bam fifo Size in elements */

   uint16_t lpq_accm_timer_val;           /**< LPQ delayed injection timer value in IPA Clock ticks */
   uint8_t  pkt_status_enabled ;          /**< Flag to indicate if HWP should generate a status packet upon TX of a zip request 
                                               0 - Not Enabled, 1 - Enabled */
   uint8_t irq_coal_timer_val;            /**< Value is in Sleep Clock Cycles(~32Khz). Valid values 1 - 255
                                               e.g: Value of 2 will run a coal timer of 62.5us */

}IpaHwZipInitCmdData_t;

/**
 * @brief   Flags of uC ZIP transfer element. 
*/
typedef enum 
{
   IPAIF_DESC_FLAG_NONE     = 0x00,
   IPAIF_DESC_FLAG_EOT      = 0x40, /**< Generate End-Of-Transfer Notification. */
   IPAIF_DESC_FLAG_INT      = 0x80, /**< Generate notification for the frame.   */
   IPAIF_DESC_FLAG_IMM_CMD  = 0x01, /**< Flag to indicate Immediate Command     */
} IPAIF_DESC_FLAGS;

/**
 * @brief   Element of uC ZIP transfer ring. 
*/
typedef struct IPAIF_Desc_t
{
   uint32_t ptr;              /**< pointer for buffer holding the data */
   uint32_t size     :16;     /**< size in bytes of transfer */
   uint32_t reserved :8;
   uint32_t flags    :8;      /**< enumerator IPAIF_DESC_FLAGS can be used for masking bits from this field */
} IPAIF_Desc_t;

#pragma pack(pop) /* Revert alignment to what it was previously*/

#endif /* IPA_UC_INTERFACE_H_ */
