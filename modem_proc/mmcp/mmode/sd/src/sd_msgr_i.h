#ifndef _SD_MSGR_I_H_
#define _SD_MSGR_I_H_

/*===========================================================================

               S Y S T E M   D E T E R M I N A T I O N

               I N T E R N A L   M S G R   H E A D E R   F I L E

DESCRIPTION
  This header file contains definitions used internally by the System
  Determination for message router processing

  This header file should NOT be included by any source file outside the
  System Determination module.

  Copyright (c) 2016 by Qualcomm Technologies, Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/sd/src/sd_msgr_i.h#1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
06/20/16   aj       Initial version.

===========================================================================*/

#include "mmcp_variation.h"

#include "customer.h" /* Customer configuration file */
#include "comdef.h"   /* Definition for basic types and macros */
#include "sys.h"
#include "sd_msgr.h"
#include "queue.h"
#include "sys.h"
#include "msgr_types.h"
#include "IxErrno.h"

#ifdef FEATURE_MMODE_CAFI
#include "cafi_msg.h"
#endif


/*===========================================================================

                           PUBLIC MACRO DEFINITIONS

===========================================================================*/

/* Union of all messages that SD is registered for
*/

typedef union sd_msgr_msg_u
{
  msgr_hdr_s                    hdr;
  /* Header for each msg message
  */

#ifdef FEATURE_MMODE_CAFI
  utils_cafi_dereg_req_s        cafi_dereg;
  /* payload for cafi dereg message
  */

  utils_cafi_reg_req_s          cafi_reg;
  /* payload for cafi reg message
  */

  utils_cafi_mmode_scan_ind_s         cafi_info;
  /* payload for cafi scan related info update
  */
#endif

} sd_msgr_msg_u_type;


/*===========================================================================
TYPEDEF sd_msgr_type

DESCRIPTION
  SD Task MSGR queue data type
===========================================================================*/
typedef struct
{
  q_link_type             link;
  /* For linking onto msgr queue
  */

  sd_msgr_msg_u_type       cmd;
  /* union of all msgs SD is registered for
  */
} sd_msgr_type;


/*===========================================================================

FUNCTION  sd_msgr_init

DESCRIPTION
  This is invoked by SD task at init to initialize SD MSGR client
  and register for messages

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern     void                sd_msgr_init(

  uint32           sig,
  /* SD msgr signal value
  */

  void             *pTCB
  /* SD task callback pointer
  */
);




/*===========================================================================

FUNCTION  sd_process_msgr

DESCRIPTION
  This is invoked by SD task whenever the SD MSGR signal is set in order to
  process the messages

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void sd_process_msgr(
  void
);


/*===========================================================================

FUNCTION  sd_msgr_send

DESCRIPTION
  This is a utility function that initializes message header and sends the
  message through MSGR primitives.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern errno_enum_type                      sd_msgr_send
(
  msgr_umid_type             umid,
  sys_modem_as_id_e_type     asid,
  msgr_hdr_struct_type      *msg_ptr,
  uint32                     msg_len
);

#if defined SD_DEBUG || defined TEST_FRAMEWORK
#error code not present
#endif /* SD_DEBUG */

#endif /* _SD_MSGR_I_H_ */
