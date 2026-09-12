###########################################################################
# $Header: //components/rel/gnss8.mpss/11.1/gnss_tools/pdapi_scripts/QXDM4/pd_GetPos.pl#1 $
# $DateTime: 2019/10/24 01:30:36 $
# $Author: pwbldsvc $
#
# This example demonstrates using a DM Command to initiate a MS_BASED
# GPS position fix via the PDAPI using CGPS_START_CMD.
#
# Before calling this script, a PDAPI client will need to be
# initialized (created)first. Also, this client will then need to register
# for PD events and activate itself before it can start a position
# session. (Run pd_InitClient.pl)
#
# Values that need to be modified for pdsm_get_pos()
#--------------------------------------------------- 
# 1.operating_type
# 2.qos_gps_sess_tout 
# 3.qos_acc_thrshld
# 4.server_ipv4_adrs
# 5.server_ipv4_port
#---------------------------------------------------
#
# Usage: perl pd_GetPos.pl 11 1 (11 is the com port the FFA is connected
# to, & 1 is the operation mode)
#
##########################################################################

use vars qw(%opts);

use Win32::OLE;
use Win32::OLE::Variant;

use Getopt::Long;
use pd_Getpos;
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

######################################################################
# Check for user input and prompt if arguments are not provided
######################################################################

$comport = $ARGV[0];
$operation_mode = $ARGV[1];
$count = 0;

#check for out of range values for operation mode
if(defined ($operation_mode)) {
    check_operation_mode();
}
# Allow user input for comport
if(not defined ($comport)) {
    print("\nUsage: perl pd_GetPos.pl <COM port> <operation_mode>\n\n");
    print ("Please enter the COM port the FFA is connected to\n");
    $count = 1;
    chomp ($comport = <STDIN>);
    print("\n");
}
#Allow user input for operation mode
if (not defined ($operation_mode)) {
    if ($count == 0) {
        print("\nUsage: perl pd_GetPos.pl <COM port> <operation_mode>\n\n");
    	  $count = 1;
    }
    #Prompt user to enter operation mode
    print ("Please enter the operating mode you want to use
    1 = Standalone
    2 = MS-BASED
    3 = MS-ASSISTED
    4 = OPTIMAL SPEED
    5 = OPTIMAL ACCURACY
    6 = OPTIMAL DATA
    7 = REFERENCE LOCATION\n");
    chomp ($operation_mode = <STDIN>);
    #check for out of range values for operation mode
    check_operation_mode();
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

$qxdm_app->SendScript("logging on");

######################################################################
# Create QXDM2 instance
######################################################################

$qxdm_app2 = $qxdm_app->GetIQXDM2();
if ($qxdm_app2 == 0)
{
   print "QXDM does not support required interface\n";
   sleep(5);
   die;
}

# Register the QXDM client.
$ReqHandle = $qxdm_app2->RegisterQueueClient( 256 );
if ($ReqHandle == 0xFFFFFFFF)
{
   print "Unable to register as QXDM client\n";
   sleep(5);
   die;
}

# Get a configuration object.
my $clientObject = $qxdm_app2->ConfigureClientByKeys( $ReqHandle );
if ($clientObject == 0)
{
   print "Unable to register as QXDM client\n";
   sleep(5);
   die;
}

######################################################################
# Register for Required Events
######################################################################

$clientObject->AddEvent( 605 );       #EVENT_GPS_PD_SESS_START
$clientObject->AddEvent( 601 );       #EVENT_GPS_PD_FIX_START
$clientObject->AddEvent( 614 );       #EVENT_GPS_PD_POSITION
$clientObject->AddEvent( 602 );       #EVENT_GPS_PD_FIX_END
$clientObject->AddEvent( 411 );       #EVENT_GPS_PD_SESS_END
$clientObject->AddEvent( 1301 );      #EVENT_GPS_LM_MGP_OFF

$clientObject->CommitConfig();

######################################################################
# Global variables initialized here
######################################################################

my @client_ids;
# Set the timeout to 2 seconds.
my $timeout = 2000;

######################################################################
# Get Position Cmd parameters used for Apptracking only
######################################################################

  my $sessions  = 1; # not used (tbf only used for app tracking)
  my $tbf       = 1; # not used (num > 1 fix -> used for app tracking)
  sleep(2);

  print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  print("Apply get_pos to Start A-GPS Session\n");
  print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

######################################################################
# Build a diagnostic request for CGPS_START_CMD to do a Standalone fix
######################################################################
  $req_getpos = new pd_GetPos;
  $req_getpos->{"cmd_code"}          = 75;            # Diag Cmd Code
  $req_getpos->{"subsys_id"}         = 13;            # Diag Subsystem ID
  $req_getpos->{"subsys_cmd_code"}   = 100;           # Diag Subsystem Cmd Code
  $req_getpos->{"cgps_cmd_code"}     = 10;            # CGPS Cmd Code
  $req_getpos->{"version"}           = 1;             # Version Number
  $req_getpos->{"session_type"}      = 1;             # 0 = latest position
                                                      # 1 = new position
									# 3 = data download
  $req_getpos->{"operating_type"}    = $operation_mode;
  printf("operating Mode = %d\n", $req_getpos->{"operating_type"});
  									# 1 = Standalone
                                                      # 2 = MS-BASED
                                                      # 3 = MS-ASSISTED
                                                      # 4 = Optimal Speed
                                                      # 5 = Optimal Accuracy
                                                      # 6 = Optimal Data
                                                      # 7 = Reference Position
  $req_getpos->{"server_option"}     = 1;             # 0 = NV Default
                                                      # 1 = Custom Local
  $req_getpos->{"server_addr_type"}  = 0;             # 0 = IpV4
                                                      # 1 = IpV6
  $req_getpos->{"class_id"}          = 0;             # not used
  $req_getpos->{"qos_gps_sess_tout"} = 89;            # QoS timeout value between 0-255 seconds
                                                      # Default for 1x         = 16 sec
                                                      # Default for UMTS       = 45 sec
                                                      # Default for Standalone = 89 sec
  $req_getpos->{"qos_acc_thrshld"}   = 50;            # QoS accuracy threshold in meters
  $req_getpos->{"time_bw_fixes"}     = $tbf;          # tbf only used for app tracking
  $req_getpos->{"num_fixes"}         = $sessions;     # num > 1 fix -> used for app tracking
  $req_getpos->{"client_id"}         = 1001;    # Returned by PDAPI duriing client registration
  $req_getpos->{"reserved"}          = 0;             # not used
  $req_getpos->{"server_ipv4_adrs"}  = "10.1.2.21";  # !!Change to the local Custom Server Address!!
  $req_getpos->{"server_ipv4_port"}  = 4921;          # !!Change to the local Custom SIA port number!!

  ######################################################################
  # Construct the DM CGPS_START_CMD capture request packet
  ######################################################################

  my $req_getpos_pkt = $req_getpos->SetCaptureCmd();
  $PrevIndex = $qxdm_app2->GetClientItemCount( $ReqHandle ) - 1;   

  ######################################################################
  # Send command request packet, receive command response packet
  ######################################################################

  my $rsp_getpos_pkt = $qxdm_app->SendDmIcdPacketEx($req_getpos_pkt, $timeout);

  my $rsp_getpos_len = length($rsp_getpos_pkt);
  printf ("\nDM CGPS_START_CMD Response Packet Length = %d\n", $rsp_getpos_len);

  $req_getpos->ParseData($rsp_getpos_pkt);

  $status    = $req_getpos->GetStatus();
  $client_id = $req_getpos->GetClientId();
  printf ("Status = %d, Client Id = %d\n\n", $status, $client_id);

  ################################################################################
  # Waiting for SESSION START, FIX START, POSITION, FIX END, & SESSION END Events
  ################################################################################

  # Process Client Events
  $CanStartNextSession = 0;
  $GPS_PD_SESS_START_RECEIVED = 0;
  $GPS_PD_FIX_START_RECEIVED = 0;
  $PdSessionEndReceived = 0;
  $LmMgpOffReceived =0;
 
  $sess_start_time = time();
  $sess_start_timeout = 10;
  $sess_end_MGP_off_timeout =  $req_getpos->{"qos_gps_sess_tout"} + 35;
  while ($CanStartNextSession == 0)
  {
    # Checking for GPS_PD_SESSION_START Event timeout.
    if ($GPS_PD_SESS_START_RECEIVED == 0)
    {
        $current_time = time();
        if ($current_time - $sess_start_time > $sess_start_timeout)
        {
          printf ("Timeout: Event GPS_PD_SESSION_START not received\n\n");
          exit;
        }
    }
    # Checking for GPS_LM_MGP_OFF Event Timeout, a event after GPS_PD_SESSION_END Event
    $current_time = time();
    if ($current_time - $sess_start_time > $sess_end_MGP_off_timeout)
    {
	printf ("Timeout: Event GPS_PD_SESSION_END_MGP_OFF not received\n\n");
	exit;
    }
    $CurrIndex = $qxdm_app2->GetClientItemCount( $ReqHandle ) - 1; # Get index of last item in client
    
    # Process all new items
    for ( my $i2 = $PrevIndex + 1; $i2 <= $CurrIndex; $i2++ )
    {
	my $Item = $qxdm_app2->GetClientItem($ReqHandle, $i2 ); # Get Item
      my $ItemKey = $Item->GetItemKeyText();
	if ($GPS_PD_SESS_START_RECEIVED == 0)
	{  
	  $current_time = time();
	  if ($current_time - $sess_start_time > $sess_start_timeout)
	  {
	    printf ("Time out: Event GPS_PD_SESS_START not received.!!\n");
	    exit;
 	  }
	  else
	  {
	    if ($ItemKey eq "[00605]")
	    {
		printf ("Received event GPS_PD_SESS_START!!\n\n");
		$GPS_PD_SESS_START_RECEIVED = 1;
	    }
	  }
	}
	if ($GPS_PD_SESS_START_RECEIVED == 1)
	{
	  if ($ItemKey eq "[00601]")
	  {
	    printf ("Received Event GPS_PD_FIX_START!!\n\n");
   	    $GPS_PD_FIX_START_RECEIVED = 1;
   	  }
	}    
	if ($GPS_PD_SESS_START_RECEIVED == 1 || GPS_PD_FIX_START_RECEIVED == 1)
	{ 
	  $current_time = time();
	  if ($current_time - $sess_start_time > $sess_end_MGP_off_timeout)
	  {
	    printf ("Timeout: Event GPS_PD_SESSION_END_MGP_off not received\n\n");
	    exit;
	  }
	  else
	  {
          if ($ItemKey eq "[00614]")
          {
            printf ("Received Event GPS_PD_POSITION!!\n\n");
	    }
	    if ($ItemKey eq "[00602]")
          {
            printf ("Received Event GPS_PD_FIX_END!!\n\n");
	    }
	    if ($ItemKey eq "[00411]")
	    {
	      printf ("Received Event GPS_PD_SESSION_END!!\n\n");
            $PdSessionEndReceived = 1;
	    }
	    if ($ItemKey eq "[01301]")
	    {
	      printf ("Received Event GPS_LM_MGP_OFF!!\n\n");
	      $LmMgpOffReceived = 1;
	    }
	  }
	}
	else
	{
	  if ($ItemKey eq "[00411]")
	  {
	    printf ("Events received out of order\n");
	    exit;
	  }
	}
	  
    }
    $PrevIndex = $CurrIndex; # Keep processing all new items while Session is not done.
    if ($PdSessionEndReceived == 1 && $LmMgpOffReceived == 1)
    {
        $CanStartNextSession = 1;
    }
  }

  sub check_operation_mode {
    while($operation_mode == 0 || $operation_mode > 7) {
      print ("operation mode out of range. Please enter a value between 1-7
    1 = Standalone
    2 = MS-BASED
    3 = MS-ASSISTED
    4 = OPTIMAL SPEED
    5 = OPTIMAL ACCURACY
    6 = OPTIMAL DATA
    7 = REFERENCE LOCATION\n");
      chomp ($operation_mode = <STDIN>);
     }
     return;
  }

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

GetPositionCmd

Syntax:   pd_GetPos [options]

Options:  -help               Display this help message.

Example:  perl pd_GetPos.pl <COM Port> <operation mode>

END

  print "$usage";
  exit (0);
}
