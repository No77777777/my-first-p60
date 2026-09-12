/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              H D R   S E A R C H   T A S K   C M D   Q U E U E

GENERAL DESCRIPTION
  This file contains prototypes and declarations for sending commands to
  the search task.

EXTERNALIZED FUNCTIONS (Global)
  None

EXTERNALIZED FUNCTIONS (Regional)
  hdrsrchcmd_init( ) - Initialization
  hdrsrchcmd_new( )  - Extracts a command buffer from free queue
  hdrsrchcmd_post( ) - Places cmd buffer on command queue & signals task
  hdrsrchcmd_get( )  - Extracts a command buffer from command queue.
  hdrsrchcmd_free( ) - Returns buffer to free queue.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  hdrsrchcmd_init( )

  Copyright (c) 2000 - 2013
                by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$PVCSPath: L:/src/asw/COMMON/vcs/hdrsrchcmd.c_v   1.8   11 Mar 2003 15:25:14   aneufeld  $
$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchcmd.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     --------------------------------------------------------- 
07/15/13   smd     Kept SET_DRC_CMD during cmd purge. 
07/01/13   dsp     Added a F3 to indicate a new cmd is being added.
07/09/12   dsp     Removed unnecessary F3s. 
05/22/12   smd     Fixed a bug in cmd purge when both tx and srch accessing
                   command queue.
04/25/12   smd     Changes to repost some commands when purging command queue.
03/10/12   wsh     Added deactivate timeout err_fatal and related traces 
03/02/11   vlc     Modified hdrsrch_cmd_type and put back memory heap changes.
11/18/10   vlc     Reverted memory heap changes. 
09/07/10   pxu     Memory heap changes. 
06/21/10   cnx     Fixed compiler warnings.
03/09/10   smd     Updated implementation for commands from message route.
09/02/09   smd     Added messages for LTE to DO pilot measurement.
10/07/08   lyl     Fixed lint errors.
10/20/05   jyw     Fixed the lint error.
03/23/05   hrk     Added function hdrsrchcmd_purge().
09/10/03   mpa     Converted F3 messages to MSG2.0
03/11/03   ajn     Updated Copyright notice
01/30/02   ajn     Added status to command buffers
03/02/01   aaj     included customer.h file
10/25/00   ajn     Cleanup.
07/07/00   ajn     Created module

==========================================================================*/

/* <EJECT> */
/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdrsrchcmd.h"

#include "customer.h"

#include "hdrsrchtask.h"

#include "hdrdebug.h"
#include "err.h"
#include "queue.h"
#include "modem_mem.h"


/* <EJECT> */
/*==========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

==========================================================================*/

/* Use void pointer as a bridge to avoid the following compiler warning: 
   "cast increases required alignment of target type". */
#define FIELD_PTR_TO_STRUCT_PTR(field_ptr,field,T) \
  /*lint --e(413) */ \
  ((T*)((void *)(((byte*)field_ptr)-FPOS(T,field)))) /*lint +e413 */

#define ITEM_PTR_FROM_CMD_PTR(cmd_ptr) \
  FIELD_PTR_TO_STRUCT_PTR(cmd_ptr,cmd,hdrsrch_cmd_q_item_type);

/*===========================================================================

                         EXTERNAL VARIABLES

===========================================================================*/

hdrsrchcmd_struct_type    hdrsrchcmd;

/* the number of commands which will be reposted. This number must match with
   the list length of hdrsrchcmd_repost_cmd_list
*/
#define HDRSRCHCMD_REPOST_CMD_CNT     2

/* The command list which we want to re-post during purging command queue */
hdrsrch_cmd_name_enum_type hdrsrchcmd_repost_cmd_list[ HDRSRCHCMD_REPOST_CMD_CNT ]= 
{
  HDRSRCH_CHANGE_SLEEP_TIMELINE_CMD,
  HDRSRCH_AT_SET_DRC_CMD
    /* the length of the list is defined in HDRSRCHCMD_REPOST_CMD_CNT. When
       adding a new item in this list, the CNT must be updated
    */
};
/* <EJECT> */
/*==========================================================================

                      FUNCTION  DEFINITIONS

==========================================================================*/

/*==========================================================================

FUNCTION HDRSRCHCMD_INIT

DESCRIPTION
  This function initializes the HDR SEARCH Command Queue Structures.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchcmd_init( )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Initialize the Command Queue, and the Command Free Queue */

  (void) q_init( &hdrsrchcmd.cmd_q );

} /* hdrsrchcmd_init( ) */


/* <EJECT> */
/*==========================================================================

FUNCTION HDRSRCHCMD_NEW

DESCRIPTION
  This function requests a free command queue item from the free queue.

DEPENDENCIES
  hdrsrchcmd_init()

PARAMETERS
  cmd_name  - Name of command structure to create

RETURN VALUE
  Pointer to HDR SEARCH command structure, or NULL if free queue is empty

SIDE EFFECTS
  None

==========================================================================*/

hdrsrch_cmd_type* hdrsrchcmd_new
(
  hdrsrch_cmd_name_enum_type cmd_name
    /* Command structure to create */
)
{
  hdrsrch_cmd_q_item_type * item;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  item = ( hdrsrch_cmd_q_item_type *) modem_mem_alloc(
           sizeof( hdrsrch_cmd_q_item_type ) , MODEM_MEM_CLIENT_HDR_CRIT );
    /* Retrieve next available free queue item */

  if ( item != NULL )
  {
    /* If a free item was available, return a pointer to the command
       portion of the structure */

    item->cmd.name   = cmd_name;
    item->cmd.status = E_NO_DATA;
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "New cmd %d buffer allocated", cmd_name );
    return &item->cmd;
  }
  else
  {
    /* No free item is available! */
    ERR_FATAL("No memory for new command buffer.", 0, 0, 0);

    return NULL;
  }

} /* hdrsrchcmd_new( ) */


/* <EJECT> */
/*==========================================================================

FUNCTION HDRSRCHCMD_POST

DESCRIPTION
  This function posts a command to the Search Task command queue.

DEPENDENCIES
  Command structure must been obtained from a call to hdrsrchcmd_new()

PARAMETERS
  Command to be posted.

RETURN VALUE
  None

SIDE EFFECTS
  HDRSRCH_CMD_Q_SIG is sent to HDR Search task.

==========================================================================*/

void hdrsrchcmd_post
(
  hdrsrch_cmd_type  * cmd
    /* Command to be posted on command queue */
)
{
  hdrsrch_cmd_q_item_type * item;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Determine q_item structure that contained the given "cmd" field. */

  item = ITEM_PTR_FROM_CMD_PTR(cmd);


  /* Add the item to the command queue */

  q_put( &hdrsrchcmd.cmd_q, &item->link);


  /* Signal Search task of presents of new command item */

  HDRSRCH_SIG_SET( HDRSRCH_CMD_Q_SIG );

} /* hdrsrchcmd_post */


/* <EJECT> */
/*==========================================================================

FUNCTION HDRSRCHCMD_GET

DESCRIPTION
  This function gets a command from the Search Task command queue.

DEPENDENCIES
  hdrsrchcmd_init()

PARAMETERS
  None

RETURN VALUE
  Command structure posted by hdrsrchcmd_post()

SIDE EFFECTS
  None

==========================================================================*/

hdrsrch_cmd_type* hdrsrchcmd_get( void )
{
  hdrsrch_cmd_q_item_type * item;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  item = (hdrsrch_cmd_q_item_type *) q_get( &hdrsrchcmd.cmd_q );
    /* Retrieve next commmand from command queue */

  if ( item != NULL )
  {
    /* If a command was available, return a pointer to the command
       portion of the structure */

    return &item->cmd;
  }
  else
  {
    /* No command is available! */

    return NULL;
  }

} /* hdrsrchcmd_get( ) */


/* <EJECT> */
/*==========================================================================

FUNCTION HDRSRCHCMD_FREE

DESCRIPTION
  This function frees a command structure returned by hdrsrchcmd_new() or
  by hdrsrchcmd_get()

DEPENDENCIES
  Command structure must been obtained from a call to hdrsrchcmd_new() or
  hdrsrchcmd_get()

PARAMETERS
  Command structure to be free'd.

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchcmd_free
(
  hdrsrch_cmd_type  * cmd
    /* Command to be returned to the free queue */
)
{
  hdrsrch_cmd_q_item_type * item;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  switch ( cmd->status )
  {
    case E_SUCCESS:
       break;

    case E_FAILURE:
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Command %d failed", cmd->name );
      break;

    case E_NOT_SUPPORTED:
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Command %d not supported", cmd->name );
      break;

    case E_CANCELED:
      HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Command %d canceled", cmd->name );
      break;

    case E_ABORTED:
      HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Command %d aborted", cmd->name );
      break;

    default:
      ERR("Command %d unexpected status %d", cmd->name, cmd->status, 0);
  }

  cmd->status = E_DATA_INVALID;
    /* Mark item as no longer valid */

  /* Determine q_item structure that contained the given "cmd" field. */

  item = ITEM_PTR_FROM_CMD_PTR(cmd);

  modem_mem_free( item , MODEM_MEM_CLIENT_HDR_CRIT );

} /* hdrsrchcmd_free */


/* <EJECT> */
/*==========================================================================

FUNCTION HDRSRCHCMD_PURGE

DESCRIPTION
  This function removes all commands from command queue and puts them in
  free queue.

DEPENDENCIES

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Command queue is empty.

==========================================================================*/

void hdrsrchcmd_purge( void )
{
  hdrsrch_cmd_type  *cmd;
    /* command pointer */
  int               i;
    /* looping index */
  hdrsrch_cmd_type  *repost_cmd[HDRSRCHCMD_REPOST_CMD_CNT+3];
    /* array to store commands which we want to process later */
  int                repost_cmd_idx =0;
    /* index of repost_cmd array */
  boolean            to_repost_cmd = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Purging commands" );

  /* purge all commands in HDRSRCH command queue. However we want to store some commands which
     we want to process them later.
  */
  while ( cmd = hdrsrchcmd_get( ), cmd != NULL )
  {
    to_repost_cmd = FALSE;
    for ( i = 0;
          ( i < HDRSRCHCMD_REPOST_CMD_CNT ) && ( to_repost_cmd == FALSE );
          i++ )
    {
      if ( cmd->name == hdrsrchcmd_repost_cmd_list[i] )
      {
        /* This is the command in repost list. */

        cmd->status = E_NO_DATA;

        if ( repost_cmd_idx < HDRSRCHCMD_REPOST_CMD_CNT+3  )
        {
          repost_cmd[ repost_cmd_idx++ ] = cmd;
            /* Store the command. Those commands will be repost after purging is done.			   
            */
          HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Stored command %d in repost list", cmd->name );

          to_repost_cmd = TRUE;
        }
        else
        {
            HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Repost cmd list is full");
        }
      }
    }
    if ( to_repost_cmd == FALSE )
    {

      /* Delete the command */
      cmd->status = E_CANCELED;
      hdrsrchcmd_free( cmd );
      /* Return command to free queue */
    }
  }

  for (  i = 0;
         i < repost_cmd_idx ;
         i++ )
  {
    hdrsrchcmd_post( repost_cmd[i] );
      /* Post the command again. Those command will be processed later.			   
      */
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Post command %d in repost list", repost_cmd[i]->name );
  }
} /* hdrsrchcmd_purge */

/* <EJECT> */
