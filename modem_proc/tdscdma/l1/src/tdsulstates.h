#ifndef UL_STATES_H
#define UL_STATES_H


/*===========================================================================
                           U L _ S T A T E S . H

DESCRIPTION
This file contains the prototypes for all of the state processing routines
for the uplink controller.


Copyright (c) 2000-2004 by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/tdsulstates.h_v   1.8   29 May 2002 11:40:34   msatyana  $
$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsulstates.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
05/08/09   sup      Changing the includes for the renaming of header files  dec.h, enci.h, l1m.h, srch.h, srchi.h, 
                    srchlog.h to tdsdec.h, tdsenci.h, tdsl1m.h, tdssrch.h, tdssrchi.h, tdssrchlog.h
03/02/07    am      Added support for log on demand for 4303 logpkt
08/14/06    yiz     Mainline HSUPA code
10/22/04    yiz     Add tdsulstates_ul_dch_sync_a_pending flag.
05/29/02    m       Added prototypes for LOD functions
01/14/01    sh      Added flag to indicate a module cleanup is pending.
01/18/2001  mdl     get next L1M state as input parameter to cleanup routines
09/22/00    sk      file created.
===========================================================================*/

#include "tdscdma_variation.h"
#include "tdsl1m.h"

#ifdef FEATURE_TDSCDMA_HSUPA
#include "tdseulcfg.h"
#endif

/* UL states */
typedef enum {
  /* un-initialized */
  TDSULSTATES_UNINIT,
  /* initialized */
  TDSULSTATES_INIT,
  /* PRACH active */
  TDSULSTATES_PRACH_TX,
  /* UL DPCH active */
  TDSULSTATES_DPCH_TX,
  /* Re-sync in HHO */
  TDSULSTATES_HHO_RE_SYNC,
  /* un-recoverable error */
  TDSULSTATES_ERROR
} tdsulstates_enum_type;

/* Initial UL state is Un-Initialized */
extern tdsulstates_enum_type tds_ul_state;

/* Flag to indicate a module cleanup is pending */
extern boolean tdsulstates_ul_module_cleanup_flg;
extern boolean tdsulstates_ul_module_drop_flg;


#ifdef FEATURE_TDSCDMA_HSUPA
/* Flag to indicate eul uplink module cleanup is pending This is used 
   in case of DCH->IDLE transition and if EDPCH is active*/
extern boolean tdsulstates_e_ul_module_cleanup_flg;
#endif /*end of FEATURE_TDSCDMA_HSUPA */

/*===========================================================================
FUNCTION        TDSULSTATES_INIT

FILE NAME       

DESCRIPTION     This is the one time initialization function for the uplink
                controller.  It just calls the associated uplink control and 
                encoder driver initializers.

DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS    
===========================================================================*/
void tdsulstates_ul_init(void);


/*===========================================================================
FUNCTION        UL_CLEANUP

FILE NAME       

DESCRIPTION     This is the one time cleanup routine for the uplink 
                controller. It calls the associated uplink control and
                encoder driver cleanup routines.
                
DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS    
===========================================================================*/
boolean tdsulstates_ul_cleanup(tdsl1m_state_enum_type next_state);


/*===========================================================================
FUNCTION        UL_IDLE_INIT

FILE NAME       

DESCRIPTION     This is the IDLE state initializer for the uplink controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void tdsulstates_ul_idle_init(void);


/*===========================================================================
FUNCTION        UL_IDLE_CLEANUP

FILE NAME       

DESCRIPTION     This is the IDLE state cleanup routine for the uplink 
                controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
boolean tdsulstates_ul_idle_cleanup(tdsl1m_state_enum_type next_state);


/*===========================================================================
FUNCTION        UL_ACQ_INIT

FILE NAME       

DESCRIPTION     This is the ACQ state initializer for the uplink controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void tdsulstates_ul_acq_init(void);


/*===========================================================================
FUNCTION        UL_ACQ_CLEANUP

FILE NAME       

DESCRIPTION     This is the ACQ state cleanup routine for the uplink 
                controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
boolean tdsulstates_ul_acq_cleanup(tdsl1m_state_enum_type next_state);

/*===========================================================================
FUNCTION        TDSULSTATES_UL_SYNC_INIT

FILE NAME

DESCRIPTION     This is the SYNC state initializer for the uplink controller.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsulstates_ul_sync_init(void);

/*===========================================================================
FUNCTION        TDSULSTATES_UL_SYNC_CLEANUP

FILE NAME       

DESCRIPTION     This is the SYNC state cleanup routine for the uplink 
                controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
boolean tdsulstates_ul_sync_cleanup(tdsl1m_state_enum_type next_state);


/*===========================================================================
FUNCTION        UL_BCH_INIT

FILE NAME       

DESCRIPTION     This is the BCH state initializer for the uplink controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void tdsulstates_ul_bch_init(void);

/*===========================================================================
FUNCTION        UL_BCH_CLEANUP

FILE NAME       

DESCRIPTION     This is the BCH state cleanup routine for the uplink 
                controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
boolean tdsulstates_ul_bch_cleanup(tdsl1m_state_enum_type next_state);


/*===========================================================================
FUNCTION        UL_PCH_INIT

FILE NAME       

DESCRIPTION     This is the PCH state initializer for the uplink controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void tdsulstates_ul_pch_init(void);

/*===========================================================================
FUNCTION        UL_PCH_CLEANUP

FILE NAME       

DESCRIPTION     This is the PCH state cleanup routine for the uplink 
                controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
boolean tdsulstates_ul_pch_cleanup(tdsl1m_state_enum_type next_state);

/*===========================================================================
FUNCTION        UL_FACH_INIT

FILE NAME       

DESCRIPTION     This is the FACH state initializer for the uplink controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void tdsulstates_ul_fach_init(void);

/*===========================================================================
FUNCTION        UL_FACH_CLEANUP

FILE NAME       

DESCRIPTION     This is the FACH state cleanup routine for the uplink 
                controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
boolean tdsulstates_ul_fach_cleanup(tdsl1m_state_enum_type next_state);

/*===========================================================================
FUNCTION        UL_FACH_CLEANUP_NEW

FILE NAME       

DESCRIPTION     
  This is the FACH state cleanup routine for the uplink controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
boolean tdsulstates_ul_fach_cleanup_new(tdsl1m_state_enum_type next_state);


/*===========================================================================
FUNCTION        UL_FACH_DROP_ALL

FILE NAME       

DESCRIPTION     
  This is the FACH state cleanup routine for the uplink controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
boolean tdsulstates_ul_fach_drop_all(tdsl1m_state_enum_type next_state);


/*===========================================================================
FUNCTION        UL_DCH_INIT

FILE NAME       

DESCRIPTION     This is the DCH state initializer for the uplink controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
void tdsulstates_ul_dch_init(void);

/*===========================================================================
FUNCTION        UL_DCH_CLEANUP

FILE NAME       

DESCRIPTION     This is the DCH state cleanup routine for the uplink 
                controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
boolean tdsulstates_ul_dch_cleanup(tdsl1m_state_enum_type next_state);

/*===========================================================================
FUNCTION        UL_DCH_CLEANUP_NEW

FILE NAME       

DESCRIPTION     
  This is the DCH state cleanup routine for the uplink controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
boolean tdsulstates_ul_dch_cleanup_new(tdsl1m_state_enum_type next_state);


/*===========================================================================
FUNCTION        UL_DCH_DROP_ALL

FILE NAME       

DESCRIPTION     
  This is the DCH state drop all routine for the uplink controller.

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
boolean tdsulstates_ul_dch_drop_all(tdsl1m_state_enum_type next_state);



#ifdef FEATURE_L1_LOG_ON_DEMAND
/*===========================================================================
FUNCTION        UL_LOG_INIT

FILE NAME

DESCRIPTION
  This function registers all the UL log on demand handlers to
  corresponding log code given out by QXDM

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsulstates_ul_log_init (void);

#ifdef FEATURE_TDSCDMA_HSUPA
/*===========================================================================
FUNCTION     EUL_LOG_INIT

DESCRIPTION  This function registers all the EUL log on demand handlers to
             corresponding log code given out by QXDM

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tdsulstates_eul_log_init (void);
#endif /* FEATURE_TDSCDMA_HSUPA*/
#endif

/*===========================================================================
FUNCTION        tdsulstates_init_ul_state

FILE NAME       

DESCRIPTION     
  This function is used by other module to init UL state.

DEPENDENCIES    
  None

RETURN VALUE    
  None

SIDE EFFECTS    
  None
===========================================================================*/
void tdsulstates_init_ul_state(void);

#endif
