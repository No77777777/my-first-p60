'''coprocessor sub-linker script generator'''

# -----------------------------------------------------------------------------
# Copyright (c) 2009-2015 by Qualcomm Technologies, Incorporated.
# All Rights Reserved.
# QUALCOMM Proprietary/GTDR
# -----------------------------------------------------------------------------

import os
import sys
import inspect

# add current directory, for import
current_dir = os.path.dirname(
  inspect.getframeinfo(
    inspect.currentframe()
  ).filename
)

if current_dir not in sys.path:
  sys.path.insert(0, current_dir)

import elf

from os.path import abspath as ap


# -----------------------------------------------------------------------------
# hooks for SCons
# -----------------------------------------------------------------------------
def exists(env):
  return env.Detect('coproc_lds_builder')


def generate(env):
  env.Append(BUILDERS={
    'CoprocLdsBuilder': env.Builder(
      action=env.GetBuilderAction(main),
    ),
  })


# -----------------------------------------------------------------------------
# builder entry point
# -----------------------------------------------------------------------------
def main(target, source, env):  # {{{
  if len(target) != 1:
    raise Exception('unhandled case of len(target) == %i' % (len(target),))

  if len(source) != 1:
    raise Exception('unhandled case of len(source) == %i' % (len(source),))

  target = ap(str(target[0]))
  source = ap(str(source[0]))

  coproc_template_lds = '''\
/*
 * The coprocessor must start at a 256K-aligned address
 */
. = ALIGN(256K);

COPROC_IMAGE (PROGBITS) :
  {
    FILL(0xdeadbeef);
    . += 0x%08x;
  } :coproc_image

. = ALIGN(4K);
'''

  coproc_elf = elf.Elf_File(source)

  if len(coproc_elf.segment_table) < 1:
    raise Exception(
      'no segments found in the coprocessor image %s' % (source,)
    )

  if len(coproc_elf.segment_table) > 1:
    raise Exception(
      'cannot handle multiple segments in the coprocessor image %s' % (
        source,
      )
    )

  coproc_lds = coproc_template_lds % (
    coproc_elf.segment_table[0].p_memsz[0][0],
  )

  with open(target, 'wb') as out_fh:
    out_fh.write(coproc_lds)

  return 0
# }}}


# -----------------------------------------------------------------------------
# standalone handler
# -----------------------------------------------------------------------------
if __name__ == '__main__':  # {{{
  sys.stderr.write('''I hope you know what you're doing.\n\n''')

  import argparse

  p = argparse.ArgumentParser(description=__doc__)

  p.add_argument('coproc_elf', help='Firmware coprocessor ELF file')
  p.add_argument('output_lds', help='Output path of the linker script')

  args = p.parse_args()

  sys.exit(main([args.output_lds], [args.coproc_elf], None))
# }}}
