#CC=gcc
CC=/opt/trendchip/mipsel-linux-uclibc-4.6.3-kernel3.18/usr/bin/mipsel-buildroot-linux-uclibc-gcc
MAIN=du
OBJS = backtraceFuntion.o du_malloc_mem.o test_library_du.o 
CFLAGS= -Wall -g -ldl -rdynamic -funwind-tables -pthread
LFLAGS = -L/opt/trendchip/mipsel-linux-uclibc-4.6.3-kernel3.18/usr/lib/ 
# LIBS = -lm -lmpcdu_malloc_mem.o
INCLUDES= -I./
#pragma once
$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

clean:
	rm -rf $(OBJS)  $(MAIN)
