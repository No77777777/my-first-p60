/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            N V    R - U I M    S U B S Y S T E M    TASK


GENERAL DESCRIPTION

  This source file contains the main NVRUIM task along with supporting functions.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2016 QUALCOMM Technologies, Incorporated and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.  Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header:

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/28/16   ar      Adding support for as_id in NVRUIM APIs for DS
03/15/16   ar      Initial version

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "nvruim_util.h"

/* <EJECT> */
/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
  /* ASCII character */
#define ASCII_A 0x41
#define ASCII_B 0x42
#define ASCII_C 0x43
#define ASCII_D 0x44
#define ASCII_E 0x45
#define ASCII_F 0x46
  
#define ASCII_a 0x61
#define ASCII_b 0x62
#define ASCII_c 0x63
#define ASCII_d 0x64
#define ASCII_e 0x65
#define ASCII_f 0x66
  
#define ASCII_0 0x30
#define ASCII_1 0x31
#define ASCII_2 0x32
#define ASCII_3 0x33
#define ASCII_4 0x34
#define ASCII_5 0x35
#define ASCII_6 0x36
#define ASCII_7 0x37
#define ASCII_8 0x38
#define ASCII_9 0x39

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                                                                         */
/*                         FUNCTION DEFINITIONS                            */
/*                                                                         */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*===========================================================================

FUNCTION NVRUIM_CONVERT_SLOT_ID_TO_INDEX

DESCRIPTION
  Converts the passed MMGSDI slot enum to a slot array index.

DEPENDENCIES
  None

RETURN VALUE
  Boolean to indicate success or failure

SIDE EFFECTS
  None

===========================================================================*/
boolean nvruim_convert_slot_id_to_index
(
  mmgsdi_slot_id_enum_type    mmgsdi_slot_type,
  uint8                     * slot_index_ptr
)
{
  if (slot_index_ptr == NULL)
  {
    return FALSE;
  }

  switch (mmgsdi_slot_type)
  {
    case MMGSDI_SLOT_1:
      *slot_index_ptr = 0;
      break;
    case MMGSDI_SLOT_2:
      *slot_index_ptr = 1;
      break;
    case MMGSDI_SLOT_3:
      *slot_index_ptr = 2;
      break;
    default:
      UIM_MSG_HIGH_1("Invalid slot id: 0x%x", mmgsdi_slot_type);
      return FALSE;
  }
  return TRUE;
} /* nvruim_convert_slot_id_to_index */


/*===========================================================================

FUNCTION NVRUIM_CONVERT_AS_ID_TO_NVRUIM_SESSION

DESCRIPTION
  Converts the passed CM SUB_ID to NVRUIM session_type

DEPENDENCIES
  None

RETURN VALUE
  nvruim_session_enum_type

SIDE EFFECTS
  None

===========================================================================*/
nvruim_session_enum_type nvruim_convert_as_id_to_nvruim_session
(
  sys_modem_as_id_e_type        as_id
)
{
  nvruim_session_enum_type nvruim_session = NVRUIM_1X_PROV_PRI_SESSION;

  switch (as_id)
  {
    case SYS_MODEM_AS_ID_1:
      nvruim_session = NVRUIM_1X_PROV_PRI_SESSION;
      break;

    case SYS_MODEM_AS_ID_2:
      nvruim_session = NVRUIM_1X_PROV_SEC_SESSION;
      break;

    case SYS_MODEM_AS_ID_3:
      nvruim_session = NVRUIM_1X_PROV_TER_SESSION;
      break;

    default:
      break;
  }
  return nvruim_session;
} /* nvruim_convert_as_id_to_nvruim_session */


/*===========================================================================

FUNCTION NVRUIM_MAP_CONTEXT_TO_NVRUIM_SESSION

DESCRIPTION
  Converts the passed context to NVRUIM session_type

DEPENDENCIES
  None

RETURN VALUE
  nvruim_session_enum_type

SIDE EFFECTS
  None

===========================================================================*/
nvruim_session_enum_type nvruim_map_context_to_nvruim_session
(
  nvruim_nv_context_type   nvruim_context
)
{
  switch (nvruim_context)
  {
    case NVRUIM_NV_CONTEXT_PRIMARY:
      return NVRUIM_1X_PROV_PRI_SESSION;
  
    case NVRUIM_NV_CONTEXT_SECONDARY:
      return NVRUIM_1X_PROV_SEC_SESSION;
  
    case NVRUIM_NV_CONTEXT_TERTIARY:
      return NVRUIM_1X_PROV_TER_SESSION;
  
    default:
      break;
  }
  return NVRUIM_1X_PROV_PRI_SESSION;
}/* nvruim_map_context_to_nvruim_session */


/*===========================================================================

FUNCTION NVRUIM_CONVERT_SESSION_TO_AS_ID

DESCRIPTION
  Converts the passed NVRUIM session_type to CM as_id

DEPENDENCIES
  None

RETURN VALUE
  sys_modem_as_id_e_type

SIDE EFFECTS
  None

===========================================================================*/
sys_modem_as_id_e_type nvruim_convert_session_to_as_id
(
  nvruim_session_enum_type        nvruim_session
)
{
  sys_modem_as_id_e_type as_id = SYS_MODEM_AS_ID_1;

  switch (nvruim_session)
  {
    case NVRUIM_1X_PROV_PRI_SESSION:
      as_id = SYS_MODEM_AS_ID_1;
      break;

    case NVRUIM_1X_PROV_SEC_SESSION:
      as_id = SYS_MODEM_AS_ID_2;
      break;

    case NVRUIM_1X_PROV_TER_SESSION:
      as_id = SYS_MODEM_AS_ID_3;
      break;

    default:
      break;
  }
  return as_id;
} /* nvruim_convert_session_to_as_id */


/*===========================================================================

FUNCTION NVRUIM_CONVERT_CONTEXT_TO_AS_ID

DESCRIPTION
  Converts the passed NVRUIM context to CM as_id

DEPENDENCIES
  None

RETURN VALUE
  sys_modem_as_id_e_type

SIDE EFFECTS
  None

===========================================================================*/
sys_modem_as_id_e_type nvruim_convert_context_to_as_id
(
  nvruim_nv_context_type              context
)
{
  sys_modem_as_id_e_type as_id = SYS_MODEM_AS_ID_1;

  switch (context)
  {
    case NVRUIM_NV_CONTEXT_PRIMARY:
      as_id = SYS_MODEM_AS_ID_1;
      break;

    case NVRUIM_NV_CONTEXT_SECONDARY:
      as_id = SYS_MODEM_AS_ID_2;
      break;

    case NVRUIM_NV_CONTEXT_TERTIARY:
      as_id = SYS_MODEM_AS_ID_3;
      break;

    default:
      break;
  }
  return as_id;
} /* nvruim_convert_context_to_as_id */


/*===========================================================================

FUNCTION NVRUIM_MAP_NVRUIM_SESSION_TO_CONTEXT

DESCRIPTION
  Converts the passed NVRUIM session_type to context

DEPENDENCIES
  None

RETURN VALUE
  uint16

SIDE EFFECTS
  None

===========================================================================*/
nvruim_nv_context_type nvruim_map_nvruim_session_to_context
(
  nvruim_session_enum_type       nvruim_session
)
{
  switch (nvruim_session)
  {
    case NVRUIM_1X_PROV_PRI_SESSION:
      return NVRUIM_NV_CONTEXT_PRIMARY;
  
    case NVRUIM_1X_PROV_SEC_SESSION:
      return NVRUIM_NV_CONTEXT_SECONDARY;
  
    case NVRUIM_1X_PROV_TER_SESSION:
      return NVRUIM_NV_CONTEXT_TERTIARY;
  
    default:
      break;
  }
  return NVRUIM_NV_CONTEXT_PRIMARY;
}/* nvruim_map_nvruim_session_to_context */


#ifdef FEATURE_UIM_SUPPORT_LBS
/*===========================================================================

FUNCTION nvruim_get_ipv4_digit_char_string

DESCRIPTION
  IPv4 addresses are usually written in dot-decimal notation, which consists
  of the four octets of the address expressed in decimal and separated by
  periods. For example: 123.0.0.121.
  Data are stored in card as 8-bit ASCII string, where each character
   is store in one byte information. For example the given string is store as
   31 32 33 25 (123.)  First  Sub Address
   30 25       (0.)    Second Sub Address
   30 25       (0.)    Third  Sub Address
   31 32 31    (0.)    Fourth Sub Address
  This function will get IPv4 address byte from EF data buffer and return
  4 hex byte information. so for given example, The result will be 0x7B000079
  where First Sub Adress is 123  -> 7B,
        Second Sub Address is 0  -> 0,
        Third Sub Address is  0  -> 0,
        Forth Sub Address is 121 -> 79,
  Note: Maximum length of a subaddress is 3.
DEPENDENCIES
  None.

RETURN VALUE
  0:  When data buffer does not have valid ipv4 address data.
  IPv4 Hex data: When data buffer have valid ipv4 address.

SIDE EFFECTS
  None

===========================================================================*/
uint32 nvruim_get_ipv4_digit_char_string
(
  const byte *data_ptr,
  byte        data_len
)
{
  /* we are only interested in 4 sub address */
  byte   sub_address[4]        = {0x00, 0x00, 0x00, 0x00};
  byte   data_index            = 0;
  uint16 address               = 0;
  byte   index                 = 0;
  byte   sub_address_length    = 0;
  uint32 ipv4_address          = 0;

  if((NULL == data_ptr) || (0 == data_len))
  {
    UIM_MSG_ERR_0(" There is no data to parse ipv4 address");
    return ipv4_address;
  }

  /* Read all '.' index and get the length of sub address
     0x2E: is ascii of '.' character,
     (index < 4): check for first 4 sub address
     */
  for (data_index = 0; (data_index < data_len) && (index < 4); data_index++)
  {
    if (data_ptr[data_index] == 0x2E)
    {
      /* Read Next offset */
      index++;
      sub_address_length=0;
      address =0;
    }
    else
    {
      /* Maximum 3 digit is allowed to express a sub address */
      if (sub_address_length > 3)
      {
        UIM_MSG_ERR_1(" Sub Address length [0x%x] is greater than 3 byte",
                      sub_address_length);
        return ipv4_address;
      }
      address = (uint16)(address * 10) + (data_ptr[data_index] & 0x0F);
      /* Value of sub address is vary from 0x00 to 0xFF */
      if (address > 0xFF)
      {
        UIM_MSG_ERR_2("Sub Address[0x%x]  0x%x is greater than 0xFF",
                      index, address);
        return ipv4_address;
      }
      sub_address[index]= address & 0xFF;
      sub_address_length++;
    }
  }/* for (data_index = 0; data_index < data_len; data_index++) */

  /* Reading sub address */
  for (index = 0; index < 4; index++)
  {
    ipv4_address = ipv4_address << 0x08;
    ipv4_address |= sub_address[index];
  } /* end of  for (index = 0; index < 4; index++)*/
  return ipv4_address;
} /* nvruim_get_ipv4_digit_char_string */


/*===========================================================================

FUNCTION nvruim_get_ipv6_hex_char_string

DESCRIPTION
   IPv6 have eight groups of 4 hex digits separated by colons.
   it also can have 2 colon ‘::’ together but only once is allowed in entire string.
   For example, all are pointing to same addresss:
   2001:0db8:0000:0000:0000:0000:1428:57ab
   2001:0db8:0000:0000:0000::1428:57ab
   2001:0db8:0:0:0:0:1428:57ab
   2001:0db8:0:0::1428:57ab
   2001:0db8::1428:57ab
   2001:db8::1428:57ab

   Data are stored in card as 8-bit ASCII string, where each character is
   store in one byte information. for example given string 2001:db8::1428:57ab
   is stored as
   32 30 30 31 3A -> 2001:
   64 62 38 3A 3A -> db8::
   31 34 32 38 3A -> 1428:
   35 37 61 62    -> 57ab

   This function will get IPv6 address byte from EF data buffer and stored in
   array of 8 word index. So for given example, output will be
   Ipv6[0] = 0x2001
   Ipv6[1] = 0x0db8
   Ipv6[2] = 0x0000
   Ipv6[3] = 0x0000
   Ipv6[4] = 0x0000
   Ipv6[5] = 0x0000
   Ipv6[6] = 0x1428
   Ipv6[7] = 0x57ab

DEPENDENCIES
  None.

RETURN VALUE
  TRUE:  When data buffer have valid ipv6 address data.
  FALSE: When data buffer will not have valid ipv6 address.

SIDE EFFECTS
  None

===========================================================================*/
boolean nvruim_get_ipv6_hex_char_string
(
  const byte  *data_ptr,
  byte         data_len,
  uint16      *ipv6_address_ptr
)
{
  /* max sub address length is 4 as 4 hex digit is allowed */
  byte sub_address_length    = 0;
  /* Store ipv6 address */
  uint16 ipv6add[8];
  byte data_index            = 0;
  /* Maximum 7 colons can be defined in ipv6 address string */
  byte remaining_colon       = 7;

  /* check for :: presence in ef data */
  byte double_colon_index    = 0;
  byte index                 = 0;

  if((NULL == data_ptr) || (0 == data_len) || (NULL == ipv6_address_ptr))
  {
    UIM_MSG_ERR_0(" Any of the Input parameters is NULL ");
    return FALSE;
  }
  memset((uint16 *)(ipv6add),0x0000, sizeof(ipv6add));

  /* Find out all ':'index, 0x3A is ascii of ':'
     (index < 8) : check for first 8 sub address */
  for (data_index = 0; (data_index < data_len) && (index < 8); data_index++)
  {
    if (0x3A == data_ptr[data_index])
    {
      /* ':' is found */
      index = index +1;
      /* set sub address length to 0 as new sub address will be start after : */
      sub_address_length = 0;
      /* decrement remaining colon as colon is found */
      if(remaining_colon > 0)
      {
        remaining_colon = remaining_colon -1;
      }

      /* double colon is allowed in ef data, so if we did not get all
         7 colon then, Check next index */
      if ((0x3A == data_ptr[data_index +1]) && (remaining_colon > 0))
      {
        /* if next index have colon */
        if(0x00 == double_colon_index)
        {
          /* set double colon index */
          double_colon_index = index + 1;
        }
        else
        {
          /* if we already found double colon then return error as
             Only one double colons is allowed */
          UIM_MSG_ERR_0("Only one double colon allowed ");
          return FALSE;
        }
      } /*  if (data_ptr[data_index +1] == 0x3A)*/
    }
    else
    {
      /* Read data_ptr if it is not colon */
      /* max sub address length is 4 as 4 hex digit is allowed for a sub address*/
      if (sub_address_length > 0x04)
      {
        UIM_MSG_ERR_0("Sub Address [0x%x] is more than 4 byte");
        return FALSE;
      }
      /* Increment sub address length */
      sub_address_length++;

      ipv6add[index] = (uint16)(ipv6add[index] << 0x04);
      switch(data_ptr[data_index])
      {
        case ASCII_A:
        case ASCII_a:
          ipv6add[index] = ipv6add[index] | 0x0A;
          break;

        case ASCII_B:
        case ASCII_b:
          ipv6add[index] = ipv6add[index] | 0x0B;
          break;

        case ASCII_C:
        case ASCII_c:
          ipv6add[index] = ipv6add[index] | 0x0C;
          break;

        case ASCII_D:
        case ASCII_d:
          ipv6add[index] = ipv6add[index] | 0x0D;
          break;

        case ASCII_e:
        case ASCII_E:
          ipv6add[index] = ipv6add[index] | 0x0E;
          break;

        case ASCII_F:
        case ASCII_f:
          ipv6add[index] = ipv6add[index] | 0x0F;
          break;

        case ASCII_0:
        case ASCII_1:
        case ASCII_2:
        case ASCII_3:
        case ASCII_4:
        case ASCII_5:
        case ASCII_6:
        case ASCII_7:
        case ASCII_8:
        case ASCII_9:
          /* 0x31 -- 0x39 */
          ipv6add[index] = ipv6add[index] | (data_ptr[data_index] & 0x0F);
          break;

        default:
          UIM_MSG_ERR_1("Char 0x%x is not allowed in IPv6 address",
                        data_ptr[data_index]);
          return FALSE;
      }/* switch(data_ptr[data_index]) */
    }/* end of else */
  } /* end of for */

  /* For given example  2001:0db8::1428:57ab the ip address contain following data
     ipv6add[0] = 0x2001, ipv6add[1]= 0x0db8,  ipv6add[2]= 0x0000,  ipv6add[3]=1428
     and  ipv6add[4] = 0x57ab */

  /* Check for remaining colon */
  if (remaining_colon)
  {
    /* we are not getting 7 colons in data string */
    /* check for double colon index */
    if (0x00 == double_colon_index)
    {
      UIM_MSG_ERR_1("Invalid format of IPv6  missing colons 0x%x",
                    remaining_colon);
      return FALSE;
    }
    /* case when double_colon_index is found, then we need to add 0 for remainin
       colon, such that we get the 7 colon in string.
       for example : 2001:0db8::1428:57ab this string will treat as
       2001:0db8:0:0:0:0:1428:57ab */

    /* index is always pointing to the last sub part address here and double colon
       index is never 0. Now we are moving data to right from double colon index
       to IPV6 higher byte index (7).
       for given string: index = 4 (found 4 colon), double colon index is 2.
       so in this loop we are moving IPV6[4] to IPV6[7] at last and IPV6[3] to IPV6[6]
       and IPV6[2] to IPV6[5].
       */
    for (data_index = 0x07; (index >= double_colon_index) && (index < 8); data_index --)
    {
      ipv6add[data_index] = ipv6add[index];
      ipv6add[index] = 0x0000;
      index = index - 1;
    }

    /* We have shifted the higher data to higher IPV6 address index, which come
       after double colon index. Now data_index is pointing to current index
       Here we are setting 0 for remaining index which is in between data_index
       and double colon index.
       For example data_index = 4 and double colon = 2 */
    while ((remaining_colon > 0) && (data_index > double_colon_index))
    {
      ipv6add[data_index] = 0x0000;
      data_index --;
      remaining_colon --;
    }
  }/* if (remaining_colon) */

  /* Parse entire data string */
  memscpy((void *)ipv6_address_ptr,
          sizeof(ipv6add),
          (const void *)ipv6add,
          sizeof(ipv6add));
  return TRUE;
}/* nvruim_get_ipv6_hex_char_string */
#endif /* FEATURE_UIM_SUPPORT_LBS */
