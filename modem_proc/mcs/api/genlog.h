/*!
  @file
  genlog.h

  @brief
  This module contains the common helper functions needed for logging log
  packets containing multiple sub packets.

*/

/*===========================================================================

  Copyright (c) 2017 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================
$Header: //components/rel/mcs.mpss/5.2/api/genlog.h#1 $
===========================================================================*/

#ifndef GENLOG_H
#define GENLOG_H

/*===========================================================================

                             INCLUDE FILES

===========================================================================*/

#include <comdef.h>
#include <log.h>
#include <IxErrno.h>
#include <stddef.h>
#include <stringl/stringl.h>
#include <diag.h>

/*===========================================================================

                      EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*----------------------------------------------------------------------------
  Subpacket Defintion
----------------------------------------------------------------------------*/

/*! External subpacket build function type */
typedef uint16 ( *genlog_subpkt_build_f )( void  *data, void  *payload );


/*! Extern subpacket memcpy function type */
typedef void * ( *genlog_subpkt_memcpy_f )( void *d1, size_t m, const void *d2, size_t n );


/*! External subpacket constant data type */
typedef struct genlog_subpkt_const_s
{
  /*! Subpacket id */
  uint8  id;
  /*! Subpacket version */
  uint8  ver;
  /*! Subpacket size */
  uint16  size;
  /*! Subpacket build callback */
  genlog_subpkt_build_f  build_f;
  /*! Memcpy function for copying the data */
  genlog_subpkt_memcpy_f  memcpy_f;

} genlog_subpkt_const_s;


/*! External subpacket definition type */
typedef struct genlog_subpkt_def_s
{
  /*! Subpacket's constant data */
  genlog_subpkt_const_s  const_data;
  /*! Subpacket's scratchpad area */
  void  *data;
  /*! Subpacket's payload */
  void  *payload;

} genlog_subpkt_def_s;


/*----------------------------------------------------------------------------
  Packet Defintion
----------------------------------------------------------------------------*/

/*! Maximum number of subpackets in a log packet */
#define GENLOG_MAX_NUM_SUBPKTS  10


/*! Version tag for the log packets.
    Increase from 7 to 50 to help LTE techs which need to implement their
    own major/minor versioning as they set the packet version.  This actually
    allows them to sub-divide versions per PL, as packets may be different for
    multiple PLs simultaneously.  Extending the versioning here helps enable
    them, without redesigning genlog APIs to support this unique requirement. */
typedef enum
{
  GENLOG_VER_1 = 1,  /*!< Log version 1 */
  GENLOG_VER_2,      /*!< Log version 2 */
  GENLOG_VER_3,      /*!< Log version 3 */
  GENLOG_VER_4,      /*!< Log version 4 */
  GENLOG_VER_5,      /*!< Log version 5 */
  GENLOG_VER_6,      /*!< Log version 6 */
  GENLOG_VER_7,      /*!< Log version 7 */
  GENLOG_VER_8,
  GENLOG_VER_9,
  GENLOG_VER_10,  GENLOG_VER_11,  GENLOG_VER_12,  GENLOG_VER_13,  GENLOG_VER_14,
  GENLOG_VER_15,  GENLOG_VER_16,  GENLOG_VER_17,  GENLOG_VER_18,  GENLOG_VER_19,
  GENLOG_VER_20,  GENLOG_VER_21,  GENLOG_VER_22,  GENLOG_VER_23,  GENLOG_VER_24,
  GENLOG_VER_25,  GENLOG_VER_26,  GENLOG_VER_27,  GENLOG_VER_28,  GENLOG_VER_29,
  GENLOG_VER_30,  GENLOG_VER_31,  GENLOG_VER_32,  GENLOG_VER_33,  GENLOG_VER_34,
  GENLOG_VER_35,  GENLOG_VER_36,  GENLOG_VER_37,  GENLOG_VER_38,  GENLOG_VER_39,
  GENLOG_VER_40,  GENLOG_VER_41,  GENLOG_VER_42,  GENLOG_VER_43,  GENLOG_VER_44,
  GENLOG_VER_45,  GENLOG_VER_46,  GENLOG_VER_47,  GENLOG_VER_48,  GENLOG_VER_49,
  GENLOG_VER_50,
  GENLOG_VER_MAX,    /*!< Maximum log version */

} genlog_ver_t;


/*! External packet definition type */
typedef struct genlog_pkt_def_s
{
  /*! Log code */
  log_code_type  log_code;
  /*! Packet version */
  genlog_ver_t  ver;
  /*! Number of subpackets */
  uint8  num_subpkts;
  /*! Subpacket array */
  uint8  subpkt_id[ GENLOG_MAX_NUM_SUBPKTS ];

} genlog_pkt_def_s;


/*----------------------------------------------------------------------------
  Genlog Defintion
----------------------------------------------------------------------------*/

/*! External Genlog structure */
typedef struct genlog_s
{
  /*! Log packet defintions */
  const genlog_pkt_def_s  *pkt_def;
  /*! Number of log packets */
  uint8  num_pkts;
  /*! Subpacket definitions */
  genlog_subpkt_def_s **subpkt_def;
  /*! Number of subpackets */
  uint8  num_subpkts;

} genlog_s;


/*----------------------------------------------------------------------------
  Defintion Macros
----------------------------------------------------------------------------*/

/*! External macro to help with defining a subpacket that has a fixed size
    and does not require a custom build function */
#define GENLOG_SUBPKT_DEF_FIXED( id, ver, data )    \
  {                                                 \
    {                                               \
      (uint8)id,                                    \
      (uint8)ver,                                   \
      (uint16)sizeof(*(data)),                      \
      (genlog_subpkt_build_f)NULL,                  \
      (genlog_subpkt_memcpy_f)memscpy                                        \
    },                                              \
    (void *)data,                                   \
    NULL,                                           \
  }


/*! External macro to help with defining a subpacket that uses a build function 
    to populate the subpacket data and/or handles variable size subpackets */
#define GENLOG_SUBPKT_DEF( id, ver, data, build_f )     \
  {                                                     \
    {                                                   \
      (uint8)id,                                        \
      (uint8)ver,                                       \
      (uint16)0,                                        \
      (genlog_subpkt_build_f)build_f,                   \
      (genlog_subpkt_memcpy_f)memscpy                   \
    },                                                  \
    (void *)data,                                       \
    NULL,                                               \
  }


/*! External macro to help with defining a subpacket that uses a build function 
    to populate the subpacket data and/or handles variable size subpackets and
    a specialized memcpy function to handle large, variable length data (eg.
    DSM items) in order to copy them to the log packet at log allocation time
    instead of at build time. */
#define GENLOG_SUBPKT_DEF_EXT( id, ver, data, build_f, memcpy_f ) \
  {                                                     \
    {                                                   \
      (uint8)id,                                        \
      (uint8)ver,                                       \
      (uint16)0,                                        \
      (genlog_subpkt_build_f)build_f,                   \
      memcpy_f,                                         \
    },                                                  \
    (void *)data,                                       \
    NULL,                                               \
  }


/*! External macro to help with defining a log packet */
#define GENLOG_PKT_DEF( id, ver, num_subpkts, ... )     \
  {                                                     \
    (log_code_type)id,                                  \
    ver,                                                \
    num_subpkts,                                        \
    { __VA_ARGS__ }                                     \
  }


/*===========================================================================

                       EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

extern errno_enum_type genlog_init
(
  genlog_s  *genlog
);

extern errno_enum_type genlog_subpkt_register
(
  genlog_s  *genlog,
  uint8  num_subpkts,
  genlog_subpkt_def_s  *subpkt_def
);

extern errno_enum_type genlog_subpkt_deregister
(
  genlog_s  *genlog,
  uint8  num_subpkts,
  genlog_subpkt_def_s  *subpkt_def

);

extern errno_enum_type genlog_subpkt_set_data
(
  genlog_s  *genlog,
  uint8  subpkt_id,
  void  *data
);

errno_enum_type genlog_subpkt_set_payload
(
  genlog_s  *genlog,
  uint8  subpkt_id,
  void  *payload
);

extern errno_enum_type genlog_pkt_commit_ext
(
  genlog_s      *genlog,
  log_code_type  log_code,
  uint32          as_id
);

extern errno_enum_type genlog_pkt_commit
(
  genlog_s      *genlog,
  log_code_type  log_code
);


/*===========================================================================

                             INLINE FUNCTIONS

===========================================================================*/

#endif /* GENLOG_H */
