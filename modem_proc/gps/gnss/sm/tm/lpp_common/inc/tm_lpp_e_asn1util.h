#ifndef TM_LPP_E_ASN1UTIL_H
#define TM_LPP_E_ASN1UTIL_H

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "customer.h"
#ifdef FEATURE_GNSS_LPPE
#include "comdef.h"
#include "tm_lpp_asn1util.h"

void tm_lpp_e_init_encoding_context();

/*===========================================================================
  tm_lpp_e_osys_asn1_encode_epdu
    
  Description:
    This function ASN.1 encodes a passed signalling message. 
    This function returns a pdu_buf_type via an output parameter. If encoding
    failed, pdu_buf.value will be NULL and pdu_buf.length is meaningless. If
    encoding succeeded, pdu_buf.value will be non-NULL and pdu_buf.length will
    contain the number of bytes in the encoded PDU.

    
   Parameters:
    unencoded_pdu_ptr - Pointer to the unencoded PDU
    encoded_pdu - This is a variable of type pdu_buf_type and needs to
      be declared by the caller and passed here as a parameter. If the
      encoding operation succeeds, then the value field of this structure will
      point to the encoded PDU and the length field will indicate the size of
      the encoded PDU in bytes
    param pdu_num - Type of the message being decoded. 
 
    Returns:

    int - status of the encode operation
=========================================================================*/

int tm_lpp_e_osys_asn1_encode_epdu 
( 
  void      *unencoded_pdu_ptr,
  tm_pdu_buf_type *pdu_buf_ptr,
  int pdu_num
);

/* ===========================================================================
  tm_lpp_e_osys_asn1_decode_epdu
 
  Description: 
    This function ASN.1 decodes a passed signalling message. 

    The pointer to decoded PDU is set to NULL if an error occurs, otherwise
    points to the decoded PDU
 
 
  Parameters 
    encoded_pdu_ptr - Pointer to the encoded PDU
    encoded_pdu_size - Size in bytes of the encoded message
    decoded_pdu_ptr - A NULL pointer declared to be type of the
      message being decoded, and must be passed in by the caller
    pdu_num - Type of the message being decoded. Must be one of
      the 'OSYS PDU Types' defined in lte_rrc_osys_asn1util.h
 
  Returns: 
    int - status of the decode operation
===========================================================================*/
void tm_lpp_e_osys_asn1_decode_epdu
(
  const void   *p_encoded_epdu_ptr,
  size_t  encoded_epdu_size,
  void   *p_epdu_decoded,
  int    *p_decode_status_ptr
);

/*===========================================================================

  FUNCTION:  tm_lpp_e_asn1_free_osys_buf

  Description:
    This function frees memory that was allocated by
    tm_osys_asn1_encode_pdu()

 
  Returns: 
    None
===========================================================================*/
void tm_lpp_e_asn1_free_osys_buf
(
  void *buf
);
  


/*===========================================================================

  FUNCTION:  tm_lpp_e_asn1_osys_IE_free

  Description:

    This function is called by the Objective Systems ASN.1 libraries in order
    to free dynamically allocated memory for individual elements within the PDU

  Returns:
    void
===========================================================================*/
void tm_lpp_e_asn1_osys_IE_free
(
  void **ptr    /*!< Pointer to memory location to be freed */
);

/*===========================================================================*/
/*!
    @brief
    This function is called by the Objective Systems ASN.1 libraries in order
    to free the dynamic memory for individual elements within the PDU while a
    PDU is being constructed by hand.

    @return
    void
*/
/*=========================================================================*/

void tm_asn1_free_lpp_e_encode_message_IEs
(
  tm_encode_OMA_LPPe_MessageExtension *p_lpp_e_pdu, 
  int pdu_num
);

/*===========================================================================

  FUNCTION:  tm_lpp_e_asn1_osys_IE_realloc

  Description
    This function is called by the Objective Systems ASN.1 libraries in order
    to allocate dynamic memory for individual elements within the PDU while a
    PDU is being constructed by hand.

  Returns:
    void
=========================================================================*/
void * tm_lpp_e_asn1_osys_IE_realloc
(
  void *ptr,    /*!< Pointer to memory location to reallocate */
  size_t size   /*!< Size in bytes to be allocated */
);

/*===========================================================================

  FUNCTION:  tm_lpp_e_asn1_osys_IE_malloc

  Descrpition:
    This function is called by the Objective Systems ASN.1 libraries in order
    to allocate dynamic memory for individual elements within the PDU while a
    PDU is being constructed by hand.

  Returns:
    void
=========================================================================*/
void * tm_lpp_e_asn1_osys_IE_malloc
(
  size_t size   /*!< Size in bytes to be allocated */
);

void tm_lpp_init_encoding_context(void);
void tm_lpp_e_init_encoding_context(void);

#endif
#endif /* ifdef LPPE*/
