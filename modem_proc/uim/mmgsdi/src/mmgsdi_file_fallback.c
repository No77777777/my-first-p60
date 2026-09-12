/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

            M M G S D I   F I L E  F A L L B A C K   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the processing of all MMGSDI file fallback functions
 
EXTERNALIZED FUNCTIONS
  None

INITIALIZATION AND SEQUENCING REQUIREMENTS

                        COPYRIGHT INFORMATION

Copyright (c) 2019, 2021 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_file_fallback.c#3 $$ $DateTime: 2021/09/09 04:03:08 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/01/21   rps     Add support for GSM FPLMN in fs fallback
09/07/21   cj      Changed default EF AD last byte to 2 indicating MNC length
09/16/19   sg      Initial Revision

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "mmgsdi_nv.h"
#include "mmgsdiutil.h"
#include "mmgsdi_file_fallback.h"
#include "mmgsdi_file.h"
#include "mmgsdicache.h"
#include "intconv.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Size of the File Info Table */
#define MMGSDI_FILE_INFO_TABLE_SIZE          \
  (sizeof(mmgsdi_file_info_table)/sizeof(mmgsdi_file_info_table[0]))

/*=============================================================================

                       STRUCTURE DECLARATIONS

=============================================================================*/

/* ----------------------------------------------------------------------------
   STRUCTURE:      MMGSDI_FILE_SEC_INFO_TABLE

   DESCRIPTION:
     The structure contains security info on the SIM files
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  /* Read Protection method */
  mmgsdi_file_security_enum_type       read_protection;
  mmgsdi_pin_enum_type                 read_protection_pin;
  /* Write Protection method */
  mmgsdi_file_security_enum_type       write_protection;
  mmgsdi_pin_enum_type                 write_protection_pin;
  /* Increase Protection method */
  mmgsdi_file_security_enum_type       increase_protection;
  mmgsdi_pin_enum_type                 increase_protection_pin;
  /* Activate Protection method */
  mmgsdi_file_security_enum_type       rehabilitate_activate_protection;
  mmgsdi_pin_enum_type                 rehabilitate_activate_protection_pin;
  /* Deactivate Protection method */
  mmgsdi_file_security_enum_type       invalidate_deactivate_protection;
  mmgsdi_pin_enum_type                 invalidate_deactivate_protection_pin;
}mmgsdi_file_sec_info_table_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      MMGSDI_FILE_INFO_TABLE_TYPE

   DESCRIPTION:
     The structure contains info on the SIM files for which fallback
     to cache or File System is supported
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  /* File name */
  mmgsdi_file_enum_type                          file;
  /* File attribute length of the file */
  uint8                                          file_attr_length;  
  /* File attribute for the file */
  const uint8*                                   default_attr_value_ptr;  
  /* File security access */
  const mmgsdi_file_sec_info_table_type*         security_access_ptr;
  /* File length of the file */
  uint8                                          file_length;
  /* Default value for the file - length would be stored in
     file_length field */
  const uint8*                                   default_value_ptr;
  /* Type of file table entry */
  mmgsdi_file_info_table_entry_categ_enum_type   table_entry_categ;
  /* Type of file */
  mmgsdi_file_structure_enum_type                file_type;
} mmgsdi_file_info_table_type;

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/


/*===========================================================================

                        FILES DEFAULT VALUES USIM

===========================================================================*/
/* USIM EF-MWIS Default Value */
static const uint8 
  mmgsdi_file_mwis_default[] = {0x00,0x00,0x00,0x00,0x00,0x00};


/* USIM EF-FPLMN Default Value */
static const uint8
  mmgsdi_file_fplmn_default[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

/* USIM EF-KEYS Default Value */
static const uint8
  mmgsdi_file_keys_default[] = {0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                0xFF,0xFF,0xFF};

/* USIM EF-KEYSPS Default Value */
static const uint8
  mmgsdi_file_keysps_default[] = {0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                  0xFF,0xFF,0xFF};

/* USIM EF-AD initial Default Value, 4th byte of data has 0x02 default MNC length.
   The 4th byte of data will be updated later in mmgsdi_card_init_update_ad_mnc_len function */
static const uint8
  mmgsdi_file_usim_ad_default[] = {0x00,0x00,0x00,0x02};

/* USIM EF-ACC initial Default Value, these values will be updated later per
   mmgsdi_card_init_validate_acc_data function */
static const uint8
  mmgsdi_file_usim_acc_default[] = {0x00,0x00};

/* 3GPP TS 31.102 Rel 13 defines services until the 14th byte.
   30 bytes defined as default value to accomodate future expansion */
static const uint8
  mmgsdi_file_ust_default[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                               0x00,0x00,0x00,0x00,0x00,0x00};

/*===========================================================================

                        FILES DEFAULT VALUES GSM

===========================================================================*/
/* GSM EF-KC Default Value */
static const uint8
  mmgsdi_file_gsm_kc_default[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x07};

/* GSM EF-AD Default Value, the 4th byte of data will be updated later per
   mmgsdi_card_init_update_ad_mnc_len function */
static const uint8
  mmgsdi_file_gsm_ad_default[] = {0x00,0x00,0x00,0x00};

/* GSM EF-ACC Default Value, these values will be updated later per
   mmgsdi_card_init_validate_acc_data function */
static const uint8
  mmgsdi_file_gsm_acc_default[] = {0x00,0x00};

/* GSM EF-SST Default Value */
static const uint8
  mmgsdi_file_sst_default[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                               0x00,0x00,0x00,0x00,0x00};

/* GSM EF-FPLMN Default Value */
static const uint8
  mmgsdi_file_gsm_fplmn_default[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                     0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};


/*===========================================================================

                        FILES ATTRIBUTES DEFAULT USIM

===========================================================================*/
/* EF-MWIS File Attribute Value */
static const uint8 
  mmgsdi_file_mwis_attr[]      = {0x62,0x40,
                                  /* File Descriptor: Record Length - 6,
                                     Number of records - 1 */
                                  0x82,0x05,0x02,0x21,0x00,0x06,0x01,
                                  /* File Identifier */
                                  0x83,0x02,0x6F,0xCA,
                                  /* Life Cycle Status Integer */
                                  0x8A,0x01,0x05,
                                  /* Security Attributes - Expanded format */
                                  0xAB,0x2C,
                                  /* Read Access */
                                  0x80,0x01,0x01,0xA4,0x06,0x83,0x01,0x01,0x95,0x01,0x08,
                                  /* Update Access */
                                  0x80,0x01,0x02,0xA4,0x06,0x83,0x01,0x01,0x95,0x01,0x08,
                                  /* Activate Access */
                                  0x80,0x01,0x10,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                                  /* Deactivate Access */
                                  0x80,0x01,0x08,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                                  /* File Size */
                                  0x80,0x02,0x00,0x06};

/* EF-FPLMN File Attribute Value */
static const uint8
  mmgsdi_file_fplmn_attr[]    = {0x62,0x3D,
                                 /* File Descriptor: Transparent file */
                                 0x82,0x02,0x01,0x1E,
                                 /* File Identifier */
                                 0x83,0x02,0x6F,0x7B,
                                 /* Life Cycle Status Integer */
                                 0x8A,0x01,0x05,
                                 /* Security Attributes - Expanded format */
                                 0xAB,0x2C,
                                 /* Read Access */
                                 0x80,0x01,0x01,0xA4,0x06,0x83,0x01,0x01,0x95,0x01,0x08,
                                 /* Update Access */
                                 0x80,0x01,0x02,0xA4,0x06,0x83,0x01,0x01,0x95,0x01,0x08,
                                 /* Activate Access */
                                 0x80,0x01,0x10,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                                 /* Deactivate Access */
                                 0x80,0x01,0x08,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                                 /* File Size */
                                 0x80,0x02,0x00,0x3C};

/* EF-KEYS File Attribute Value */
static const uint8
  mmgsdi_file_keys_attr[]    = {0x62,0x3D,
                                /* File Descriptor: Transparent file */
                                0x82,0x02,0x01,0x21,
                                /* File Identifier */
                                0x83,0x02,0x6F,0x08,
                                /* Life Cycle Status Integer */
                                0x8A,0x01,0x05,
                                /* Security Attributes - Expanded format */
                                0xAB,0x2C,
                                /* Read Access */
                                0x80,0x01,0x01,0xA4,0x06,0x83,0x01,0x01,0x95,0x01,0x08,
                                /* Update Access */
                                0x80,0x01,0x02,0xA4,0x06,0x83,0x01,0x01,0x95,0x01,0x08,
                                /* Activate Access */
                                0x80,0x01,0x10,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                                /* Deactivate Access */
                                0x80,0x01,0x08,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                                /* File Size */
                                0x80,0x02,0x00,0x21};

/* EF-KEYSPS File Attribute Value */
static const uint8
   mmgsdi_file_keysps_attr[]    = {0x62,0x3D,
                                   /* File Descriptor: Transparent file */
                                   0x82,0x02,0x01,0x21,
                                   /* File Identifier */
                                   0x83,0x02,0x6F,0x09,
                                   /* Life Cycle Status Integer */
                                   0x8A,0x01,0x05,
                                   /* Security Attributes - Expanded format */
                                   0xAB,0x2C,
                                   /* Read Access */
                                   0x80,0x01,0x01,0xA4,0x06,0x83,0x01,0x01,0x95,0x01,0x08,
                                   /* Update Access */
                                   0x80,0x01,0x02,0xA4,0x06,0x83,0x01,0x01,0x95,0x01,0x08,
                                   /* Activate Access */
                                   0x80,0x01,0x10,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                                   /* Deactivate Access */
                                   0x80,0x01,0x08,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                                   /* File Size */
                                   0x80,0x02,0x00,0x21};

/* EF-KC File Attribute Value */
static const uint8
  mmgsdi_file_usim_kc_attr[]    = {0x62,0x3D,
                                   /* File Descriptor: Transparent file */
                                   0x82,0x02,0x01,0x09,
                                   /* File Identifier */
                                   0x83,0x02,0x4F,0x20,
                                   /* Life Cycle Status Integer */
                                   0x8A,0x01,0x05,
                                   /* Security Attributes - Expanded format */
                                   0xAB,0x2C,
                                   /* Read Access */
                                   0x80,0x01,0x01,0xA4,0x06,0x83,0x01,0x01,0x95,0x01,0x08,
                                   /* Update Access */
                                   0x80,0x01,0x02,0xA4,0x06,0x83,0x01,0x01,0x95,0x01,0x08,
                                   /* Activate Access */
                                   0x80,0x01,0x10,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                                   /* Deactivate Access */
                                   0x80,0x01,0x08,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                                   /* File Size */
                                   0x80,0x02,0x00,0x09};

/* EF-KCGPRS File Attribute Value */
static const uint8
  mmgsdi_file_usim_kcgprs_attr[]    = {0x62,0x3D,
                                       /* File Descriptor: Transparent file */
                                       0x82,0x02,0x01,0x09,
                                       /* File Identifier */
                                       0x83,0x02,0x4F,0x52,
                                       /* Life Cycle Status Integer */
                                       0x8A,0x01,0x05,
                                       /* Security Attributes - Expanded format */
                                       0xAB,0x2C,
                                       /* Read Access */
                                       0x80,0x01,0x01,0xA4,0x06,0x83,0x01,0x01,0x95,0x01,0x08,
                                       /* Update Access */
                                       0x80,0x01,0x02,0xA4,0x06,0x83,0x01,0x01,0x95,0x01,0x08,
                                       /* Activate Access */
                                       0x80,0x01,0x10,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                                       /* Deactivate Access */
                                       0x80,0x01,0x08,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                                       /* File Size */
                                       0x80,0x02,0x00,0x09};

/* USIM EF-AD File Attribute Value */
static const uint8
  mmgsdi_file_usim_ad_attr[]    = {0x62,0x37,
                                   /* File Descriptor: Transparent file */
                                   0x82,0x02,0x01,0x04,
                                   /* File Identifier */
                                   0x83,0x02,0x6F,0xAD,
                                   /* Life Cycle Status Integer */
                                   0x8A,0x01,0x05,
                                   /* Security Attributes - Expanded format */
                                   0xAB,0x26,
                                   /* Read Access */
                                   0x80,0x01,0x01,0x90,0x00,
                                   /* Update Access */
                                   0x80,0x01,0x02,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                                   /* Activate Access */
                                    0x80,0x01,0x10,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                                   /* Deactivate Access */
                                   0x80,0x01,0x08,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                                   /* File Size */
                                   0x80,0x02,0x00,0x04};

/* USIM EF-ACC File Attribute Value */
static const uint8
  mmgsdi_file_usim_acc_attr[]    = {0x62,0x3D,
                                    /* File Descriptor: Transparent file */
                                    0x82,0x02,0x01,0x02,
                                    /* File Identifier */
                                    0x83,0x02,0x6F,0x78,
                                    /* Life Cycle Status Integer */
                                    0x8A,0x01,0x05,
                                    /* Security Attributes - Expanded format */
                                    0xAB,0x2C,
                                    /* Read Access */
                                    0x80,0x01,0x01,0xA4,0x06,0x83,0x01,0x01,0x95,0x01,0x08,
                                    /* Update Access */
                                    0x80,0x01,0x02,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                                    /* Activate Access */
                                    0x80,0x01,0x10,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                                    /* Deactivate Access */
                                    0x80,0x01,0x08,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                                    /* File Size */
                                    0x80,0x02,0x00,0x02};

/* USIM EF-UST File Attribute Value */
static const uint8
  mmgsdi_file_ust_attr[]    = {0x62,0x3D,
                               /* File Descriptor: Transparent file,
                               Number of records - 1 */
                               0x82,0x02,0x01,0x1E,
                               /* File Identifier */
                               0x83,0x02,0x6F,0x38,
                               /* Life Cycle Status Integer */
                               0x8A,0x01,0x05,
                               /* Security Attributes - Expanded format */
                               0xAB,0x2C,
                               /* Read Access */
                               0x80,0x01,0x01,0xA4,0x06,0x83,0x01,0x01,0x95,0x01,0x08,
                               /* Update Access */
                               0x80,0x01,0x02,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                               /* Activate Access */
                               0x80,0x01,0x10,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                               /* Deactivate Access */
                               0x80,0x01,0x08,0xA4,0x06,0x83,0x01,0x0A,0x95,0x01,0x08,
                               /* File Size */
                               0x80,0x02,0x00,0x1E};

/*===========================================================================

                        FILES ATTRIBUTES DEFAULT GSM

===========================================================================*/
/* GSM Files Attribute Value per spec 1111-891 section 9.3 in case of an EF */
/* 2 bytes RFU, 2 bytes file size, 2 bytes file id, 1 byte file type, 
   1 byte for record based file(record length multiplied by the number of records of the EF), 
   3 bytes access condition, 1 byte file status, 1 byte length of the following data , data bytes */

/* GSM EF-KC: Info on fallback file in File System */
static const uint8
  mmgsdi_file_gsm_kc_attr[]    =  {0x00,0x00,0x00,0x09,0x6F,0x20,0x04,0x00,0x11,0x00,0x44,0x21,0x02,0x00,0x00};

/* GSM EF-KCGPRS: Info on fallback file in File System */
static const uint8
  mmgsdi_file_gsm_kcgprs_attr[] = {0x00,0x00,0x00,0x09,0x6F,0x52,0x04,0x00,0x11,0x00,0x44,0x21,0x02,0x00,0x00};

/* GSM EF-AD: Info on fallback file in File System */
static const uint8
  mmgsdi_file_gsm_ad_attr[]    =  {0x00,0x00,0x00,0x04,0x6F,0xAD,0x04,0x00,0x04,0x00,0x44,0x21,0x02,0x00,0x00};

/* GSM EF-ACC: Info on fallback file in File System */
static const uint8
  mmgsdi_file_gsm_acc_attr[]    = {0x00,0x00,0x00,0x02,0x6F,0x78,0x04,0x00,0x14,0x00,0x44,0x21,0x02,0x00,0x00};

/* GSM EF-SST: Info on fallback file in File System */
static const uint8
  mmgsdi_file_sst_attr[]    =     {0x00,0x00,0x00,0x0F,0x6F,0x38,0x04,0x00,0x14,0x00,0x44,0x21,0x02,0x00,0x00};

/* GSM EF-FPLMN: Info on fallback file in File System */
static const uint8
  mmgsdi_file_gsm_fplmn_attr[] = {0x00,0x00,0x00,0x0C,0x6F,0x7B,0x04,0x00,0x11,0x40,0x44,0x21,0x02,0x00,0x00};

/*===========================================================================

                                   TABLES

===========================================================================*/

/*===========================================================================
                       MMGSDI_FILE_SEC_INFO_TABLE
===========================================================================*/
/* Contains security access condition and pin type, and be used in mmgsdi_file_info_table,
   from the mmgsdi_file_info_table we can point to this table using the index */

static const mmgsdi_file_sec_info_table_type mmgsdi_file_sec_info_table[] =
{
  /* Read access conditon, read pin, write access condition, write pin
     increase access condition, increase pin, rehabilitate_activate access condition,
     rehabilitate_activate pin, invalidate_deactivate access condition,
     invalidate_deactivate pin */
  /* Files that require PIN verification to read and write */
  {MMGSDI_SINGLE_ALLOWED, MMGSDI_PIN1, MMGSDI_SINGLE_ALLOWED, MMGSDI_PIN1, 
   MMGSDI_SINGLE_ALLOWED, MMGSDI_PIN1, MMGSDI_SINGLE_ALLOWED,MMGSDI_ADM1, 
   MMGSDI_SINGLE_ALLOWED, MMGSDI_ADM1},
  /* Files that are ALWAYS for read, and ADM protected for write */
  {MMGSDI_ALWAYS_ALLOWED, MMGSDI_MAX_PIN_ENUM, MMGSDI_SINGLE_ALLOWED, MMGSDI_ADM1, 
   MMGSDI_ALWAYS_ALLOWED, MMGSDI_MAX_PIN_ENUM, MMGSDI_SINGLE_ALLOWED, MMGSDI_ADM1, 
   MMGSDI_SINGLE_ALLOWED, MMGSDI_ADM1},
  /* Files that are PIN protected for read, and ADM protected for write */
  {MMGSDI_SINGLE_ALLOWED, MMGSDI_PIN1, MMGSDI_SINGLE_ALLOWED, MMGSDI_ADM1,
   MMGSDI_ALWAYS_ALLOWED, MMGSDI_MAX_PIN_ENUM, MMGSDI_SINGLE_ALLOWED, MMGSDI_ADM1, 
   MMGSDI_SINGLE_ALLOWED, MMGSDI_ADM1}
};

/*===========================================================================
                       MMGSDI_FILE_INFO_TABLE
===========================================================================*/
/* ----------------------------------------------------------------------------
                          IMPORTANT NOTE
----------------------------------------------------------------------------
When adding a new file to the MMGSDI File Info table
a. Add an entry to the mmgsdi_file_info_table
b. Create a Default file value array similar to mmgsdi_file_mwis_default
   and link this in mmgsdi_file_info_table_type.default_value_ptr entry
c. Create a Default file attr value array similar to mmgsdi_file_mwis_attr
   and link this in mmgsdi_file_info_table_type.default_attr_value_ptr entry
d. Add an entry to the mmgsdi_file_sec_info_table if the security 
   info is not present in the table and point to the new entry from this table,
   similar to &mmgsdi_file_sec_info_table[0]
 
Contains list of the files and their attributes */
static const mmgsdi_file_info_table_type mmgsdi_file_info_table[] =
{
  /* File name, File Attribute Length      File Attribute Value
     security attributes  default length  default values  fallback type*/

  {MMGSDI_USIM_MWIS, sizeof(mmgsdi_file_mwis_attr), mmgsdi_file_mwis_attr,
   &mmgsdi_file_sec_info_table[0], sizeof(mmgsdi_file_mwis_default), 
   mmgsdi_file_mwis_default, MMGSDI_ENTRY_CATEG_FS_FALLBACK, MMGSDI_TRANSPARENT_FILE},

  {MMGSDI_USIM_FPLMN, sizeof(mmgsdi_file_fplmn_attr), mmgsdi_file_fplmn_attr,
   &mmgsdi_file_sec_info_table[0], sizeof(mmgsdi_file_fplmn_default), 
   mmgsdi_file_fplmn_default, MMGSDI_ENTRY_CATEG_FS_OR_CACHE_FALLBACK, MMGSDI_TRANSPARENT_FILE},

  {MMGSDI_USIM_KEYS, sizeof(mmgsdi_file_keys_attr), mmgsdi_file_keys_attr,
   &mmgsdi_file_sec_info_table[0], sizeof(mmgsdi_file_keys_default), 
   mmgsdi_file_keys_default, MMGSDI_ENTRY_CATEG_FS_OR_CACHE_FALLBACK, MMGSDI_TRANSPARENT_FILE},

  {MMGSDI_USIM_KEYSPS, sizeof(mmgsdi_file_keysps_attr), mmgsdi_file_keysps_attr,
   &mmgsdi_file_sec_info_table[0], sizeof(mmgsdi_file_keysps_default), 
   mmgsdi_file_keysps_default, MMGSDI_ENTRY_CATEG_FS_OR_CACHE_FALLBACK, MMGSDI_TRANSPARENT_FILE},

  {MMGSDI_USIM_UST, sizeof(mmgsdi_file_ust_attr), mmgsdi_file_ust_attr,
   &mmgsdi_file_sec_info_table[2], sizeof(mmgsdi_file_ust_default), 
   mmgsdi_file_ust_default, MMGSDI_ENTRY_CATEG_CACHE_FALLBACK, MMGSDI_TRANSPARENT_FILE},

  {MMGSDI_USIM_AD, sizeof(mmgsdi_file_usim_ad_attr), mmgsdi_file_usim_ad_attr,
   &mmgsdi_file_sec_info_table[1], sizeof(mmgsdi_file_usim_ad_default), 
   mmgsdi_file_usim_ad_default, MMGSDI_ENTRY_CATEG_CACHE_FALLBACK, MMGSDI_TRANSPARENT_FILE},

  {MMGSDI_USIM_ACC, sizeof(mmgsdi_file_usim_acc_attr), mmgsdi_file_usim_acc_attr,
   &mmgsdi_file_sec_info_table[2], sizeof(mmgsdi_file_usim_acc_default), 
   mmgsdi_file_usim_acc_default, MMGSDI_ENTRY_CATEG_CACHE_FALLBACK, MMGSDI_TRANSPARENT_FILE},

  {MMGSDI_GSM_KC, sizeof(mmgsdi_file_gsm_kc_attr), mmgsdi_file_gsm_kc_attr,
   &mmgsdi_file_sec_info_table[0],sizeof(mmgsdi_file_gsm_kc_default), 
   mmgsdi_file_gsm_kc_default, MMGSDI_ENTRY_CATEG_FS_OR_CACHE_FALLBACK, MMGSDI_TRANSPARENT_FILE},

  {MMGSDI_GSM_AD, sizeof(mmgsdi_file_gsm_ad_attr), mmgsdi_file_gsm_ad_attr,
   &mmgsdi_file_sec_info_table[1], sizeof(mmgsdi_file_gsm_ad_default), 
   mmgsdi_file_gsm_ad_default, MMGSDI_ENTRY_CATEG_CACHE_FALLBACK, MMGSDI_TRANSPARENT_FILE},

  {MMGSDI_GSM_ACC, sizeof(mmgsdi_file_gsm_acc_attr), mmgsdi_file_gsm_acc_attr,
   &mmgsdi_file_sec_info_table[2], sizeof(mmgsdi_file_gsm_acc_default), 
   mmgsdi_file_gsm_acc_default, MMGSDI_ENTRY_CATEG_CACHE_FALLBACK, MMGSDI_TRANSPARENT_FILE},

  {MMGSDI_GSM_SST, sizeof(mmgsdi_file_sst_attr), mmgsdi_file_sst_attr,
   &mmgsdi_file_sec_info_table[2], sizeof(mmgsdi_file_sst_default), 
   mmgsdi_file_sst_default, MMGSDI_ENTRY_CATEG_CACHE_FALLBACK, MMGSDI_TRANSPARENT_FILE},
   
  {MMGSDI_GSM_FPLMN, sizeof(mmgsdi_file_gsm_fplmn_attr), mmgsdi_file_gsm_fplmn_attr,
   &mmgsdi_file_sec_info_table[0], sizeof(mmgsdi_file_gsm_fplmn_default), 
   mmgsdi_file_gsm_fplmn_default, MMGSDI_ENTRY_CATEG_FS_OR_CACHE_FALLBACK, MMGSDI_TRANSPARENT_FILE},
};

/*=============================================================================

                                 FUNCTIONS

=============================================================================*/

/*=============================================================================
FUNCTION        MMGSDI_FILE_FALLBACK_GET_FILE_INFO_TABLE_INDEX

DESCRIPTION
  This function retrieves the index in the mmgsdi_file_info_table
  corresponding to the file requested

DEPENDENCIES
  None

LIMITATIONS
  NONE

RETURN VALUE
  MMGSDI_RETURN_ENUM_TYPE

SIDE EFFECTS
  NONE
=============================================================================*/
static mmgsdi_return_enum_type  mmgsdi_file_fallback_get_file_info_table_index(
  mmgsdi_session_id_type    session_id,
  const mmgsdi_access_type *access_ptr,
  uint8                    *index_ptr
)
{
  uint8                       table_index = 0;
  mmgsdi_file_enum_type       file        = MMGSDI_MAX_FILE_ENUM;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL_2(access_ptr, index_ptr);

  if(access_ptr->access_method == MMGSDI_BY_PATH_ACCESS)
  {
    file = mmgsdi_util_convert_path_to_enum(session_id,
                                            access_ptr->file.path_type);
  }
  else if(access_ptr->access_method == MMGSDI_EF_ENUM_ACCESS)
  {
    file = access_ptr->file.file_enum;
  }

  for(table_index = 0; table_index < MMGSDI_FILE_INFO_TABLE_SIZE; table_index++)
  {
    if(mmgsdi_file_info_table[table_index].file == file)
    {
      *index_ptr = table_index;
      return MMGSDI_SUCCESS;
    }
  }
  return MMGSDI_NOT_FOUND;
} /* mmgsdi_file_fallback_get_file_info_table_index */


/*=============================================================================
FUNCTION    MMGSDI_FILE_FALLBACK_SET_DEFAULT_SEC_ATTR_FROM_FILE_INFO_TABLE

DESCRIPTION
  This function retrieves the default security attributes from
  mmgsdi_file_info_table corresponding to the file requested

DEPENDENCIES
  None

LIMITATIONS
  NONE

RETURN VALUE
  MMGSDI_RETURN_ENUM_TYPE

SIDE EFFECTS
  NONE
=============================================================================*/
mmgsdi_return_enum_type  mmgsdi_file_fallback_set_default_sec_attr_from_file_info_table(
  mmgsdi_file_enum_type              mmgsdi_file,
  mmgsdi_file_security_access_type * operation_ptr
)
{
  uint8                              table_index              = 0;
  uint32                             pin_length_read          = 0;
  uint32                             pin_length_write         = 0;
  uint32                             pin_length_activate      = 0;
  uint32                             pin_length_deactivate    = 0;
  uint32                             pin_length_increase      = 0;

  if (operation_ptr == NULL)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  for(table_index = 0; table_index < MMGSDI_FILE_INFO_TABLE_SIZE; table_index++)
  {
    if(mmgsdi_file_info_table[table_index].file != mmgsdi_file)
    {
      continue;
    }

    if (mmgsdi_file_info_table[table_index].security_access_ptr == NULL)
    {
      return MMGSDI_ERROR;
    }

    /* Protection method */
    operation_ptr->read.protection_method                    = mmgsdi_file_info_table[table_index].security_access_ptr->read_protection;
    operation_ptr->write.protection_method                   = mmgsdi_file_info_table[table_index].security_access_ptr->write_protection;
    operation_ptr->increase.protection_method                = mmgsdi_file_info_table[table_index].security_access_ptr->increase_protection;
    operation_ptr->rehabilitate_activate.protection_method   = mmgsdi_file_info_table[table_index].security_access_ptr->rehabilitate_activate_protection;
    operation_ptr->invalidate_deactivate.protection_method   = mmgsdi_file_info_table[table_index].security_access_ptr->invalidate_deactivate_protection;

    /* Read access */
    if (mmgsdi_file_info_table[table_index].security_access_ptr->read_protection     == MMGSDI_ALWAYS_ALLOWED || 
        mmgsdi_file_info_table[table_index].security_access_ptr->read_protection     == MMGSDI_NEVER_ALLOWED )
    {
      operation_ptr->read.num_protection_pin                   = 0;
      operation_ptr->read.protection_pin_ptr                   = NULL;
    }
    else
    {
      operation_ptr->read.num_protection_pin     = 1;
      pin_length_read = sizeof(mmgsdi_pin_enum_type) * int32touint32(operation_ptr->read.num_protection_pin);
      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(operation_ptr->read.protection_pin_ptr, pin_length_read);
      if(operation_ptr->read.protection_pin_ptr == NULL)
      {
        return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
      }
      *(operation_ptr->read.protection_pin_ptr)  = 
        mmgsdi_file_info_table[table_index].security_access_ptr->read_protection_pin;
    }

    /* Write access */
    if (mmgsdi_file_info_table[table_index].security_access_ptr->write_protection    == MMGSDI_ALWAYS_ALLOWED ||
        mmgsdi_file_info_table[table_index].security_access_ptr->write_protection    == MMGSDI_NEVER_ALLOWED)
    {
      operation_ptr->write.num_protection_pin                  = 0;
      operation_ptr->write.protection_pin_ptr                  = NULL;
    }
    else
    {
      operation_ptr->write.num_protection_pin     = 1;
      pin_length_write = sizeof(mmgsdi_pin_enum_type) * int32touint32(operation_ptr->write.num_protection_pin);
      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(operation_ptr->write.protection_pin_ptr, pin_length_write);
      if(operation_ptr->write.protection_pin_ptr == NULL)
      {
        MMGSDIUTIL_TMC_MEM_FREE(operation_ptr->read.protection_pin_ptr);
        return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
      }
      *(operation_ptr->write.protection_pin_ptr)  = 
        mmgsdi_file_info_table[table_index].security_access_ptr->write_protection_pin;
    }

    /* Increase access */
    if (mmgsdi_file_info_table[table_index].security_access_ptr->increase_protection == MMGSDI_ALWAYS_ALLOWED ||
        mmgsdi_file_info_table[table_index].security_access_ptr->increase_protection == MMGSDI_NEVER_ALLOWED )
    {
      operation_ptr->increase.num_protection_pin               = 0;
      operation_ptr->increase.protection_pin_ptr               = 0;
    }
    else
    {
      operation_ptr->increase.num_protection_pin     = 1;
      pin_length_increase = sizeof(mmgsdi_pin_enum_type) * int32touint32(operation_ptr->increase.num_protection_pin);
      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(operation_ptr->increase.protection_pin_ptr, pin_length_increase);
      if(operation_ptr->increase.protection_pin_ptr == NULL)
      {
        MMGSDIUTIL_TMC_MEM_FREE(operation_ptr->read.protection_pin_ptr);
        MMGSDIUTIL_TMC_MEM_FREE(operation_ptr->write.protection_pin_ptr);
        return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
      }
      *(operation_ptr->increase.protection_pin_ptr)  = 
        mmgsdi_file_info_table[table_index].security_access_ptr->increase_protection_pin;
    }

    /* Rehabilitate_activate access */
    if (mmgsdi_file_info_table[table_index].security_access_ptr->rehabilitate_activate_protection == MMGSDI_ALWAYS_ALLOWED ||
        mmgsdi_file_info_table[table_index].security_access_ptr->rehabilitate_activate_protection == MMGSDI_NEVER_ALLOWED)
    {
      operation_ptr->rehabilitate_activate.num_protection_pin  = 0;
      operation_ptr->rehabilitate_activate.protection_pin_ptr  = NULL;
    }
    else
    {
      operation_ptr->rehabilitate_activate.num_protection_pin     = 1;
      pin_length_activate = sizeof(mmgsdi_pin_enum_type) * int32touint32(operation_ptr->rehabilitate_activate.num_protection_pin);
      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(operation_ptr->rehabilitate_activate.protection_pin_ptr, pin_length_activate);

      if(operation_ptr->rehabilitate_activate.protection_pin_ptr == NULL)
      {
        MMGSDIUTIL_TMC_MEM_FREE(operation_ptr->read.protection_pin_ptr);
        MMGSDIUTIL_TMC_MEM_FREE(operation_ptr->write.protection_pin_ptr);
        MMGSDIUTIL_TMC_MEM_FREE(operation_ptr->increase.protection_pin_ptr);
        return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
      }
      *(operation_ptr->rehabilitate_activate.protection_pin_ptr)  = 
        mmgsdi_file_info_table[table_index].security_access_ptr->rehabilitate_activate_protection_pin;
    }

    /* Invalidate_deactivate access */
    if (mmgsdi_file_info_table[table_index].security_access_ptr->invalidate_deactivate_protection == MMGSDI_ALWAYS_ALLOWED ||
        mmgsdi_file_info_table[table_index].security_access_ptr->invalidate_deactivate_protection == MMGSDI_NEVER_ALLOWED)
    {
      operation_ptr->invalidate_deactivate.num_protection_pin  = 0;
      operation_ptr->invalidate_deactivate.protection_pin_ptr  = NULL;
    }
    else
    {
      operation_ptr->invalidate_deactivate.num_protection_pin     = 1;
      pin_length_deactivate = sizeof(mmgsdi_pin_enum_type) * int32touint32(operation_ptr->invalidate_deactivate.num_protection_pin);
      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(operation_ptr->invalidate_deactivate.protection_pin_ptr, pin_length_deactivate);

      if(operation_ptr->invalidate_deactivate.protection_pin_ptr == NULL)
      {
        MMGSDIUTIL_TMC_MEM_FREE(operation_ptr->read.protection_pin_ptr);
        MMGSDIUTIL_TMC_MEM_FREE(operation_ptr->write.protection_pin_ptr);
        MMGSDIUTIL_TMC_MEM_FREE(operation_ptr->increase.protection_pin_ptr);
        MMGSDIUTIL_TMC_MEM_FREE(operation_ptr->rehabilitate_activate.protection_pin_ptr);
        return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
      }
      *(operation_ptr->invalidate_deactivate.protection_pin_ptr)  = 
        mmgsdi_file_info_table[table_index].security_access_ptr->invalidate_deactivate_protection_pin;
    }

    return MMGSDI_SUCCESS;
  }
  return MMGSDI_NOT_FOUND;
} /* mmgsdi_file_fallback_set_default_sec_attr_from_file_info_table */


/*===========================================================================
FUNCTION         MMGSDI_FILE_FALLBACK_PARSE_UIM_GET_FILE_ATTR_RSP

DESCRIPTION:
  This function will get file attr, 
  and if the file is not found in the card and fallback to cache or
  file system is supported, provide default file attributes

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  MMGSDI_RETURN_ENUM_TYPE

SIDE EFFECTS
  None
===========================================================================*/
mmgsdi_return_enum_type  mmgsdi_file_fallback_parse_uim_get_file_attr_rsp (
  mmgsdi_uim_report_rsp_type              *mod_uim_rsp_ptr,
  const mmgsdi_request_header_type        *req_header_ptr,
  const mmgsdi_client_req_extra_info_type *extra_param_ptr,
  mmgsdi_return_enum_type                  report_status
)
{
  mmgsdi_return_enum_type                        mmgsdi_status     = MMGSDI_SUCCESS;
  mmgsdi_file_info_table_entry_categ_enum_type   table_entry_categ = MMGSDI_ENTRY_CATEG_CACHE_FALLBACK;
  mmgsdi_len_type                                file_attr_length  = 0;
  uint8                                         *file_attr_ptr     = NULL;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL_3(mod_uim_rsp_ptr,
                              req_header_ptr,
                              extra_param_ptr);        
  
  /* If file is not found on card, check if content can be retrieved from
     file system */
  if((report_status != MMGSDI_NOT_FOUND) &&
     (report_status != MMGSDI_UIM_CMD_TIMEOUT))
  {
    return report_status;
  }

  /* Check if file is file and fetaure sim file fallback is supported */
  mmgsdi_status = mmgsdi_file_fallback_get_file_info(extra_param_ptr->get_file_attr_data.access.file.file_enum,
                                                     NULL,
                                                     NULL, 
                                                     &table_entry_categ, 
                                                     NULL,
                                                     &file_attr_length,
                                                     &file_attr_ptr);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(file_attr_ptr);
    return report_status;
  }

  /* If fallback type is FS only and fallback to FS is not supported, return */
  if (table_entry_categ == MMGSDI_ENTRY_CATEG_FS_FALLBACK && 
      mmgsdi_nv_get_device_feature_status(MMGSDI_FEATURE_SIM_FILE_FALLBACK_TO_FS) ==
      MMGSDI_FEATURE_DISABLED)
  {
    MMGSDIUTIL_TMC_MEM_FREE(file_attr_ptr);
    return report_status;
  }

  /* If the table entry category is not a fallback category, return */
  if (table_entry_categ != MMGSDI_ENTRY_CATEG_CACHE_FALLBACK &&
      table_entry_categ != MMGSDI_ENTRY_CATEG_FS_OR_CACHE_FALLBACK &&
      table_entry_categ != MMGSDI_ENTRY_CATEG_FS_FALLBACK)
  {
    MMGSDIUTIL_TMC_MEM_FREE(file_attr_ptr);
    return report_status;
  }

  /* Allocate memory for the File Attributes */
  MMGSDIUTIL_TMC_MEM_FREE(mod_uim_rsp_ptr->rsp_data.data_ptr);
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(mod_uim_rsp_ptr->rsp_data.data_ptr,
                                     file_attr_length);

  /* Copy the file attributes */
  if (mod_uim_rsp_ptr->rsp_data.data_ptr)
  {
    mod_uim_rsp_ptr->rsp_data.data_len = file_attr_length;
    mmgsdi_memscpy(mod_uim_rsp_ptr->rsp_data.data_ptr,
                   mod_uim_rsp_ptr->rsp_data.data_len,
                   file_attr_ptr,
                   file_attr_length);
    mod_uim_rsp_ptr->cmd_transacted = TRUE;
    mod_uim_rsp_ptr->is_uim_success = TRUE;
    mod_uim_rsp_ptr->mmgsdi_error_status = MMGSDI_SUCCESS;
    mod_uim_rsp_ptr->status_word.sw1 = GSDI_SIM_SW1_NORM_END;
    mod_uim_rsp_ptr->status_word.sw2 = GSDI_SIM_SW2_NORM_END;
    report_status = MMGSDI_SUCCESS;      
  }

  MMGSDIUTIL_TMC_MEM_FREE(file_attr_ptr);
  return report_status;
} /* mmgsdi_file_fallback_parse_uim_get_file_attr_rsp */


/* ==========================================================================
FUNCTION:      MMGSDI_FILE_FALLBACK_GET_FILE_INFO

DESCRIPTION:
  This function gets the file length/ file values/ fallback type/filetype
  from MMGSDI File Info table.

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type:
    MMGSDI_SUCCESS:  If file is found in the table 
    MMGSDI_NOT_FOUND: If file is not found in the table 

SIDE EFFECTS:

==========================================================================*/
mmgsdi_return_enum_type mmgsdi_file_fallback_get_file_info(
  mmgsdi_file_enum_type                             file,
  uint8                                            *file_length_ptr,
  const uint8                                     **default_value_pptr,
  mmgsdi_file_info_table_entry_categ_enum_type     *table_entry_categ_ptr,
  mmgsdi_file_structure_enum_type                  *file_type,
  mmgsdi_len_type                                  *file_attr_length_ptr,
  uint8                                           **file_attr_pptr
)
{
  uint8                 table_index = 0;

  for(table_index = 0; table_index < MMGSDI_FILE_INFO_TABLE_SIZE; table_index++)
  {
    if(mmgsdi_file_info_table[table_index].file == file)
    {
      if (default_value_pptr != NULL)
      {
        *default_value_pptr = mmgsdi_file_info_table[table_index].default_value_ptr;
      }
      if (file_length_ptr != NULL)
      {
        *file_length_ptr = mmgsdi_file_info_table[table_index].file_length;
      }
      if (table_entry_categ_ptr != NULL)
      {
        *table_entry_categ_ptr = mmgsdi_file_info_table[table_index].table_entry_categ;
      }
      if (file_type != NULL)
      {
        *file_type = mmgsdi_file_info_table[table_index].file_type;
      }
      if(file_attr_length_ptr != NULL)
      {
        *file_attr_length_ptr = mmgsdi_file_info_table[table_index].file_attr_length;
      }
      if(file_attr_pptr != NULL)
      {
        MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(*file_attr_pptr, mmgsdi_file_info_table[table_index].file_attr_length);
        if(*file_attr_pptr == NULL)
        {
          return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
        }
        mmgsdi_memscpy(*file_attr_pptr, 
                       mmgsdi_file_info_table[table_index].file_attr_length,
                       mmgsdi_file_info_table[table_index].default_attr_value_ptr,
                       mmgsdi_file_info_table[table_index].file_attr_length);
      }
      return MMGSDI_SUCCESS;
    }
  }
  return MMGSDI_NOT_FOUND;
}/* mmgsdi_file_fallback_get_file_info */


/* ==========================================================================
FUNCTION:      MMGSDI_FILE_FALLBACK_CHECK_CACHE_DELETE_ALLOWED

DESCRIPTION:
  This function checks the fallback type and if it allowed to delete the cache 

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  boolean:

SIDE EFFECTS:

==========================================================================*/
boolean mmgsdi_file_fallback_check_cache_delete_allowed(
  mmgsdi_file_enum_type           file
)
{
  uint8                 table_index = 0;

  for(table_index = 0; table_index < MMGSDI_FILE_INFO_TABLE_SIZE; table_index++)
  {
    if(mmgsdi_file_info_table[table_index].file == file)
    {
      /* If fallback to cache is supported then do not allow delete the file from the cache */
      if(mmgsdi_file_info_table[table_index].table_entry_categ == MMGSDI_ENTRY_CATEG_CACHE_FALLBACK ||
        (mmgsdi_file_info_table[table_index].table_entry_categ ==  MMGSDI_ENTRY_CATEG_FS_OR_CACHE_FALLBACK && 
        (mmgsdi_nv_get_device_feature_status(MMGSDI_FEATURE_SIM_FILE_FALLBACK_TO_FS) == MMGSDI_FEATURE_DISABLED)))
     {
       return FALSE;
     }
    }
  }
  return TRUE;
}/* mmgsdi_file_fallback_check_cache_delete_allowed */
