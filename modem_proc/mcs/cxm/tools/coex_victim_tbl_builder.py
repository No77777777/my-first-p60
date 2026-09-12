# --------------------------------------------------------------------------- #
# Description   ::  Defines the SCons Builder for Coex Victim Table Parser
# --------------------------------------------------------------------------- #
# $Header: //components/rel/mcs.mpss/5.2/cxm/tools/coex_victim_tbl_builder.py#1 $
# --------------------------------------------------------------------------- #
# Copyright (c) 2015 Qualcomm Technologies Incorporated.
#
# All Rights Reserved. Qualcomm Confidential and Proprietary
# Export of this technology or software is regulated by the U.S. Government.
# Diversion contrary to U.S. law prohibited.
#
# All ideas, data and information contained in or disclosed by
# this document are confidential and proprietary information of
# Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
# By accepting this material the recipient agrees that this material
# and the information contained therein are held in confidence and in
# trust and will not be used, copied, reproduced in whole or in part,
# nor its contents revealed in any manner to others without the express
# written permission of Qualcomm Technologies Incorporated.
# --------------------------------------------------------------------------- #

# --------------------------------------------------------------------------- #
#                       IMPORT MODULES & DEFINITONS                           #
# --------------------------------------------------------------------------- #
import os
import types
import sys
import subprocess

# --------------------------------------------------------------------------- #
#                       GLOBAL VARIABLE DEFINITIONS                           #
# --------------------------------------------------------------------------- #

# --------------------------------------------------------------------------- #
#                       SCONS HOOKS (exist/generate)                          #
# --------------------------------------------------------------------------- #
def exists(env):
    '''Scons function to check if builder exists'''
    return env.Detect('coex_victim_tbl_builder')

def generate(env):
    '''Function to register actions with scons'''
    import SCons.Scanner
    coex_vt_act = env.GetBuilderAction(vict_tbl_builder)
    coex_vt_bld = env.Builder(action = coex_vt_act, emitter = vict_tbl_emitter,
                              suffix = '.h', src_suffix = '.xlsx')
    env.Append(BUILDERS = {'CoexVictTbl': coex_vt_bld})

def vict_tbl_emitter(target, source, env):
    vict_tbl_c = str(target[0])
    vict_tbl_h = str(target[1])
    # Dependencies
    env.Depends(vict_tbl_c, "${BUILD_ROOT}/mcs/cxm/tools/coex_victim_tbl_builder.py")
    env.Depends(vict_tbl_h, "${BUILD_ROOT}/mcs/cxm/tools/coex_victim_tbl_builder.py")

    return (target, source)

def vict_tbl_builder(target, source, env):
    '''Victim table builder function'''
    verbose = int(env.GetOption('verbose'))
    xl_src = str(source[0])
    c_target = str(target[0])
    h_target = str(target[1])
    # Information
    env.PrintInfo('CXM: Translating excel workbook to C structures')

    cmd = "python ${BUILD_ROOT}/mcs/cxm/tools/cxm_victim_tbl_translation.py "
    cmd += xl_src + " "
    cmd += c_target + " "
    cmd += h_target
    cmd = env.subst(cmd)
    
    env.PrintInfo(cmd)
    
    try:
        proc = subprocess.Popen(cmd.split(), stdout=subprocess.PIPE)
        output, err = proc.communicate()
    except CalledProcessError as e:
        env.PrintInfo(e.message)
        sys.exit(1)
    
    env.PrintInfo("Finished subprocess call")
