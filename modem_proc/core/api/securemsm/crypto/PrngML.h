#ifndef __PRNGML_H__
#define __PRNGML_H__
/*===========================================================================

                       P R N G E n g i n e D r i v e r

                       H e a d e r  F i l e (e x t e r n a l)

DESCRIPTION
  This header file contains HW Crypto specific declarations.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None
  
Copyright (c) 2010 - 2014 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/api/securemsm/crypto/PrngML.h#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $ 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/17/14   nk      Api document related changes
07/25/10   yk      Initial version
============================================================================*/
#include "comdef.h"

/**
    Prng Main Library error codes.
*/
typedef enum
{
    PRNGML_ERROR_NONE,            /**< Success.             */
    PRNGML_ERROR_BUSY,            /**< Busy.                */
    PRNGML_ERROR_FAILED,          /**< Failed.              */
    PRNGML_ERROR_INVALID_PARAM,   /**< Invalid Input Param. */
    PRNGML_ERROR_DMOV,            /**< Data Mover error.    */
    PRNGML_ERROR_UNSUPPORTED,     /**< Unsupported.         */
    PRNGML_ERROR_BAD_STATE        /**< Bad State.           */
} PrngML_Result_Type;

/**
   This function returns random number from hardware 
   random number generator.  

   @param[in/out] random_ptr     Random number pointer
   @param[in]     random_len     Length of random number 

   @return  PrngML_Resut_Type
 */
 PrngML_Result_Type PrngML_getdata
(
  uint8*  random_ptr,
  uint16  random_len
);

/**
   This function returns random number and can be used when Mutex,
   DAL framework are not yet initialised.  

   @param[in/out] random_ptr     Random number pointer
   @param[in]     random_len     Length of random number 

   @return  PrngML_Resut_Type
 */
PrngML_Result_Type PrngML_getdata_lite
(
  uint8*  random_ptr,
  uint16  random_len
);

#endif /*__PRNGML_H__*/
