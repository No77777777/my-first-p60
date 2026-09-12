import xml.etree.ElementTree as ET
import types
import collections
import re
from HWIORegQuery import HWIORegQuery

MEMMAP_TYPE_NAME = "ACMapAttributesType_t"
hwioquery = HWIORegQuery('../../../../../api/systemdrivers/hwio/msm8996/ARM_ADDRESS_FILE.FLAT')

# Generate c code for includes and struct declarations
def InitialDefinitions(code):
	code += "#ifndef AC_STATIC_CONFIG_TEST_H" + '\n'
	code += "#define AC_STATIC_CONFIG_TEST_H" + '\n'
	code += '#include "AccessControlHyp.h"' + '\n'

	code += "typedef struct" + '\n' + '{' + '\n'
	code += '\t' + "char						  *region_name; // in bytes"  + '\n'
	code += '\t' + "uint64						  size;" + '\n'
	code += '\t' + "uint64                        phys_addr;" + '\n'
	code += '\t' + "uint64                        virt_addr;" + '\n'
	code += '\t' + "uint32                        permissions;" + '\n'
	code += '\t' + "ACMemoryShareabilityType          shareability;" + '\n'
	code += '\t' + "ACMemoryAttributesType               mem_attr;" + '\n'
	code += '\t' + "uint32		                  writable;" + '\n'
	code += "} ACMapAttributesType_t;" + '\n' + '\n'
	code += "typedef struct \n{\n"
	code += '\t' + "int aperture_size;" + '\n'
	code += '\t' + "const ACMapAttributesType_t *aperture;" + '\n'
	code += '\t' + 'ACVirtualMachineId eVM;' + '\n'
	code += "}aperture;"  + '\n' + '\n'
	return code


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

def indent(lines, tabSize = 4):
	tab = ""
	for i in range(tabSize):
		tab += " "
	out = ""
	for line in lines.splitlines(True):
		out += tab + line
	return out

# Converts a string with spaces to spinal_case (all lowercase snake_case)
# Does not convert camelCase
def toSpinalCase(s):
	return str.replace(str.lower(s), ' ', '_')

h_start_addr = 0x0
h_out  = ""

# Generate c code for a single memmap that would be in an array initialization
# region_name (str) - region name
# ac_size (str) - the size of the region
# ac_offset(int) - offset if the region has any
# permissions (str) -
# rationale (str) - Comments from XML
# region_address (str) - Exact address of the region as per Jeremy
def generateMemmapStruct(region_name, ac_size, ac_offset, permissions, rationale, region_address, shareability = "AC_MEMORY_SHAREABLE_DEFAULT", mem_attr = "AC_MEMORY_ATTR_DEFAULT"):
	global h_start_addr, h_out
	perm = parsePermissions2(permissions)
	start_addr = int(region_address,16)
	end_addr = int(region_address,16) + int(ac_size,16)
	found_rw = 0
	out = "{"
	out += '"' + region_name + '"' + ', '#\n'
	out += ac_size + ', '#\n'
	h_ac_size = start_addr - h_start_addr
	if h_ac_size != 0:
		h_out += "{"
		h_out += '"' + "HOLE" + '"' + ', '#\n'
		h_out += hex(h_ac_size) + ', '#\n'
		h_out += hex(h_start_addr)  + ', '#\n
		h_out += hex(h_start_addr ) + ', '#\n
		h_out += "0" + ', '#\n'
		h_out += shareability + ', '#\n'
		h_out += mem_attr + ', '#\n'
		h_out += "0"
		h_out += '},'
		h_out += '\n'


	if permissions == "0":
		return ""
	h_start_addr = end_addr
	# Find a register address with RW attributes by looking up in the attributes header file
	if (permissions == "RW") or (permissions == "W") or permissions == "RW "  or permissions == "R" or permissions == "R ":
		registers = hwioquery.registers_in_range(start_addr, end_addr)
		#print "For region " + region_name + " " + "Range: " + hex(start_addr) + ' - ' + hex(end_addr)
		for register in registers:
			if register.Writeable() and register.Readable() and ("CMD" not in register.Symbol()) and ("CTRL" not in register.Symbol()) and ("CONFIG" not in register.Symbol()) and ("CTL" not in register.Symbol()) and ("CTRL" not in register.Symbol()) and ("CONTROL" not in register.Symbol()) and ("MODE" not in register.Symbol()) and ("HMSS" not in register.Symbol()):
				region_address = hex(register.Address())
				print hex(register.Address()) + " " + register.Symbol()


				out += region_address + ', '#\n'
				out += region_address  + ', '#\n'
				found_rw = 1
				break

		if found_rw !=1:
			#print "NO RW entry found, skipping this"
			out += region_address  + " + " + hex(ac_offset) +', '#\n'
			out += region_address  + " + " + hex(ac_offset) +', '#\n'
			out += perm + ', '#\n'
			out += shareability + ', '#\n'
			out += mem_attr + ', '#\n'
			out += "0"
			out += '},'
			out += "\t" + "/*" + region_name + "*/"
			return out
	else:
		if ac_offset=="0":
			out += region_address  + ', '#\n'
			out += region_address  + ', '#\n'
		else:
			if (isinstance(ac_offset,types.NoneType)) != True:
				out += region_address + " + " + hex(ac_offset) + ', '#\n'
				out += region_address + " + " + hex(ac_offset) + ', '#\n'

	out += perm + ', '#\n'
	out += shareability + ', '#\n'
	out += mem_attr + ', '#\n'
	out += "1"
	out += '},'
	out += "\t" + "/*" + region_name + "*/"
	return out

# Generate c code for an array of memmap structs. This array is the CB or Aperture.
# aperture - [apertureName, (addr, size, permissions), ...]
# static (bool) - True if the returned CB array code should be static
# const (bool)	- True if the returned CB array code should be constant
def generateCB(aperture, prev, static = True, const = True):
	global h_out
	global h_start_addr
	const = "const" if const else ""
	static = "static" if static else ""
	out = ""
	if prev != aperture[0]:
		if prev[0] != "NULL":
			out = "};\n" + '\n'
			h_out += "};\n" + '\n'
		out += static + " " + const + " " + MEMMAP_TYPE_NAME + " " + toSpinalCase(aperture[0]) + "[] = {\n"
		h_out += static + " " + const + " " + MEMMAP_TYPE_NAME + " " + toSpinalCase(aperture[0]) + "_holes[] = {\n"
		h_start_addr = 0
	for granule in aperture[1:(len(aperture)+1)]:
		out += indent(generateMemmapStruct(granule[0], granule[1], granule[2], granule[3], granule[4], granule[5])) + '\n'
	return out



# xmlFile (str) - the name/location of the XML file to parse
# returns - c code for CB arrays, includes as many arrays as there are Apertures in the XML
def generateCBInitializationFromXMLFile(xmlFile):
	global h_out
	tree = ET.parse(xmlFile)
	root = tree.getroot()
	apertures = translateXML(root)
	# Sort the apertures w.r.t aperture names
	#apertures = sorted(apertures)

	# Spit out #include and structure declarations
	code = ""
	code = InitialDefinitions(code)

	# For each aperture generate structures array
	apertureNames = []
	prev = ["NULL"]
	for aperture in apertures:
		code += generateCB(aperture,prev,next)
		apertureNames.append(aperture[0])
		prev = aperture[0]

	h_out  += "};\n" + '\n'
	# To calculate the size of each aperture
	counter = collections.Counter(apertureNames)
	code += "};\n" + '\n'
	code += generateTopLevelArray(counter)
	code = code + h_out

	return code

# Takes a dictionary which has the name and size of the aperture and creates an array of struct aperture
# counter - dictionary with size and name of aperture
# returns - structure declaration
def generateTopLevelArray(counter):
	global h_out
	declaration = "static aperture aperture_groups[] = {\n"
	h_declaration = "static aperture aperture_groups_holes[] = {\n"
	definition = ""
	h_definition = ""
	items = counter.items()
	for aperture in items:
		definition += '{' + "sizeof("+str(toSpinalCase(aperture[0]))+")/sizeof(" + str(toSpinalCase(aperture[0])) + "[0])" + ',' + str(toSpinalCase(aperture[0])) + ',' + 'AC_VM_' + str(aperture[0]).replace(' ', '_').upper() + "},\n"
		h_definition += '{' + "sizeof("+str(toSpinalCase(aperture[0]))+"_holes)/sizeof(" + str(toSpinalCase(aperture[0])) + "_holes[0])" + ',' + str(toSpinalCase(aperture[0])) + '_holes,' + 'AC_VM_' + str(aperture[0]).replace(' ', '_').upper() + "},\n"
	h_out +=  h_declaration + indent(h_definition) + "};\n" + "#endif"
	return declaration + indent(definition) + "};\n"



# Takes the root of an XML tree and translates the XML into std data structures that can be passed to other functions
# root - the root of the XML
# returns - a list of the apertures in the XML, which is a list of the aperture in those apertures, which are tuples of the attributes of those aperture
#			[[apertureName, (addr,region_size,ac_size,offset,perm,rationale), ...], ...]
def translateXML(root):
	out = []
	error = 0
	for record in root:
		if (isinstance(record.find('s2aperture_name').text,types.NoneType)):
			error += 1
			continue
		if (isinstance(record.find('region_name').text,types.NoneType)):
			error += 1
			continue
		name = record.find('s2aperture_name').text
		aper = [name]
		addr = record.find('region_address').text
		region_name = record.find('region_name').text
		if ',' in region_name:
			region_name = region_name.split(',')[0]
		ac_size = record.find('ac_size').text
		if (isinstance(record.find('ac_permission').text,types.NoneType)):
			perm = "0"
		else:
			perm = record.find('ac_permission').text
		rationale = record.find('ac_rationale').text
		if (isinstance(rationale,types.NoneType)):
			rationale = ""
		offset = record.find('ac_offset').text
		offset = int(offset,16)
		if addr[-3:] != "000":
			print addr + " " + region_name
			continue
		#var = raw_input("Please enter something: ")
		aper.append((region_name,ac_size,offset,perm,rationale,addr))
		out.append(aper)
		#print error
	return out


def xml_parse():
	code = generateCBInitializationFromXMLFile('../../../cfg/8996/hyp/sw_aperture_MSM8996(Istari)_combined.xml')
	out_file = open("../../../cfg/8996/hyp/test/ACSmmuStaticConfig_Test.h","w")
	code = code.encode('ascii','ignore')
	out_file.write(code)
	out_file.close()
	return None

xml_parse()
