##############################################################################
#
#    Makefile
#
#    Makefile for GNU Make Utility to performance tests for
#    arageli library.
#
#    This file is a part of the Arageli library.
#
#    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
#    Copyright (C) 2006--2007 Aleksey Bader
#
#    The Arageli Library is free software; you can redistribute it and/or
#    modify it under the terms of the GNU General Public License version 2
#    as published by the Free Software Foundation.
#
#    The Arageli Library is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License along
#    with this program; if not, write to the Free Software Foundation, Inc.,
#    51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
#
#    We are also open for dual licensing for the whole library or
#    for its particular part. If you are interested to get the library
#    in this way, i.e. not under the GNU General Public License,
#    please contact Arageli Support Service support.arageli@gmail.com.
#
##############################################################################

BIN=../bin
ARAGELIPERFFLAGS += -pg -O3

$(BIN)/karatsuba_vs_classic:karatsuba_vs_classic.cpp perfarageli
	$(CXX) $(ARAGELIPERFFLAGS) $< -o $@ -I"../src" -L"../lib" -lperfarageli

perfarageli:
	$(MAKE) -C ../src/arageli ../../lib/libperfarageli.a

clean:
	rm -f $(BIN)/karatsuba_vs_classic
	$(MAKE) -C ../src/arageli perfclean

cleanall: clean
