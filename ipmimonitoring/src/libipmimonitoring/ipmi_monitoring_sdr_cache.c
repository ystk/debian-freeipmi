/*****************************************************************************\
 *  $Id: ipmi_monitoring_sdr_cache.c,v 1.17 2008/09/19 20:37:18 chu11 Exp $
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#if HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#include <assert.h>
#include <errno.h>

#include "ipmi_monitoring.h"
#include "ipmi_monitoring_debug.h"
#include "ipmi_monitoring_defs.h"
#include "ipmi_monitoring_fiid_wrappers.h"
#include "ipmi_monitoring_ipmi_communication.h"
#include "ipmi_monitoring_sdr_cache.h"

#include "freeipmi-portability.h"

#define IPMI_MONITORING_SDR_CACHE_DIRECTORY      IPMI_MONITORING_SDR_CACHE_DIR

#define IPMI_MONITORING_SDR_CACHE_FILENAME       "ipmimonitoringsdrcache"
#define IPMI_MONITORING_SDR_CACHE_INBAND         "localhost"

char sdr_cache_directory[MAXPATHLEN+1];
int sdr_cache_directory_set = 0;

char sdr_cache_filename_format[MAXPATHLEN+1];
int sdr_cache_filename_format_set = 0;

extern uint32_t _ipmi_monitoring_flags;

static int
_ipmi_monitoring_sdr_cache_ctx_init(ipmi_monitoring_ctx_t c, const char *hostname)
{
  assert(c);
  assert(c->magic == IPMI_MONITORING_MAGIC);

  if (!(c->sc))
    {
      if (!(c->sc = ipmi_sdr_cache_ctx_create()))
        {
          IPMI_MONITORING_DEBUG(("ipmi_sdr_cache_create: %s", strerror(errno)));
          if (errno == EPERM || errno == EACCES)
            c->errnum = IPMI_MONITORING_ERR_PERMISSION;
          else
            c->errnum = IPMI_MONITORING_ERR_INTERNAL_ERROR;
          return -1;
        }
      
      if (_ipmi_monitoring_flags & IPMI_MONITORING_FLAGS_DEBUG_IPMI_PACKETS)
        {
          /* Don't error out, if this fails we can still continue */
          if (ipmi_sdr_cache_ctx_set_flags(c->sc, IPMI_SDR_CACHE_FLAGS_DEBUG_DUMP) < 0)
            IPMI_MONITORING_DEBUG(("ipmi_sdr_cache_ctx_set_flags: %s", ipmi_sdr_cache_ctx_strerror(ipmi_sdr_cache_ctx_errnum(c->sc))));

          if (hostname)
            {
              if (ipmi_sdr_cache_ctx_set_debug_prefix(c->sc, hostname) < 0)
                IPMI_MONITORING_DEBUG(("ipmi_sdr_cache_ctx_set_debug_prefix: %s",
                                       ipmi_sdr_cache_ctx_strerror(ipmi_sdr_cache_ctx_errnum(c->sc))));
            }
        }
    } 

  return 0;
}

static int
_ipmi_monitoring_sdr_cache_filename(ipmi_monitoring_ctx_t c,
                                    const char *hostname,
                                    char *buf,
                                    unsigned int buflen)
{
  char sdr_cache_filename[MAXPATHLEN+1];
  char *dir;

  assert(c);
  assert(c->magic == IPMI_MONITORING_MAGIC);
  assert(buf);
  assert(buflen);

  if (sdr_cache_directory_set)
    dir = sdr_cache_directory;
  else
    dir = IPMI_MONITORING_SDR_CACHE_DIRECTORY;

  if (!hostname)
    hostname = IPMI_MONITORING_SDR_CACHE_INBAND;

  memset(sdr_cache_filename, '\0', MAXPATHLEN+1);
  if (sdr_cache_filename_format_set)
    {
      int index = 0;
      int percent = 0;
      char *str;

      str = sdr_cache_filename_format;
      while (str && *str && index < MAXPATHLEN)
        {
          if (percent)
            {
              percent = 0;
              if (*str == '%')
                {
                  sdr_cache_filename[index] = *str;
                  index++;
                }
              else if (*str == 'L')
                {
                  char local_hostname[MAXHOSTNAMELEN+1];
                  char *ptr;
                  
                  memset(local_hostname, '\0', MAXHOSTNAMELEN+1);
                  if (gethostname(local_hostname, MAXHOSTNAMELEN) < 0)
                    {
                      IPMI_MONITORING_DEBUG(("gethostname: %s", strerror(errno)));
                      c->errnum = IPMI_MONITORING_ERR_SYSTEM_ERROR;
                      return -1;
                    }

                  /* shorten hostname if necessary */
                  if ((ptr = strchr(local_hostname, '.')))
                    *ptr = '\0';

                  if ((index + strlen(local_hostname)) >= MAXPATHLEN)
                    {
                      IPMI_MONITORING_DEBUG(("_ipmi_monitoring_sdr_cache_filename: overflow"));
                      c->errnum = IPMI_MONITORING_ERR_INTERNAL_ERROR;
                      return -1;
                    }                  

                  strcat(&sdr_cache_filename[index], local_hostname);
                  index += strlen(local_hostname);
                }
              else if (*str == 'H')
                {
                  if ((index + strlen(hostname)) >= MAXPATHLEN)
                    {
                      IPMI_MONITORING_DEBUG(("_ipmi_monitoring_sdr_cache_filename: overflow"));
                      c->errnum = IPMI_MONITORING_ERR_INTERNAL_ERROR;
                      return -1;
                    }
                  strcat(&sdr_cache_filename[index], hostname);
                  index += strlen(hostname);
                }
              else
                {
                  sdr_cache_filename[index] = '%';
                  index++;
                  sdr_cache_filename[index] = *str;
                  index++;
                }
            }
          else if (*str == '%')
            percent = 1;
          else 
            {
              sdr_cache_filename[index] = *str;
              index++;
            }
          str++;
        }
    }
  else
    snprintf(sdr_cache_filename,
             MAXPATHLEN,
             "%s.%s",
             IPMI_MONITORING_SDR_CACHE_FILENAME,
             hostname);
             
  memset(buf, '\0', buflen);
  snprintf(buf, 
           buflen - 1, 
           "%s/%s", 
           dir,
           sdr_cache_filename);

  return 0;
}

static int
_ipmi_monitoring_sdr_cache_retrieve(ipmi_monitoring_ctx_t c,
                                    const char *hostname,
                                    char *filename)
{
  assert(c);
  assert(c->magic == IPMI_MONITORING_MAGIC);
  assert(c->sc);
  assert(c->ipmi_ctx);
  assert(filename && strlen(filename));
  
  if (ipmi_sdr_cache_create(c->sc,
                            c->ipmi_ctx,
                            filename,
                            IPMI_SDR_CACHE_CREATE_FLAGS_DEFAULT,
                            IPMI_SDR_CACHE_VALIDATION_FLAGS_DEFAULT,
                            NULL,
                            NULL) < 0)
    {
      IPMI_MONITORING_DEBUG(("ipmi_sdr_cache_create: %s", ipmi_sdr_cache_ctx_strerror(ipmi_sdr_cache_ctx_errnum(c->sc))));
      if (ipmi_sdr_cache_ctx_errnum(c->sc) == IPMI_SDR_CACHE_CTX_ERR_FILESYSTEM)
        c->errnum = IPMI_MONITORING_ERR_SDR_CACHE_FILESYSTEM;
      else if (ipmi_sdr_cache_ctx_errnum(c->sc) == IPMI_SDR_CACHE_CTX_ERR_PERMISSION)
        c->errnum = IPMI_MONITORING_ERR_SDR_CACHE_PERMISSION;
      else if (ipmi_sdr_cache_ctx_errnum(c->sc) == IPMI_SDR_CACHE_CTX_ERR_IPMI_ERROR)
        ipmi_monitoring_ipmi_ctx_error_convert(c);
      else if (ipmi_sdr_cache_ctx_errnum(c->sc) == IPMI_SDR_CACHE_CTX_ERR_SYSTEM_ERROR)
        c->errnum = IPMI_MONITORING_ERR_SYSTEM_ERROR;
      else
        c->errnum = IPMI_MONITORING_ERR_INTERNAL_ERROR;
      return -1;
    }
         
  return 0;
}

static int
_ipmi_monitoring_sdr_cache_delete(ipmi_monitoring_ctx_t c,
                                  const char *hostname,
                                  char *filename)
{
  assert(c);
  assert(c->magic == IPMI_MONITORING_MAGIC);
  assert(c->sc);
  assert(c->ipmi_ctx);

  if (ipmi_sdr_cache_delete(c->sc, filename) < 0)
    {
      if (ipmi_sdr_cache_ctx_errnum(c->sc) !=  IPMI_SDR_CACHE_CTX_ERR_FILENAME_INVALID)
        {
          IPMI_MONITORING_DEBUG(("ipmi_sdr_cache_delete: %s", ipmi_sdr_cache_ctx_strerror(ipmi_sdr_cache_ctx_errnum(c->sc))));
          if (ipmi_sdr_cache_ctx_errnum(c->sc) == IPMI_SDR_CACHE_CTX_ERR_PERMISSION)
            c->errnum = IPMI_MONITORING_ERR_SDR_CACHE_PERMISSION;
          else 
            c->errnum = IPMI_MONITORING_ERR_INTERNAL_ERROR;
          return -1;
        }
    }

  return 0;
}

int
ipmi_monitoring_sdr_cache_load(ipmi_monitoring_ctx_t c,
                               const char *hostname)
{
  char filename[MAXPATHLEN+1];
  
  assert(c);
  assert(c->magic == IPMI_MONITORING_MAGIC);
  assert(c->ipmi_ctx);

  if (_ipmi_monitoring_sdr_cache_filename(c, hostname, filename, MAXPATHLEN + 1) < 0)
    goto cleanup;

  if (_ipmi_monitoring_sdr_cache_ctx_init(c, hostname) < 0)
    goto cleanup;

  if (ipmi_sdr_cache_open(c->sc, 
                          c->ipmi_ctx,
                          filename) < 0)
    {
      if (ipmi_sdr_cache_ctx_errnum(c->sc) == IPMI_SDR_CACHE_CTX_ERR_CACHE_READ_CACHE_DOES_NOT_EXIST)
        {
          if (_ipmi_monitoring_sdr_cache_retrieve(c, hostname, filename) < 0)
            goto cleanup;
        }
      else if (ipmi_sdr_cache_ctx_errnum(c->sc) == IPMI_SDR_CACHE_CTX_ERR_CACHE_INVALID
               || ipmi_sdr_cache_ctx_errnum(c->sc) == IPMI_SDR_CACHE_CTX_ERR_CACHE_OUT_OF_DATE)
        {
          if (_ipmi_monitoring_sdr_cache_delete(c, hostname, filename) < 0)
            goto cleanup;

          if (_ipmi_monitoring_sdr_cache_retrieve(c, hostname, filename) < 0)
            goto cleanup;
        }
      else if (ipmi_sdr_cache_ctx_errnum(c->sc) == IPMI_SDR_CACHE_CTX_ERR_FILESYSTEM)
        {
          c->errnum = IPMI_MONITORING_ERR_SDR_CACHE_FILESYSTEM;
          goto cleanup;
        }
      else if (ipmi_sdr_cache_ctx_errnum(c->sc) == IPMI_SDR_CACHE_CTX_ERR_PERMISSION)
        {
          c->errnum = IPMI_MONITORING_ERR_SDR_CACHE_PERMISSION;
          goto cleanup;
        }
      else
        {
          IPMI_MONITORING_DEBUG(("ipmi_sdr_cache_open: %s", ipmi_sdr_cache_ctx_strerror(ipmi_sdr_cache_ctx_errnum(c->sc))));
          c->errnum = IPMI_MONITORING_ERR_INTERNAL_ERROR;
          goto cleanup;
        }

      /* 2nd try after the sdr was retrieved*/
      if (ipmi_sdr_cache_open(c->sc, 
                              c->ipmi_ctx,
                              filename) < 0)
        {
          if (ipmi_sdr_cache_ctx_errnum(c->sc) == IPMI_SDR_CACHE_CTX_ERR_FILESYSTEM)
            {
              c->errnum = IPMI_MONITORING_ERR_SDR_CACHE_FILESYSTEM;
              goto cleanup;
            }
          else if (ipmi_sdr_cache_ctx_errnum(c->sc) == IPMI_SDR_CACHE_CTX_ERR_PERMISSION)
            {
              c->errnum = IPMI_MONITORING_ERR_SDR_CACHE_PERMISSION;
              goto cleanup;
            }
          else
            {
              IPMI_MONITORING_DEBUG(("ipmi_sdr_cache_open: %s", ipmi_sdr_cache_ctx_strerror(ipmi_sdr_cache_ctx_errnum(c->sc))));
              c->errnum = IPMI_MONITORING_ERR_INTERNAL_ERROR;
              goto cleanup;
            }
        }
    }

  return 0;

 cleanup:
  if (c->sc)
    {
      if (strlen(filename))
        ipmi_sdr_cache_delete(c->sc, filename);
      ipmi_sdr_cache_ctx_destroy(c->sc);
      c->sc = NULL;
    }
  return -1;
}

int
ipmi_monitoring_sdr_cache_unload(ipmi_monitoring_ctx_t c)
{
  assert(c);
  assert(c->magic == IPMI_MONITORING_MAGIC);

  if (c->sc)
    {
      ipmi_sdr_cache_close(c->sc);
      ipmi_sdr_cache_ctx_destroy(c->sc);
      c->sc = NULL;
    }
  return 0;
}

int
ipmi_monitoring_sdr_cache_flush(ipmi_monitoring_ctx_t c,
                                const char *hostname)
{
  char filename[MAXPATHLEN+1];
  
  assert(c);
  assert(c->magic == IPMI_MONITORING_MAGIC);

  if (_ipmi_monitoring_sdr_cache_filename(c, hostname, filename, MAXPATHLEN + 1) < 0)
    goto cleanup;

  if (_ipmi_monitoring_sdr_cache_ctx_init(c, hostname) < 0)
    goto cleanup;

  if (_ipmi_monitoring_sdr_cache_delete(c, hostname, filename) < 0)
    goto cleanup;

  return 0;

 cleanup:
  if (c->sc)
    {
      ipmi_sdr_cache_ctx_destroy(c->sc);
      c->sc = NULL;
    }
  return -1;
}
