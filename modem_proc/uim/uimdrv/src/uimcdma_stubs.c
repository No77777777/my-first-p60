/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


              U I M   C D M A   S T A T E   M A C H I N E

GENERAL DESCRIPTION
  This module contains the state machine for the CDMA protocol of the UIM
  interface.

EXTERNALIZED FUNCTIONS

  uim_process_cdma_command
    Processes a generic UIM server command

  uim_send_cdma_err_rpt
    Processes an error condition for a cdma UIM server command

  uim_cdma_command_response
    This function processes the UIM responses from cdma APDUs.

Copyright (c) 2016 QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uimcdma_stubs.c#1 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/03/16   dd      Removed lint comments
02/01/16   ssr     Adding uimcdma stubs for IOE variant

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "target.h"
#include "customer.h"
#include "comdef.h"
#include "intconv.h"

#include "bit.h"
#include "uimdrv_msg.h"
#include "err.h"
#include "uimi.h"
#include "uimdrv.h"
#include "uimcdma.h"
#include "uimutil.h"
#include <memory.h>
#include "uimglobals.h"


/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/*===========================================================================

FUNCTION UIM_SEND_CDMA_ERR_RPT

DESCRIPTION
  This function sends an error report for a generic command that is being
  requested. We pass cmd_ptr explicitly here since we may be sending err
  report for am unprocessed command that was present in command queue while
  processing hotswap card removal or card error.

DEPENDENCIES
  This function queues an error report onto a client queue.

RETURN VALUE
  None

SIDE EFFECTS
  Releases the command buffer.

===========================================================================*/
void uim_send_cdma_err_rpt
(
  uim_cmd_type             *cmd_ptr, /* the command to queue up */
  uim_instance_global_type *uim_ptr
)
{
  if (cmd_ptr == NULL)
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id, "uim_send_cdma_err_rpt cmd_ptr is null");
    return;
  }

  /* We will reset the report options flag in the command to NONE for
     backward compatibility purposes */

  cmd_ptr->hdr.options = UIM_OPTION_NONE;

  /* Copy user data from received command to report buffer */
  uim_ptr->command.rpt_buf.user_data = cmd_ptr->hdr.user_data;
  /* Report back the status as UIM_FAIL indicating failure in the R-UIM */
  uim_ptr->command.rpt_buf.rpt_status = UIM_FAIL;

  /* Check if the client task has supplied a report back function. */
  if (cmd_ptr->hdr.rpt_function != NULL)
  {
     UIMDRV_MSG_HIGH_1(uim_ptr->id, "Reporting error for CDMA command 0x%x ",
                       cmd_ptr->hdr.command );
    /* Process the error report based on the command that failed. */
    switch(cmd_ptr->hdr.command)
    {
      case UIM_STORE_ESN_ME_F:              /* Store ESN_ME command */
      {
          uim_ptr->command.rpt_buf.rpt_type = UIM_STORE_ESN_ME_R;
      } /* end case - UIM_STORE_ESN_ME_F */
      break;

      case UIM_SSD_UPDATE_F:                /* Begin process to update SSD */
      {
         uim_ptr->command.rpt_buf.rpt_type = UIM_SSD_UPDATE_R;

      } /* end case - UIM_SSD_UPDATE_F */
      break;

      case UIM_CONFIRM_SSD_F:         /* Finish process to update SSD */
      {
        /* Did the client give a reportback functiom. */
          uim_ptr->command.rpt_buf.rpt_type = UIM_CONFIRM_SSD_R;
          /* Indicate the SSD update failed. */
          uim_ptr->command.rpt_buf.rpt.ssd_update_ok = FALSE;
      } /* end case - UIM_CONFIRM_SSD_F */
      break;

      case UIM_RUN_CAVE_F:                 /* Perform auth signature */
      {
        /* Report back auth signature, if requested */
          uim_ptr->command.rpt_buf.rpt_type = UIM_RUN_CAVE_R;
          uim_ptr->command.rpt_buf.rpt.run_cave.auth = 0;
      } /* end case - UIM_RUN_CAVE_F */
      break;

      case UIM_AKA_AUTH_F:          /* Perform AKA authentication */
      {
          uim_ptr->command.rpt_buf.rpt_type = UIM_AKA_AUTH_R;
      } /* end case - UIM_AKA_AUTH_F */
      break;

      case UIM_GENERATE_KEYS_F:              /* Generate CMEA key and VPM */
      {
        /* Report back the result, if requested */
          uim_ptr->command.rpt_buf.rpt_type = UIM_GENERATE_KEYS_R;
      } /* end case - UIM_GENERATE_KEYS_F */
      break;

      case UIM_BS_CHAL_F:                   /* Do a Base station Challenge */
      {
          uim_ptr->command.rpt_buf.rpt_type = UIM_BS_CHAL_R;
          uim_ptr->command.rpt_buf.rpt.bs_chal.randbs = 0;
      } /* end case - UIM_BS_CHAL_F */
      break;

      case UIM_COMMIT_F:              /* Commit Request */
      {
        /* Report back that Commit was not completed */
          uim_ptr->command.rpt_buf.rpt_type = UIM_COMMIT_R;
      } /* end case - UIM_COMMIT_F */
      break;

      case UIM_MS_KEY_REQ_F:          /* MS Key Request */
      {
          uim_ptr->command.rpt_buf.rpt_type = UIM_MS_KEY_REQ_R;
      } /* end case - UIM_MS_KEY_REQ_F */
      break;

      case UIM_KEY_GEN_REQ_F:         /* Key Generation Request */
      {
          uim_ptr->command.rpt_buf.rpt_type = UIM_KEY_GEN_REQ_R;
          uim_ptr->command.rpt_buf.rpt.key_gen.ms_result_len = 0;
      } /* end case - UIM_KEY_GEN_REQ_F */
      break;

      case UIM_SERV_KEY_GEN_REQ_F:         /* Service Key Generation Request */
      {
          uim_ptr->command.rpt_buf.rpt_type = UIM_SERV_KEY_GEN_REQ_R;
          uim_ptr->command.rpt_buf.rpt.serv_key_gen.result = 0;
      } /* end case - UIM_SERV_KEY_GEN_REQ_F */
      break;

      case UIM_VALIDATE_F:            /* Validation Request */
      {
        /* Report back that Validation Request was not completed */
          uim_ptr->command.rpt_buf.rpt_type = UIM_VALIDATE_R;
      } /* end case - UIM_VALIDATE_F */
      break;

      case UIM_CONFIG_REQ_F:             /* Configuration Request */
      {
        /* Report back that Config Request was not completed */
          uim_ptr->command.rpt_buf.rpt_type = UIM_CONFIG_REQ_R;
          /* Fill the block id in the response from the command */
          uim_ptr->command.rpt_buf.rpt.configuration.block_id =
            cmd_ptr->configuration.block_id;
          uim_ptr->command.rpt_buf.rpt.configuration.block_length = 0;
      } /* end case - UIM_CONFIG_REQ_F */
      break;

      case UIM_DOWNLOAD_REQ_F:           /* Download Request */
      {
          uim_ptr->command.rpt_buf.rpt_type = UIM_DOWNLOAD_REQ_R;
      } /* end case - UIM_DOWNLOAD_REQ_F */
      break;

      case UIM_SSPR_CONFIG_REQ_F:        /* SSPR Configuration Request */
      {
          uim_ptr->command.rpt_buf.rpt_type = UIM_SSPR_CONFIG_REQ_R;
          uim_ptr->command.rpt_buf.rpt.sspr_config.block_length = 0;
      } /* end case - UIM_SSPR_CONFIG_REQ_F */
      break;

      case UIM_SSPR_DOWNLOAD_REQ_F:      /* SSPR Download Reqeust */
      {
         uim_ptr->command.rpt_buf.rpt_type = UIM_SSPR_DOWNLOAD_REQ_R;
      } /* end case - UIM_SSPR_DOWNLOAD_REQ_F */
      break;

      case UIM_OTAPA_REQ_F:           /* Otapa Request */
      {
         uim_ptr->command.rpt_buf.rpt_type = UIM_OTAPA_REQ_R;
      } /* end case - UIM_OTAPA_REQ_F */
      break;

      case UIM_COMPUTE_IP_AUTH_F:
      {
          uim_ptr->command.rpt_buf.rpt_type = UIM_COMPUTE_IP_AUTH_R;
      } /* end case - UIM_COMPUTE_IP_AUTH_F */
      break;

      case UIM_STORE_ESN_MEID_ME_F:              /* Store ESN_MEID_ME command */
      {
        /* Report back that store ESN_MEID_ME was not completed */
          uim_ptr->command.rpt_buf.rpt_type = UIM_STORE_ESN_MEID_ME_R;
      } /* end case - UIM_STORE_ESN_MEID_ME_F */
      break;

      default:
        UIMDRV_MSG_ERR_1(uim_ptr->id,
                      "Unknown command 0x%x in uim_send_cdma_err_rpt",
                         cmd_ptr->hdr.command );
        break;

    }  /* switch */

     /* Fill in necessary fields of command block */
      uim_ptr->command.rpt_buf.rpt_hdr.task_ptr = NULL;

      /* Report to requesting task */
      (*(cmd_ptr->hdr.rpt_function))( &uim_ptr->command.rpt_buf );

    /* We only allocate cmd_ptr memory for the external commands that have
       valid callback. Internal UIM commands that use static buffers
       have callback set to NULL. */
    if (cmd_ptr == uim_ptr->command.cmd_ptr)
    {
      /* If cmd_ptr is same as current command in progress set global
         cmd_ptr to NULL as well */
      uim_ptr->command.cmd_ptr = NULL;
    }
    UIM_FREE(cmd_ptr);

  } /* end if - the client has supplied a report back function. */
}


/*===========================================================================

FUNCTION UIM_PROCESS_CDMA_COMMAND

DESCRIPTION
  This procedure processes a command sent to the UIM task. It sets the UIM
  state variable and calls the UIM state machine to process the command.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  It sets the UIM_CDMA state variable amongst others.

===========================================================================*/
boolean uim_process_cdma_command
(
  uim_instance_global_type *uim_ptr
)
{

  /* This indicates that the command is processed by the state machine */
  if (uim_ptr->command.cmd_ptr == NULL)
  {
    UIM_MSG_ERR_0("uim_process_gsm_command cmd_ptr is NULL");
    return FALSE;
  }

  /* Send an error report to the client and release the buffer */
  uim_ptr->command.rpt_buf.sw1 = (uim_sw1_type) 0;
  uim_ptr->command.rpt_buf.sw2 = (uim_sw2_type) 0;
  uim_ptr->command.rpt_buf.user_data = uim_ptr->command.cmd_ptr->hdr.user_data;	
  uim_ptr->command.rpt_buf.cmd_transacted  = FALSE;	
  uim_send_cdma_err_rpt(uim_ptr->command.cmd_ptr, uim_ptr);
  return FALSE;
}
 /* uim_process_cdma_command() */



/*===========================================================================

FUNCTION UIM_CDMA_COMMAND_RESPONSE

DESCRIPTION
  This procedure processes the response to a generic comamnd that has been
  recd from the UIM.

DEPENDENCIES
  None

RETURN VALUE
  Status which is of apdu_status_type

SIDE EFFECTS
  Changes the UIM state variable uim_ptr->command.cdma_state_ptr.

===========================================================================*/

uim_cmd_status_type uim_cdma_command_response
(
  uim_rsp_buf_type const *rsp_ptr,
  uim_instance_global_type *uim_ptr
)
{

  if (rsp_ptr == NULL || uim_ptr == NULL)
  {
    UIM_MSG_ERR_0("uim_cdma_command_response either uim_ptr or rsp_ptr is NULL");
    return UIM_CMD_ERR;	
  }
  uim_ptr->command.cmd_ptr->hdr.cmd_count = uim_ptr->setting.max_num_cmd_attempts;
  return UIM_CMD_ERR;
   
} /* uim_cdma_command_response */



/*===========================================================================

FUNCTION UIM_CURRENT_DIR_CDMA_CMD

DESCRIPTION
  This procedure determines if an intermediate select is needed for all
  CDMA commands

===========================================================================*/
void uim_current_dir_cdma_cmd
(
  uim_instance_global_type     *uim_ptr,
  uint8                         curr_ch,
  uim_intermediate_select_type *intermediate_sel_ptr
)
{

  intermediate_sel_ptr->is_needed    = FALSE;
} /* uim_current_dir_cdma_cmd */


