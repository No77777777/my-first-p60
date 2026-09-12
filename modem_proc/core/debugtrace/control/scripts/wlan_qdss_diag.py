#!/usr/bin/env python
from __future__ import print_function
import subprocess
import sys
import os
import struct
import binascii
import argparse
import re


class Utils:
    @staticmethod
    def this_dir():
        return os.path.dirname(os.path.realpath(__file__))

    @staticmethod
    def get_diag_port():
        try:
            return subprocess.check_output([os.path.join(Utils.this_dir(),'diagports.exe'),'-u'])
        except:
            return ''

    @staticmethod
    def qdss_diag(cmd_bytes):
        qdss_diag_cmd=[os.path.join(Utils.this_dir(),'qdssdiag.exe'),Args.port,cmd_bytes]

        qdss_diag_cmd=' '.join(qdss_diag_cmd)
        print(qdss_diag_cmd)
        qdss_diag_cmd=qdss_diag_cmd.split()
        if (not Args.norun):
            return subprocess.check_output(qdss_diag_cmd)
        else:
            return ''

    @staticmethod
    def is_process_running(process_name):
        return re.search("^%s" % process_name,
                         subprocess.check_output('tasklist'),
                         re.MULTILINE)



    @staticmethod
    def split_bytes(s):
        return ' '.join([s[i:i+2] for i in range(0, len(s), 2)])

    @staticmethod
    def uint8(n):
        return binascii.hexlify(struct.pack('B',int(n,0)))
     
    @staticmethod
    def uint16(n):
        return Utils.split_bytes(binascii.hexlify(struct.pack('H',int(n,0))))
     
    @staticmethod
    def uint32(n):
        return Utils.split_bytes(binascii.hexlify(struct.pack('L',int(n,0))))

    @staticmethod
    def this_function_name():
        return sys._getframe(1).f_code.co_name

    @staticmethod
    def caller_name():
        return sys._getframe(2).f_code.co_name




class Wlan:
    pd_mask=3 #for wlan pd
    tpdm_id='0'
    tpda_id='0'
    reserved='0'

    wlan_diag_cmds={'TRACE_SINK':0x01,
                    'TMC':0x30,
                    'TPDM':0x46,
                    'TPDA':0x47,
                    'WCSS':0x48,
                    }
    tpdm_cmd={
        'tpdm_set':'01 00',
        'tpdm_dataset_set':'02 00', 
        'tpdm_get_gpr':'03 00', 
        'tpdm_set_gpr': '04 00', 
        'tpdm_dsb_set_edcmr':'05 00', 
        'tpdm_dsb_get_param':'06 00', 
        'tpdm_dsb_set_param':'07 00', 
        'tpdm_cmb_get_param':'08 00', 
        'tpdm_cmb_set_param':'09 00', 
        }

    tpda_cmd={
        'tpda_set_param':'01 00', 
        'tpda_get_param':'02 00', 
        }

    wcss_cmd={
        'wcss_set_param':'01 00', 
        'wcss_get_param':'02 00', 
        }



    @classmethod
    def base_cmd(cls,s):
        if Args.qxdm:
            return "4b 5a %.02x %0.2x" % (cls.wlan_diag_cmds[s],cls.pd_mask)
        else:
            return "%.02x%0.2x" % (cls.pd_mask,cls.wlan_diag_cmds[s])


class CommandProcessor:
    def __init__(self,tokens):
        self.tokens=tokens
        self.current=-1

    def next_token(self):
        index=self.current+1
        if (len(self.tokens) > index):
            self.current=index
            token=self.tokens[index].strip()
            if not token:
                return 'EOL'
            else:
                return token
        else:
            return('EOL')
        
    def process(self):
        token=self.next_token()
        try:
            getattr(self,token)()
        except AttributeError as e:
            raise Exception("Unrecognized command '%s'" %(token))
    
    @staticmethod
    def command_strings():
        return '''
etb_set_mode <mode> (0 for CBUF, 2 for HWFIFO)
etb_dump <filename> filename
tpdm_set <state> 
tpdm_dataset_set <enable_mask> <state>
tpdm_get_gpr <reg_num>
tpdm_set_gpr <reg_num> <val>
tpdm_dsb_set_edcmr <start> <end> <state>
tpdm_dsb_get_param <param_id>
tpdm_dsb_set_param <param_id> <val>
tpdm_cmb_get_param <param_id>
tpdm_cmb_set_param <param_id> <val>
tpda_get_param <param_id>
tpda_set_param <param_id> <val>
wcss_get_param <param_id>
wcss_set_param <param_id> <val>
'''



    def etb_set_mode(self):
        sink=self.next_token()
        self.execute(' '.join( [Wlan.base_cmd('TRACE_SINK'),
                                Utils.uint8(sink)]))

    def etb_dump(self):
        filename=self.next_token()
        self.execute(' '.join( [Wlan.base_cmd('TMC'),
                                filename]))

        
    def tpdm_cmd(self):
        return [Wlan.base_cmd('TPDM'),
                Wlan.tpdm_cmd[Utils.caller_name()],
                Utils.uint16(Wlan.reserved)]

    def tpdm_set(self):
        state=self.next_token()
        self.execute(' '.join(self.tpdm_cmd() +
                [Utils.uint8(Wlan.tpdm_id),
                 Utils.uint8(state)]))

    def tpdm_dataset_set(self):
        enable_mask=self.next_token()
        state=self.next_token()
        self.execute(' '.join( self.tpdm_cmd() +
                [Utils.uint8(Wlan.tpdm_id),
                 Utils.uint8(enable_mask),
                 Utils.uint8(state)]))

    def tpdm_get_gpr(self):
        reg_num=self.next_token()
        self.execute(' '.join( self.tpdm_cmd() +
                               [Utils.uint8(Wlan.tpdm_id),
                                Utils.uint8(reg_num)]))

    def tpdm_set_gpr(self):
        reg_num=self.next_token()
        val=self.next_token()
        self.execute(' '.join( self.tpdm_cmd() +
                               [Utils.uint8(Wlan.tpdm_id),
                                Utils.uint8(reg_num),
                                Utils.uint16(Wlan.reserved),
                                Utils.uint32(val),
                                ]))

    def tpdm_dsb_set_edcmr(self):
        start=self.next_token()
        end=self.next_token()
        state=self.next_token()
        self.execute(' '.join( self.tpdm_cmd() +
                               [Utils.uint8(Wlan.tpdm_id),
                                Utils.uint8(start),
                                Utils.uint8(end),                   
                                Utils.uint8(state),
                                ]))

    def tpdm_dsb_get_param(self):
        param_id=self.next_token()
        self.execute(' '.join( self.tpdm_cmd() +
                               [Utils.uint8(Wlan.tpdm_id),
                                Utils.uint8(param_id),
                                ]))

    def tpdm_dsb_set_param(self):
        param_id=self.next_token()
        val=self.next_token()
        self.execute(' '.join( self.tpdm_cmd() +
                [Utils.uint8(Wlan.tpdm_id),
                 Utils.uint8(param_id),
                 Utils.uint16(Wlan.reserved), 
                 Utils.uint32(val)]))


    def tpdm_cmb_get_param(self):
        param_id=self.next_token()
        self.execute(' '.join( self.tpdm_cmd() +
                               [Utils.uint8(Wlan.tpdm_id),
                                Utils.uint8(param_id),
                                ]))


    def tpdm_cmb_set_param(self):
        param_id=self.next_token()
        val=self.next_token()
        self.execute(' '.join( self.tpdm_cmd() +
                [Utils.uint8(Wlan.tpdm_id),
                 Utils.uint8(param_id),
                 Utils.uint16(Wlan.reserved), 
                 Utils.uint32(val)]))


    def tpda_cmd(self):
        return [Wlan.base_cmd('TPDA'),
                Wlan.tpda_cmd[Utils.caller_name()],
                Utils.uint16(Wlan.reserved)]



    def tpda_get_param(self):
        param_id=self.next_token()
        self.execute(' '.join( self.tpda_cmd() +
                               [Utils.uint8(Wlan.tpda_id),
                                Utils.uint8(param_id),
                                ]))


    def tpda_set_param(self):
        param_id=self.next_token()
        val=self.next_token()
        self.execute(' '.join( self.tpda_cmd() +
                [Utils.uint8(Wlan.tpda_id),
                 Utils.uint8(param_id),
                 Utils.uint16(Wlan.reserved), 
                 Utils.uint32(val)]))


    def wcss_cmd(self):
        return [Wlan.base_cmd('WCSS'),
                Wlan.wcss_cmd[Utils.caller_name()],
                Utils.uint16(Wlan.reserved)]



    def wcss_get_param(self):
        param_id=self.next_token()
        self.execute(' '.join( self.wcss_cmd() +
                               [Utils.uint8(param_id),
                                ]))


    def wcss_set_param(self):
        param_id=self.next_token()
        val=self.next_token()
        self.execute(' '.join( self.wcss_cmd() +
                [Utils.uint8(param_id),
                 Utils.uint16(Wlan.reserved), 
                 Utils.uint32(val)]))

        

    def execute(self,cmd_bytes):
        if Args.qxdm:
            print('send_data %s' % (' '.join(['0x'+x for x in cmd_bytes.split()])))
            return ''
        else:
            response=Utils.qdss_diag(cmd_bytes)
            print(response)
            


class ArgsException(Exception):
    pass

class Args:
    @classmethod
    def parse(self):
        self.parser = argparse.ArgumentParser(description='', formatter_class=argparse.RawTextHelpFormatter)
        self.parser.add_argument("-q", "--qxdm", action="store_true",
                                 help="Output send_data command usable from QXDM")
        self.parser.add_argument("-n", "--norun", action="store_true",
                                 help="Only display the qdssdiag command. Do not execute it.")

        self.parser.add_argument("-p", "--port", type=str,
                                 help="DIAG port number to use. If not specified, will try to determine this")
        self.parser.add_argument("-c", "--command_file", type=str,
                                 help="command file")



        self.parser.add_argument('command_string', nargs=argparse.REMAINDER,
                                 help=CommandProcessor.command_strings())
        self.parser.parse_args(namespace=self)
        if self.port:
            m=re.match('^(?:COM|com)?(\d+)$',self.port)
            if m:
                self.port="COM%s" %  (m.group(1))
            else:
                raise ArgsException('%s does not look like a COM port' % self.port)
        if not self.command_string and not self.command_file:
            raise ArgsException('No command specified')

        if (not self.qxdm):
            if (not self.port):
                if (self.norun):
                    self.port='COMx'
                else:
                    self.port=Utils.get_diag_port().strip()
            if (not self.port):
                raise Exception('Cannot find a unique DIAG port to use')
            
            if (not Utils.is_process_running('QPSTServer.exe') and not self.norun):
                raise Exception('QPST server is not running')

            


if __name__ == '__main__':
    try:
        Args.parse()
        if (Args.command_file):
            with open(Args.command_file) as f:
                for line in f.readlines():
                    line = line.partition('#')[0].strip()
                    if line:
                        CommandProcessor(line.split()).process()
        else:
            CommandProcessor(Args.command_string).process()
    except ArgsException as e:
        print('usage error : %s\n' % str(e))
        Args.parser.print_help()
    except Exception as e:
        print('Error: %s' % e)
        raise
  
