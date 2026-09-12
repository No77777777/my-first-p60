/*==============================================================================

                        ds_JSONValueType.h

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
#ifndef DS_JSON_VALUE_TYPE_H
#define DS_JSON_VALUE_TYPE_H

#include "comdef.h"
#include "ds_ASString.h"
#include "ds_ASVector.h"
#include "ds_ASMap.h"


namespace Appsrv
{
namespace Utils
{
  class ASOStream;
} /* namespace Utils */

namespace JSON
{



enum JSONType
{
  JSON_TYPE_UNSET = 0,
  JSON_TYPE_NULL  = JSON_TYPE_UNSET,
  JSON_TYPE_NUMBER,
  JSON_TYPE_STRING,
  JSON_TYPE_BOOLEAN,
  JSON_TYPE_ARRAY,
  JSON_TYPE_OBJECT,
  JSON_TYPE_INVALID
};



class JSONValueType
{
public:
  JSONValueType();
  JSONValueType(JSONType        type); // Only method to init array/object type
  JSONValueType(int32           value);
  JSONValueType(const ASString& value);
  JSONValueType(bool            value);

  // Copy constructor & operator
  JSONValueType(const JSONValueType& value);
  JSONValueType& operator=(const JSONValueType& rhs);

  ~JSONValueType();
  void clear();

  bool is_null() const;

  void     set_type(JSONType new_type);
  JSONType get_type() const;
  uint32   get_array_size() const;
  uint32   get_object_count() const;

  /* Read-only accessors */
  // Returns JSON_TYPE_INVALID if this type is not ARRAY/OBJECT
  const JSONValueType& operator[](const ASString& name) const;
  const JSONValueType& operator[](uint32 offset) const;

  // JSON value getter. Returns an invalid value if getter of invalid type is called
  int32                 get_number() const;
  const ASString&       get_string() const;
  bool                  get_bool() const;

  JSONValueType*        get_array_value(uint32 offset);
  const JSONValueType*  get_array_value(uint32 offset) const;

  JSONValueType*        get_object_value(const ASString& name);
  const JSONValueType*  get_object_value(const ASString& name) const;

  /* JSON value setter. New value will replace old value. If current value type
  /   is different from the new value, it will convert itself to the new type
  /   before storing the new value. */
  void set_value(const JSONValueType& value); // Copy

  void set_value(int32 value);                          // JSON_TYPE_NUMBER
  void set_value(const ASString& value);                // JSON_TYPE_STRING
  void set_value(const char* value, uint32 value_len);  // JSON_TYPE_STRING
  void set_value(bool value);                           // JSON_TYPE_BOOL

  void set_array_value(uint32 offset, const JSONValueType& value); // JSON_TYPE_ARRAY
  void insert_value_to_list(const JSONValueType& value);           // JSON_TYPE_ARRAY

  void set_object_value(const ASString& name, const JSONValueType& value); // JSON_TYPE_OBJECT


private:
  void copy(const JSONValueType& orig);
  JSONValueType* find_obj_value(const ASString& name) const;


  friend Utils::ASOStream& operator<<(Utils::ASOStream& json_ostream, const JSONValueType& value_obj);

private:
  // Class members
  JSONType                               value_type;

  int32                                  number_value;
  ASString                               string_value;
  bool                                   bool_value;
  ASVector<JSONValueType*>               array_value;
  Utils::ASMap<ASString, JSONValueType*> object_value;
};


Utils::ASOStream& operator<<(Utils::ASOStream& json_ostream, const JSONValueType& value_obj);


} /* namespace JSON */
} /* namespace Appsrv */


#endif /* DS_JSON_VALUE_TYPE_H */
