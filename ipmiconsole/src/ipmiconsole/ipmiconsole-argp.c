/*****************************************************************************\
 *  $Id: ipmiconsole-argp.c,v 1.21 2008/08/12 18:14:37 chu11 Exp $
 *****************************************************************************
 *  Copyright (C) 2007-2008 Lawrence Livermore National Security, LLC.
 *  Copyright (C) 2006-2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Albert Chu <chu11@llnl.gov>
 *  UCRL-CODE-221226
 *  
 *  This file is part of Ipmiconsole, a set of IPMI 2.0 SOL libraries
 *  and utilities.  For details, see http://www.llnl.gov/linux/.
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
#include <sys/param.h>
#include <assert.h>
#include <errno.h>

#include <ipmiconsole.h>        /* lib ipmiconsole.h */

#include "ipmiconsole_.h"       /* tool ipmiconsole.h */
#include "ipmiconsole-argp.h"

#include "freeipmi-portability.h"
#include "conffile.h"
#include "error.h"
#include "secure.h"
#include "tool-cmdline-common.h"
#include "tool-common.h"
#include "tool-config-file-common.h"

#define IPMICONSOLE_CONFIG_FILE_DEFAULT "/etc/ipmiconsole.conf"

const char *argp_program_version =
  "ipmiconsole - " PACKAGE_VERSION "\n"
  "Copyright (C) 2007-2008 Lawrence Livermore National Security, LLC.\n"
  "Copyright (C) 2006-2007 The Regents of the University of California.\n"
  "This program is free software; you may redistribute it under the terms of\n"
  "the GNU General Public License.  This program has absolutely no warranty.";

const char *argp_program_bug_address = 
  "<" PACKAGE_BUGREPORT ">";

static char cmdline_doc[] = 
  "ipmiconsole - IPMI console utility";

static char cmdline_args_doc[] = "";

static struct argp_option cmdline_options[] =
  {
    ARGP_COMMON_OPTIONS_OUTOFBAND,
    ARGP_COMMON_OPTIONS_CIPHER_SUITE_ID,
    /* legacy */
    {"cipher-suite-id", 'c', "CIPHER-SUITE-ID", OPTION_HIDDEN,                         
     "Specify the IPMI 2.0 cipher suite ID to use.", 14},
    ARGP_COMMON_OPTIONS_PRIVILEGE_LEVEL_ADMIN,
    ARGP_COMMON_OPTIONS_CONFIG_FILE,
    ARGP_COMMON_OPTIONS_WORKAROUND_FLAGS,
    ARGP_COMMON_OPTIONS_DEBUG,
    {"escape-char", ESCAPE_CHAR_KEY, "CHAR", 0,
     "Specify an alternate escape character (default char '&')", 30},
    {"dont-steal", DONT_STEAL_KEY, 0, 0,
     "Do not steal an SOL session if one is already detected as being in use.", 31},
    {"deactivate", DEACTIVATE_KEY, 0, 0,
     "Deactivate a SOL session if one is detected as being in use and exit.", 32},
    {"lock-memory", LOCK_MEMORY_KEY, 0, 0,
     "Lock sensitive information (such as usernames and passwords) in memory.", 33},
#ifndef NDEBUG
    {"debugfile", DEBUGFILE_KEY, 0, 0,
     "Output debugging to the debugfile rather than to standard output.", 34},
    {"noraw", NORAW_KEY, 0, 0,
     "Don't enter terminal raw mode.", 35},
#endif
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
  struct ipmiconsole_arguments *cmd_args = state->input;
  error_t ret;

  switch (key)
    {
    case ESCAPE_CHAR_KEY:          /* --escape-char */
      cmd_args->escape_char = *arg;
      break;
    case DONT_STEAL_KEY:       /* --dont-steal */
      cmd_args->dont_steal++;
      break;
    case DEACTIVATE_KEY:       /* --deactivate */
      cmd_args->deactivate++;
      break;
    case LOCK_MEMORY_KEY:       /* --lock-memory */
      cmd_args->lock_memory++;
      break;
#ifndef NDEBUG
    case DEBUGFILE_KEY:	/* --debugfile */
      cmd_args->debugfile++;
      break;
    case NORAW_KEY:	/* --noraw */
      cmd_args->noraw++;
      break;
#endif /* NDEBUG */
    case ARGP_KEY_ARG:
      /* Too many arguments. */
      argp_usage (state);
      break;
    case ARGP_KEY_END:
      break;
      /* 'c' for backwards compatability */
    case 'c':	/* --cipher-suite-id */
      ret = common_parse_opt (ARGP_CIPHER_SUITE_ID_KEY, arg, state, &(cmd_args->common));
      return ret;
      break;
    default:
      ret = common_parse_opt (key, arg, state, &(cmd_args->common));
      return ret;
    }

  return 0;
}

static void
_ipmiconsole_config_file_parse(struct ipmiconsole_arguments *cmd_args)
{
  struct config_file_data_ipmiconsole config_file_data;

  memset(&config_file_data, 
         '\0',
         sizeof(struct config_file_data_ipmiconsole));

  if (!cmd_args->common.config_file)
    {
      /* try legacy file first */
      if (!config_file_parse (IPMICONSOLE_CONFIG_FILE_DEFAULT,
                              1,         /* do not exit if file not found */
                              &(cmd_args->common),
                              NULL,
                              NULL,
                              CONFIG_FILE_OUTOFBAND,
                              CONFIG_FILE_TOOL_IPMICONSOLE,
                              &config_file_data))
        goto out;
    }

  if (config_file_parse (cmd_args->common.config_file,
                         0,
                         &(cmd_args->common),
                         NULL,
                         NULL,
                         CONFIG_FILE_OUTOFBAND,
                         CONFIG_FILE_TOOL_IPMICONSOLE,
                         &config_file_data) < 0)
    {
      fprintf(stderr, "config_file_parse: %s\n", strerror(errno));
      exit(1);
    }

 out:
  if (config_file_data.escape_char_count)
    cmd_args->escape_char = config_file_data.escape_char;
  if (config_file_data.dont_steal_count)
    cmd_args->dont_steal = config_file_data.dont_steal;
  if (config_file_data.lock_memory_count)
    cmd_args->lock_memory = config_file_data.lock_memory;
}

void
_ipmiconsole_args_validate (struct ipmiconsole_arguments *cmd_args)
{
  if (!cmd_args->common.hostname)
    err_exit("hostname input required");
}

void
ipmiconsole_argp_parse (int argc, char **argv, struct ipmiconsole_arguments *cmd_args)
{
  init_common_cmd_args_admin (&(cmd_args->common));
  cmd_args->escape_char = '&';
  cmd_args->dont_steal = 0;
  cmd_args->deactivate = 0;
  cmd_args->lock_memory = 0;
#ifndef NDEBUG
  cmd_args->debugfile = 0;
  cmd_args->noraw = 0;
#endif /* NDEBUG */

  /* special case to ipmiconsole, different timeout defaults */
  cmd_args->common.session_timeout = 60000;
  cmd_args->common.retransmission_timeout = 500;

  argp_parse (&cmdline_config_file_argp, argc, argv, ARGP_IN_ORDER, NULL, &(cmd_args->common));

  _ipmiconsole_config_file_parse(cmd_args);

  argp_parse (&cmdline_argp, argc, argv, ARGP_IN_ORDER, NULL, cmd_args);
  verify_common_cmd_args (&(cmd_args->common));
  _ipmiconsole_args_validate (cmd_args);
}

