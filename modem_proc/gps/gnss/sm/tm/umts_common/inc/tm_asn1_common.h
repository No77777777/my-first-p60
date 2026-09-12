#ifndef TM_ASN1_COMMON_H
#define TM_ASN1_COMMON_H
/*===========================================================================
                          TM_ASN1_COMMON.H

DESCRIPTION
  This header file contains common data structure and data type independent
  of either OSS or OSYS based ASN1 encode/decode methods.

  Copyright (c) 2010 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 

  Export of this technology or software is regulated
  by the U.S. Government. Diversion contrary to U.S. law prohibited.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/umts_common/inc/tm_asn1_common.h#1 $
  $DateTime: 2019/10/24 01:30:36 $
  $Author: pwbldsvc $


===========================================================================*/
/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
typedef enum
{
  PROTO_RRC,
  PROTO_PRESUPL,
  PROTO_SUPL,
  PROTO_RRLP,
  PROTO_IS801B,
  PROTO_LPP,
  PROTO_LPP_E
} tm_asn1_protocol_type;


typedef struct {
    long           length;
    unsigned char *value;
} tm_pdu_buf_type;

#endif /* TM_ASN1_COMMON_H */
