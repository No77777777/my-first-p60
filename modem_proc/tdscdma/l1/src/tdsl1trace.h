#ifndef TDS_L1_TRACE_H
#define TDS_L1_TRACE_H

/*============================================================================
              L1 TRACE   H E A D E R   F I L E

DESCRIPTION
This files contains all necessary macros and definitions for tracing
L1 SW operation and its timing.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2000 - 2012 Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsl1trace.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
03/10/12    cdf/cxc  file created.
                    
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"
#include "tdsstmr.h"
#include "tdsl1std.h"
#include "tdsl1m.h"

/*===========================================================================

                     DATA TYPE DEFINITIONS

===========================================================================*/

#ifdef FEATURE_TDSCDMA_L1_TRACE
typedef enum
{
  TDSL1TRACE_ACTION_NONE = 0,
  TDSL1TRACE_ACTION_START,
  TDSL1TRACE_ACTION_END,
  TDSL1TRACE_ACTION_IND
} tdsl1trace_action_type_enum;

typedef enum
{
  TDSL1TRACE_OP_NONE = 0,
  TDSL1TRACE_L1M_STATE_CLEANUP,
  TDSL1TRACE_DROP_ALL_CHAN,
  TDSL1TRACE_CCTRCH_SETUP,
  TDSL1TRACE_ADD_CHAN,
  TDSL1TRACE_DROP_CHAN,
  TDSL1TRACE_RECFG_CHAN,
  TDSL1TRACE_SUSPEND_CHAN,
  TDSL1TRACE_RESUME_CHAN,
  TDSL1TRACE_ASET_UPDATE,
  TDSL1TRACE_TUNE_RF_FREQ,
  TDSL1TRACE_DROP_CHAN_DONE_IND,
  TDSL1TRACE_RESUME_CHAN_DONE_IND,
  TDSL1TRACE_SUSPEND_CHAN_DONE_IND
} tdsl1trace_traced_op_type_enum;


typedef struct
{
  tdsstmr_counter_debug_info_type cur_time;
  tdsl1trace_action_type_enum action;
  tdsl1trace_traced_op_type_enum traced_op;
  tdsl1m_module_enum_type traced_module;
  union {
  	tdsl1m_state_enum_type l1m_state;
	tdsl1_dl_phychan_enum_type dl_chan_type;
	tdsl1_ul_phychan_enum_type ul_chan_type;
	uint32 undefined;
  } module_spec; /* module specific field */
}tdsl1trace_action_db_type;


/*=========================================================================

FUNCTION      tdsl1trace_init

DESCRIPTION
  This function initialize L1 SW trace database.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  trace db is cleared, and db index is set to 0;

=========================================================================*/

void tdsl1trace_init(void);


/*=========================================================================

FUNCTION      tdsl1trace_put

DESCRIPTION
  This function put a trace record into trace db.

DEPENDENCIES

RETURN VALUE
  The current trace db idx.

SIDE EFFECTS
  trace db idx increment

=========================================================================*/

uint16 tdsl1trace_put( tdsl1trace_action_type_enum action,
                                       tdsl1trace_traced_op_type_enum traced_op,
                                       tdsl1m_module_enum_type traced_module);


/*=========================================================================

FUNCTION      tdsl1trace_put_dl_ul_spec

DESCRIPTION
  This function put a DL/UL specific trace record into trace db.

DEPENDENCIES

RETURN VALUE
  The current trace db idx.

SIDE EFFECTS
  trace db idx increment
  
=========================================================================*/  
uint16 tdsl1trace_put_spec( tdsl1trace_action_type_enum action,
                                                   tdsl1trace_traced_op_type_enum traced_op,
                                                   tdsl1m_module_enum_type traced_module,
                                                   uint32 spec);

#endif /* FEATURE_TDSCDMA_L1_TRACE */
#endif /* TDS_L1_TRACE_H */
