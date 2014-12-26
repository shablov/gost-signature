##############################################################################
#
#    Makefile
#
#    Makefile for GNU Make Utility to build the library
#    documentation.
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

dvi_dir = ../dvi
dvi_from_build = ../doc/dvi
source_files = arageli_overview_announce_preprint

all: dvi

dvi: $(addprefix $(dvi_dir)/, $(addsuffix .dvi, $(source_files)))

$(dvi_dir)/%.dvi: %.tex
	cd ../../build && latex -output-directory $(dvi_from_build) ../doc/src/$(source_files)
	cd ../../build && latex -output-directory $(dvi_from_build) ../doc/src/$(source_files)

cleanall:
	rm -f $(addprefix $(dvi_dir)/, $(addsuffix .*, $(source_files)))
