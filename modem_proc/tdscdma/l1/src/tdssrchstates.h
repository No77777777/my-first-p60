#ifndef TDSSRCHSTATES_H
#define TDSSRCHSTATES_H

/*===========================================================================

                          Search States Header File

GENERAL DESCRIPTION
  
EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS  
  

  Copyright (c) 2000, 2011 by Qualcomm Technologies Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdssrchstates.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
-------    ---     --------------------------------------------------------


===========================================================================*/
#include "comdef.h"
#include "tdsl1m.h"


/*===========================================================================

FUNCTION tds_search_init

FILE NAME

DESCRIPTION
  This function initializes searcher global parameters - write-once parameters.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tds_search_init( void );



/*===========================================================================

FUNCTION TDSSRCHIDLE_INIT

FILE NAME

DESCRIPTION
  This function performs search module idle state initializations. It is called
  by L1 manager when L1 enters the Idle state. 

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tdssrchidle_init(void);



/*===========================================================================
FUNCTION        TDSSRCHIDLE_CLEANUP

FILE NAME       

DESCRIPTION     

DEPENDENCIES    

RETURN VALUE    

SIDE EFFECTS    
===========================================================================*/
extern boolean tdssrchidle_cleanup(tdsl1m_state_enum_type next_state);


/*===========================================================================

FUNCTION tdssrchacq_init

FILE NAME

DESCRIPTION
  This function performs search module acquisiton initializations. It is called
  by L1 manager when L1 enters the "Acquisition" state. 

DEPENDENCIES
  It is assumed that RF has tuned to correct WCDMA frequency before this 
  function is called.

RETURN VALUE
  Boolean value TRUE.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tdssrchacq_init(void);

/*===========================================================================

FUNCTION tdssrchsync_init

FILE NAME

DESCRIPTION
  This function performs search module initializations. It is called
  by L1 manager when L1 enters the SYNC state. 

DEPENDENCIES
  

RETURN VALUE
  none

SIDE EFFECTS
  None.

===========================================================================*/
extern void tdssrchsync_init(void);


/*===========================================================================
FUNCTION        

FILE NAME       

DESCRIPTION     

DEPENDENCIES    

RETURN VALUE    
  Boolean value TRUE.

SIDE EFFECTS    
===========================================================================*/
extern boolean tdssrchacq_cleanup(tdsl1m_state_enum_type next_state);

/*===========================================================================
FUNCTION    TDSSRCHSYNC_CLEANUP 

FILE NAME       

DESCRIPTION     

DEPENDENCIES    

RETURN VALUE    
  Boolean value TRUE.

SIDE EFFECTS    
===========================================================================*/
extern boolean tdssrchsync_cleanup(tdsl1m_state_enum_type next_state);


/*===========================================================================

FUNCTION SRCH_BCH_INIT

FILE NAME

DESCRIPTION
  This function performs search module BCH initializations. It is called
  by L1 manager when L1 enters the "BCH" state. 

DEPENDENCIES
  None.

RETURN VALUE
  Boolean value TRUE.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tdssrchbch_init( void );

/*===========================================================================

FUNCTION SRCH_BCH_CLEANUP

FILE NAME

DESCRIPTION
  This function performs search module BCH cleanup. It is called
  by L1 manager when L1 exits the "BCH" state. 

DEPENDENCIES
  None.

RETURN VALUE
  Boolean value TRUE.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean tdssrchbch_cleanup( tdsl1m_state_enum_type next_state );


/*===========================================================================

FUNCTION TDSSRCHPCH_INIT

FILE NAME

DESCRIPTION
  This function performs search module BCH initializations. It is called
  by L1 manager when L1 enters the "PCH" state. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tdssrchpch_init( void );


/*===========================================================================

FUNCTION TDSSRCHPCH_CLEANUP

FILE NAME

DESCRIPTION
  This function performs search module PCH cleanup. It is called
  by L1 manager when L1 exits the PCH state. 

DEPENDENCIES
  None.

RETURN VALUE
  Boolean value TRUE.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean tdssrchpch_cleanup( tdsl1m_state_enum_type next_state );


/*===========================================================================

FUNCTION SRCH_DCH_INIT

FILE NAME

DESCRIPTION
  This function performs search module DCH initializations. It is called
  by L1 manager when L1 enters the "DCH" state. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tdssrchdch_init( void );

/*===========================================================================

FUNCTION SRCH_DCH_CLEANUP

FILE NAME

DESCRIPTION
  This function performs search module DCH cleanup. It is called
  by L1 manager when L1 exits the DCH state. 

DEPENDENCIES
  None.

RETURN VALUE
  Boolean value TRUE.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean tdssrchdch_cleanup( tdsl1m_state_enum_type next_state );

#endif
