/*****************************************************************************\
 *  $Id: ipmipower_wrappers.h,v 1.28 2008/05/19 23:27:51 chu11 Exp $
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

#ifndef _IPMIPOWER_WRAPPERS_H
#define _IPMIPOWER_WRAPPERS_H

#include "argv.h"
#include "ierror.h"
#include "fd.h"
#include "list.h"
#include "cbuf.h"
#include "hostlist.h"
#include "secure.h"
#include "timeval.h"
#include "wrappers.h"
#include "conffile.h"

#include <freeipmi/freeipmi.h>

/* Cbuf_create
 * - Create cbuf buffer
 * - Automaticaly sets CBUF_OPT_OVERWRITE to CBUF_WRAP_MANY
 */
cbuf_t Cbuf_create(int minsize, int maxsize);

/* Cbuf_drop_all
 * - drop all data from buffer
 */
void Cbuf_drop_all(cbuf_t buf); 

/* Cbuf_read_to_fd
 * - move data from buf to fd
 */
void Cbuf_read_to_fd(cbuf_t buf, int fd);

/* Cbuf_write_from_fd
 * - move data from fd to buf
 */
void Cbuf_write_from_fd(cbuf_t buf, int fd);

/* Cbuf_write
 * - wrapper for cbuf_write, with various error checks
 */
void Cbuf_write(cbuf_t buf, void *buffer, int len);

/* Cbuf_peek_and_drop
 * - wrapper for cbuf_peek and cbuf_drop, with various error checks
 * - will drop remaining data in cbuf if buffer not large enough
 * Returns length of packet received, 0 if no packet seen
 */
int Cbuf_peek_and_drop(cbuf_t buf, void *buffer, int len);

/* Cbuf_peek_to_fd
 * - wrapper for cbuf_peek_to_fd
 */
int Cbuf_peek_to_fd(cbuf_t src, int dstfd, int len);

/* Fiid_obj_create
 * - Allocate a fiid object
 */
fiid_obj_t Fiid_obj_create(fiid_template_t tmpl);

/* Fiid_obj_destroy
 * - Destroy a fiid object
 */
void Fiid_obj_destroy(fiid_obj_t obj);

/* Fiid_obj_clear
 * - Clear a fiid object 
 */
void Fiid_obj_clear(fiid_obj_t obj);

/* Fiid_obj_clear_field
 * - Clear a fiid field
 */
void Fiid_obj_clear_field(fiid_obj_t obj, char *field);

/* Fiid_obj_get
 * - Get fiid field data
 */
void Fiid_obj_get(fiid_obj_t obj, char *field, uint64_t *val);

/* Fiid_obj_get_data
 * - Get fiid field data
 */
int32_t Fiid_obj_get_data(fiid_obj_t obj, char *field, uint8_t *data, uint32_t data_len);

/* Fiid_obj_set_data
 * - Set all object field data
 */
int32_t Fiid_obj_set_data(fiid_obj_t obj, char *field, uint8_t *data, uint32_t data_len);

/* Fiid_obj_set_all
 * - Set all object data
 */
int32_t Fiid_obj_set_all(fiid_obj_t obj, uint8_t *data, uint32_t data_len);

#endif /* _IPMIPOWER_WRAPPERS_H */
