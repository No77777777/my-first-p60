
/*===========================================================================

                    MCALTDS DSDX

DESCRIPTION
  This file contains global declarations and external references
  for the DSDX MCAL layer

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2000-2014 by Qualcomm Technologies, Incorporated. All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/mcaltds_dsdx.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/19/14   zd     init draft
===========================================================================*/
#ifndef MCALTDS_DSDX_H
#define MCALTDS_DSDX_H
/*===========================================================================

                     INCLUDE FILES FOR MODULE


===========================================================================*/
#include "custtdscdma.h"
#include "tfw_sw_intf_msg.h"

/*****************************************************************************************************************/
/*   MACRO defination    */
/*****************************************************************************************************************/
/* Interface provides passing parameters for 7 Regular + 2 Special slots   */
#define MCALTDS_DSDX_MULTI_SIM_SLOT_MAX    9



/*****************************************************************************************************************/
/*   Global Variable defination    */
/*****************************************************************************************************************/
typedef enum
{
  MCALTDS_DSDX_DSDS_GAP_START = 1,
  MCALTDS_DSDX_DSDS_GAP_ABORT
} mcaltds_dsds_gap_cmd_enum_type;

typedef enum
{
  SINGLE_SIM_MODE = 0,
  MULTI_SIM_MODE_DSDS,
  MULTI_SIM_MODE_DSDA,
  /*! \brief This is the multi SIM mode of TFW in the TFW_STATE_STANDBY or
     TFW_STATE_INACTIVE states.
     TL1 will configure a different multi SIM mode for changing TFW to a
     different state.
     IF TL1 places TFW to TFW_STATE_STANDBY or TFW_STATE_INACTIVE state
     TL1 sets state of Multi SIM Mode to TFW_SIM_MODE_NOT_CONFIGURED
  */
  SIM_MODE_NOT_CONFIGURED
} mcaltds_dsdx_sim_mode_enum_type;

typedef struct
{
  /*! priority to register activities with
     the coexistence manager
  */
  uint32  cxmPriority;

  /*! desense Id for register activities with
     the coexistence manager
  */
  uint32  cxmFreqId;
  #if (defined (FEATURE_DIMEPM_MODEM) || defined (FEATURE_JOLOKIA_MODEM)) || defined (FEATURE_TABASCO_MODEM)
  tfw_cxm_slot_call_type_e cxmCallType;
  #endif

} mcaltds_dsdx_cxm_parameters_struct_type;

typedef struct
{
  /*! This is the Multi Sim Operation Mode*/
  mcaltds_dsdx_sim_mode_enum_type mode;

  /*! This is the indication of subframe number in rxTime the MULTI_SIM_CONFIG
     message will be latched by the FW. For TFW updating CXM parameters immediately,
     pls. set it to 0xFFFF using the macro TFW_MULTI_SIM_SFN_IMMEDIATE define above.
  */
  uint16 actionSfn;

  /*! flag to swap priority every frame(10 ms), for cell-DCH state, to protect DL
      TFCI decoding, need to use a high-high-low-low approach to protect whole TFCI
      and not block GSM activity.?Refer to TFW design doc for details.?
      TRUE:  swap priority every 10ms ( For sfm_no % 4 = 0,1 params will be used )
                                    ??( For sfm_no % 4 = 2,3 paramsSwap will be used )
      FALSE: do not swap priority     ( TFW will only use param buffer for all subframes)
  */
  boolean swapPriority;
  

  /*! For TFW states such as acquisition and IRAT, TFW will use the first
  one or two elements of the array for registration as no slot timings have been established
  yet. If needed, TL1 can set all array elements to the same values.
  Special slot dwpts uses array no 7 and Uppts uses array no 8. Regular slots use
   locations 0-7
  */
  mcaltds_dsdx_cxm_parameters_struct_type params[MCALTDS_DSDX_MULTI_SIM_SLOT_MAX];

  mcaltds_dsdx_cxm_parameters_struct_type paramsSwap[MCALTDS_DSDX_MULTI_SIM_SLOT_MAX];

} mcaltds_dsdx_multi_sim_settings_struct_type;



/*****************************************************************************************************************/
/*   Global Variable declaration    */
/*****************************************************************************************************************/



/*****************************************************************************************************************/
/*   Function declaration    */
/*****************************************************************************************************************/

/*===========================================================================
FUNCTION   mcaltds_dsdx_send_multi_sim_config_cmd_to_fw

DESCRIPTION
  This function send TDSCDMA_FW_MULTI_SIM_CONFIG_IND to FW

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mcaltds_dsdx_send_multi_sim_config_cmd_to_fw(mcaltds_dsdx_multi_sim_settings_struct_type *multi_sim_setting);

/*===========================================================================
FUNCTION   mcaltds_dsdx_set_fw_in_single_sim_mode

DESCRIPTION
  This function send TDSCDMA_FW_MULTI_SIM_CONFIG_IND to FW, configure it as Single SIM mode

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mcaltds_dsdx_set_fw_in_single_sim_mode(void);

/*===========================================================================
FUNCTION     tdsl1dsds_qta_send_fw_qta_gap_cfg_cmd

DESCRIPTION
  This function indicates FW if QTA start or abort.

DEPENDENCIES
  None.

PARAMETERS
  tfw_multi_sim_settings_t *stop_qta_cmd - Unused.

RETURN VALUE
  None.
===========================================================================*/
void mcaltds_dsdx_qta_send_fw_qta_gap_cfg_cmd(mcaltds_dsds_gap_cmd_enum_type cmd_type, uint8 cleanup_buff_ind, boolean is_QBTA);

#endif /* #ifndef MCALTDS_DSDX_H */
