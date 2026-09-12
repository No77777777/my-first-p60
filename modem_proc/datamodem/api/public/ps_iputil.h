#ifndef PS_IPUTIL_H
#define PS_IPUTIL_H
/**
   @file ps_iputil.h
   @brief contains declarations for utility functions used by the IP module. 
 */

/*===========================================================================

                   I P   I N T E R N A L   U T I L I T Y

                           F U N C T I O N S

DESCRIPTION
  This file contains declarations for internal utility functions
  used by the IP module.

Copyright (c) 2002-2011 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*=============================================================================
NOTE: The @brief description and any detailed descriptions above do not appear 
      in the PDF. 

      The dms_mainpage.dox file contains all file/group descriptions 
      that are in the output PDF generated using Doxygen and Latex. To edit or 
      update any of the file/group text in the PDF, edit the 
      dms_mainpage.dox file or contact Tech Pubs.

      The above description for this file is part of the ipUtilities group 
      description in the dms_mainpage.dox file. 
=============================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $PVCSPath: L:/src/asw/MM_DATA/vcs/ps_iputil.h_v   1.1   31 Jan 2003 18:20:00   ubabbar  $
  $Header: //components/rel/data.mpss/3.5..2.1/api/public/ps_iputil.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/02/15   cx      Fix bug when subtract 0 cksum from 0 cksum in cksum_sub 
11/11/14   cx      Added support for checksum offload
10/20/14   xc      Added cksum_update
08/28/11   sd      (Tech Pubs) Applied new Doxygen grouping markup.
07/20/11   gs      Removed inclusion of <target.h>
12/14/08   pp      Common Modem Interface: Public/Private API split.
10/13/04   vp      Removed inclusion of ps_iphdr.h and included ps_ip4_hdr.h.
                   Removed ntohip() and htonip() functions. Added declaration
                   of buffer_checksum() function.
11/11/03   rc/aku  Added parameter 'offset' to the cksum() function.
01/31/03   usb     Added an extra arg to ntohip() to support hdr copying only
07/25/02   om      Created module from ip.h

===========================================================================*/
/* group: ipUtilities */
/*===========================================================================

                                INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "dsm.h"
#include "ps_pkt_info.h"

/*=============================================================================
MACRO CKSUM_ADD()
=============================================================================*/
/** @ingroup cksum_add
  This MACRO will add two cksum together

  @param[in] cksum_1  the first checksum
  @param[in] cksum_2  the second checksum

  @return
  16-bit checksum.\n 

  @dependencies
  None.
  @newpage
*/
#define cksum_add(cksum_1, cksum_2)\
  (~ eac((~(cksum_1)&0xffff) + (~(cksum_2)&0xffff)))

/*=============================================================================
MACRO CKSUM_SUB()
=============================================================================*/
/** @ingroup cksum_sub
  This function will substract the cksum from the other cksum
  The passed in value should be checksum value instead of one's complement sum
  Checksum is one's complement of one's complement sum.

  @param[in] minuend      the number being substracted from
  @param[in] subtrahend   the number being substracted

  @return
  16-bit checksum.\n 

  @dependencies
  None.
  @newpage
*/
INLINE uint16 cksum_sub
(
  uint16  minuend,
  uint16  subtrahend
)
{
  minuend = ~minuend;
  subtrahend = ~subtrahend;

  if(minuend > subtrahend)
  {
    return (uint16)(~(minuend - subtrahend));
  }
  else if(minuend < subtrahend)
  {
    return (uint16)(~((minuend - subtrahend - 1) & 0xFFFF));
  }
  else
  {
    return 0;
  }
}

/*=============================================================================
FUNCTION EAC()
=============================================================================*/
/** @ingroup eac
  This function will perform end-around-carry adjustment

  @param[in] sum     the checksum value

  @return
  16-bit checksum.\n 

  @dependencies
  None.
  @newpage
*/
uint16 eac
(
  register uint32 sum
);

/*=============================================================================
FUNCTION BUFFER_CKSUM()
=============================================================================*/
/** @ingroup buffer_cksum
  Compute the cksum of the passed buffer of data.

  @param[in] buf            Pointer to consecutive buffer to be checksummed.
  @param[in] count          Count of number of bytes in the buffer.

  @return
  16-bit checksum.\n 

  @dependencies
  None.
  @newpage
*/
uint16 buffer_cksum
(
  uint16 *buf,
  uint16 count
);

/*=============================================================================
FUNCTION CKSUM()
=============================================================================*/
/** @ingroup cksum
  This function calculates the IP checksum.

  @param[in] ph       Pseudo header.
  @param[in] m        IP packet (DSM Pointer)
  @param[in] len      IP header length
  @param[in] offset   DSM offset
  
  @return
  16-bit checksum.\n 

  @dependencies
  None.
  @newpage
*/
uint16 cksum
(
  pseudo_header_type  *ph,
  struct dsm_item_s    *m,
  uint16               len,
  uint16               offset
);

/*=============================================================================
FUNCTION CKSUM_UPDATE()
=============================================================================*/
/** @ingroup cksum
  This function calculates the new IP checksum given an old checksum and 
  old and new pseudo headers

  @param[in] old_ph   Old pseudo header
  @param[in] new_ph   New pseudo header
  @param[in] old_cksum  Old checksum
  
  @return
  16-bit checksum.\n 

  @dependencies
  None.
  @newpage
*/
uint16 cksum_update
(
  pseudo_header_type  *old_ph,  /* old pseudo header */
  pseudo_header_type   *new_ph,  /* old pseudo header */
  uint16                old_cksum /* old cksum */
);

/*=============================================================================
FUNCTION LCSUM()
=============================================================================*/
/** @ingroup lcsum
  This function computes the 1's-complement sum of the given data buffer.
  This uses a inline assembly routine of the processor which reduces
  CPU utilization greatly. The inline assembly uses an unwound loop method 
  to reduce iterations of loop.

  @param[in] buf            Pointer to consecutive buffer to be checksummed.
  @param[in] count          Count of number of bytes in the buffer.
  
  @return
  The ones complement sum of the bytes in the buffer.\n 

  @dependencies
  None.
  @newpage
*/
uint16 lcsum
(
  uint16 *buf,
  uint16 cnt
);

/*=============================================================================
Funtion CKSUM_EX()
=============================================================================*/
/** @ingroup cksum_sub
  This function will substract the cksum from the other cksum

  @param[in] pkt_info_ptr     pointer points to the packet info
  @param[in] ph_ptr           pointer points to the pseduo header
  @param[in] m                pointer points to dsm item
  @param[in] len              the length of data  
  @param[in] offset           the offset in dsm item to calculate the cksum
  
  @return
  16-bit checksum.\n 

  @dependencies
  None.
  @newpage
*/
uint16 cksum_ex
(
  ip_pkt_info_type    *pkt_info_ptr,
  pseudo_header_type  *ph_ptr,
  struct dsm_item_s    *m,
  uint16               len,
  uint16               offset
);


/*=============================================================================
FUNCTION CKSUM_OFFLOAD_INIT()
=============================================================================*/
/** @ingroup cksum_offload_init
  This function will trigger the checksum offload based on the NV 
  configuration

  @return 

  @dependencies
  None.
  @newpage
*/
void cksum_offload_init(void);

#endif  /* PS_IPUTIL_H */
