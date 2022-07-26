#ifndef SPX_COMMON_H
#define SPX_COMMON_H

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/wait.h>
#include <strings.h>
#include <math.h>
#include <stddef.h>
#include <poll.h>
int snprintf(char *buf, size_t size, const char *fmt, ...);

#define FIFO_EXCHANGE "/tmp/spx_exchange_%d"
#define FIFO_TRADER "/tmp/spx_trader_%d"
#define FEE_PERCENTAGE 1

#endif