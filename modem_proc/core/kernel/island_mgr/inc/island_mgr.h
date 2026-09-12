#ifndef ISLAND_MGR_H
#define ISLAND_MGR_H

/**
  @file island_mgr.h 
  @brief  Prototypes of Island Manager API The APIs allow
    	  entering and exiting island mode where the memory
    	  accesses are limited to local memory.

  EXTERNAL FUNCTIONS
   None.

INITIALIZATION AND SEQUENCING REQUIREMENTS
   None.

Copyright (c) 2014  by Qualcomm Technologies, Inc.  All Rights Reserved.

=============================================================================*/

#include <qurt.h>
#include <qurt_restricted.h>
/*=====================================================================
Constants and macros
======================================================================*/
#define ISLAND_MGR_EOK                             0  /**< Operation successfully performed. */ 
#define ISLAND_MGR_EFAILED                        12  /**< Operation failed. */
#define ISLAND_MGR_ECANCEL                        QURT_ECANCEL /**< Operation Cancelled */


/**@ingroup func_island_mgr_island_init
  Initialize Island Manager.\n
  This function prepares the data structures and performs the
  operations required to initialize the Island Manager.
 
  @return
  ISLAND_MGR_EOK -- Successfully initialized. \n
  ISLAND_MGR_EFAILURE -- Failed to initilize.

  @dependencies
  None.
 */
int island_mgr_init (void);

/**@ingroup func_island_mgr_island_enter
  Enter island mode.\n
  This function causes the system to enter island mode.
  The system should be in single threaded Mode to be able to
  enter island mode.
 
  @return
  ISLAND_MGR_EOK -- Successfully entered island mode. \n
  ISLAND_MGR_ECANCEL -- Bailed out of island enter due to a request to cancel island \n
  ISLAND_MGR_EFAILURE -- Failed to enter island mode.

  @dependencies
  None.
 */
int island_mgr_island_enter (void);

/**@ingroup island_mgr_island_exit
  Exit island mode.\n
  This function brings the system out of island mode. The system should be in
  island mode to be able to bring the system out of island mode.
 
  @return
  ISLAND_MGR_EOK -- Operation was successfully performed. \n
  ISLAND_MGR_EFAILED -- Operation failed.

  @dependencies
  None.
 */
int island_mgr_island_exit (unsigned short);

/**@ingroup island_mgr_island_cancel
  Cancel island mode.\n
  
  This function bails out of Island Enter if it is in progress.
  Call island_mgr_island_exit to exit out of Island.
 
  @return
  None. \n

  @dependencies
  None.
 */
void island_mgr_island_cancel(void);

#endif /* ISLAND_MGR_H */
