/*===========================================================================
               PS _ SOCK _ REMOTESOCKET _ ATTRIBUTE_EPHEMERAL . CPP

DESCRIPTION

  PS _ SOCK _ PLATFORM _ LocalEphemeralPortGen - Implementation.

FUNCTIONS

Copyright (c) 2016 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/pssock/src/PS_Sock_Platform_LocalEphemeralPortGen.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
03/03/15    ad     Initial version 
===========================================================================*/

/*===========================================================================

                                INCLUDE FILES

===========================================================================*/
#include <stringl/stringl.h>
#include "ds_Utils_DebugMsg.h"
#include "PS_Sock_RemoteSocket_Manager.h"
#include "PS_Sock_Platform_LocalEphemeralPortGen.h"
#include "PS_Sock_Platform_SocketFactory.h"
#include "ps_utils.h"
#include "ps_utils_defs.h"

using namespace PS::Sock::RemoteSocket;
using namespace ds::Error;
using namespace ds::Utils;

/*===========================================================================
                                    PRIVATE DATATYPES
===========================================================================*/

static uint16  ephemeralBasePort;
static uint16  ephemeralPortOffset;

typedef PACKED struct PACKED_POST
{
 uint16  basePort;
 uint16  portOffset;
} ephemeral_config_info_type;
/*===========================================================================
                                    PUBLIC  FUNCTIONS
===========================================================================*/

void LocalEphemeralPortGen::Init(void)
{
  ephemeral_config_info_type  ephemeralConf;
  const char      * NVConfigFilepath = 
      PS_NV_CONF_ITEM_INFO(PS_NV_CONF_EPHEMERAL_PORT_CONFIG);
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  if (0 != ps_read_efs_nv( NVConfigFilepath, 
                           &ephemeralConf, 
                           sizeof(ephemeralConf) ) )
  {
    /*-------------------------------------------------------------------------
      Set defaults if NV not configured
    -------------------------------------------------------------------------*/
    ephemeralConf.basePort  = EPHEMERAL_PORT_BASE;
    ephemeralConf.portOffset = EPHEMERAL_PORT_MAX_OFFSET;
    
    if(ps_write_efs_nv(NVConfigFilepath,
                       &ephemeralConf,
                       sizeof(ephemeralConf)))
    {
      LOG_MSG_ERROR_0("failed to write port config to NV item");
    }
  }

  /*-------------------------------------------------------------------------
    Validate base port and offset read from NV
  -------------------------------------------------------------------------*/
  if ( 0 == ephemeralConf.basePort ||
       0 == ephemeralConf.portOffset )
  {
    /*-------------------------------------------------------------------------
      Set default values
    -------------------------------------------------------------------------*/
    ephemeralConf.basePort  = EPHEMERAL_PORT_BASE;
    ephemeralConf.portOffset = EPHEMERAL_PORT_MAX_OFFSET;
  }
  
  SetEphemeralBasePort( ephemeralConf.basePort );
  SetEphemeralPortOffset( ephemeralConf.portOffset);

  LOG_MSG_INFO1_2("Ports config info from NV base %d offset %d",
        ephemeralConf.basePort,ephemeralConf.portOffset  );
    
} /* LocalEphemeralPortGen::Init */

void LocalEphemeralPortGen::DeInit(void)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

} /* LocalEphemeralPortGen::DeInit */

uint16 LocalEphemeralPortGen::GenerateEphemeralPort
(
   ds::Sock::ProtocolType  protocol
)
{
  uint16           basePort  = GetEphemeralBasePort();
  uint16           maxPort   = basePort + GetEphemeralPortOffset();
  PortPreference   portPref;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  portPref.port_property   = DSS_PORT_PROP_MIN;
  portPref.base_port_range = basePort;
  portPref.max_port_range  = maxPort;
  
  return GenerateEphemeralPort( protocol, portPref);
}/* LocalEphemeralPortGen::GenerateEphemeralPort() */

uint16 LocalEphemeralPortGen::GenerateEphemeralPort
(
  ds::Sock::ProtocolType  protocol,
  PortPreference          portPref
)
{
  SocketFactory                * platformSockFactoryPtr;
  ds::SockAddrIN6InternalType  tmpSockAddr;
  int32                        iter;
  uint16                       primaryPort = 0;
  uint16                       secondaryPort =0;
  uint16                       NumOfItr;
  ds::Sock::ProtocolType       SecSockProtocol;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_4("LocalEphemeralPortGen::GenerateEphemeralPort(): "
                           "Proto %d  property %d max %d base %d",
                           protocol, 
                           portPref.port_property,
                           portPref.max_port_range,
                           portPref.base_port_range);

  tmpSockAddr.family = ds::AddrFamily::QDS_AF_INET6;
  memscpy( tmpSockAddr.addr, sizeof( ds::INAddr6Type),
           &ps_in6addr_any, sizeof( ds::INAddr6Type));

  /*-------------------------------------------------------------------------
    1. Generate a random value between EPHEMERAL_PORT_BASE and
       EPHEMERAL_PORT_BASE + EPHEMERAL_PORT_MAX_OFFSET
    2. Check if any socket is already using this port for this socket's
       protocol

    Repeat above 2 steps until a port is found or port space is exhausted
  -------------------------------------------------------------------------*/
  NumOfItr = portPref.max_port_range - portPref.base_port_range;

  platformSockFactoryPtr = SocketFactory::CreateInstance();
      
  DS_UTILS_ASSERT( 0 != platformSockFactoryPtr); 

  if ( DSS_PORT_PROP_ADJACENT_START_EVEN == portPref.port_property ||
       DSS_PORT_PROP_SYMMETRIC           == portPref.port_property )
  {
    for (iter = 0; iter <= NumOfItr; iter++)
    {
      /*-------------------------------------------------------------------------
            Random Number generate API return same number when port range is small hence we do
            linear search for small port range.
          -------------------------------------------------------------------------*/
      if(NumOfItr > EPHEMERAL_PORT_LINEAR_SEARCH_THRESHOLD)
      {
        primaryPort = ps_utils_generate_16bit_rand_num_by_range( 
                     portPref.base_port_range, portPref.max_port_range); 
      }
      else
      {
        primaryPort =  portPref.base_port_range + iter;
      }
      
      if( (DSS_PORT_PROP_ADJACENT_START_EVEN == portPref.port_property)  && 
          ( primaryPort % 2 != 0  || primaryPort == portPref.max_port_range ))
      {
        primaryPort = 0;
        continue;
      }
      
      tmpSockAddr.port = ps_htons( primaryPort);
      if (true == platformSockFactoryPtr->IsSockAddrInUse( protocol, &tmpSockAddr, false, true))
      {
        primaryPort = 0;
        continue;
      }
      //Found  primary port. Search companion port
      
      if(DSS_PORT_PROP_ADJACENT_START_EVEN == portPref.port_property)
      {
        secondaryPort   = primaryPort + 1;
        SecSockProtocol = protocol;
      }
      else
      {
        secondaryPort   = primaryPort;
        SecSockProtocol = OTHERPROTO(protocol);
      }
      
      tmpSockAddr.port = ps_htons( secondaryPort);

      if (false == platformSockFactoryPtr->IsSockAddrInUse( SecSockProtocol,
                     &tmpSockAddr, false, true))
      {
        break;
      }
      primaryPort   = 0;
      secondaryPort = 0;
    }
  }
  else
  {
    for (iter = 0; iter <= NumOfItr; iter++)
    {
      primaryPort = ps_utils_generate_16bit_rand_num_by_range( 
                       portPref.base_port_range, portPref.max_port_range);    
    
      tmpSockAddr.port = ps_htons( primaryPort);
      
      if (false == platformSockFactoryPtr->IsSockAddrInUse( protocol, &tmpSockAddr, false, true))
      {
        break;
      }
      
      primaryPort = 0;
    }
  }
  LOG_MSG_FUNCTION_EXIT_2("LocalEphemeralPortGen::GenerateEphemeralPort(): "
                          "Returning Primary port %d Second port %d", 
                           primaryPort, secondaryPort);
  
  DS_UTILS_RELEASEIF(platformSockFactoryPtr);
  /*-----------------------------------------------------------------------
      GenerateEphemeralPort return port in Network byte order
  -----------------------------------------------------------------------*/ 
  return  ps_htons(primaryPort);

} /* LocalEphemeralPortGen::GenerateEphemeralPort() */

void LocalEphemeralPortGen::SetEphemeralBasePort
(
  uint16  basePort
)               
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ephemeralBasePort = basePort;
}/*SocketFactory::SetEphemeralBasePort*/

uint16 LocalEphemeralPortGen::GetEphemeralBasePort
(
  void
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return ephemeralBasePort;
}/*SocketFactory::GetEphemeralBasePort*/

void LocalEphemeralPortGen::SetEphemeralPortOffset
(
  uint16  offset
)              
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ephemeralPortOffset = offset;
}/*SocketFactory::SetEphemeralPortOffset*/

uint16 LocalEphemeralPortGen::GetEphemeralPortOffset
(
  void
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return ephemeralPortOffset;
}/*SocketFactory::GetEphemeralPortOffset*/


