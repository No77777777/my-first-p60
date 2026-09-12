#ifndef VIDRV_MHI_H
#define VIDRV_MHI_H

/**---------------------------------------------------------------------------
 * @file     VIDRV_mhi.h
 *
 * @brief    This is the interface of MHI VI driver
 * 
 * Copyright (c) 2012, 2013 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/* -----------------------------------------------------------------------
**                          INCLUDES
** ----------------------------------------------------------------------- */
#include "HALcomdef.h"
#include "VIDRV_ipa.h"
#include "ipa_customer.h"

#ifdef __cplusplus
extern "C" {
#endif

/*those defintions are part of C99 but required for ipa_uc_interface.h file*/

typedef  uint64             uint64_t;     /* Unsigned 64 bit value */
#ifndef _STDINT /* TODO: find more elegant way to fix MSS compilation */
typedef  uint32             uint32_t;     /* Unsigned 32 bit value */
#endif
typedef  uint16             uint16_t;     /* Unsigned 16 bit value */
typedef  uint8              uint8_t;      /* Unsigned 8 bit value */

#include "ipa_uc_interface.h"

/**
 * @brief   The maximum number of channel according to MHI spec. 
*/
#define VIDRV_MHI_NUM_CHANNELS                    IPA_UC_MAX_NUMBER_OF_CHANNELS

/* -----------------------------------------------------------------------
**                           TYPES
** ----------------------------------------------------------------------- */

/**
 * @brief   The IPA pipe structure. 
*/
typedef struct VIDRV_mhi_ChannelInfo
{
   VIDRV_ipa_PipeHandle hPipe;   /* IPA endpoint handle*/
   uint8 hChannel;               /* Channel handle in the range 0 - (VIDRV_MHI_NUM_CHANNELS-1)*/
   uint8 channelIndex;           /* channel index in channel context array*/
   uint16 pipeNum;               /* BAM pipe id*/
} VIDRV_mhi_ChannelInfo;

/* Handle of a channel */
typedef struct VIDRV_mhi_ChannelInfo*   VIDRV_mhi_ChannelHandle;          


/**
 * @brief   Values that represent the possible error types for a IPA driver. 
*/
typedef enum
{
  VIDRV_MHI_OUT_CHAHNNEL = 1,
  VIDRV_MHI_IN_CHAHNNEL =  2,
} VIDRV_mhi_ChannelDirectionType;

/**
 * @brief   Parameters for configuring the MHI engine. 
 * @sa VIDRV_mhi_Init
*/
typedef struct VIDRV_mhi_InitParamsType
{
   uint32 msiAddress;          	    /**< The MSI base (in device space) used for asserting the interrupt (MSI) associated with the event ring.*/
   uint32 mmioBaseAddress;           /**< The address (in device space) of MMIO structure in host space*/
   uint32 deviceMhiCtrlBaseAddress;  /**< Base address of the memory region in the device address space where the MHI control data structures are allocated by the host, including channel context array, event context array, and rings. This value is used for host/device address translation */
   uint32 deviceMhiDataBaseAddress;  /**< Base address of the memory region in the device address space where the MHI data buffers are allocated by the host. This value is used for host/device address translation */
   uint32_t firstChannelIndex;  	   /**< First channel ID. Doorbell 0 is mapped to this channel. */
   uint32_t firstEventRingIndex;  	   /**< First event ring ID. Doorbell 16 is mapped to this event ring. */
   boolean loadCode;                 /**< TRUE - load uC code during initialization sequence, FALSE - skip code loading */
   boolean workInDdr;                /**< TRUE - mmio space resides in DDR, FALSE - mmio space resides in PCIe address */
}VIDRV_mhi_InitParamsType;

/**
 * @brief   Enumeration for the different priority uC ZIP Queues
*/
typedef enum
{
  ZIP_HPQ        = 0x0,  /**< High Priority Q */
  ZIP_LPQ        = 0x1,  /**< Low Priority Q */
  ZIP_MAX_Q_CNT
}zip_q_type_e;

/**
 * @brief   Enumeration for the different priority uC ZIP Queues
*/
typedef enum
{
   IPA_ZIP_HP_CONSUMER,
   IPA_ZIP_HP_PRODUCER,
   IPA_ZIP_LP_CONSUMER,
   IPA_ZIP_LP_PRODUCER,
   IPA_ZIP_NUM_OF_PIPES
}IpaZipPipesType;

/**
 * @brief   Parameters of uC ZIP interface
*/
typedef struct IpaZipUcInterfaceStruct
{
   IpaHwZipInitCmdData_t*  initParams;
   uint32                  ringSizes[IPA_ZIP_NUM_OF_PIPES];
   uint32*                 readIndexes[IPA_ZIP_NUM_OF_PIPES];
   uint32*                 writeIndexes[IPA_ZIP_NUM_OF_PIPES];
   void*                   ringBuffers[IPA_ZIP_NUM_OF_PIPES];
}IpaZipUcInterfaceType;

/* -----------------------------------------------------------------------
**               MHI INTERFACE
** ----------------------------------------------------------------------- */
 
/** @addtogroup MHI
 *
 *  The following functions describe the MHI interface of the IPA driver
 *
 *  @{
 */

/**
 * @brief   Initialize and start MHI engine. This interface is expected to be called once 
 *          after system initialization. 
 *
 * @param   params  Initialization parameters. See VIDRV_mhi_InitParamsType.
 *
 * 
 * @return  VIDRV_IPA_SUCCESS if successful, else failed.  
*/
VIDRV_ipa_ResultType VIDRV_mhi_Init(const VIDRV_mhi_InitParamsType* params);

/**
 * @brief   Tear down and release all resources allocated by MHI engine 
*/
void VIDRV_mhi_UnInit(void);

/**
 * @brief   Allocates a new channel for MHI usage.
 * 
 * @param   pipeNum     The pipe number. Let BAM_PIPE_NUMBER_AUTO as pipe number to get 1st available 
 * @param   direction   The direction. 
 * @param   contexArrayIndex   The index of channel in the context array. 
 *
 * @return  The channel handle. If the channel was already allocated function return NULL.
*/
VIDRV_mhi_ChannelHandle VIDRV_mhi_ChannelAlloc(uint32 pipeNum, BAM_DRV_PipeDir direction, uint32 contexArrayIndex);

/**
 * @brief   Release the channel and the corresponding descriptors and transfers. 
 *
 * @param   channelHandle  Handle of the pipe. 
*/
void VIDRV_mhi_ChannelFree(VIDRV_mhi_ChannelHandle channelHandle);

/**
 * @brief   Change MHI Channel state to running state. 
 *
 * @param   channelHandle  Handle of the channel identifying the channel. 
 *
 * @return     VIDRV_IPA_SUCCESS if successful, else failed.  
*/
VIDRV_ipa_ResultType VIDRV_mhi_ChannelResume(VIDRV_mhi_ChannelHandle channelHandle);

/**
 * @brief   Change MHI Channel state to suspended state. 
 *
 * @param   channelHandle  Handle of the channel identifying the channel. 
 *
 * @return   VIDRV_IPA_SUCCESS if successful, else failed.  
*/
VIDRV_ipa_ResultType VIDRV_mhi_ChannelSuspend(VIDRV_mhi_ChannelHandle channelHandle);

/**
 * @brief   Change MHI Channel state to stopped state. 
 *
 * @param   channelHandle  Handle of the channel identifying the channel. 
 *
 * @return   VIDRV_IPA_SUCCESS if successful, else failed.  
*/
VIDRV_ipa_ResultType VIDRV_mhi_ChannelStop(VIDRV_mhi_ChannelHandle channelHandle);

/**
 * @brief   Change MHI Channel state to enabled state. 
 *
 * @param   channelHandle  Handle of the channel identifying the channel. 
 *
 * @return   VIDRV_IPA_SUCCESS if successful, else failed.  
*/
VIDRV_ipa_ResultType VIDRV_mhi_ChannelReset(VIDRV_mhi_ChannelHandle channelHandle);

/**
 * @brief   Update msi vector. 
 *
 * @param   msiAddress  The MSI base (in device space) used for asserting the interrupt (MSI) associated with the event ring.
 *
 * @return   VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_mhi_UpdateMsiData(uint32 msiAddress);

/**
 * @brief   Update uC software flags. 
 *
 * @param   flags The new flags. 
 *
 * @return   VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_mhi_UpdateFlags(uint32 flags);

/**
 * @brief   Perform polling until specific response is sent by uC
 *
 * @param   ucResponse the type of response to wait for. 
 * @param   timeoutInMs time out in ms. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_mhi_PollingForResponse(IPA_UC_2_CPU_RESPONSES ucResponse, uint32 timeoutInMs);

/**
 * @brief   Perform polling until specific event is sent by uC
 *
 * @param   ucEvent the type of event to wait for. 
 * @param   timeoutInMs time out in ms. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_mhi_PollingForEvent(IPA_UC_2_CPU_EVENTS ucEvent, uint32 timeoutInMs);

/**
 * @brief   send command to uC
 *
 * @param   opcode the command opcode. 
 * @param   params command parameters. 
 * @param   triggerInterrupt indication whether need to send intterupt for the command. 
*/
void VIDRV_mhi_WriteCmd(IPA_CPU_2_UC_COMMANDS opcode, uint32 params, boolean triggerInterrupt);

/**
 * @brief   read response from uC
 *
 * @param   ucResponse [out] the response opcode. 
 * @param   params [out] the response parameters. 
*/
void VIDRV_mhi_ReadResponse(IPA_UC_2_CPU_RESPONSES *ucResponse, uint32 *params);

/**
 * @brief   Perform polling until specific response is sent by uC
 *
 * @note    The function works with updated uC interface,
 *          for more information look at definition of enum IPA_HW_2_CPU_RESPONSES 
 *          and IpaHwSharedMemeMapping_t struct.
 *
 * @param   ucResponse the type of response to wait for. 
 * @param   timeoutInMs time out in ms. 
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_mhi_PollingForResponseV3x(IPA_HW_2_CPU_RESPONSES ucResponse, uint32 timeoutInMs);

/**
 * @brief   Load uC software team image and start its execution. 
 *
 * @return  VIDRV_IPA_SUCCESS for success, otherwise for failure. 
*/
VIDRV_ipa_ResultType VIDRV_mhi_InitV3x(void);

/**
 * @brief   Initialize uC Q6ZIP WA
 *
 * @param   hpConsumer              High priority consumer pipe
 * @param   hpProducer              High priority producer pipe
 * @param   lpConsumer              Low priority consumer pipe
 * @param   lpProducer              Low priority producer pipe
 * @param   hpqConsumerRingSize     High priority consumer descriptor ring size
 * @param   hpqProducerRingSize     High priority producer descriptor ring size
 * @param   lpqConsumerRingSize     Low priority consumer descriptor ring size 
 * @param   lpqProducerRingSize     Low priority producer descriptor ring size 
 * @param   irqCoalTimerValue       The number of timer ticks to introduce delay on uC side
 *
 * @return  VIDRV_IPA_SUCCESS for success, otherwise for failure. 
*/
VIDRV_ipa_ResultType VIDRV_mhi_InitZipV3x( VIDRV_ipa_PipeHandle hpConsumer,
                                           VIDRV_ipa_PipeHandle hpProducer,
                                           VIDRV_ipa_PipeHandle lpConsumer,
                                           VIDRV_ipa_PipeHandle lpProducer,
                                           uint32 hpqConsumerRingSize,
                                           uint32 hpqProducerRingSize,
                                           uint32 lpqConsumerRingSize,
                                           uint32 lpqProducerRingSize,
                                           uint32 irqCoalTimerValue );

/**
 * @brief   Queue a transfer to/from uC Q6ZIP. 
 *
 * @param   hPipe                uC ZIP WA pipe index. 
 * @param [in,out]   bufferArray If non-null, array of buffers. 
 * @param   bufferArraySize      Size of the buffer array. 
 * @param   flags                The uC ZIP WA descriptor flags.
 *
 * @note    User shall not specify IPAIF_DESC_FLAG_EOT for producer pipe.
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_mhi_ZipQueueDescriptorsV3x(IpaZipPipesType pipe, VIDRV_ipa_BufferType *buffers, uint32 num, IPAIF_DESC_FLAGS flags);

/**
 * @brief   Start a transfer to/from uC Q6ZIP. 
 *
 * @param   mbox                pair index (high or low priority)
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
*/
VIDRV_ipa_ResultType VIDRV_mhi_ZipStartTransferV3x(zip_q_type_e mbox);

/**
 * @brief   Wait till the fifo is empty.
 *
 * @param   hPipe                uC ZIP WA pipe index.
 * @param   timeout              Timeout
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 */
VIDRV_ipa_ResultType VIDRV_mhi_ZipWaitUntilFifoEmptyV3x(IpaZipPipesType pipe, uint32 timeout);

/**
 * @brief   Wait till the end of the current transaction in the pipe.
 *
 * @param   timeout              Timeout
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 */
VIDRV_ipa_ResultType VIDRV_mhi_ZipWaitUcCompletionIrqV3x(uint32 timeout);

/**
 * @brief   Uninitialize uC Q6ZIP WA
 *
 * @return  VIDRV_IPA_SUCCESS if successful, else failed. 
 */
VIDRV_ipa_ResultType VIDRV_mhi_UnInitZipV3x(void);

/** @} */ /* Ends the MHI interface documentation */

#ifdef __cplusplus
}
#endif

#endif /* VIDRV_IPA_H */

