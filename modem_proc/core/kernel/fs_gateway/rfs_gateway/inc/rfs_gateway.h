/*=============================================================================
  Copyright (c) 2008 Qualcomm Technologies Incorporated.
           All Rights Reserved.
 * rfs_gateway.h
=============================================================================*/

#ifndef __RFS_GATEWAY_H__
#define __RFS_GATEWAY_H__

/**
 QURT-FS should call this function to get a file from RFS.
 This function will allocate a buffer, fetch file from Remote File System 
 and run authentication routines on the read file. 
 Finally it will install this file onto Qurt-FS.

 @param path
    Path of the file to read.
*/
void rfs_gateway(void *arg, int handle, const char *path);

/*
* Call in order to register rfs_gateway with QuRT RAMFS, ideally in RCINIT
*/
void rfs_gateway_init(void);
int rfs_gateway_error(void);


#endif