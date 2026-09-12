'''
=============================================================================

                  Q S H R I N K D B F R O M E L F . P Y

Copyright (c) 2015-2020 by QUALCOMM Technologies Inc.  All Rights Reserved.

=============================================================================
DESCRIPTION
    Get QShrink 4.0 Database from provided elf file
=============================================================================

=============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order
$Header: //components/rel/perf.mpss/4.4.2/scripts/qshrink/QShrinkDBFromElf.py#1 $
$Date: 2022/10/12 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/01/17   sh      QShrink 4.0 commercial version
=============================================================================
'''

import getopt,sys
import hashlib
import uuid
import re
import string
import struct
import csv
import os, errno
import shutil

__qsrdebug__=0

# ======================== start globals ========================

const = {
  # Constants as defined in the ELF specification
  # Nowhere near complete but sufficient for this program.

  # ELF Sizes
  'Elf32_Addr'    : 4,
  'Elf32_Half'    : 2,
  'Elf32_Off'     : 4,
  'Elf32_Sword'   : 4,
  'Elf32_Word'    : 4,
  # ELF Type
  'ET_NONE'       : 0,
  'ET_REL'        : 1,
  'ET_EXEC'       : 2,
  'ET_DYN'        : 3,
  'ET_CORE'       : 4,
  'ET_LOPROC'     : 0xff00, # lower processor-specific range
  'ET_HIPROC'     : 0xffff, # higher processor-specific range
  # ELF Machine
  'EM_NONE'       : 0, # Error?
  'EM_M32'        : 1,
  'EM_SPARC'      : 2,
  'EM_386'        : 3,
  'EM_68K'        : 4,
  'EM_88K'        : 5,
  'EM_860'        : 7,
  'EM_MIPS'       : 8,
  'EM_MIPS_RS4_BE': 10,
  'EM_ARM'        : 40, # Only ones that this script handles
  # ELF Version
  'EV_NONE'       : 0, # Error
  'EV_CURRENT'    : 1, # Good
  # ELF Identifier Index
  'EI_MAG0'       : 0,
  'EI_MAG1'       : 1,
  'EI_MAG2'       : 2,
  'EI_MAG3'       : 3,
  'EI_CLASS'      : 4, # File Class (32/64-bit)
  'EI_DATA'       : 5, # Data Encoding (Little/Big-endian)
  'EI_VERSION'    : 6, # Error/Current
  'EI_PAD'        : 7, # Start of padding.  Should be 0 for 8 bytes.
  'EI_NIDENT'     : 16, # Size of ELF Identifier
  # Magic Numbers.  Identify ELF files
  'ELFMAG0'       : 0x7f,
  'ELFMAG1'       : ord("E"),
  'ELFMAG2'       : ord("L"),
  'ELFMAG3'       : ord("F"),
  # 32-bit or 64-bit
  'ELFCLASSNONE'  : 0, # Error
  'ELFCLASS32'    : 1, # 32-bit
  'ELFCLASS64'    : 2, # 64-bit
  # Endian-ness
  'ELFDATANONE'   : 0, # Error
  'ELFDATA2LSB'   : 1, # Little-endian
  'ELFDATA2MSB'   : 2, # Big-endian
  # Elf Header Offset
  'ELFH_PHOFF_OFFSET'     : 28, # e_phOff
  'ELFH_SHOFF_OFFSET'     : 32, # e_shOff
  'ELFH_PHNUM_OFFSET'     : 44, # e_phNum
  'ELFH_SHNUM_OFFSET'     : 48, # e_phNum
  'ELFH_SHSTRINDEX_OFFSET'     : 50, # e_phNum
  # Section Header Offset
  'SECTIONH_OFFSET'     : 16, #sh_off
  'SECTIONH_SIZE'       : 20, #sh_size
  'SECTIONH_LINK'       : 24, #sh_link
  # Section Header Entry
  'SHN_UNDEF'     : 0, # should ignore
  'SHN_LORESERVE' : 0xff00, # lower reserved range
  'SHN_HIRESERVE' : 0xffff, # higher reserved range
  'SHN_LOPROC'    : 0xff00, # lower processor specific range
  'SHN_HIPROC'    : 0xff1f, # higher processor specific range
  'SHN_ABS'       : 0xfff1, # if symbol is relative to, use fixed address
  'SHN_COMMON'    : 0xfff2, # if symbol is relative to,treat as common symbol
  # Section Header Type
  'SHT_NULL'      : 0, # no section data
  'SHT_PROGBITS'  : 1, # holds data defined by program
  'SHT_SYMTAB'    : 2, # holds symbol table (same as SHT_DYNSYM ?)
  'SHT_STRTAB'    : 3, # holds string table
  'SHT_RELA'      : 4, # holds relocation entries with explicit addends
  'SHT_HASH'      : 5, # holds symbol hash table
  'SHT_DYNAMIC'   : 6, # holds dynamic linking information
  'SHT_NOTE'      : 7, # holds information on the file (labels?)
  'SHT_NOBITS'    : 8, # does not go into file. sh_offset not real
  'SHT_REL'       : 9, # holds relocation entries without explicit addends
  'SHT_SHLIB'     : 10, # reserved
  'SHT_DYNSYM'    : 11, # holds a symbol table (same as SHT_SYMTAB ?)
  'SHT_LOPROC'    : 0x70000000, # lower processor specific semantics range
  'SHT_HIPROC'    : 0x7fffffff, # higher processor specific semantics range
  'SHT_LOUSER'    : 0x80000000, # lower application reserve range
  'SHT_HIUSER'    : 0xffffffff, # higher application reserve range
  # Section Header Flags
  'SHF_WRITE'     : 0x1, # section is writable during execution
  'SHF_ALLOC'     : 0x2, # occupies memory during process execution
  'SHF_EXECINSTR' : 0x4, # executable machine instructions
  'SHF_MASKPROC'  : 0xf0000000 # processor-specific flags (just a mask)
}

header = {
# global ELF header stat.  Set in initializeWithElfHeader 
  'endian'            : 0, # 1 = little endian  :: 2 = big endian
  'headerSize'        : 0, # size of the ELF Header. (always 52?)
  'programHeaderOff'  : 0, # start of program header table
  'programHeaderSize' : 0, # size of each program header entry
  'programHeaderNum'  : 0, # number of program header entries in table
  'sectionHeaderOff'  : 0, # start of section header table
  'sectionHeaderSize' : 0, # size of each section header entry
  'sectionHeaderNum'  : 0, # number of section header entries in table
  'entry'             : 0, # entry address into executable image
  'flags'             : 0,
  'sectionNameTable'  : 0  # Section Header String Table Index
}

#Qshrink 4.0 globals

class ElfShrinkStateClass:
  '''
  Used to hold most variables for ElfShrinkState.
  '''


QSR_MSG_START = 0
# Virtual address of the start of the QSR_MSG section

QSR_MSG_SIZE = 0
# Size of the QSR msg section

QSR_MSG_END = 0
# Virtual address of the end of the QSR_MSG section

QSR_MSG_OFFSET = 0
# Elf Offset of QSR_MSG section



# ======================== end globals ========================

#----------------------------------------------------------------------------
# Main function
#----------------------------------------------------------------------------
def main():
  opts, args = getopt.gnu_getopt(sys.argv[1:], "",
                                 ['OutputPath='])
  optDict = {
    '--OutputPath' : [],
    }

  for i in opts :
    optDict[i[0]].append(i[1])

  
  if len(args) == 0:
          print 'Usage:\n + QshrinkDBFromElf.py "inputElfFile"'
          print '\t --OutputPath="output Directory for logs,generated hashfile"'
          exit()

  for k in optDict :
    assert len(optDict[k]) <= 1

  buildOutputPath = ""
  if len(optDict['--OutputPath']) == 1 :
    buildOutputPath = optDict['--OutputPath'][0]

  QShrinkDBGet(elfFile=args[0],
              OutputPath=buildOutputPath)

#=========================================================================
# Function : symbolTableEntryGenerator 
#
# Arguments
#   bytes: (str) String of bytes which contains the Symbol Table
#   strTabData: (str) String of bytes which contains the String table
#
# Description
#   Creates a generator over the given bytes (should be the Symbol Table) that 
#   yields the next 16 bytes as a Symbol Table Entry.
#
# Return Values:
#  (generator:SymbolTableEntry) Iterable which has SymbolTableEntry
#          objects according to bytes
#=========================================================================
def symbolTableEntryGenerator(bytes, strTabData):
  assert len(bytes) % 16 == 0
  for i in xrange(0, len(bytes), 16) :
    yield SymbolTableEntry(bytes[i : i + 16], strTabData)

#=========================================================================
# Function : getString 
#
# Arguments
#  bytes: (str) Bytes that contain a null character.
#  begin: (int) Where to start in the given string of bytes.
#  end: (int) Where to end, non-inclusive, in the given string of bytes
#
# Description
#   Gets the first null-terminated string from bytes[begin : end].  If end is
#   not given, then it will search the bytes[begin : ].  If no null is found,
#   then it will return bytes[begin : ]
#
# Return Values:
#   All characters from begin to either end or the first null char.
#=========================================================================
def getString(bytes, begin=0, end= -1):
  index = begin
  if end == -1 : end = len(bytes)
  while (index < end and bytes[index] != '\x00'):
#         and bytes[index] != '\x01'):
    index += 1
  return bytes[begin:index]
  

def getStringGenerator(bytes, begin=0, end= -1):
  new_index = 0
  index = begin
  if end == -1 : end = len(bytes)    
  while index < end :
    while index < end and bytes[index] != '\x00' :
      index += 1
    new_index = index
    while index < end :
      if bytes[index] == '\x00' :
        index += 1
      else:
        break
    yield begin,bytes[begin:new_index]
    begin = index

#=========================================================================
# Function : convertFromElf 
#
# Arguments
#   typeOrLen: str|int - either name of Elf32 type or length in bytes
#   byteString: to be converted to an unsigned number
#   endian: should endianness be adjusted from ELF file? 
#
# Description
#   This accounts for endianness.
#
# Return Values:
#   a number based on the given format
#=========================================================================
def convertFromElf(typeOrLen, byteString, endian=True):
  endianness = ">" #big by default, like english, MSB first
  if endian == True:
    if header['endian'] == 1:
      endianness = "<" #little
    else:
      endianness = ">" #big

  size = 0
  if typeOrLen in const:
    size = const[typeOrLen]
  else:
    size = typeOrLen
  assert size == len(byteString)

  if size == 4:
    format = "L"
  elif size == 2:
    format = "H"
  elif size == 1:
    format = "B"
  elif size == 8:
    format = "Q"
  else:
    raise Exception("bad convertFromElf input: " +
                    str(typeOrLen) + " " + byteString)

  return struct.unpack(endianness + format, byteString)[0]

#=========================================================================
# Function : convertToElf 
#
# Arguments
#   typeOrLen: str|int - either name of Elf32 type or length in bytes
#   number: unsigned number to be converted
#   endian: should endianness be adjusted to match ELF file? 
#
# Description
#   This accounts for endianness.
#
# Return Values:
#   a byte string in the correct format
#=========================================================================
def convertToElf(typeOrLen, number, endian=True):

  endianness = ">" #big by default, like english, MSB first
  if endian == True:
    if header['endian'] == 1:
      endianness = "<" #little
    else:
      endianness = ">" #big

  size = 0
  if typeOrLen in const:
    size = const[typeOrLen]
  else:
    size = typeOrLen

  if size == 4:
    format = "L"
  elif size == 2:
    format = "H"
  elif size == 1:
    format = "B"
  elif size == 8:
    format = "Q"
  else:
    raise Exception("bad convertToElf input: " +
                    str(typeOrLen) + " " + str(number))

  return struct.pack(endianness + format, number)

#=========================================================================
# Function : getSectionData 
#
# Arguments
#   section: ([str|int]) Index of the section whose data you want
#
# Description
#   Reads the input ELF file and grabs the section's data.  Does not cache to 
#   avoid a Memory Error when part of a build.
#
# Return Values:
#   section data
#=========================================================================
def getSectionData(section):
  secheader = getSectionHeader(section)
  offset = secheader.data['sh_offset']
  size = secheader.data['sh_size']
  try:
    input = open(ElfShrinkState.path, 'rb')
  except IOError:
    print "Error: Could not open input ELF file, ",ElfShrinkState.path
    exit(1)
  else:
    input.seek(offset)
    ret = input.read(size)
    input.close()
  return ret


def getSectionHeader(*args):

  global ElfShrinkState
  if len(args) != 0:
    if args[0] in ElfShrinkState.sectionHeaderDict :
      return ElfShrinkState.sectionHeaderDict[args[0]]
    else:
      return SectionHeaderClass(args[0])
  return SectionHeaderClass()

class SectionHeaderClass:
  def __init__(self, *args):
    '''
    @param *args: Eats up optional parameters.  If they are given, there must
                  be two of them.  The first one is used as the path and the 
                  second the index in a call to readIndex right after the 
                  creation of the data dictionary.
                  
    Initializes data dictionary and grabs header data if index or name
    is given to the constructor.
    '''
    self.name = "NO_NAME"
    self.data = {
      'sh_name'      : 0, # index in section header string table with name
      'sh_type'      : 0, # section's contents and semantics type
      'sh_flags'     : 0,
      'sh_addr'      : 0, # address of section's first byte in image
      'sh_offset'    : 0, # offset from beginning of file to section data
      'sh_size'      : 0, # size of section's data
      'sh_link'      : 0, # section header table index link. depends on type
      'sh_info'      : 0, # depends on type
      'sh_addralign' : 0, # alignment of the entire section
      'sh_entsize'   : 0  # size of entries. only for certain types  
    }
    self.index = 0
    if len(args) != 0 :
      if (type(args[0]) is int
        and 0 <= args[0] <= header['sectionHeaderNum']):
        self.readIndex(args[0])
      elif type(args[0]) is str :
        self.getByName(args[0])
      else: assert False


  def getByName(self, name):
    for i in range(header['sectionHeaderNum'] - 1, 0, -1) :
      self.readIndex(i)
      if self.name == name : return
    print "SectionHeaderClass : getByName(", name, "): Bad Name"
    assert False

  def readIndex(self, index):
    '''
    @param path: The path to the file to read through
    @param index: The index of the section header to read.  File offset is 
                  calculated based off of this.  0 is the null header.
    Gets header data by index.  Also assigns index to object.  Not all data
    is needed and so is not read to save time.
    '''
    global ElfShrinkState
    assert 0 <= index <= header['sectionHeaderNum']
    offset = header['sectionHeaderOff'] + index * header['sectionHeaderSize']
    try :
      input = open(ElfShrinkState.path, 'rb')
    except IOError:
      print "Error: Could not open Input, elf file", ElfShrinkState.path
      exit(1)
    else:
      input.seek(offset)
      bytes = input.read(header['sectionHeaderSize'])
      self.data['sh_name'] = convertFromElf('Elf32_Word', bytes[0:4])
      self.data['sh_addr'] = convertFromElf('Elf32_Addr', bytes[12:16])
      self.data['sh_offset'] = convertFromElf('Elf32_Off', bytes[16:20])
      self.data['sh_size'] = convertFromElf('Elf32_Word', bytes[20:24])
      self.data['sh_link'] = convertFromElf( 'Elf32_Word', bytes[24:28] )
      self.data['sh_addralign'] = convertFromElf('Elf32_Word', bytes[32:36])
      input.close()
    self.index = index
    # need this check in case this header is used to find sectionNameData
    if ElfShrinkState.sectionNameData is not None :
      self.name = getString(ElfShrinkState.sectionNameData, self.data['sh_name'])

  def printData(self):
    print "SECTION HEADER:", self.index, self.name
    flags = self.data['sh_flags']
    if flags & 0x1 != 0 :         print "FLAG: SHF_WRITE"
    if flags & 0x2 != 0 :         print "FLAG: SHF_ALLOC"
    if flags & 0x4 != 0 :         print "FLAG: SHF_EXECINSTR"
    if flags & 0xf0000000 != 0 :  print "FLAG: SHF_MASKPROC"

    type = self.data['sh_type']
    if   type == const['SHT_NULL']:     print "TYPE: SHT_NULL"
    elif type == const['SHT_PROGBITS']: print "TYPE: SHT_PROGBITS"
    elif type == const['SHT_SYMTAB']:   print "TYPE: SHT_SYMTAB"
    elif type == const['SHT_STRTAB']:   print "TYPE: SHT_STRTAB"
    elif type == const['SHT_RELA']:     print "TYPE: SHT_RELA"
    elif type == const['SHT_HASH']:     print "TYPE: SHT_HASH"
    elif type == const['SHT_DYNAMIC']:  print "TYPE: SHT_DYNAMIC"
    elif type == const['SHT_NOTE']:     print "TYPE: SHT_NOTE"
    elif type == const['SHT_NOBITS']:   print "TYPE: SHT_NOBITS"
    elif type == const['SHT_REL']:      print "TYPE: SHT_REL"
    elif type == const['SHT_SHLIB']:    print "TYPE: SHT_SHLIB"
    elif type == const['SHT_DYNSYM']:   print "TYPE: SHT_DYNSYM"
    elif const['SHT_LOPROC'] <= type <= const['SHT_HIPROC']:
      print "TYPE: PROCESSOR SPECIFIC"
    elif const['SHT_LOUSER'] <= type <= const['SHT_HIUSER']:
      print "TYPE: RESERVED FOR APPLICATIONS"

    for key in self.data :
      if key != "sh_flags" and key != "sh_type" :
        print key, ":", self.data[key], hex(self.data[key])

class ProgramHeader:
  def __init__(self, *args):
    self.data = {
      'p_type'    : 0,
      'p_offset'  : 0,
      'p_vaddr'   : 0,
      'p_paddr'   : 0,
      'p_filesz'  : 0,
      'p_memsz'   : 0,
      'p_flags'   : 0,
      'p_align'   : 0,
    }
    self.index = 0
    if len(args) != 0:
      self.readIndex(args[0])

  def readIndex(self, index):
    global ElfShrinkState
    assert (index < header['programHeaderNum'])

    self.offset = header['programHeaderOff'] + index * header['programHeaderSize']
    try:
      input = open(ElfShrinkState.path, 'rb')
    except IOError:
      print "Error: Could not open Input, elf file", ElfShrinkState.path
      exit(1)
    else:
      input.seek(self.offset)
      bytes = input.read(header['programHeaderSize'])
      self.data['p_type'] = convertFromElf('Elf32_Word', bytes[0:4])
      self.data['p_offset'] = convertFromElf('Elf32_Off', bytes[4:8])
      self.data['p_vaddr'] = convertFromElf('Elf32_Addr', bytes[8:12])
      self.data['p_paddr'] = convertFromElf('Elf32_Addr', bytes[12:16])
      self.data['p_filesz'] = convertFromElf('Elf32_Word', bytes[16:20])
      self.data['p_memsz'] = convertFromElf('Elf32_Word', bytes[20:24])
      self.data['p_flags'] = convertFromElf('Elf32_Word', bytes[24:28])
      self.data['p_align'] = convertFromElf('Elf32_Word', bytes[28:32])
      input.close()
    self.index = index

class SymbolTableEntry:
  def __init__(self, bytes, stringTable):
    self.data = {
      'st_name'   : 0,
    }
    self.value = convertFromElf('Elf32_Addr', bytes[4:8])
    self.size = convertFromElf('Elf32_Word', bytes[8:12])
    self.sectionIndex = convertFromElf('Elf32_Half', bytes[14:16])
    self.name = getString(stringTable, convertFromElf('Elf32_Word', bytes[0:4]))

  def printData(self):
    print "Symbol Table Entry:", self.name
    for key in self.data :
      print key, ":", self.data[key], hex(self.data[key])
    print ""



def processHeader(elf_header):

  global header
  header_data = header # local name is faster
  e_ident = elf_header[0:16]
  num = ord(e_ident[const['EI_DATA']])
  header_data['endian'] = num
  header_data["entry"] = convertFromElf('Elf32_Addr', elf_header[24:28])
  header_data["programHeaderOff"] = convertFromElf(
                                       'Elf32_Off' , elf_header[28:32])
  header_data["sectionHeaderOff"] = convertFromElf(
                                       'Elf32_Off' , elf_header[32:36])
  header_data["headerSize"] = convertFromElf(
                                       'Elf32_Half', elf_header[40:42])
  header_data["programHeaderSize"] = convertFromElf(
                                       'Elf32_Half', elf_header[42:44])
  header_data["programHeaderNum"] = convertFromElf(
                                       'Elf32_Half', elf_header[44:46])
  header_data["sectionHeaderSize"] = convertFromElf(
                                       'Elf32_Half', elf_header[46:48])
  header_data["sectionHeaderNum"] = convertFromElf(
                                       'Elf32_Half', elf_header[48:50])
  header_data["sectionNameTable"] = convertFromElf(
                                       'Elf32_Half', elf_header[50:52])

def initializeWithElfHeader():
  global ElfShrinkState
  global QSR_MSG_START, QSR_MSG_SIZE, QSR_MSG_END, QSR_MSG_OFFSET
  global QSR_STRING_START_OUTPUT

  QSR_MSG_START = 0
  QSR_MSG_SIZE = 0
  QSR_MSG_END = 0
  QSR_MSG_OFFSET = 0

  for key in ElfShrinkState.sectionHeaderDict.keys():
    ElfShrinkState.sectionHeaderDict.pop(key) #clear all

  # set global header's entries according to ELF header
  try: 
    input = open(ElfShrinkState.path, 'rb')
  except IOError:
    print "Error, Could not open input Elf File, ", ElfShrinkState.path
    exit(1)
  else:
    elf_header = input.read(52)
    processHeader(elf_header)
    input.close()

  # get all section headers and map by index and name
  ElfShrinkState.sectionNameHeader = getSectionHeader(header['sectionNameTable'])
  ElfShrinkState.sectionNameData = getSectionData(header['sectionNameTable'])
  for i in xrange(0, header['sectionHeaderNum']) :
    ElfShrinkState.sectionHeaderDict[i] = getSectionHeader(i)
    ElfShrinkState.sectionHeaderDict[getSectionHeader(i).name] = ElfShrinkState.sectionHeaderDict[i]
  if ('QSR_4_0_MSG' in ElfShrinkState.sectionHeaderDict):
    qsrStringHeader = getSectionHeader('QSR_4_0_MSG')
    QSR_MSG_START = qsrStringHeader.data['sh_addr']
    QSR_MSG_SIZE = qsrStringHeader.data['sh_size']
    QSR_MSG_END = QSR_MSG_START + QSR_MSG_SIZE
    QSR_MSG_OFFSET = qsrStringHeader.data['sh_offset']

#=========================================================================
# Function : QShrinkDBGet
#
# Arguments
#
# Description Get DB from Qshrink 4.0 section
#
# Return Values:
#=========================================================================
def QShrinkDBGet(elfFile, OutputPath):

  global ElfShrinkState
  global QSR_MSG_START, QSR_MSG_SIZE, QSR_MSG_END, QSR_MSG_OFFSET

  ElfShrinkState = ElfShrinkStateClass()
  
  ElfShrinkState.path = elfFile
  ElfShrinkState.sectionNameHeader = None
  ElfShrinkState.sectionNameData = None
  ElfShrinkState.bytesDict = dict()
  ElfShrinkState.sectionHeaderDict = {}
  ElfShrinkState.outputPath = OutputPath
  initializeWithElfHeader()
 
  readVariables()
  ElfShrinkState.bytesDict['QSR_4_0_MSG'] = getSectionData('QSR_4_0_MSG')
  getStrings()
  CreateHashFile()

def readVariables():
 
  global ElfShrinkState
  global QSR_MSG_START, QSR_MSG_SIZE, QSR_MSG_END, QSR_MSG_OFFSET
 
  qsr4structFound = 0
  
# get the symbol table and the string table to find symbol names
  symbolTableData = getSectionData('.symtab')
# all symbol data
  stringTableData = getSectionData('.strtab')
  
  MSG_v4_re = re.compile(r'msg_v4_')
  qsr_4_re = re.compile(r'qsr_4_0_')  
  
  QSR4MtraceStartRE= re.compile(r'__QSR_4_0_MTRACE_START__')
  QSR4MtraceEndRE= re.compile(r'__QSR_4_0_MTRACE_END__')
  QSR4QTRACEStrStartRE= re.compile(r'__QSR_4_0_QTRACE_STR_START__')
  QSR4QTRACEStrEndRE= re.compile(r'__QSR_4_0_QTRACE_STR_END__')
  QSR4AlignedStartAddrRE = re.compile(r'__QSR_4_0_MSG_ALIGNED_START__')  
  QSR4UnAlignedStartAddrRE = re.compile(r'__QSR_4_0_MSG_UNALIGNED_START__')  
  QSR4MsgEndAddrRE = re.compile(r'__QSR_4_0_MSG_END__')  
  
  ElfShrinkState.QSR4AlignedStartAddr = 0
  ElfShrinkState.QSR4UnAlignedStartAddr = 0
  ElfShrinkState.QSR4MsgEndAddr =0
  ElfShrinkState.QSR4MTRACEStart = 0
  ElfShrinkState.QSR4MTRACEEnd = 0 
  ElfShrinkState.QSR4QTRACEStrStart = 0
  ElfShrinkState.QSR4QTRACEStrEnd = 0
  
  
  ElfShrinkState.errors = []
  mtracestructFound = 0
  
  # iterate through entries in the symbol table (uses a generator) to get the 
  # structure size
  for entry in symbolTableEntryGenerator(symbolTableData, stringTableData):
  # check for a QSR struct symbol    
    if QSR4MtraceStartRE.match(entry.name):
      mtracestructFound = 1
      ElfShrinkState.QSR4MTRACEStart = entry.value

    elif QSR4MtraceEndRE.match(entry.name):
      mtracestructFound = 1
      ElfShrinkState.QSR4MTRACEEnd = entry.value

    elif QSR4QTRACEStrStartRE.match(entry.name):
      ElfShrinkState.QSR4QTRACEStrStart = entry.value

    elif QSR4QTRACEStrEndRE.match(entry.name):
      ElfShrinkState.QSR4QTRACEStrEnd = entry.value
    
    elif (ElfShrinkState.QSR4AlignedStartAddr  == 0) and (QSR4AlignedStartAddrRE.match(entry.name)):      
        ElfShrinkState.QSR4AlignedStartAddr = entry.value
 
    elif (ElfShrinkState.QSR4UnAlignedStartAddr  == 0) and (QSR4UnAlignedStartAddrRE.match(entry.name)):      
        ElfShrinkState.QSR4UnAlignedStartAddr = entry.value
 
    elif (ElfShrinkState.QSR4MsgEndAddr == 0) and (QSR4MsgEndAddrRE.match(entry.name)):      
        ElfShrinkState.QSR4MsgEndAddr = entry.value
    else:
      continue

  # end for symbol table iterator
  if ((ElfShrinkState.QSR4AlignedStartAddr == 0) or (ElfShrinkState.QSR4UnAlignedStartAddr == 0)
      or (ElfShrinkState.QSR4MsgEndAddr == 0) ):
        print "Error : QShrink 4.0 database not found; Check ELF File"
        exit(1)

def processV4MtraceStructs():

  global ElfShrinkState
  global QSR_MSG_START, QSR_MSG_SIZE, QSR_MSG_END, QSR_MSG_OFFSET
  MTraceRE = re.compile('(.*?):(.*?):(.*?):(.*?):(.*?):(.*)')

  if (ElfShrinkState.QSR4MTRACEEnd == 0):
    ElfShrinkState.QSR4MTRACEEnd = ElfShrinkState.QSR4MTRACEStart;

#Read QSR_4_0_MSG section and create hash table and DIAG run time filtering tables
  strPointerOffset = ElfShrinkState.QSR4MTRACEStart  - QSR_MSG_START
  strPointerOffsetEnd = ElfShrinkState.QSR4MTRACEEnd - QSR_MSG_START

  #while msg_offset < msg_end_offset :    
  for hashval,entry in getStringGenerator(ElfShrinkState.bytesDict['QSR_4_0_MSG'], strPointerOffset,strPointerOffsetEnd):
      hashval +=  QSR_MSG_START
      string = re.sub('\x0d', r'', entry)
      string = re.sub(r"\\", r" ", string)      
      string = re.sub('\x0a', r'\\n', string)
      match = MTraceRE.match(string)
      if not match:
        print "\n ERROR: Unexpected MTRACE patter string in MSG(SSID:LINE:FILE:MSG),",string
        ElfShrinkState.errCount += 1 
        return

      tag = match.group(5)
      tag2=tag[0:32];
      tag2 = tag2.upper()
     
      hashstring = "".join([
                      str(hashval),":",
                      match.group(1),":",
                      match.group(2),":",
                      match.group(3),":",
                      match.group(4),":",
                      tag2,":",
                      match.group(6),
                      "\n"])
      ElfShrinkState.QSR4_MTRACESTRINGlist.append(hashstring)

def processV4QtraceStructs():

  global ElfShrinkState
  global QSR_MSG_START, QSR_MSG_SIZE, QSR_MSG_END, QSR_MSG_OFFSET

  if (ElfShrinkState.QSR4QTRACEStrEnd == 0):
    ElfShrinkState.QSR4QTRACEStrEnd = ElfShrinkState.QSR4QTRACEStrStart;

#Read QSR_4_0_MSG section 
  strPointerOffset = ElfShrinkState.QSR4QTRACEStrStart  - QSR_MSG_START
  strPointerOffsetEnd = ElfShrinkState.QSR4QTRACEStrEnd - QSR_MSG_START

  #while msg_offset < msg_end_offset :    
  for hashval,entry in getStringGenerator(ElfShrinkState.bytesDict['QSR_4_0_MSG'], strPointerOffset,strPointerOffsetEnd):
      hashval +=  QSR_MSG_START
      string = re.sub('\x0d', r'', entry)
      string = re.sub(r"\\", r" ", string)      
      # replace new lines (all unix (0x0a) internally) with special chars to 
      # avoid partial strings in future Master Hash load
      string = re.sub('\x0a', r'\\n', string)
      hashstring = "".join([
                      str(hashval),":",
                      string,
                      "\n"])
      ElfShrinkState.QSR4_QTRACESTRINGlist.append(hashstring)
      
def getStrings():

  global ElfShrinkState
  global QSR_MSG_START, QSR_MSG_SIZE, QSR_MSG_END, QSR_MSG_OFFSET

  ElfShrinkState.QSR4_MTRACESTRINGlist = []      
  ElfShrinkState.QSR4_QTRACESTRINGlist = []      
  ElfShrinkState.QSR4_STRINGlist = []      
      
#Read QSR_4_0_MSG section and create hash table and DIAG run time filtering tables
  msg_offset = 0 
  msg_end_offset = ElfShrinkState.QSR4MsgEndAddr - QSR_MSG_START
  msg_unaligned_offset = ElfShrinkState.QSR4UnAlignedStartAddr - QSR_MSG_START
  cur_id = 0
  cur_mask = 0
  while msg_offset < msg_end_offset :    
    hash = msg_offset
    hashval = hash>>3
    msg_mask = convertFromElf('Elf32_Word',
                      ElfShrinkState.bytesDict['QSR_4_0_MSG'][
                          msg_offset : msg_offset +
                          const['Elf32_Word']])
    msg_offset += const['Elf32_Word']
    msg_qsr_str = convertFromElf('Elf32_Word',
                      ElfShrinkState.bytesDict['QSR_4_0_MSG'][
                          msg_offset : msg_offset +
                          const['Elf32_Word']]) 
    msg_offset += const['Elf32_Word']
    # Filler data - 0xaaaaaaaa
    if ((msg_mask != 0xaaaaaaaa) or (msg_qsr_str != 0xaaaaaaaa)):
    # valid struct      
      strPointerOffset = msg_qsr_str - QSR_MSG_START
      string = getString(ElfShrinkState.bytesDict['QSR_4_0_MSG'], strPointerOffset)
      string = re.sub('\x0d', r'', string)
      string = re.sub(r"\\", r" ", string)      
      # replace new lines (all unix (0x0a) internally) with special chars to 
      # avoid partial strings in future Master Hash load
      string = re.sub('\x0a', r'\\n', string)
      hashstring = "".join([
                      str(hashval),
                      ":",
                      str(msg_mask),
                      ":",
                      string, "\n"])
      ElfShrinkState.QSR4_STRINGlist.append(hashstring)

      ssid = int(re.split(':',string)[0])      
      if hash < msg_unaligned_offset:
        if hash % 8192 == 0:
          ssid = int(re.split(':',string)[0])
          if (cur_id != ssid) or (cur_mask != msg_mask):          
            cur_id = ssid
            cur_mask = msg_mask
      else:
        cur_id = ssid
        cur_mask = msg_mask

 ##################################
 # Add mtrace string 
 ##################################
  if ElfShrinkState.QSR4MTRACEStart!=0:
   processV4MtraceStructs()

  if ElfShrinkState.QSR4QTRACEStrStart!=0:
   processV4QtraceStructs()
   
 #got all the strings
 #Get Database GUID
  m = hashlib.sha1()
  m.update("".join(ElfShrinkState.QSR4_STRINGlist))
  if len(ElfShrinkState.QSR4_MTRACESTRINGlist) !=0:
    m.update("".join(ElfShrinkState.QSR4_MTRACESTRINGlist))
  if len(ElfShrinkState.QSR4_QTRACESTRINGlist) !=0:
    m.update("".join(ElfShrinkState.QSR4_QTRACESTRINGlist))

  x = m.digest()[-16:]
  ElfShrinkState.dBaseGUID = uuid.UUID(bytes=x)

  ElfShrinkState.bytesDict.clear() #release memory  
  

def CreateHashFile():

  global ElfShrinkState    
 #write Hash File
  Hashfile_path = ElfShrinkState.outputPath  
  Hashfile_temp = "".join(["msg_hash_",                   
                           str(ElfShrinkState.dBaseGUID),
                           ".qsr4"])  
  Hashfile_name = os.path.join(Hashfile_path,Hashfile_temp)  
  contentStr = [ \
    " ".join(["##################################################################","\n"]),
    " ".join(["# Qshrink 4.0 Hash Database File ","\n"]),
    " ".join(["##################################################################","\n"]),
    " ".join(["# GENERAL DESCRIPTION            ","\n"]),
    " ".join(["#                                ","\n"]),
    " ".join(["# Copyright (c) 2015 - 2020 by Qualcomm Technologies Inc. All Rights Reserved.","\n"]),
    " ".join(["# Export of this technology or software is regulated by the U.S. Government. ","\n"]),  
    " ".join(["# Diversion contrary to U.S. law prohibited. ","\n"]),  
    " ".join(["#                                ","\n"]),
    " ".join(["# Hash File:", Hashfile_name,"\n"]),
    " ".join(["# Hash File Format : <hash>:<ss_mask>:<ssid>:<line>:<file>:<string>    ","\n"]),
    " ".join(["# Qtrace Format : <hash>:<line>:<level>:<client>:<file>:<tag>:<string>    ","\n"]),
    " ".join(["#                                ","\n"]),
    " ".join(["##################################################################","\n"]),
    " ".join(["<GUID>",str(ElfShrinkState.dBaseGUID),"<\GUID>","\n"]),
    " ".join(["<Version>","00.10.00","<\Version>","\n"]),
    " ".join(["<Content>","\n"]),
    "".join(ElfShrinkState.QSR4_STRINGlist),
    " ".join(["<\Content>","\n"])    
  ]

  if len(ElfShrinkState.QSR4_MTRACESTRINGlist) !=0:    
    contentStr = ["".join(contentStr),
                         " ".join(["<MtraceContent>","\n"]),
                         "".join(ElfShrinkState.QSR4_MTRACESTRINGlist),
                         " ".join(["<\MtraceContent>","\n"])    
                       ]

  if len(ElfShrinkState.QSR4_QTRACESTRINGlist) !=0:    
    contentStr = ["".join(contentStr),
                         " ".join(["<QtraceStrContent>","\n"]),
                         "".join(ElfShrinkState.QSR4_QTRACESTRINGlist),
                         " ".join(["<\QtraceStrContent>","\n"])    
                       ]
                       
  try:
    Hashfile = open(Hashfile_name, 'w')
  except IOError:
    print "Error: Could not open Hashfile, ",Hashfile_name
    exit(1)
  else:
    for strn in contentStr:
      Hashfile.write(strn)
    Hashfile.close()

if __name__ == '__main__':
    main()

