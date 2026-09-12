#===============================================================================
# Device Config Linker Script Builder, LLVM
#
# GENERAL DESCRIPTION
#    Contains _init_ definition
#
# Copyright (c) 2014-16, 2018 by Qualcomm Technologies Inc.
# All Rights Reserved.
# QUALCOMM Proprietary/GTDR
#===============================================================================
class DevcfgInput:
    def __init__(self, blob, file, fake, includes):
        self.blob = blob
        self.file = file
        self.fake = fake
        self.includes = includes
