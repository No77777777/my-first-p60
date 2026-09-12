/*===========================================================================

               S Y S T E M   D E T E R M I N A T I O N

               I N T E R N A L   M S G R   F I L E

DESCRIPTION
  This file contains definitions and utility functions used internally by the
  System Determination for message router processing


  Copyright (c) 2016 by Qualcomm Technologies, Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/sd/src/sd_msgr.c#1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
06/20/16   aj       Initial version.

===========================================================================*/


#include "sd_msgr_i.h"
#include "queue.h"
#include <stringl/stringl.h>
#include "msgr_rex.h"
#include "sddbg.h"

#if defined(SD_DEBUG)
#error code not present
#endif
#if defined(SD_DEBUG) || defined(TEST_FRAMEWORK)
#error code not present
#endif

#ifdef FEATURE_MMODE_QTF
#error code not present
#endif /* FEATURE_MMODE_QTF */


#ifdef FEATURE_MMODE_CAFI
#include "sdsense.h"
#endif


/*==============================================================================
                           Global definitions
==============================================================================*/

/*  The SD MSGR client
 */
static  msgr_client_t         sd_msgr_client;

/*  The MSGR queue.
 */
static  q_type                sd_msgr_q;


#define SD_OFFSETOF(type,member) (((char*)(&((type*)1)->member))-((char*)1))

/*==============================================================================
                           Function  definitions
==============================================================================*/

/*===========================================================================

FUNCTION  sd_msgr_handler

DESCRIPTION
  This is a utility function that processes each registered message that
  is recieved by SD and forwards it to the appropriate handler

  msg pointer in *void should be cast to sd_msgr_type before use.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

static    void              sd_msgr_handler(

  void         *param
  /* message router hdr+payload
  */
)
{

  sd_msgr_type       *msg;
  if (param == NULL)
    return;

  msg = (sd_msgr_type *)param;

  switch ( msg->cmd.hdr.id )
  {

    #ifdef FEATURE_MMODE_CAFI
    case UTILS_CAFI_MMODE_SCAN_REG_CNF:
      sdsense_msgr_handle(param);
      break;

    case UTILS_CAFI_MMODE_SCAN_DEREG_CNF:
      sdsense_msgr_handle(param);
      break;

    case UTILS_CAFI_MMODE_SCAN_IND:
      sdsense_msgr_handle(param);
      break;
    #endif //  #ifdef FEATURE_MMODE_CAFI

    default:
      break;
  }
}


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
errno_enum_type                          sd_msgr_send
(
  msgr_umid_type             umid,

  sys_modem_as_id_e_type     subs_id,

  msgr_hdr_struct_type      *msg_ptr,
  /* Pointer to message to be sent. Note that the first member of the message
   *  that is pointed to by this pointer must be of msgr_hdr_struct_type type
   */
  uint32                     msg_len
  /* Total message length (header and payload) in bytes */
)
{
  errno_enum_type msgr_error = E_SUCCESS;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( msg_ptr != NULL )
  {
    msgr_init_hdr_variant( msg_ptr, MSGR_MM_SD, umid, subs_id);
#ifdef TEST_FRAMEWORK
    #error code not present
#else
    msgr_error = msgr_send( msg_ptr, msg_len );
#endif

  }
  return msgr_error;
} /* sd_msgr_send */

/*========================================================================
  FUNCTION sd_msgr_register

  DESCRIPTION
    register to msgr messages

  PARAMETERS
    msgr client object pointer

  RETURN VALUE
    None
===========================================================================*/
static           void             sd_msgr_register(  )
{

  #ifdef FEATURE_MMODE_CAFI
  errno_enum_type err;
  err = msgr_register_variant( MSGR_MM_SD, &sd_msgr_client, MSGR_ID_REX, UTILS_CAFI_MMODE_SCAN_REG_CNF , SYS_MODEM_AS_ID_1);
  if(err != E_SUCCESS)
  {
    SD_MSG_HIGH_1(" UTILS_CAFI_SCAN_REG_CNF register ret %d", err);
  }

  err = msgr_register_variant(  MSGR_MM_SD, &sd_msgr_client, MSGR_ID_REX, UTILS_CAFI_MMODE_SCAN_DEREG_CNF , SYS_MODEM_AS_ID_1);
  if(err != E_SUCCESS)
  {
    SD_MSG_HIGH_1(" UTILS_CAFI_SCAN_DEREG_CNF register ret %d", err);
  }

  err = msgr_register_variant(  MSGR_MM_SD, &sd_msgr_client, MSGR_ID_REX, UTILS_CAFI_MMODE_SCAN_IND , SYS_MODEM_AS_ID_1);
  if(err != E_SUCCESS)
  {
    SD_MSG_HIGH_1("  UTILS_CAFI_SCAN_IND register ret %d", err);
  }
  #endif //FEATURE_MMODE_CAFI

}


/*===========================================================================

FUNCTION SD_PROCESS_MSGR()

DESCRIPTION
  This function is called in response to SD_MSGR_SIGNAL. It
  processes the commands from the SD msgr queue.

DEPENDENCIES
  An item must be on the SD msgr queue.

RETURN VALUE
  none

SIDE EFFECTS
  None.

===========================================================================*/
void                          sd_process_msgr()
{
  sd_msgr_type   *msg;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  while ((msg = q_get(&sd_msgr_q)) != NULL)
  {
    /*-----------------------------------------------------------------------
      Do not manipulate msg contents. Other services may handle same message.
      So do not change the message. And do not free the item back to free q.
      Freeing q should be done at the end of this section. Again, this is due
      to possibility of same message being handled by multiple services.
    -----------------------------------------------------------------------*/
    sd_msgr_handler( (void *)msg );

    msgr_rex_free_msg_buf(&(msg->link));
  }
} /* sd_process_msgr() */

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
void                                    sd_msgr_init(
  uint32                    sig,
  /* SD MSGR signal value
  */
  void                     *pTCB
  /* SD task CB pointer
  */
)
{
  errno_enum_type msgr_err;

  /*  Initialize the message router queue
   */
  (void) q_init( &sd_msgr_q );

  /*  Create a message router client
   */
  msgr_err = msgr_client_create( &sd_msgr_client );
  if (msgr_err != E_SUCCESS)
  {
    SD_MSG_HIGH_0("Unable to create MSGR client");
    return;
  }

  msgr_err = msgr_client_set_rex_q( &sd_msgr_client,
                                    (rex_tcb_type *) pTCB,
                                    (rex_sigs_type) sig,
                                    &sd_msgr_q,
                                    MSGR_NO_QUEUE,
                                    SD_OFFSETOF(sd_msgr_type, cmd),
                                    MSGR_NO_OFFSET,
                                    sizeof(sd_msgr_type));

  SD_MSG_HIGH_1("msgr_client_set_rex_q returned %d", msgr_err);

  /* register for all msgs that SD is interested in
  */
  sd_msgr_register( );
}



#if defined SD_DEBUG || defined TEST_FRAMEWORK
#error code not present
#endif /* +TEST_FRAMEWORK. */
