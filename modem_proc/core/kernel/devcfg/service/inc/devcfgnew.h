#ifndef DEVCFGNEW_H
#define DEVCFGNEW_H
/*==============================================================================
@file devcfgnew.h


Device Config Internal Types


        Copyright © 2007, 2018 Qualcomm Technologies Inc.
        All Rights Reserved.
        Qualcomm Confidential and Proprietary
==============================================================================*/

/*------------------------------------------------------------------------------
* Include Files
*-----------------------------------------------------------------------------*/
#include "DALSysInt.h"
//#include "DevCfg_ImgParams.h"
//#include "stdlib.h"
//#ifndef DEVCFG_STANDALONE
//    #include "stringl/stringl.h"
//#endif

void devcfg_init_new(void);
void devcfg_blob_append(DALProps * newPropInfo, int bid);
void devcfg_blob_prepend(DALProps * newPropInfo, int bid);
DALProps * devcfgGetBlobByID(int bid);
#endif
