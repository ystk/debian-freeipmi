/*****************************************************************************\
 *  $Id: ipmi-sdr-cache-common.c,v 1.5 2008/06/07 16:10:00 chu11 Exp $
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
#include <assert.h>

#include "freeipmi/sdr-cache/ipmi-sdr-cache.h"
#include "freeipmi/api/ipmi-sdr-repository-cmds-api.h"
#include "freeipmi/cmds/ipmi-sdr-repository-cmds.h"
#include "freeipmi/record-format/ipmi-sdr-record-format.h"

#include "ipmi-sdr-cache-common.h"
#include "ipmi-sdr-cache-defs.h"

#include "libcommon/ipmi-err-wrappers.h"
#include "libcommon/ipmi-fiid-wrappers.h"

#include "freeipmi-portability.h"

void
ipmi_sdr_cache_init_ctx(ipmi_sdr_cache_ctx_t ctx)
{
  assert(ctx);
  assert(ctx->magic == IPMI_SDR_CACHE_MAGIC);

  ctx->operation = IPMI_SDR_CACHE_OPERATION_UNINITIALIZED;

  ctx->sdr_version = 0;
  ctx->record_count = 0;
  ctx->most_recent_addition_timestamp = 0;
  ctx->most_recent_erase_timestamp = 0;

  ctx->fd = -1;
  ctx->file_size = 0;
  ctx->records_start_offset = 0;
  ctx->sdr_cache = NULL;
}

int
ipmi_sdr_cache_info(ipmi_sdr_cache_ctx_t ctx,
                    ipmi_ctx_t ipmi_ctx,
                    uint8_t *sdr_version,
                    uint16_t *record_count,
                    uint32_t *most_recent_addition_timestamp,
                    uint32_t *most_recent_erase_timestamp)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t val;
  int rv = -1;
  
  assert(ctx);
  assert(ctx->magic == IPMI_SDR_CACHE_MAGIC);
  assert(ipmi_ctx);
  assert(sdr_version);
  assert(record_count);
  assert(most_recent_addition_timestamp);
  assert(most_recent_erase_timestamp);
  
  SDR_CACHE_FIID_OBJ_CREATE_CLEANUP(obj_cmd_rs, 
                                    tmpl_cmd_get_sdr_repository_info_rs);

  SDR_CACHE_ERR_IPMI_ERROR_CLEANUP(!(ipmi_cmd_get_sdr_repository_info (ipmi_ctx, 
                                                                       obj_cmd_rs) < 0));

  *sdr_version = 0;
  SDR_CACHE_FIID_OBJ_GET_CLEANUP(obj_cmd_rs,
                                 "sdr_version_minor",
                                 &val);
  *sdr_version = val;

  SDR_CACHE_FIID_OBJ_GET_CLEANUP(obj_cmd_rs,
                                 "sdr_version_major",
                                 &val);
  *sdr_version |= (val << 4);

  *record_count = 0;
  SDR_CACHE_FIID_OBJ_GET_CLEANUP(obj_cmd_rs,
                                 "record_count",
                                 &val);
  *record_count = val;

  *most_recent_addition_timestamp = 0;
  SDR_CACHE_FIID_OBJ_GET_CLEANUP(obj_cmd_rs,
                                 "most_recent_addition_timestamp",
                                 &val);
  *most_recent_addition_timestamp = val;

  *most_recent_erase_timestamp = 0;
  SDR_CACHE_FIID_OBJ_GET_CLEANUP(obj_cmd_rs,
                                 "most_recent_erase_timestamp",
                                 &val);
  *most_recent_erase_timestamp = val;

  rv = 0;
 cleanup:
  if (obj_cmd_rs)
    fiid_obj_destroy(obj_cmd_rs);
  return rv;
}

const char *
ipmi_sdr_cache_record_type_str(ipmi_sdr_cache_ctx_t ctx,
			       uint8_t *sdr_record,
			       unsigned int sdr_record_len)
{
  fiid_obj_t obj_sdr_record_header = NULL;
  uint64_t record_type;
  int32_t sdr_record_header_len;
  char *rv = NULL;

  assert(ctx);
  assert(ctx->magic == IPMI_SDR_CACHE_MAGIC);
  assert(sdr_record);
  assert(sdr_record_len);

  FIID_TEMPLATE_LEN_BYTES_CLEANUP(sdr_record_header_len, tmpl_sdr_record_header);

  if (sdr_record_len < sdr_record_header_len)
    goto cleanup;
  
  FIID_OBJ_CREATE_CLEANUP(obj_sdr_record_header, tmpl_sdr_record_header);
  
  FIID_OBJ_SET_ALL_CLEANUP(obj_sdr_record_header,
			   sdr_record,
			   sdr_record_header_len);
  
  FIID_OBJ_GET_CLEANUP(obj_sdr_record_header, "record_type", &record_type);

  if (record_type == IPMI_SDR_FORMAT_FULL_RECORD)
    rv = "SDR Full Sensor Record";
  else if (record_type == IPMI_SDR_FORMAT_COMPACT_RECORD)
    rv = "SDR Compact Sensor Record";
  else if (record_type == IPMI_SDR_FORMAT_EVENT_ONLY_RECORD)
    rv = "SDR Event Only Record";
  else if (record_type == IPMI_SDR_FORMAT_ENTITY_ASSOCIATION_RECORD)
    rv = "SDR Entity Association Record";
  else if (record_type == IPMI_SDR_FORMAT_DEVICE_RELATIVE_ENTITY_ASSOCIATION_RECORD)
    rv = "SDR Device Relative Entity Association Record";
  else if (record_type == IPMI_SDR_FORMAT_GENERIC_DEVICE_LOCATOR_RECORD)
    rv = "SDR Generic Device Locator Record";
  else if (record_type == IPMI_SDR_FORMAT_FRU_DEVICE_LOCATOR_RECORD)
    rv = "SDR FRU Device Locator Record";
  else if (record_type == IPMI_SDR_FORMAT_MANAGEMENT_CONTROLLER_DEVICE_LOCATOR_RECORD)
    rv = "SDR Management Controller Device Locator Record";
  else if (record_type == IPMI_SDR_FORMAT_MANAGEMENT_CONTROLLER_CONFIRMATION_RECORD)
    rv = "SDR Management Controller Confirmation Record";
  else if (record_type == IPMI_SDR_FORMAT_BMC_MESSAGE_CHANNEL_INFO_RECORD)
    rv = "SDR Message Channel Info Record";
  else if (record_type == IPMI_SDR_FORMAT_OEM_RECORD)
    rv = "SDR OEM Record";
  else
    rv = "SDR Unknown Record";

 cleanup:
  FIID_OBJ_DESTROY(obj_sdr_record_header);
  return rv;
}
