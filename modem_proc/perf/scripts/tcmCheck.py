##  @file       tcmCheck.py.
#   @author     rdhindsa.
#   @brief      This script prints the detailed TCM breakdown with finer granularity.
#               Assumption : TCM Linker variables with finer granularity should have one more level
#               eg __tcm_static_fw_text_start__ and __tcm_static_fw_text_common_start__
#   @version    3.0
#===============================================================================
# Copyright (c) 2017 by Qualcomm Technologies, Incorporated.
# All Rights Reserved.
# Qualcomm Technologies Proprietary/GTDR
#
#-------------------------------------------------------------------------------
#                      EDIT HISTORY FOR FILE
#
#  This section contains comments describing changes made to the module.
#  Notice that changes are listed in reverse chronological order.
#
# when      who     ver     what, where, why
# --------  ------  ----    ----------------------------------------------------
# 05/05/17  cravinar  3.0   Added check to limit the number of Overlays
# 05/16/16  rdhindsa  2.0   Added budget check for TCM sections  
# 11/16/15  rdhindsa  1.0   Created the script
#-------------------------------------------------------------------------------

# Imports
import re
import optparse
import os
import re
import sys
import time
import inspect
import xml.etree.ElementTree as et

file_path = inspect.getframeinfo(inspect.currentframe()).filename
file_dir, file_name = os.path.split(file_path)
#import elfManipulator {{{
elfManipulator_path = os.path.abspath(
os.path.join(
    file_dir,
    'elfManipulator',
    'include'))

sys.path.insert(0, elfManipulator_path)

import elfConstants as const
import elfFileClass as elfFileClass

##
# @brief    This class implements a customized exception for TCM Budgeting
class tcmBudgetError(Exception):
    def __init__ (self, expr):
        self.expr = expr

##
# @brief    Function used to raise an tcmBudgetError
# @param    errorMessage  Error message to print
def raiseTcmBudgetingError(errorMessage):
    try:
        raise tcmBudgetError(errorMessage)
    except tcmBudgetError as e:
		#Printing the error message, Exception is raised further by raise
        print "Exception raised during TCM Budgeting: " + e.expr
        raise
    
# @brief    Class for TCM budget section information
class budgetSection:
    def __init__(self):
        # Name of marker to be compared
        self.markerName = None
        # Budgeted size for section from XML
        self.budgetedMax = 0
        # Allocated size to section from ELF file
        self.allocSize = 0
        # Priority for the section, 0 for static sections
        self.priority = 0
    def setMarkerName(self, printName):
        self.printName = printName
    def getMarkerName(self):
        return self.printName
    def getBudgetedMax(self):
        return self.budgetedMax
    def setBudgetedMax(self, maxVal):
        self.budgetedMax = maxVal
    def setAllocSize(self, val):
        self.actualSize = val
    def getAllocSize(self):
        return self.actualSize
    def getPriority(self):
        return self.priority
    def setPriority(self, prio):
        self.priority = prio

        
# @brief    Class for TCM linker symbol information
class tcmLinkSymbol:
    def __init__(self,symName):
        # Name of TCM linker symbol
        self.symName = symName
        # Start address of the section represented by symbol
        self.startAddress = 0
        # End address of the section represented by symbol
        self.endAddress = 0
        # Size of the section
        self.symSize = 0
        # Hierarchical level of the TCM sub-section
        self.levelInfo = 0
    def getStartAddr(self):
        return self.startAddress
    def getEndAddr(self):
        return self.endAddress
    def getLevelInfo(self):
        return self.levelInfo
    def getSymName(self):
        return self.symName
    def getSymSize(self):
        return self.symSize
    def setStartAddr(self,startAddress):
        self.startAddress = startAddress
    def setEndAddr(self, endAddress):
        self.endAddress = endAddress
    def setLevelInfo(self,levelInfo):
        self.levelInfo = levelInfo
    def setSymSize(self, symSize):
        self.symSize = symSize
##
# @brief    Prints the TCM section sizes with finer granularity
# @param    tcmLinkSymbols      List containing information about tcm linker symbols     
# @param    tcmLinkSymDict      Dictionary containing section names and sizes
def printSectionSizes(tcmLinkSymObjList,tcmLinkSymObjDict):
    #Sorts the TCM linker symbols in increasing order of address
    #When multiple symbols are at same address, preference is given to parent level(as it has lower level value)
    tcmLinkSymObjList.sort(key=lambda x:(x.getStartAddr(),x.getLevelInfo()))
    print "-----------------------TCM SECTIONS------------------------"
    for tcmLinkSymbolObj in tcmLinkSymObjList:
        #Prints the section where it starts
        if (tcmLinkSymbolObj.getStartAddr()!=0)and (tcmLinkSymbolObj.getEndAddr()!=0):
            symName=tcmLinkSymbolObj.getSymName()
            tcmLinkSymbolObj.setSymSize(tcmLinkSymbolObj.getEndAddr() - tcmLinkSymbolObj.getStartAddr())
            #Prints if size of granular section is not zero
            if (tcmLinkSymbolObj.getSymSize())>0:
                parentSymName='_'.join((symName.split('_')[:-1]))
                #print  str(tcmLinkSymbolObj.getSymSize()),parentSymName
                if('static' in parentSymName):
                   if((tcmLinkSymbolObj.getLevelInfo()<2)or((tcmLinkSymbolObj.getLevelInfo()>=2)and (parentSymName in tcmLinkSymObjDict.keys()))):
                #Check that based on name, its parent is at one lower level
                       if((tcmLinkSymbolObj.getLevelInfo()<2)or((tcmLinkSymbolObj.getLevelInfo()>=2)and(tcmLinkSymObjDict[parentSymName].getLevelInfo())+1==tcmLinkSymbolObj.getLevelInfo())):
                           print ('\t'*tcmLinkSymbolObj.getLevelInfo()),symName,tcmLinkSymbolObj.getSymSize()
                elif('overlay' in parentSymName):
                   print ('\t'*tcmLinkSymbolObj.getLevelInfo()),symName,tcmLinkSymbolObj.getSymSize()
                else:
                   print "Mismatch with marker name of",symName,"with parent key",parentSymName
                   #return const.RC_ERROR
            else:
                print "Tuple mismatch  ",tcmLinkSymbolObj.getSymName()
                #return const.RC_ERROR
    return const.RC_SUCCESS

##
# @brief    Checks if the linker symbol is relevant TCM linker symbol for tracking TCM
# @param    symName      Name of the symbol    
# @param    symAddress      Address of the symbol
# @returns  true    If the symbol is a valid TCM linker symbol
def chkIfRelevantSymbol(symName, symAddress ):
    #Check if its a TCM linker symbol and its address is not zero
    if ('__tcm' in symName)and(symAddress!=0)and('alloc'not in symName):
        #Ensure that symbol contains start/end and text/data
        if((('start' in symName)or('end' in symName))and('rodata' not in symName)):
            if(('text' in symName)or('data' in symName)or('static_section' in symName)):
                return True
    return False

##
# @brief    Checks if the sections are within the budgets specified in XML
# @param    budgetSymObjDict    Dict containing all budget symbols with their size    
# @param    staticMax           TCM static size 
# @param    staticAlloc         If the symbol is a valid TCM linker symbol
def checkBudgets(budgetSymObjDict, staticMax, staticAlloc):
    print "STATIC_TOTAL".ljust(20), str(staticMax).ljust(20), str(staticAlloc).ljust(20)
    for symName in budgetSymObjDict.keys():
        if(budgetSymObjDict[symName].getAllocSize() > budgetSymObjDict[symName].getBudgetedMax()):
            raise raiseTcmBudgetingError(symName + ' TCM overflow')
    if(staticAlloc > staticMax):
        raise raiseTcmBudgetingError(" STATIC TCM OVERFLOW")
    
##
# @ brief    Checks if the number of overlays defined exceed limit
# @ param    elf    Elf File Class Instance
# @ param    max_overlay_sections    Extracted from tcm_config.xml
def checkOverlays(elf,max_overlay_sections):
    overlay_count = 0
    for section in elf.sectionHeaderTable:
        if 'overlay' in section.sh_nameStr and re.search('\d+', section.sh_nameStr):
            overlay_count+=1
    if(overlay_count > max_overlay_sections):
        print "ERROR: Exceeded expected number of overlay sections.\nBUDGET : %d\t DETECTED : %d" % (max_overlay_sections, overlay_count)
        raiseTcmBudgetingError()
##
# @brief    Parse the tcm_config.xml file in the config directory
# @param    budgetSymObjDict    Dict containing budget symbols as key and object as value    
# @param    root                Element to be processed
# @param    section             Represents section attribute in XML, value : 'section' or 'subSection'
# @param    sectionName         Section Name : appending name on level basis 
# @param    tcmLinkSymObjDict   Dict containing sections based on markers
def parseTcmConfigXml(budgetSymObjDict, root, section, sectionName, tcmLinkSymObjDict):
    for child in root.iter(section):
        name = sectionName
        tcmSubSection = child.attrib['subSection']
        if name =="":
            name = child.attrib['name']
        else:
            name = name +":"+child.attrib['name']
        if(tcmSubSection == "true"):
            parseTcmConfigXml(budgetSymObjDict, child, 'subSection', name, tcmLinkSymObjDict)
        # Process if the section is budgeted
        if (child.attrib['budgeted']=="true"):
            tcmType = child.attrib['tcmType']
            budgetSymObj = budgetSection()
            if(tcmType=="OVERLAY"):
                overlayTag = child.find('Overlay')
                processOverlay(overlayTag, budgetSymObj)
            isSectionInElf = processBudget(child.find('Budget'), tcmLinkSymObjDict, budgetSymObj)
            if isSectionInElf==True:
                budgetSymObjDict[name] = budgetSymObj
                print name.ljust(20), str(budgetSymObj.getBudgetedMax()).ljust(20), str(budgetSymObj.getAllocSize()).ljust(20)

##
# @brief    Processes the overlay element in XML
# @param    tagName         Overlay tag to be processed
# @param    budgetSymObj    Dict containing the budget symbol to be updated with budget info
def processOverlay(tagName, budgetSymObj):
    prio = tagName.find('Priority').text
    budgetSymObj.setPriority(int(prio))    

##
# @brief    Processes Budget Element extracting budget from XML and actual size from tcmLinkSymObjDict dictionary
# @param    tagName             Budget tag to be processed
# @param    tcmLinkSymObjDict   Dict containing sections based on markers from elf
# @param    budgetSymObj        Dict containing the budget symbol to be updated with budget
# @returns  True                If the respective marker is in the Elf and needs budgeting  
# @todo     Revisit replace function calls while extracting markers
# @todo     Consider using the section header table instead of the symbol table wherever possible
def processBudget(tagName, tcmLinkSymObjDict, budgetSymObj): 
    # Getting budget information from XML
    budgetType = tagName.find('BudgetType').text
    budgetMarker = tagName.find('Marker').text
    budgetSize = tagName.find('BudgetSize').text
    markerName= budgetMarker.replace('__tcm_','').replace('_max_size__','').replace('_reserved__','')
    symNamList = markerName.split('_')
    if (budgetType!='textdata'):
        if(markerName not in tcmLinkSymObjDict.keys()):
            return False
    else:
        text_marker = '_'.join(symNamList[0:-1])+'_text_'+''.join(symNamList[-1])
        data_marker = '_'.join(symNamList[0:-1])+'_data_'+''.join(symNamList[-1])
        if((text_marker not in tcmLinkSymObjDict.keys())and(data_marker not in tcmLinkSymObjDict.keys())):
            return False
    if (('K' in budgetSize)or('k' in budgetSize)):
        budgetSize = budgetSize.replace('k','').replace('K','')
        budgetSize = int(budgetSize)
        budgetSize = budgetSize*1024
    else:
        budgetSize = int(budgetSize)
    # Extracting actual size based on type of budget
    if(budgetType=='textdata'):
        actualSize = 0
        if text_marker in tcmLinkSymObjDict.keys():
            actualSize += tcmLinkSymObjDict[text_marker].getSymSize()
        if data_marker in tcmLinkSymObjDict.keys():
            actualSize += tcmLinkSymObjDict[data_marker].getSymSize()
    else:
        actualSize = tcmLinkSymObjDict[markerName].getSymSize()
    # Updating the budget information in the object
    budgetSymObj.setBudgetedMax(int(budgetSize))   
    budgetSymObj.setAllocSize(actualSize)
    budgetSymObj.setMarkerName(markerName)
    return True
    
##
# @brief    Main entry point
def main():
    # Use optparse to set up usage
    global verboseFlag
    use = "Usage: python %prog [options] <Input ELF> <XML path>"
    parser = optparse.OptionParser(usage = use)
    options, arguments = parser.parse_args()
    if(len(arguments)<2):
        print("Less than required number of arguments input, please check script usage\nExiting......")
        exit(1)#Dirty Exit
 
    # Get the ELF Path
    elfPath = arguments[0]
    xmlPath = arguments[1]

    elf = elfFileClass.elfFile(elfPath)
    #Dictionary of symbol name as key and TCM link symbol object as the associated value 
    tcmLinkSymObjDict ={}
    #List containing all the TCM link symbol object, and used to eventually sort them based on starting addresses
    tcmLinkSymObjList=[]
    symtab = elf.getSectionByName(".symtab")
    strtab = elf.getSectionByName(".strtab")
    if (symtab != const.RC_ERROR and strtab != const.RC_ERROR):
        for symbol in elfFileClass.Elf32_SymGenerator(symtab, strtab):
            # For TCM linker symbols, saves their name, address,and level 
            if(chkIfRelevantSymbol(symbol.st_nameStr,int(str(symbol.st_value)))):
                symName = symbol.st_nameStr.replace('__tcm_','').replace('_start__','').replace('_end__','')
                # If first linker symbol corresponding to symName, create a tcmLinkSymbol object and save the object to Dictionary and List
                if (symName not in tcmLinkSymObjDict.keys()):
                    tcmLinkSymbolObj =tcmLinkSymbol(symName)
                    tcmLinkSymObjDict[symName] = tcmLinkSymbolObj
                    tcmLinkSymObjList.append(tcmLinkSymbolObj)
                # If already an object is associated, access the object to update its members
                else:
                    tcmLinkSymbolObj = tcmLinkSymObjDict[symName]
                numLevels = len(symName.split('_'))
                # Save level info, start or end addresses for the objects
                if 'static' in symName:
                    tcmLinkSymbolObj.setLevelInfo(numLevels - 2)  
                elif 'overlay' in symName:
                    tcmLinkSymbolObj.setLevelInfo(numLevels - 2)  
                if ('start' in symbol.st_nameStr):
                    tcmLinkSymbolObj.setStartAddr(symbol.st_value)
                elif ('end' in symbol.st_nameStr):
                    tcmLinkSymbolObj.setEndAddr(symbol.st_value)
    retVal = printSectionSizes(tcmLinkSymObjList,tcmLinkSymObjDict)
    if(retVal == const.RC_ERROR):
        print "TUPLE MISMATCH"
    #Dictionary of symbol name as key and Budget object as the associated value 
    budgetSymObjDict ={}
    print '-------------------------Budgeted SECTIONS------------------------------------'
    tree = et.parse(xmlPath)
    root = tree.getroot()
    parseTcmConfigXml(budgetSymObjDict, root, 'section','', tcmLinkSymObjDict)
    #print budgetSymObjDict
    maxStaticSize = root.attrib['static_section']
    if (('K' in maxStaticSize)or('k' in maxStaticSize)):
        maxStaticSize = int(maxStaticSize.replace('k','').replace('K',''))
        maxStaticSize = maxStaticSize*1024
    else:
        maxStaticSize = int(maxStaticSize)
    #Added by Shilpa - Limit overlays change, call checkOverlays only if overlay_count is defined in tcm_config.xml
    if "overlay_count" in root.attrib:
        maxOverlayCount = int(root.attrib['overlay_count'])
        checkOverlays(elf, maxOverlayCount)
    if 'static_section' in tcmLinkSymObjDict:
        checkBudgets(budgetSymObjDict, maxStaticSize,tcmLinkSymObjDict['static_section'].getSymSize())
    #exit(0) #Clean Exit
           
if __name__ == "__main__":
    main()


