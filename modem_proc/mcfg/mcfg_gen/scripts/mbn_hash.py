#==============================================================================
#  $Header: //components/rel/mcfg.mpss/8.1.1/mcfg_gen/scripts/mbn_hash.py#1 $
#  $DateTime: 2021/04/06 20:45:53 $
#  $Author: pwbldsvc $
#  $Change: 29968045 $
#==============================================================================
#*
#* Copyright (c) 2016 Qualcomm Technologies, Inc.
#* All rights reserved.
#* Qualcomm Technologies, Inc. Confidential and Proprietary.
#*
#==============================================================================
#                      EDIT HISTORY FOR FILE
#
#  This section contains comments describing changes made to the module.
#  Notice that changes are listed in reverse chronological order.
#
# when       who     what, where, why
# --------   ---     ---------------------------------------------------------
# 12/18/15   kushk      Create
#
#==============================================================================


import hashlib
import sys
BLOCKSIZE = 65536
def generate_hash(filename):
    with open(filename, 'rb') as f:
        m = hashlib.sha256()
        buf = f.read(BLOCKSIZE)
        while len(buf) > 0:
            m.update(buf)
            buf = f.read(BLOCKSIZE)
        sha_bin = m.digest()
    print (sha_bin)
    
if __name__ == "__main__":
    src = sys.argv[1]
    generate_hash(src)
