#===============================================================================
# Device Config Builder
#    Contains builder(s) definitions
# Copyright (c) 2011,2014-16,2018,2020 by Qualcomm Technologies Incorporated.
# All Rights Reserved.
# QUALCOMM Proprietary/GTDR
#===================================================================================
import re
import os
import subprocess
import string
from SCons.Script import *
import types
import shutil
import SCons.Scanner

#cuz scons isnt python, and doesnt know where the hell i am
import imp
image_config = imp.load_source('image_config',os.path.dirname(os.path.realpath(__file__))+'/image_config.py')
devcfg_input = imp.load_source('devcfg_input',os.path.dirname(os.path.realpath(__file__))+'/devcfg_input.py')
propgen = imp.load_source('propgen',os.path.dirname(os.path.realpath(__file__))+'/propgen.py')

#------------------------------------------------------------------------------
# Hooks for Scons
#------------------------------------------------------------------------------
def exists(env):
    return True

def generate(env):
    rootenv = env.get('IMAGE_ENV')
    
    #Allow clients to use DeviceConfig
    rootenv.Replace(USES_DEVCFG = True)
    rootenv.Replace(USES_DEVCFG_STRICT_XML = True)
    
    #Set the macros DEVCFG_SCRIPTS and DEVCFG_ROOT
    rootenv.Replace(DEVCFG_SCRIPTS = os.path.dirname(os.path.realpath(__file__)))
    rootenv.Replace(DEVCFG_ROOT = os.path.dirname(rootenv.get('DEVCFG_SCRIPTS')))
    
    #Install AddDevcfgInfo API to the rootenv. Allows clients to pass XML/C config
    rootenv.AddMethod(add_devcfg_info, "AddDevCfgInfo")
    #Returns the actual number of tags used by clients in a multi-cfg setting
    rootenv.AddMethod(get_devcfg_currently_used_tags, "GetDevcfgRelevantTags")

    env.AddMethod(buildDevcfgData, "buildDevcfgData")
    
    env.Replace(IMAGE_CONFIG_XML = os.path.realpath(env.subst("${IMAGE_CONFIG_XML}")))
    
    image_config_xml = env.subst("${IMAGE_CONFIG_XML}")
    if not os.path.isfile(image_config_xml):
        raise ValueError("ERROR : image_cfg.xml not found !")
        exit()
    
    config = image_config.Config(image_config_xml)
    print "Configs : "
    print config.devcfgImgBuildTag

    rootenv.Replace(DEVCFG = config)
    rootenv.Replace(DEVCFG_SOURCES = [])
    rootenv.Replace(DEVCFG_XML_TAGS = [config.devcfgImgBuildTag])
    env.Replace(PROPGEN_CMD = "${DEVCFG_SCRIPTS}/propgen.py")
    env.Replace(IMGCONFIG_CMD = "${DEVCFG_SCRIPTS}/image_config.py")

    #Add 'devcfgImgBuildTag' to the image BUILD_TAGS
    devcfg_build_tag = config.devcfgImgBuildTag
    if devcfg_build_tag not in env.get('BUILD_TAGS'):
        env.get('BUILD_TAGS').append(devcfg_build_tag)
        env.Dictionary().setdefault(devcfg_build_tag,env.get('PROC'))
    
    env.Append(CPPDEFINES = ["DEVCFG_TARGET_" + (config.targetImg).upper()])
    if config.tcsrPhyToVirSame == 'true':
        env.Append(CPPDEFINES = ["TCSR_ADDR_PHS_SAME_AS_VIR"])
    if config.multiCfg == 'true':
        env.Append(CPPDEFINES = ["DEVCFG_MULTICFG_ENABLED"])
    if config.devcfgPlatformXMLTagList != []:
        env.Append(CPPDEFINES = ["DEVCFG_MULTIPLATFORM_ENABLED"])
    if config.oemCfgBaseAddr != '':
        env.Append(CPPDEFINES = ["DEVCFG_OEM_ENABLED"])
        
    env.Append(CPPDEFINES = ["DAL_CONFIG_TARGET_ID="+config.dalConfigTargetId])
    env.Replace(DAL_CONFIG_TARGET_ID = config.dalConfigTargetId)
        
    #Install all builders
    devcfg_img_header_file_generate(env)
    devcfg_xml_generate(env)
    structPtrHeader_generate(env)
    dal_moddir_src_generate(env)
    devcfgDataSrc_generate(env)
    devcfgManifest_generate(env)
    devcfgIncPaths_generate(env)
    devcfgBlobTableBuilder_generate(env)

    return
   
def devcfg_img_header_file_generate(env):
    env.Append(BUILDERS = {'GenerateDevcfgImgHdr' : Builder(action=devcfg_img_header_file_builder, action_source=None)})

def devcfg_img_header_file_builder(env, target, source):
    fp = open(str(target[0]),'w')
    image_name = env.get('DEVCFG').targetImg
    fp.write('extern DEVCFG_TARGET_INFO devcfg_target_soc_info_'+image_name+'[];\n')
    fp.write('#define devcfg_target_soc_info devcfg_target_soc_info_'+image_name+'\n')
    fp.close()
    return

def devcfg_target_scan_func(node, env, path):
    rootenv = env.get('IMAGE_ENV')
    devcfgInput = rootenv.get('DEVCFG_SOURCES')
    for i in devcfgInput:
        #if not i.file.lower().endswith(".o"):
        env.Depends(node, i.file)
    return []

def get_devcfg_currently_used_tags(env, targets, input_dict):
    rootenv = env.get('IMAGE_ENV')
    return env.get('DEVCFG',None).get('DEVCFG_CURRENT_USED_XML_TAGS',None)

#------------------------------------------------------------------------------
# Add client input API
#------------------------------------------------------------------------------
def add_devcfg_info(env, images, input_dict):
    #record tag, file(s), cpppath
    env = env.Clone()
    rootenv = env.get('IMAGE_ENV')

    image_config_xml = env.subst("${IMAGE_CONFIG_XML}")
    if not os.path.isfile(image_config_xml):
        raise ValueError("ERROR : {0} is not a valid xml config path".format(image_config_xml))
        exit()

    config = image_config.Config(image_config_xml)
    if 'DEVCFG_BLOB_ID' not in env:
        tag_list = config.devcfgXMLTagList
    else:
        tag_list = set(config.devcfgXMLTagList) & set([env["DEVCFG_BLOB_ID"]])

    new_dict = {}
    keys = (input_dict.keys())
    for key in keys:
        if key in ['SOC_XML','soc_xml','devcfg_xml']:
            for tag in tag_list:
                if 'PLATFORM' not in tag:
                    new_dict[tag] = input_dict[key]
    if not bool(new_dict):
        new_dict = input_dict
    input_dict = new_dict

    if 'DEVCFG_STANDALONE' in env:
        return_list = []

    if isinstance(images, str):
        raise ValueError("ERROR: Invalid Image list specified when adding Device Config Input")
        return -1
    for image in (set(images) & set(env.get('BUILD_TAGS'))):
        for xml_tag in tag_list:
            thisenv = env.Clone()
            if xml_tag in input_dict:
                if isinstance(input_dict[xml_tag], str):
                    raise ValueError("ERROR: device config input dictionary expects a list as its value")
                    return -1
                for file in input_dict[xml_tag]:
                    if os.path.splitext(file)[1] == '.c':
                        try:
                            structHeaaderPath = env["DEVCFG"].structHeaders[xml_tag]
                        except Exception as e:
                            structHeaaderPath = "$DEVCFG_ROOT/build/${{BUILDPATH}}/data/{}/devcfgStructs.h".format(xml_tag)
                        thisenv.Append(CCFLAGS = " -include {} ".format(structHeaaderPath))
                        thisenv.Replace(OBJPREFIX = xml_tag)
                        thisobj = thisenv.AddObject(image, file)
                        env.Depends(thisobj, structHeaaderPath)
                        if 'DEVCFG_STANDALONE' in env:
                            return_list += thisobj
                    rootenv.Append(DEVCFG_SOURCES = [devcfg_input.DevcfgInput(xml_tag,os.path.realpath(env.subst(file)),[],env.get('CPPPATH'))])
    if 'DEVCFG_STANDALONE' in env:
        return return_list

#------------------------------------------------------------------------------
# Devcfg Manifest Builder
#------------------------------------------------------------------------------
def devcfgManifest_generate(env):
    env.Append(BUILDERS = {'devcfgManifest' : env.Builder(action = devcfgManifestBuilder, target_scanner=env.Scanner(devcfg_target_scan_func))})

def devcfgManifestBuilder(env, target, source):
    manifestFH = open(str(target[0]), "w")
    fileList = getDevcfgClientData(env, '')[0]
    for inputFile in fileList:
        manifestFH.write(inputFile+"\n")
    manifestFH.close()

#------------------------------------------------------------------------------
# Devcfg Manifest Builder
#------------------------------------------------------------------------------
def devcfgIncPaths_generate(env):
    env.Append(BUILDERS = {'devcfgIncpaths' : env.Builder(action = devcfgIncPathsBuilder, target_scanner=env.Scanner(devcfg_target_scan_func))})

def devcfgIncPathsBuilder(env, target, source):
    incpathsFH = open(str(target[0]), "w")
    incPaths = getDevcfgClientData(env, '')[1]
    for incPath in incPaths:
        incpathsFH.write("-I{} ".format(env.subst(incPath).replace("\\","/")))
    incpathsFH.close()

#------------------------------------------------------------------------------
# Devcfg Master XML Builder
#------------------------------------------------------------------------------
def devcfg_xml_generate(env):
    env.Append(BUILDERS = {'devcfgMasterXml' : env.Builder(action = devcfg_xml_builder, target_scanner=env.Scanner(devcfg_target_scan_func))})

def devcfg_xml_builder(env, target, source):
    xml_file_list = getDevcfgClientData(env, '.xml')[0]
    return propgen.genMasterXML(env.RealPath(str(target[0])), xml_file_list)

#------------------------------------------------------------------------------
# Devcfg Data Source Builder
#------------------------------------------------------------------------------
def structPtrHeader_generate(env):
    env.Append(BUILDERS = {'structPtrHeader' : env.Builder(action = structPtrHeaderBuilder, target_scanner=env.Scanner(devcfg_target_scan_func))})

def structPtrHeaderBuilder(env, target, source):
    (memory_optimise, struct_size, config, symbol) = getPropgenCommonArgs (env)
    xml_file_list = getDevcfgClientData(env, '.xml')[0]
    inputStr=""
    for xmlFile in xml_file_list:
        inputStr += " -i {}".format(xmlFile)
    cmd2 = "python ${{PROPGEN_CMD}} --StructHeader={}{} --ConfigType={} {} {} {}".format(target[0], inputStr, config, symbol, memory_optimise, struct_size)
    print env.subst(cmd2)
    return subprocess.call(env.subst(cmd2).split())

#------------------------------------------------------------------------------
# Build the devcfg library (put it all together
#------------------------------------------------------------------------------
def buildDevcfgData(env, blobname):
    print "Blobname: ", blobname
    thisblob = ""
    for b in env.get('DEVCFG').blobs:
        if b["id"] == blobname:
            thisblob = b
    if thisblob == "":
        raise ValueError("ERROR : DEVCFG blob is not configured for {0}. Please review {1}".format(blobname, env.subst("${IMAGE_CONFIG_XML}")))
        exit()
    targetPathBaseStr = "${{BUILDPATH}}/{}/{}"
    #generate Manifest
    manifest = env.devcfgManifest(targetPathBaseStr.format(blobname,'manifest.txt'), [], DEVCFG_BLOBNAME=blobname)
    #generate Incpaths
    incPaths = env.devcfgIncpaths(targetPathBaseStr.format(blobname,'incpaths.txt'), [], DEVCFG_BLOBNAME=blobname)
    #call builder for masterxml
    masterXml = env.devcfgMasterXml(targetPathBaseStr.format(blobname,'master.xml'), [], DEVCFG_BLOBNAME=blobname)
    #call builder for structheaderfile
    structHeader = env.structPtrHeader(targetPathBaseStr.format(blobname,'devcfgStructs.h'), [], CONFIG=blobname, DEVCFG_BLOBNAME=blobname)
    env["DEVCFG"].structHeaders[blobname]=structHeader[0]
    #call builder to compile masterxml
    devcfgDataSrc = env.DALConfigSrcBuilder([targetPathBaseStr.format(blobname,'devcfg.c'), targetPathBaseStr.format(blobname,'devcfg_data.c')], [masterXml], CONFIG=blobname)
    #generate library from objects above
    cenv = env.Clone()
    cenv.Append(CCFLAGS = " @"+os.path.abspath(str(incPaths[0])))
    if (thisblob['type']) == 1 or (thisblob['type']) == 2:
        cenv.Append(CCFLAGS = " -fPIC -fdata-sections ")
    #devcfgDataSrc += getDevcfgClientData(env, '.o')[0]
    print blobname+" devcfgdatasrc : "+str(devcfgDataSrc)
    devcfgLib = cenv.Library(targetPathBaseStr.format(blobname,'devcfg.lib'), [devcfgDataSrc])
    env.Depends(devcfgDataSrc, incPaths)
    env.Depends(devcfgDataSrc, manifest)
    #devcfgLib = env.DevcfgLibrary(targetPathBaseStr.format(blobname,'devcfg.lib'), [devcfgDataSrc])
    return (devcfgLib, masterXml)

#------------------------------------------------------------------------------
# devcfg blob table Builder
#------------------------------------------------------------------------------
def devcfgBlobTableBuilder_generate(env):
    env.Append(BUILDERS = {'devcfgBlobTableBuilder' : env.Builder(action = devcfgBlobTableBuilder, suffix = '.c', src_suffix = '.xml')})
   
def devcfgBlobTableBuilder(env, target, source):
    cmd = "python ${{IMGCONFIG_CMD}} {1} {0}".format(target[0], source[0])
    print env.subst(cmd)
    return subprocess.call(env.subst(cmd).split())

#------------------------------------------------------------------------------
# DAL ModDir Builder
#------------------------------------------------------------------------------
def dal_moddir_src_generate(env):
    DALModDir_src_bld = env.Builder(action = DALModDir_src_builder, source_scanner = SCons.Scanner.C.CScanner(), suffix = '.c', src_suffix = '.xml')
    env.Append(BUILDERS = {'DALModDirSrcBuilder' : DALModDir_src_bld})
   
def DALModDir_src_builder(env, target, source):
    (memory_optimise, struct_size, config, symbol) = getPropgenCommonArgs (env)
    cmd = "python ${{PROPGEN_CMD}} --input={} --ModDirFile={} --ConfigType={} {} --RemoveDups {} {}".format(source[0], str(target[0]), config, symbol, memory_optimise, struct_size)
    print env.subst(cmd)
    return subprocess.call(env.subst(cmd).split())
   
#------------------------------------------------------------------------------
# Devcfg Data Source Builder
#------------------------------------------------------------------------------
def devcfgDataSrc_generate(env):
    env.Append(BUILDERS = {'DALConfigSrcBuilder' : env.Builder(action = devcfgDataSrcBuilder, target_scanner=env.Scanner(devcfg_target_scan_func))})

#def devcfgDataSrc_emitter(target, source, env):
#    target.append(getDevcfgClientData(env, '.o')[0])
#    return target, source

def devcfgDataSrcBuilder(env, target, source):
    (memory_optimise, struct_size, config, symbol) = getPropgenCommonArgs (env)
    fixed_addr = env.subst("${DEVCFG_FIXED_ADDR}")
    if fixed_addr != "":
        fixed_addr = "--fixed-addr={}".format(fixed_addr)

    cmd = "python ${{PROPGEN_CMD}} --input={} --ConfigFile={} --DevcfgDataFile={} --ConfigType={} {} {} {} {}".format(source[0], str(target[0]), str(target[1]), config, symbol, memory_optimise, struct_size, fixed_addr)
    print env.subst(cmd)
    return subprocess.call(env.subst(cmd).split())


#------------------------------------------------------------------------------
# helper functions
#------------------------------------------------------------------------------
def getPropgenCommonArgs (env):
    memory_optimise = ""
    if env.get('DEVCFG').propsMemoryOptimise:
        memory_optimise = "--MemoryOptimiseProps"

    struct_size = ""
    if env.get('DEVCFG').structSize:
        struct_size = "--structSize"

    config = env.get('CONFIG')
    if not config:
        config = ""

    symbol = ""
    for b in env.get('DEVCFG').blobs:
        if config == b["id"]:
            symbol = "--Symbol={}".format(b["name"])

    return (memory_optimise, struct_size, config, symbol)

def getDevcfgClientData(env, extFilter):
    file_list = []
    incPaths = []
    rootenv = env.get('IMAGE_ENV')
    blobName = env.get("DEVCFG_BLOBNAME")
    for i in rootenv.get('DEVCFG_SOURCES'):
        if blobName is None or i.blob == blobName:
            if extFilter == '' or os.path.splitext(i.file)[1] == extFilter:
                file_list.append(i.file)
                incPaths = list(set(i.includes) | set(incPaths))
    return file_list, incPaths
