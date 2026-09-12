/** vi: tw=128 ts=3 sw=3 et
@file pd_dump_fs_shell.h
@brief This file contains Abstraction layer for writing user process dumps into file systems.
*/
/*=============================================================================
NOTE: The @brief description above does not appear in the PDF.
The tms_mainpage.dox file contains the group/module descriptions that
are displayed in the output PDF generated using Doxygen and LaTeX. To
edit or update any of the group/module text in the PDF, edit the
tms_mainpage.dox file or contact Tech Pubs.
===============================================================================*/
/*=============================================================================
Copyright (c) 2018-2019,2022 by Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Confidential and Proprietary
=============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/pd_mon/src/pd_dump_fs_al.h#3 $
$DateTime: 2022/06/22 22:40:36 $
$Change: 38006310 $
$Author: pwbldsvc $
===============================================================================*/

int pd_dump_fs_al_open_file_for_read
(
  const char * filename,
  int *file_desc
);

int pd_dump_fs_al_open_file_for_create_write
(
  const char * filename,
  int *file_desc
);

unsigned int pd_dump_fs_al_write
(
  unsigned int fd,
  const byte * buf,
  unsigned int nbyte
);

unsigned int pd_dump_fs_al_read
(
  unsigned int fd,
  void * buf,
  unsigned int nbyte
);

unsigned int pd_dump_fs_al_close
(
  unsigned int fd
);

/** pd_dump_fs_al_delete
 *
 * @param filename : pd dump file name
 *
 * @return 1 in case of successful operation else
 *         -1.
 */
int pd_dump_fs_al_delete
(
  const char * filename
);
