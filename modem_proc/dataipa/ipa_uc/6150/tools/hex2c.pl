#!/usr/bin/perl
#! -*- perl -*-
eval 'exec perl -x -S $0 ${1+"$@"} ;'
  if 0;

use strict;
use English;

my (%records);
my (%a_start);
my (%a_end);
my ($cnt)=0;
while ($_=scalar(<>)) {
  s/\r//; chomp;
#  print "$_\n";
  my $binrec = pack( 'H*', substr( $_ , 1 ) );
  die "Checksum error" unless unpack( "%8C*", $binrec ) == 0;
#  print "\n".join(",",unpack ("C*", $binrec))."\n";
  my( $addr, $type, $data ) = unpack( "x n C X4 C x3 /a", $binrec);
  my (@data) = unpack("C*",$data);
  push (@data,0) while ($#data %4 != 3);
  $data=pack("C*",@data);
  next unless ($type == 0); # ignore all except data records
#  print "$addr:" . unpack ('H*', $data) . "\n";
  @data = unpack ("L*",$data);
#  print ":XXXXXXXX".join("",map {sprintf("%08X",$_)} @data)."XX\n";
  die "Duplicate address" if exists($a_start{$addr});
  if (1 && exists($a_end{$addr})) {
    my ($record)=$a_end{$addr};
    $record->{'end'} = $addr + ($#data+1)*4 -1;
    push (@{$record->{'data'}},@data);
    delete $a_end{$addr};
    $a_end{$record->{'end'}+1}=$record;
  } else {
    my ($record) = {
                    'id'=>$cnt++,
                    'start'=>$addr,
                    'end'=>($addr + ($#data+1)*4 -1),
                    'data' => [@data],
                   };
    $a_start{$addr}=$record;
    $a_end{$record->{'end'}+1}=$record;
    $records{$record->{'id'}}=$record;
  }
}

# attempt to merge more
my ($merged)=1;
while ($merged) {
  $merged = 0;
  foreach my $addr (keys %a_start) {
    if (exists $a_end{$addr}) {
      my (@data)=@{$a_start{$addr}{'data'}};
      my ($record)=$a_end{$addr};
      $record->{'end'} = $addr + ($#data+1)*4 -1;
      push (@{$record->{'data'}},@data);
      delete $records{$a_start{$addr}{'id'}};
      delete $a_start{$addr};
      delete $a_end{$addr};
      $a_end{$record->{'end'}+1}=$record;
      $merged=1;
    }
  }
}

use constant LINE_LEN=>8;

my ($comma)=(" ");
foreach (sort {$a->{'start'} <=> $b->{'start'}} (values %records)) {
#  print "".($_->{'start'})." ... ".($_->{'end'})." : ".join(",",@{$_->{'data'}})."\n";
  my ($pData)=$_->{'data'};
  print "${comma}/*Addr=*/".sprintf("0x%08X",$_->{'start'}).",/*Length=*/".sprintf("0x%08X",($#{$pData}+1))."\n";
  $comma=",";
  for (my $i=0; $i <= $#{$pData}; $i+=LINE_LEN) {
    my ($last)=$i + LINE_LEN -1; $last = $#{$pData} if ($last > $#{$pData});
    print "\t$comma".join(",",map {sprintf("0x%08X",$_)} (@{$pData}[$i...$last]))."\n";
  }
  print "/*Last address: ".sprintf("0x%08X",$_->{'end'})."*/\n\n";
}
