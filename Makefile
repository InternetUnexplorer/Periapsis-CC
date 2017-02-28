# Makefile for the Periapsis C Compiler
#
# Copyright (C) 2017 Alex White <internetunexplorer@gmail.com>
#
# This software may be modified and distributed under the terms
# of the MIT license.  See the LICENSE file for details. 

CC      = cc
CFLAGS  = -Werror -ansi -pedantic

.PHONY: all clean

all: pcc

pcc: main.c 
	$(CC) $< -o $@ 

clean:
	rm -f pcc
