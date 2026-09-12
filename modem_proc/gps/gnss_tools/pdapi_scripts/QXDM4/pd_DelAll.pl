##########################################################################
# $Header: //components/rel/gnss8.mpss/11.1/gnss_tools/pdapi_scripts/QXDM4/pd_DelAll.pl#1 $
# $DateTime: 2019/10/24 01:30:36 $
# $Author: pwbldsvc $
#
# This example demonstrates using a DM Command to delete various CGPS
# related parameters before making a fix by sending a
# CGPS_SET_PARAM_CMD. This script deletes almanac,ephemeris, position,
# time, iono, UTC, RTI, Health, SV Direction, SV Steering, and Sensitivity
# Assiatance in order to do a cold start. This command does not start a fix;
# rather it just sets the parameter that need to be deleted.
#
# Before calling this script, a PDAPI client will need to first be
# initialized (created). Also, this client will then need to register
# for PD events and activate itself before it can set delete all
# related parameters. (Run pd_InitClient.pl)
#
# Usage: perl pd_DelAll.pl 11 (11 is the com port the FFA is connected to)
#
##########################################################################

use vars qw(%opts);

use Win32::OLE;
use Win32::OLE::Variant;

use Getopt::Long;
use pd_DelAll;
#use pd_DelAll_kg_callisto;
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
    print("\nUsage: perl pd_DelAll.pl <COM port>\n\n");
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
# Build a diagnostic request for CGPS_SET_PARAM_CMD: DELETE_PARAMS
######################################################################

  $req_coldstart = new pd_DelAll;
  $req_coldstart->{"cmd_code"}        = 75;  # Diag Cmd Code
  $req_coldstart->{"subsys_id"}       = 13;  # Diag Subsystem ID
  $req_coldstart->{"subsys_cmd_code"} = 100; # Diag Subsystem Cmd Code
  $req_coldstart->{"cgps_cmd_code"}   = 12;  # CGPS Cmd Code
  $req_coldstart->{"version"}         = 1;   # Version Number
  $req_coldstart->{"client_id"}       = 1001;  # TEST2
  $req_coldstart->{"param_type"}      = 4;  # PA_DELETE_PARAMS
  $req_coldstart->{"param_length"}    = 72; # 16 x (4 byte fields)#64 
  $req_coldstart->{"delete_bitmap"}   = 0xFFFFFF; # Delete All (0001-1000|0111|1111|1111 = 0x187FF)
  
                                              # GPSDIAG_PA_DELETE_EPH     = 0x0001,
											  # GPSDIAG_PA_DELETE_ALM     = 0x0002,
											  # GPSDIAG_PA_DELETE_POS     = 0x0004,
											  # GPSDIAG_PA_DELETE_TIME    = 0x0008,
											  # GPSDIAG_PA_DELETE_IONO    = 0x0010,
											  # GPSDIAG_PA_DELETE_UTC     = 0x0020,
											  # GPSDIAG_PA_DELETE_HEALTH  = 0x0040,
											  # GPSDIAG_PA_DELETE_SVDIR   = 0x0080,
											  # GPSDIAG_PA_DELETE_SVSTEER = 0x0100,
											  # GPSDIAG_PA_DELETE_SADATA  = 0x0200,
											  # GPSDIAG_PA_DELETE_RTI     = 0x0400,
											  # GPSDIAG_PA_TIME_UNC       = 0x0800,
											  # GPSDIAG_PA_POS_UNC        = 0x1000,
											  # GPSDIAG_PA_TIME_OFFSET    = 0x2000,
											  # GPSDIAG_PA_POS_OFFSET     = 0x4000,
											  # GPSDIAG_PA_DELETE_CELLDB_INFO = 0x8000,
											  # GPSDIAG_PA_DELETE_ALM_CORR = 0x10000,
											  # GPSDIAG_PA_FREQ_BIAS_EST  =  0x20000,
											  # GPSDIAG_PA_DELETE_SV_NO_EXIST = 0x40000,
											  # GPSDIAG_PA_DELETE_MB_DATA    = 0x80000,
											  # GPSDIAG_PA_DELETE_TGD_DATA_GPS = 0x100000,
											  # GPSDIAG_PA_DELETE_TIME_GPS = 0x200000
											  # GPSDIAG_PA_DELETE_XTRA_INTEGRITY_GPS = 0x400000,
											  # GPSDIAG_PA_DELETE_GNSS_SV_BLACKLIST_GPS  = 0x800000


  $req_coldstart->{"gnss_delete_bitmap"} = 0xFFFFFFFFFFFF;#7FFFFFFF
                                               # GNSSDIAG_PA_DELETE_EPH_GLO       = GPSDIAG_SET_U64BITMASK_BIT(0),
											   # GNSSDIAG_PA_DELETE_EPH_SBAS      = GPSDIAG_SET_U64BITMASK_BIT(1),
											   # GNSSDIAG_PA_DELETE_ALM_GLO       = GPSDIAG_SET_U64BITMASK_BIT(2),
											   # GNSSDIAG_PA_DELETE_ALM_SBAS      = GPSDIAG_SET_U64BITMASK_BIT(3),
											   # GNSSDIAG_PA_DELETE_SVDIR_GLO     = GPSDIAG_SET_U64BITMASK_BIT(4),
											   # GNSSDIAG_PA_DELETE_SVDIR_SBAS    = GPSDIAG_SET_U64BITMASK_BIT(5),
											   # GNSSDIAG_PA_DELETE_SVSTEER_GLO   = GPSDIAG_SET_U64BITMASK_BIT(6),
											   # GNSSDIAG_PA_DELETE_SVSTEER_SBAS  = GPSDIAG_SET_U64BITMASK_BIT(7),
											   # GNSSDIAG_PA_DELETE_ALM_CORR_GLO  = GPSDIAG_SET_U64BITMASK_BIT(8),
											   # GNSSDIAG_PA_DELETE_CLOCK_INFO    = GPSDIAG_SET_U64BITMASK_BIT(9),
											   # GNSSDIAG_PA_DELETE_EPH_BDS       = GPSDIAG_SET_U64BITMASK_BIT(10),
											   # GNSSDIAG_PA_DELETE_ALM_BDS       = GPSDIAG_SET_U64BITMASK_BIT(11),
											   # GNSSDIAG_PA_DELETE_SVDIR_BDS     = GPSDIAG_SET_U64BITMASK_BIT(12),
											   # GNSSDIAG_PA_DELETE_SVSTEER_BDS   = GPSDIAG_SET_U64BITMASK_BIT(13),
											   # GNSSDIAG_PA_DELETE_ALM_CORR_BDS  = GPSDIAG_SET_U64BITMASK_BIT(14),
											   # GNSSDIAG_PA_DELETE_GNSS_SV_BLACKLIST_GPS  = GPSDIAG_SET_U64BITMASK_BIT(15),
											   # GNSSDIAG_PA_DELETE_GNSS_SV_BLACKLIST_GLO  = GPSDIAG_SET_U64BITMASK_BIT(16),
											   # GNSSDIAG_PA_DELETE_GNSS_SV_BLACKLIST_BDS  = GPSDIAG_SET_U64BITMASK_BIT(17),
											   # GNSSDIAG_PA_DELETE_GNSS_SV_BLACKLIST_GAL  = GPSDIAG_SET_U64BITMASK_BIT(18),
											   # GNSSDIAG_PA_DELETE_EPH_GAL       = GPSDIAG_SET_U64BITMASK_BIT(19),
											   # GNSSDIAG_PA_DELETE_ALM_GAL       = GPSDIAG_SET_U64BITMASK_BIT(20),
											   # GNSSDIAG_PA_DELETE_SVDIR_GAL     = GPSDIAG_SET_U64BITMASK_BIT(21),
											   # GNSSDIAG_PA_DELETE_SVSTEER_GAL   = GPSDIAG_SET_U64BITMASK_BIT(22),
											   # GNSSDIAG_PA_DELETE_ALM_CORR_GAL  = GPSDIAG_SET_U64BITMASK_BIT(23),
											   # GNSSDIAG_PA_DELETE_GNSS_SV_BLACKLIST_QZSS  = GPSDIAG_SET_U64BITMASK_BIT(24),
											   # GNSSDIAG_PA_DELETE_EPH_QZSS       = GPSDIAG_SET_U64BITMASK_BIT(25),
											   # GNSSDIAG_PA_DELETE_ALM_QZSS       = GPSDIAG_SET_U64BITMASK_BIT(26),
											   # GNSSDIAG_PA_DELETE_SVDIR_QZSS     = GPSDIAG_SET_U64BITMASK_BIT(27),
											   # GNSSDIAG_PA_DELETE_SVSTEER_QZSS   = GPSDIAG_SET_U64BITMASK_BIT(28),
											   # GNSSDIAG_PA_DELETE_ALM_CORR_QZSS  = GPSDIAG_SET_U64BITMASK_BIT(29),
											   # GNSSDIAG_PA_DELETE_IONO_QZSS      = GPSDIAG_SET_U64BITMASK_BIT(30),
											   # GNSSDIAG_PA_DELETE_IONO_BDS       = GPSDIAG_SET_U64BITMASK_BIT(31),
											   # GNSSDIAG_PA_DELETE_IONO_GAL       = GPSDIAG_SET_U64BITMASK_BIT(32),
											   # GNSSDIAG_PA_DELETE_HEALTH_GLO     = GPSDIAG_SET_U64BITMASK_BIT(33),
											   # GNSSDIAG_PA_DELETE_HEALTH_BDS     = GPSDIAG_SET_U64BITMASK_BIT(34),
											   # GNSSDIAG_PA_DELETE_HEALTH_GAL     = GPSDIAG_SET_U64BITMASK_BIT(35),
											   # GNSSDIAG_PA_DELETE_TGD_DATA_GLO   = GPSDIAG_SET_U64BITMASK_BIT(36),
											   # GNSSDIAG_PA_DELETE_TGD_DATA_GAL   = GPSDIAG_SET_U64BITMASK_BIT(37),
											   # GNSSDIAG_PA_DELETE_TGD_DATA_BDS   = GPSDIAG_SET_U64BITMASK_BIT(38),
											   # GNSSDIAG_PA_DELETE_TGD_DATA_QZSS  = GPSDIAG_SET_U64BITMASK_BIT(39),
											   # GNSSDIAG_PA_DELETE_TIME_GLO       = GPSDIAG_SET_U64BITMASK_BIT(40),
											   # GNSSDIAG_PA_DELETE_TIME_GAL       = GPSDIAG_SET_U64BITMASK_BIT(41),
											   # GNSSDIAG_PA_DELETE_TIME_BDS       = GPSDIAG_SET_U64BITMASK_BIT(42),
											   # GNSSDIAG_PA_DELETE_XTRA_INTEGRITY_GLO  = GPSDIAG_SET_U64BITMASK_BIT(43),
											   # GNSSDIAG_PA_DELETE_XTRA_INTEGRITY_GAL  = GPSDIAG_SET_U64BITMASK_BIT(44),
											   # GNSSDIAG_PA_DELETE_XTRA_INTEGRITY_BDS  = GPSDIAG_SET_U64BITMASK_BIT(45),
											   # GNSSDIAG_PA_DELETE_XTRA_INTEGRITY_QZSS = GPSDIAG_SET_U64BITMASK_BIT(46)



  $req_coldstart->{"gnss_reserved"}   = 0;    # not used at present
  $req_coldstart->{"time_unc"}        = 0;  # Inject Time Uncertainty
                                            # Only applicable if INJECT_TIME_UNC bit = 1
  $req_coldstart->{"pos_unc"}         = 0;  # Inject Position Uncertainty
                                            # Only applicable if INJECT_POS_UNC bit = 1
  $req_coldstart->{"time_offset"}     = 0;  # Add a Time Offset
                                            # Only applicable if INJECT_TIME_OFFSET bit = 1
  $req_coldstart->{"pos_offset"}      = 0;  # Add a Position Offset
                                            # Only applicable if INJECT_POS_OFFSET bit = 1
  $req_coldstart->{"eph_sv_mask"}     = 0xFFFFFFFF;  # Ephemeris SV Mask
  $req_coldstart->{"alm_sv_mask"}     = 0xFFFFFFFF;  # Almanac SV Mask

  $req_coldstart->{"glo_eph_sv_mask"}     = 0xFFFFFFFF;  # not used
  $req_coldstart->{"glo_alm_sv_mask"}     = 0xFFFFFFFF;  # not used
  $req_coldstart->{"sbas_eph_sv_mask"}     = 0;  # delete SBAS alm
  $req_coldstart->{"sbas_alm_sv_mask"}     = 0;  # delete SBAS eph
  $req_coldstart->{"delete_celldb_mask"} = 0xFFFFFFFF;  # Delete Cell Database Mask
                                                        # Only applicable if DELETE_CELLDB_INFO bit = 1
  $req_coldstart->{"delete_clk_info"}     = 0xFFFFFF;  # delete clock info
  $req_coldstart->{"reserved"}     = 0;  # not used

  ######################################################################
  # Construct the DM CGPS_SET_PARAM_CMD capture request packet
  ######################################################################

  my $req_coldstart_pkt = $req_coldstart->SetCaptureCmd();

  ######################################################################
  # Send command request packet, receive command response packet
  ######################################################################
  my $a= hex($req_coldstart_pkt);
print "\n $a";
  my $rsp_coldstart_pkt = $qxdm_app->SendDmIcdPacketEx($req_coldstart_pkt, $timeout);

  my $rsp_coldstart_len = length($rsp_coldstart_pkt);
  printf ("DM CGPS_SET_PARAM_CMD Response Packet Length: %d\n", $rsp_coldstart_len);

  $req_coldstart->ParseData($rsp_coldstart_pkt);

  $status    = $req_coldstart->GetStatus();
  $client_id = $req_coldstart->GetClientId();
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

SetColdStartParametersCmd

Syntax:   pd_DelAll [options]

Options:  -help               Display this help message.

Example:  perl pd_DelAll.pl <com port>

END

  print "$usage";
  exit (0);
}
