/*****************************************************************************\
 *  $Id: ipmiconsole_packet.h,v 1.6 2008/03/28 00:14:40 chu11 Exp $
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

#ifndef _IPMICONSOLE_PACKET_H
#define _IPMICONSOLE_PACKET_H

#include <freeipmi/freeipmi.h>

#include "ipmiconsole.h"
#include "ipmiconsole_defs.h"

fiid_field_t *ipmiconsole_packet_template(ipmiconsole_ctx_t c,
					  ipmiconsole_packet_type_t p);

fiid_obj_t ipmiconsole_packet_object(ipmiconsole_ctx_t c,
				     ipmiconsole_packet_type_t p);

int ipmiconsole_packet_dump(ipmiconsole_ctx_t c,
			    ipmiconsole_packet_type_t p,
			    uint8_t *buf,
			    uint32_t buflen);

int ipmiconsole_packet_dump_unknown(ipmiconsole_ctx_t c,
				    uint8_t *buf,
				    uint32_t buflen);

int ipmiconsole_ipmi_packet_assemble(ipmiconsole_ctx_t c, 
				     ipmiconsole_packet_type_t p,
				     uint8_t *buf,
				     uint32_t buflen);

int ipmiconsole_sol_packet_assemble(ipmiconsole_ctx_t c,
				    uint8_t packet_sequence_number,
				    uint8_t packet_ack_nack_sequence_number,
				    uint8_t accepted_character_count,
				    uint8_t generate_break,
				    uint8_t *character_data,
				    uint32_t character_data_len,
				    uint8_t *buf,
				    uint32_t buflen);

int ipmiconsole_packet_unassemble(ipmiconsole_ctx_t c, 
				  ipmiconsole_packet_type_t *p,
				  uint8_t *buf,
				  uint32_t buflen);

int ipmiconsole_calculate_errnum(ipmiconsole_ctx_t c, 
				 ipmiconsole_packet_type_t p);

#endif /* _IPMICONSOLE_PACKET_H */
