#ifndef QPD_H
#define QPD_H
/*=============================================================================

                 qpd.h -- H E A D E R  F I L E

GENERAL DESCRIPTION
   Prototypes of qpd API

EXTERNAL FUNCTIONS
   None.

INITIALIZATION AND SEQUENCING REQUIREMENTS
   None.

      Copyright (c) 2010
                    by Qualcomm Technologies Incorporated.  All Rights Reserved.

=============================================================================*/

#include <qtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

/*=============================================================================
                      TYPEDEFS                                
=============================================================================*/

typedef void * qpd_t;

typedef uint8_t qprocessor_t;

/*=============================================================================
                      FUNCTION DECLARATIONS                                
=============================================================================*/
/**
 * Get current PD
 *
 * @return Current PD handle
 */
static inline qpd_t qpd_myself(void) { return NULL; }

/**
 * Get processor identifier from qpd_t
 *  
 * @return  processor identifier from qpd_t
 */
// qprocessor_t qpd_myprocessor( qpd_t my_pd );

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif /* QPD_H */
