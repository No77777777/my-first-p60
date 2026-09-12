/*#============================================================================
#  Name:                                                                     
#    root_init.c 
#
#  Description:                                                              
#    elf_loader startup code before User PD is spwan.
#                                                                            
# Copyright (c) 2016, 2020 by Qualcomm Technologies, Incorporated.  All Rights Reserved.        
#============================================================================
#-------------------------------------------------------------------------------
#
#  $Header: //components/rel/core.mpss/10.0/kernel/elf_loader/root_init/src/root_init.c#2 $
#  $DateTime: 2020/06/16 09:59:17 $
#  $Change: 24386207 $
#                      EDIT HISTORY FOR FILE
#
#  This section contains comments describing changes made to the module.
#  Notice that changes are listed in reverse chronological order.
#
# when       who     what, where, why
# --------   ---     ---------------------------------------------------------
# 11/09/16    tkuo    add RFS query support before spawning User PD.
# 09/21/16    tkuo    add service regitery check before spawning User PD.
#-------------------------------------------------------------------------------
*/
#include <stdio.h>
#include "dlfcn.h"
#include "pd_mon_qurt.h"
#include "qurt.h"
#include "pthread.h"
#include "servreg_common.h"
#include "servreg_wrapper_domain_service.h"
#include "err.h"
#include "msg.h"
#include "rcinit.h"
#include "rfs_api.h"
#include "rfs_errno.h"
#include "rfs_gateway.h"
#include "root_init.h"
#include "DDITimetick.h"
#include "elf_loader_logevent.h"
/* Defines service registry related signal and a possible thread */

#define SERVREG_TEST_SIG_LOC_AVAILABLE 0x1 //Signal mask used for service locatory availability
#define ELOADER_THREAD_COMPLETED_ACK 0x4   //signal mask used for sucessfully terminate a thread
#define ELOADER_LOW_TASK_PRIORITY 0x1  //notes: we use pthread priority here. 
#define RETRY_IN_MINS 1 /* Retry in 1 mins */
#define SMALL_STACK_SIZE 0x600

char eloader_svc_name[]= "kernel/elf_loader";  //service registry idex name to search for domain by "service/provider"

elf_loader_debug_info_t ELF_LOADER_DEBUG_TABLE[ELF_LOADER_TABLE_MAX_INDEX+1];
unsigned volatile elf_loader_debug_index;

/* Defines PD information for service registry, RFS file link, and use different qurt signal for each spawning thread */
eloader_pd_info_t pd_info_carry[N_MAX_NUM_USERPD]=
{{"msm/modem/wlan_pd","wlan_process","wlanmdsp.mbn","/readonly/firmware/image/wlanmdsp.mbn","/readonly/vendor/firmware_mnt/image/wlanmdsp.mbn","/readonly/vendor/firmware/wlanmdsp.mbn", {NULL,NULL,0}},
 {"msm/modem/test_pd","test_process","testpd.mbn","/readonly/firmware/image/testpd.mbn","/readonly/vendor/firmware_mnt/image/testpd.mbn","/readonly/vendor/firmware/testpd.mbn", {NULL,NULL,0}}
};
/* Use different priority if we have to spawn 2 more User PD */
uint32_t eloader_thread_priority = ELOADER_LOW_TASK_PRIORITY;
  /* =====================================================================
 * Function:
 *    Eloader_servreg_notify
 *
 * Description:
 *    Notify client that the service registry is ready.
 * Parameters:
 *     eloader_pd_info_t *pd_info_carry; contain both domain name and process name.
 * Returns:
 *     None
 * =====================================================================  */
static void eloader_servreg_notify(boolean result, sr_wrapper_cb_data_t *wrapper_data)
{   
  qurt_anysignal_t * sig = (wrapper_data)->data;

  if ( result == SERVREG_TRUE )
  {
     qurt_signal_set(sig, SERVREG_TEST_SIG_LOC_AVAILABLE);// This will ensure that thread had spawned worker thread sucessfully...
  }
  else
  {
     qurt_signal_set(sig, ELOADER_THREAD_COMPLETED_ACK);// This will ensure that thread had spawned worker thread sucessfully...
  }
  free(wrapper_data);
  return;
}/* eloader_sesrvreg_notify */


/* =====================================================================
 * Function:
 *    Eloader_servreg_query
 *
 * Description:
 *      This is called from the thread created as a part of Elf Loader bootup to check service registry availablity.
 *      This thread will talk to service registry, When service locatory is available.
 *      This thread will spawn a PD if it's required.
 * Parameters:
 *     eloader_pd_info_t *pd_info_carry; contain both domain name and process name.
 * Returns:
 *     None
 * =====================================================================  */
static void* eloader_servreg_query(void *param)
{   
   uint32_t signal = 0x0;
   PD_MON_HANDLE hPDElfLoader;
   eloader_pd_info_t *pd_info_carry = (eloader_pd_info_t *) param;
   struct rfs_stat_buf *fstat_buf = NULL;
   int rfs_err = RFS_ENOERROR;
   //Use Dal Timer to wait for 1 min
   DalDeviceHandle *hTimetick = NULL;
   DalTimetickTime64Type tstart, t1, tmin = 0;
   DALResult result;

   //Use service registry callback to query service domain availability
   sr_wrapper_cb_data_t * wrapper_data;
   wrapper_data = calloc(1, sizeof(sr_wrapper_cb_data_t));
   if ( wrapper_data == NULL )
   {
     ELF_LOADER_DEBUG(ERR_MODE_ELF_LOADER , ERR_ELF_LOADER_MEM_ALLOC_SERVREG_CB , "Eloader Failed to get memory- NO PD is spwan");  
     free(pd_info_carry->eloader_thread.sig);
     pthread_exit(0); 
   }

   //Setup wrapper data for service registry to call the new thread 
   wrapper_data->domain  = (char *) pd_info_carry->domain_name;
   wrapper_data->service =  eloader_svc_name;
   //register with service registry. child thread will get notified when service registry locator is UP
   wrapper_data->cb      = eloader_servreg_notify;
   wrapper_data->data    = pd_info_carry->eloader_thread.sig;
   servreg_wrapper_domain_service(wrapper_data);

   fstat_buf = (struct rfs_stat_buf*)malloc(sizeof(struct rfs_stat_buf));  
   if ( fstat_buf == NULL )
   {
     ELF_LOADER_DEBUG(ERR_MODE_ELF_LOADER , ERR_ELF_LOADER_MEM_ALLOC_FSTAT_BUF , "Eloader Failed to get memory- NO PD is spwan");   
     free(wrapper_data);
     free(pd_info_carry->eloader_thread.sig);
     pthread_exit(0);;
   }
   //Wait on a signal for Service notifications  
   signal = ( uint32_t )qurt_anysignal_wait(pd_info_carry->eloader_thread.sig, SERVREG_TEST_SIG_LOC_AVAILABLE | ELOADER_THREAD_COMPLETED_ACK);

   if(SERVREG_TEST_SIG_LOC_AVAILABLE & signal) 
   {
     qurt_anysignal_clear(pd_info_carry->eloader_thread.sig, SERVREG_TEST_SIG_LOC_AVAILABLE);
     qurt_printf ("ELF_LOADER CAN SPWAN User PD:%s\n",pd_info_carry->domain_name);
     // Make sure RFS server is ready before we spawn a PD.
     // Wait for 1 min max 
     result = DalTimetick_Attach("SystemTimer",&hTimetick);
     if (hTimetick == NULL)
     {
       ELF_LOADER_DEBUG(ERR_MODE_ELF_LOADER , ERR_ELF_LOADER_DALTIMER_ATTACH_FAIL , "DalTimer attached failed");  
       free(wrapper_data);
       free(pd_info_carry->eloader_thread.sig);
       pthread_exit(0);
       return NULL;
     }
     result = DalTimetick_GetTimetick64(hTimetick, &tstart);
     // We wait at most 1 min from here, in case RFS server isn't ready on HLOS side
     while(tmin < RETRY_IN_MINS) 
     {
       
       //search vendor folder first
       rfs_err = rfs_stat(pd_info_carry->rfs_path_vendor, fstat_buf);

       //search vendor_2 folder next  
       if (rfs_err == RFS_ENOENT)
       {
         rfs_err = rfs_stat(pd_info_carry->rfs_path_vendor_2, fstat_buf);
       }
       //search default folder next  
       if (rfs_err == RFS_ENOENT)
       {
         //search default folder last
         rfs_err = rfs_stat(pd_info_carry->rfs_path, fstat_buf);
       }
       
       //we break, either no error or cannot find the file
       if (rfs_err == RFS_ENOERROR || rfs_err == RFS_ENOENT)
         break;
       else 
       {
         DalTimetick_GetTimetick64(hTimetick, &t1);
         t1 =  t1 - tstart;
         result = DalTimetick_CvtFromTimetick64(hTimetick, t1, T_MIN, &tmin);
       }
     }/* while */
     if (rfs_err == RFS_ENOERROR) 
     {
       //RFS server is UP 
       //Call PD monitor to Spawn PD 
       hPDElfLoader = pd_mon_spawn(NULL, pd_info_carry->process_name);
     }
     else if (rfs_err == RFS_ENOENT)
     {
       // we bail out because we did not find the image
       ELF_LOADER_DEBUG(ERR_MODE_FS_GATEWAY , ERR_ELF_LOADER_WLAN_IMG_NOT_FOUND , "RFS_ENOENT - cannot find Wlan image, CANNOT spawn a PD"); 
     }
     else //if ((tmin>= RETRY_IN_MINS) || (rfs_err != RFS_ENOERROR))
     {
       // we bail out after we reach limit
       ELF_LOADER_DEBUG(ERR_MODE_FS_GATEWAY , ERR_ELF_LOADER_RFS_SERVER_ERR , "RFS SERVER isn't ready or RFS Read ERROR, CANNOT spawn a PD");  
     }
   }
   //Kill the thread once the signal is rcved 
   if (signal & ELOADER_THREAD_COMPLETED_ACK) 
   {
     qurt_anysignal_clear(pd_info_carry->eloader_thread.sig, ELOADER_THREAD_COMPLETED_ACK);
     qurt_printf("Elf Loader removes PD spwan thread from ROOT PD.\n");
   }
   free(fstat_buf);
   qurt_signal_destroy(pd_info_carry->eloader_thread.sig);
   free(pd_info_carry->eloader_thread.sig);
   pthread_exit(0);
   return NULL;
}/* eloader_sesrvreg_query */

 

 /* =====================================================================
 * Function:
 *    libdl_init 
 *
 * Description:
 *      This function invokes service regirstry and query which PD to start with
 *
 * Parameters:
 *     eloader_pd_info_t *pd_info_carry; contain both domain name and process name.
 * Returns:
 *     None
 * =====================================================================  */

void libdl_init(eloader_pd_info_t *pd_info_carry)
{

    pthread_attr_t attr;
    pd_info_carry->eloader_thread.sig = malloc(sizeof( qurt_anysignal_t));

    if (pd_info_carry->eloader_thread.sig == NULL )
    {
       free(pd_info_carry->eloader_thread.sig);
       ELF_LOADER_DEBUG(ERR_MODE_ELF_LOADER , ERR_ELF_LOADER_MEM_ALLOC , "Elf Loader thread creation failed! Malloc failed");
       return;
    }
    //Try to init the signal..
    qurt_anysignal_init (pd_info_carry->eloader_thread.sig);
    pthread_attr_init(&attr);   //Init the params
    /* Set Detached state to allow the therad to be kill after it's done */
    attr.detachstate= PTHREAD_CREATE_DETACHED;
    /* Set priority as low */
    attr.priority=eloader_thread_priority++;
    /* Set stack size as small size */
    attr.stacksize=SMALL_STACK_SIZE;
    //set name for the thread
    snprintf(attr.name,PTHREAD_NAME_LEN,"eloader-%s",pd_info_carry->process_name);
    if (QURT_EOK != pthread_create(&pd_info_carry->eloader_thread.tid,&attr,eloader_servreg_query, (void *) pd_info_carry)) 
    {
       ELF_LOADER_DEBUG(ERR_MODE_ELF_LOADER , ERR_ELF_LOADER_THREAD_CREAT , "Elf Loader thread creation failed! User PD isn't spawn");
       free(pd_info_carry->eloader_thread.sig);
       return;
    }
    
} //libdl_init 
/*=====================================================================
 * Function:
 *    libdl_testpd_init 
 *
 * Description:
 *      This function invokes Test PD and WLAN PD if .jsn exists.
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================*/
 void libdl_testpd_init(void)
{
#if ENABLE_SERVREG_AUTO_SPAWN_PD
   //Call service registry to see if we need to spawn Test User PD or not */
   libdl_init(&pd_info_carry[N_TEST_PD]);
   libdl_init(&pd_info_carry[N_WLAN_PD]);
#else 
   PD_MON_HANDLE hPDElfLoader;
   hPDElfLoader = pd_mon_spawn(NULL, pd_info_carry[N_TEST_PD].process_name);
 //  hPDElfLoader = pd_mon_spawn(NULL, pd_info_carry[N_WLAN_PD].process_name);
#endif /* #if */
  return;
}/* libdl_testpd_init */
/* =====================================================================
 * Function:
 *    libdl_wlanpd_init 
 *
 * Description:
 *      This function invokes WLAN PD if .jsn exists.
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================  */
 void libdl_wlanpd_init(void)
   {
#if ENABLE_SERVREG_AUTO_SPAWN_PD
  libdl_init(&pd_info_carry[N_WLAN_PD]);
#else 
  PD_MON_HANDLE hPDElfLoader;
  hPDElfLoader = pd_mon_spawn(NULL, pd_info_carry[N_WLAN_PD].process_name);
#endif /* #if */
   return;
}/* libdl_wlanpd_init */
