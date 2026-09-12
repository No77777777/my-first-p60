#ifndef PBM_TUNING_H
#define PBM_TUNING_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                          PHONEBOOK MANAGER FUNCTIONS

GENERAL DESCRIPTION
  This file contains the constants that may be useful to customize
  for a particular phonebook implementation.

  Copyright (c) 2005-2009, 2015 byQUALCOMM Technologies, Inc(QTI).
  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/api/pbm_tuning.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/29/15   stv     Reduce EFS storage
10/14/09  krishnac Added support for reduced RAM usage
07/10/09  ashwanik Support for CSIM and Dual Sim Features
03/04/08   cvs     Increase record size for non-EFS enabled PB's
01/17/08   clm     Add FEATURE_PBM_USIM_SUPPORT to featurize USIM features.
03/12/07   cvs     Featurize 4 PB set support
02/09/07   cvs     Support for 4 phonebook sets
11/01/06   cvs     multi pb set fixes
06/12/06   cvs     add feature for 200 entry PB
03/27/06   cvs     Direct featurization to create EFS phonebook
09/22/05   cvs     Initial Revision
===========================================================================*/

#define  PBM_MAX_EFS_REC_LEN     1500
#define  PBM_MAX_RECORD_SIZE     PBM_MAX_EFS_REC_LEN

#define  PBM_MAX_FIELD_SIZE      256

/* Size of EFS based phonebooks */
#define  PBM_MAX_EFS_RECS        25
#define  PBM_MAX_CALL_HISTORY    20
#define  PBM_MAX_SPEED_DIALS     20

/* Number of deletes before doing garbage recollection */
#define PBM_MAX_EFS_DELETES      10

/* When converting old files, if they are too big, reading the entire file would take too long. 
   If we try to do garbage collection with this many deletes, we return PBM_ERROR*/
#define PBM_MAX_DELETE_SIZE      1000

/* max number of clients that can call new PBM session enum rec init simultaneoulsy */
#define PBM_MAX_NUM_CLIENTS_FOR_ENUM_REC_INIT 10 

#endif /* PBM_TUNING_H */
