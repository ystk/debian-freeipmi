/*****************************************************************************\
 *  $Id: ipmi-fru-util.h,v 1.7 2008/03/28 00:14:34 chu11 Exp $
 *****************************************************************************
 *  Copyright (C) 2007-2008 Lawrence Livermore National Security, LLC.
 *  Copyright (C) 2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Albert Chu <chu11@llnl.gov>
 *  UCRL-CODE-232183
 *
 *  This file is part of Ipmi-fru, a tool used for retrieving
 *  motherboard field replaceable unit (FRU) information. For details,
 *  see http://www.llnl.gov/linux/.
 *
 *  Ipmi-fru is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  Ipmi-fru is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Ipmi-fru.  If not, see <http://www.gnu.org/licenses/>.
\*****************************************************************************/

#ifndef _IPMI_FRU_UTIL_H
#define _IPMI_FRU_UTIL_H

fru_err_t ipmi_fru_read_fru_data (ipmi_fru_state_data_t *state_data,
                                  uint8_t device_id,
                                  uint8_t *frubuf,
                                  unsigned int frubuflen,
                                  unsigned int offset_in_bytes,
                                  unsigned int fru_read_bytes);

fru_err_t ipmi_fru_output_type_length_field(ipmi_fru_state_data_t *state_data,
                                            uint8_t *frubuf,
                                            unsigned int frubuflen,
                                            unsigned int offset_in_bytes,
                                            uint8_t *language_code,
                                            unsigned int *len_parsed,
                                            char *str);

fru_err_t ipmi_fru_get_info_area_length(ipmi_fru_state_data_t *state_data,
                                        uint8_t device_id,
                                        unsigned int offset_in_bytes,
                                        char *str,
                                        uint64_t *info_area_length);

fru_err_t ipmi_fru_check_checksum(ipmi_fru_state_data_t *state_data,
                                  uint8_t *frubuf,
                                  uint64_t length_in_bytes,
                                  uint8_t checksum_init,
                                  char *str);

fru_err_t ipmi_fru_dump_hex(ipmi_fru_state_data_t *state_data,
                            uint8_t *frubuf,
                            uint64_t length_in_bytes,
                            char *str);

#endif
