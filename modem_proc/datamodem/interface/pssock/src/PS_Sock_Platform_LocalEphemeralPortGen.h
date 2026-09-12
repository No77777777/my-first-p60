#ifndef PS_SOCK_PLATFORM_LOCALEPHEMERALPORTGEN_H
#define PS_SOCK_PLATFORM_LOCALEPHEMERALPORTGEN_H
/*===========================================================================

                 PS _ SOCK _ PLATFORM _ LocalEphemeralPortGen . H

DESCRIPTION

  PS SOCK REMOTESOCKET ATTRIBUTE EPHEMERAL - Definitions

  
Copyright (c) 2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/pssock/src/PS_Sock_Platform_LocalEphemeralPortGen.h#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
03/03/13    ad    First revision
===========================================================================*/

/*===========================================================================

                                INCLUDE FILES

===========================================================================*/
#include "ds_Utils_DebugMsg.h"
#include "ds_Addr_Def.h"
#include "ds_Sock_Def.h"


using namespace PS::Sock::Platform;

/*===========================================================================

                                 DEFINITIONS

===========================================================================*/

namespace PS
{
  namespace Sock
  {
    namespace RemoteSocket
    { 
    
      #define EPHEMERAL_PORT_BASE                     40000
      #define EPHEMERAL_PORT_MAX_OFFSET               4999
      #define EPHEMERAL_PORT_LINEAR_SEARCH_THRESHOLD  10

      namespace LocalEphemeralPortGen
      {
          /**
            @brief Initializes ephemeral generator resources 
           
            @param None
           
            @retval None
          */        
          void Init(void); 
            
          /**
            @brief Frees ephemeral generator resources
           
            @param None
           
            @retval None
          */          
          void DeInit(void);
          /**
            @brief set ephemeral base port
           
           @param[in] basePort              Ephemeral Base port
           
            @retval None
          */          
        void SetEphemeralBasePort
        (
          uint16  basePort
        );               
          /**
            @brief Get ephemeral base port
           
            @param None
           
            @retval ephemeral base port
          */          
        uint16 GetEphemeralBasePort
        (
          void
        ); 
          /**
            @brief  set ephemeral port offset
           
            @param offset ephemeral port offset
           
            @retval None
          */          

        void SetEphemeralPortOffset
        (
          uint16  offset
        );               
          /**
            @brief get ephemeral port offset
           
            @param None
           
            @retval  ephemeral port offset
          */          

        uint16 GetEphemeralPortOffset
        (
          void
        ); 
          /**
            @brief  This member function will generate ephemeral port based on protocol 
           
            @param None
           
            @retval None
          */          

        uint16 GenerateEphemeralPort
        (
          ds::Sock::ProtocolType  protocol
        );
          /**
            @brief  This member function will generate ephemeral port based on protocol and port preference
           
            @param None
           
            @retval None
          */          
        uint16 GenerateEphemeralPort
        (
            ds::Sock::ProtocolType  protocol,
            PortPreference          portPref
        );

       }
    } /* namespace RemoteSocket */
  } /* namespace SOCK */
} /* namespace PS */



#endif /*PS_SOCK_PLATFORM_LOCALEPHEMERALPORTGEN_H*/
