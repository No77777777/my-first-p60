#!/usr/bin/perl
#/*============================================================================
#  FILE:         adc_diag.pl
#
#  OVERVIEW:     Reads an ADC channel over diag.
#
#  DEPENDENCIES: None
#
#                Copyright (c) 2014-2015 Qualcomm Technologies, Inc.
#                All Rights Reserved.
#                Qualcomm Technologies Proprietary and Confidential.
#============================================================================*/
#/*============================================================================
#  EDIT HISTORY FOR MODULE
#
#  This section contains comments describing changes made to the module.
#  Notice that changes are listed in reverse chronological order.  Please
#  use ISO format for dates.
#
#  $Header: //components/rel/core.mpss/10.0/hwengines/adc/scripts/adc_diag.pl#1 $$DateTime: 2019/04/24 00:03:26 $$Author: pwbldsvc $
#
#  when        who  what, where, why
#  ----------  ---  -----------------------------------------------------------
#  2015-07-08  jjo  Fixed print issue.
#  2015-03-04  jjo  Fixed percent and microvolt values.
#  2014-10-29  jjo  Initial revision.
#
#============================================================================*/
use Win32::OLE;
use Win32::OLE::Variant;

$num_args = $#ARGV + 1;
if ($num_args != 2)
{
   print("Invalid command line argument. Usage: perl adc_diag.pl COMx CHANNEL_NAME\n");
   print("Example: perl adc_diag.pl COM24 VPH_PWR\n");
   exit;
}

$port = $ARGV[0];
$channel = $ARGV[1];

$timeout_in_ms = 2000;
$debug = 0;

$prod_id = "QPSTAtmnServer.Application";
eval
{
   $qpst = Win32::OLE->GetActiveObject($prod_id)
};
die "$prod_id not installed" if $@;
unless (defined $qpst)
{
   $qpst = Win32::OLE->new($prod_id, sub {$_[0]->Quit;}) or die "Cannot start $prod_id";
}
if (defined $qpst)
{
   # Select the port
   $port = $qpst->GetPort($port);
   if (!defined $port)
   {
      print("Invalid port\n");
      exit;
   }

   # Get the device / channel index
   $channel = $channel."\0";

   @header = (75, 75, 31);
   @payload = (length($channel), 0);

   $diag_cmd = pack("CCv", @header);
   $diag_cmd = $diag_cmd.pack("vC", @payload);
   $diag_cmd = $diag_cmd.$channel;

   $diag_cmd_var = Variant(VT_ARRAY | VT_UI1, length($diag_cmd));
   $diag_cmd_var->Put($diag_cmd);

   @cmd = unpack("C*", $diag_cmd_var->Value());

   if ($debug)
   {
      print("cmd:   @cmd\n");
   }

   $reply = $port->SendCommand($diag_cmd_var, $timeout_in_ms);
   if (defined $reply)
   {
      @reply_values = unpack("CCvvvVV", $reply);

      if ($debug)
      {
         print("reply: @reply_values\n");
      }

      $device_idx = $reply_values[5];
      $channel_idx = $reply_values[6];

      print("Device Index: $device_idx, Channel Index: $channel_idx\n");
   }

   # Select the channel
   @header = (75, 75, 35);
   @payload = (1, 0, $device_idx, $channel_idx);

   $diag_cmd = pack("CCv", @header);
   $diag_cmd = $diag_cmd.pack("vvVV", @payload);

   $diag_cmd_var = Variant(VT_ARRAY | VT_UI1, length($diag_cmd));
   $diag_cmd_var->Put($diag_cmd);

   @cmd = unpack("C*", $diag_cmd_var->Value());
   if ($debug)
   {
      print("cmd:   @cmd\n");
   }

   $reply = $port->SendCommand($diag_cmd_var, $timeout_in_ms);
   if (defined $reply)
   {
      @reply_values = unpack("CCvVvvVVvv", $reply);

      if ($debug)
      {
         print("reply: @reply_values\n");
      }
   }

   # Get the reading
   @header = (75, 75, 36);
   @payload = (1, 0, $device_idx, $channel_idx);

   $diag_cmd = pack("CCv", @header);
   $diag_cmd = $diag_cmd.pack("vvVV", @payload);

   $diag_cmd_var = Variant(VT_ARRAY | VT_UI1, length($diag_cmd));
   $diag_cmd_var->Put($diag_cmd);

   @cmd = unpack("C*", $diag_cmd_var->Value());

   if ($debug)
   {
      print("cmd:   @cmd\n");
   }

   $reply = $port->SendCommand($diag_cmd_var, $timeout_in_ms);
   if (defined $reply)
   {
      @reply_values = unpack("CCvvvVVVvvlVVVV", $reply);

      if ($debug)
      {
         print("reply: @reply_values\n");
      }

      $physical = $reply_values[10];
      $percent = $reply_values[11];
      $micro_volts = $reply_values[12];
      $code = $reply_values[13];

      printf("Physical: %d, MicroVolts: %u, Percent: %u, Code: 0x%x\n", $physical, $micro_volts, $percent, $code);
   }

   undef $qpst;
}
