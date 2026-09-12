#ifndef QURT_FUTEX_H
#define QURT_FUTEX_H
/*======================================================================
                        qurt_futex.h

GENERAL DESCRIPTION
  Prototypes of Kernel futex API functions      
  Futex calls directly in case you want to play with them from C
  
 EXTERNALIZED FUNCTIONS
  none

 INITIALIZATION AND SEQUENCING REQUIREMENTS
  none

 Copyright (c) 2009-2011
             by Qualcomm Technologies, Inc. All Rights Reserved.
 ======================================================================*/

/*=====================================================================
 Functions
======================================================================*/

int qurt_futex_wait(void *lock, int val);
int qurt_futex_wake(void *lock, int n_to_wake);

#endif /* QURT_FUTEX_H */

