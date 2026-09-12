 /*!
  @file
  rfcmd_msg.h

  @brief
  Generic interface RF commands to FW

*/

/*===========================================================================

  Copyright (c) 2012-2015 Qualcomm Technologies Incorporated. All Rights Reserved

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

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.
 
$Header: //source/qcom/qct/modem/fw/components/fw_common.mpss/rel/17.0/api/rfcmd_msg.h#1 $
 
when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/15/13   SYH     Add parameters in txagc override payload for immediate txagc override
                   support
05/22/13   BW      Echo back ASD script token to RFSW through RFCMD message
11/08/12   SYH     Add txlm static setting buffer id to get dm computing parameters 
11/07/12   SYH     Add txagc delay parameter for computing all DM blocks
10/30/12   nbb     Request TQ pair instead of individual TQ 
10/18/12   nbb     Sample capture response messages
09/28/12   nbb     Reinclude fw_rf_irat_intf.h
08/17/12   nbb     Added TXAGC override command
08/15/12   nbb     Increase bitwidth of start_addr and num_words in TXC_TABLE_WRITE
08/01/12   nbb     Add response for free TXC banks
07/30/12   nbb     Messages for request/ free TQ from RFSW
07/19/12   nbb     Added support for event interface and DPD load table
06/15/12   nbb     Initial version with dummy messages
===========================================================================*/

#ifndef RFCMD_MSG_H
#define RFCMD_MSG_H

#include "msgr.h"
#include "fw_rf_common_intf.h"

/*===========================================================================

      Macros

===========================================================================*/
/*! @brief Macro containing header and payload */
#define RFCMD_INTF_REQ_MSG( MSG_NAME )                 \
typedef struct {                                   \
  msgr_hdr_s                        msg_hdr;       \
  rfcmd_##MSG_NAME##_payload        msg_payload;   \
} rfcmd_##MSG_NAME##_t

/*! @brief Macro containing header and response */
#define RFCMD_INTF_RSP_MSG( MSG_NAME )                 \
typedef struct {                                   \
  msgr_hdr_s                        msg_hdr;       \
  rfcmd_##MSG_NAME##_rsp            msg_rsp;   \
} rfcmd_##MSG_NAME##_rsp_t


/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/
/*! @brief RFA msgr with FW module */
  /* XXX --- confirm that no L1 uses 0x80 */
#define MSGR_RFA_FW         MSGR_TECH_MODULE( MSGR_TECH_RFA, 0x80 )

/*! @brief Structure for setting FTM mode */
typedef struct
{
  uint32 ftm_mode;
} rfcmd_set_ftm_mode_payload;

RFCMD_INTF_REQ_MSG(set_ftm_mode);

/*! @brief Structure for Shutting RF thread */
typedef struct
{
  uint32 shutdown;
} rfcmd_shutdown_payload;

RFCMD_INTF_REQ_MSG(shutdown);

/*! @brief LABELS of INVALID ARGUMENTS */
#define DP_ENV_SCALE_INVALID 0xFFFFFFFF
#define DP_IQ_GAIN_INVALID 0xFFFFFFFF
#define BANK_ID_INVALID 255
#define TXAGC_MAX_DELAY_ABS 1000

/*! @brief Enum for Power tracking mode */
typedef enum 
{
  /*! @brief APT mode */
  RFCMD_XPT_MODE_APT,
  /*! @brief EPT (DPD) mode */
  RFCMD_XPT_MODE_EPT,
  /*! @brief Envelope tracking mode */
  RFCMD_XPT_MODE_ET,
  /*! @brief INVALID power tracking mode */
  RFCMD_XPT_MODE_INVALID = -1
} rfcmd_xpt_mode_e;

/*! @brief Table size for AMAM/ AMPM/ EPT can be 
  16, 32, 64, 128, 256 32-bit entries */
typedef enum
{
  RFCMD_XPT_BANK_256_WORDS_FVAL,
  RFCMD_XPT_BANK_128_WORDS_FVAL,
  RFCMD_XPT_BANK_64_WORDS_FVAL,
  RFCMD_XPT_BANK_32_WORDS_FVAL,
  RFCMD_XPT_BANK_16_WORDS_FVAL,
  RFCMD_XPT_BANK_NUM_SIZES
} rfcmd_xpt_bank_size_e;

/*! @brief Structure for sending size/bank/base for AMAM/ AMPM/ EPT to FW */
typedef struct
{
  /*! @brief Base addr (offset within bank) */
  uint16 base;
  /*! @brief Number of 16bit writes in table (this is per HWIO) */
  rfcmd_xpt_bank_size_e bank_size;
  /*! @brief Bank ID */
  uint8 bank_id;
} rfcmd_xpt_table_info_t;

/*! @brief Structure for ADC VCM CAL */
typedef struct
{
  /*! @brief RxLM buffer ID */
  uint32 buf_id;
  /*! @brief Capture time in usec */
  uint32 capture_time_us;
  /*! @brief Sequence number */
  uint32 snum;
} rfcmd_adc_vcm_cal_payload;

RFCMD_INTF_REQ_MSG(adc_vcm_cal);

/*! @brief Structure to send processing done for ADC VCM CAL */
typedef struct
{
  /*! @brief TRUE means ADC_VCM_CAL success */
  uint32 adc_vcm_cal_success;
  /*! @brief Sequence number given by RFSW */
  uint32 snum;
  /*! @brief WBDC_EST2_CURR_I value */
  uint32 wbdc_est2_curr_i;
  /*! @brief WBDC_EST2_CURR_Q value */
  uint32 wbdc_est2_curr_q;
} rfcmd_adc_vcm_cal_done_rsp;

RFCMD_INTF_RSP_MSG(adc_vcm_cal_done);


/* Sample capture message is sent to each tech separately today
   (Please refer to common structure to be sent in SAPT command
   in fw_dpd_intf.h). Only the results are sent back via
   a common message to RF SW */

/*! @brief Structure to send capture done for a single capture */
typedef struct
{
  /*! @brief ID of capture */
  uint32 seq_id;
  /*! @brief Buffer ID for capture done */
  uint8 capt_buf_id;
} rfcmd_sample_capt_done_rsp;

RFCMD_INTF_RSP_MSG(sample_capt_done);

/*! @brief Structure to send processing done for a single capture */
typedef struct
{
  /*! @brief ID of capture */
  uint32 seq_id;
  /*! @brief Buffer ID for capture done, indicates which capture buffer produced results 
    XXX - Can be removed if required */
  uint8 capt_buf_id;
  /*! @brief Buffer ID for processing done */
  uint8 proc_buf_id;
} rfcmd_xpt_cal_proc_done_rsp;

RFCMD_INTF_RSP_MSG(xpt_cal_proc_done);


/*! @brief Structure to send processing done for all captures */
/* MSGR enum */
enum {
  /* Command messages */
  MSGR_DEFINE_UMID( RFA, FW, CMD,  SET_FTM_MODE,      0x00, rfcmd_set_ftm_mode_t  ),
  MSGR_DEFINE_UMID( RFA, FW, CMD,  ADC_VCM_CAL,       0x08, rfcmd_adc_vcm_cal_t ),

  /* Internal commands */
  MSGR_DEFINE_UMID( RFA, FW, CMDI, SHUTDOWN,          0x00, rfcmd_shutdown_t    ),

  /* Response messages */
  MSGR_DEFINE_UMID( RFA, FW, RSP,  SAMPLE_CAPT_DONE,  0x05, rfcmd_sample_capt_done_rsp_t ),
  MSGR_DEFINE_UMID( RFA, FW, RSP,  ADC_VCM_CAL_DONE,  0x0B, rfcmd_adc_vam_cal_done_rsp_t ),
  MSGR_DEFINE_UMID( RFA, FW, RSP,  XPT_CAL_PROC_DONE, 0x06, rfcmd_xpt_cal_proc_done_rsp_t )
};


#endif // RFCMD_H
