/*==============================================================================

                        ds_JSONDocument.h

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
01/21/16    ml     Created file/Initial version.
==============================================================================*/
#ifndef DS_JSON_DOCUMENT_H
#define DS_JSON_DOCUMENT_H

#include "comdef.h"

#include "ds_JSONValueType.h"

#include "ds_ASList.h"
#include "ds_ASStrStream.h"

class ASString;

namespace Appsrv
{
namespace JSON
{


class JSONDocument
{
public:
  JSONDocument();
  JSONDocument(const ASString& json_content);
  JSONDocument(const char* json_content, uint32 content_size);
  ~JSONDocument();

  void clear();

  JSONValueType* get_root();
  const JSONValueType* get_root() const;

  /* Read-only accessors */
  const JSONValueType& operator[](const ASString& name) const;
  const JSONValueType& operator[](uint32 offset) const;

  bool parse(const char* file_path);
  bool parse(const char* json_content, uint32 content_len);
  bool serialize(const char* file_path);
  bool serialize(const char*& content_ptr, uint32& content_size);

private:
  bool parse_helper(Utils::ASIStream& json_iss);


private:
  JSONValueType root; // Root is object or array

  Utils::ASOStringStream serialize_oss; // Used for JSON serialization on cache only
};



} /* namespace JSON */
} /* namespace Appsrv */

#endif /* DS_JSON_DOCUMENT_H */
