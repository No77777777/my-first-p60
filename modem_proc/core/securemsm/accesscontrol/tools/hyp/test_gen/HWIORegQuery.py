import sys
import bisect
from HWIOParser import LoadAddressFile

class HWIORegQuery(object):
    '''
    Class that will load FLAT file into memory and allow client to call registers_in_range()
    to retrieve a list of registers within the range from start to end.
    To use this in your own script:
    
    from HWIORegQuery import HWIORegQuery
    hwio_req_query = HWIORegQuery(<FLAT_FILE_NAME>)
    hwio_reg_query.register_in_range(0x100000, 0x200000)
    '''
    def __init__(self, flat_file):
        '''
        Construct the object. FLAT file name is input parameter.
        '''
        self._bases = LoadAddressFile(flat_file)
    def registers_in_range(self, start, end):
        '''
        Return list of HWIORegister objects between start and end
        '''
        registers = []
        self._bases.sort(key=lambda base: base.address)
        base_start_idx = bisect.bisect_left([base.address for base in self._bases], start + 1) - 1
        base_end_idx = bisect.bisect_left([base.address for base in self._bases], end)
        #print "Start at 0x{:08x} - {}".format(self._bases[base_start_idx].address, self._bases[base_start_idx].name)
        #print "End   at 0x{:08x} - {}".format(self._bases[base_end_idx].address, self._bases[base_end_idx].name)
        for base in self._bases[base_start_idx:base_end_idx]:
            base.modules.sort(key=lambda module: module.offset)
            mod_start_idx = bisect.bisect_left([mod.Address() for mod in base.modules], start + 1) - 1
            mod_end_idx = bisect.bisect_left([mod.Address() for mod in base.modules], end)
            #print "Start at 0x{:08x} - {}".format(base.modules[mod_start_idx].Address(), base.modules[mod_start_idx].name)
            #print "End   at 0x{:08x} - {}".format(base.modules[mod_end_idx].Address(), base.modules[mod_end_idx].name)
            for module in base.modules[mod_start_idx:mod_end_idx]:
                module.registers.sort(key=lambda register: register.offset)
                reg_start_idx = bisect.bisect_left([register.Address() for register in module.registers], start)
                reg_end_idx = bisect.bisect_left([register.Address() for register in module.registers], end)                
                #print "Start at 0x{:08x} - {}".format(module.registers[reg_start_idx].Address(), module.registers[reg_start_idx].Symbol())
                #print "End   at 0x{:08x} - {}".format(module.registers[reg_end_idx].Address(), module.registers[reg_end_idx].Symbol())
                for register in module.registers[reg_start_idx:reg_end_idx]:
                    registers.append(register)
        return registers
        

if __name__ == "__main__":
    '''
    Demonstration code for HWIORegQuery class declared above.
    for the purposes of this example, execute this script as:
        python HWIORegQuery.py <FLAT_FILE_NAME> <StartAddrHex> <EndAddrHex>
    ex. python HWIORegQuery.py ARM_ADDRESS_FILE_SW.FLAT 0x100000 0x200000
    '''
    # Construct the HWIORegQuery object. Pass in 
    hwioquery = HWIORegQuery(sys.argv[1])
    # Call the registers_in_range() method to get list of HWIORegister objects.
    # Check HWIOParser to learn what methods can be called on these objects to retrieve
    # register name, address, readable/writeable capability and to determine if it's 
    # an array, etc.
    registers = hwioquery.registers_in_range(int(sys.argv[2], 16), int(sys.argv[3], 16))
    # For demo purposes, print the list of registers retrieved
    for register in registers:
        rwcstatus = ""
        if register.Readable():
            rwcstatus += "R"
        else:
            rwcstatus += "-"
        if register.Writeable():
            rwcstatus += "W"
        else:
            rwcstatus += "-"
        if register.Command():
            rwcstatus += "C"
        else:
            rwcstatus += "-"
        print "REGISTER: 0x{:08x}: {} {}".format(register.Address() , rwcstatus, register.Symbol())

    