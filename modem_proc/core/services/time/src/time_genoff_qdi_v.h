#ifndef TIME_GENOFF_QDI_V_H
#define TIME_GENOFF_QDI_V_H
/*=============================================================================

                Time_genoff_qdi_v.h  --  Header File

GENERAL DESCRIPTION
      Time Genoff QDI Layer Header File

EXTERNAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS
   None.

      Copyright (c) 2009 - 2015
      by QUALCOMM Technologies Incorporated.  All Rights Reserved.

=============================================================================*/

/*=============================================================================

                        EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.


$Header: //components/rel/core.mpss/10.0/services/time/src/time_genoff_qdi_v.h#1 $ 
$DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when         who     what, where, why
--------   ---     ------------------------------------------------------------
09/01/15   abh     Changes for correctly handling ATS_PRIVATE bases.
06/20/13   abh     File created

=============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/


#include "qurt_qdi.h"
#include "qurt_qdi_driver.h"
#include "time_genoff_v.h"
#include "time_genoff_client_v.h"


/*===========================================================================
                           MACRO DEFINITION
===========================================================================*/


/* Num of elements that the pipe will be able to queue */
#define TIME_GENOFF_QDI_CLIENT_PIPE_SIZE     (2 * ATS_MAX)

/* Num of max user PD clients to time_genoff */
#define TIME_GENOFF_QDI_MAX_CLIENTS          4

/* Error values retruned by function */
#define TIME_ERR_NONE                       0x0
#define TIME_ERR_QDI_CPY_FAIL               0x1
#define TIME_ERR_CANCEL_WAIT                0x2


/* Name of the time-services QDI driver */
#define TIME_GENOFF_DRIVER_NAME              "/dev/time_genoff"


/* Defines for the method number of the QDI */
#define TIME_GENOFF_QDI_OPR                     (0+QDI_PRIVATE)
#define TIME_GENOFF_QDI_PRIVATE_BASE_INIT       (1+TIME_GENOFF_QDI_OPR)
#define TIME_GENOFF_QDI_PRIVATE_BASE_DEINIT     (1+TIME_GENOFF_QDI_PRIVATE_BASE_INIT)
#define TIME_GENOFF_QDI_GET_UPDATE_INFO			(1+TIME_GENOFF_QDI_PRIVATE_BASE_DEINIT)
#define TIME_GENOFF_QDI_REPORT_OUT_OF_COVERAGE  (1+TIME_GENOFF_QDI_GET_UPDATE_INFO)
#define TIME_GENOFF_QDI_GET_INIT_UPDATES        (1+TIME_GENOFF_QDI_REPORT_OUT_OF_COVERAGE)


/*=============================================================================
                           STRUCTURES
=============================================================================*/


/* Structure for the QDI object */
typedef struct time_genoff_qdi_opener_type {
  qurt_qdi_obj_t  qdiobj;
  uint32          process_idx;
  qurt_pipe_t    *pipe;
}time_genoff_qdi_opener;


/* Structure for passing the update info in between the PD's */
typedef struct {
  /* storage for the updated offsets info */   
  uint32 base;
  time_genoff_client_struct_type time_genoff_client;
}time_genoff_qdi_update_container ;


/*==============================================================================
                            TIME GENOFF SET HELPER INLINE MACROS
=============================================================================*/

static inline int time_genoff_invoke_opr(
                                   int                             handle,
                                   time_genoff_args_ptr            pargs)
{
   return qurt_qdi_handle_invoke( handle, TIME_GENOFF_QDI_OPR, pargs);
}

static inline int time_genoff_invoke_private_base_init(
                                   int                             handle,
                                   char *                          f_name,
                                   uint32 *                        ret_base,
                                   time_genoff_client_ptr          ptime_genoff)
{
   return qurt_qdi_handle_invoke( handle, TIME_GENOFF_QDI_PRIVATE_BASE_INIT, f_name, ret_base, ptime_genoff);
}

static inline int time_genoff_invoke_private_base_deinit(
                                   int                             handle,
                                   uint32                          base,
                                   char *                          f_name)
{
   return qurt_qdi_handle_invoke( handle, TIME_GENOFF_QDI_PRIVATE_BASE_DEINIT, base, f_name);
}

static inline int  time_genoff_invoke_report_out_of_coverage(
                                   int                             handle,
                                   time_bases_type                 base)
{
   return qurt_qdi_handle_invoke( handle, 
   	                              TIME_GENOFF_QDI_REPORT_OUT_OF_COVERAGE, 
   	                              base);
}

static inline int time_genoff_invoke_get_update_info(
	                      int                                    handle, 
                          time_genoff_qdi_update_container       *update_info)
{
   return qurt_qdi_handle_invoke(handle, TIME_GENOFF_QDI_GET_UPDATE_INFO, update_info);
}

static inline int time_genoff_invoke_get_init_updates( int handle )
{
   return qurt_qdi_handle_invoke(handle, TIME_GENOFF_QDI_GET_INIT_UPDATES);
}

#endif /* TIME_GENOFF_QDI_V_H */
