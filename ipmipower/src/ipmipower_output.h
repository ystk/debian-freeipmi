/*****************************************************************************\
 *  $Id: ipmipower_output.h,v 1.12 2008/05/17 16:12:36 chu11 Exp $
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

#ifndef _IPMIPOWER_OUTPUT_H
#define _IPMIPOWER_OUTPUT_H

#include "ipmipower.h"

/* ipmipower_output
 * - Output msg with hostname prefix to ttyout
 */
void ipmipower_output(msg_type_t num, const char *hostname); 

/* ipmipower_output_finish
 * - Output final results
 */
void ipmipower_output_finish(void);

#endif /* _IPMIPOWER_OUTPUT_H */
