#ifndef TDSRRCLOG_H
#define TDSRRCLOG_H
/*===========================================================================
              R R C L O G  H E A D E R  F I L E

DESCRIPTION
  This file contains data types and declarations associated with the
  functions for RRC Logging.
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrclog.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $


when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/07/11   mkh     Renamed log pkts to TDSxxx, Moved log pkt to file where 
                   other RRC log pks are.
03/10/11   ttl     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/

/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/
#include "comdef.h"
#include "log.h"
#include "tdsuecomdef.h"

/*===========================================================================
**                  CONSTANT/DEFINE DECLARATIONS
**=========================================================================*/

#define TDSSDU_HDR_RES_LENGTH   8             /* SDU Header length in bytes */
#define TDSTIME_STAMP_LENGTH    8             /* Time Stamp length in bytes */
#define TDSTAG_LENGTH           4             /* File Header Tag length in bytes*/
#define TDSFILE_HDR_RES_LENGTH  6             /* File Header length in bytes */

#define TDSFILE_FORMAT_VER      1             /* File Format Version */

#define TDSMAJOR_SPEC_RELEASE   6             /* Major Specification release */
#define TDSMINOR_SPEC_RELEASE   8             /* Minor Specification release */
#define TDSMINOR_MINOR_SPEC_RELEASE 0         /* Minor minor Spec release */


/*===========================================================================
**                  ENUMERATED TYPES
**=========================================================================*/

/*--------------------------------------------------------------------------- 
  Type for defining the status of a logical channel entity.  
---------------------------------------------------------------------------*/

 
/*===========================================================================
**                     TYPE DECLARATIONS
**=========================================================================*/
typedef enum
{
  TDSRRCLOG_FILE_CLOSED,                       /* The Log file is closed */
  TDSRRCLOG_FILE_OPENED                        /* The Log File is opened */
}tdsrrclog_file_status_e_type;

typedef enum
{
  TDSRRCLOG_FAILURE,                           /* Failure Status */
  TDSRRCLOG_SUCCESS                            /* Success Status */
} tdsrrclog_status_e_type;

typedef enum
{
  TDSRRCLOG_BCCH_BCH = 0,                      /* BCCH Logical Channel mapped
                                               to BCH transport Channel*/
  TDSRRCLOG_BCCH_FACH,                         /* BCCH Logical Channel mapped 
                                               to FACH transport Channel*/
  TDSRRCLOG_PCCH,                              /* PCCH Logical Channel */
  TDSRRCLOG_CCCH,                              /* CCCH Logical Channel */
  TDSRRCLOG_DCCH                               /* DCCH logical Channel */
}tdsrrclog_log_ch_e_type;

typedef struct
{
  uint16 log_ch;                            /* Logical Channel type */
  uint16 rb_id;                             /* Radio Bearer Id */
  uint32 num_bytes;                         /* SDU length */
  uint8 time_stamp[TDSTIME_STAMP_LENGTH];      /* 8 bytes are reserved for
                                               time stamp.*/

  uint8 reserved_bytes[TDSSDU_HDR_RES_LENGTH]; /* 8 bytes are reserved for 
                                               future expansion */
} tdsrrclog_sdu_hdr_type;

typedef struct
{
  uint8 file_hdr_tag[TDSTAG_LENGTH];            /* This is file header tag
                                                and set to "RRC ". */
  uint16 file_format_ver;                    /* File Format Version */
  uint8 major_spec;                          /* Major Specification release.*/
  uint8 minor_spec;                          /* Minor Specification release.*/
  uint8 minor_minor_spec;                    /* Minor minor Specification
                                                release .*/
  uint8 reserved_1;                          /* Reserved for funture 
                                                expansion */
  uint8 reserved_2[TDSFILE_HDR_RES_LENGTH];     /* Resereved for future 
                                                expansion */
} tdsrrclog_file_hdr_type;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================

FUNCTION RRC_SEND_RB_RATE_INFO_PACKET

DESCRIPTION

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None. 

===========================================================================*/
void tdsrrclog_send_rb_rate_info_packet
(
void
);

/*===========================================================================

FUNCTION tdsrrclog_create_log_file

DESCRIPTION
  This function creates and opens a new data file for write only.

DEPENDENCIES
  None

RETURN VALUE
  tdsrrclog_status_e_type

SIDE EFFECTS
  None
===========================================================================*/

tdsrrclog_status_e_type tdsrrclog_create_log_file
(    
  const char *filename  
);

/*===========================================================================
FUNCTION tdsrrclog_close_log_file

DESCRIPTION
  This function closes the file.

DEPENDENCIES
  None

RETURN VALUE
  tdsrrclog_status_e_type

SIDE EFFECTS
  None
===========================================================================*/

tdsrrclog_status_e_type tdsrrclog_close_log_file
(    
  void
);

/*===========================================================================
FUNCTION tdsrrclog_log_sig_msg

DESCRIPTION
  This function writes the SDU header first into binary file and then writes
  the SDU data into binary file.

DEPENDENCIES
  None

RETURN VALUE
  tdsrrclog_status_e_type

SIDE EFFECTS
  None
===========================================================================*/

tdsrrclog_status_e_type tdsrrclog_log_sig_msg
(    
  void  *sdu,                                /* SDU data */
  uint32 sdu_size,                           /* SDU size in bytes */
  uint16 rb_id,                              /* Radio Bearer Id to which SDU
                                                belongs */
  tdsrrclog_log_ch_e_type log_ch                /* Logical Channel type to which
                                                SDU belongs */           
);

#endif /* TDSRRCLOG_H */

/*=========================================================================*/
