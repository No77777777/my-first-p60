/**---------------------------------------------------------------------------
 * @file  VVDRV_deo_mutex_os.c
 *
 * @brief declares the DEO mutex defines for VI_BUILD operating system
 *
 * Copyright (c) 2017 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Include Files
 *---------------------------------------------------------------------------*/
#include "VVDRV_deo_mutex.h"           /* apis implemented in this c file */
#include "VVDRV_deo_common.h"          /* deo results */

#if defined(DEO_MUTEX_SUPPORTED)
#include DEO_MUTEX_H_INCLUDE_FILE      /* apss lock apis */
#endif /* DEO_MUTEX_SUPPORTED */


/*------------------------------------------------------------------------------
 * Static Variable Definitions
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 * Global Function Definitions
 *----------------------------------------------------------------------------*/
#if defined(DEO_MUTEX_SUPPORTED)
int32 VVDRV_deo_mutex_init(void) /* DEO_MUTEX_SUPPORTED */
{
   return DEO_SUCCESS;
}

int32 VVDRV_deo_mutex_create /* DEO_MUTEX_SUPPORTED */
(
   deo_mutex_t* mutex,
   deo_mutex_attr_t attr
)
{
   if (NULL == mutex)
   {
      return DEO_ERROR;
   }

   mutex->attr = attr;
   return DEO_SUCCESS;
}

int32 VVDRV_deo_mutex_lock(deo_mutex_t *mutex) /* DEO_MUTEX_SUPPORTED */
{
   if (NULL == mutex)
   {
      return DEO_ERROR;
   }

   mutex_lock(&mutex->lock);
   return DEO_SUCCESS;
}

int32 VVDRV_deo_mutex_unlock(deo_mutex_t *mutex) /* DEO_MUTEX_SUPPORTED */
{
   if (NULL == mutex)
   {
      return DEO_ERROR;
   }

   mutex_unlock(&mutex->lock);
   return DEO_SUCCESS;
}

int32 VVDRV_deo_mutex_destroy(deo_mutex_t *mutex) /* DEO_MUTEX_SUPPORTED */
{
   (void)mutex;
   return DEO_SUCCESS;
}

void VVDRV_deo_mutex_uninit(void) /* DEO_MUTEX_SUPPORTED */
{
   return;
}

#else /* !DEO_MUTEX_SUPPORTED */

int32 VVDRV_deo_mutex_init(void) /* !DEO_MUTEX_SUPPORTED */
{
   return DEO_SUCCESS;
}

int32 VVDRV_deo_mutex_create /* !DEO_MUTEX_SUPPORTED */
(
   deo_mutex_t *mutex,
   deo_mutex_attr_t attr
)
{
   (void)mutex;
   (void)attr;
   return DEO_SUCCESS;
}

int32 VVDRV_deo_mutex_lock(deo_mutex_t *mutex) /* !DEO_MUTEX_SUPPORTED */
{
   (void)mutex;
   return DEO_SUCCESS;
}

int32 VVDRV_deo_mutex_unlock(deo_mutex_t *mutex) /* !DEO_MUTEX_SUPPORTED */
{
   (void)mutex;
   return DEO_SUCCESS;
}

int32 VVDRV_deo_mutex_destroy(deo_mutex_t* mutex) /* !DEO_MUTEX_SUPPORTED */
{
   (void)mutex;
   return DEO_SUCCESS;
}

void VVDRV_deo_mutex_uninit(void) /* !DEO_MUTEX_SUPPORTED */
{
   return;
}

#endif /* DEO_MUTEX_SUPPORTED */
