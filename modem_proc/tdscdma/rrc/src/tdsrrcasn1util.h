#ifndef TDSRRCASN1UTIL_H
#define TDSRRCASN1UTIL_H
/*===========================================================================
                          R R C A S N 1 U T I L

DESCRIPTION
  This header file contains function prototypes for the functions in 
  tdsrrcasn1util.h.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcasn1util.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------

===========================================================================*/


/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "tdsuecomdef.h"
#include "tdsrrcasn1.h"
#include "tdsrrc_dependancy.h"
#include "tdsrrcdata.h"


/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

typedef struct {
    long           length;
    unsigned char *value;
} tdspdu_buf_type;

/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

extern OSCTXT tdsenc_ctxt;
typedef struct
{
  OSCTXT dec_context;
  boolean isfree;
  int pdu_num;              /* Added parameters pdu_num,state,msg_type
                             * in order to debug which PDU is leaking and 
                             * what state are we in*/
  tdsrrc_state_e_type state;
  uint8 msg_type;
  void * dec_ptr_address;
}tdsrrcasn1util_obj_ctxt_list_type;

#define TDSRRC_MAX_NUM_ASN1_CTXT 40
extern tdsrrcasn1util_obj_ctxt_list_type tdsdec_ctxt_list[TDSRRC_MAX_NUM_ASN1_CTXT];

#ifdef FEATURE_3GPP_CSFB_TDS
#define TDSRRC_MAX_NUM_SYS_INFO_CONT_SIBS 10
typedef struct
{
  OSCTXT enc_context;
  boolean isfree;
  void * enc_ptr_address;
} tdsrrcasn1util_obj_enc_ctxt_list_type;
#endif

/* =======================================================================
**                          Macro Definitions
** ======================================================================= */



/* =======================================================================
**                        Function Declarations
** ======================================================================= */


/*===========================================================================

FUNCTION RRCASN1_ENCODE_PDU

DESCRIPTION
  This function ASN.1 encodes a passed signalling message. Note that space for
  the encoded message is dynamically allocated and must be freed by calling 
  tdsrrcasn1_free_buf().

DEPENDENCIES
  The memory manager and the ASN.1 subsystem must have been previously 
  initialized.

RETURN VALUE
  This function returns a tdspdu_buf_type. If encoding failed, pdu_buf.value will
  be NULL and pdu_buf.length is meaningless. If encoding succeeded, 
  pdu_buf.value will be non-NULL and pdu_buf.length will contain the number 
  of bytes in the encoded PDU.

SIDE EFFECTS
  None.

===========================================================================*/

tdspdu_buf_type tdsrrcasn1_encode_pdu
(
  void *unencoded_pdu_ptr,    // The pointer to the PDU to encode
  int pdu_num                 // The PDU number to encode
);


/*===========================================================================

FUNCTION RRCASN1_DECODE_PDU

DESCRIPTION
  This function ASN.1 decodes a passed signalling message. Note that the
  decoded PDU is dynamically allocated and needs to be freed by calling 
  tdsrrcasn1_free_pdu().

DEPENDENCIES
  The memory_manager and the ASN.1 subsystem must have been previously 
  initialized.

RETURN VALUE
  NULL if an error occurred during decoding, otherwise a pointer to the
  decoded PDU.

SIDE EFFECTS
  None.

===========================================================================*/

void *tdsrrcasn1_decode_pdu
(  
  void *encoded_pdu_ptr,      // The pointer to the PDU to decode
  int pdu_num,                // The PDU number to decode
  uint32 encoded_pdu_size     // The size (in bytes) of the PDU
);


/*===========================================================================

FUNCTION RRCASN1_DECODE_PDU_WITH_BUF

DESCRIPTION
  This function ASN.1 decodes a passed signalling message. Note that the
  decoded PDU is not dynamically allocated and needs to be freed by calling 
  rrcasn1_free_pdu_with_buf().

DEPENDENCIES
  The memory_manager and the ASN.1 subsystem must have been previously 
  initialized.

RETURN VALUE
  NULL if an error occurred during decoding, otherwise a pointer to the
  decoded PDU.

SIDE EFFECTS
  None.
===========================================================================*/

void *tdsrrcasn1_decode_pdu_with_buf
(  
  void *encoded_pdu_ptr,      // The pointer to the PDU to decode
  int pdu_num,                // The PDU number to decode
  uint32 encoded_pdu_size,     // The size (in bytes) of the PDU
  uint32 length
);
/*===========================================================================

FUNCTION RRCASN1_FREE_BUF

DESCRIPTION
  This function frees memory that was allocated by tdsrrcasn1_encode_pdu().

DEPENDENCIES
  The memory_manager and the ASN.1 subsystem must have been previously 
  initialized.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrrcasn1_free_buf
(
  void *buf                 // Pointer to the buffer to free
);


/*===========================================================================

FUNCTION RRCASN1_FREE_PDU

DESCRIPTION
  This function frees memory that was allocated by tdsrrcasn1_decode_pdu().

DEPENDENCIES
  The memory_manager and the ASN.1 subsystem must have been previously 
  initialized.

RETURN VALUE
  Non-zero if an error occurred; else zero.

SIDE EFFECTS
  None.

===========================================================================*/

int tdsrrcasn1_free_pdu
(
  int pdunum,               // The PDU number to be freed
  void *buf                 // The PDU to be freed
);

/*===========================================================================

FUNCTION RRCASN1_GET_NUMBER_OF_PAD_BITS

DESCRIPTION
  This function returns the number of padding bits applied to the most
  recently encoded message..

DEPENDENCIES
  The memory_manager and the ASN.1 subsystem must have been previously 
  initialized.

RETURN VALUE
  Returns the number of padding bits appended to the most recently
  encoded Message.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 tdsrrcasn1_get_number_of_pad_bits
(
  void
);

 /*===========================================================================
 
 FUNCTION tdsrrc_enc_UE_RadioAccessCapabilityInfo_PDU
 
 DESCRIPTION
   This function Encodes the UE Radio Access capbilityInfo PDU
 
 DEPENDENCIES
 None
 
 RETURN VALUE
	 NULL if an error occurred during decoding, otherwise a pointer to the
	 encoded PDU.
 
 SIDE EFFECTS
   None.
 ===========================================================================*/

uint32  tdsrrc_enc_UE_RadioAccessCapabilityInfo_PDU
(
    void * unencoded_pdu_ptr,
    tdsrrc_InterRATHandoverInfo_present * lbitstr
);

/*===========================================================================

FUNCTION tdsrrc_get_free_asn1_ctxt_idx

DESCRIPTION
  This function gets the first available free cntxt for decoding PDU

DEPENDENCIES
None

RETURN VALUE
  	Context index.

SIDE EFFECTS
  None.
===========================================================================*/

uint8 tdsrrc_get_free_asn1_ctxt_idx
(
  void
);
/*===========================================================================

FUNCTION tdsrrc_init_asn1_ctx

DESCRIPTION
  This function init the Cntx used for objective system for the decoded Msg

DEPENDENCIES
None

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/

void tdsrrc_init_asn1_ctx
(
  void
);
 /*===========================================================================

FUNCTION tdsrrc_free_asn1_ctxt_idx

DESCRIPTION
  This function frees the context index passed, and makes it available for further use.

DEPENDENCIES
None

RETURN VALUE
	None

SIDE EFFECTS
  None.
===========================================================================*/

void tdsrrc_free_asn1_ctxt_idx
(
  uint8 index
);

/*===========================================================================

FUNCTION tdsrrc_get_free_asn1_ctxt_idx

DESCRIPTION
  This function gets the context index allocated for the ASn1 ptr passed

DEPENDENCIES
None

RETURN VALUE
	Context index.

SIDE EFFECTS
  None.
===========================================================================*/

uint8 tdsrrc_get_used_ctxt_idx_for_asn1_ptr
(
  void * ptr_adress
);

/*===========================================================================

FUNCTION rrc_init_asn1_encode_ctx

DESCRIPTION
  This function init asn1 context for re-encoding sibs 

DEPENDENCIES
  The memory_manager and the ASN.1 subsystem must have been previously 
  initialized.

RETURN VALUE
  Non-zero if an error occurred; else zero.

SIDE EFFECTS
  None.
  
===========================================================================*/
void tdsrrc_init_asn1_encode_ctx
(
  void
);

/*===========================================================================
 
FUNCTION rrcasn1_free_re_encoded_sibs

DESCRIPTION
  This function frees memory that was allocated re-encoded sibs.

DEPENDENCIES
  The memory_manager and the ASN.1 subsystem must have been previously 
  initialized.

RETURN VALUE
  Non-zero if an error occurred; else zero.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 tdsrrcasn1_free_re_encoded_sibs
(
  int pdu_num,               // The PDU number to be freed
  void *buf                 // The PDU to be freed
);

#endif /* TDSRRCASN1UTIL_H */
