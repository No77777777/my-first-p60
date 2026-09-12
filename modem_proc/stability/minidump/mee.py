#!/usr/bin/env python
'''mee.py - minidump elf editor 

Edit contents of minidump related variables in elf. 
===============================================================================

 mee.py  - Minidump Elf Editor

 GENERAL DESCRIPTION
    This script contains code for populating contents of 'minidump_var_tab' 
    during compilation time.  The input for this script will be read from
    minidump.xml file.
-------------------------------------------------------------------------------

  $Header: //components/rel/stability.mpss/2.4/minidump/mee.py#6 $
  $DateTime: 2020/06/25 23:38:58 $

===============================================================================


'''
import sys, os, operator
import argparse
import inspect

sys.path.append(os.path.abspath(os.path.dirname(sys.argv[0])))
minidump_dir = os.path.dirname(inspect.getframeinfo(inspect.currentframe()).filename)
elftoolspath = os.path.join(minidump_dir, '..', '..', 'perf', 'scripts')

sys.path.append(elftoolspath)
import mc,re
import collections
import dynRec_manip as editor
import elfFileClass as elfFileClass
# import xmltodict
from xml.etree import ElementTree as et
from xml.dom import minidom
from collections import defaultdict
import elfConstants as const
import elfStructs as structs
import time

MAX_BLOB_ENTRIES = 0x18
BLOB_ENTRY_SIZE = 0xC
SIZE_B4_BLOB_ENTRY = 0x10

def Elf32_SymGenerator(symtab, strtab):
	assert len(symtab.contents) % symtab.sh_entsize == 0
	for i in xrange(0, len(symtab.contents), symtab.sh_entsize) :
		yield structs.Elf32_Sym(symtab.contents[i : i + symtab.sh_entsize], strtab)

class minidump_elf_editor:
	def __init__(self, elf_pfn, xml_pfn, custxml_pfn):
		if os.path.exists(elf_pfn):
		    self.elf_handle = elfFileClass.elfFile(elf_pfn)
		    self.elf_pfn = elf_pfn
		else:
		    raise Exception("mee.__init__(): ELF path not accessible")
		if os.path.exists(xml_pfn):
		    self.xml_pfn = xml_pfn
		    self.xml_tree = et.ElementTree(file=xml_pfn)
		    self.xml_root = self.xml_tree.getroot()
		    logfile = os.path.join(os.path.dirname(xml_pfn), 'minidump_elf_editor.log')
		    self.log = open(logfile, 'w')
#            cmmfile = os.path.join(os.path.dirname(xml_pfn), 'minidump_elf_loader.cmm')
#            self.cmm =open(cmmfile,'w')
		else:
		    raise Exception("mee.__init__(): XML paths not accessible")
		if os.path.exists(custxml_pfn):
		    self.custxml_pfn = custxml_pfn
		    self.custxml_tree = et.ElementTree(file=custxml_pfn)
		    self.custxml_root = self.custxml_tree.getroot()
		else:
		    raise Exception("mee.__init__(): Cust Config XML paths not accessible")

		self.vid = dict()  # initialize variable_info_dictionary
		self.ss_sz_limit = dict()  # initialize dictionary to track  area size limits
		# In minidump_info struct, the minidump_var_tab starts after the heap_map(which is nothing but entries of blobs)
		# offset to minidump_var_tab
		self.offset_var_tab = SIZE_B4_BLOB_ENTRY + (MAX_BLOB_ENTRIES * BLOB_ENTRY_SIZE)
		# Type3 for variables larger than 200000 Bytes
		self.type_3_thres = 130000
		self.swapped_start_string = '__swapped_segments_start__'
		self.swapped_text_start_string = '__swapped_segments_text_start__'
		self.swapped_text_end_string = '__swapped_segments_text_end__'
		self.ro_start_string = '__swapped_segments_rodata_start__'
		self.ro_end_string = '__swapped_segments_rodata_end__'
		self.swapped_end_string = '__swapped_segments_end__'
		self.log.write("mee.__init__(): ELF path: " + self.elf_pfn + "\n")
		self.log.write("mee.__init__(): minidumpXML path: " + xml_pfn + "\n")
		self.regvid = dict()
		#to handle type7 variables which are contiguous and have same sub string 
		#example is all the static stack variables start with rcinit_stk_
		self.regext7VarsDict = dict()
		self.regext7VarsDictCount = 0

	def add_variable_to_elf(self):
	    minidump_sym = self.elf_handle.getSymbolByName("minidump_info")
	    ret = editor.setDataByAddress(self.elf_handle, minidump_sym.st_value + 24, 4, 3297706648)
	    self.log.write(minidump_sym.st_value + "\n")
	    self.log.write(ret + "\n")

	def write_to_elf(self, elf_pfn=None):
	    if elf_pfn:
	        self.log.write("mee.write_to_elf(): writing to " + elf_pfn + "\n")
	        self.elf_handle.writeOutELF(elf_pfn)
	    else:
	        self.log.write("mee.write_to_elf(): writing to " + self.elf_pfn + "\n")
	        self.elf_handle.writeOutELF(self.elf_pfn)

###
# @brief :  *  This function XPath searches for all the elements with interested attribute and
#              removes it from parent elements.
#           *  Once the necessary removal is done , root will be cleaned from interested attributes
#
#
#
	def remove_need_drop_attributes(self, attr_string, op, for_chipset):

	    # check for all parents with attr_string
	    pr_a_l = self.xml_root.findall('.//*[@%s]/..' % attr_string)      #parents list which have attr_string
	    pr_a_l = list(set(pr_a_l))

	    # remove variables that have attr_sting :
	    #  * value == chipset  in case of drop_only_in
	    #  * value != chipset  in case of need_only_in
	    for pr in pr_a_l:
	        ele_l = pr.findall('*[@%s]' % attr_string)
	        for el in ele_l:
	            if (op(el.attrib[attr_string], for_chipset)):
	                pr.remove(el)
	            del el.attrib[attr_string]  # clean up this attribute


###
# @brief : Calls subfunctions to remove the elements with intereseted attributes
#
#
# @todo : To handle 'need_only_in' or 'drop_only_in" elements , currently supports only attributes
#
	def clean_for_chipset(self, for_chipset):

	    if for_chipset:
	        self.log.write("mee.clean_for_chipset(): Filtering xml input for "+for_chipset+"\n\n\n")
	    self.remove_need_drop_attributes('need_only_in', operator.ne, for_chipset)
	    self.remove_need_drop_attributes('drop_only_in', operator.eq, for_chipset)



        ###

    # @brief :
    #
    # @todo : How to handle subvariables like lte_ml1_ulm_scc_status_update_req.scc_cfg_recfg_mask
    #

	def read_var_details_from_xml(self, in_file, in_ss=None, for_chipset=None):
	    self.log.write("mee.read_var_details_from_xml(): Reading vars from xml  \n\n\n")
	    size_ar = []  # size array to keep track of size
	    as_idx = 0
	    rm_list = []
	    if in_file and (os.path.exists(in_file)):
	        if in_ss is None:
	            in_ss = os.path.basename(in_file).split('.')[0]

	    self.clean_for_chipset(for_chipset)
	    variable_info = self.xml_root.find('variable_info')

	    for each_area in variable_info.getchildren():
	        area_name = each_area.attrib['name']
	        # self.log.write("mee.read_var_details_from_xml(): In area " + area_name + " \n")
	        size_ar.append([])
	        for each_subsys in each_area.getchildren():
	            subsys_name = each_subsys.attrib['name']
	            self.log.write("mee.read_var_details_from_xml(): reading subsystem  " + subsys_name + " \n")
	            if in_ss and (subsys_name.upper() == in_ss.upper()):
	                self.log.write("mee.read_var_details_from_xml(): Filling from tin\n")
	                fill_xml_from_text_input(in_file, each_subsys)

	            # self.log.write("mee.read_var_details_from_xml(): In subsys "+subsys_name+ "\n")
	            if 'size_limit' in each_subsys.attrib.keys():
	                self.ss_sz_limit[subsys_name] = int(each_subsys.attrib['size_limit'], 16)
	            ss_size = 0
	            for each_var in each_subsys.getchildren():
	                variable_name = each_var.find('var_name').text.strip()
	                var_type = int(each_var.find('var_type').text.strip(), 16)
	                # self.log.write("mee.read_var_details_from_xml(): processing variable: "+variable_name+"\n")
	                if variable_name not in self.vid.keys():
						#check if the variable is a regex var of type 7
						each_var.find('var_name').text = variable_name
						if(var_type == 7):
							self.regvid[variable_name] = dict()
							self.regvid[variable_name]['area'] = area_name
							self.regvid[variable_name]['subsys'] = subsys_name
							self.regvid[variable_name]['type'] = var_type
							self.regvid[variable_name]['xml_ele']=each_var
						else:
						    self.vid[variable_name] = dict()
						    self.vid[variable_name]['area'] = area_name
						    self.vid[variable_name]['subsys'] = subsys_name
						    self.vid[variable_name]['xml_ele'] = each_var
	                else:
	                    if variable_name:
	                        self.log.write(
	                            "mee.read_var_details_from_xml(): duplicate variable: " + variable_name + " ; Discarded \n")
	                    else:
	                        self.log.write(
	                            "mee.read_var_details_from_xml(): Unknown/Empty variable found in xml. Discarding..\n")
	                    rm_list.append(each_var)
	            for rm_item in rm_list:
	                each_subsys.remove(rm_item)
	            rm_list = []
	        # size_ar[as_idx].append(ss_size)
	        # each_subsys.attrib['size']=format(ss_size,'#010x')
	        # a_size = sum(size_ar[as_idx])
	        # each_area.attrib['size'] = format(a_size,'#010x')
	        # as_idx = as_idx+1
	        # total size caluclation
	        # total_var_size = sum( [ sum(x) for x in size_ar ] )

    ###
    # @brief :
    #
    # @todo : check to see if the size of a variable increases more than 3 Bytes
    #
	def calc_mem_size(self):
	    rm_list = []
	    size_dict = defaultdict(lambda: defaultdict(int))
	    size_ar_dict = defaultdict(int)
	    tot_sz = 0
	    for each_var in self.vid:
	        ar = self.vid[each_var]['area']
	        sb = self.vid[each_var]['subsys']
	        ev_size = self.vid[each_var]['size']
	        if ev_size > self.type_3_thres:
	            # need not consider this for total minidump size
	            continue
	        if sb in self.ss_sz_limit:
	            if (size_dict[ar][sb] + ev_size) > self.ss_sz_limit[sb]:
	                self.log.write("mee.calc_mem_size:SS limit exceeded. Discarding variable " + each_var + "\n")
	                self.vid[each_var]['rm_list'] = True
	                rm_list.append(each_var)
	                continue

	        tot_sz += ev_size  # adding each_var 's size to total size
	        size_ar_dict[ar] += ev_size  # adding each_var 's size to size_area dictionary
	        size_dict[ar][sb] += ev_size  # adding each_var 's size to size dictionary

	    # writing the memory sizes to xml
	    for each_area in size_dict:
	        elem = self.find_xml_element_from_area(each_area)
	        elem.attrib['size'] = format(size_ar_dict[each_area], '#08x')
	        for each_subsys in size_dict[each_area]:
	            elem = self.find_xml_element_from_area_subsys(each_area, each_subsys)
	            elem.attrib['size'] = format(size_dict[each_area][each_subsys], '#08x')
	    self.remove_elements_from_rm_list(rm_list)

	    return tot_sz

    ###
    # @brief :
    #           * checks for minidump_size elements
    #           * if more than 1 elements found:
    #                   * check if chipset is in the attribute_list ; if found thats the size needed
    #                   * if chipset is not there in attribute list , then deault minidump_size is considered
    #                   * delete all other minidump_size elements
    #           * else
    #                   * consider this minidump_size element and clean any attributes
    #
	def get_minidump_size(self,for_chipset):
	    ret = 0
	    msel_l  = self.xml_root.findall('.//minidump_size')          #list of memory size elements
	    pr_msel_l  = self.xml_root.findall('.//minidump_size/..')     #parent of memory size element
	    if len(pr_msel_l)==1:
	        pr_msel = pr_msel_l[0]
	    else :
	        raise Exception("mee.get_minidump_size(): Error, minidump_size element should be defined only in meta_info ")
	    if len(msel_l)>1:
	        chip_l = [x.attrib['chipset'] for x in msel_l if 'chipset' in x.attrib]
	        if for_chipset in chip_l:
	            msel = self.xml_root.find(".//minidump_size/[@chipset='%s']"%for_chipset)
	            ret = int(msel.text,16)
	        for x in msel_l:
	            if ('chipset'not in x.attrib) and (ret==0):       #check for default minidump_size element
	                ret = int(x.text ,16)
	            elif ('chipset' in x.attrib) and (x.attrib['chipset']==for_chipset) :
	                del x.attrib['chipset']
	            else :
	                pr_msel.remove(x)
	    else :
	        msel = msel_l[0]
	        ret = int(msel.text,16)

	    return ret



	def read_and_sanitize_xml(self, in_file, in_ss, for_chipset):

	    self.read_var_details_from_xml(in_file, in_ss, for_chipset)
	    ret = self.read_var_details_from_elf()
	    if (ret != 0):
	        return ret
	    total_var_size = self.calc_mem_size()
	    meta_data = self.xml_root.find('metadata_info')
	    if for_chipset:
	        chpst_ele = meta_data.find('chipset')
	        chpst_ele.text = for_chipset
	    minidump_size = self.get_minidump_size(for_chipset)
	    self.log.write("mee.read_and_sanitize_xml():total_var_size "+str(hex(total_var_size))+"\n")
	    #minidump_size = int(meta_data.find('minidump_size').text, 16)
	    if total_var_size > (minidump_size - self.offset_var_tab):
	        raise Exception("mee.read_and_sanitize_xml(): aggregated variables size is greater than minidump size")
	    self.log.write("mee.read_and_sanitize_xml(): updating minidump.xml \n")
	    txt = et.tostring(self.xml_root)
	    txt = minidom.parseString(txt).toprettyxml(indent='  ')
	    txt = '\n'.join([each_line for each_line in txt.splitlines() if each_line.strip()])
	    xml_out_pfn = os.path.join(os.path.dirname(self.xml_pfn), 'autogen_minidump.xml')
	    xml_out = open(xml_out_pfn, 'w')
	    xml_out.write(txt)
	    xml_out.close()
	    return 0

	def is_var_not_in_ro(self, addr):
		if not (self.ro_start == 0 and self.ro_end == 0):
			if((addr >= self.ro_start.st_value) and (addr <= self.ro_end.st_value)):
				return 0

		if not (self.swapped_text_start == 0 and self.swapped_text_end == 0):
			if((addr >= self.swapped_text_start.st_value) and (addr <= self.swapped_text_end.st_value)):
				return 0

		return 1

	def is_var_in_comprs(self, addr):
		if((self.swapped_start == 0 and self.swapped_end == 0)):
			return 0
		if ((addr >= self.swapped_start.st_value) and (addr <= self.swapped_end.st_value)):
			return 1
		else:
			return 0

	def dict_addr_size_items_contiguous(self, sd):
		#sd stands for sorted dictionary by ascending order of addr
		dcnt =0
		#Address is 16bit aligned. Need to check this when new regex var gets added
		align_addr = 0
		
		#number of items in dict
		dlen = len(sd)
		for addr,sz in sd.items():
			if (dcnt+1) < dlen:
				align_addr = sz + addr
				if((align_addr & 0xF) != 0):
					align_addr_new = (align_addr | 0xF) + 1
					sd[addr] = sz+(align_addr_new-align_addr)
					align_addr = align_addr_new
				if not(align_addr == sd.keys()[dcnt+1]):
					self.regext7VarsDictCount = dcnt+1
					self.log.write("dict_addr_size_items_contiguous self.regext7VarsDictCount is "+str(hex(self.regext7VarsDictCount))+"\n")
					return 0
					#self.log.write("dict does not have cont locations "+str(hex(addr)))
					#return 1
			dcnt = dcnt + 1
		#All items in the sorted dict are in contiguous memory locations
		self.regext7VarsDictCount = dcnt+1
		return 0


	def read_custxml_write_image_size_to_elf(self):
	    self.log.write("mee.read_custxml_write_image_size_to_elf() : reading cust xml and writing img size \n \n")

	    for pool in self.custxml_root.findall('physical_pool'):
	        if (pool.get('name') == 'DEFAULT_PHYSPOOL'):
	            for child in pool.findall('region'):
	                image_size = child.get('size')

	    img = self.elf_handle.getSymbolByName("ts_md_image_size")
	    if (img == const.RC_ERROR):
	        self.log.write("no ts_md_image_size variable found in the elf - ddr size not updated \n")
	    else:
	        address = img.st_value
	        print "read custxml address is ", address
	        ret = editor.setDataByAddress(self.elf_handle, address, 4, int(image_size,16))
	        if (ret != 0):
	            raise Exception("mee.write_minidump_var_tab_entries_to_elf(): Error while updating the elf for image size \n")


	def write_minidump_var_tab_entries_to_elf(self):
	    self.log.write("mee.write_minidump_var_tab_entries_to_elf(): Writing var details to ELF  \n\n\n")
	    # minidump_sym = self.elf_handle.getSymbolByName("minidump_info")
	    #        self.init_cmm()
	    address = self.minidump_sym.st_value + self.offset_var_tab
	    num_entries = (self.minidump_sym.st_size - self.offset_var_tab) / 8
	    entries_captured = 0
	    entries_skipped = 0
	    end_list = list()
	    end_list_count = 0

	    for each_var in self.vid:
	        if entries_captured < (num_entries - 1):
	            v_address = self.vid[each_var]['address']
	            v_size = self.vid[each_var]['size']
	            v_type = self.vid[each_var]['type']
	            ty_sz_str = '{0:02x}{1:06x}'.format(v_type, v_size)
	            ty_sz = int(ty_sz_str, base=16)
	            if self.is_var_in_comprs(v_address) == 0:
	                self.log.write("write_minidump_var_tab_entries_to_elf : "+str(hex(v_address))+"\n")
	                ret1 = editor.setDataByAddress(self.elf_handle, address, 4, v_address)
	                ret2 = editor.setDataByAddress(self.elf_handle, address + 4, 4, ty_sz)
	                address = address + 8
	                if ((ret1 != 0) or (ret2 != 0)):
	                    raise Exception("mee.write_minidump_var_tab_entries_to_elf(): Error while updating the elf \n")
	            else:
	                end_list_count += 1
	                end_list.append((v_address, ty_sz))

	        else:
	            entries_skipped += 1
	            self.log.write(
	                "mee.write_minidump_var_tab_entries_to_elf(): maximum number of entries reached. Skipping " + each_var + "\n")
	        entries_captured += 1
	    self.log.write(
	        "mee.write_minidump_var_tab_entries_to_elf(): Writing addresses of COMPRESSED variables (at the end) \n")
	    for i in range(end_list_count):
			if entries_captured < (num_entries-1):
				(v_address, ty_sz) = end_list.pop()
				ret1 = editor.setDataByAddress(self.elf_handle, address, 4, v_address)
				ret2 = editor.setDataByAddress(self.elf_handle, address + 4, 4, ty_sz)
				address = address + 8
				if ((ret1 != 0) or (ret2 != 0)):
				    raise Exception("mee.write_minidump_var_tab_entries_to_elf(): Error while updating the elf(End) \n")
				else:
					entries_captured += 1


	def get_symbol_from_symbol_list(self, sym_list, sym_name_list, symbol):
	    if symbol in sym_name_list:
	        for sym in sym_list:
	            if sym.st_nameStr == symbol:
	                return sym
	    else:
	        return 0


	def add_regex_type_var(self, symb):
		
		for varStr in self.regvid.keys():
			if(re.search(varStr,symb.st_nameStr) != None):
				if(self.regvid[varStr]['type'] == 7):
					if varStr not in self.regext7VarsDict.keys():
						self.regext7VarsDict[varStr] = dict()
					self.regext7VarsDict[varStr][symb.st_nameStr] = symb
					self.log.write("add_regex_type_var : symb.st_nameStr is  "+symb.st_nameStr+"\n")
#					print symb.st_nameStr
					return 1
		return 0

	def create_new_var_xml_element(self, area_name, subsys_name, symb):
		#Create a new element
		newElemVar = et.Element("var")
		newElemVarname = et.Element("var_name")
		newElemVarname.text = symb.st_nameStr
		newElemVarsize = et.Element("var_size")
		newElemVarsize.text = str(hex(symb.st_size))
		newElemVartype = et.Element("var_type")
		newElemVartype.text = "0"
		newElemVar.append(newElemVarname)
		newElemVar.append(newElemVarsize)
		newElemVar.append(newElemVartype)
		
		#add xml element to the subsystem
		variable_info=self.xml_root.find('variable_info')
		for each_area in variable_info.getchildren():
			if (each_area.attrib['name'] == area_name) :
				for each_subsys in each_area.getchildren():
					if (each_subsys.attrib['name'] == subsys_name) :
						self.log.write("Adding XML element to "+subsys_name+" with name "+symb.st_nameStr+"\n")
						each_subsys.append(newElemVar)
						return newElemVar

		return None

	def process_regex_type7_vars(self, sym_list_e, sym_name_list_e):

		for varStr in self.regvid.keys():
			if(len(self.regext7VarsDict[varStr].keys()) == 0):
				continue
			self.log.write("process_regex_type7_vars for "+varStr+"\n")	
			uncomprVarsDict = dict()
			ret_uncompr = 1
			compr_start_addr = 0
			compr_size = 0
			uncompr_start_addr = 0
			uncompr_size = 0
			
			#segregate the compr and uncompr static stacks
			for sym in self.regext7VarsDict[varStr].values():
				if self.is_var_not_in_ro(sym.st_value) == 1:
					if (self.is_var_in_comprs(sym.st_value) == 1):
						#Cannot support compressed static stacks as continuous blob
						#q6zip can only decompress 4K at once. 
						#compr region when uncomprssed is not in contiguous PA locations in DDR
						newElem = self.create_new_var_xml_element(self.regvid[varStr]['area'], self.regvid[varStr]['subsys'], sym)
						if(newElem is not None):
							sym_list_e.append(sym)
							sym_name_list_e.append(sym.st_nameStr)
							self.vid[sym.st_nameStr] = dict()
							self.vid[sym.st_nameStr]['area'] = self.regvid[varStr]['area']
							self.vid[sym.st_nameStr]['subsys'] = self.regvid[varStr]['subsys']
							self.vid[sym.st_nameStr]['xml_ele']=newElem
					else:
						uncomprVarsDict[sym.st_value] = sym.st_size

			if(len(uncomprVarsDict.keys()) > 0):
				oucd = collections.OrderedDict(sorted(uncomprVarsDict.items())) 
				iterator = 0
				counter = 0
				while(len(oucd) != 0):
					ret_uncompr = self.dict_addr_size_items_contiguous(oucd)
					uncompr_start_addr = oucd.keys()[0]
					uncompr_size = sum(oucd.values()[0:(self.regext7VarsDictCount)])
					self.log.write("process_regex_type7_vars uncompr_start_addr  "+str(hex(uncompr_start_addr))+"  uncompr size is  "+str(hex(uncompr_size))+"\n")
					#create temporary ELF symbols to add to sym_list_e
					#Add names to sym_name_list_e. These should match with names in self.vid.keys()
					if (ret_uncompr == 0):
						self.log.write("varStr is "+varStr+"reg ext count is "+str(hex(self.regext7VarsDictCount))+"\n")
						uncompr_sym = self.regext7VarsDict[varStr].values()[counter]
						uncompr_sym.st_nameStr = varStr+"uncompr"+str(iterator)
						uncompr_sym.st_value = uncompr_start_addr
						uncompr_sym.st_size = uncompr_size
						self.log.write("name string is  "+uncompr_sym.st_nameStr+"  value is  "+str(hex(uncompr_start_addr))+"  size is  "+str(hex(uncompr_size))+"\n")
						sym_list_e.append(uncompr_sym)
						sym_name_list_e.append(uncompr_sym.st_nameStr)
						self.vid[uncompr_sym.st_nameStr] = dict()
						self.vid[uncompr_sym.st_nameStr]['area'] = self.regvid[varStr]['area']
						self.vid[uncompr_sym.st_nameStr]['subsys'] = self.regvid[varStr]['subsys']
						if (iterator == 0):
							self.vid[uncompr_sym.st_nameStr]['xml_ele'] = self.regvid[varStr]['xml_ele']
						else:
							self.vid[uncompr_sym.st_nameStr]['xml_ele'] = self.create_new_var_xml_element(self.vid[uncompr_sym.st_nameStr]['area'],self.vid[uncompr_sym.st_nameStr]['subsys'],uncompr_sym)
							if(self.vid[uncompr_sym.st_nameStr]['xml_ele'] is None):
								self.log.write("process_regex_type7_vars : Could not create a new xml ele for reg ext var \n")
						each_uncompr_var = self.vid[uncompr_sym.st_nameStr]['xml_ele']
						each_uncompr_var.find('var_name').text=uncompr_sym.st_nameStr
						self.log.write("process_regex_type7_vars print the dictionary \n")
						for key in [key for key in oucd if key < (uncompr_start_addr + uncompr_size)]: 
							del oucd[key]
							counter = counter + 1
						self.log.write("process_regex_type7_vars print the updated dictionary \n")
						iterator = iterator + 1



	def read_var_details_from_elf(self):
		self.log.write("mee.read_var_details_from_elf(): Reading var details from ELF  \n\n\n")
		req_var_list = self.vid.keys()
		req_var_list.append(self.swapped_start_string)
		req_var_list.append(self.swapped_end_string)
		req_var_list.append(self.ro_start_string)
		req_var_list.append(self.ro_end_string)
		req_var_list.append(self.swapped_text_start_string)
		req_var_list.append(self.swapped_text_end_string)
		req_var_list.append('minidump_info')

		sym_list_e = []

		# Make the var List unique
		req_var_list = list(set(req_var_list))

		#Check if there any regex type variables
		lenregexvar = len(self.regvid.keys())
		
		#Go through the ELF and get the symbols for all the vars
		symtab = self.elf_handle.getSectionByName(".symtab")
		strtab = self.elf_handle.getSectionByName(".strtab")
		if (symtab != const.RC_ERROR and strtab != const.RC_ERROR):
			for symbol in Elf32_SymGenerator(symtab, strtab):
				# get the symbols using regular expression
				if (lenregexvar > 0) and (self.add_regex_type_var(symbol) == 1) :
					continue
				else:
					if (symbol.st_nameStr in req_var_list) and (symbol.st_value != 0):
						sym_list_e.append(symbol)
		
			sym_name_list_e = [each_sym.st_nameStr for each_sym in sym_list_e]

			rm_list = []
			self.swapped_start = self.get_symbol_from_symbol_list(sym_list_e, sym_name_list_e, self.swapped_start_string)
			self.swapped_end = self.get_symbol_from_symbol_list(sym_list_e, sym_name_list_e, self.swapped_end_string)
			self.ro_start = self.get_symbol_from_symbol_list(sym_list_e, sym_name_list_e, self.ro_start_string)
			self.ro_end = self.get_symbol_from_symbol_list(sym_list_e, sym_name_list_e, self.ro_end_string)
			self.swapped_text_start = self.get_symbol_from_symbol_list(sym_list_e,sym_name_list_e,self.swapped_text_start_string)
			self.swapped_text_end   = self.get_symbol_from_symbol_list(sym_list_e,sym_name_list_e,self.swapped_text_end_string)
			self.minidump_sym = self.get_symbol_from_symbol_list(sym_list_e, sym_name_list_e, 'minidump_info')

			if (self.ro_start == 0) and (self.ro_end == 0):
				# minidump_sym = self.elf_handle.getSymbolByName("minidump_info")
				if type(self.minidump_sym) is int and (self.minidump_sym == 0):
				    self.log.write("Skipping minidump for No Modem Variant")
				    return 1

			#Process Regex variables of different types here
			if (lenregexvar > 0):
				#currently type7 vars are supported which are contiguous in compressed/uncompressed mem regions 
				if(len(self.regext7VarsDict.keys()) > 0):
					self.process_regex_type7_vars(sym_list_e, sym_name_list_e)

			for each_sym in self.vid.keys():
				if each_sym in sym_name_list_e:
					sym_e = [sym for sym in sym_list_e if sym.st_nameStr == each_sym][0]
					elem = self.vid[each_sym]['xml_ele']
					if ('rm_list' in self.vid[each_sym]) and self.vid[each_sym]['rm_list']:
					    rm_list.append(each_sym)
					    continue
					self.vid[each_sym]['address'] = sym_e.st_value
					self.vid[each_sym]['size'] = sym_e.st_size
					if (sym_e.st_size > 16777215):
					    raise Exception("mee.read_var_details_from_elf():More than max allowed size , 0xFFFFFF")
					elif self.is_var_not_in_ro(sym_e.st_value) == 0:
					    # skipping variables in swapped_ro section
					    # skipping variables in swapped_text_section
					    self.log.write("Discarding RO/text section varaible :" + each_sym)
					    rm_list.append(each_sym)
					elif (sym_e.st_size > self.type_3_thres):
					    self.vid[each_sym]['type'] = 3
					    elem.find('var_type').text = format(self.vid[each_sym]['type'], '#0x')
					    elem.find('var_size').text = format(self.vid[each_sym]['size'], '#08x')
					else:
					    self.vid[each_sym]['type'] = 0
					    elem.find('var_type').text = format(self.vid[each_sym]['type'], '#0x')
					    elem.find('var_size').text = format(self.vid[each_sym]['size'], '#08x')
				else:
				    # del self.vid[each_sym]
				    # Ideally this element should be deleted in xml , but for now updating just size as 0

				    # elem.find('var_size').text = format(0, '#010x')
				    self.log.write("mee.read_var_details_from_elf(): not found :"+each_sym + " ; Skipped and Discarding for xml\n")
				    rm_list.append(each_sym)  # adding  to remove list

			self.remove_elements_from_rm_list(rm_list)
			return 0
		else:
			self.log.write("--------Error symtab or strtab entries not found--------");
			return 1


	def remove_elements_from_rm_list(self, rm_list):

	    for s in rm_list:
	        elem = self.vid[s]['xml_ele']
	        # elem   = self.find_xml_element_from_var_name(s)
	        pr_ele = self.find_xml_element_from_area_subsys(self.vid[s]['area'], self.vid[s]['subsys'])
	        pr_ele.remove(elem)
	        del self.vid[s]  # iterate through rm_list and delete them for var_dict

	def find_xml_element_from_var_name(self, var_name):
	    a_nm = self.vid[var_name]['area']
	    s_nm = self.vid[var_name]['subsys']
	    x_path = 'variable_info//area[@name="' + a_nm + '"]//subsystem[@name="' + s_nm + '"]//var//var_name/..'
	    # x_path = 'variable_info//area[@name="modem"]//subsystem[@name="LTE"]//var//var_name/..'
	    for ele in self.xml_root.findall(x_path):
	        if ele.find('var_name').text == var_name:
	            return ele
	    raise Exception("mee.find_xml_element_from_var_name(): Unable to find var_name in xml :" + var_name)

	def find_xml_element_from_area(self, a_nm):
	    # a_nm=self.vid[var_name]['area']
	    # s_nm=self.vid[var_name]['subsys']
	    x_path = 'variable_info//area[@name="' + a_nm + '"]'
	    # x_path = 'variable_info//area[@name="modem"]//subsystem[@name="LTE"]//var//var_name/..'
	    ele = self.xml_root.findall(x_path)
	    if len(ele) == 1:
	        return ele[0]
	    raise Exception("mee.find_xml_element_from_area(): multiple (or none) area found  :" + a_nm)

	def find_xml_element_from_area_subsys(self, a_nm, s_nm):
	    x_path = 'variable_info//area[@name="' + a_nm + '"]//subsystem[@name="' + s_nm + '"]'
	    # x_path = 'variable_info//area[@name="modem"]//subsystem[@name="LTE"]//var//var_name/..'
	    ele = self.xml_root.findall(x_path)
	    if len(ele) == 1:
	        return ele[0]
	    raise Exception(
            "mee.find_xml_element_from_area_subsys(): multiple (or none) area,subsys found  :" + a_nm + "," + s_nm)


# def init_cmm(self):
#        txt  = ';'+'-'*80+'\n'
#        txt += "; usage: do minidump_elf_loader.cmm <minidump_var_tab start address>\n"
#        txt += ';' + '-' * 80 + '\n'
#
#        txt += "entry &sa   ; sa = start address\n\n\n\n"
#        self.cmm.write(txt)

#    def write_cmm(self,offset,size,va,var):
#        txt = "; copying "+var+"\n"
#        txt+= "Data.COPY (&sa+0x%X)--(&sa+0x%X)  0x%X \n\n\n"%(offset,offset+size-1,va)
#        #txt+= "Data.COPY  &sa+"+str(offset)+" -- &sa+"+str(offset+size)+" "+str(va)+ "  \n"
#        self.cmm.write(txt)




def fill_xml_from_text_input(in_file, xml_e):
    if (os.path.exists(in_file)):
        # print "filling xml from text input file"
        var_list = open(in_file).read().splitlines()
        var_list = [var.strip() for var in var_list if var.strip() != '']  # cleaning out empty lines and tabs
        # sub_sys_name = os.path.basename(in_file).split('.')[0]
        # xt = et.ElementTree(file=x_f)
        # xr = xt.getroot()
        # vi = xr.find('variable_info')
        #        for e_a in vi.getchildren():
        #            for e_s in e_a.getchildren():
        #                if e_s.attrib['name'].upper() == sub_sys_name.upper() :
        for var in var_list:
            # print var
            v_elem = et.Element('var')
            v_child_elem = et.SubElement(v_elem, 'var_name')
            v_child_elem.text = var
            v_child_elem = et.SubElement(v_elem, 'var_size')
            v_child_elem.text = "0x0"
            v_child_elem = et.SubElement(v_elem, 'var_type')
            v_child_elem.text = "0"
            # et.dump(v_elem)
            xml_e.append(v_elem)

        # for var in var_list:
        #    print var


def main(argv):
    start_time = time.time()
    parser = argparse.ArgumentParser(description='mee.py ')
    parser.add_argument('--xml', help='xml file path', required=True)
    parser.add_argument('--elf', help='elf file path', required=True)
    parser.add_argument('--chipset', help='chipset from build environment', default=None)
    parser.add_argument('--short_buildpath', help='short_buildpath from build environment', default=None)
    parser.add_argument('--tin', help="text input for variables")
    parser.add_argument('--in_ss', help="subsystem for the text input")
    parser.add_argument('--cust_config_path', help="cust config path")
    args = vars(parser.parse_args(argv))
    # version number :
    version = '2017.05.30.1'
    # In future this version will be automatically build , by reading the $Header and $DateTime info , at the start of this document.
    # this_file = __import__(__name__)
    # with open(this_file.__file__,'r') as f_read:
    #    for line in f_read:
    print "version: ", version
    print "mee.main(): Called mee with arguments"
    print "mee.main(): raw args: \n", '\n'.join(argv)
    print "mee.main(): elf path: ", args['elf']
    print "mee.main(): xml path: ", args['xml']
    short_build_path_info = args['short_buildpath']
    cust_config_path = os.path.join(minidump_dir, '..', '..', 'config',short_build_path_info.split(".", 1)[0], 'cust_config.xml')
    if os.path.exists(cust_config_path):
        print "mee.main(): Full path for cust_config : ", cust_config_path
        args['cust_config_path'] = cust_config_path
    else:
        print "mee.main() : Full path for cust_config invalid"
    if args['chipset']:
        print "mee.main(): chipset: ", args['chipset']
    fwc_path = os.path.join(minidump_dir, '..', '..', 'fw_common', 'bin')
    if args['short_buildpath']:
        fwc_in_path = os.path.join(fwc_path, args['short_buildpath'], 'modem_fw_minidump.txt')
        fwc_ext_path = os.path.join(fwc_path, args['short_buildpath'], 'modem_fw_minidump.ext.txt')
        if os.path.exists(fwc_in_path):
            args['tin'] = fwc_in_path
            args['in_ss'] = 'FW_COMMON'
        elif os.path.exists(fwc_ext_path):
            args['tin'] = fwc_ext_path
            args['in_ss'] = 'FW_COMMON'

    # if args['tin']:
    #    fill_xml_from_text_input(args['tin'],args['xml'])
    mee = minidump_elf_editor(args['elf'], args['xml'],args['cust_config_path'])
    mdump_sym = mee.elf_handle.getSymbolByName("minidump_info")
    if (mdump_sym == const.RC_ERROR):
        print "mee.main(): minidump_info symbol not found"
        return
    print "mee.main(): reading and updating xml"
    ret = mee.read_and_sanitize_xml(args['tin'], args['in_ss'], args['chipset'])
    if ret != 0:
        print "mee.main(): read_and_sanitize_xml failed"
        return
    print "mee.main(): updating minidump_var_tab entries in elf"
    mee.write_minidump_var_tab_entries_to_elf()
    print "mee.main(): read cust config xml and write to elf"
    mee.read_custxml_write_image_size_to_elf()
    print "mee.main(): generating new elf file (output)"
    mee.write_to_elf()
    if not (mee.log.closed):
        mee.log.close()
    print "mee.main(): Done. Total execution time : ", time.time() - start_time, " seconds"


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
