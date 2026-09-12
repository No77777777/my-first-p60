###############################################################################
# Author: Juan Gutierrez                                                      #
# Date: 01/26/2015                                                            #
# Version v1.0                                                                #
#                                                                             # 
# Description: This script will convert the header file "ipa_uc.h"           #
#  into a binary file that will be side loaded via EFS. The header file is a  #
#  C-style header file. We will pattern match against the hex words, careful  #
#  to ignore hex words inside comments, and we will create a binary output    # 
#  file that matches the byte array 'including' this header file would        #
#  produce. The header file consists of two segments. IRAM and DRAM. Each of  #
#  which is prefixed by one word containing the start address of that segment #
#  followed by a word indicating the length of the segment in 32bit words.    #
#                                                                             #
# Expected Format:                                                            #
#  <COMMENT><IRAM_START_ADDRESS><COMMENT><IRAM_SIZE_IN_32BITWORDS>            #
#  <IRAM_DATA...><...IRAM_DATA>                                               #
#  <COMMENT_WITH_END_ADDRESS_IRAM>                                            #
#  <COMMENT><DRAM_START_ADDRESS><COMMENT><DRAM_SIZE_IN_32BITWORDS>            #
#  <DRAM_DATA...><...DRAM_DATA>                                               #
#  <COMMENT_WITH_END_ADDRESS_DRAM>                                            #
###############################################################################

#!/usr/bin/python

import sys
import os.path
import re
import struct
from binascii import unhexlify

# Func PrintUsage
# Print the usage of this parser 
def PrintUsage():
   "This prints how to use this script"
   print 'Usage: c2EFSBin.py <PATH_TO_ipa_uc.H>'
   return

# Func swap32
# Swap the endiness of the 32bit word   
def swap32(i):
    return struct.unpack("<i", struct.pack(">i", i))[0]   

# Check for num of args
if (len(sys.argv) != 2):
   PrintUsage()
   sys.exit()
   
# Check that the filepath provided exists 
if (os.path.exists(sys.argv[1]) == False):
   print 'Invalid path: ', str(sys.argv[1])
   PrintUsage()
   sys.exit()
   
ipa_uc_h_path = str(sys.argv[1])
ipa_uc_h_dir = os.path.dirname(ipa_uc_h_path)

resultOfConversion = False
 
print '\n------------------------ELF -> EFS conversion------------------------' 
print 'Path to ipa_uc.h file: ',  str(sys.argv[1])

print 'Opening input file for reading'
ipa_uc_h_file = open(ipa_uc_h_path, 'r')

print 'Split input file along the commas'
print 'Split input file along the commas'
img_list = ipa_uc_h_file.read().split(',')

print 'Searching for hex words'
word_array = list() 
# Should match against the first hex word found on a line (ignoring the second word if it appears in a comment)
for str in img_list:
   word_group = re.search('0[xX][0-9a-fA-F]+', str, re.L)
   word_array.append(word_group.group())
print 'Creating the EFS output file'
efs_temp = open(ipa_uc_h_dir + '/ipa_hwp_efs.temp', 'wb')
efs_temp.write(unhexlify(''.join(format(i[2:], '>02s') for i in word_array)))
# Close File
efs_temp.close()

#swap endianess
efs_temp = open(ipa_uc_h_dir + '/ipa_hwp_efs.temp', 'rb')
ipa_hwp_efs_swapped = open(ipa_uc_h_dir + '/ipa_hwp_efs', 'wb')
try:
   word = efs_temp.read(4)
   while word != "":
      #Note that unpack always returns a tuple, so struct.unpack('i', word)[0] gives the integer value that you are after.
      ipa_hwp_efs_swapped.write(struct.pack('i', swap32(int(struct.unpack('i', word)[0]))))
      word = efs_temp.read(4)
finally:
   ipa_hwp_efs_swapped.close()

# Close File
efs_temp.close()   

#Remove temp file   
if os.path.exists(ipa_uc_h_dir + '/ipa_hwp_efs.temp'):
   os.remove(ipa_uc_h_dir + '/ipa_hwp_efs.temp')   

print 'Writing to output file finished'  
resultOfConversion = True

# Close File
ipa_uc_h_file.close()

# Print Results
if (resultOfConversion == False):
   print 'Conversion FAILED'
else:
   print 'Conversion Completed.'

print '------------------------ELF -> EFS conversion------------------------\n'
sys.exit()