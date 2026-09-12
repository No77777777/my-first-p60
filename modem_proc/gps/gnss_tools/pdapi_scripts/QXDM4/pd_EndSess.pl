###########################################################################
# $Header: //components/rel/gnss8.mpss/11.1/gnss_tools/pdapi_scripts/QXDM4/pd_EndSess.pl#1 $
# $DateTime: 2019/10/24 01:30:36 $
# $Author: pwbldsvc $
#
# This example demonstrates using a DM Command to terminate 
# an onging GPS position fix via the PDAPI using CGPS_END_CMD
#
# In order to terminate another script which does CGPS_START_CMD, 
# CTRL-C that script first and then run this script. This can be used
# mainly to stop a tracking session.
#
# Before calling this script, a PDAPI client will need to be
# initialized (created) first. Also, this client will then need to register
# for PD events and activate itself before it can end a position
# session. (Run pd_InitClient.pl)
#
# Usage: perl pd_EndSess.pl 11 (11 is the com port the FFA is connected to)
#
###########################################################################

use vars qw(%opts);

use Win32::OLE;
use Win32::OLE::Variant;

use Getopt::Long;
use pd_EndSess;
use constant true                => 1;

%opts = ();
GetOptions (\%opts, 'help');

&usage() if ($opts{'help'});


######################################################################
# Create QXDM instance
######################################################################

# my $qxdm_app = new Win32::OLE 'QXDM.Application';
# if (!$qxdm_app)
# {
  # print "ERROR: Unable to invoke the QXDM application.\n";
  # die;
# }
my $IQXDM = new Win32::OLE 'QXDM.QXDMAutoApplication';
if (!$IQXDM)
{
  print "ERROR: Unable to invoke the QXDM application.\n";
  die;
}
			$qxdm_app = $IQXDM->GetAutomationWindow();
			$qxdm_app2 = $qxdm_app;
		$qxdm_app->SetVisible(true);

#####################################################################
# Prompt for user input if arguments are not provided
#####################################################################

$comport = $ARGV[0];

# Allow user input for comport
if (not defined ($comport)) {
    print("\nUsage: perl EndSess.pl <COM port>\n\n");
    print ("Please enter the COM port the FFA is connected to\n");
    $count = 1;
    chomp ($comport = <STDIN>);
    print("\n");
}

######################################################################
# Open QXDM Connection by setting the COM port from command line.
######################################################################

# COM Port value should be supplied as first argument to this script
$qxdm_app->{COMPort} = $comport;

# Wait until phone is connected
while ($qxdm_app->{IsPhoneConnected} == 0)
{
  print "Unable to Connect to the Phone. Retrying...\n";
  sleep(1);
}

######################################################################
# Global variables initialized here
######################################################################

my @client_ids;
# Set the timeout to 2 seconds.
my $timeout = 2000;

######################################################################
# Build a diagnostic request for CGPS_END_CMD
######################################################################

$req_endsess = new pd_EndSess;
$req_endsess->{"cmd_code"}        = 75;  # Diag Cmd Code
$req_endsess->{"subsys_id"}       = 13;  # Diag Subsystem ID
$req_endsess->{"subsys_cmd_code"} = 100; # Diag Subsystem Cmd Code
$req_endsess->{"cgps_cmd_code"}   = 11;  # CGPS Cmd Code
$req_endsess->{"version"}         = 1;   # Version Number
$req_endsess->{"end_type"}        = 0;   # 0 = Position Session
                                         # 1 = Data Download
$req_endsess->{"client_id"}       = 1001; # Client Id of the ongoing session

  ######################################################################
  # Construct the DM CGPS_END_CMD capture request packet
  ######################################################################

  my $req_endsess_pkt = $req_endsess->SetCaptureCmd();

  ######################################################################
  # Send command request packet, receive command response packet
  ######################################################################

  my $rsp_endsess_pkt = $qxdm_app->SendDmIcdPacketEx($req_endsess_pkt, $timeout);

  my $rsp_endsess_len = length($rsp_endsess_pkt);
  printf ("DM CGPS_END_CMD Response Packet Length: %d\n", $rsp_endsess_len);

  $req_endsess->ParseData($rsp_endsess_pkt);

  $status    = $req_endsess->GetStatus();
  $rsp_client_id = $req_endsess->GetClientId();
  printf ("Status = %d, Client Id = %d\n\n", $status, $client_id);
  
  $qxdm_app = NULL;

######################################################################
# METHOD:
#   usage
#
# DESCRIPTION:
#   Display usage information.
#
# RETURN VALUE:
#   None
#
######################################################################
sub usage
{
  my $usage = <<END;

EndSessionCmd

Syntax:   pd_EndSess [options]

Options:  -help               Display this help message.

Example:  perl pd_EndSess.pl <COM Port>

END

  print "$usage";
  exit (0);
}