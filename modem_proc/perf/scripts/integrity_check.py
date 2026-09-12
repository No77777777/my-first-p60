##  @file       integrity_check.py.
#   @author     raholr.
#   @brief      This script calculates the checksums for the RO_Compressed Region.
#   @version    1.0.
#===============================================================================
# Copyright (c) 2014 by Qualcomm Technologies, Incorporated.
# All Rights Reserved.
# Qualcomm Technologies Proprietary/GTDR
#
#-------------------------------------------------------------------------------
#                      EDIT HISTORY FOR FILE
#
#  This section contains comments describing changes made to the module.
#  Notice that changes are listed in reverse chronological order.
#
# when      who     ver     what, where, why
# --------  ------  ----    ----------------------------------------------------
# 07/26/16  raholr  2.1     Remove FW crashdump section from elf if not overlaid
# 06/15/16  raholr  2.0     Adding Support for CLADE, update symbols in
#                           crash_hook under shutdown, multi-region support
#                           for integrity check
# 06/30/15  raholr  1.0     Created the script
# 
#-------------------------------------------------------------------------------

import sys
import re
import os
import inspect
import struct

file_path = inspect.getframeinfo(inspect.currentframe()).filename
file_dir, file_name = os.path.split(file_path)

sz_bit = 14
block_size = 1 << sz_bit
max_checksum_blocks = 1 << (sz_bit -2)
max_size = max_checksum_blocks << sz_bit
checksum_size = 4

# symbols pertaining to integritycheck
symbol_list = [
  "perf_integritycheck_compress_section_begin", 
  "perf_integritycheck_compress_section_size", 
  "perf_integritycheck_checksum_buf"]

#sections to run integrity_check over
integrity_chunks = [".clade.comp"]

fw_overlay_sh_name = "overlay_mem_dump"
# import elfManipualtor {{{
elfManipulator_path = os.path.abspath(
  os.path.join(
    file_dir,
    'elfManipulator',
    'include'))

sys.path.insert(0, elfManipulator_path)

import elfUtils as utils
import elfConstants as const
import elfFileClass as elfFileClass
import dynRec_manip as editor

##
# @brief    calculate xor checksums for specified address range and store in specified location
# @param    elf          The elfFileClass object to read from
# @param    start      The starting point to read from
# @param    end        end point
# @pre      returns pointer to next location in checksum array
def checksum_xor(elf_handle, start, end, write_ptr):
  count = 0
  temp = editor.readDataByAddress(elf_handle, start, 4)
  print temp
  while start < end:
    step_end = start + block_size
    if step_end > end:
      step_end = end
    block_xor = 0
    while start < step_end:
      temp = editor.readDataByAddress(elf_handle, start, 4)
      block_xor = block_xor ^ temp
      start = start + 4
    editor.setDataByAddress(elf_handle, write_ptr, 4, block_xor)
    write_ptr = write_ptr + 4
    count = count + 1
  return write_ptr
  

##
# @brief    calculate fletcher's checksums for specified address range and store in specified location
# @param    elf          The elfFileClass object to read from
# @param    start      The starting point to read from
# @param    end        end point
# @pre      returns pointer to next location in checksum array  
def checksum_fletcher(elf_handle, contents, start, end, write_ptr):
  section_start = start
  while start < end:
    step_end = start + block_size
    if step_end > end:
      step_end = end
    sa = 0
    sb = 0
    while start < step_end:
      temp = contents[start - section_start : start - section_start + 4]
      temp = struct.unpack_from("<L", temp, 0)[0] #based on convertFromELF in elfUtils.py
      sa = sa + temp
      sb = sb + sa
      start = start + 4
    sf = (sb ^ sa) % 4294967296
    editor.setDataByAddress(elf_handle, write_ptr, 4, sf)
    write_ptr = write_ptr + 4
  return write_ptr  
  


def main(argv): 
  Dict = {}
  retcode = 0
  
  start_va_rx = []
  end_va_rx = []
  elf_handle = elfFileClass.elfFile(argv[1])
  integrity_check_symbols = elf_handle.getSymbolByNameList(symbol_list)

  for j in integrity_chunks:
    sh = elf_handle.getSectionByName(j)
    if sh == const.RC_ERROR:
      print "Skipping Integrity Check for section that was not found - %s" %(j)
      retcode = 1
      return retcode

  for i in xrange(len(symbol_list)):
    symbolname = symbol_list[i]
    symbol = integrity_check_symbols[i]
    if symbol == const.RC_ERROR:
      print "Error: Integritycheck failed/disabled. Symbol missing: %s"%(symbolname)
      retcode = 1


  begin_arr_addr = integrity_check_symbols[0].st_value
  sz_arr_addr = integrity_check_symbols[1].st_value
  start_ptr = integrity_check_symbols[2].st_value
  total_size = 0
  count = 0

  for j in integrity_chunks:
    sh = elf_handle.getSectionByName(j)
    if sh == const.RC_ERROR:
      print "   Warning: Skipping Integrity Check for section that was not found - %s" %(j)
      continue
    start_va_rx.append(sh.sh_addr)
    end_va_rx.append(sh.sh_addr + sh.sh_size)
    total_size += sh.sh_size
    
    if total_size > max_size:
      print "   Warning: Integrity Check max size reached. Skipping section - %s" %(j)
      retcode = 2
    else:
      start_ptr = checksum_fletcher(elf_handle, sh.contents, sh.sh_addr, sh.sh_addr + sh.sh_size, start_ptr)
      editor.setDataByAddress(elf_handle, begin_arr_addr, 4, sh.sh_addr)
      editor.setDataByAddress(elf_handle, sz_arr_addr, 4, sh.sh_size)
      begin_arr_addr += 4
      sz_arr_addr += 4
      count += 1
  
##
# If the FW crashdump section is not overlaid, then remove the section
# here. Workaround for qurt image builder and CLADE compatibility. This 
# removes section if it is of type PROG_BITS

  fw_overlay_sh = elf_handle.getSectionByName(fw_overlay_sh_name)
  if fw_overlay_sh != const.RC_ERROR:
    Dict["start_va_map_fw_dump_overlay"] = fw_overlay_sh.sh_addr
    Dict["end_va_map_fw_dump_overlay"] = fw_overlay_sh.sh_addr + (((max(end_va_rx)/4096)*4096) - (((min(start_va_rx)+4095)/4096)*4096))
    elf_handle.removeSectionByName(fw_overlay_sh_name)
    
  Dict["perf_integritycheck_num_sections"] = count
  Dict["start_va_enable_fw_dump_overlay"] = min(start_va_rx)
  Dict["end_va_enable_fw_dump_overlay"] = ((max(end_va_rx) + 4095)/4096) * 4096

  elf_handle.updateSymbolValuesByDict(Dict)
  elf_handle.writeOutELF(sys.argv[2])
  return retcode
  
if __name__ == '__main__':
  sys.exit(main(sys.argv))

