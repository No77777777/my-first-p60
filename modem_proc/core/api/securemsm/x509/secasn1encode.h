#ifndef SECASN1ENCODE_H 
#define SECASN1ENCODE_H 

/** 
  @file secasn1encode.h
  @brief ASN.1 DER encoding.
  This file contains the definitions for the constants, data structures and 
  interfaces that provide ASN.1 DER encoding.
*/
/*===========================================================================
NOTE: The @brief description and any detailed descriptions above do not appear 
      in the PDF. 

      The secx509_mainpage.dox file contains all file/group descriptions that
      are in the output PDF generated using Doxygen and Latex. To edit or 
      update any of the file/group text in the PDF, edit the 
      secx509_mainpage.dox file or contact Tech Pubs.

      The above description for this file is part of the secx509_apis group 
      description in the secx509_mainpage.dox file. 
===========================================================================*/
/*===========================================================================
  Copyright (c) 2018 Qualcomm Technologies Incorporated.
  All rights reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*==========================================================================
          EDIT HISTORY FOR FILE

  $Header: //components/rel/core.mpss/10.0/api/securemsm/x509/secasn1encode.h#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/01/18   fn     Initial version

==========================================================================*/


/*==========================================================================

             Include Files for Module

==========================================================================*/
#include "comdef.h"
/** @addtogroup secx509_apis 
  @{ */

/*--------------------------------------------------------------------------
                Function Declarations
--------------------------------------------------------------------------*/
/** @addtogroup secx509_apis 
  @{ */

/**
  Starts the ASN.1 DER encoding by creating an initial container for the 
  next_field() function.

  @param[in] data         Pointer to the data.
  @param[in,out] ret_ptr  Pointer to the returned data to start the ASN.1 block.

  @return
  E_ASN1_SUCCESS     -- Sequence is properly started. \n
  E_ASN1_INVALID_ARG -- Pointer arguments have a NULL value. \n
  E_ASN1_INVALID_TAG -- No sequence tag at the beginning.

  @dependencies
  None.
*/
extern secasn1_err_type secasn1encode_start
(
  uint8 *input_data,
  uint32 data_len,
  uint8 tag_id,
  secasn1_data_type *ret_ptr
);

/**
  Closes and verifies the ASN.1 DER encoding by checking that no 
  data has been left unprocessed at the end of the stream.

  @param[in] data_ptr   Pointer to the end of the data holder.

  @return
  E_ASN1_SUCCESS      -- Document has been closed successfully. \n
  E_ASN1_INVALID_ARG  -- Pointer argument has a NULL value. \n 
  E_ASN1_INVALID_DATA -- All the data was not processed. 

  @dependencies
  None.
*/
extern secasn1_err_type secasn1encode_end
(
  secasn1_data_type *data_ptr
);

/**
  This function creates issuer dn and issuer key SHA1 hashes for ocsp request
 
  @param[in] issuer_dn Issuer domain name of server certificate
  @param[in] issuer_name_len Name of issuer dn
  @param[in] issuer_key Public key of issuer of server certificate
  @param[in] issuer_key_len Length of issuer key
  @param[in,out] hash_issuer_dn Ocsp request output
  @param[in,out] req_buffer_len Length of ocsp request_output
 
  @return
  E_ASN1_SUCCESS     -- Hash is computed successfully. \n
  E_ASN1_INVALID_ARG -- Pointer arguments have a NULL value. \n

  @dependencies
   Issuer key and domain name are parsed from server cert and given in the
   input parameters.User should know SHA1 hash output are 20
   bytes
*/
extern secasn1_err_type secasn1encode_gen_ocsp_hashes
(
  uint8*           issuer_dn,
  uint16           issuer_dn_len,
  uint8*           issuer_key,
  uint16           issuer_key_len,
  uint8*           hash_issuer_dn,
  uint8*           hash_issuer_key 
);

#endif /* SECASN1ENCODE_H */
