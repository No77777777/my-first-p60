#ifndef MPLM_EXTERN_H_
#define MPLM_EXTERN_H_

/*=========================================================================

            M O D E M   P O W E R   L O G   M A N A G E R

              E X T E R N A L   H E A D E R   F I L E


  GENERAL DESCRIPTION
    This file contains the external interface functions for the client outside of
    MODEM SW and definitions for Modem Power Log Manager (MPLM).

  EXTERNALIZED FUNCTIONS
    MPLM_Init

  INITIALIZATION AND SEQUENCING REQUIREMENTS
    Invoke the MPLM_Init function to initialize the MPLM.

        Copyright (c) 2021 by QUALCOMM Technologies, Inc.  All Rights Reserved.



==========================================================================*/
/*==========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ---------------------------------------------------------
05/15/2014 sz      Initial version.
==========================================================================*/
/* =========================================================================
**  Function : MPLM_Init
** =========================================================================*/
/**
  This function is called once at powerup. This function initializes MPLM variables.

  @param
  None.

  @return
  None.

  @dependencies
  TBD.

*/
extern void MPLM_Init(void);

/* =========================================================================
**  Function : MPLM_Task
** =========================================================================*/
/**
  This function is to start a task from RCINIT.

  @param
  None.

  @return
  None.

  @dependencies
  TBD.

*/
extern void MPLM_Task(void);
#endif /* MPLM_EXTERN_H_ */
