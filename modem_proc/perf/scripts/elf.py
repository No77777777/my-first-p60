#!/usr/bin/env python

'''
Common ELF utilities used by the performance team
'''

import sys
import mmap
import struct
import fnmatch
import datetime

from os.path import abspath as ap

__author__ = ('alexh',)
__version__ = '$Revision: #1 $'


class Generic(object):  # {{{
  def __init__(self, **kw):  # {{{
    for k, v in kw.items():
      setattr(self, k, v)
  # }}}

  def __getitem__(self, k):  # {{{
    return getattr(self, k)
  # }}}

  def __setitem__(self, k, v):  # {{{
    return setattr(self, k, v)
  # }}}
# }}}


class Helpers(object):  # {{{
  @staticmethod
  def parse_mmap(mmap, offset, fmt):  # {{{
    orig_pos = mmap.tell()
    mmap.seek(offset)

    parsed = Generic()

    for name, struct_fmt, display in fmt:
      raw = mmap.read(struct.calcsize(struct_fmt))
      cooked = struct.unpack(struct_fmt, raw)

      if type(display) == type(lambda x: x):
        parsed[name] = (cooked, [display(i) for i in cooked])

      elif isinstance(display, str):
        parsed[name] = (cooked, display % cooked)

      else:
        raise Exception('could not get a proper display for %r' % (cooked,))

    mmap.seek(orig_pos)

    return parsed
  # }}}

  @staticmethod
  def parse_bytes(raw, fmt):  # {{{
    i = 0

    parsed = Generic()

    for name, struct_fmt, display in fmt:
      size = struct.calcsize(struct_fmt)
      cooked = struct.unpack(struct_fmt, raw[i:i + size])
      i += size

      if type(display) == type(lambda x: x):
        parsed[name] = (cooked, [display(j) for j in cooked])

      elif isinstance(display, str):
        parsed[name] = (cooked, display % cooked)

      else:
        raise Exception('could not get a proper display for %r' % (cooked,))

    return parsed
  # }}}

  @staticmethod
  def flip_dict(in_dict):  # {{{
    return dict([
      (v, k)
      for (k, v) in in_dict.items()
      if not fnmatch.fnmatch(k, '__*__')
    ])
  # }}}
# }}}


class Elf_Const(object):  # {{{
  class e_ident:
    EI_NIDENT = 16

    ELFMAG0 = 0x7f
    ELFMAG1 = ord('E')
    ELFMAG2 = ord('L')
    ELFMAG3 = ord('F')

    ELFCLASSNONE = 0
    ELFCLASS32 = 1
    ELFCLASS64 = 2

    ELFDATANONE = 0
    ELFDATA2LSB = 1
    ELFDATA2MSB = 2

    EV_NONE = 0
    EV_CURRENT = 1

  class e_type:
    ET_NONE = 0
    ET_REL = 1
    ET_EXEC = 2
    ET_DYN = 3
    ET_CORE = 4
    ET_LOPROC = 0xff00
    ET_HIPROC = 0xffff

  class e_machine:
    EM_NONE = 0
    EM_M32 = 1
    EM_SPARC = 2
    EM_386 = 3
    EM_68K = 4
    EM_88K = 5
    EM_860 = 7
    EM_MIPS = 8
    EM_HEXAGON = 164

  Elf32_Ehdr = (
    ('e_ident', '<%iB' % (e_ident.EI_NIDENT,), lambda x: chr(x)),
    ('e_type', '<H', lambda x: Elf_Const.reverse('e_type', x)),
    ('e_machine', '<H', lambda x: Elf_Const.reverse('e_machine', x)),
    ('e_version', '<I', '0x%x'),
    ('e_entry', '<I', '0x%08x'),
    ('e_phoff', '<I', '0x%x'),
    ('e_shoff', '<I', '0x%x'),
    ('e_flags', '<I', '0x%x'),
    ('e_ehsize', '<H', '0x%x'),
    ('e_phentsize', '<H', '0x%x'),
    ('e_phnum', '<H', '0x%x'),
    ('e_shentsize', '<H', '0x%x'),
    ('e_shnum', '<H', '0x%x'),
    ('e_shstrndx', '<H', '0x%x'),
  )

  Elf_Eident = (
    ('EI_MAG0', '<B', '0x%x'),
    ('EI_MAG1', '<B', '%s'),
    ('EI_MAG2', '<B', '%s'),
    ('EI_MAG3', '<B', '%s'),
    ('EI_CLASS', '<B', '0x%x'),
    ('EI_DATA', '<B', '0x%x'),
    ('EI_VERSION', '<B', '0x%x'),
    # ignore EI_PAD from here to the end of e_ident
  )

  class special_shndx:
    SHN_UNDEF = 0
    SHN_LORESERVE = 0xff00
    SHN_LOPROC = 0xff00
    SHN_HIPROC = 0xff1f
    SHN_ABS = 0xfff1
    SHN_COMMON = 0xfff2
    SHN_HIRESERVE = 0xffff

  Elf32_Shdr = (
    ('sh_name', '<I', '0x%x'),
    ('sh_type', '<I', '0x%x'),
    ('sh_flags', '<I', '0x%x'),
    ('sh_addr', '<I', '0x%x'),
    ('sh_offset', '<I', '0x%x'),
    ('sh_size', '<I', '0x%x'),
    ('sh_link', '<I', '0x%x'),
    ('sh_info', '<I', '0x%x'),
    ('sh_addralign', '<I', '0x%x'),
    ('sh_entsize', '<I', '0x%x'),
  )

  class sh_type:
    SHT_NULL = 0
    SHT_PROGBITS = 1
    SHT_SYMTAB = 2
    SHT_STRTAB = 3
    SHT_RELA = 4
    SHT_HASH = 5
    SHT_DYNAMIC = 6
    SHT_NOTE = 7
    SHT_NOBITS = 8
    SHT_REL = 9
    SHT_SHLIB = 10
    SHT_DYNSYM = 11
    SHT_LOPROC = 0x70000000
    SHT_HIPROC = 0x7fffffff
    SHT_LOUSER = 0x80000000
    SHT_HIUSER = 0xffffffff

  class sh_flags:
    SHF_WRITE = 0x1
    SHF_ALLOC = 0x2
    SHF_EXECINSTR = 0x4
    SHF_MASKPROC = 0xf0000000

  Elf32_Sym = (
    ('st_name', '<I', '0x%x'),
    ('st_value', '<I', '0x%x'),
    ('st_size', '<I', '0x%x'),
    ('st_info', '<B', '%s'),
    ('st_other', '<B', '%s'),
    ('st_shndx', '<H', '0x%x'),
  )

  Elf32_Phdr = (
    ('p_type', '<I', '0x%x'),
    ('p_offset', '<I', '0x%x'),
    ('p_vaddr', '<I', '0x%x'),
    ('p_paddr', '<I', '0x%x'),
    ('p_filesz', '<I', '0x%x'),
    ('p_memsz', '<I', '0x%x'),
    ('p_flags', '<I', '0x%x'),
    ('p_align', '<I', '0x%x'),
  )

  class ph_type:
    PT_NULL = 0
    PT_LOAD = 1
    PT_DYNAMIC = 2
    PT_INTERP = 3
    PT_NOTE = 4
    PT_SHLIB = 5
    PT_PHDR = 6
    PT_LOPROC = 0x70000000
    PT_HIPROC = 0x7fffffff

  class ph_flags:
    PF_X = 0x1
    PF_W = 0x2
    PF_R = 0x4

  @classmethod
  def reverse(self, attr, value):  # {{{
    return Helpers.flip_dict(getattr(self, attr).__dict__)[value]
  # }}}
# }}}


class Elf_File(object):  # {{{
  extra_header_validations = (
    ('e_type', Elf_Const.e_type.ET_EXEC),
    ('e_machine', Elf_Const.e_machine.EM_HEXAGON),
  )

  def __init__(self, path, verbose=False):  # {{{
    self.path = ap(path)

    self._handle = open(self.path, 'rb')
    self.mmap = mmap.mmap(self._handle.fileno(), 0, access=mmap.ACCESS_READ)

    self.header = Helpers.parse_mmap(self.mmap, 0, Elf_Const.Elf32_Ehdr)
    self.e_ident = Helpers.parse_mmap(self.mmap, 0, Elf_Const.Elf_Eident)

    if not self.valid_e_ident():
      raise Exception('Invalid/unsupported ELF file %s' % (self.path,))

    self.validate_extra_header_fields()

    if self.header.e_shoff == 0:
      raise Exception(
        'No section header table for ELF file %s' % (self.path,)
      )

    # section and symbol tables {{{
    self.section_table = self.decode_section_table()

    if self.header.e_shstrndx[0][0] != Elf_Const.special_shndx.SHN_UNDEF:
      if self.header.e_shstrndx[0][0] >= len(self.section_table):
        raise Exception('e_shstrndx is greater than the number of sections')

      self.section_names = self.decode_section_names()

    else:
      self.section_names = [len(self.section_table) * '']

    if '.symtab' in self.section_names:
      if verbose:
        start = datetime.datetime.now()
        print('Populating symbol table, this will take some time')

      self.symbol_table = self.decode_symbol_table()
      self.symbol_names = self.decode_symbol_names()

      if verbose:
        delta = datetime.datetime.now() - start
        delta_str = '%s' % (delta,)

        if '.' in delta_str:
          delta_str = delta_str[0:delta_str.index('.') + 3]

        print('Parsed symbol table; elapsed time: %s' % (delta_str,))

    else:
      self.symbol_table = None
      self.symbol_names = []
    # }}}

    # segment table {{{
    self.segment_table = self.decode_segment_table()
    # }}}
  # }}}

  def __del__(self):  # {{{
    self.mmap.close()
    self._handle.close()
  # }}}

  def valid_e_ident(self):  # {{{
    for i in range(4):
      if (
        getattr(self.e_ident, 'EI_MAG%i' % (i,))[0][0]
        != getattr(Elf_Const.e_ident, 'ELFMAG%i' % (i,))
      ):
        return False

    if self.e_ident.EI_CLASS[0][0] != Elf_Const.e_ident.ELFCLASS32:
      # only support 32-bit objects in our implemetation
      return False

    if self.e_ident.EI_DATA[0][0] != Elf_Const.e_ident.ELFDATA2LSB:
      # hexagon architecture uses LSB, only support that for now
      return False

    if self.e_ident.EI_VERSION[0][0] != Elf_Const.e_ident.EV_CURRENT:
      return False

    return True
  # }}}

  def validate_extra_header_fields(self):  # {{{
    # validations we don't have to error out on
    for k, expected in self.extra_header_validations:
      actual = getattr(self.header, k)[0][0]

      if actual != expected:
        e = Elf_Const.reverse(k, expected)
        a = Elf_Const.reverse(k, actual)

        sys.stderr.write(
          'Warning: expected %(k)s of %(e)s; actual %(k)s: %(a)s\n' % locals()
        )
  # }}}

  def decode_segment_table(self):  # {{{
    return self.decode_table(
      Elf_Const.Elf32_Phdr,
      self.header.e_phoff[0][0],
      self.header.e_phnum[0][0],
    )
  # }}}

  def decode_section_table(self):  # {{{
    return self.decode_table(
      Elf_Const.Elf32_Shdr,
      self.header.e_shoff[0][0],
      self.header.e_shnum[0][0],
    )
  # }}}

  def decode_table(self, fmt, offset, n_entries):  # {{{
    entries = []
    entry_size = sum([struct.calcsize(i[1]) for i in fmt])

    for i in range(offset, offset + n_entries * entry_size, entry_size):
      entries.append(Helpers.parse_mmap(self.mmap, i, fmt))

    return entries
  # }}}

  def decode_section_names(self):  # {{{
    return self.decode_names(
      'section_table',
      self.section_table[self.header.e_shstrndx[0][0]].sh_offset[0][0],
      'sh_name',
    )
  # }}}

  def decode_names(self, table, initial_offset, offset_attr):  # {{{
    entries = []

    for item in getattr(self, table):
      start = initial_offset + getattr(item, offset_attr)[0][0]
      end = self.mmap.find('\0', start)

      if end < 0:
        raise Exception(
          'Could not find null terminator from offset 0x%x' % (start,)
        )

      entries.append(self.mmap[start:end])

    return entries
  # }}}

  def decode_symbol_table(self):  # {{{
    symtab_section = self.section_table[self.section_names.index('.symtab')]

    return self.decode_table(
      Elf_Const.Elf32_Sym,
      symtab_section.sh_offset[0][0],
      symtab_section.sh_size[0][0] / symtab_section.sh_entsize[0][0]
    )
  # }}}

  def decode_symbol_names(self):  # {{{
    return self.decode_names(
      'symbol_table',
      self.section_table[self.section_names.index('.strtab')].sh_offset[0][0],
      'st_name',
    )
  # }}}
# }}}


if __name__ == '__main__':
  raise Exception('this file is not meant to be run standalone')
