/*****************************************************************************\
 *  $Id: ipmimonitoring-argp.c,v 1.21.2.2 2009/01/27 00:00:36 chu11 Exp $
 *****************************************************************************
 *  Copyright (C) 2007-2008 Lawrence Livermore National Security, LLC.
 *  Copyright (C) 2006-2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Albert Chu <chu11@llnl.gov>
 *  UCRL-CODE-222073
 *
 *  This file is part of Ipmimonitoring, an IPMI sensor monitoring
 *  library.  For details, see http://www.llnl.gov/linux/.
 *
 *  Ipmimonitoring is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  Ipmimonitoring is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Ipmimonitoring.  If not, see <http://www.gnu.org/licenses/>.
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

#include "ipmimonitoring.h"
#include "ipmimonitoring-argp.h"

#include "freeipmi-portability.h"
#include "tool-cmdline-common.h"
#include "tool-config-file-common.h"

const char *argp_program_version = 
  "ipmimonitoring - " PACKAGE_VERSION "\n"
  "Copyright (C) 2007-2008 Lawrence Livermore National Security, LLC.\n"
  "Copyright (C) 2006-2007 The Regents of the University of California.\n"
  "This program is free software; you may redistribute it under the terms of\n"
  "the GNU General Public License.  This program has absolutely no warranty.";

const char *argp_program_bug_address = 
  "<" PACKAGE_BUGREPORT ">";

static char cmdline_doc[] = 
  "ipmimonitoring - IPMI monitoring utility";

static char cmdline_args_doc[] = "";

static struct argp_option cmdline_options[] = 
  {
    ARGP_COMMON_OPTIONS_DRIVER,
    ARGP_COMMON_OPTIONS_INBAND,
    ARGP_COMMON_OPTIONS_OUTOFBAND,
    ARGP_COMMON_OPTIONS_AUTHENTICATION_TYPE,
    ARGP_COMMON_OPTIONS_CIPHER_SUITE_ID,
    ARGP_COMMON_OPTIONS_PRIVILEGE_LEVEL_OPERATOR,
    ARGP_COMMON_OPTIONS_CONFIG_FILE,
    ARGP_COMMON_OPTIONS_WORKAROUND_FLAGS,
    ARGP_COMMON_SDR_OPTIONS,
    ARGP_COMMON_HOSTRANGED_OPTIONS,
    ARGP_COMMON_OPTIONS_DEBUG,
    {"verbose", VERBOSE_KEY, 0, 0, 
     "Increase verbosity in output.", 30}, 
    /* maintain "regenerate-sdr-cache" for backwards compatability */
    {"regenerate-sdr-cache", REGENERATE_SDR_CACHE_KEY, 0, OPTION_HIDDEN,
     "Regenerate the SDR cache.", 31},
    /* maintain "cache-dir" for backwards compatability */
    {"cache-dir", CACHE_DIR_KEY, "DIRECTORY", OPTION_HIDDEN,
     "Specify an alternate directory to read and write SDR caches..", 32},
    {"quiet-readings", QUIET_READINGS_KEY,  0, 0,
     "Do not output sensor readings, only states.", 33},
    {"list-groups",    LIST_GROUPS_KEY,    0, 0, 
     "List sensor groups.", 34}, 
    {"groups",         GROUPS_KEY,       "GROUP-LIST", 0, 
     "Show sensors belonging to a specific group.", 35}, 
    {"sensors",        SENSORS_KEY, "SENSORS-LIST", 0, 
     "Show sensors by record id.  Accepts space or comma separated lists", 36}, 
    {"bridge-sensors", BRIDGE_SENSORS_KEY, NULL, 0,
     "Bridge addresses to read non-BMC owned sensors.", 37},
    {"sensor-config-file", SENSOR_CONFIG_FILE_KEY, "FILE", 0,
     "Specify an alternate sensor configuration file.", 38},
    { 0 }
  };

static error_t cmdline_parse (int key, char *arg, struct argp_state *state);

static struct argp cmdline_argp = { cmdline_options,
                                    cmdline_parse, 
                                    cmdline_args_doc, 
                                    cmdline_doc };

static struct argp cmdline_config_file_argp = { cmdline_options,
                                                cmdline_config_file_parse,
                                                cmdline_args_doc,
                                                cmdline_doc };

static error_t 
cmdline_parse (int key, char *arg, struct argp_state *state)
{
  struct ipmimonitoring_arguments *cmd_args = state->input;
  char *ptr;
  char *tok;
  int value;
  error_t ret;
  
  switch (key)
    {
    case VERBOSE_KEY:
      cmd_args->verbose = 1;
      break;
      /* legacy option */
    case REGENERATE_SDR_CACHE_KEY:
      cmd_args->regenerate_sdr_cache = 1;
      break;
      /* legacy option */
    case CACHE_DIR_KEY:
      return sdr_parse_opt (ARGP_SDR_CACHE_DIRECTORY_KEY, 
                            arg, 
                            state, 
                            &(cmd_args->sdr));
      break;
    case QUIET_READINGS_KEY:
      cmd_args->quiet_readings = 1;
      break;
    case LIST_GROUPS_KEY:
      cmd_args->list_groups = 1;
      break;
    case GROUPS_KEY:
      cmd_args->groups_wanted = 1;
      tok = strtok(arg, " ,");
      while (tok && cmd_args->groups_length < IPMIMONITORING_MAX_GROUPS)
        {
          strncpy(cmd_args->groups[cmd_args->groups_length],
                  tok,
                  IPMIMONITORING_MAX_GROUPS_STRING_LENGTH);
          cmd_args->groups_length++;
          tok = strtok(NULL, " ,");
        }
      break;
    case SENSORS_KEY:
      cmd_args->sensors_wanted = 1;
      tok = strtok(arg, " ,");
      while (tok && cmd_args->sensors_length < IPMIMONITORING_MAX_RECORD_IDS)
        {
          value = 0;
          ptr = NULL;
          errno = 0;

          value = strtol(tok, &ptr, 10);

          if (errno
              || ptr[0] != '\0'
              || value < 0
              || value < IPMI_SDR_RECORD_ID_FIRST
              || value > IPMI_SDR_RECORD_ID_LAST)
            {
              fprintf (stderr, "invalid sensor record id: %d\n", value);
              exit(1);
            }

          cmd_args->sensors[cmd_args->sensors_length] = value;
          cmd_args->sensors_length++;
          tok = strtok(NULL, " ,");
        }
      break;
    case BRIDGE_SENSORS_KEY:
      cmd_args->bridge_sensors = 1;
      break;
    case SENSOR_CONFIG_FILE_KEY:
      if (cmd_args->sensor_config_file)
        free (cmd_args->sensor_config_file);
      if (!(cmd_args->sensor_config_file = strdup(arg)))
        {
          perror("strdup");
          exit(1);
        }
      break;
    case ARGP_KEY_ARG:
      /* Too many arguments. */
      argp_usage (state);
      break;
    case ARGP_KEY_END:
      break;
    default:
      ret = common_parse_opt (key, arg, state, &(cmd_args->common));
      if (ret == ARGP_ERR_UNKNOWN)
        ret = sdr_parse_opt (key, arg, state, &(cmd_args->sdr));
      if (ret == ARGP_ERR_UNKNOWN)
        ret = hostrange_parse_opt (key, arg, state, &(cmd_args->hostrange));
      return ret;
    }
  
  return 0;
}

static void
_ipmimonitoring_config_file_parse(struct ipmimonitoring_arguments *cmd_args)
{
  struct config_file_data_ipmimonitoring config_file_data;

  memset(&config_file_data,
         '\0',
         sizeof(struct config_file_data_ipmimonitoring));

  if (config_file_parse (cmd_args->common.config_file,
                         0,
                         &(cmd_args->common),
                         &(cmd_args->sdr),
                         &(cmd_args->hostrange),
                         CONFIG_FILE_INBAND | CONFIG_FILE_OUTOFBAND | CONFIG_FILE_SDR | CONFIG_FILE_HOSTRANGE,
                         CONFIG_FILE_TOOL_IPMIMONITORING,
                         &config_file_data) < 0)
    {
      fprintf(stderr, "config_file_parse: %s\n", strerror(errno));
      exit(1);
    }

  if (config_file_data.quiet_readings_count)
    cmd_args->quiet_readings = config_file_data.quiet_readings;
  if (config_file_data.groups_count && config_file_data.groups_length)
    {
      int i;
      
      assert(IPMIMONITORING_MAX_GROUPS == CONFIG_FILE_IPMIMONITORING_MAX_GROUPS);
      assert(IPMIMONITORING_MAX_GROUPS_STRING_LENGTH == CONFIG_FILE_IPMIMONITORING_MAX_GROUPS_STRING_LENGTH);
      
      for (i = 0; i < config_file_data.groups_length; i++)
        strncpy(cmd_args->groups[i],
                config_file_data.groups[i],
                IPMIMONITORING_MAX_GROUPS_STRING_LENGTH);
      cmd_args->groups_wanted++;
      cmd_args->groups_length = config_file_data.groups_length;
    }
  if (config_file_data.bridge_sensors_count)
    cmd_args->bridge_sensors = config_file_data.bridge_sensors;
  if (config_file_data.sensor_config_file_count)
    cmd_args->sensor_config_file = config_file_data.sensor_config_file;
}

void 
ipmimonitoring_argp_parse (int argc, char **argv, struct ipmimonitoring_arguments *cmd_args)
{
  int i;

  init_common_cmd_args_operator (&(cmd_args->common));
  init_sdr_cmd_args (&(cmd_args->sdr));
  init_hostrange_cmd_args (&(cmd_args->hostrange));
  cmd_args->verbose = 0;
  cmd_args->regenerate_sdr_cache = 0;
  cmd_args->quiet_readings = 0;
  cmd_args->list_groups = 0;
  cmd_args->groups_wanted = 0;
  for (i = 0; i < IPMIMONITORING_MAX_GROUPS; i++)
    memset(cmd_args->groups[i],
           '\0',
           IPMIMONITORING_MAX_GROUPS_STRING_LENGTH+1);
  cmd_args->groups_length = 0;
  cmd_args->sensors_wanted = 0;
  memset(cmd_args->sensors, 
         '\0', 
         sizeof(unsigned int)*IPMIMONITORING_MAX_RECORD_IDS);
  cmd_args->sensors_length = 0;
  cmd_args->bridge_sensors = 0;
  cmd_args->sensor_config_file = NULL;

  memset(&(cmd_args->conf), '\0', sizeof(struct ipmi_monitoring_ipmi_config));
  cmd_args->ipmimonitoring_flags = 0;
  memset(cmd_args->ipmimonitoring_groups,
         '\0',
         sizeof(unsigned int)*IPMIMONITORING_MAX_GROUPS);

  cmd_args->ipmimonitoring_groups_length = 0;

  argp_parse (&cmdline_config_file_argp, argc, argv, ARGP_IN_ORDER, NULL, &(cmd_args->common));

  _ipmimonitoring_config_file_parse(cmd_args);

  argp_parse (&cmdline_argp, argc, argv, ARGP_IN_ORDER, NULL, cmd_args);
  verify_common_cmd_args (&(cmd_args->common));
  verify_sdr_cmd_args (&(cmd_args->sdr));
  verify_hostrange_cmd_args (&(cmd_args->hostrange));
}


