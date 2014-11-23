/*
 * Copyright (c) 2014 Jumpnow Technologies, LLC
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer. 2.
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 */

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/gpio.h>

#define GPIO_DEVICE "/dev/gpioc0"

void
usage(char *argv_0)
{
	printf("Usage: %s <pin> [<value>]\n", argv_0);
	printf("  Read or write a gpio pin.\n");
	printf("  pin		Pin number to read or write (0-191 for OMAP4)\n");
	printf("  value		Value to write, 0 or 1\n");
	exit(1);
}

int
main(int argc, char **argv)
{
	int 			fd;
	const char 		*errstr;
	struct gpio_req 	req;

	if (argc < 2 || argc > 3)
		usage(argv[0]);

	req.gp_pin = strtonum(argv[1], 0, 191, &errstr);

	if (errstr != NULL)
		errx(1, "Invalid pin number: %s", errstr);

	if ((fd = open(GPIO_DEVICE, O_RDWR)) < 0)
		err(1, NULL);

	if (argc == 3) {
		req.gp_value = strtonum(argv[2], 0, 1, &errstr);

		if (errstr != NULL) {
			printf("Can only write 0 or 1: %s\n", errstr);
			close(fd);
			usage(argv[0]);
		}

		if (ioctl(fd, GPIOSET, &req) < 0)
			perror("ioctl(GPIOSET)");
	}
	else {
		if (ioctl(fd, GPIOGET, &req) < 0)
			perror("ioctl(GPIOGET)");
		else
			printf("%d\n", req.gp_value);
	}

	close(fd);
	return 0;
}

