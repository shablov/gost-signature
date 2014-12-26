##############################################################################
#
#    Makefile
#
#    This makefile builds examples of arageli using.
#
#    This file is a part of the Arageli library.
#
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
INCLUDE_DIR=../src
LIBRARY_DIR=../lib

all: \
     $(LIBRARY_DIR)/libarageli.a\
	 ../src/arageli/arageli.hpp.gch\
     $(BIN)/adv_simplex_method\
     $(BIN)/big_int_basic\
     $(BIN)/big_int_rational_creation\
     $(BIN)/big_int_rational_functions\
     $(BIN)/big_int_rational_operations\
     $(BIN)/factorial\
     $(BIN)/gram_schmidt\
     $(BIN)/knuth_supergenerator\
     $(BIN)/lll_reduction\
     $(BIN)/lll_reduction_int\
     $(BIN)/matrix_basic\
     $(BIN)/matrix_column_row\
     $(BIN)/matrix_gauss\
     $(BIN)/matrix_linear_system\
     $(BIN)/matrix_rref\
     $(BIN)/matrix_smith\
     $(BIN)/polynom_basic\
     $(BIN)/residue\
     $(BIN)/residue_linear_system\
     $(BIN)/sparse_polynom_algorithms\
     $(BIN)/sparse_polynom_basic\
     $(BIN)/sparse_polynom_creation\
     $(BIN)/sparse_polynom_io\
     $(BIN)/sparse_polynom_iterator\
     $(BIN)/sparse_polynom_kroneker\
     $(BIN)/sparse_polynom_lagrange\
     $(BIN)/sparse_polynom_matrix\
     $(BIN)/sparse_polynom_matrix_smith\
     $(BIN)/sparse_polynom_operations\
     $(BIN)/sparse_polynom_other_operations\
     $(BIN)/sparse_polynom_properties\
     $(BIN)/vector_entries\
     $(BIN)/vector_lcm_gcd\
     $(BIN)/vector_matrix_arithmetic\
     $(BIN)/vector_matrix_creation\
     $(BIN)/vector_matrix_io\

$(LIBRARY_DIR)/libarageli.a:
	$(MAKE) -C ../src/arageli

../src/arageli/arageli.hpp.gch:../src/arageli/arageli.hpp
	$(MAKE) -C ../src/arageli arageli.hpp.gch

$(BIN)/adv_simplex_method: adv_simplex_method.cpp adv_simplex_method.task.txt ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" adv_simplex_method.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/big_int_basic: big_int_basic.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" big_int_basic.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/big_int_rational_creation: big_int_rational_creation.cpp big_int_rational_creation.cpp.in.txt ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" big_int_rational_creation.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/big_int_rational_functions: big_int_rational_functions.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" big_int_rational_functions.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/big_int_rational_operations: big_int_rational_operations.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" big_int_rational_operations.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/factorial: factorial.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" factorial.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/gram_schmidt: gram_schmidt.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" gram_schmidt.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/knuth_supergenerator: knuth_supergenerator.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" knuth_supergenerator.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/lll_reduction: lll_reduction.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" lll_reduction.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/lll_reduction_int: lll_reduction_int.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" lll_reduction_int.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/matrix_basic: matrix_basic.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" matrix_basic.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/matrix_column_row: matrix_column_row.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" matrix_column_row.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/matrix_gauss: matrix_gauss.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" matrix_gauss.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/matrix_linear_system: matrix_linear_system.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" matrix_linear_system.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/matrix_rref: matrix_rref.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" matrix_rref.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/matrix_smith: matrix_smith.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" matrix_smith.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/polynom_basic: polynom_basic.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" polynom_basic.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/residue: residue.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" residue.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/residue_linear_system: residue_linear_system.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" residue_linear_system.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/sparse_polynom_algorithms: sparse_polynom_algorithms.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" sparse_polynom_algorithms.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/sparse_polynom_basic: sparse_polynom_basic.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" sparse_polynom_basic.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/sparse_polynom_creation: sparse_polynom_creation.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" sparse_polynom_creation.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/sparse_polynom_io: sparse_polynom_io.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" sparse_polynom_io.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/sparse_polynom_iterator: sparse_polynom_iterator.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" sparse_polynom_iterator.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/sparse_polynom_kroneker: sparse_polynom_kroneker.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" sparse_polynom_kroneker.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/sparse_polynom_lagrange: sparse_polynom_lagrange.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" sparse_polynom_lagrange.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/sparse_polynom_matrix: sparse_polynom_matrix.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" sparse_polynom_matrix.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/sparse_polynom_matrix_smith: sparse_polynom_matrix_smith.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" sparse_polynom_matrix_smith.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/sparse_polynom_operations: sparse_polynom_operations.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" sparse_polynom_operations.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/sparse_polynom_other_operations: sparse_polynom_other_operations.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" sparse_polynom_other_operations.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/sparse_polynom_properties: sparse_polynom_properties.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" sparse_polynom_properties.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/vector_entries: vector_entries.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" vector_entries.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/vector_lcm_gcd: vector_lcm_gcd.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" vector_lcm_gcd.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/vector_matrix_arithmetic: vector_matrix_arithmetic.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" vector_matrix_arithmetic.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/vector_matrix_creation: vector_matrix_creation.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" vector_matrix_creation.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

$(BIN)/vector_matrix_io: vector_matrix_io.cpp ../src/arageli/arageli.hpp.gch $(LIBRARY_DIR)/libarageli.a
	$(CXX) -I"$(INCLUDE_DIR)" vector_matrix_io.cpp -o$@ -L"$(LIBRARY_DIR)" -larageli

clean:
	rm -f \
     $(BIN)/adv_simplex_method\
     $(BIN)/big_int_basic\
     $(BIN)/big_int_rational_creation\
     $(BIN)/big_int_rational_functions\
     $(BIN)/big_int_rational_operations\
     $(BIN)/factorial\
     $(BIN)/gram_schmidt\
     $(BIN)/knuth_supergenerator\
     $(BIN)/lll_reduction\
     $(BIN)/lll_reduction_int\
     $(BIN)/matrix_basic\
     $(BIN)/matrix_column_row\
     $(BIN)/matrix_gauss\
     $(BIN)/matrix_linear_system\
     $(BIN)/matrix_rref\
     $(BIN)/matrix_smith\
     $(BIN)/polynom_basic\
     $(BIN)/residue\
     $(BIN)/residue_linear_system\
     $(BIN)/sparse_polynom_algorithms\
     $(BIN)/sparse_polynom_basic\
     $(BIN)/sparse_polynom_creation\
     $(BIN)/sparse_polynom_io\
     $(BIN)/sparse_polynom_iterator\
     $(BIN)/sparse_polynom_kroneker\
     $(BIN)/sparse_polynom_lagrange\
     $(BIN)/sparse_polynom_matrix\
     $(BIN)/sparse_polynom_matrix_smith\
     $(BIN)/sparse_polynom_operations\
     $(BIN)/sparse_polynom_other_operations\
     $(BIN)/sparse_polynom_properties\
     $(BIN)/vector_entries\
     $(BIN)/vector_lcm_gcd\
     $(BIN)/vector_matrix_arithmetic\
     $(BIN)/vector_matrix_creation\
     $(BIN)/vector_matrix_io\
	
cleanall: clean
	rm -f ../src/arageli/arageli.hpp.gch
