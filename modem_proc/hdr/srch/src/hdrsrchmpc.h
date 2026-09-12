

/*===========================================================================

        H D R   M O D E M   P O W E R    C O L L A P S e 

GENERAL DESCRIPTION
  Header file for the modem power collapse module

EXTERNALIZED FUNCTIONS  
  boolean hdrsrchmpc_check_precover_flag
  boolean hdrsrchmpc_check_psleep_flag
  boolean hdrsrchmpc_ok_to_collapse
  void    hdrsrchmpc_init
  void    hdrsrchmpc_update_stats
  void    hdrsrchmpc_prep_collapse
  void    hdrsrchmpc_pcollapse_save_cb
  void    hdrsrchmpc_pcollapse_restore_cb
  void    hdrsrchmpc_restore_mdsp
  void    hdrsrchmpc_print_stats


Copyright (c) 2005, 2006 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

              EDIT HISTORY FOR FILE
$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchmpc.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/06/2006 jyw     Created this file.

===========================================================================*/

#include "hdr_variation.h"

