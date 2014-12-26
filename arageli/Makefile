##############################################################################
#
#    Makefile
#
#    Makefile for GNU Make Utility to build the library and
#    other programs from the Arageli Package and run some test and
#    examples.
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

# build arageli library

.PHONY: arageli all check install clean example dvi TAGS perf script HTML

arageli:
	$(MAKE) -C src/arageli

all: arageli check example dvi TAGS perf script HTML
	

check:
	$(MAKE) -C tool/ts/src/ts/
	$(MAKE) -C test
	make runtests

perf:
	$(MAKE) -C perf

runtests:
	if ls bin/test; then bin/test; else echo \*\*\* Try make check before running tests! \*\*\*; fi;

example:
	$(MAKE) -C example/

# call 'make install' only with root previlegues
install:

# run script to satisfy Coding Guidelines
script:
	./tool/script/tab_finding.py ./
	./tool/script/crlf_finding.py ./
#	./tool/script/end_spaces_del.py ./
#	./tool/script/crlf2lf.py ./
#	./tool/script/tab_replacing.py ./

# build documentation for arageli library
dvi:
	mkdir -p doc/dvi
	$(MAKE) -C doc/src

HTML:
	$(MAKE) -C src/arageli HTML

HTMLclean:
	$(MAKE) -C src/arageli HTMLclean

# update TAGS for source files
TAGS:
	ctags -R -h .cpp.hpp

clean:
	$(MAKE) -C src/arageli clean

cleanall: clean HTMLclean
	$(MAKE) -C tool/ts/src/ts cleanall
	$(MAKE) -C test cleanall
	$(MAKE) -C doc/src cleanall
	$(MAKE) -C example cleanall
	$(MAKE) -C perf cleanall
	rm -rf doc/dvi
	rm -f tags
	rm -f io_simple_binary.hpp.tmp #TODO: resolve problem with this file!!!
