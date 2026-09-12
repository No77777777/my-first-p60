#!/pkg/qct/bin/perl

# Convert a raw IU memory dump to a core file.
#
# Usage:
# makecore.pl [options] input.bin output.core
# Options:
#   --elf file.elf
#   Specify an ELF file for reading saved registers, which should contain
#   the iu_core_intf structure. If not specified, register state defaults
#   to all zero.

use strict;
use warnings;
use Getopt::Long;
use XML::Simple;
use Data::Dumper;
use FindBin;
require 5.6.0;


# Get command line options
my $elf = undef;
GetOptions('elf=s' => \$elf);

@ARGV == 2 or die "Usage: makecore.pl [--elf file.elf] input.bin output.core\n";
my $input = shift @ARGV;
my $output = shift @ARGV;


# Corefile header.
my @header = (
  # ELF header
  0x464c457f, 0x00010101, 0x00000000, 0x00000000,  # e_ident
  0x81110004,   # e_type/e_machine
  0x00000001,   # e_version
  0x00000000,   # e_entry
  0x00000034,   # e_phoff
  0x00000000,   # e_shoff
  0x00000000,   # e_flags
  0x00200034,   # e_phentsize/e_ehsize
  0x00000002,   # e_shentsize/e_phnum
  0x00000000,   # e_shstrndx/e_shnum

  # PHDR[0]: PT_NOTE
  0x00000004,   # p_type
  0x00000074,   # p_offset
  0x00000000,   # p_vaddr
  0x00000000,   # p_paddr
  0x00000104,   # p_filesz
  0x00000000,   # p_memsz
  0x00000000,   # p_flags
  0x00000000,   # p_align

  # PHDR[1]: PT_LOAD
  0x00000001,   # p_type
  0x00000178,   # p_offset
  0x00000000,   # p_vaddr
  0x00000000,   # p_paddr
  0x00000000,   # p_filesz : offset[25]
  0x00000000,   # p_memsz  : offset[26]
  0x00000007,   # p_flags
  0x00000000,   # p_align

  # PT_NOTE section
  0x00000008,   # namesz
  0x000000f0,   # descsz
  0x00000001,   # type
  0x45524f43, 0x00000000,  # name
  # NT_PRSTATUS header/unused
  0x00000000, 0x00000000, 0x00000000, 0x00000005,
  0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000,
  # index[52]: GPR0..31
  0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000,
  # index[84]: SPR0..8
  0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000,
  # index[93] : PC
  0x00000000
);


open INPUT, $input or die "Can't open input file '$input'";
binmode INPUT;
my $size = (stat(INPUT))[7];
my $data;
read INPUT, $data, $size;

# Fill in header with file size data
$header[25] = $header[26] = $size;

# If ELF file is provided, read and populate saved registers.
if ($elf) {

  # Attempt to find registers in iu_core_reg, or iu_core_intf
  my $nm = set_regs_nm();
  if ($nm) {
    my $elftool = set_regs_elftool();
    if ($elftool) {
      die "Can't find register structures in '$elf', errors follow:\n".join("\n",$nm,$elftool);
    }
  }

} else {
  print STDERR "Note: No ELF file provided, register state will default to zero.\n";
}

# Write output file with header+raw image data
open OUTPUT, ">$output" or die "Can't open output file '$output'";
binmode OUTPUT;
print OUTPUT pack("V*",@header);
print OUTPUT $data;

0;


# Set registers for simple register save structures, using nm.
sub set_regs_nm {

  my $cmd = "/pkg/qct/software/iu/releases/tools/1.0.01/Tools/bin/iu-nm -print-size -numeric-sort $elf";
  eval { no warnings 'all'; open NM, "$cmd|" or die "failed"; };
  return "Can't execute '$cmd': $@" if ($@);
  my @sym = grep { /\siu_core_regs$/ } <NM>;
  close NM or return "iu-nm error: $? $!";

  if (@sym != 1) { return "Can't find iu_core_regs"; };
  chomp $sym[0];

  $sym[0] =~ /^([0-9a-fA-F]+) \s+ ([0-9a-fA-F]+) \s+ \S+ \s+ iu_core_regs$/x or return "Can't parse nm output '$sym[0]'";
  my ($addr,$size) = (hex($1),hex($2));

  return set_core_regs($addr,$size);
}


# Set registers using elftool for complex structures.
sub set_regs_elftool {

  my $elftool = "$FindBin::Bin/../../../tools/elftool";
  -f $elftool or return "Can't find '$elftool'";
  my $cmd = "$elftool --symbol-dump iu_core_intf $elf";
  eval { no warnings 'all'; open ELFTOOL, "$cmd|" or die "failed"; };
  return "Can't open '$cmd': $@" if ($@);
  my @sym = <ELFTOOL>;
  close ELFTOOL or return "elftool error: $? $!";

  my $s = XMLin(join("",@sym),
                forcearray => 0,
                keyattr => [ 'name' ] );

  exists($s->{symbol}{member}{GPR}{addr}) or return "Can't find iu_core_intf 'GPR'";
  my $gpr_addr = hex($s->{symbol}{member}{GPR}{addr});
  $s->{symbol}{member}{GPR}{size} == 32*4 or return "GPR size incorrect in iu_core_intf";

  exists($s->{symbol}{member}{SPR}{addr}) or return "Can't find iu_core_intf 'SPR'";
  my $spr_addr = hex($s->{symbol}{member}{SPR}{addr});
  $s->{symbol}{member}{SPR}{size} == 9*4 or return "SPR size incorrect in iu_core_intf";

  return set_core_regs($gpr_addr, ($spr_addr - $gpr_addr + 9*4));
}


sub set_core_regs {
  my $addr = shift;
  my $size = shift;

  $size == 49*4 or return sprintf("Got incorrect register size %d, expected %d (for addr 0x%08x)", $size, 49*4, $addr);
  

  
  my @reg = unpack("V*", substr($data,$addr,41*4));
  (@reg == 41) or return sprintf("Could not parse required number of registers (got %d, expected %d) for addr 0x%08x", scalar(@reg), 41, $addr);

  @header[52..92] = @reg;

  # PC = NMI_RETPC since this was exception-saved.
  $header[93] = $reg[32+5];

  return undef;
}



