from SCons.Script import *
import re
import subprocess
import signal
from threading import Timer

#------------------------------------------------------------------------------
# Hooks for Scons
#------------------------------------------------------------------------------
def exists(env):
   return env.Detect('qurt_builders')

def generate(env):
   # load builder into enviroment
   cust_config_obj_generate(env)
   cust_config_src_generate(env)
   obj_dump_generate(env)
   sim_generate(env)
   reloc_generate(env)
   qurt_image_generate(env)
   build_param_generate(env)
   config_vars_generate(env)
   const_autogen_generate(env)
   ppend_cust_config_generate(env)
   traptab_generate(env)
   return None

#------------------------------------------------------------------------------
# Customer Configuration definition
#------------------------------------------------------------------------------
def cust_config_obj_generate(env):
   CustConfigObjBuilder = Builder(action = 'python ${Q6_RTOS_ROOT}/scripts/qurt_config.py update ${USE_LLVM_OPT} -T ${Q6_TOOLS_ROOT} -o $TARGET $SOURCE',
                       emitter = cust_config_emitter, 
                       suffix = '.o', 
                       src_suffix = '.xml')
   env['BUILDERS']['CustConfigObj'] = CustConfigObjBuilder

def cust_config_src_generate(env):
   CustConfigSrcBuilder = Builder(action = 'python ${Q6_RTOS_ROOT}/scripts/qurt_config.py update --src_only -T ${Q6_TOOLS_ROOT} -o $TARGET $SOURCE',
                       emitter = cust_config_emitter,
                       suffix = '.c',
                       src_suffix = '.xml')
   env['BUILDERS']['CustConfigSrc'] = CustConfigSrcBuilder

def cust_config_emitter(target, source, env):
   # Add dependencies on scripts
   env.Depends(target, "${INSTALL_DIR}/scripts/Input/cust_config.c")
   env.Depends(target, "${INSTALL_DIR}/scripts/qurt_config.py")
   # clean side effects
   env.Clean(target, "build/qurt_config.c")
   return (target, source)

#------------------------------------------------------------------------------
# Object Dump
#------------------------------------------------------------------------------
def obj_dump_generate(env):
   ObjDumpBuilder = Builder(action = '${OBJDUMP} ${OBJDUMPOPT} $SOURCE > $TARGET', 
                       suffix = '.dump', 
                       src_suffix = '.pbn')
   env['BUILDERS']['ObjectDump'] = ObjDumpBuilder

#------------------------------------------------------------------------------
# Simulator
#------------------------------------------------------------------------------
def sim_generate(env):
   SimBuilder = Builder(action = "${SIM} -S $TARGET --simulated_returnval $SOURCE ${SIM_RTOS} ${SYMFILE} ${SIM_COSIM}", 
                       suffix = '.txt')
   env['BUILDERS']['Simulator'] = SimBuilder

#------------------------------------------------------------------------------
# Relocation
#------------------------------------------------------------------------------
def reloc_generate(env):
   RelocBuilder = Builder(action = "${OBJCOPY} --change-start +${RELOC_ADDR} " +
                                 "--change-section-lma .start+${RELOC_ADDR} " +
                                 "--change-section-lma .init+${RELOC_ADDR} " +
                                 "--change-section-lma .text+${RELOC_ADDR} " +
                                 "--change-section-lma .fini+${RELOC_ADDR} " +
                                 "--change-section-lma .rodata+${RELOC_ADDR} " +
                                 "--change-section-lma .data+${RELOC_ADDR} " +
                                 "--change-section-lma .eh_frame+${RELOC_ADDR} " +
                                 "--change-section-lma .ctors+${RELOC_ADDR} " +
                                 "--change-section-lma .dtors+${RELOC_ADDR} " +
                                 "--change-section-lma .bss+${RELOC_ADDR} " +
                                 "--change-section-lma .sdata+${RELOC_ADDR} " +
                                 "--change-section-lma .sbss+${RELOC_ADDR} " +
                                 "$SOURCE $TARGET")
   env['BUILDERS']['Relocate'] = RelocBuilder

#------------------------------------------------------------------------------
# Qurt Image
#------------------------------------------------------------------------------
#guest os image needs to be first in source list
def qurt_image_generate(env):
   QurtImageBuilder = Builder(action = "python ${Q6_RTOS_ROOT}/scripts/qurt-image-build.py $SOURCES -o $TARGET -p ${PADDR} -t ${Q6_TOOLS_ROOT}")
   env['BUILDERS']['QurtImage'] = QurtImageBuilder

#------------------------------------------------------------------------------
# Builder: Deriving build_params.txt
#------------------------------------------------------------------------------
def build_param_generate(env):
   build_param_bld = Builder(action = gen_build_param)
   env['BUILDERS']['build_param_builder'] = build_param_bld

def gen_build_param(target, source, env):
   target = str(target[0])   
   fileHandle = open (target, 'w')
   fileHandle.write("Q6VERSION="         +env.subst('$Q6VERSION')+"\n")
   fileHandle.write("BUILD_CONFIG_FLAGS="+env.subst('$BUILD_CONFIG_FLAGS')+"\n")
   fileHandle.write("Q6_TOOLS_ROOT="     +os.path.realpath(env.subst('$Q6_TOOLS_ROOT')))
   fileHandle.close()

#------------------------------------------------------------------------------
# Builder: Prepending cust_config.c
#------------------------------------------------------------------------------
def ppend_cust_config_generate(env):
   cust_config_bld = Builder(action = ppend_cust_config)
   env['BUILDERS']['cust_config_builder'] = cust_config_bld

def ppend_cust_config(target, source, env):
   target = str(target[0])
   tmplt  = str(source[0])
   mcr    = str(source[1])
   
   fileTarget = open (target, 'w')
   fileTmplt  = open (tmplt, 'r')
   fileMcr    = open (mcr, 'r')
   
   for line in fileMcr:
      if line.find('THREAD_CONTEXT_TOTALSIZE') != -1:
         fileTarget.write(line)
      elif line.find('QURT_CONFIG_MAX_HTHREAD') != -1:
         fileTarget.write(line)
   
   for line in fileTmplt:
      fileTarget.write(line)

   fileTarget.close()

#------------------------------------------------------------------------------
# Builder: qurt_config_vars.h
#------------------------------------------------------------------------------
def config_vars_generate(env):
   env['BUILDERS']['ConfigVarsGen'] = Builder(action = config_vars_builder)

def config_vars_builder(target, source, env):
    arglist = env.subst("$CC -E -P -DGEN_CONFIG_HEADER").split(" ")
    for w in source:
        if re.match('.*\\.h', str(w)):
            arglist += ["-include", str(w)]
    for w in source:
        if re.match('.*\\.c', str(w)):
            arglist += [str(w)]
    outinc_file = open(str(target[0]), 'w')
    outinc_file.write('/* This file is automatically generated. */\n\n')
    outinc_file.write('#ifndef GEN_CONFIG_HEADER_USED\n')
    outinc_file.write('#define GEN_CONFIG_HEADER_USED\n')
    outinc_file.flush()
    print arglist
    cpp = subprocess.Popen(arglist, stdout=outinc_file)
    val = cpp.wait()
    if val == 0:
        outinc_file.write('#endif /* GEN_CONFIG_HEADER_USED */\n')
    outinc_file.close()
    if val != 0:
        os.remove(str(target[0]))
    return val


#------------------------------------------------------------------------------
# Builder: qurt_config_vars.h
#------------------------------------------------------------------------------
def const_autogen_generate (env):
   env['BUILDERS']['const_autogen_builder'] = Builder(action = const_autogen_builder)

def const_autogen_builder(target, source, env):
    arglist = env.subst("${SIM} --quiet "+str(source[0])).split(" ")
    outinc_file = open(str(target[0]), 'w')
    print arglist
    cpp = subprocess.Popen(arglist, stdout=outinc_file)
    val = cpp.wait()
    outinc_file.close()
    if val != 0:
        os.remove(str(target[0]))
    return val
#------------------------------------------------------------------------------
# xml config with paddr option 
#------------------------------------------------------------------------------
def qurt_image_generate(env):
   QurtImageBuilder = Builder(action = "python ${Q6_RTOS_ROOT}/scripts/qurt-image-build.py $SOURCES -o $TARGET -p ${PADDR} -t ${Q6_TOOLS_ROOT}")
   env['BUILDERS']['QurtImage'] = QurtImageBuilder

#------------------------------------------------------------------------------
# traptab builder
#------------------------------------------------------------------------------
def traptab_generate(env):
   env['BUILDERS']['TrapTabGen'] = Builder(generator = traptab_generator)
def traptab_generator(source, target, env, for_signature):
   return ' '.join(['python ', str(source[0]),'-outtab', str(target[0]),'-outinc', str(target[1]),env['CC'],env.subst('$CCFLAGS'),' '.join("-I%s" % w for w in env['CPPPATH']),'-E -x c', str(source[1])])

