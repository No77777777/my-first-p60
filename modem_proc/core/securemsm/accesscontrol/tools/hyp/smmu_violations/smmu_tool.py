import re
import subprocess
from cStringIO import StringIO
import json
from pprint import pprint
import os
import xml.etree.ElementTree as ET
from types import *
import collections
import csv
import sys
import getopt
import types

# Point it to the combined XML and the logs generated from Virtio
# Usage - python smmu_tool.py  --in_xml="sw_aperture_MSM8996(Istari)_combined.xml" --in_log="Q6_SMMU_debug.log" > all_transactions.txt

def comment_remover(text):
    def replacer(match):
        s = match.group(0)
        if s.startswith('/'):
            return " " # note: a space and not an empty string
        else:
            return s
    pattern = re.compile(
        r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
        re.DOTALL | re.MULTILINE
    )
    return re.sub(pattern, replacer, text)

def create_json():
	src_code = ""
	new_line = ""
	source_code = open("../../../../../kernel/smmu/v2/8996/SMMUConfig.c","r")
	out_file = open("new_SMMUConfig.c","w")
	source_code = source_code.read()
	new_source_code = comment_remover(source_code)
	new_source_code = new_source_code.splitlines(True)
	replace_hex = re.compile(r"(0x[A-Z0-9\-]+)")
	replace_string = re.compile(r"([A-Z0-9_]+)")
	for i, line in enumerate(new_source_code):
		
		if (i == 0 or not line.startswith('#'))and line.strip() and (not ("SMMU") in line):
			
			line= line.replace('{','[').replace('}', ']')
			
			new_line = line
			
			
			if '0x' in line:
				new_line = ""
				
				if '[' in line:
					new_line += "["
					
				for ele in re.findall(replace_hex, line):		    
				    new_line +=  '"' + ele + '",'
				if ']' in line:
					if ',' in line:
						new_line =  new_line[:-1] + "],"
					else:
						new_line =  new_line[:-1] + "]"
						
				
				new_line += "\n"
			
			else:
				new_line = ''
				
				if '[' in line:
					new_line += "["
					
				for ele in re.findall(replace_string, line):
				    
				    new_line +=  '"' + ele + '",'
				if ']' in line:
					if ',' in line:
						new_line =  new_line[:-1] + "],"
					else:
						new_line =  new_line[:-1] + "]"
				
				new_line += "\n"
			
		src_code += new_line		
		new_line = ""	
	out_file.write(src_code)
	out_file.close()
	
	src_code = ""
	in_file = open("new_SMMUConfig.c","r")
	out_file = open("json_SMMUConfig.c","w")
	
	lines = in_file.readlines()
	lines = lines[:-2]
	for index, line in enumerate(lines):
		if index < (len(lines) -1 ):
			if (']' in lines[index+1] ) and ('],' in line):
					src_code += line[:-2] + '\n'
			else:
				src_code += line
		else:
			src_code += line
	out_file.write(src_code)
	out_file.close()
	in_file.close()
	os.remove("new_SMMUConfig.c")
	
	
create_json()
json_data = open('json_SMMUConfig.c')
data = json.load(json_data)

# Takes the root of an XML tree and translates the XML into std data structures that can be passed to other functions
# root - the root of the XML
# returns - a list of the apertures in the XML, which is a list of the aperture in those apertures, which are tuples of the attributes of those aperture
#			[[apertureName, (addr,region_size,ac_size,offset,perm,rationale), ...], ...]
def translateXML(root):
	out = []
	for record in root:
		if (isinstance(record.find('s2aperture_name').text,types.NoneType)):
			continue
		if (isinstance(record.find('region_name').text,types.NoneType)):
			continue
		#name = toSpinalCase(record.find('s2aperture_name').text)
		name = record.find('s2aperture_name').text
		aper = [name]
		addr = record.find('region_address').text
		#region_size = record.find('region_size').text
		region_name = record.find('region_name').text
		ac_size = record.find('ac_size').text
		if (isinstance(record.find('ac_permission').text,types.NoneType)):
			perm = "0"
		else:
			perm = record.find('ac_permission').text
		rationale = record.find('ac_rationale').text
		if (isinstance(rationale,types.NoneType)):
			rationale = ""
		offset = record.find('ac_offset').text
		aper.append((addr,ac_size,offset,perm,rationale))
		out.append(aper)

	return out


# Translate a permisison string (like 'rw') to c code for those permissions.
# Assumes permissions are defined (include trustzone_images\core\kernel\smmu\v2\SMMUTranslation.c or similar)
def parsePermissions2(permissions):
	types = ['r', 'w', 'x']
	perm = "( "
	if "R" in permissions:
		perm += 'AC_PERM_R | '
	if "W" in permissions:
		perm += 'AC_PERM_W | '
	if "X" in permissions:
		perm += 'AC_PERM_X | '
	if len(perm) > 2:
		perm = perm[0:-2] + ')'
	else:
		perm = '0'
	return perm
	
def find_vmid(smmu_inst, SID):
	for entry in data:
		# Find the SMMU entry
		if entry[1] == smmu_inst:
			# Find the CB list
			if len(entry) == 10:
				cb_list = entry[9]
			else:
				cb_list = entry[10]
			
			# Now scan the CB list for SID
			for cb in cb_list:
				if len(cb) == 8:
					sid_list = cb[7]
				elif len(cb) == 7:
					sid_list = cb[6]
				
				if sid_list[0] == "0":
					continue
				else:
					for sid in sid_list:
						sid = int((sid.lower())[-4:],16)
						if sid == SID:
							return cb[0]
	
	return 0

# xmlFile (str) - the name/location of the XML file to parse
# returns - c code for CB arrays, includes as many arrays as there are Apertures in the XML
def check_violation(xmlFile, transaction_addr, access_type, vmid):
	tree = ET.parse(xmlFile)
	root = tree.getroot()
	apertures = translateXML(root)
	status = 1
	# Sort the apertures w.r.t aperture names
	apertures = sorted(apertures)
	for aperture in apertures:
		vm = 'AC_VM_' + str(aperture[0]).replace(' ', '_').upper()
		if vm == vmid:
			granule = aperture[1]
			size = granule[1]
			start_address = int(granule[0],16)
			end_address = start_address + int(size,16)
			perm = granule[3]
			#Address in range?
			if (int(transaction_addr,16) >= start_address ) and (int(transaction_addr,16) <= end_address):
				#Check perm
				if access_type in perm:
					return True
				else:
					return False
			else:
				status = 0
				
	if status == 0:
		return False
				
			
			
g_header_fp = None
g_log_error = True #print error messages
g_log_info = False #print informational messages
false = False
true = True

def LogMsg(logit, s ):
	if (logit == True):
		print s + "\n"
  #LogMsg_fp.write(s)
  #LogMsg_fp.write("\n")
  
#----------------------------------------------------------------------------
# usage
# Help on how to use this script
#----------------------------------------------------------------------------
def usage ():
  print("========================================================")
  print("-h, --help       print this help\n")
  print("usage\n")
  print("   where <xml_file> specifies the full path + filename of the XML to parse")
  print("   where <header dir> specifies where the full path + filename of the test header file to be written\n")
  print("   where <chipset> specifies target. This is an optional argument \n")
  print("e.g: tzt_xml_parser.py --in_xml=\"c:\\temp\\AccessControl.xml\" --out_h=\"c:\\test\\inc\\tztac.h\"")
  print("e.g: tzt_xml_parser.py --in_xml=\"c:\\temp\\AccessControl.xml\" --out_h=\"c:\\test\\inc\\tztac.h\" --chipset=\"msm8974\"")
  print("========================================================\n")
  

smmu_mappings = {'Q6_SMMU':'LPASS_Q6',
								'CORE_SMMU':'LPASS_CORE',
								'qsmmu_cpp':'CPP',
								'qsmmu_jpeg':'JPEG',
								'qsmmu_vfe':'VFE',
								'qsmmu_mdp':'MDP',
								'qsmmu_rot':'ROT',
								'qsmmu_venus':'VENUS',
								'agnoc0_smmu':'ANOC0',
								'agnoc1_smmu':'ANOC1',
								'agnoc2_smmu':'ANOC2',
								'qsmmu_gfx':'GPU',
								}
		
def main(argv):
	if type(argv) is StringType:
    #This happens when Sconscript calls the function with string argument
		argv = argv.split(' ')
	try:
		opts, args = getopt.getopt(argv, '', ["in_xml=", "in_log="])
	except getopt.GetoptError, err:
      # LogMsg help information and exit:
		print str(err) 
		usage()
		sys.exit(2)

	in_xml = None
	in_log = None

	if (len(sys.argv) < 2):
		usage()
		sys.exit(2)

	for opt, arg in opts:
		if (opt == "--in_xml"):
			in_xml = arg        
			LogMsg(g_log_info, "--in_xml is %s" % in_xml)
		elif (opt == "--in_log"):
			in_log = arg
			LogMsg(g_log_info, "--in_log is %s" + in_log)
   

	if (in_xml is None):
		print "XML file not specified" 
		usage()
		sys.exit(2)    

	if (in_log is None):
		print "Header file not specified" 
		usage()
		sys.exit(2)    
	
	with open(in_log,'rb') as file:
		reader = csv.reader(file)
		for row in reader:
			if len(row) < 8:
				continue
			smmu_inst =  (re.search(r"\[([A-Za-z0-9_]+)\]", row[0])).group(1)
			smmu_inst = smmu_mappings[smmu_inst]
			transaction_addr = (row[4].split('='))[1]
			access_type = (row[5].split('='))[1]
			SID = (row[6].split('='))[1]
			SID = int(SID,16)
			
			if access_type == 1:
				access_type = "W"
			else:
				access_type = "R"
				
			# Ignore DDR Region access
			if int(transaction_addr,16) >= 0x80000000:
				#print "Ignoring region"
				continue
			
			#bUG, ssc sid 	is wrong
			if smmu_inst == "ANOC2":
				SID = 0x18
			vmid = find_vmid(smmu_inst,SID)
			if (vmid == 0):
				print "No valid vmid found \n"
			ret = check_violation(in_xml, transaction_addr, access_type, vmid)
			if not ret:
				print "Invalid Transaction: " + ', '.join(map(str, row)) 
			else:
				print "Valid Transaction: " + ', '.join(map(str, row)) 
	
	json_data.close()
	os.remove("json_SMMUConfig.c")

if __name__ == "__main__":
	main(sys.argv[1:])