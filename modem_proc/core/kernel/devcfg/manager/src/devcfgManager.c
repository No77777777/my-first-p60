
/*============================================================================

DALSYS initialization, common and any miscellaneous functions

        Copyright © 2013, 2015, 2018,2020 QUALCOMM Technologies Incorporated.
        All Rights Reserved.
        Qualcomm Confidential and Proprietary
==============================================================================*/




#include "DALSysInt.h"
#include "stdlib.h"
#include "assert.h"

#ifdef DEVCFG_BOOT
#include "XBLConfig.h"
#endif

#ifndef ASSERT
#define ASSERT assert
#endif

extern void devcfg_blob_append(DALProps * newPropInfo, int bid);
extern devcfgBlobTableType devcfgBlobTable[];
extern int devcfgBlobTableSize;
extern DEVCFG_PLATFORM_INFO gtcsrSOCHWVersion;

#ifndef DEVCFG_NO_DAL
extern void DAL_setModInfo(DALProps * mod_info_addr, DALREG_DriverInfoList * global_drivers);
extern DALProps DAL_Mod_Info;
extern DALREG_DriverInfoList gDALModDriverInfoList;
#endif

DALProps devcfgPropsInfo;

void devcfgManagerInit()
{
   int i = 0;
   DALProps * dalpropsptr = NULL;
   //iterate through blob table
   for (i = 0; i < devcfgBlobTableSize; i++ )
   {
      if (devcfgBlobTable[i].loadtype == DEVCFG_LOADTYPE_RAM)
      {
         if(gtcsrSOCHWVersion.soc_number == devcfgBlobTable[i].soc_id){
               dalpropsptr = devcfgBlobTable[i].name;
               devcfg_blob_append(dalpropsptr, 0);
         }
#ifndef DEVCFG_NO_DAL
   DALProps * mod_info_ptr = &DAL_Mod_Info;
   DALREG_DriverInfoList * global_drivers = &gDALModDriverInfoList;
   DAL_setModInfo(mod_info_ptr, global_drivers);
#endif
}
      else if (devcfgBlobTable[i].loadtype == DEVCFG_LOADTYPE_XBL)
      {
#ifdef DEVCFG_BOOT
         xblconfig_status_type status;
         unsigned int xblconfig_size;
#ifndef DEVCFG_STANDALONE
         unsigned int xblconfig_handle;
         status = xblconfig_open((char *)devcfgBlobTable[i].name, &xblconfig_handle, &xblconfig_size);
#else
         FILE * xblconfig_handle = malloc(sizeof(FILE));
         status = xblconfig_open((char *)devcfgBlobTable[i].name, xblconfig_handle, &xblconfig_size);
#endif
         ASSERT(status == XBLCONFIG_SUCCESS);

         int currentOffset = 0x0;
         devcfgMetaDataType blobMetaData;
         status = xblconfig_read(xblconfig_handle, currentOffset, sizeof(devcfgMetaDataType), &blobMetaData);
         ASSERT(status == XBLCONFIG_SUCCESS);

         currentOffset += sizeof(devcfgMetaDataType);
         DALSYSPropStructTblType * devcfgStructPtrs = malloc(blobMetaData.structPtrTableSize);
         status = xblconfig_read(xblconfig_handle, currentOffset, blobMetaData.structPtrTableSize, (void *)devcfgStructPtrs);
         ASSERT(status == XBLCONFIG_SUCCESS);

         currentOffset += blobMetaData.structPtrTableSize;
         byte * devcfgPropsBin = malloc(blobMetaData.propBinSize);
         status = xblconfig_read(xblconfig_handle, currentOffset, blobMetaData.propBinSize, (void *)devcfgPropsBin);
         ASSERT(status == XBLCONFIG_SUCCESS);

         currentOffset += blobMetaData.propBinSize;
         StringDevice * devcfgDriverlist = malloc(blobMetaData.deviceListSize);
         status = xblconfig_read(xblconfig_handle, currentOffset, blobMetaData.deviceListSize, (void *)devcfgDriverlist);
         ASSERT(status == XBLCONFIG_SUCCESS);

         int numDrivers = blobMetaData.deviceListSize/sizeof(StringDevice);

         //patch struct ptr table
         void * thisStruct;
         if(devcfgStructPtrs)
         {
            int j = 0;
            do
            {
               thisStruct = malloc(devcfgStructPtrs[j].dwSize);
               status = xblconfig_read(xblconfig_handle, (unsigned long)devcfgStructPtrs[j].pStruct, devcfgStructPtrs[j].dwSize, thisStruct);
               devcfgStructPtrs[j].pStruct = thisStruct;
               ASSERT(status == XBLCONFIG_SUCCESS);
               j++;
            }while((devcfgStructPtrs[j].dwSize != 0) && (devcfgStructPtrs[j].pStruct != 0));
         }
         
         devcfgPropsInfo.pDALPROP_PropBin = devcfgPropsBin;
         devcfgPropsInfo.pDALPROP_StructPtrs = devcfgStructPtrs;
         devcfgPropsInfo.dwDeviceSize = numDrivers;
         devcfgPropsInfo.pDevices = devcfgDriverlist;

         devcfg_blob_append(&devcfgPropsInfo, 0);
         status = xblconfig_close(xblconfig_handle);
#endif
      }
      else if (devcfgBlobTable[i].loadtype == DEVCFG_LOADTYPE_XBL_FIXED)
      {
#ifdef DEVCFG_BOOT
         xblconfig_status_type status;
         unsigned int xblconfig_size;
#ifndef DEVCFG_STANDALONE
         unsigned int xblconfig_handle;
         status = xblconfig_open((char *)devcfgBlobTable[i].name, &xblconfig_handle, &xblconfig_size);
#else
         FILE * xblconfig_handle = malloc(sizeof(FILE));
         status = xblconfig_open((char *)devcfgBlobTable[i].name, xblconfig_handle, &xblconfig_size);
#endif
         ASSERT(status == XBLCONFIG_SUCCESS);

         devcfgMetaDataType blobMetaData;
         status = xblconfig_read(xblconfig_handle, 0x0, sizeof(devcfgMetaDataType), &blobMetaData);
         ASSERT(status == XBLCONFIG_SUCCESS);

         DALSYSPropStructTblType * devcfgStructPtrs = (DALSYSPropStructTblType *)blobMetaData.fixedAddr;
         status = xblconfig_read(xblconfig_handle, sizeof(devcfgMetaDataType), xblconfig_size - sizeof(devcfgMetaDataType), (void *)devcfgStructPtrs);
         ASSERT(status == XBLCONFIG_SUCCESS);

         byte * devcfgPropsBin = (byte *)(blobMetaData.fixedAddr + blobMetaData.structPtrTableSize);
         StringDevice * devcfgDriverlist = (StringDevice *)(blobMetaData.fixedAddr + blobMetaData.structPtrTableSize + blobMetaData.propBinSize);

         int numDrivers = blobMetaData.deviceListSize/sizeof(StringDevice);
         
         devcfgPropsInfo.pDALPROP_PropBin = devcfgPropsBin;
         devcfgPropsInfo.pDALPROP_StructPtrs = devcfgStructPtrs;
         devcfgPropsInfo.dwDeviceSize = numDrivers;
         devcfgPropsInfo.pDevices = devcfgDriverlist;

         devcfg_blob_append(&devcfgPropsInfo, 0);
         status = xblconfig_close(xblconfig_handle);
#endif
      }
   }
   if(dalpropsptr == NULL){
	dalpropsptr = devcfgBlobTable[0].name;
       devcfg_blob_append(dalpropsptr, 0);
      //ERR_FATAL("Soc Id check fail", 0, 0, 0);
   }
}
