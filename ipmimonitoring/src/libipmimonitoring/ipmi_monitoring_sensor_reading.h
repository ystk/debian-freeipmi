/*****************************************************************************\
 *  $Id: ipmi_monitoring_sensor_reading.h,v 1.7 2008/03/28 00:14:44 chu11 Exp $
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

#ifndef _IPMI_MONITORING_SENSOR_READING_H
#define _IPMI_MONITORING_SENSOR_READING_H

#include "ipmi_monitoring.h"

int ipmi_monitoring_get_sensor_reading(ipmi_monitoring_ctx_t c, 
                                       unsigned int sensor_reading_flags,
                                       uint8_t *sdr_record,
                                       unsigned int sdr_record_len,
                                       unsigned int *sensor_groups,
                                       unsigned int sensor_groups_len);

#endif /* _IPMI_MONITORING_SENSOR_READING_H */
