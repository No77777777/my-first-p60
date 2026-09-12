/*==============================================================================

                        ds_json_utils.h

GENERAL DESCRIPTION
  Wrapper API for C to parse JSON

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
01/21/16    ml     Created file/Initial version.
==============================================================================*/
#ifndef DS_JSON_UTILS_H
#define DS_JSON_UTILS_H

#include "comdef.h"
#include "ds_ASVector.h"
#include "ds_ASString.h"

class ASBuffer;

namespace Appsrv
{
namespace Utils
{
class ASIStream;
} /* namespace Utils */


namespace JSON
{


const char DS_JSON_DELIM_NAME         = ':';
const char DS_JSON_DELIM_VALUE        = ',';
const char DS_JSON_DELIM_STRING       = '"';
const char DS_JSON_DELIM_BEGIN_ARRAY  = '[';
const char DS_JSON_DELIM_END_ARRAY    = ']';
const char DS_JSON_DELIM_BEGIN_OBJECT = '{';
const char DS_JSON_DELIM_END_OBJECT   = '}';

const char DS_JSON_NULL_STRING[]      = "null";
const char DS_JSON_TRUE_STRING[]      = "true";
const char DS_JSON_FALSE_STRING[]     = "false";



bool ds_json_parse_string(Utils::ASIStream& json_iss, ASBuffer& str);
bool ds_json_parse_and_validate_raw_str(Utils::ASIStream& json_iss, ASBuffer& str, const char* cmp_value);
bool ds_json_peek_next_char(Utils::ASIStream& json_iss, char& next_char);


} /* namespace JSON */
} /* namespace Appsrv */

#endif /* DS_JSON_UTILS_H */
