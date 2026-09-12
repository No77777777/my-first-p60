#ifndef TDSRRC_RRC_IF_H
#define TDSRRC_RRC_IF_H
/*===========================================================================
         T D S R R C  -  R R C  I N T E R F A C E

DESCRIPTION

  This module is the header module that contains the type definitions for
  the primitives between RRC and TDS RRC.
  
Copyright (c) 2002-2012 Qualcomm Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/api/tdsrrc_rrc_if.h#1 $    


===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "comdef.h"
#include "rrc_rr_types.h"
#include "tdsrrccmd.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/***************************************************************************/
/********************** MSG ID's Rxd by RRC **********************************/
/***************************************************************************/

#if 0 
/* RRC - TDSRRC Command Id type */
typedef enum
{
   /**********************************************************************/
   /* WCDMA RRC to RRC command base.                                     */
   TDSRRC_RRC_BASE = TDSRRC_RRC_CMD_ID_BASE,

   /* Add new WCDMA RRC to RRC commands below */

   /**********************************************************************/
   /******* Responses from WCDMA RRC for the requests sent by RRC ********/
   /**********************************************************************/

   /****** Response for Stop Mode from RRC -> WCDMA RRC***/
   RRC_TDSRRC_STOP_WCDMA_CNF,

   /*********************************************************************/
   /************ Requests from WCDMA RRC received by RRC  ***************/
   /*********************************************************************/

   /****** WCDMA RRC -> RRC Stop Mode request ***********/
   TDSRRC_RRC_STOP_TDS_REQ
   
} tdsrrc_rrc_cmd_e_type;
#endif

typedef struct
{
   sys_modem_as_id_e_type as_id;
   rrc_rr_stop_wcdma_cause_e_type	  stop_req_cause;
} tdsrrc_rrc_stop_tds_req_type;

typedef struct
{
   sys_modem_as_id_e_type as_id;
   rrc_rr_stop_wcdma_cause_e_type	  stop_cnf_cause;
} rrc_tdsrrc_stop_wcdma_cnf_type;

/* -----------------------------------------------------------------------
** 
** ----------------------------------------------------------------------- */

typedef union
{

   /* Union of individual command structures */
   /*--------------------------------------------------------*/
   /* Command Structure for RRC commands Sent by WCDMA RRC   */
   /*--------------------------------------------------------*/
   tdsrrc_rrc_stop_tds_req_type       tdsrrc_rrc_stop_tds_req;

   rrc_tdsrrc_stop_wcdma_cnf_type     rrc_tdsrrc_stop_wcdma_cnf;


   /* This dummy field has been added , so that both external and the 
    * RRC internal cmds have the same size for the cmd union. 
    * As the size of the command unoin is determined by the highest data type
    * present in the structure in ARM.
    */
   /*Currently disabled for Q6 build*/
   /*uint64 dummyPadding;*/

} tdsrrc_rrc_cmd_data_u_type;

typedef struct
{
   q_link_type            link;    
   
   tdsrrc_cmd_e_type      cmd_id; 
   
   uint8                  seq_num;
   
} tdsrrc_rrc_cmd_hdr_type;

typedef struct
{
   /* Command header common for WCDMA RRC commands*/
   tdsrrc_rrc_cmd_hdr_type    cmd_hdr;  

   tdsrrc_rrc_cmd_data_u_type cmd;

} tdsrrc_rrc_cmd_type;

/*===========================================================================

FUNCTION TDSRRC_PUT_RRC_CMD

DESCRIPTION

  This function puts a command buffer into TDSCDMA RRC's
  command queue. Note that the command buffer must have been
  requested through a call to rrc_get_tdsrrc_cmd_buf(). Also,
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

  A signal is set for the TDSCDMA RRC task and this could cause a context
  switch.

===========================================================================*/
void tdsrrc_put_rrc_cmd
(
   tdsrrc_rrc_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
);
/*===========================================================================

FUNCTION TDSRRC_GET_RRC_CMD_BUF

DESCRIPTION

  This function returns a TDSCDMA RRC command buffer to the calling function.
  This function dynamically allocates memory for the necessary buffer.
  Note that this buffer MUST be freed by calling tdsrrc_free_cmd_buf().

  The freeing is done by the function that dequeues and processes this
  command buffer.

  Calling function MUST check for a NULL return value and take necessary
  action.

DEPENDENCIES

  None.

RETURN VALUE

  A buffer of type tdsrrc_rrc_cmd_type. If memory is not available, NULL
  is returned.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrc_rrc_cmd_type * tdsrrc_get_rrc_cmd_buf( void );

#endif /* TDSRRC_RRC_IF_H */
