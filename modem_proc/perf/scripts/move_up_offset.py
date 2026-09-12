#!usr/bin/env python

'''
Post-link process script, to move up section offset after both dynamic
reclaiming and QShrink are done.

Copyright (c) 2015 by QUALCOMM Technologies, Inc. All Rights Reserved.
'''

import os
import os.path
import sys
import inspect

file_path = inspect.getframeinfo(inspect.currentframe()).filename
file_dir, file_name = os.path.split(file_path)

# import elfManipualtor {{{
elfManipulator_path = os.path.abspath(
  os.path.join(
    file_dir,
    'elfManipulator',
    'include'))

sys.path.insert(0, elfManipulator_path)

import elfFileClass


# supposed to be called at the end to move up section offset with
# elfManipulator API.
def main():  # {{{
  # validate input argument.
  if len(sys.argv) != 3 or not os.path.isfile(sys.argv[1]):
    sys.stderr.write("ERROR: invalid argument.")
    sys.stderr.flush()
    retcode = 1

  else:
    inpath = sys.argv[1]
    outpath = sys.argv[2]
    if inpath == outpath:
      sys.stderr.write("WARNING: incorrect output file name.")
      sys.stderr.flush()
      outpath = inpath + ".moveupoffset"

    print('\n  Moving up section offset after QShrink operation...')

    elf = elfFileClass.elfFile(inpath)
    elf.moveupElfOffsetSectionByName(".dynamicReclaim")
    elf.moveupElfOffsetSectionByName("QSR_STRING")
    elf.writeOutELF(outpath)

    retcode = 0

  return retcode
# }}}



if __name__ == '__main__':
  main()
