/*============================================================================
@file npa_target_routines.c

Copyright (c) 2010 - 2014 Qualcomm Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR

$Header: //components/rel/core.mpss/10.0/power/npa/hw/core_guest/npa_target_routines.c#1 $
============================================================================*/

#ifdef NPA_USES_QDI
/* Protocol init routine, defined in npa_remote_qdi_protocol.c */
extern void npa_qdi_remote_init(void);
#endif

void npa_target_init( void )
{
#ifdef NPA_USES_QDI
  npa_qdi_remote_init();
#endif
}
