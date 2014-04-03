/*****************************************************************************\
 *  $Id: ierror.h,v 1.2 2008/05/18 15:41:35 chu11 Exp $
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
 *  The code in this file began with the code in the Powerman project.
 *  See below for original copyright information.
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
 *****************************************************************************
 *  Copyright (C) 2001-2002 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Andrew Uselton (uselton2@llnl.gov>
 *  UCRL-CODE-2002-008.
 *  
 *  This file is part of PowerMan, a remote power management program.
 *  For details, see <http://www.llnl.gov/linux/powerman/>.
 *  
 *  PowerMan is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *  
 *  PowerMan is distributed in the hope that it will be useful, but WITHOUT 
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
 *  for more details.
 *  
 *  You should have received a copy of the GNU General Public License along
 *  with PowerMan; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
\*****************************************************************************/

#ifndef IERROR_H
#define IERROR_H

#include <cbuf.h>
#include <stdio.h>
#include <stdarg.h>

/* achu
 * 
 * Modified to support multiple error outputting types
 */

/* Initialize error module with name of program.  Calls openlog().
 */
void ierr_init(char *prog);

/* Turn on error logging to sets of potential outputs
 */
void ierr_none();
void ierr_syslog(int toggle);
void ierr_file_stream(int toggle, FILE *stream);
void ierr_file_descriptor(int toggle, int fd);
void ierr_cbuf(int toggle, cbuf_t buf);
void ierr_cbuf_dump_file_stream(int toggle, FILE *stream);

/* Emit error message with a newline appended.  
 * If syslogging, use LOG_ERR level.
 */
void ierr_output(const char *fmt, ...);

/* Emit error message with a newline appended, then exit. If
 * syslogging, use LOG_ERR level.
 */
void ierr_exit(const char *fmt, ...);

/* Emit debug message with a newline appended.  If syslogging, use
 * LOG_DEBUG level.
 */
void ierr_dbg(const char *fmt, ...);

/* Hooks for cbuf.c and list.c error handling.
 */
void lsd_fatal_error(char *file, int line, char *mesg);
void *lsd_nomem_error(char *file, int line, char *mesg);

void cbuf_printf(cbuf_t cbuf, const char *fmt, ...);

#endif /* IERROR_H */

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
