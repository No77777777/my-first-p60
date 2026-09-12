#ifndef TDSDLDPCH_H
#define TDSDLDPCH_H

/*===========================================================================
                 D L N S C H M G R . H

GENERAL DESCRIPTION
  This file contains the declarations and definitions of dlnschmgr needed
  across modules.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2007 - 2008 by Qualcomm Technologies Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/tlm.c_v   1.153   17 Jul 2002 16:36:16   halbhavi  $
$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsdldpch.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     ---------------------------------------------------------
09/21/10    jintaoh      Initial version
                    
                    ===========================================================================*/
#include "customer.h"
#include "comdef.h"
#include "tdsl1const.h"
#include "tdsdlchmgr.h"
#include "tdsdrxmgr.h"

typedef struct {
  boolean in_use;
  tdsdlchmgr_cfg_db_idx_type chmgr_cfg_db_idx;
  tdsdrxmgr_entity_handle_type drx_handle;
    /*Integer(1, 2,4,8,16,32,64). Default is continuous allocation. Value 1 indicate continuous. (10.3.6.10)*/
  uint8 repetition_period;

  /*Integer(1.. Repetition period -1 ). NOTE:	This is empty if repetition period is set to 1.  (10.3.6.10)*/
  uint8 repetition_length;

  /*CFN number start of the physical channel existence.
  **Integer(0..255). Default value is "Now".(10.3.3.31)
  */
  uint16 activation_time;
} tdsdldpch_cfg_db_struct_type;

#define TDSDLDPCH_CFG_DB_SIZE 1


/* This structure holds the reponse functions for channel type: DPCH*/
extern tdsdlchmgr_resp_func_cb_struct_type tdsdldpch_resp_func_cb;

extern tdsdlchmgr_idx_mgmt_func_struct_type tdsdldpch_idx_mgmt_func;

extern tdsdlchmgr_op_cb_func_struct_type tdsdldpch_op_cb_func;

extern tdsdrxmgr_channel_cb_struct_type tdsdldpch_drx_cb_functions;

extern tdsdlchmgr_schedule_para_cb_func_struct_type tdsdldpch_schedule_para_cb_func;

extern tdsdldpch_cfg_db_struct_type tdsdldpch_cfg_db; 

#define TDSDLDPCH_GET_REPETITION_LENGTH() \
     (tdsdldpch_cfg_db.repetition_length)
  
#define TDSDLDPCH_GET_REPETITION_PERIOD() \
     (tdsdldpch_cfg_db.repetition_period)

#define TDSDLDPCH_GET_ACTIVATION_CFN() \
      (tdsdldpch_cfg_db.activation_time) 

/*===========================================================================
FUNCTION        tdsdldpch_init

DESCRIPTION     This function initializes the nsch manager. 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern void tdsdldpch_init( void );
 
#endif /* DLNSCHMGR_H */

