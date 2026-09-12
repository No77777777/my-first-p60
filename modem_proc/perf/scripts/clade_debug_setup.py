##  @file       clade_debug_setup.py.
#   @author     raholr.
#   @brief      This script is used to initialize the debug framework for CLADE
#   @version    1.0.
#===============================================================================
# Copyright (c) 2016 by Qualcomm Technologies, Incorporated.
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
# 05/27/16  raholr  1.0     Created the script
#-------------------------------------------------------------------------------

#import system modules
import sys
import re
import os
import inspect
import array

#configure PATHS and create output directoires
file_path = inspect.getframeinfo(inspect.currentframe()).filename
file_dir, file_name = os.path.split(file_path)

elfManipulator_path = os.path.abspath(
  os.path.join(
    file_dir,
    'elfManipulator',
    'include'))

clade_dict_name = "clade_dict.bin"
clade_abs_path = os.path.abspath(file_dir)
perf_path, scripts = os.path.split(clade_abs_path)

build_ms_path = os.path.join(perf_path,'..','build', 'ms')
clade_dict_path_name = os.path.join(build_ms_path, clade_dict_name)
    
sys.path.insert(0, elfManipulator_path)

#import ELF Framework modules
import elfUtils as utils
import elfConstants as const
import elfFileClass as elfFileClass
import dynRec_manip as editor


def get_seg_size_from_sh(elf, section_header):
  sect_idx = section_header.idx
  for seg_idx, sec_indices in elf.sectionToSegmentDict.items():
    if sect_idx in sec_indices:
      phdr = elf.getSegmentByIndex(seg_idx)
      return phdr.p_filesz
  print "Could not find matching segment for section index ", sect_idx
  return 0


def main():
  retcode = 0
  elf_handle = elfFileClass.elfFile(sys.argv[1])
  
  clade_roSection_name = ".clade.dict"
  clade_roSection = elf_handle.getSectionByName(clade_roSection_name)
  
  if clade_roSection == const.RC_ERROR:
    print " "*3 + "-> ! No clade dictionary section found"
    sys.exit(1)
  
  #find dictionary start and end addresses
  dict_start = clade_roSection.sh_addr
  dict_end = dict_start + clade_roSection.sh_size
  
  dict_array = array.array('I')
  
  #read dictionary from ELF file
  for i in range(dict_start,dict_end,4):
    dict_array.append(editor.readDataByAddress(elf_handle,i,4))
  
  #write dictionary to output file
  dict_file = open(clade_dict_path_name,"wb")
  dict_array.tofile(dict_file)
  dict_file.close()
  
  cladeHighSh = elf_handle.getSectionByName(".region_high_clade")
  cladeLowSh = elf_handle.getSectionByName(".region_low_clade")
  cladeCompSh = elf_handle.getSectionByName(".clade.comp")
  cladeExcLowLargeSh = elf_handle.getSectionByName(".clade.exception_low_large")
  cladeExcLowSmallSh = elf_handle.getSectionByName(".clade.exception_low_small")
  cladeExcHighSh = elf_handle.getSectionByName(".clade.exception_high")
  cladeDictSh = elf_handle.getSectionByName(".clade.dict")
  cladeMetaSh = elf_handle.getSectionByName(".clade.metadata")
  
  cladeHighSz = 0
  cladeLowSz = 0
  cladeCompSz = 0
  cladeExcLowLargeSz = 0
  cladeExcLowSmallSz = 0
  cladeExcHighSz = 0
  cladeDictSz = 0
  cladeMetaSz = 0
  
  # if cladeHighSh != const.RC_ERROR:
  #   cladeHighSz = cladeHighSh.sh_size
    
  # if cladeLowSh != const.RC_ERROR:
  #   cladeLowSz = cladeLowSh.sh_size
  
  # if cladeCompSh != const.RC_ERROR:
  #   cladeCompSz = cladeCompSh.sh_size

  # if cladeExcLowLargeSh != const.RC_ERROR:
  #   cladeExcLowLargeSz = cladeExcLowLargeSh.sh_size

  # if cladeExcLowSmallSh != const.RC_ERROR:
  #   cladeExcLowSmallSz = cladeExcLowSmallSh.sh_size

  # if cladeExcHighSh != const.RC_ERROR:
  #   cladeExcHighSz = cladeExcHighSh.sh_size

  if cladeDictSh != const.RC_ERROR:
    cladeDictSz = cladeDictSh.sh_size

  # if cladeMetaSh != const.RC_ERROR:
  #   cladeMetaSz = cladeMetaSh.sh_size

  if cladeHighSh != const.RC_ERROR and cladeLowSh != const.RC_ERROR:
    cladeHighSz = get_seg_size_from_sh(elf_handle, cladeHighSh)
    cladeLowSz = get_seg_size_from_sh(elf_handle, cladeLowSh)

  uncompressed_size = cladeHighSz + cladeLowSz
  compressed_size = get_seg_size_from_sh(elf_handle, cladeDictSh)
  print("Memory Savings Report: CLADE RO: Gross Memory Saved: %u"%(uncompressed_size-compressed_size))

  sys.exit(retcode)


if __name__ == '__main__':
  main()