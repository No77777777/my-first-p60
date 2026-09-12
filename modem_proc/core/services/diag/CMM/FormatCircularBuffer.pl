#========================================================================
#
# FormatCircularBuffer.pl
#
# This perl file parses the Data Blocks in the Circular buffer and generates a QMDL file.
# The input to this perl file are as below:
# 1. Circular Buffer Dump
# 2. QMDL File output Path
# 3. Circular Buffer Head
# 4. Circular Buffer Tail
# Output of the perl script is the QMDL file with circular buffer data.
#
#
# Copyright (c) 2016-2018 by QUALCOMM Technologies, Inc
#=======================================================================

#==========================================================================
#
#                       EDIT HISTORY FOR MODULE
#
#when       who     what, where, why
#--------   ---     ------------------------------------------------------------
#07/31/18   gn      Fixed bug in token file parsing resulting in APEX failure while opening qmdl2 file
#06/13/17   gn      Added support for version 2 qmdl2 
#04/20/17   gn      Added support for data tagging with Diag ID
#03/07/17   gn      Added logs for debugging
#01/13/16   gn      Created.

#===========================================================================*

my $circular_data_out;
my $circular_data_in ;
my $head ;
my $tail ;
my $i=0,$item;
my $buffer = "";
my $pattern = "";
my $hashfile,$hashfile1,$hashfile2;
# first_block is 1 if we are checking the first block from the tail in the circular buffer
my $first_block=1;
my $diag_tagging_support=0;  
my $diag_tag_header_len=0x4;
my $err_count=0;
my $max_err_count=10;

my @hashes,@hashbytes;
my @header = (0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);
my @header_v2 = (0x00,0x00,0x00,0x00);

my $filename;
my $token_file, $token, $payload;

my @params,@diagID_table,$diagID_tbl_idx=0;
my @guid_table,$guid_tbl_idx=0;

# Use this variable to switch between different versions of qmdl2
my $QMDL2_VERSION = 0x2;
printf("\nqmdl2 version %d is enabled", $QMDL2_VERSION);

#First argument represents the Token file
if (defined($ARGV[0])) 
{
    $filename = $ARGV[0];
	printf ("\n\nToken file            : %s", $filename);
} 
else 
{ 
	printf "ERROR: Token file not specified.\n";
    die "Token file not specified.\n";
}

# Open token file 
open($token_file, '<:encoding(UTF-8)', $filename) or die "Could not open file '$filename' $!";

#Parse through each line of token file and read the parameters based on TOKEN
while (<$token_file>)
{
    $row = $_;
    if (/^(\w*) (.*)/)
	{
		$token = $1;
		$token =~ s/[\r\n]//;
		$payload = $2;
		$payload =~ s/[\r\n]//;
		#printf "$token $payload\n";
		
		if($token eq "INPUT_FILE")
		{
			$circular_data_in = $payload;
			printf ("\nInput bin file        : %s", $circular_data_in);
		}
		elsif($token eq "OUTPUT_FILE")
		{
			$circular_data_out = $payload;
			printf ("\nOutput qmdl file      : %s", $circular_data_out);
		}
		elsif($token eq "HEAD")
		{
			$head = hex($payload);
			printf ("\nCircular buffer head  : 0x%x", $head);
		}
		elsif($token eq "TAIL")
		{
			$tail = hex($payload);
			printf ("\nCircular buffer tail  : 0x%x", $tail);
		}
		elsif($token eq "TAGGING")
		{
			$diag_tagging_support = hex($payload);
			printf ("\nDiag Tagging support  : 0x%x", $diag_tagging_support);
		}
		# Add the GUIDs to guid_table
		elsif($token eq "GUID")
		{
			@params = split / /, $payload;
			$tbl_idx = 0;
			if(hex($params[$tbl_idx])!=0)
			{
				foreach(@params)
				{
					$guid_table[$guid_tbl_idx][$tbl_idx] = $params[$tbl_idx];
					$tbl_idx++;
				}
				printf "\nGUID                  : $guid_table[$guid_tbl_idx][0] $guid_table[$guid_tbl_idx][1]";
				$guid_tbl_idx++;
			}
		}		
		# Add the diagID structure fields to diagID_table
		elsif($token eq "diagID")
		{
			@params = split / /, $payload;
			$tbl_idx = 0;
			foreach(@params)
			{
				$diagID_table[$diagID_tbl_idx][$tbl_idx] = $params[$tbl_idx];
				$tbl_idx++;
			}
			printf "\ndiagID structure      : \n    diagID            : $diagID_table[$diagID_tbl_idx][0] \n    diagID name       : $diagID_table[$diagID_tbl_idx][1] \n    GUID              : $diagID_table[$diagID_tbl_idx][2] $diagID_table[$diagID_tbl_idx][3]\n";
			$diagID_tbl_idx++;
		}
	}    
}

if($diagID_tbl_idx == 0)
{
	printf "\n\nForce switch to qmdl2 version 1 since this build doesnt have diagID support for QSH trace";
	$QMDL2_VERSION = 1;
}

open (INFILE,$circular_data_in) or die "Not able to open the $circular_data_in file. \n";
binmode(INFILE);

open (OUTFILE, ">", $circular_data_out) or die "Not able to open the file for writing. \n";
binmode (OUTFILE);


#QMDL2 Header (Version 1)
# Field                 Length (in bytes)     Description 
# HeaderLength             4                  Number of bytes reserved for header
# Version                  1                  Version value to set (0)
# HDLC DataType            1                  0 - indicates hdlc encoding removed
#                                             1-indicates hdlc encoding enabled
# GuidListEntryCount       4                  Number of Guids available to read
# GUID array               16*                An array of available GUIDs.This can expand upto 
#											  the maximum number of GUIDlist entries reserved in diag.

#Printing Header to QMDL file 
#Sample Header : 1A 00 00 00         01          00      01 00 00 00    B9 E2 67 63 1B 62 E8 7E F6 F9 A1 D5 DA 48 4D 92
#                Header Length(4) Version(1) HDLCtype(1) GUIDcount(4)                  GUIDarray    

#QMDL2 Header (Version 2)
# Field                 Length (in bytes)     Description 
# HeaderLength             4                  Number of bytes reserved for header
# Version                  1                  Version value to set
#												1 – Contains GUID entry information
#												2 – indicates DIAG ID <--->GUID mapping is present in Header 
#													following the version 1 GUID entry information.
# HDLC DataType            1                  0 - indicates hdlc encoding disabled
#                                             1 - indicates hdlc encoding enabled
# GuidListEntryCount       4                  Number of Guids available to read
# GUID array               16*                An array of available GUIDs.This can expand upto 
#											  the maximum number of GUIDlist entries reserved in diag.
# DiagIDCount              4                  Number of DIAG IDs available to read
# [DiagID,DiagID_name,     47*                An array of sets of Diag ID, Diag ID string and GUID.
# GUID set array]                             Diag ID1, Diag ID1 string, GUID1, DIAG ID2, Diag ID2 string, GUID2 ….

#Printing Header to QMDL file 
#Sample Header : 
#1A 00 00 00         01          00      01 00 00 00    B9 E2 67 63 1B 62 E8 7E F6 F9 A1 D5 DA 48 4D 92  01 00 00 00   02     mdm/modem/root_pd   B9 E2 67 63 1B 62 E8 7E F6 F9 A1 D5 DA 48 4D 92
#Header Length    Version      HDLCtype   GUIDcount                    GUIDarray                         diagIDcount  DiagID    DiagID string                        GUID 

# If version 1 is enabled write Header, version(1), HDLCtype, GUIDcount and GUIDarray
if($QMDL2_VERSION == 1)
{
	# Calculate the header length based on the GUIDs available
	$header[0] = 4 + 1 + 1 + 4 + $guid_tbl_idx * 16;
	# update qmdl2 version
	$header[4] = $QMDL2_VERSION;
	# update no of GUIDs available
	$header[6] = $guid_tbl_idx;
	# Write header till GUIDcount
	for ($i=0; $i < 10; $i++){ print OUTFILE (chr $header[$i]);}
	# Write GUID array
	Write_GUID_array();
}

# If version 1 is enabled write Header, version(1), HDLCtype, GUIDcount, GUIDarray, diagIDcount, DiagID array (DiagID, DiagID name and GUID associated with DiagID)
elsif($QMDL2_VERSION == 2)
{
	if($diagID_tbl_idx > 0)
	{
		# Calculate the header length based on the GUIDs and diagIDs available
		$header[0] = 4 + 1 + 1 + 4 + $guid_tbl_idx * 16 + 4 + $diagID_tbl_idx * 47;
		# update qmdl2 version
		$header[4] = $QMDL2_VERSION;
		# update no of GUIDs available
		$header[6] = $guid_tbl_idx;
		# Write header till GUIDcount
		for ($i=0; $i < 10; $i++){ print OUTFILE (chr $header[$i]);}
		# Write GUID array
		Write_GUID_array();
		# Update no of diagIDs available
		$header_v2[0] = $diagID_tbl_idx;
		# Write header till diagIDcount
		for ($i=0; $i < 4; $i++){ print OUTFILE (chr $header_v2[$i]);}
		# Write diagID array
		Write_diagID_array();
	}
}

sub Write_GUID_array
{
	$idx = 0;
	while($idx < $guid_tbl_idx)
	{
		# Write the hashes to the output file
		Write_Hashes($guid_table[$idx][0],$guid_table[$idx][1]);
		$idx++;
	}
}
sub Write_diagID_array
{
	#QShrink Hash file
	$idx = 0;
	$i = 0;
	while($idx < $diagID_tbl_idx)
	{
		# Write the diagID to the output file
		print OUTFILE (chr hex $diagID_table[$idx][0]);
		$out_pos = tell(OUTFILE);
		# Zero out 30 characters before writing diagID name
		for($i=0;$i<30;$i++){print OUTFILE (chr hex 0);}
		seek(OUTFILE, $out_pos,0);
		# Write the diagID name to the output file
		print OUTFILE ($diagID_table[$idx][1]);
		seek(OUTFILE, $out_pos+0x1E,0);
		# Write the hashes to the output file
		Write_Hashes($diagID_table[$idx][2],$diagID_table[$idx][3]);
		$idx++;
	}
}

sub Write_Hashes
{
		$hashfile1 =$_[0];
		$hashfile1 =~ s/0x//;
		$hashfile2 =$_[1];
		$hashfile2 =~ s/0x//;
		$hashfile = join("",$hashfile1, $hashfile2);
		#Splitting the hash string into 2character array 
		@hashes = split //, $hashfile; 
		$i=0;
		foreach (@hashes) {$hashbytes[$i/2].=$hashes[$i];$i+=1;}
		$i=0;
		foreach (@hashbytes) {print OUTFILE (chr hex $hashbytes[$i]); $i+=1;}
		# @hashbytes = (B9,E2,67,63,1B,62,E8,7E,F6,F9,A1,D5,DA,48,4D,92)
		# Printing the GUID to qmdl file 
		# printf "\nHash file GUID        : ";
		# $i=0;
		# foreach (@hashbytes) {print "$hashbytes[$i] "; $i+=1; }
		@hashes = ();
		@hashbytes = ();
}

#Start processing from Tail
seek(INFILE, $tail,0);
printf "\n\n**************** Starting file processing from Tail ****************\n";

# Process the Header of the first Data block
ProcessHeader();   
	
#Read length bytes from the input file 
while(read(INFILE, $buffer, ($length)) !=0 )
{
   # Copy all the data from the input file to qmdl file
   foreach (split(//,$buffer)) 
   {
      $item = sprintf("%02x", ord($_));  
      print OUTFILE (chr hex $item);
   }
   
   # Adjust for padding
   $pad=(($length+3)& (~3)) - $length;
   seek(INFILE, $pad,1);
   
   $length = $length + $pad;
   printf ("\nLength of the block   : 0x%x\n", $length);
   
   $file_pos = tell(INFILE);
   
   #Check for the wrap case
	if ($file_pos == $head+$pad)
   {
		printf ("\n\n************ END OF FILE PROCESSING ************");
        exit;
   }

# Process the Header of the next Data block
ProcessHeader(); 

}

	 
# Block structure : Header + Data 
#
# Header :
# Pattern : DEAD : 2 Bytes
# Length of block : 2 Bytes
#
# Data (4byte aligned) (Non HDLC encoded)
# Starting character    : 7E
# Version               : 01
# Length 			    : Length of payload (2 Bytes)
# Payload               : Data
# Terminating character : 7E

#Sample Data in the block
# ________address|________0________4________8________C_0123456789ABCDEF
     # D:8AF15A90|>189EDEAD 0013017E 00110099 0D650400 ....~.........e.
     # D:8AF15AA0| 0000013B 00005C6F 7E004D92 0013017E ;...o\...M.~~...
     # D:8AF15AB0| 00110099 0D6A1800 0000013B 00005C6E ......j.;...n\..
     # D:8AF15AC0| 7E004D92 0013017E 00110099 0D6D8000 .M.~~.........m.

# Subroutine to process the Header of the Data block 
sub ProcessHeader{

use constant DIAGBUF_PATTERN1 => 0xDE;
use constant DIAGBUF_PATTERN2 => 0xAD;

$file_pos = tell(INFILE);
printf ("\nCurrent file position : 0x%x", $file_pos);

 #Read the pattern of the Block
   if(read(INFILE, $pattern,1) !=0)
   {
      $code =sprintf("%02x",ord($pattern));
      $code_hex2 = $code;
	  
	  read(INFILE, $pattern,1);
	  $code =sprintf("%02x",ord($pattern));
      $code_hex1 = $code;
	        
      if ((hex($code_hex1) != DIAGBUF_PATTERN1) && (hex($code_hex2) != DIAGBUF_PATTERN2))
      {
		 printf "\n\n**************** ERROR: DEAD pattern missing at the beggining of the block ****************\n";
		 if($first_block==1)
		 {
			#Buffer header corrupted, Should never happen ideally
			printf "\n\n**************** ERROR: Invalid Circular Buffer pattern for the packet: $code_hex ****************\n";
			die ("Invalid Circular Buffer pattern for the packet: $code_hex \n");
		 }
		 else
		 {
			if($err_count>=$max_err_count)
			{
				printf "\n\n****************  Reached maximum no of errors, Exiting ****************\n";
				exit;
			}
			$err_count++;
			#Reached end of buffer need to wrap
			$file_pos=0;
			seek(INFILE,$file_pos,0);
			printf "\n\n****************  Reached end of file, wrapping around ****************\n";
			ProcessHeader();
			return;
		 }
      } 
   }
   
   #Done processing first block header, set first_block to 0
   $first_block=0;
   
   #Read the lower byte of the length
   if(read(INFILE, $length,1)!=0)
   {
     $length1 = sprintf("%02x", ord($length));
   }
   else
   {
      printf ("ERROR: Unable to read the lower byte of the Length\n");
      die ("Unable to read the lower byte of the Length\n");
   }
   
   #Read the upper byte of the length   
   if(read(INFILE, $length,1)!=0)
   {
      $length2 = sprintf("%02x", ord($length));     
   }
   else
   {
	  printf ("ERROR: Unable to read the Upper byte of the Length\n");
      die ("Unable to read the Upper byte of the Length\n");
   }
   
   #Calculate the length of the Block
   $length=join("",$length2, $length1);
   $length=sprintf("%d", hex($length)); 
   
   if($diag_tagging_support==0x1)
   {
	  $length = $length - $diag_tag_header_len;
	  seek(INFILE,$diag_tag_header_len,1);
   }   
} 
close (TOKEN) or die "Not able to close the file: $tokenfile \n";
close (INFILE) or die "Not able to close the file: $infile \n";
close (OUTFILE) or die "Not able to close the file: $outfile \n";