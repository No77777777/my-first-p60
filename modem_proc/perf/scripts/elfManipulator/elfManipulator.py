##  @file       elfManipulator.py
#   @author     Christopher Ahn (cahn@qualcomm.com)
#   @brief      This script provides a generic manipulator for ELF files.
#   @version    1.10
#   @todo       The elf manipulator currently only supports a sub-class of ELF
#               files. Specifically, it is limited to 32-bit little-endian
#               executable targeted for QDSP6V5, with PT_LOAD segments only,
#               no dynamic linking, and no relocations. It is a future action
#               item to broaden the scope of this script.

#===============================================================================
# Copyright (c) 2013 by Qualcomm Technologies, Incorporated.
# All Rights Reserved.
# QUALCOMM Proprietary/GTDR
#
#-------------------------------------------------------------------------------
#  $Header: //components/rel/perf.mpss/4.4.2/scripts/elfManipulator/elfManipulator.py#1 $
#  $DateTime: 2022/10/12 18:54:20 $
#  $Author: pwbldsvc $
#  $Change: 40258507 $
#                      EDIT HISTORY FOR FILE
#
#  This section contains comments describing changes made to the module.
#  Notice that changes are listed in reverse chronological order.
#
# when      who     ver     what, where, why
# --------  ------  ----    ----------------------------------------------------
# 05/27/16  raholr  1.16    Included the .clade.* sections in calculating space available for the overlay_memdump section
# 12/17/15  anandj  1.15    Reordered Q6Zip build-tools search paths
# 03/05/15  hzhi    1.14    CR 800980 add fix: remove conditional code path when 'coprocimage'
#                           is defined.
# 02/27/15  hzhi    1.14    CR 800980: move section offset move up towards the end of post
#                           link stage..
# 02/23/15  anandj  1.13    Usage of q6zip_versions module (if it exists) for choosing
#                           compressor based on versions of algorithm selected [cporter]
# 02/05/15  ao      1.12    Added support for CR 777884 (partial decompressions) and CR 700782 (q6zip info to build.log memory summary)
# 02/03/15  anandj  1.11    Fixed setting of end_va_compressed_text, end_va_compressed_rw
# 12/01/14  cahn    1.10    Added support for coproc ELF merging.
# 09/12/14  ao      1.09    Fixed setting end_va_compressed_text bug introduced in ver 1.06
# 09/11/14  ao      1.08    Remove use of updateSymbolValueByName and printSymbolInfoByName
#                           for faster execution.
# 09/10/14  cahn    1.07    Added ELF offset adjustments to RW compression to
#                           address NAND boot-up crashes.
# 07/22/14  ao      1.06    Added RW compression.
# 03/13/14  cahn    1.05    Removing executable flag from q6zip section/segment.
# 02/24/14  cahn    1.04    Added -M option for overlay_mem_dump feature.
# 01/20/14  ao      1.03    Import compressor only if there is a candidate_compress_section
# 01/20/14  ao      1.02    Added q6zip3 compressor call
# 12/11/13  cahn    1.01    Moving q6zip include only if section is present.
# 11/21/13  cahn    1.00    Optimizations and speedup.
# 04/05/13  cahn    0.17    Added support for getting only 1 symbol's info.
# 04/03/13  cahn    0.16    Fixed bug wherein the image_vend was being updated
#                           based on the last segment in the linker instead
#                           of segment with the highest virtual address. Also
#                           some cosmetic changes.
# 03/29/13  cahn    0.15    Various new APIs added. Changed the name of functions.
# 03/26/13  cahn    0.14    Support for ro_fatal compression.
# 03/18/13  cahn    0.13    Added __init__.py and more sample modifications
# 03/18/13  cahn    0.12    Added support for resizing of sections and updating
#                           symbol value by name.
# 03/15/13  cahn    0.11    Updated usage text.
# 03/15/13  cahn    0.10    ALPHA release. Added support for removal of section.
# 03/10/13  cahn    0.09    Added functionality for writing out modified ELF.
# 03/07/13  cahn    0.08    Specification review, cleanup and added Doxygen
#                           comments.
# 03/07/13  cahn    0.07    Code abstracted to elfFileClass.py.
# 03/06/13  cahn    0.06    Added support for reading the symbol table.
#                           Added debug print, partially modeled from readelf.
# 03/05/13  cahn    0.05    Added mapping of section names from .shstrtab index.
# 03/05/13  cahn    0.04    Added support for reading the section header table.
# 03/05/13  cahn    0.03    Added support for reading the program header table.
# 03/04/13  cahn    0.02    Added support for reading the ELF Header.
# 03/04/13  cahn    0.01    Creation.
#===============================================================================

# Imports
import os
import sys
import time
import inspect
import optparse
import include.elfConstants as const
import include.elfFileClass as elfFileClass
import include.elfUtils as utils
import hashlib

from os.path import join as pj
from os.path import split as ps
from os.path import exists as pe
from os.path import abspath as ap

##
# @brief    Main entry point
def main():
    # Use optparse to set up usage
    use = "Usage: python %prog [options] <Input ELF> <Output ELF>"
    use += "\n       python %prog -s symbolName <ELF>"
    parser = optparse.OptionParser(usage = use, version="%prog 1.16")
    parser.add_option(  "-d", action="store_true", dest="debug",
                        help="enable debug")
    parser.add_option(  "-r", action="store", dest="removeSection", type="str",
                        help="Section to remove")
    parser.add_option(  "-t", action="store_true", dest="timing",
                        help="print program execution time")
    parser.add_option(  "-V", action="store_true", dest="verification",
                        help="enable ELF verification (currently none)")
    parser.add_option(  "-R", action="store_true", dest="ro_fatal",
                        help="Enable ro fatal section removal/compression")
    parser.add_option(  "-M", action="store_true", dest="overlay_mem_dump",
                        help="Enable memory dump overlay")
    parser.add_option(  "-c", action="store", dest="compressorRoot",
                        help="Specify the root folder of the Q6Zip compressor")
    parser.add_option(  "-s", action="store", dest="coprocMerge", type="str",
                        help="Path of coprocessor (silver) ELF")
    parser.add_option(  "-Q", action="append", dest="q6zipFlags", type="str",
                        help="USES flags that start with USES_Q6ZIP_")
    options, arguments = parser.parse_args()

    # Check arguments
    if len(arguments) != 2:
        parser.error("Unexpected argument length")
        exit(const.RC_ERROR)

    baseELF = arguments[0]
    modifiedELF = arguments[1]
    if not pe(baseELF):
        parser.error("Specified ELF file does not exist.")
        exit(const.RC_ERROR)

    # Print configuration
    if not options.debug:
        print "================================================================"
        print " elfManipulator.py - Generic manipulator for ELF files"
        print "----------------------------------------------------------------"
        print " Base ELF: ".ljust(20) + baseELF
        print " Modified ELF: ".ljust(20) + modifiedELF
        print " Debug: ".ljust(20) + str(options.debug)
        print " Verification: ".ljust(20) + str(options.verification)
        print "================================================================"

    # Record the starting time
    if options.timing:
        start_time = time.time()

    # Get the elf file as an elfFile object
    if not options.debug:
        print "----------------------------------------------------------------"
        print " Getting ELF data from input ELF..."
        print "----------------------------------------------------------------"
    elf = elfFileClass.elfFile(baseELF)

    # If debug is enabled, show ELF contents similarly to readelf -a
    if options.debug:
        elf.printInfo()

    if not options.debug:
        print "----------------------------------------------------------------"
        print " Applying requested ELF modifications..."
        print "----------------------------------------------------------------"

    """
    HERE IS WHERE ALL THE ELF MODIFICATIONS SHOULD HAPPEN
    """
    # -r option: Remove specified section
    if options.removeSection:
        if not options.debug:
            print "Attempting to remove section '" + options.removeSection + "'"
        elf.removeSectionByName(options.removeSection)

    symbolDict = {}

    # -M option: Remove BSS section for 'overlay_mem_dump' feature
    overlay_mem_dumpSize = -1
    if options.overlay_mem_dump:
        print "OVERLAY_MEM_DUMP"
        overlay_mem_dumpSh = elf.getSectionByName("overlay_mem_dump")
        ro_fatalSh = elf.getSectionByName("ro_fatal")
        if overlay_mem_dumpSh != const.RC_ERROR and ro_fatalSh != const.RC_ERROR:
            # If an overlay_mem_dump section is found
            print "\t'overlay_mem_dump' section found"
            print "\tAttempting to remove 'overlay_mem_dump' BSS section."
            overlay_mem_dumpSize = overlay_mem_dumpSh.sh_size
            elf.removeBssOverlaySectionByName("overlay_mem_dump")
            if overlay_mem_dumpSize > ro_fatalSh.sh_size:
                # It is expected that size of ro_fatal section matches the
                # program segment size in subsequent steps. elfManipulator is
                # currently written to manipulate single section segments,
                # with the EXCEPTION of removing BSS sections.
                print "\t'overlay_mem_dump' exceeded 'ro_fatal' size. Re-sizing."
                elf.resizeSectionByName("ro_fatal", len(ro_fatalSh.contents))
        else:
            if overlay_mem_dumpSh == const.RC_ERROR:
                print "\tNo 'overlay_mem_dump' section found. No-op."
            if ro_fatalSh == const.RC_ERROR:
                print "\tNo 'ro_fatal' section found. No-op."

    # -R option: Remove or compress 'ro_fatal' section
    if options.ro_fatal:
        print
        print "RO_FATAL COMPRESSION"
        # Get the section in question
        sh = elf.getSectionByName("ro_fatal")
        if sh != const.RC_ERROR:
            # If an ro_fatal section is found
            print "\t'ro_fatal' section found"
            if sh.sh_size == 0:
                # If 0 size section is found, remove the section for mbn
                print "\tZero-sized 'ro_fatal' section. Attempting removal."
                elf.removeSectionByName("ro_fatal")
            else:
                # Move up the ro_fatal section and then compress it
                print "\tMoving up 'ro_fatal' to closest section"
                symbolDict["__ro_fatal_old_start"] = sh.sh_addr
                oldSize = sh.sh_size
                elf.moveupSectionByName("ro_fatal", 262144)
                print "\tCompressing 'ro_fatal' contents using zlib"
                sh = elf.compressSectionByName("ro_fatal")
                if (sh != const.RC_ERROR):
                    # If compression is successful, update symbols accordingly
                    symbolDict["__ro_fatal_new_start"] = sh.sh_addr
                    symbolDict["__ro_fatal_new_end"] = sh.sh_addr + len(sh.contents)
                else:
                    utils.raiseElfManipulatorError("Failed to compress ro_fatal")
                # Print out statistics
                print "\tOld Size: " + str(oldSize)
                print "\tNew Size: " + str(sh.sh_size)

        else:
            # The last segment is still created due to linker script
            print "\tNo 'ro_fatal' section found, checking for zero-sized segment"
            if ((elf.programHeaderTable[-1].p_memsz == 0) and
                (elf.programHeaderTable[-1].p_vaddr == 0)):
                print "\tRemoving zero-sized segment"
                elf.elfHeader.e_phnum -= 1
                elf.programHeaderTable.pop()

    #=============================================
    # START: Q6_ZIP CHANGES
    #=============================================
    q6_roSection = ".candidate_compress_section"
    q6_rwSection = ".rw_candidate_compress_section"
    
    pageSize = 4096

    # determine where the compressors reside
    my_d = ps(inspect.getframeinfo(inspect.currentframe()).filename)[0]

    # *WARNING* Order of search path in compressorDirs is important. It must
    # look for q6zip_versions.py (asynchronous DL pager styled organization)
    # first and fall back to q6zip_uncompress.py (synchronous DL pager styled
    # organization) 
    compressorDirs = [
        ap(pj(my_d, "../../q6zip/core/compressor")),
        ap(pj(my_d, "../../../core/kernel/dlpager/compressor")),
        ap(pj(my_d, "./include")),
    ]

    if options.compressorRoot:
        compressorDirs.insert(0, options.compressorRoot)

    for d in compressorDirs:
        if os.path.isdir(d):
            if 'q6zip_versions.py' in os.listdir(d):
                sys.path.insert(0, d)
                import q6zip_versions
                q6zip_versions.init( options.q6zipFlags )
                sys.path.insert(0, q6zip_versions.RO_BUILDTIME_PATH)
                sys.path.insert(0, q6zip_versions.RW_BUILDTIME_PATH)
                print "Q6Zip versions:  RO %s, RW %s" % ( q6zip_versions.RO_VERSION, q6zip_versions.RW_VERSION )
                print "Q6Zip IPA used: %s" % q6zip_versions.ENABLE_Q6ZIP_IPA
                print "Q6Zip RO build-tool path: %s" % q6zip_versions.RO_BUILDTIME_PATH
                print "Q6Zip RW build-tool path: %s" % q6zip_versions.RW_BUILDTIME_PATH
                break
            elif 'q6zip_compress.py' in os.listdir(d):
                sys.path.insert(0, d)
                break

    print
    print "Q6ZIP FEATURE (RO)"

    # Get the ELF section containing the code to compress
    sh = elf.getSectionByName(q6_roSection)
    # Check if the section exists
    if (sh != const.RC_ERROR):

        # Save the old VA before ANY modifications to the section/segment
        oldVA = sh.sh_addr
        oldSize = sh.sh_size
        print "oldVA = " + hex(oldVA)
        print "Size of " + q6_roSection + " is " + str(sh.sh_size)
        symbolDict["start_va_uncompressed_text"] = sh.sh_addr
        symbolDict["end_va_uncompressed_text"] = sh.sh_addr + sh.sh_size

        # Move up the vaddr of the section, corresponding segment and mark all
        # its symbols as SHN_UNDEF. Will fail if q6_roSection is part of multi-
        # section segment in the ELF.
        # Save the new VA after moving up the section/segment
        print "Moving up " + q6_roSection + " to closest segment"
        elf.moveupSectionByName(q6_roSection)
        newVA = sh.sh_addr
        print "newVA = " + hex(newVA)

        print "Compress " + q6_roSection
        import q6zip_compress
        try:
            text_nonpartial_start = elf.getSymbolByName("__swapped_range_text_partial_end__").st_value
            if text_nonpartial_start != 0:
                size_partial = text_nonpartial_start - oldVA
            print "text nonpartial start = " + hex(text_nonpartial_start)
            print "text partial size = " + str(size_partial)
            sh.contents = q6zip_compress.compress(pageSize, newVA, sh.contents, size_partial)
        except:
            sh.contents = q6zip_compress.compress(pageSize, newVA, sh.contents)

        # *WARNING* Assign values to these special Q6ZIP symbols *before*
        # padding to page boundary. Ensures that size of last compressed block
        # is computed correctly by DL pager
        symbolDict["start_va_compressed_text"] = newVA
        symbolDict["end_va_compressed_text"] = sh.sh_addr + len( sh.contents )

        # Alignment needed for Q6ZIP RW in the next section
        alignedSize = pageSize * (len(sh.contents)/pageSize+1)
        for i in xrange(alignedSize - len(sh.contents)):
            sh.contents += '\0'
        elf.resizeSectionByName(q6_roSection, alignedSize)

        print("Memory Savings Report: Q6Zip RO: Gross Memory Saved: %u\n"%(oldSize-sh.sh_size))
        print 'start_va_compressed_text=0x%08x end_va_compressed_text=0x%08x' % ( symbolDict[ 'start_va_compressed_text' ], symbolDict[ 'end_va_compressed_text' ] )

        # Change permissions (remove execute)
        print "\tRemoving X from " + q6_roSection
        sh.sh_flags = sh.sh_flags & ~const.sectionFlags.SHF_EXECINSTR
        ph = elf.getProgramHeaderBySectionName(q6_roSection)
        if ph != const.RC_ERROR:
            ph.p_flags &= ~const.segmentFlags.PF_X
        else:
            utils.raiseElfManipulatorError("Unexpected error while changing permissions for " + q6_roSection)


        if (sh != const.RC_ERROR):
            print "Success compressing " + q6_roSection
        else:
            utils.raiseElfManipulatorError("Failed to compress " + q6_roSection)
    else:
        # Like ro_fatal, need to check for zero-sized segment
        print "No " + q6_roSection + " section found, checking for zero-sized segment"
        if ((elf.programHeaderTable[-1].p_memsz == 0) and
                (elf.programHeaderTable[-1].p_vaddr == 0)):
                print "Removing zero-sized segment"
                elf.elfHeader.e_phnum -= 1
                elf.programHeaderTable.pop()

    print
    print "Q6ZIP FEATURE (RW)"
    # Get the ELF section containing the code to compress
    sh = elf.getSectionByName(q6_rwSection)
    # Check if the section exists
    if (sh != const.RC_ERROR):
        print "\t'" + q6_rwSection + " section found"
        # Save the old VA before ANY modifications to the section/segment
        oldSize = sh.sh_size
        bss_common_start = elf.getSymbolByName("__swapped_segments_bss_start__").st_value
        print "\bss_common_start = " + hex(bss_common_start)
        symbolDict["start_va_uncompressed_rw"] = sh.sh_addr
        symbolDict["end_va_uncompressed_rw"] = sh.sh_addr + sh.sh_size
        rw_contents =  bss_common_start - sh.sh_addr
        print "\t rw_contents = " + hex(rw_contents)

        # Move up the section
        print "\tMoving up " + q6_rwSection + " to closest section"
        elf.moveupSectionByName(q6_rwSection)
        newVA = sh.sh_addr

        print "\tCompressing " + q6_rwSection + " contents using q6 compressor"
        import rw_py_compress
        sh.contents = rw_py_compress.rw_py_compress(pageSize, newVA, sh.contents[0:rw_contents])

        # *WARNING* Assign values to these special Q6ZIP symbols *BEFORE*
        # padding to page boundary. Ensures that size of last compressed block
        # is computed correctly by DL pager
        symbolDict["start_va_compressed_rw"] = newVA
        symbolDict["end_va_compressed_rw"] = sh.sh_addr + len( sh.contents )

        # Pad the section out to nearest alignment (64KiB) to keep TLB usage
        # and footprint creep to minimum
        alignment = ( 64 * 1024 )
        padding   = alignment - ( symbolDict[ 'end_va_compressed_rw' ] % alignment )
        if padding != 0:
            sh.contents = sh.contents.ljust( len( sh.contents ) + padding )

        # Resize the section
        elf.resizeSectionByName(q6_rwSection, len(sh.contents))

        print 'start_va_compressed_rw=0x%08x end_va_compressed_rw=0x%08x padding=%u B' % ( symbolDict[ 'start_va_compressed_rw' ], symbolDict[ 'end_va_compressed_rw' ], padding )

        # Change permissions (remove execute)
        print "\tRemoving X from " + q6_rwSection
        sh.sh_flags = sh.sh_flags & ~const.sectionFlags.SHF_EXECINSTR
        ph = elf.getProgramHeaderBySectionName(q6_rwSection)
        if ph != const.RC_ERROR:
            ph.p_flags &= ~const.segmentFlags.PF_X
        else:
            utils.raiseElfManipulatorError("Unexpected error while changing permissions for " + q6_rwSection)

        # Print out statistics
        print("Memory Savings Report: Q6Zip RW: Gross Memory Saved: %u"%(oldSize-sh.sh_size))

        # Moving up sections
        elf.moveupElfOffsetSectionByName(q6_roSection)
        elf.moveupElfOffsetSectionByName(q6_rwSection)
        # moving this part to pplkcmd.py for CR800980.
        #elf.moveupElfOffsetSectionByName(dynrec_section)
        #elf.moveupElfOffsetSectionByName("QSR_STRING")
    else:
        # Like ro_fatal, need to check for zero-sized segment
        print "\tNo " + q6_rwSection + " section found, checking for zero-sized segment"
        if ((elf.programHeaderTable[-1].p_memsz == 0) and
                (elf.programHeaderTable[-1].p_vaddr == 0)):
                print "\tRemoving zero-sized segment"
                elf.elfHeader.e_phnum -= 1
                elf.programHeaderTable.pop()

    #=============================================
    # END: Q6_ZIP CHANGES
    #=============================================

    # -M option: Remove BSS section for 'overlay_mem_dump' feature
    if options.overlay_mem_dump and overlay_mem_dumpSize >= 0:
        # Need to check that the overlay_memdump does not exceed ro_fatal +
        # candidate_compress_section + CLADE compressed sections AFTER manipulations.
        roFatalSh = elf.getSectionByName("ro_fatal")
        roCompressSh = elf.getSectionByName(q6_roSection)
        rwCompressSh = elf.getSectionByName(q6_rwSection)
        cladeCompSh = elf.getSectionByName(".clade.comp")
        #cladeExcLowLargeSh = elf.getSectionByName(".clade.exception_low_large")
        #cladeExcLowSmallSh = elf.getSectionByName(".clade.exception_low_small")
        #cladeExcHighSh = elf.getSectionByName(".clade.exception_high")
        #cladeDictSh = elf.getSectionByName(".clade.dict")
        #cladeMetaSh = elf.getSectionByName(".clade.metadata")
        

        roFatalSz = 0
        roCompressSz = 0
        rwCompressSz = 0
        cladeCompSz = 0
        cladeExcLowLargeSz = 0
        cladeExcLowSmallSz = 0
        cladeExcHighSz = 0
        cladeDictSz = 0
        cladeMetaSz = 0
        
        mini_mem_dumpSize = 0
        tcm_mem_dumpSize = 0
        qsh_dump_ssize = 0
        clade_protected_reserved_size = 0
        clade_protected_size = 0

        if cladeCompSh != const.RC_ERROR:
            cladeCompSz = cladeCompSh.sh_size

        if roFatalSh != const.RC_ERROR:
            roFatalSz = roFatalSh.sh_size

        if roCompressSh != const.RC_ERROR:
            roCompressSz = roCompressSh.sh_size

        if rwCompressSh != const.RC_ERROR:
            rwCompressSz = rwCompressSh.sh_size

        ##Checking the availability of the Mini dump and TCM max size symbols for dumping
        try:
            tcm_mem_dumpSize=elf.getSymbolByName("__tcm_max_size__").st_value
        except:
            pass
        
        try:
            mini_mem_dumpSize=elf.getSymbolByName("__minidump_size__").st_value
        except:
            pass
        
        try:
            qsh_dump_ssize=elf.getSymbolByName("__qshdump_size__").st_value
        except:
            pass

        try:
            clade_protected_reserved_size=elf.getSymbolByName("__clade_protected_reserve_size__").st_value
            print "__clade_protected_reserve_size__ addr : 0x%x " %(clade_protected_reserved_size)
            clade_protected_reserved_size = elf.getSymbolValueByName("__clade_protected_reserve_size__")
            print "__clade_protected_reserve_size__ value : 0x%x " %(clade_protected_reserved_size)
        except:
            pass

        print "#===========================================================================#"
        print "                          Memory Sections Details                            "
        print "#===========================================================================#"
        print "ro_fatal size: %.4f MB" %(roFatalSz/1024.0/1024)
        
        try:
            clade_protected_sh = elf.getSectionByName(".region_high_clade_protected_memory")
            if clade_protected_sh != const.RC_ERROR:
                clade_protected_size = clade_protected_sh.sh_size
                print "Clade1 protected size                 : %.4f MB" % (clade_protected_size/1024.0/1024)
                clade_protected_size = (clade_protected_size + 1024*1024-1) & (~(1024*1024-1)) # align to next 1MB
                print "Clade1 protected size aligned         : %.4f MB" % (clade_protected_size/1024.0/1024)
                print "__clade_protected_reserve_size__      : %.4f MB" %(clade_protected_reserved_size/1024.0/1024)
                clade_protected_reserved_size = clade_protected_size - mini_mem_dumpSize
                symbolDict["__clade_protected_reserve_size__"] = clade_protected_reserved_size
                print "updated __clade_protected_reserve_size__ to %.4f MB" %(clade_protected_reserved_size/1024.0/1024)
        except:
            print "\tNo 'region_high_clade_protected_memory' section found. No-op."
            pass

        total_available_size=cladeCompSz-clade_protected_size

        print "Clade1 Compress section size    : %.4f MB" % (cladeCompSz/1024.0/1024)
        print "Clade1 protected size           : %.4f MB" % (clade_protected_size/1024.0/1024)
        print "Overlay available size          : %.4f MB" % (total_available_size/1024.0/1024)
        print "Overlay dump size with minidump : %.4f MB" % (overlay_mem_dumpSize/1024.0/1024)
        print "Minidump size                   : %.4f MB" % (mini_mem_dumpSize/1024.0/1024)
        print "Overlay after minidump adjust   : %.4f MB" % ((overlay_mem_dumpSize-mini_mem_dumpSize)/1024.0/1024)
        print "Qshdump size                    : %.4f MB" % (qsh_dump_ssize/1024.0/1024)
        print "TCM Mem Dump size               : %.4f MB" % (tcm_mem_dumpSize/1024.0/1024)
        
        print "#===========================================================================#"

    # After all modifications, update image_vend pointing to the end of the
    # last program segment, aligned using integer division
    lastPh = elf.programHeaderTable[-1]
    for ph in elf.programHeaderTable:
        if ph.p_vaddr > lastPh.p_vaddr:
            lastPh = ph
    endAddress = lastPh.p_vaddr + lastPh.p_align * (lastPh.p_memsz/lastPh.p_align+1)
    symbolDict["image_vend"] = endAddress

    #===========================================================================
    # Option:       -s <COPROC_ELF>
    # Description:  Combine silver coprocessor image with primary ELF. No debug
    #               information will be preserved.
    # Requirements:
    #   [1] Coprocessor image only has a single segment
    #   [2] Primary image has a single section segment (COPROC_IMAGE)
    #   [3] Coprocessor segment does not exceed the original size of COPROC_IMAGE
    #   [4] Coprocessor segment contents becomes the contents of COPROC_IMAGE
    #   [5] __coproc_image_start__ must point to the start of the section
    #===========================================================================
    if options.coprocMerge:
        print "Combining coprocessor image with primary image..."
        # Ensure that coprocessor segment does not exceed COPROC_IMAGE size
        coproc_imageSh = elf.getSectionByName("COPROC_IMAGE")
        coproc_imagePh = elf.getProgramHeaderBySectionName("COPROC_IMAGE")
        print "\tLooking for COPROC_IMAGE section in primary image"
        if coproc_imageSh != const.RC_ERROR and coproc_imagePh != const.RC_ERROR:
            print "\tCOPROC_IMAGE section found in primary image"
            print "\tValidating coprocessor ELF"
            coprocELF = options.coprocMerge
            # Read in the coprocessor ELF and verify [1]
            coproc = elfFileClass.elfFile(coprocELF)
            if len(coproc.programHeaderTable) != 1:
                utils.raiseElfManipulatorError("Coproc image has more than 1 segment")
            print "\tComparing size of coprocessor image with reserved COPROC_IMAGE"
            if coproc_imagePh.p_filesz == coproc.programHeaderTable[0].p_filesz:
                # Read in the coprocessor image
                coprocImage = utils.getDataFromELF( coprocELF,
                                                    coproc.programHeaderTable[0].p_offset,
                                                    coproc.programHeaderTable[0].p_filesz)

                # Replace the contents of COPROC_IMAGE
                print "\tReplacing contents of COPROC_IMAGE"
                coproc_imageSh.contents = coprocImage

                print "\tUpdating __coproc_image_start__"
                symbolDict["__coproc_image_start__"] = coproc_imagePh.p_vaddr
                symbolDict["fw_coproc_image_start"] = coproc_imagePh.p_vaddr
            else:
                # If coprocessor segment exists but is larger than reserved size, need to catch this immediately
                utils.raiseElfManipulatorError("Coprocessor segment != COPROC_IMAGE")
        else:
            print "\tPrimary image does not have COPROC_IMAGE (no-op)"
	
    #If section exists, move shared sw fw region into the modem VA range, 
    #by setting section VA in the reserved space
    section = elf.getSectionByName(".sw_fw_CB")
    sw_fw_postlink_sym = elf.getSymbolByName("__sw_fw_common_buffer_post_link_VA__")
    if section != const.RC_ERROR and sw_fw_postlink_sym != const.RC_ERROR:
      segment = elf.getProgramHeaderBySectionName(".sw_fw_CB")
      segment.p_vaddr = sw_fw_postlink_sym.st_value
      segment.p_paddr = sw_fw_postlink_sym.st_value
      section.sh_addr = sw_fw_postlink_sym.st_value
    
    #If section exists, move shared sw fw region into the modem TCM VA range, 
    #by setting section VA in the reserved space
    section = elf.getSectionByName(".data_tcm_static_fwswnr5G")
    sw_fw_postlink_sym = elf.getSymbolByName("__tcm_static_fwsw_cb_data_start__")
    if section != const.RC_ERROR and sw_fw_postlink_sym != const.RC_ERROR:
      segment = elf.getProgramHeaderBySectionName(".data_tcm_static_fwswnr5G")
      segment.p_vaddr = sw_fw_postlink_sym.st_value
      segment.p_paddr = sw_fw_postlink_sym.st_value
      section.sh_addr = sw_fw_postlink_sym.st_value
        


    # Update all symbols
    if symbolDict:
        print "----------------------------------------------------------------"
        print " Updating symbol values from all ELF modifications..."
        print "----------------------------------------------------------------"
        elf.updateSymbolValuesByDict(symbolDict)

    # If verification is enabled, enable verification on
    if options.verification:
        print "----------------------------------------------------------------"
        print " Verifying modified ELF data..."
        print "----------------------------------------------------------------"
        elf.verify()

    # Write out the ELF file based on the elfFile object
    if not options.debug:
        print "----------------------------------------------------------------"
        print " Writing out modified ELF..."
        print "----------------------------------------------------------------"
    elf.writeOutELF(modifiedELF)

    # Record the starting time
    if options.timing:
        print ("Execution time: %.2f seconds" % (time.time() - start_time))

    # elfManipulator ran to completed, exit with return code 0
    exit(const.RC_SUCCESS)

if __name__ == "__main__":
    main()

# vim: set ts=4 sw=4 et ai nosi:
