#ifndef __Q6ZIP_UNCOMPRESS_H__
#define __Q6ZIP_UNCOMPRESS_H__


/*===========================================================================
 * FILE:         q6zip_uncompress.h
 *
 * SERVICES:     DL PAGER
 *
 * DESCRIPTION:  q6zip uncompressor header
 *
 * Copyright (c) 2014 Qualcomm Technologies Incorporated.
 * All Rights Reserved. QUALCOMM Proprietary and Confidential.
===========================================================================*/

/*===========================================================================

  EDIT HISTORY FOR MODULE
  
  
when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/17/14   ao      Initial revision (collaboration with Ric Senior)
03/18/14   rs      Updated to version q6zip3v3
04/25/14   rs      Updated to version q6zip3v4
===========================================================================*/


#define DICT1_BITS 10
#define DICT2_BITS 12
#define Q6ZIP_DICT_SIZE ((1<<DICT1_BITS)+(1<<DICT2_BITS))

int q6zip_uncompress(char* out_buf, int* out_buf_size,
		     char* in_buf,  int in_buf_size,
		     char* dict);

#endif
