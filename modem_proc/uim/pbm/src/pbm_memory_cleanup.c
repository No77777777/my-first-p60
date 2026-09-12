/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                 PBM - MEMORY CLEANUP RELATED FUNCTIONS


GENERAL DESCRIPTION

  This source file contains functions for freeing up the memory allocated
  and held by PBM during its normal operation in off-target environment.
  This is called only after all PBM tasks have stopped

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

                        COPYRIGHT INFORMATION

Copyright (c) 2015 - 2016 QUALCOMM Techologies, Inc (QTI) and its licensors.  All Rights
Reserved.  QUALCOMM Techologies Proprietary.  Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/12/16    nr     Legacy API cleanup
04/18/16    sp     ECC only support in PBM
04/13/16    sp     Reduce global memory used by PBM
02/05/16    nr     Update ECC cache and send ready after NV Refresh
12/07/15    sp     Remove unused variables
11/12/15    lm     Remove compiler warnings
09/30/15    lm     Initial version
===========================================================================*/
/*===========================================================================

                         INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_msg.h"
#include "pbm.h"
#include "pbmcache.h"
#include "pbmutils.h"
#include "pbmuim.h"
#include "pbmgsdi.h"
#include "pbmlib.h"
#include "pbmtask_i.h"
#include "pbmtask.h"
#include "pbm_nv_refresh.h"

/*
Note : The functions in the  file are used only when FEATURE_UIM_TEST_FRAMEWORK
enabled. */

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */