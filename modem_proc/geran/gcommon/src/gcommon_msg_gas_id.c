/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               G A S _ I D   I N   M S G S   G E T / S E T

GENERAL DESCRIPTION
   This module provides functions to get the gas_id out of internal messages
   or set them in messages.

EXTERNALIZED FUNCTIONS
  gcommon_msg_l2_gas_id_get / set

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2014 by QUALCOMM Technologies, Incorporated. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gcommon/src/gcommon_msg_gas_id.c#1 $

when       who      what, where, why
--------   ---      ---------------------------------------------------------
2014-10-23 tjw      New file

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "gcommon_msg_gas_id.h"
#include "rr_l2_l1.h"
#include "rr_l2.h"
#include "rr_l2_g.h"
#include "l2_l1.h"
#include "ms.h"

/*===========================================================================

FUNCTION  gcommon_msg_gas_id_get / set

DESCRIPTION
  Get or set the gas_id in a msg

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
typedef struct
{
  unsigned            sig_id;
  size_t              offset_to_gas_id;
} offset_to_gas_id_table_t;

/* Hashing function for msg set plus msg ID */
#define MSG_SET__MSG_ID(msg_set, msg_id)  (((msg_set) << 16) + (msg_id))
/* Macro to simplify generating the table */
#define OFFSET_ENTRY(id, msg_t)        {id, offsetof(msg_t, gas_id)}

const offset_to_gas_id_table_t offset_to_gas_id_table[] =
{ /* RR->L2 primitives */
  OFFSET_ENTRY(MSG_SET__MSG_ID(MS_RR_L2, DL_ESTABLISH_REQ),     dl_establish_req_T),
  OFFSET_ENTRY(MSG_SET__MSG_ID(MS_RR_L2, DL_RELEASE_REQ),       dl_release_req_T),
  OFFSET_ENTRY(MSG_SET__MSG_ID(MS_RR_L2, DL_DATA_REQ),          dl_data_req_T),
  OFFSET_ENTRY(MSG_SET__MSG_ID(MS_RR_L2, DL_UNIT_DATA_REQ),     dl_unit_data_req_T),
  OFFSET_ENTRY(MSG_SET__MSG_ID(MS_RR_L2, DL_SUSPEND_REQ),       dl_suspend_req_T),
  OFFSET_ENTRY(MSG_SET__MSG_ID(MS_RR_L2, DL_RESUME_REQ),        dl_resume_req_T),
  OFFSET_ENTRY(MSG_SET__MSG_ID(MS_RR_L2, DL_RECONNECT_REQ),     dl_reconnect_req_T),
  OFFSET_ENTRY(MSG_SET__MSG_ID(MS_RR_L2, DL_RANDOM_ACCESS_REQ), dl_random_access_req_T),
  OFFSET_ENTRY(MSG_SET__MSG_ID(MS_RR_L2, MDL_RELEASE_REQ),      mdl_release_req_T),
  OFFSET_ENTRY(MSG_SET__MSG_ID(MS_RR_L2, DL_DATA_MUX_IND),      dl_data_mux_ind_T),
  /* L1->L2 primitives */
  OFFSET_ENTRY(MSG_SET__MSG_ID(MS_L2_L1, PH_READY_TO_SEND_IND), ph_ready_to_send_ind_T),
  OFFSET_ENTRY(MSG_SET__MSG_ID(MS_L2_L1, PH_DATA_IND),          ph_data_ind_T),
  OFFSET_ENTRY(MSG_SET__MSG_ID(MS_L2_L1, PH_RANDOM_ACCESS_CNF), ph_random_access_cnf_T),
  OFFSET_ENTRY(MSG_SET__MSG_ID(MS_L2_L1, PH_CONNECT_IND),       ph_connect_ind_T)
};

gas_id_t gcommon_msg_gas_id_get(const void *msg)
{
  gas_id_t gas_id;
  int i;

  for (i = 0; i < ARR_SIZE(offset_to_gas_id_table); i++)
  {
    if (MSG_SET__MSG_ID(((IMH_T *)msg)->message_set,
                        ((IMH_T *)msg)->message_id) ==
         offset_to_gas_id_table[i].sig_id)
    {
      gas_id = *(gas_id_t *)(((byte *)(msg) + offset_to_gas_id_table[i].offset_to_gas_id));
      return gas_id;
    }
  }
  MSG_GERAN_FATAL_0("gas_id offset not in offset_to_gas_id_table");
  return GERAN_ACCESS_STRATUM_ID_UNDEFINED;
}

void gcommon_msg_gas_id_set(void *msg, const gas_id_t gas_id)
{
  int i;

  for (i = 0; i < ARR_SIZE(offset_to_gas_id_table); i++)
  {
    if (MSG_SET__MSG_ID(((IMH_T *)msg)->message_set,
                        ((IMH_T *)msg)->message_id) ==
         offset_to_gas_id_table[i].sig_id)
    {
      *(gas_id_t *)(((byte *)(msg) + offset_to_gas_id_table[i].offset_to_gas_id)) = gas_id;
      return; /* Success */
    }
  }
  MSG_GERAN_FATAL_0("gas_id offset not in offset_to_gas_id_table");
}

