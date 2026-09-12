

/*===========================================================================

        H D R   M O D E M   P O W E R    C O L L A P S e ( MPC )

GENERAL DESCRIPTION
  The file contains the MPC module which redirect and hardware io access and
  mdsp access to make the access history; It also contains other functions
  to recover the mdsp and hardware after power collapse sleeps


EXTERNALIZED FUNCTIONS
  boolean hdrsrchmpc_check_precover_flag
  boolean hdrsrchmpc_check_psleep_flag
  void    hdrsrchmpc_init
  void    hdrsrchmpc_update_stats
  void    hdrsrchmpc_out_dword
  void    hdrsrchmpc_out_dword_masked
  uint32  hdrsrchmpc_in_dword_masked
  uint32  hdrsrchmpc_in_dword
  uint16  hdrsrchmpc_mdsp_read16
  void    hdrsrchmpc_mdsp_write16
  uint32  hdrsrchmpc_mdsp_read32
  void    hdrsrchmpc_mdsp_write32
  void    hdrsrchmpc_prep_collapse
  void    hdrsrchmpc_restore_mdsp
  void    hdrsrchmpc_pcollapse_save_cb
  void    hdrsrchmpc_pcollapse_restore_cb
  boolean hdrsrchmpc_ok_to_collapse
  void    hdrsrchmpc_print_stats

INITIALIZATION AND SEQUENCING REQUIREMENTS
  hdrsrchmpc_init() has to be called first

Copyright (c) 2005 - 2010 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

              EDIT HISTORY FOR FILE
$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchmpc.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/22/09   wsh     Replaced msmhwio.h with msm.h 07/06/09   lyl     Fixed the compilation issue in RUMI build.
04/17/09   rkc     Used renamed HWIO_MDSP_UP_IRQ_VECTOR_OUT() on Poseidon2.
06/29/05   jyw     Merged with latest build after the initial test.
11/29/05   jyw     Created this file.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "hdr_variation.h"
#include "msm.h"
#include "hdrdebug.h"
#include "hdrsrchstate.h"
#include "hdrsrchmpc.h"
#include "hdrmdspmcr.h"

