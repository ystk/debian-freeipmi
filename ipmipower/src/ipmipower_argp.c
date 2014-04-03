/*****************************************************************************\
 *  $Id: ipmipower_argp.c,v 1.2.10.1 2008/11/08 00:09:33 chu11 Exp $
 *****************************************************************************
 *  Copyright (C) 2007-2008 Lawrence Livermore National Security, LLC.
 *  Copyright (C) 2003-2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Albert Chu <chu11@llnl.gov>
 *  UCRL-CODE-155698
 *  
 *  This file is part of Ipmipower, a remote power control utility.
 *  For details, see http://www.llnl.gov/linux/.
 *  
 *  Ipmipower is free software; you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by the 
 *  Free Software Foundation; either version 2 of the License, or (at your 
 *  option) any later version.
 *  
 *  Ipmipower is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
 *  for more details.
 *  
 *  You should have received a copy of the GNU General Public License along
 *  with Ipmipower.  If not, see <http://www.gnu.org/licenses/>.
\*****************************************************************************/

#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#if HAVE_ARGP_H
#include <argp.h>
#else /* !HAVE_ARGP_H */
#include "freeipmi-argp.h"
#endif /* !HAVE_ARGP_H */
#if HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#include <assert.h>
#include <errno.h>

#include "ipmipower_argp.h"
#include "ipmipower_output.h"
#include "ipmipower_util.h"
#include "ipmipower_wrappers.h"

#include "freeipmi-portability.h"
#include "pstdout.h"
#include "secure.h"
#include "tool-common.h"
#include "tool-cmdline-common.h"
#include "tool-config-file-common.h"
      
#define IPMIPOWER_CONFIG_FILE_DEFAULT "/etc/ipmipower.conf"

extern struct ipmipower_connection *ics;

const char *argp_program_version = 
  "ipmipower - " PACKAGE_VERSION "\n"
  "Copyright (C) 2007-2008 Lawrence Livermore National Security, LLC.\n"
  "Copyright (C) 2003-2007 The Regents of the University of California.\n"
  "This program is free software; you may redistribute it under the terms of\n"
  "the GNU General Public License.  This program has absolutely no warranty.";

const char *argp_program_bug_address = 
  "<" PACKAGE_BUGREPORT ">";

static char cmdline_doc[] = 
  "ipmipower - IPMI power control utility";

static char cmdline_args_doc[] = "";

static struct argp_option cmdline_options[] =
  {
    ARGP_COMMON_OPTIONS_DRIVER,
    /* maintain "ipmi-version" for backwards compatability */
    {"ipmi-version", IPMI_VERSION_KEY, "IPMIVERSION", OPTION_HIDDEN,
     "Specify the IPMI protocol version to use.", 11},
    ARGP_COMMON_OPTIONS_OUTOFBAND_HOSTRANGED,
    /* maintain legacy short options to timeout/session-timeout */    
    {"bogus-long-option1", SESSION_TIMEOUT_KEY, "MILLISECONDS", OPTION_HIDDEN,
     "Specify the session timeout in milliseconds.", 12},
    /* maintain legacy short options to retry-timeout/retransmission-timeout */
    {"bogus-long-option2", RETRANSMISSION_TIMEOUT_KEY, "MILLISECONDS", OPTION_HIDDEN,
     "Specify the packet retransmission timeout in milliseconds.", 11},
    ARGP_COMMON_OPTIONS_AUTHENTICATION_TYPE,
    ARGP_COMMON_OPTIONS_CIPHER_SUITE_ID,
    ARGP_COMMON_OPTIONS_PRIVILEGE_LEVEL_OPERATOR,
    ARGP_COMMON_OPTIONS_CONFIG_FILE,
    ARGP_COMMON_OPTIONS_WORKAROUND_FLAGS,
    ARGP_COMMON_HOSTRANGED_OPTIONS,
    ARGP_COMMON_OPTIONS_DEBUG,
#ifndef NDEBUG
    {"rmcpdump", RMCPDUMP_KEY, 0, 0,
     "Turn on RMCP packet dump output.", 27},
#endif
    {"on", ON_KEY, 0, 0,
     "Power on the target hosts.", 30},
    {"off", OFF_KEY, 0, 0,
     "Power off the target hosts.", 31},
    {"cycle", CYCLE_KEY, 0, 0,
     "Power cycle the target hosts.", 32},
    {"reset", RESET_KEY, 0, 0,
     "Reset the target hosts.", 33},
    {"stat", STAT_KEY, 0, 0,
     "Get power status of the target hosts.", 34},
    {"pulse", PULSE_KEY, 0, 0,
     "Send power diagnostic interrupt to target hosts.", 35},
    {"soft", SOFT_KEY, 0, 0,
     "Initiate a soft-shutdown of the OS via ACPI.", 36},
    {"on-if-off", ON_IF_OFF_KEY, 0, 0,
     "Issue a power on command instead of a power cycle or hard reset "
     "command if the remote machine's power is currently off.", 38},
    {"wait-until-off", WAIT_UNTIL_OFF_KEY, 0, 0,
     "Regularly query the remote BMC and return only after the machine has powered off.", 39},
    {"wait-until-on", WAIT_UNTIL_ON_KEY, 0, 0,
     "Regularly query the remote BMC and return only after the machine has powered on.", 40},
    /* retry-wait-timeout maintained for backwards comptability */
    {"retry-wait-timeout", RETRY_WAIT_TIMEOUT_KEY, "MILLISECONDS", OPTION_HIDDEN,
     "Specify the retransmission timeout length in milliseconds.", 41},
    {"retransmission-wait-timeout", RETRANSMISSION_WAIT_TIMEOUT_KEY, "MILLISECONDS", 0,
     "Specify the retransmission timeout length in milliseconds.", 41},
    /* retry-backoff-count maintained for backwards comptability */
    {"retry-backoff-count", RETRY_BACKOFF_COUNT_KEY, "COUNT", OPTION_HIDDEN,
     "Specify the retransmission backoff count for retransmissions.", 42},
    {"retransmission-backoff-count", RETRANSMISSION_BACKOFF_COUNT_KEY, "COUNT", 0,
     "Specify the retransmission backoff count for retransmissions.", 42},
    {"ping-interval", PING_INTERVAL_KEY, "MILLISECONDS", 0,
     "Specify the ping interval length in milliseconds.", 43},
    {"ping-timeout", PING_TIMEOUT_KEY, "MILLISECONDS", 0,
     "Specify the ping timeout length in milliseconds.", 44},
    {"ping-packet-count", PING_PACKET_COUNT_KEY, "COUNT", 0,
     "Specify the ping packet count size.", 45},
    {"ping-percent", PING_PERCENT_KEY, "PERCENT", 0,
     "Specify the ping percent value.", 46},
    {"ping-consec-count", PING_CONSEC_COUNT_KEY, "COUNT", 0,
     "Specify the ping consecutive count.", 47},
    { 0 }
  };

static error_t cmdline_parse (int key, char *arg, struct argp_state *state);

static struct argp cmdline_argp = {cmdline_options,
                                   cmdline_parse,
                                   cmdline_args_doc,
                                   cmdline_doc};

static struct argp cmdline_config_file_argp = {cmdline_options,
                                               cmdline_config_file_parse,
                                               cmdline_args_doc,
                                               cmdline_doc};

static error_t
cmdline_parse (int key,
               char *arg,
               struct argp_state *state)
{
  struct ipmipower_arguments *cmd_args = state->input;
  char *ptr;
  error_t ret;
  int tmp = 0;

  switch (key) 
    {
    /* IPMI_VERSION_KEY for backwards compatability */
    case IPMI_VERSION_KEY:	/* --ipmi-version */
      if (!strcasecmp(arg, "1.5"))
        tmp = IPMI_DEVICE_LAN;
      else if (!strcasecmp(arg, "2.0"))
        tmp = IPMI_DEVICE_LAN_2_0;
      else
        ierr_exit("Command Line Error: invalid driver type specified");
      cmd_args->common.driver_type = tmp;
      break;
#ifndef NDEBUG
    case RMCPDUMP_KEY:       /* --rmcpdump */
      cmd_args->rmcpdump++;
      break;
#endif /* !NDEBUG */
    case ON_KEY:       /* --on */ 
      cmd_args->powercmd = POWER_CMD_POWER_ON;
      break;
    case OFF_KEY:       /* --off */ 
      cmd_args->powercmd = POWER_CMD_POWER_OFF;
      break;
    case CYCLE_KEY:       /* --cycle */ 
      cmd_args->powercmd = POWER_CMD_POWER_CYCLE;
      break;
    case RESET_KEY:       /* --reset */ 
      cmd_args->powercmd = POWER_CMD_POWER_RESET;
      break;
    case STAT_KEY:       /* --stat */ 
      cmd_args->powercmd = POWER_CMD_POWER_STATUS;
      break;
    case PULSE_KEY:       /* --pulse */
      cmd_args->powercmd = POWER_CMD_PULSE_DIAG_INTR;
      break;
    case SOFT_KEY:       /* --soft */
      cmd_args->powercmd = POWER_CMD_SOFT_SHUTDOWN_OS;
      break;
    case ON_IF_OFF_KEY:       /* --on-if-off */
      cmd_args->on_if_off++;
      break;
    case WAIT_UNTIL_OFF_KEY:       /* --wait-until-on */
      cmd_args->wait_until_on++;
      break;
    case WAIT_UNTIL_ON_KEY:       /* --wait-until-off */
      cmd_args->wait_until_off++;
      break;
      /* RETRY_WAIT_TIMEOUT for backwards compatability */
    case RETRY_WAIT_TIMEOUT_KEY:
    case RETRANSMISSION_WAIT_TIMEOUT_KEY:       /* --retransmission-wait-timeout */
      tmp = strtol(arg, &ptr, 10);
      if (ptr != (arg + strlen(arg))
          || tmp <= 0)
        ierr_exit("Command Line Error: retransmission wait timeout length invalid");
      cmd_args->retransmission_wait_timeout = tmp;
      break;
      /* RETRY_BACKOFF_COUNT for backwards compatability */
    case RETRY_BACKOFF_COUNT_KEY:
    case RETRANSMISSION_BACKOFF_COUNT_KEY:       /* --retransmission-backoff-count */
      tmp = strtol(arg, &ptr, 10);
      if (ptr != (arg + strlen(arg))
          || tmp <= 0)
        ierr_exit("Command Line Error: retransmission backoff count invalid");
      cmd_args->retransmission_backoff_count = tmp;
      break;
    case PING_INTERVAL_KEY:       /* --ping-interval */
      tmp = strtol(arg, &ptr, 10);
      if (ptr != (arg + strlen(arg))
          || tmp < 0)
        ierr_exit("Command Line Error: ping interval length invalid");
      cmd_args->ping_interval = tmp;
      break;
    case PING_TIMEOUT_KEY:       /* --ping-timeout */
      tmp = strtol(arg, &ptr, 10);
      if (ptr != (arg + strlen(arg))
          || tmp < 0)
        ierr_exit("Command Line Error: ping timeout length invalid");
      cmd_args->ping_timeout = tmp;
      break;
    case PING_PACKET_COUNT_KEY:       /* --ping-packet-count */
      tmp = strtol(arg, &ptr, 10);
      if (ptr != (arg + strlen(arg))
          || tmp < 0)
        ierr_exit("Command Line Error: ping packet count invalid");
      cmd_args->ping_packet_count = tmp;
      break;
    case PING_PERCENT_KEY:       /* --ping-percent */
      tmp = strtol(arg, &ptr, 10);
      if (ptr != (arg + strlen(arg))
          || tmp < 0)
        ierr_exit("Command Line Error: ping percent invalid");
      cmd_args->ping_percent = tmp;
      break;
    case PING_CONSEC_COUNT_KEY:       /* --ping-consec-count */
      tmp = strtol(arg, &ptr, 10);
      if (ptr != (arg + strlen(arg))
          || tmp < 0)
        ierr_exit("Command Line Error: ping consec count invalid");
      cmd_args->ping_consec_count = tmp;
      break;
      /* backwards compatability */
    case SESSION_TIMEOUT_KEY:
      ret = common_parse_opt (ARGP_SESSION_TIMEOUT_KEY, arg, state, &(cmd_args->common));
      return ret;
      break;
      /* backwards compatability */
    case RETRANSMISSION_TIMEOUT_KEY:
      ret = common_parse_opt (ARGP_RETRANSMISSION_TIMEOUT_KEY, arg, state, &(cmd_args->common));
      return ret;
      break;
    default:
      ret = common_parse_opt (key, arg, state, &(cmd_args->common));
      if (ret == ARGP_ERR_UNKNOWN)
        ret = hostrange_parse_opt (key, arg, state, &(cmd_args->hostrange));
      return ret;
    } 

  return 0;
}

void 
_ipmipower_config_file_parse(struct ipmipower_arguments *cmd_args)
{
  struct config_file_data_ipmipower config_file_data;

  memset(&config_file_data,
         '\0',
         sizeof(struct config_file_data_ipmipower));

  /* try legacy file first */
  if (!cmd_args->common.config_file)
    {
      if (!config_file_parse (IPMIPOWER_CONFIG_FILE_DEFAULT,
                              1,         /* do not exit if file not found */
                              &(cmd_args->common),
                              NULL,
                              &(cmd_args->hostrange),
                              CONFIG_FILE_OUTOFBAND | CONFIG_FILE_HOSTRANGE,
                              CONFIG_FILE_TOOL_IPMIPOWER,
                              &config_file_data))
        goto out;
    }
  
  if (config_file_parse (cmd_args->common.config_file,
                         0,
                         &(cmd_args->common),
                         NULL,
                         &(cmd_args->hostrange),
                         CONFIG_FILE_OUTOFBAND | CONFIG_FILE_HOSTRANGE,
                         CONFIG_FILE_TOOL_IPMIPOWER,
                         &config_file_data) < 0)
    {
      fprintf(stderr, "config_file_parse: %s\n", strerror(errno));
      exit(1);
    }

 out:
  if (config_file_data.on_if_off_count)
    cmd_args->on_if_off = config_file_data.on_if_off;
  if (config_file_data.wait_until_on_count)
    cmd_args->wait_until_on = config_file_data.wait_until_on;
  if (config_file_data.wait_until_off_count)
    cmd_args->wait_until_off = config_file_data.wait_until_off;
  if (config_file_data.retransmission_wait_timeout_count)
    cmd_args->retransmission_wait_timeout = config_file_data.retransmission_wait_timeout;
  if (config_file_data.retransmission_backoff_count_count)
    cmd_args->retransmission_backoff_count = config_file_data.retransmission_backoff_count;
  if (config_file_data.ping_interval_count)
    cmd_args->ping_interval = config_file_data.ping_interval;
  if (config_file_data.ping_timeout_count)
    cmd_args->ping_timeout = config_file_data.ping_timeout;
  if (config_file_data.ping_packet_count_count)
    cmd_args->ping_packet_count = config_file_data.ping_packet_count;
  if (config_file_data.ping_percent_count)
    cmd_args->ping_percent = config_file_data.ping_percent;
  if (config_file_data.ping_consec_count_count)
    cmd_args->ping_consec_count = config_file_data.ping_consec_count;
}

void 
_ipmipower_args_validate (struct ipmipower_arguments *cmd_args)
{
  if (cmd_args->common.driver_type == IPMI_DEVICE_LAN
      && cmd_args->common.password
      && strlen(cmd_args->common.password) > IPMI_1_5_MAX_PASSWORD_LENGTH)
    ierr_exit("Error: password too long");

  if (cmd_args->common.retransmission_timeout > cmd_args->common.session_timeout)
    ierr_exit("Error: Session timeout length must be longer than retransmission timeout length");

  if (cmd_args->retransmission_wait_timeout > cmd_args->common.session_timeout)
    ierr_exit("Error: Session timeout length must be longer than retransmission wait timeout length");
  
  if (cmd_args->powercmd != POWER_CMD_NONE && !cmd_args->common.hostname)
    ierr_exit("Error: Must specify target hostname(s) in non-interactive mode");

  if (cmd_args->ping_interval > cmd_args->ping_timeout)
    ierr_exit("Error: Ping timeout interval length must be "
              "longer than ping interval length");

  if (cmd_args->ping_consec_count > cmd_args->ping_packet_count)
    ierr_exit("Error: Ping consec count must be larger than ping packet count");
}

void 
ipmipower_argp_parse (int argc, char **argv, struct ipmipower_arguments *cmd_args)
{
  init_common_cmd_args_operator (&(cmd_args->common));
  init_hostrange_cmd_args (&(cmd_args->hostrange));

  /* ipmipower differences */
  cmd_args->common.driver_type = IPMI_DEVICE_LAN;
  cmd_args->common.driver_type_outofband_only = 1;
  cmd_args->common.session_timeout = 20000; /* 20 seconds */
  cmd_args->common.retransmission_timeout = 400; /* .4 seconds */

#ifndef NDEBUG
  cmd_args->rmcpdump = 0;
#endif /* NDEBUG */

  cmd_args->powercmd = POWER_CMD_NONE;
  cmd_args->on_if_off = 0;
  cmd_args->wait_until_on = 0;
  cmd_args->wait_until_off = 0;
  cmd_args->retransmission_wait_timeout = 500; /* .5 seconds  */
  cmd_args->retransmission_backoff_count = 8;
  cmd_args->ping_interval = 5000; /* 5 seconds */
  cmd_args->ping_timeout = 30000; /* 30 seconds */
  cmd_args->ping_packet_count = 10;
  cmd_args->ping_percent = 50;
  cmd_args->ping_consec_count = 5;

  argp_parse(&cmdline_config_file_argp, argc, argv, ARGP_IN_ORDER, NULL, &(cmd_args->common));
  
  _ipmipower_config_file_parse(cmd_args);
  
  argp_parse(&cmdline_argp, argc, argv, ARGP_IN_ORDER, NULL, cmd_args);

  /* achu: don't do these checks, we don't do inband, so checks aren't appropriate 
   * checks will be done in ipmipower_config_check_values().
   */
  /* verify_common_cmd_args (&(cmd_args->common)); */
  verify_hostrange_cmd_args (&(cmd_args->hostrange));
  
  _ipmipower_args_validate (cmd_args);
}
