#ifndef TDSRRCF9DRIVER_H
#define TDSRRCF9DRIVER_H
/*===========================================================================

            M S M 5 2 0 0  F 9  ( I N T E G R I T Y   E N G I N E ) 
                  E X T E R N A L   H E A D E R   F I L E

DESCRIPTION

  This header file contains all the external definitions for 
  The F9 (Integrity Protection Engine) driver.
  
  Copyright (c) 2010-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcf9driver.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who   what, where, why
--------   ---   ----------------------------------------------------------
03/10/12   hx     Merged to VU_MODEM_WCDMA_NIKEL_3_0_MHCI.00.00.00.00.08.04
03/10/11   hx     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/

#include "tdscdma_variation.h"
#include "comdef.h"
#include "customer.h"
#include "tdsrrcmm_v.h"
#include "tdsuecomdef.h"
/*===========================================================================

                DEFINITIONS AND DECLARATIONS

===========================================================================*/


/* A 64-bit structure to help with endian issues */

typedef union {
  uint32 b32[2];
  uint16 b16[4];
  uint8 b8[8];
} tdsrrc_register64;

           
typedef enum
{
  TDSRRC_INTEGRITY_DIR_UPLINK,  /* Uplink message        */
  TDSRRC_INTEGRITY_DIR_DOWNLINK /* Downlink message      */
}tdsrrc_integrity_direction_type;

                  
/*===========================================================================

  FUNCTION RRCF9_KEY_SCHEDULE

  DESCRIPTION

    This function builds the key schedule.  Most "key" operations use 16-bit
    subkeys so  build uint16-sized arrays that are "endian" correct.

  DEPENDENCIES
    None.

  RETURN VALUE
    None

===========================================================================*/

void tdsrrcf9_key_schedule
(
  uint8* key
);
/*===========================================================================

  FUNCTION RRCF9_KASUMI

  DESCRIPTION
    This function is implementation of KASUMI, the core algorithm for the
 	  3GPP Confidentiality and Integrity algorithms.
 
 	  This has been coded for clarity, not necessarily for efficiency.
 
 	  This will compile and run correctly on both Intel (little endian)
 	  and Sparc (big endian) machines. (Compilers used supported 32-bit ints

  DEPENDENCIES
    None.

  RETURN VALUE
    None

===========================================================================*/

void tdsrrcf9_kasumi 
(
  uint8* data 
);
/*===========================================================================

  FUNCTION RRCF9_PERFORM_F9_ALGO

  DESCRIPTION
    This function is implementation of f9, the 3GPP Confidentiality algorithm.
      
    This has been coded for clarity, not necessarily for efficiency.

    This will compile and run correctly on both Intel (little endian)
    and Sparc (big endian) machines. (Compilers used supported 32-bit ints)

  DEPENDENCIES

    None.

  RETURN VALUE

    None

===========================================================================*/
uint8* tdsrrcf9_perform_f9_algo
(
  uint8* key,
  int count,
  int fresh,
  int dir,
  uint8* data,
  int length
);


/*===========================================================================

  FUNCTION tdsrrcf9_init_key_values

  DESCRIPTION

    This function performs initialization for the F9 device
    for each RRC connection. Each time an RRC connection
    is eastablished, once the input parameters are availabe
    this function should be called.

    The following are initialized by this function -
  
    Integrity Key - 128 bit key used by the F9 engine. The
    key is different for CS connections and PS connections.
  
   
  DEPENDENCIES

    None.

  RETURN VALUE

    None

===========================================================================*/
void tdsrrcf9_init_key_values
( 
  byte *cs_key_value,     /* Integrity Key value for PS domain */
  byte *ps_key_value      /* Integrity Key value for CS domain */
);
/*===========================================================================

  FUNCTION tdsrrcf9_init_fresh_per_connection

  DESCRIPTION

    This function performs initialization for the F9 device
    for each RRC connection. Each time an RRC connection
    is eastablished, once the input parameters are availabe
    this function should be called.

    The following are initialized by this function -
  
    FRESH - FRESH is a 32 bit value unique to a UE. It's given
    by the UTRAN in the security mode command.
   
   
  DEPENDENCIES

    None.

  RETURN VALUE

    None

===========================================================================*/
void tdsrrcf9_init_fresh_per_connection
( 
  uint32 fresh_value      /* Value of Fresh */
);
/*===========================================================================

  FUNCTION tdsrrcf9_update_key_value

  DESCRIPTION

    This function performs updates the key_value for the
    domain for which it is changed.  This is called whenever
    RRC_CIPHER_REQ is received from MM layer to update the
    key stored in the engine.

    The following are initialized by this function -
  
    Integrity Key - 128 bit key used by the F9 engine. The
    key is different for CS connections and PS connections.
  
    CN_DOMAIN - is the domain id for which the key has
    changed.   
   
  DEPENDENCIES

    None.

  RETURN VALUE

    None

===========================================================================*/
void tdsrrcf9_update_key_value
( 
  rrc_cn_domain_identity_e_type  cn_domain, /*domain id */
  byte *key_value                           /* Key value         */
);
/*===========================================================================

  FUNCTION RRCF9_GENERATE_MAC_FOR_MESSAGE

  DESCRIPTION

    This function passes the encoded RRC message and the
  additional parameters that are unique to the message 
  to the integrity engine. It returns the generated
  MAC-I value.
  
  This function is to be used for both downlink and 
  uplink messages.
  

  DEPENDENCIES

    None.

  RETURN VALUE

    A uint32 value that represents the MAC-I (or XMAC-I) for
  this message.

===========================================================================*/
uint32 tdsrrcf9_generate_mac_for_message
( 
  uint32 count_i,                           /* COUNT-I value for the RB */
  tdsrrc_integrity_direction_type dir,         /* Direction - uplink or downlink message */
  uint8 *msg,                               /* Pointer to the encoded message.    */
  uint32 msg_length,                        /* Length of the message in bits */
  rrc_cn_domain_identity_e_type  cn_domain,  /* Key for which domain_id to be used */
  uecomdef_umts_integrity_protection_algorithm_e_type integrity_algo /*integrity protection algorithm*/
);

#ifdef FEATURE_TDSCDMA_SNOW3G_SECURITY
/*===========================================================================

  FUNCTION RRCF9_GENERATE_MAC_FOR_MESSAGE

  DESCRIPTION

    This function passes the encoded RRC message and the
  additional parameters that are unique to the message 
  to the integrity engine. It returns the generated
  MAC-I value.
  
  This function is to be used for both downlink and 
  uplink messages.
  

  DEPENDENCIES

    None.

  RETURN VALUE

    A uint32 value that represents the MAC-I (or XMAC-I) for
  this message.

===========================================================================*/
uint32 tdsrrcf9_generate_mac_for_message_using_snow3g
( 
  uint32 count_i,                           /* COUNT-I value for the RB */
  tdsrrc_integrity_direction_type dir,         /* Direction - uplink or downlink message */
  uint8 *msg,                               /* Pointer to the encoded message.    */
  uint32 msg_length,                        /* Length of the message in bits */
  rrc_cn_domain_identity_e_type  cn_domain,  /* Key for which domain_id to be used */
  uecomdef_umts_integrity_protection_algorithm_e_type integrity_algo /*integrity protection algorithm*/
);

#endif /* FEATURE_TDSCDMA_SNOW3G_SECURITY */

#endif /* TDSRRCF9DRIVER_H */
