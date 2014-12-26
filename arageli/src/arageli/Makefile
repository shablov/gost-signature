##############################################################################
#
#    Makefile
#
#    Makefile for GNU Make Utility to build the Arageli library.
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

FASTCXXFLAGS += -MD -DARAGELI_SOURCE -O3 -DARAGELI_DEBUG_LEVEL=0 -DARAGELI_ASSERT_THROW_EXCEPTION
ARAGELICXXFLAGS += -MD -DARAGELI_SOURCE -O2 -DARAGELI_DEBUG_LEVEL=1
TESTSCXXFLAGS += -MD -DARAGELI_SOURCE -g -DARAGELI_DEBUG_LEVEL=3 -DARAGELI_ASSERT_THROW_EXCEPTION
PERFCXXFLAGS += -MD -pg -DARAGELI_SOURCE -g -O3 -DARAGELI_DEBUG_LEVEL=0 -DARAGELI_ASSERT_THROW_EXCEPTION
obj_dirs = . hermite smith

all: ../../lib/libarageli.a

../../lib/libarageli.a: $(patsubst %.cpp,%.o, $(wildcard $(addsuffix /*.cpp,$(obj_dirs))))
	$(AR) -r $@ $(patsubst %.cpp,%.o, $(wildcard $(addsuffix /*.cpp,$(obj_dirs))))

../../lib/libtestarageli.a: $(patsubst %.cpp,%t.o, $(wildcard $(addsuffix /*.cpp,$(obj_dirs))))
	$(AR) -r $@ $(patsubst %.cpp,%t.o, $(wildcard $(addsuffix /*.cpp,$(obj_dirs))))

../../lib/libperfarageli.a: $(patsubst %.cpp,%p.o, $(wildcard $(addsuffix /*.cpp,$(obj_dirs))))
	$(AR) -r $@ $(patsubst %.cpp,%p.o, $(wildcard $(addsuffix /*.cpp,$(obj_dirs))))

../../lib/libfastarageli.a: $(patsubst %.cpp,%f.o, $(wildcard $(addsuffix /*.cpp,$(obj_dirs))))
	$(AR) -r $@ $(patsubst %.cpp,%f.o, $(wildcard $(addsuffix /*.cpp,$(obj_dirs))))

arageli.hpp.gch:arageli.hpp
	$(CC) -x c++-header $(ARAGELICXXFLAGS) $< -o $@

%.o:%.cpp
	$(CXX) -c $(ARAGELICXXFLAGS) $< -o $@

%t.o:%.cpp
	$(CXX) -c $(TESTSCXXFLAGS) $< -o $@

%p.o:%.cpp
	$(CXX) -c $(PERFCXXFLAGS) $< -o $@

%f.o:%.cpp
	$(CXX) -c $(FASTCXXFLAGS) $< -o $@

TAGS:
	ctags $(wildcard $(addsuffix /*.cpp,$(obj_dirs))) $(wildcard $(addsuffix /*.hpp,$(obj_dirs)))

fastclean:
	rm -f $(wildcard $(addsuffix /*f.o,$(obj_dirs))) $(wildcard $(addsuffix /*.d,$(obj_dirs)))
	rm -f ../../lib/libfastarageli.a

perfclean:
	rm -f $(wildcard $(addsuffix /*p.o,$(obj_dirs))) $(wildcard $(addsuffix /*.d,$(obj_dirs)))
	rm -f ../../lib/libperfarageli.a

testclean:
	rm -f $(wildcard $(addsuffix /*t.o,$(obj_dirs))) $(wildcard $(addsuffix /*.d,$(obj_dirs)))
	rm -f ../../lib/libtestarageli.a

clean:
	rm -f ../../lib/libarageli.a
	rm -f $(wildcard $(addsuffix /*.o,$(obj_dirs))) $(wildcard $(addsuffix /*.d,$(obj_dirs)))

cleanall: clean perfclean HTMLclean testclean fastclean
	rm -f arageli.hpp.gch
	rm -f tags

include $(wildcard *.d)

HTML:
	doxygen Doxyfile

HTMLclean:
	rm -rf ../../doc/reference_manual_html
