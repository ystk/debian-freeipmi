/*****************************************************************************\
 *  $Id: ipmiconsole_ctx.h,v 1.13 2008/04/18 01:30:09 chu11 Exp $
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
 *  Ipmiconsole is free software; you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by the 
 *  Free Software Foundation; either version 2 of the License, or (at your 
 *  option) any later version.
 *  
 *  Ipmiconsole is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
 *  for more details.
 *  
 *  You should have received a copy of the GNU General Public License along
 *  with Ipmiconsole.  If not, see <http://www.gnu.org/licenses/>.
\*****************************************************************************/

#ifndef _IPMICONSOLE_CTX_H
#define _IPMICONSOLE_CTX_H

#include "ipmiconsole.h"

int ipmiconsole_ctx_setup(ipmiconsole_ctx_t c);

void ipmiconsole_ctx_cleanup(ipmiconsole_ctx_t c);

/* Wrapper for list callback */
void ipmiconsole_ctx_list_cleanup(ipmiconsole_ctx_t c);

int ipmiconsole_ctx_config_setup(ipmiconsole_ctx_t c,
                                 char *hostname,
                                 struct ipmiconsole_ipmi_config *ipmi_config,
                                 struct ipmiconsole_protocol_config *protocol_config,
                                 struct ipmiconsole_engine_config *engine_config);

void ipmiconsole_ctx_config_cleanup(ipmiconsole_ctx_t c);

int ipmiconsole_ctx_debug_setup(ipmiconsole_ctx_t c);

void ipmiconsole_ctx_debug_cleanup(ipmiconsole_ctx_t c);

int ipmiconsole_ctx_signal_setup(ipmiconsole_ctx_t c);

void ipmiconsole_ctx_signal_cleanup(ipmiconsole_ctx_t c);

int ipmiconsole_ctx_non_blocking_setup(ipmiconsole_ctx_t c,
                                       Ipmiconsole_callback callback,
                                       void *callback_arg);

int ipmiconsole_ctx_blocking_setup(ipmiconsole_ctx_t c);

void ipmiconsole_ctx_blocking_cleanup(ipmiconsole_ctx_t c);

int ipmiconsole_ctx_connection_setup(ipmiconsole_ctx_t c);

void ipmiconsole_ctx_connection_cleanup_session_submitted(ipmiconsole_ctx_t c);

void ipmiconsole_ctx_connection_cleanup_session_not_submitted(ipmiconsole_ctx_t c);

int ipmiconsole_ctx_session_setup(ipmiconsole_ctx_t c);

void ipmiconsole_ctx_fds_setup(ipmiconsole_ctx_t c);

void ipmiconsole_ctx_fds_cleanup(ipmiconsole_ctx_t c);

int ipmiconsole_ctx_get_errnum(ipmiconsole_ctx_t c);

void ipmiconsole_ctx_set_errnum(ipmiconsole_ctx_t c, int errnum);

#endif /* _IPMICONSOLE_CTX_H */
