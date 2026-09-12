################################################################################
# $Header: //components/rel/gnss8.mpss/11.1/gnss_tools/pdapi_scripts/QXDM4/XTRA_Injection/pd_XtraDiagClient.pl#1 $
# $Revision: #1 $
# $DateTime: 2019/10/24 01:30:36 $
#
# Parses the test configuration file, Fetches the current XTRA bin file from XTRA
# server as per configuration,Gets SNTP time FROM SNTP server,
# passes the file to Injector and Starts Power tests if configured.
#
# NOTE : Look at XtraDiagInjector.cfg for TEST CONFIGURATION details
#
# Copyright (C) 2010 Qualcomm, Inc.
#
# usage: perl XtraDiagClient.pl 
#
#
# Revision History
# when       who    what, where, why
# --------   ---    ----------------------------------------------------------
# 08/23/2010 ssk    New XTRA Diag Client
################################################################################

use strict;

our $client_id_test2;
our $client_id_xtra;
our $qxdm_app;
our $qxdm_app2;
our $clientObjectGlobal;
our $ReqHandle;
our $CurrIndex;
our $PrevIndex;

our $QOS; 
our $COMPORT1;
our $START_POWER_TEST ; 
our $TBF; 
our $FIXCOUNT; 
our $USE_COARSE_POS; 
our $ALTITUDE; 
our $LONGITUDE; 
our $LATITUDE; 
our $LOC_UNC_HOR; 
our $LOC_UNC_VERT; 
our $CONFIDENC_HOR; 
our $CONFIDENCE_VERT; 
our $ALT_DEF; 
our $MAXSEG_SIZE; 
our $DIAGHDR_SIZE;
our $INJECT_COARSE_TIME; 
our $XTRASNTPURL1; 
our $XTRASNTPURL2; 
our $XTRASNTPURL3; 
our $XTRASNTPURL4; 
our $XTRASNTPURL5; 
our $XTRASNTPURL6; 
our $XTRAHTTPURL; 
our $RESETPHONE;
our $SM_VERSION;  # tells us if this is Gen7 or Gen8 system
our $XTRAFILESIZE;
our $GlobalGPSTime;
our $GlobalUTCTime;

use Time::HiRes qw( usleep  );

# use lib "..\\..\\PdapiScripts";
# use lib "..\\..\\pdapi_scripts";
use lib '..\\XTRA_Injection';    

use pd_DelAll;
use pd_SetTimeInfo;
use pd_GpsClientsStartStopXTRA3;
use pd_ExternCoarsePosInj;
#use pd_PowerTester;
use pd_XtraBinFileInject;
#use pd_SntpGetTime;

our $currentDir = cwd ();
$currentDir =~ s/\//\\/g;
print "current dir path: $currentDir \n"; 

use Win32::OLE;
use Win32::OLE::Variant;
use POSIX;

use Getopt::Long;
use Cwd;
use pd_StartDiag;
use pd_EndDiag;

# Set the timeout to 2 seconds.
use constant TIMEOUT => 2000;

# CGPS client type
use constant CLIENT_TYPE => 8;

# Clear options for single sv track
use constant SINGLE_SV_CLR => 1;
use constant SINGLE_SV_NOCLR => 2;

use LWP::Simple;
use File::Copy;

my $XtraDataFileDone = 1;
our  $MAXSEG_SIZE = 1960;
our $DIAGHDR_SIZE = 40;   


# print ("Please enter the COM port the FFA is connected to\n");
    my $count = 1;
    my $COMPORT =$ARGV[0];
    # chomp (my $COMPORT = <STDIN>);
	
	# print ("Comport is $COMPORT \n");

#################################
# Initialize the configuration  #
#################################

$SM_VERSION = 7;

my $Logfile = "XtraInjectorLogs.txt";
open LOGFILE, ">$Logfile" or die "failed to write to savefile $Logfile\n";

print LOGFILE  "Version: pd_XtraDiagClient.pl#1 \n\n";


#####################################
# Sub routines Section              #
#####################################

################################################################################
# METHOD:
#   usage
#
# DESCRIPTION:
#   This subroutine resets the phone
#
# RETURN VALUE:
#   None
#
################################################################################
sub reset_phone
{ 

  ##############################################################################
  # Delete all of the CGPS parameters (alm, eph, XTRA Data etc.)
  ##############################################################################
  # &cgps_delete;
  
  ##############################################################################
  # Reset the device if needed
  ##############################################################################
  if ($RESETPHONE == 1)
  { 
    &reset_device;
  }
} # End of reset_phone


################################################################################
# METHOD:
#   usage
#
# DESCRIPTION:
#   This subroutine downloads the XTRA.bin file
#
# RETURN VALUE:
#   None
#
################################################################################
sub GetXtraDataFile
{ 
  my $content;
  print LOGFILE ("Downloading XTRA data file from  = %s \n", $XTRAHTTPURL);

  # Download latest XTRA file

  chomp($XTRAHTTPURL);
  $XTRAHTTPURL = lc($XTRAHTTPURL);
  unless (defined ($content = get "$XTRAHTTPURL"))
  {
    warn "could not get file from $XTRAHTTPURL \n \n";
    return -1;
  }

  # save to disk as "xtra.bin" since this is the file used for both XTRA1 and XTRA2
  my $savefile = "xtra.bin";
  open SAVEFILE, ">$savefile" or die "failed to write to savefile $savefile\n";
  binmode(SAVEFILE);
  print SAVEFILE "$content";
  close SAVEFILE;

  # check for bad file download
  my $Localfilesize = -s $savefile ;

  print LOGFILE  "\nValidating download $savefile filesize = $Localfilesize \n";
  open SAVEFILE, "$savefile" or die "Error opening $savefile for output.\n";
  my $savefile_opened = 1;
  if ($savefile_opened == 1)
  {
    print LOGFILE  "\n$savefile file check passed\n";
    close SAVEFILE;  
    return 1;
  }
  else
  {
    return -1;
  }

} # End of GpsInvokeClientStart


################################################################################
# METHOD:
#   usage
#
# DESCRIPTION:
#   This subroutine Init and register for the client, delete all old almanac information
#
# RETURN VALUE:
#   None
#
################################################################################
sub GpsInvokeClientStart
{ 

  &GpsClientStart($COMPORT);

  print LOGFILE ("Starting Clients XtraClientId =" . sprintf("%d",$client_id_xtra). ",test2_id =". sprintf("%d",$client_id_test2) . "\n\n"); 

} # end sub GpsInvokeClientStart


################################################################################
# METHOD:
#   usage
#
# DESCRIPTION:
# Stop the client
#
# RETURN VALUE:
#   None
#
################################################################################
sub GpsClientsStop
{ 
  &GpsClientStop;
  print LOGFILE  "\n Clients Stopped \n";
} # end  of sub GpsClientsStop




################################################################################
# METHOD:
#   usage
#
# DESCRIPTION:
#   This subroutine Injects the XTRA Data
#
# RETURN VALUE:
#   None
#
################################################################################
sub InjectXtraData
{ 

  #Let us initialize, Register, & Activate the client
  &GpsInvokeClientStart;

  $PrevIndex = $qxdm_app2->GetClientItemCount( $ReqHandle ) - 1;   
 




  #lets call the XTRA bin file injector 
  # we call this in a loop for the different XTRA URLs in the config file.
  my $i;
  for($i = 0; $i < 1; $i++)
  {
    my $filename = $currentDir."\\xtra.bin"; #'/path/to/your/file.doc';
    $XTRAFILESIZE = -s $filename;
    printf LOGFILE ("\n Size of Xtra Bin file is: %d \n", $XTRAFILESIZE);  
    
    &GpsXtraBinFileSend;
  }

  &GpsClientEventsProc;
  $PrevIndex = $CurrIndex;
  $CurrIndex = $qxdm_app2->GetClientItemCount( $ReqHandle ) - 1; # Get index of last item in client

} # End of InjectXtraData



################################################################################
# Main Body of The XTRA diag Client
################################################################################

#binmode(LOGFILE);
print LOGFILE "***************START OF XTRA INJECTOR LOGS *********************\n\n";
  

#Inject Xtra Data;
&InjectXtraData;

print LOGFILE "\n\n**********************     END OF XTRA INJECTOR LOGS   *******************\n";


&GpsClientEventsProc;

close LOGFILE;
#################END OF FILE###################


