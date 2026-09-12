/*======================================================
FILE:  tm_buffer.h

SERVICES: Growable memory tm_buffer.

DESCRIPTION: 

Growable memory tm_buffer declaration.

Copyright © 2009 Qualcomm Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR
=====================================================*/
#ifndef __TM_BUFFER_H__
#define __TM_BUFFER_H__

#include "comdef.h"
#include "customer.h"
#include "aries_os_api.h"

/*------------------------------------------------------------------------------
structure that incapsulates a growable tm_buffer.  
------------------------------------------------------------------------------*/
//struct _buffer;
struct _buffer {
   int8 * pBuffer; /* underline byte buffer */
   int32 nCapacity; /* buffer capacity */
   int32 nSize; /* buffer size */
   int32 nGrowFactor;
   int32 nRealSize;
};

typedef struct _buffer tm_buffer;


/*-----------------------------------------------------------------------------
Function Prototypes
-----------------------------------------------------------------------------*/

/*=============================================================================
FUNCTION: tm_buffer_create
DESCRIPTION: Creates and initializes tm_buffer object. 

PROTOTYPE:
tm_buffer * tm_buffer_Create(int32 nsize);
PARAMETERS:
piEnv: [in]: pointer to IEnv interface
nsize: [in]: initial tm_buffer size
DEPENDENCIES:  
None

RETURN VALUE
tm_buffer*: pointer to the newly created object
SIDE EFFECTS:  
None
=============================================================================*/
tm_buffer * tm_buffer_create(int32 n_size);

/*=============================================================================
FUNCTION: tm_buffer_free
DESCRIPTION: Frees all resources from tm_buffer object. 

PROTOTYPE:
void tm_buffer_Free(tm_buffer * p_buffer);
PARAMETERS:
p_buffer: [in]: pointer to tm_buffer object to be freed
DEPENDENCIES:  
None

RETURN VALUE
None
SIDE EFFECTS:  
None
=============================================================================*/
void tm_buffer_free(tm_buffer * p_buffer);

/*=============================================================================
FUNCTION: tm_buffer_append_bytes
DESCRIPTION: appends bytes to a tm_buffer object. Underline tm_buffer will grow to 
accommodate required dimension.

PROTOTYPE:
void tm_buffer_appendBytes(tm_buffer * p_buffer, const int8 * psBytes, int32 nLen);
PARAMETERS:
p_buffer: [in]: pointer to destination tm_buffer
p_bytes: [in]: pointer to string of bytes to be appended
n_len: [in]: number of bytes in psBytes
DEPENDENCIES:  
None

RETURN VALUE
TRUE if successful
SIDE EFFECTS:  
None
=============================================================================*/
boolean tm_buffer_append_bytes(tm_buffer * p_buffer, const int8 * p_bytes, int32 n_len);

/*=============================================================================
FUNCTION: tm_buffer_append_buffer
DESCRIPTION: appends a tm_buffer object to another tm_buffer object. Underline 
tm_buffer will grow to accommodate required dimension.

PROTOTYPE:
void tm_buffer_appendbuffer(tm_buffer * p_buffer, const tm_buffer * pSrcbuffer);
PARAMETERS:
p_buffer: [in]: pointer to destination tm_buffer.
p_src_buffer: [in]: pointer to source tm_buffer.
DEPENDENCIES:  
None

RETURN VALUE
TRUE if successful
SIDE EFFECTS:  
None
=============================================================================*/
boolean tm_buffer_append_buffer(tm_buffer * p_buffer, const tm_buffer * p_src_buffer);

/*=============================================================================
FUNCTION: tm_buffer_get_bytes
DESCRIPTION: Returns underline tm_buffer.

PROTOTYPE:
const int8 * tm_buffer_getBytes(tm_buffer * p_buffer);
PARAMETERS:
p_buffer: [in]: pointer to tm_buffer.
DEPENDENCIES:  
None

RETURN VALUE
const int8*: Pointer to underline tm_buffer.
SIDE EFFECTS:  
None
=============================================================================*/
const int8 * tm_buffer_get_bytes(const tm_buffer * p_buffer);

/*=============================================================================
FUNCTION: tm_buffer_getsize
DESCRIPTION: Returns current tm_buffer size.

PROTOTYPE:
int32 tm_buffer_getsize(tm_buffer * p_buffer);
PARAMETERS:
p_buffer: [in]: pointer to tm_buffer
DEPENDENCIES:  
None

RETURN VALUE
int32: Current tm_buffer size.
SIDE EFFECTS:  
None
=============================================================================*/
int32 tm_buffer_get_size(const tm_buffer * p_buffer);

/*=============================================================================
FUNCTION: tm_buffer_get_capacity
DESCRIPTION: Returns current tm_buffer capacity.

PROTOTYPE:
int32 tm_buffer_getCapacity(tm_buffer * p_buffer);
PARAMETERS:
p_buffer: [in]: pointer to tm_buffer
DEPENDENCIES:  
None

RETURN VALUE
int32: Current tm_buffer capacity.
SIDE EFFECTS:  
None
=============================================================================*/
int32 tm_buffer_get_capacity(const tm_buffer * p_buffer);

/*=============================================================================
FUNCTION: tm_buffer_release
DESCRIPTION: Releases responsibility of tm_buffer object to free underline 
          tm_buffer.

PROTOTYPE:
int8 * tm_buffer_release(tm_buffer * p_buffer, int32 * nsize);
PARAMETERS:
p_buffer: [in]: pointer to tm_buffer
n_size: [out]: tm_buffer size
DEPENDENCIES:  
None

RETURN VALUE
int8*: Pointer to underline tm_buffer.
SIDE EFFECTS:  
None
=============================================================================*/
int8 * tm_buffer_release(tm_buffer * p_buffer, int32 * n_size);

/*=============================================================================
FUNCTION: tm_buffer_set_grow_factor
DESCRIPTION: sets the grow factor of the internal tm_buffer of the tm_buffer 
          object. By default it is 2

PROTOTYPE:
void tm_buffer_set_grow_factor(tm_buffer * p_buffer,int32 nGrowFactor);
PARAMETERS:
p_buffer: [in]: pointer to tm_buffer
n_grow_factor: [out]: grow factor
DEPENDENCIES:  
None

RETURN VALUE
None
SIDE EFFECTS:  
None
=============================================================================*/
void tm_buffer_set_grow_factor(tm_buffer * p_buffer, int32 n_grow_factor);
/*=============================================================================
FUNCTION: tm_buffer_roll_back
DESCRIPTION: move in the internal tm_buffer nBytes behind

PROTOTYPE:
void tm_buffer_roll_back(tm_buffer * p_buffer, int32 nBytes)
PARAMETERS:
p_buffer: [in]: pointer to destination tm_buffer
nBytes: [in]: how many bytes to go back 
NOTE: the tm_buffer will not be reallocated to a smaller tm_buffer.

DEPENDENCIES:  
None

RETURN VALUE
TRUE if successful
SIDE EFFECTS:  
None
=============================================================================*/
boolean tm_buffer_roll_back(tm_buffer * p_buffer, int32 n_bytes);

#endif /* __TM_BUFFER_H__ */
