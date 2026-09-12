#ifndef VIDRV_WDI_3_0_H
#define VIDRV_WDI_3_0_H

/**---------------------------------------------------------------------------
 * @file     VIDRV_wdi_3_0.h
 *
 * @brief    This is the interface of WDI 3.0 driver.
 *           The driver attempts to simulate WDI related SW and FW.
 * 
 * Copyright (c) 2016 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/* -----------------------------------------------------------------------
**                          INCLUDES
** ----------------------------------------------------------------------- */
#include "VIDRV_ipa.h"

/* -----------------------------------------------------------------------
**                           DEFINES
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
**                           TYPES
** ----------------------------------------------------------------------- */

/**
 * @brief   WDI 3.0 Channel context handle
*/
typedef struct WDI_chCntxt *WDI_chCntxtHandle;

/**
* @brief   WDI 3.0 Channel configuration
*/
typedef struct WDI_Config *WDI_chCnConfigHandle;
typedef struct WDI_Config
{
   /* input parameters */
   uint32                 numElements;                     /**< number of ring elements associated with the WDI channel */
   uint32                 channelModerationThreshold : 5;  /**< number of packets that IPA will process before Wifi transfer ring RP will be updated */
   uint32                 eventModerationConfig : 8;       /**< duration of the timer for update event ring RP moderation in mSec */
   uint32                 qmuxId : 8;                      /**< Rx only, used for setting metadata register in IPA */
} WDI_chCnConfig;

/**
* @brief   WDI 3.0 metadata layout as GSI FW writes to IPA metadata register
*/
typedef union
{
   uint32 raw;
   struct
   {
      uint32 vap_id     : 3;
      uint32 da_is_mcbc : 1;
      uint32 reserved   : 12;
      uint32 qmux_id    : 16;
   } metadata;
} WDI_metadata;


/* -----------------------------------------------------------------------
**                           INTERFACES
** ----------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Initialize a WDI3.0 channel, buffers and internal data.
 *
 * @param  [in] dir                WDI channel direction: consumer or producer
 * @param  [in] configHandle       WDI channel configuration pointer
 *
 * @return  pointer to the newly created channel context, null on failure.
*/
WDI_chCntxtHandle VIDRV_WDI_init(BAM_DRV_PipeDir dir, const WDI_chCnConfigHandle configHandle);

/**
 * @brief  Cleanup a WDI3.0 channel, buffers and internal data.
 *
 * @param  [in] chCntxt      channel context
 *
 * @return  None.
*/
void VIDRV_WDI_cleanup(WDI_chCntxtHandle chCntxtHandle);

/**
 * @brief   Transfer data on a WDI channel.
 *
 * @param  [in] chCntxt          channel context
 * @param  [in] numElements      number of elements to transfer
 * @param  [in] buffers          pointer to WDI buffers
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType VIDRV_WDI_transfer(WDI_chCntxtHandle chCntxtHandle,
                                        VIDRV_ipa_BufferType* buffers,
                                        uint32 numElements);

/**
 * @brief   validate channel related data.
 *
 * @param  [in] chCntxt      channel context
 * @param  [in] outBuffers   output buffers to use in validation
 *
 * @return  TRUE if validation passed, FALSE if failed.
*/
boolean VIDRV_WDI_validate(const WDI_chCntxtHandle chCntxtHandle, VIDRV_ipa_BufferType* outBuffers);

/**
 * @brief   getter function for WDI channel handle.
 *
 * @param  [in] wdiCntxtHandle  WDI channel handle
 *
 * @return  handle to the WDI endpoint
*/
VIDRV_ipa_PipeHandle VIDRV_WDI_getWdiEndpoint(WDI_chCntxtHandle wdiCntxtHandle);

/**
 * @brief   reset WDI channel.
 *
 * @param  [in] wdiCntxtHandle  WDI channel context
 *
 * @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType VIDRV_WDI_reset(WDI_chCntxtHandle wdiCntxtHandle);

/**
* @brief   update qmux ID in channel scratch.
*
* @param  [in] chCntxtHandle  WDI channel context
* @param  [in] qmuxId  qmux ID
*
* @return  VIDRV_IPA_SUCCESS on success, else failed.
*/
VIDRV_ipa_ResultType VIDRV_WDI_updateQmuxId(WDI_chCntxtHandle chCntxtHandle, uint32 qmuxId);

#ifdef __cplusplus
}
#endif

#endif /* VIDRV_WDI_3_0_H */
