##############################################################################
#
#    Makefile
#
#    Makefile for GNU Make Utility to build tests for arageli
#    library.
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

test_dirs = . ITLab-produced
CXXFLAGS += -MD -g -DARAGELI_SOURCE -O2 -DARAGELI_DEBUG_LEVEL=3 -DARAGELI_ASSERT_THROW_EXCEPTION
tests_name = test
LINKFLAGS = -L"../lib" -L"../tool/ts/bin/" -ltestarageli -lts

all: ../bin/$(tests_name)

../bin/$(tests_name): ../tool/ts/bin/libts.a ../lib/libtestarageli.a stdafx.hpp.gch $(patsubst %.cpp,%.o, $(wildcard $(addsuffix \
                /*.cpp,$(test_dirs))))
	make -C ../src/arageli ../../lib/libtestarageli.a
	$(CXX) -o ../bin/$(tests_name) $(patsubst %.cpp,%.o, $(wildcard \
    $(addsuffix /*.cpp,$(test_dirs)))) $(LINKFLAGS)

../tool/ts/bin/libts.a:
	$(MAKE) -C ../tool/ts/src/ts/

stdafx.hpp.gch: stdafx.hpp
	$(CC) -x c++-header $(CXXFLAGS) -o $@ $< -I"../src/" -I"../tool/ts/src/"

%.o:%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ -I"../src/" -I"../tool/ts/src/"

../lib/libtestarageli.a:
	$(MAKE) -C ../src/arageli ../../lib/libtestarageli.a

TAGS:
	ctags $(wildcard $(addsuffix /*.cpp,$(test_dirs))) $(wildcard \
	$(addsuffix /*.hpp,$(test_dirs))) #-f TAGS

clean:
	rm -f ../bin/$(tests_name)
	rm -f $(wildcard $(addsuffix /*.o,$(test_dirs))) $(wildcard $(addsuffix /*.d,$(test_dirs)))

cleanall: clean
	rm -f stdafx.hpp.gch
	rm -f tags

include $(wildcard *.d)
