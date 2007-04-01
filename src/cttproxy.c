/*
 * Functions for managing transparent proxying with CTTPROXY.
 * This file should be compiled only if CTTPROXY is enabled.
 *
 * Copyright 2000-2007 Willy Tarreau <w@1wt.eu>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <common/compat.h>
#include <common/config.h>
#include <common/time.h>

#include <types/global.h>

#include <import/ip_tproxy.h>

/*
 * Checks that CTTPROXY is available and in the right version.
 * Returns 0 if OK, -1 if wrong version, -2 if not available or other error.
 */
int check_cttproxy_version() {
	struct in_tproxy itp1;
	int fd;

	memset(&itp1, 0, sizeof(itp1));
		
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
		return -2;

	itp1.op = TPROXY_VERSION;
	itp1.v.version = 0x02000000; /* CTTPROXY version 2.0 expected */
	
	if (setsockopt(fd, SOL_IP, IP_TPROXY, &itp1, sizeof(itp1)) == -1) {
		if (errno == -EINVAL)
			return -1; /* wrong version */
		else
			return -2; /* not supported or other error */
	}
	return 0;
}


/*
 * Local variables:
 *  c-indent-level: 8
 *  c-basic-offset: 8
 * End:
 */