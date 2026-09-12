/*==========================================================================*/
/*!
  @file 
  ds_Net_Init.cpp

  @brief
  This file provides functions that are used to perform initializations 
  of DSNET module.

  Copyright (c) 2008-2014 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
*/
/*==========================================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_Init.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-12-01 aa  Removing usage of scratchpad. Using system heap instead.
  2009-08-17 pp  DS_Net module uses system heap memory via scratchpad.
  2009-02-27 hm  Added scratchpad memory for DSNET client.
  2008-05-02 hm  Created module.

===========================================================================*/

/*---------------------------------------------------------------------------
  Include Files
---------------------------------------------------------------------------*/
#include "comdef.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Net_Init.h"
#include "ds_Net_MemManager.h"
#include "ds_Net_EventManager.h"
#include "ds_Net_Platform.h"
#include "ds_Net_NetworkFactory.h"

#include "ds_Net_INetworkFactory.h"
#include "ds_Net_ITechUMTS.h"
#include "ds_Net_CNetworkFactory.h"
#include "ds_Net_CreateInstance.h"

#include "ds_Net_NetworkFactory.h"
#include "ds_Net_TechUMTSFactory.h"

using namespace ds::Net;

/*---------------------------------------------------------------------------
  Local declarations.
---------------------------------------------------------------------------*/

#define DS_NET_INIT_ASSERT(expression) \
         dsnet_init_assert_wrapper(__LINE__, expression)

/*===========================================================================

FUNCTION dsnet_init_assert_wrapper()

DESCRIPTION
  Wrapper function for DS_NET_INIT_ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void dsnet_init_assert_wrapper
(
  unsigned int     line_num,
  int              expression
)
{
  if ( !expression )
  {
    ERR_FATAL("DSNET_FATAL at line:%d ",  
                                   line_num,0,0);  
  }
}/* dsnet_init_assert_wrapper */



/*===========================================================================
FUNCTION PS_LOGGING_ERR_FATAL

DESCRIPTION
  Wrapper function for ERROR_FATALS
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
void ps_logging_err_fatal
(  
  const char      *file_name,
  const char      *func_name,
  unsigned int     line_num,
  const char       *str
)
{ 

  /* Print the File name ,Function name and Line number before calling ERR_FATAL */
  if (NULL != file_name && NULL != func_name) 
  {
    LOG_MSG_STR_INFO1_03("ERROR_FATAL at %s , function: %s ,line:%d ",
                          file_name,func_name,line_num);
  }
      
  if (NULL == str)
  {
    ERR_FATAL("Assertion failed at %s line: %d", file_name, line_num, 0 );
    return;
  }

  ERR_FATAL("Error Fatal: %s at %s line: %d", str,file_name, line_num);

}/* ps_logging_err_fatal */

/*===========================================================================

FUNCTION PS_LOGGING_ASSERT

DESCRIPTION
  Wrapper function for ASSERTION
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
void ps_logging_assert
(
  const char      *file_name,
  const char      *func_name,
  unsigned int     line_num,
  int              expression
)
{
  /* Print the file name and line number before calling ERR_FATAL */
  if ((!expression))
  {
    LOG_MSG_STR_INFO1_03("ASSERT at %s , function: %s , line: %d ASSERT",
                         file_name,func_name,line_num);
	
    ERR_FATAL("Assertion failed at %s ,function: %s , line: %d", file_name, func_name,line_num);
  }

}/* ps_logging_assert */


void DSNetPowerup
(
  void
)
{
  int res = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MemoryManager::MemPoolInit();

  NetPlatform::Init();

  ds::Net::EventManager::Init();

  res = NetworkFactory::InstanceInit();
  if (AEE_SUCCESS != res) {
    DS_NET_INIT_ASSERT(0);
  }

  res = TechUMTSFactory::InstanceInit();
  if (AEE_SUCCESS != res) {
    DS_NET_INIT_ASSERT(0);
  }

} /* DSNetPowerupInit() */

void DSNetPowerdown
(
  void
)
{
  int res = 0;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  EventManager::Deinit();

  res = NetworkFactory::InstanceRelease();
  if (AEE_SUCCESS != res) {
    DS_NET_INIT_ASSERT(0);
  }

  res = TechUMTSFactory::InstanceRelease();
  if (AEE_SUCCESS != res) {
    DS_NET_INIT_ASSERT(0);
  }

} /* DSNetPowerdown() */


void DSNetInit
(
  void
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

} /* DSNetInit() */

void DSNetDeinit
(
  void
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

} /* DSNetDeinit() */


