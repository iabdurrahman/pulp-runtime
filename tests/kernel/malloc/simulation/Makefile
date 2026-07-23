# Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT.

APP = test

C_OBJS = build/test.o build/alloc.o

RM ?= rm -f

MKDIR ?= mkdir -p
RMDIR ?= rmdir

.PHONY: all

all: build/$(APP)

build/$(APP): $(C_OBJS)
	$(CC) -o $@ $^

build/test.o: ../test.c build
	$(CC) -g -std=c11 -Wall -Wextra -pedantic -I. -c -o $@ $<

build/alloc.o: alloc.c build
	$(CC) -g -std=c11 -Wall -Wextra -pedantic -I. -c -o $@ $<

build:
	$(MKDIR) $@

clean:
	$(RM)     build/$(APP) $(C_OBJS)
	$(RMDIR)  build
