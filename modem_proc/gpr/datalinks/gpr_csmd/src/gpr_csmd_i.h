#ifndef _GPR_CSMD_I_H_
#define _GPR_CSMD_I_H_
/*=============================================================================
Copyright (c) 2018-2020 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.

===============================================================================
                                 Edit History

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //source/qcom/qct/multimedia2/gpr/gpr2/mainline/main/latest/datalink/gpr_csmd/inc/gpr_csmd.h
  $Author: mplcsds1 $

when         who    what, where, why
----------   ---   ------------------------------------------------------------
4/11/18     akr      Created
=============================================================================*/

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include "gpr_comdef.h"
#include "ipc_dl_api.h"
#include "ar_osal_error.h"
#include "ar_types.h"
/******************************************************************************
 * Defines                                                                    *
 *****************************************************************************/
#define GPR_CSMD_MAX_PORTS (GPR_PL_NUM_TOTAL_DOMAINS_V)
/**< Max number of GPR CSMD ports. */

#define GPR_CSMD_MAX_CHANNEL_NAME_SIZE (16)
/**< Max channel name size allowed. */

/*TODO: Assign guids */
#define GPR_CSMD_DISCONNECTED (0)
/**< GPR csmd connection is disconnected. */

#define GPR_CSMD_CONNECTED (1)
/**< GPR csmd connection is fully connected with remote proc. */

/* Names of subsystems */
#define GPR_REMOTE_SS_MODEM ("mpss")
#define GPR_REMOTE_SS_APPS ("apss")
#define GPR_REMOTE_SS_LPASS ("lpass")

/* Unique Names for subsystem connections */
#define GPR_IPC_TO_MODEM_PORT_NAME ("to_modem")
#define GPR_IPC_TO_DSP_PORT_NAME ("to_dsp")
#define GPR_IPC_TO_APPS_PORT_NAME ("to_apps")

/******************************************************************************
 * Type Definitions                                                           *
 *****************************************************************************/
/* GPR CSMD port info struct*/
typedef struct gpr_csmd_port_info_t gpr_csmd_port_info_t;

struct gpr_csmd_port_info_t
{
   uint16_t domain_id;
   char_t * remote_ss;
   char_t * port_name;
   int32_t  index;
};

/* CSMD related port data for all domains */
static gpr_csmd_port_info_t gpr_csmd_port_data[GPR_PL_NUM_TOTAL_DOMAINS_V] = {
   { GPR_IDS_DOMAIN_ID_INVALID_V, "TBR", GPR_IPC_TO_DSP_PORT_NAME },
   { GPR_IDS_DOMAIN_ID_MODEM_V, GPR_REMOTE_SS_MODEM, GPR_IPC_TO_MODEM_PORT_NAME },
   { GPR_IDS_DOMAIN_ID_ADSP_V, GPR_REMOTE_SS_LPASS, GPR_IPC_TO_DSP_PORT_NAME },
   { GPR_IDS_DOMAIN_ID_APPS_V, GPR_REMOTE_SS_APPS, GPR_IPC_TO_APPS_PORT_NAME },
};

/******************************************************************************
 * Core Routines                                                              *
 *****************************************************************************/
/**
 * Send data to the specified GPR CSMD port.
 *
 * \param[in] port_handle The handle of the port to write data.
 * \param[in] buf The data to be written.
 * \param[in] size The size of the data in bytes.
 *
 * \return GPR_EOK on success, GPR error code on failure.
 */
uint32_t gpr_csmd_tx(uint32_t domain_id, void *buf, uint32_t size);

/**
 * Indicate GPR CSMD that a previously given Rx buffer is done.
 *
 * \param[in] port_handle The handle of the port for which rx buffer is done.
 * \param[in] buf The rx buffer pointer for which rx is done.
 *
 * \return GPR_EOK on succes, GPR error code on failure.
 */
uint32_t gpr_csmd_rx_done(uint32_t domain_id, void *buf);

#endif /* _GPR_CSMD_I_H_ */
