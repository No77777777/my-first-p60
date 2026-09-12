/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        H I T   Q U E U E    S E R V I C E S

GENERAL DESCRIPTION

  A hit_queue is a simple data structure used for logically storing and re-
  trieving data blocks, in a first in - first out (FIFO) order without
  physically copying them.

          +-------+     +-------+     +-------+
          | DATA  |     | DATA  |     | DATA  |
          | BLOCK |     | BLOCK |     | BLOCK |
          +-------+     +-------+     +-------+
   |----->| LINK  |---->| LINK  |---->| LINK  |---- |
   |      +-------+     +-------+     +-------+     |
   |                                                |
   |------------------------------------------------|

  The Queue Services provide a small set of declarations and functions for
  placing a data block at the tail of a queue and removing a data block from
  the head of a queue.

  The HIT Queue Services impose no restrictions on the size of structure of 
  data blocks used with with queues. This allows software to pass virtually 
  any type of data on queues.

EXTERNALIZED FUNCTIONS
  hitqueue_init
  hitqueue_put
  hitqueue_get

REGIONAL FUNCTIONS
  None
  
INITIALIZATION AND SEQUENCING REQUIREMENTS
  Prior to use, a call to hitqueue_init should be made.

  Copyright (c) 2006, 2007 by Qualcomm Technologies Incorporated.  All Rights Reserved.
  
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                              Edit History

$Header: //components/rel/hdr.mpss/6.0/hit/src/hitqueue.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when         who     what, where, why
----------   ---     --------------------------------------------------------
01/15/2013   smd     Featurized hit cmd and hit diag.
05/07/2007   rmg     Changed dynamic memory allocation to static one
03/28/2007   vh      Created

===========================================================================*/

/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/
#include "hdr_variation.h"
#include "customer.h"

#ifdef FEATURE_HIT
#endif /* FEATURE_HIT */
