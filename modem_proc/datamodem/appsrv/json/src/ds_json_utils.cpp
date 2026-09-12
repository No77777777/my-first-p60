/*==============================================================================

                              ds_json_document.cpp

GENERAL DESCRIPTION



  Copyright (c) 2016 by Qualcomm Technologies Incorporated. All Rights Reserved.
==============================================================================*/

/*==============================================================================
  Copyright (c) 2016 IETF Trust and the persons identified as authors of the
  code. All rights reserved

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    Neither the name of Internet Society, IETF or IETF Trust, nor the names of
    specific contributors, may be used to endorse or promote products derived
    from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE
==============================================================================*/

/*==============================================================================
                           EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ----------------------------------------------------------
01/26/16    ml     Created file/Initial version.
==============================================================================*/
#include "ds_json_utils.h"

#include "ds_ASStream.h"
#include "ds_ASBuffer.h"
#include "ds_ASIOStream.h"

#include "data_msg.h"

#include <stringl/stringl.h>
#include <cctype>
#include <cstdlib>



namespace Appsrv
{
namespace JSON
{


bool ds_json_parse_string(Appsrv::Utils::ASIStream& json_iss, ASBuffer& str)
{
  char next_char;
  json_iss.get(next_char);

  while(json_iss.good())
  {
    if( 0 == isprint(next_char) )
    {
      return false;
    }

    if('\"' == next_char)
    {
      // Found end delim
      return true;
    }

    if( '\\' == next_char)
    {
      json_iss.get(next_char);
      if( !json_iss.good() )
      {
        return false;
      }

      switch(next_char)
      {
        case '\"':
        case '\\':
        case '/':
          break;

        case 'b':
          next_char = '\b';
          break;

        case 'f':
          next_char = '\f';
          break;

        case 'n':
          next_char = '\n';
          break;

        case 'r':
          next_char = '\r';
          break;

        case 't':
          next_char = '\t';
          break;

        case 'u':
        {
          // Hex string; get next 4 chars and convert
          char hexval[3];
          memset(hexval, 0, sizeof(char) * 3);

          for(uint16 i = 0; i < 4; ++i)
          {
            json_iss.get(next_char);
            if( !json_iss.good() || 0 == isxdigit(next_char) )
            {
              return false;
            }

            hexval[i%2] = next_char;

            if(1 == i)
            {
              if(0 != strtoul(hexval, NULL, 16))
              {
                // ???: Unsupported? UTF-8 only?
                DATA_APPSRV_MSG2(MSG_LEGACY_ERROR, "json_parse_string - UTF-16 escape not supported; UTF-8 only %d %d", hexval[0], hexval[1]);
                return false;
              }
            }
            else if(3 == i)
            {
              next_char = (char)strtoul(hexval, NULL, 16);
            }
          }
        }
        break;

        default:
        {
          DATA_APPSRV_MSG0(MSG_LEGACY_ERROR, "json_parse_string - Invalid escape char");
          return false;
        }
      } // switch
    } // if

    str.append(next_char);
    json_iss.get(next_char);
  }

  return false;
}




bool ds_json_parse_and_validate_raw_str(Appsrv::Utils::ASIStream& json_iss, ASBuffer& raw_str, const char* cmp_value)
{
  uint32 read_size = strlen(cmp_value);

  if(read_size < raw_str.size())
    return false;

  read_size -= raw_str.size();

  for(uint16 i = 0; i < read_size; ++i)
  {
    char read_char;
    json_iss.get(read_char);

    if( !json_iss.good() || 0 == isalpha(read_char) )
    {
      return false;
    }

    raw_str.append(read_char);
  }

  if ( 0 != strcasecmp(raw_str.c_str(), cmp_value) )
    return false;

  return true;
}



bool ds_json_peek_next_char(Appsrv::Utils::ASIStream& json_iss, char& next_char)
{
  next_char = json_iss.peek();

  while( json_iss.good() && isspace(next_char) )
  {
    json_iss.get();
    next_char = json_iss.peek();
  }

  return json_iss.good();
}



} /* namespace JSON */
} /* namespace Appsrv */
