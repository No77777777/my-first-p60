#ifndef TDSRRCCMD_L2_H
#define TDSRRCCMD_L2_H
/*===========================================================================
                      R R C  L2  C O M M A N D S

DESCRIPTION

  This module contains external definitions for the
  command queues used by RRC-L2 tasks. This file should be
  included by L2 module sending commands to RRC's
  command queues.


Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/inc/tdsrrccmd_l2.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/13/11   hx      Move definition for RLC from tdsrrccmd_v.h to this file 
06/06/11   hx      Move definition for MAC from tdsrrccmd_v.h to this file

===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"
#include "queue.h"
#include "tdsmacrrcif.h"
#include "tdsrlcrrcif.h"

#include "tdsrrccmd.h"

#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#endif

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/*--------------------------------------------------------*/
/*                 RRC COMMAND IDs                        */
/*                                                        */
/*  Command Ids for commands sent from various tasks to   */
/*  RRC are defined here. All command ids, regardless of  */
/*  which command queue is used to queue them are defined */
/*  in a single enum type given below.                    */
/*                                                        */
/*  NOTE! When you add a new command ID please make sure  */
/*  the corrosponding command structure is added to the   */
/*  structure tdsrrc_cmd_type that contains all the commands.*/
/*                                                        */
/*--------------------------------------------------------*/


/*
typedef enum{          
                       
}tdsrrc_mac_cmd_e_type;
                       
typedef enum{          
                       
}tdsrrc_rlc_cmd_e_type;
*/

/*
** L2 Command header. This header is a part of all commands sent
** to the RRC layer. This contains the command id for the command sent
** to RRC, and a Queue link in order to put the command in the
** appropriate queue
*/

typedef struct
{
  q_link_type   link;        /* Queue link     */
  tdsrrc_cmd_e_type cmd_id;     /* The command Id */
  /* Other items may be added here for debugging purposes.
  this is still TBD. */
  uint8         seq_num;
}tdsrrc_mac_cmd_hdr_type;


typedef struct
{
  q_link_type   link;        /* Queue link     */
  tdsrrc_cmd_e_type cmd_id;     /* The command Id */
  /* Other items may be added here for debugging purposes.
  this is still TBD. */
  uint8         seq_num;
}tdsrrc_rlc_cmd_hdr_type;

/*--------------------------------------------------------*/
/*                 L2 COMMAND BUFFER                     */
/*                                                        */
/*  The Command buffer used for all L2 command queues is */
/*  defined here. The command buffer contains a RRC       */
/*  command header and a union of all possible commands   */
/*  for any RRC command queue                             */
/*--------------------------------------------------------*/


typedef union
{
  /*--------------------------------------------------------*/
  /* Command Structure for RRC commands Sent by MAC         */
  /*--------------------------------------------------------*/
  tdsmac_status_ind_e_type      cmac_status_ind;
  tdsmac_ul_traffic_meas_ind_type    cmac_ul_traffic_ind;
  tdsmac_meas_ctrl_cnf_type          mac_meas_ctrl_cnf;
  tdsmac_tfc_subset_cfg_cnf_type     mac_tfc_subset_cfg_cnf_info;

 /* This dummy field has been added , so that both external and the 
 * RRC internal cmds have the same size for the cmd union. 
 * As the size of the command unoin is determined by the highest data type
 * present in the structure in ARM.*/
  /*Currently disabled for Q6 build*/
  /*uint64 dummyPadding;*/

}tdsrrc_mac_cmd_data_u_type;

typedef struct
{
  /* Command header common for all commands */
  tdsrrc_mac_cmd_hdr_type    cmd_hdr;

  tdsrrc_mac_cmd_data_u_type cmd;

}tdsrrc_mac_cmd_type;


typedef union
{
  /*--------------------------------------------------------*/
  /* Command Structure for RRC commands Sent by RLC         */
  /*--------------------------------------------------------*/
  tdsrlc_status_ind_type        crlc_status_ind;
  tdsrlc_ul_tm_config_cnf_type  crlc_ul_tm_config_cnf;
  tdsrlc_dl_tm_config_cnf_type  crlc_dl_tm_config_cnf;
  tdsrlc_ul_um_config_cnf_type  crlc_ul_um_config_cnf;
  tdsrlc_dl_um_config_cnf_type  crlc_dl_um_config_cnf;
  tdsrlc_am_config_cnf_type     crlc_am_config_cnf;

 /* This dummy field has been added , so that both external and the 
 * RRC internal cmds have the same size for the cmd union. 
 * As the size of the command unoin is determined by the highest data type
 * present in the structure in ARM.*/
  /*Currently disabled for Q6 build*/
  /*uint64 dummyPadding;*/

}tdsrrc_rlc_cmd_data_u_type;

typedef struct
{
  /* Command header common for all commands */
  tdsrrc_rlc_cmd_hdr_type    cmd_hdr;

  tdsrrc_rlc_cmd_data_u_type cmd;

}tdsrrc_rlc_cmd_type;




/*===========================================================================

FUNCTION RRC_GET_MAC_CMD_BUF

DESCRIPTION

  This function returns a MAC command buffer to the calling function.
  This function dynamically allocates memory for the necessary buffer.
  Note that this buffer MUST be freed by calling tdsrrc_free_cmd_buf().

  The freeing is done by the function that dequeues and processes this
  command buffer.

  Calling function MUST check for a NULL return value and take necessary
  action.

DEPENDENCIES

  None.

RETURN VALUE

  A buffer of type tdsrrc_cmd_type. If memory is not available, NULL
  is returned.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrc_mac_cmd_type *tdsrrc_get_mac_cmd_buf( void );



/*===========================================================================

FUNCTION RRC_PUT_MAC_CMD

DESCRIPTION

  This function puts a command buffer into RRC's mac
  command queue. Note that the command buffer must have been
  requested through a call to get_rrc_mac_cmd_buf(). Also,
  The calling function must have filled all the necessary
  data including the command id before passing the buffer
  to this function.

  This function also sets the appropriate signal to the RRC
  task to indicate that a command has been placed in the
  command queue.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  A signal is set for the RRC task and this could cause a context
  switch.

===========================================================================*/
void tdsrrc_put_mac_cmd
(
  tdsrrc_mac_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
);



/*===========================================================================

FUNCTION RRC_GET_RLC_CMD_BUF

DESCRIPTION

  This function returns a RLC command buffer to the calling function.
  This function dynamically allocates memory for the necessary buffer.
  Note that this buffer MUST be freed by calling tdsrrc_free_cmd_buf().

  The freeing is done by the function that dequeues and processes this
  command buffer.

  Calling function MUST check for a NULL return value and take necessary
  action.

DEPENDENCIES

  None.

RETURN VALUE

  A buffer of type tdsrrc_cmd_type. If memory is not available, NULL
  is returned.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrc_rlc_cmd_type *tdsrrc_get_rlc_cmd_buf( void );


/*===========================================================================

FUNCTION RRC_PUT_RLC_CMD

DESCRIPTION

  This function puts a command buffer into RRC's rlc
  command queue. Note that the command buffer must have been
  requested through a call to get_rrc_rlc_cmd_buf(). Also,
  The calling function must have filled all the necessary
  data including the command id before passing the buffer
  to this function.

  This function also sets the appropriate signal to the RRC
  task to indicate that a command has been placed in the
  command queue.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  A signal is set for the RRC task and this could cause a context
  switch.

===========================================================================*/
void tdsrrc_put_rlc_cmd
(
  tdsrrc_rlc_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
);


#endif /*tdsrrccmd_l2.h*/
