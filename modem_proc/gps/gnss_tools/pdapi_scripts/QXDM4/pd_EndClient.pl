######################################################################
# $Header: //components/rel/gnss8.mpss/11.1/gnss_tools/pdapi_scripts/QXDM4/pd_EndClient.pl#1 $
# $DateTime: 2019/10/24 01:30:36 $
# $Author: pwbldsvc $
#
# This example demonstrates using a DM Command to Deactivate, DeRegister,
# and release a PDAPI client using CGPS_API_CMD.
#
# This perl script should be called, after all the positioning sessions have
# ended to release the PDAPI client
#
# Values that need to be modified
# 1. //MODIFY_ME: Client Type should be modified in the line
#    $clientType = 8; # Default client type is TEST2
#
# Usage: perl pd_EndClient.pl 11 (11 is the com port the FFA is connected to)
#
######################################################################

use vars qw(%opts);

use Win32::OLE;
use Win32::OLE::Variant;

use Getopt::Long;
use pd_EndClient;
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
# Promt for user input if arguments are not provided
#####################################################################

$comport = $ARGV[0];

# Allow user input for comport
if (not defined ($comport)) {
    print("\nUsage: perl EndClient.pl <COM port>\n\n");
    print ("Please enter the COM port the FFA is connected to\n");
    $count = 1;
    chomp ($comport = <STDIN>);
    print("\n");
}

######################################################################
# Open QXDM Connection by setting the COM port from command line. 
######################################################################

# COM Port value should be supplied as first argument to this script
$qxdm_app->{COMPort} = $comport; # Default is COM1

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
# Build a diagnostic request for CGPS_DEACTIVATE_CMD to deactivate the client
# //MODIFY_ME: Set Client Type here.
######################################################################
$clientType = 8; # Default client type is TEST2

$req_apicmd1 = new pd_EndClient;
$req_apicmd1->{"cmd_code"}        = 75;  # Diag Cmd Code
  $req_apicmd1->{"subsys_id"}       = 13;  # Diag Subsystem ID
  $req_apicmd1->{"subsys_cmd_code"} = 100; # Diag Subsystem Cmd Code
  $req_apicmd1->{"cgps_cmd_code"}   = 8;   # CGPS Cmd Code
  $req_apicmd1->{"version"}         = 1;   # Version Number
  $req_apicmd1->{"client_id"}       = 1001; # Client Type

  ######################################################################
  # Construct the DM CGPS_API_CMD capture request packet
  ######################################################################

  my $req_apicmd_pkt = $req_apicmd1->SetCaptureCmd();
  printf ("Sending CGPS_API_CMD: Deactivate client type %d\n", $clientType);

  ######################################################################
  # Send command request packet, receive command response packet
  ######################################################################

  my $rsp_apicmd_pkt = $qxdm_app->SendDmIcdPacketEx($req_apicmd_pkt, $timeout);

  my $rsp_apicmd_len = length($rsp_apicmd_pkt);
  printf ("DM CGPS_API_CMD Response Packet Length: %d\n ", $rsp_apicmd_len);

  $req_apicmd1->ParseData($rsp_apicmd_pkt);

  my $status    = $req_apicmd1->GetStatus();
  my $client_id = $req_apicmd1->GetClientId();
  printf ("Status = %d, Client Id = %d\n", $status, $client_id);

  sleep(2);

######################################################################
# Build a diagnostic request for CGPS_REGISTER_PD_CMD to Deregister the client
######################################################################

$req_apicmd2 = new pd_EndClient;
$req_apicmd2->{"cmd_code"}        = 75;  # Diag Cmd Code
  $req_apicmd2->{"subsys_id"}       = 13;  # Diag Subsystem ID
  $req_apicmd2->{"subsys_cmd_code"} = 100; # Diag Subsystem Cmd Code
  $req_apicmd2->{"cgps_cmd_code"}   = 1;   # CGPS Cmd Code
  $req_apicmd2->{"version"}         = 1;   # Version Number
  $req_apicmd2->{"client_id"}       = 1001;  # Client ID from Deactivate
  $req_apicmd2->{"reg_type"}        = 1;   # Deregister

  ######################################################################
  # Construct the DM CGPS_API_CMD capture request packet
  ######################################################################

  my $req_apicmd_pkt = $req_apicmd2->SetCaptureCmd();
  printf ("Sending CGPS_API_CMD: DeRegister client ID %d\n", $client_id);

  ######################################################################
  # Send command request packet, receive command response packet
  ######################################################################

  my $rsp_apicmd_pkt = $qxdm_app->SendDmIcdPacketEx($req_apicmd_pkt, $timeout);

  my $rsp_apicmd_len = length($rsp_apicmd_pkt);
  printf ("DM CGPS_API_CMD Response Packet Length: %d\n ", $rsp_apicmd_len);

  $req_apicmd2->ParseData($rsp_apicmd_pkt);

  my $status    = $req_apicmd2->GetStatus();
  my $client_id = $req_apicmd2->GetClientId();
  printf ("Status = %d, Client Id = %d\n", $status, $client_id);

  sleep(2);

######################################################################
# Build a diagnostic request for CGPS_RELEASE_CMD to Release the client
######################################################################

$req_apicmd3 = new pd_EndClient;
$req_apicmd3->{"cmd_code"}        = 75;  # Diag Cmd Code
  $req_apicmd3->{"subsys_id"}       = 13;  # Diag Subsystem ID
  $req_apicmd3->{"subsys_cmd_code"} = 100; # Diag Subsystem Cmd Code
  $req_apicmd3->{"cgps_cmd_code"}   = 9;   # CGPS Cmd Code
  $req_apicmd3->{"version"}         = 1;   # Version Number
  $req_apicmd3->{"client_id"}       = 1001;  # Client ID from Deactivate

  ######################################################################
  # Construct the DM CGPS_API_CMD capture request packet
  ######################################################################

  my $req_apicmd_pkt = $req_apicmd3->SetCaptureCmd();
  printf ("Sending CGPS_API_CMD: Release client ID %d\n", $client_id);

  ######################################################################
  # Send command request packet, receive command response packet
  ######################################################################

  my $rsp_apicmd_pkt = $qxdm_app->SendDmIcdPacketEx($req_apicmd_pkt, $timeout);

  my $rsp_apicmd_len = length($rsp_apicmd_pkt);
  printf ("DM CGPS_API_CMD Response Packet Length: %d\n ", $rsp_apicmd_len);

  $req_apicmd3->ParseData($rsp_apicmd_pkt);

  my $status    = $req_apicmd3->GetStatus();
  my $client_id = $req_apicmd3->GetClientId();
  printf ("Status = %d, Client Id = %d\n", $status, $client_id);
  sleep(2);

######################################################################
# Build a diagnostic request for CGPS_DIAG_END_TASK to END the
# GPSDIAG task
######################################################################

  my $req_endtask_len = 4;
  my $req_endtask_pkt = Variant(VT_ARRAY | VT_UI1, $req_endtask_len);

  $req_endtask_pkt->Put(0, 75); # Diag Cmd Code
  $req_endtask_pkt->Put(1, 13); # Diag Subsystem ID
  $req_endtask_pkt->Put(2,  12); # Diag Subsystem Cmd Code
  $req_endtask_pkt->Put(3,  0);
  printf ("Sending CGPS_DIAG_END_TASK command...\n");

  sleep(2);

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

ApiEndCLientCmd

Syntax:   pd_EndClient [options]

Options:  -help               Display this help message.

Example:  perl pd_EndClient.pl <com port>


END

  print "$usage";
  exit (0);
}
