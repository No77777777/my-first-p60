/*============================================================================
@file npa_target_routines.c

Copyright (c) 2010-2020 Qualcomm Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR

$Header: //components/rel/core.mpss/10.0/power/npa/hw/core_root/npa_target_routines.c#3 $
============================================================================*/
#include "npa_internal.h"
#include "npa_remote_resource.h"

/* RPM available callback, defined in npa_remote_rpm_protocol.c */
extern void npa_rpm_available_cb( void*, unsigned int, void*, unsigned int );

#ifdef NPA_USES_QDI
/* NPA/QDI init routine, defined in npa_remote_qdi_drv.c */
extern void npa_qdi_init(void);

static const char *resources_for_userpds[] = 
{ 
  "/icb/alc",
  "/core/cpu/tcm",
  "/clk/bimc",
  "/clk/snoc",
  "/clk/ipa",
  "/clk/cpu",
  "/core/cpu/latency/usec",
  "/sleep/max_duration/usec",
  "/icb/arbiter",
  "/core/cpu/vdd",
  "/tech/arb",
  "/core/mca",
  "/core/mca/clk",
  "/core/cpu/busy",
  "/mcpm/stats",
  "/mcpm/conc_mgr",
};
#endif /* NPA_USES_QDI */

void npa_target_init( void )
{
  npa_resource_available_cb( "/init/rpm", npa_rpm_available_cb,  NULL ); 

#ifdef NPA_USES_QDI
  npa_qdi_init();
  
  /* Publish some resources */
  npa_remote_publish_resources( NPA_ALL_PDS, 
                                sizeof(resources_for_userpds)/sizeof(char *),
                                resources_for_userpds );
  
  /* Publish some other resources */
  npa_remote_publish_resource( NPA_ALL_PDS, "/clk/qdss" );
#endif
}

#ifdef NPA_SCHEDULED_REQUESTS_SUPPORTED
#include "tms_rcinit.h"

/* Create the NPA Scheduler Thread */
void npa_scheduler_init_rctask( void )
{
  RCINIT_INFO handle = rcinit_lookup("npaScheduler");

  if (RCINIT_NULL != handle)
  {
    rcinit_initfn_spawn_task(handle, npaScheduler_task);
  }
}
#endif /* NPA_SCHEDULED_REQUESTS_SUPPORTED */
