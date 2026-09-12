#ifndef PS_APPEFS_H
#define PS_APPEFS_H
/*=========================================================================*/
/*!
@file
ps_AppEFS.h

@brief
Application that provides FTP server service 
 
Copyright (c) 2014-15 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/
/*===========================================================================
EDIT HISTORY FOR MODULE

Please notice that the changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/protocols/app/src/ps_AppEFS.h#2 $
$DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $
 
when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/08/14    fn     Created Module 
===========================================================================*/

/*===========================================================================

INCLUDE FILES FOR MODULE 

===========================================================================*/

#include "comdef.h"

/*===========================================================================

CLASS DEFINITION

===========================================================================*/
namespace PS_APP
{
  /**-------------------------------------------------------------------------- 
    *    @class AppEFS
    *    @brief PS FTP server EFS utility
  ---------------------------------------------------------------------------*/
  class AppEFS
  {
  public:

    /** @fn AppEFS ()
    *   @brief constructor
    */
    AppEFS();

    /** @fn  ~AppEFS()
    *   @brief Default destructor.
    */
    ~AppEFS();

    /** @fn  OpenFile
    *   @brief Opens a given file with directory 
    */
    int OpenFile( const char* filename, const char* file_dir, 
                  uint32* file_size, boolean create_file);

    /** @fn  WriteToFile
    *   @brief Writes to file
    */
    int WritetoFile( const uint8* write_content, 
                     uint32 content_len,
                     boolean append);

  protected:
    int file_handle;
  };// AppEFS
} //PS_APP

#endif // PS_APPEFS_H

