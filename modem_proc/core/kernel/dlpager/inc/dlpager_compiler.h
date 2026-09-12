#pragma once
/**
  @file dlpager_compiler.h
  @brief Compiler specific macros, declarations etc...

 EXTERNALIZED FUNCTIONS
  none

 INITIALIZATION AND SEQUENCING REQUIREMENTS
  none

 Copyright (c) 2014  by Qualcomm Technologies, Inc.  All Rights Reserved.
 Confidential and Proprietary - Qualcomm Technologies, Inc.
 =============================================================================*/ 
/*==============================================================================
 
                        EDIT HISTORY FOR FILE
 
    This section contains comments describing changes made to the
    module. Notice that changes are listed in reverse chronological
    order.
   
 
 
  when         who     what, where, why
  ---------- -------- ----------------------------------------------------------
  2014-10-03 anandj   Created header

 =============================================================================*/
#if !defined( DLPAGER_CONSTRUCTOR )
/** @brief Forces the associated symbol to be treated as a constructor for
           global object that is invoked before main() */
#define DLPAGER_CONSTRUCTOR __attribute__ ((__constructor__))
#endif
