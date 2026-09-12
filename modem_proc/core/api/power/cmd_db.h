#ifndef CMD_DB_H
#define CMD_DB_H
/*===========================================================================
                             cmd_db.h

DESCRIPTION:
  Public API to command DB.  Provides query functionality into command DB.

Copyright (c) 2016-2018 QUALCOMM Technologies, Inc. (QTI). All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/

//===========================================================================
//                   Includes and Public Data Declarations
//===========================================================================

//---------------------------------------------------------------------------
// Include Files
//------------------- --------------------------------------------------------
#include <stdint.h>


//===========================================================================
//                             Function Definitions
//===========================================================================

/**
 * <!-- cmd_db_query_addr -->
 *
 * @brief Query command db for resource id address.
 *
 *  This is used to retrieve resource address based on resource
 *  id.
 *
 *  @param res_id : resource id to query for address
 *
 *  @return 0 for failure, non-zero for offset address
 */
uint32_t cmd_db_query_addr(const char *res_id);

#endif // CMD_DB_H
