********************************************************************

                The Arageli Library Readme File
                   Version 2.2.9.412 Prealpha

********************************************************************

Arageli is a C++ library and a package of programs for
computations in arithmetic, algebra, geometry, linear and integer
linear programming. Arageli is a library for dealing with precise,
i.e. symbolic or algebraic, computations. It contains a definition
to model basic algebraic structures such as integer numbers with
arbitrary precision, rational numbers, vectors, matrices,
polynomials etc. Arageli is written in C++ and use power and
expressiveness of the language.

The home page: http://www.arageli.org

The Arageli Library is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License version 2
as published by the Free Software Foundation.

The Arageli Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.

We are also open for dual licensing for the whole library or
for its particular part. If you are interested to get the library
in this way, i.e. not under the GNU General Public License,
please contact Arageli Support Service support.arageli@gmail.com.

********************************************************************

Table of Content

    1. Software Requirements
        1.1 To build the library
        1.2 To build all guides distributed as LaTeX sources
        1.3 To build The Reference Guide
    2. Building
        2.1 Microsoft Windows
        2.2 Linux
    3. How to use
    4. Additional Documentation and Feedback
    5. Known issues/bugs
    6. Contributors

********************************************************************

1. Software Requirements

To build the library and prepare documentation you need several
applications installed. Without some of them you will not
able to create certain parts of the documentation or the library
itself. Please note that depending on distribution type, some
parts of the package may already be prebuilt.

This section describes different sets of tools that are
necessary for building particular parts of Arageli package.


1.1 To build the library

You need one of the C++ 1998 standard (or later) compilers. We
have checked correct compilation with:

    - GCC 4.1.3 20070929 (prerelease) (Ubuntu 4.1.2-16ubuntu2)
      (64-bit),
    - GCC 4.4.3 (Ubuntu 4.4.3-4ubuntu5) (32-bit),
    - Microsoft (R) 32-bit C/C++ Optimizing Compiler Version 15
      for 80x86 (included in Visual Studio 2008),
    - Microsoft (R) C/C++ Optimizing Compiler Version 15 for x64
      (included in Visual Studio 2008),
    - Microsoft (R) 32-bit C/C++ Optimizing Compiler Version
      16.00.30319.01 for 80x86 (included in Visual Studio 2010),
    - Microsoft (R) C/C++ Optimizing Compiler Version 16.00.30319.01
      for x64 (included in Visual Studio 2010)


Please let us know if you are face troubles with those compilers or
you have success or failure to build with other C++ compilers/
platforms to include or exclude them to/from the list.


1.2 To build all guides distributed as LaTeX sources

You need a LaTeX installation. We have checked the correct building
with MiKTeX 2.8.3553.


1.3 To build The Reference Guide

You need doxygen installation. Without this you cannot extract
documented items from the library sources. We have checked the
correct building with Windows doxygen 1.7.0 bits.


To create all parts of the distributive you should have all
programs referenced at 1.1--1.3. If you plan to contribute into
the library, we recommend you to have all these programs and
additionally Subversion client installation to be able to commit
changes to the repository.

If you need some part of the library compiled (both library or
documentation), you can download it at the home page of Arageli
http://www.arageli.org.

********************************************************************

2. Building

Almost all build scripts are located in build directory of
the distributive but some of them are spread along the directories.


2.1 Microsoft Windows

If you use Microsoft Visual Studio 2008, you can build
solution that located at build/msvs_2008/arageli_all.sln.

To build the documentation, go to build directory and run one of
the following batch files:

    - create_guide.bat - to build all guides except Reference Guide;
    - create_ref.bat   - to build The Reference Guide only;
    - create_doc.bat   - to build all available documentation.


2.2 Linux

Please note that Linux platforms have worse support because we
do not have enough resources to provide full-featured build scripts.
Particularly if you need to build documentation, you have to do it
manually -- no scripts provided. Sorry for inconvenience.

So if you are in some Linux system, you can build the library,
test system and test set by the make utility. To make only
Arageli library go to library root directory and type:

    make

After that, libarageli.a file appears in the lib directory
of the package. To make test set and run them, type:

    make check

After that, test executive file appears in the bin directory
of the package. Note, while executing the last command
Arageli and ts systems have being built. To make tests and
run them, type:

    make runtests

After that, test.log file appears in the status directory
of the package.

You can use CXXFLAGS, ARAGELICXXFLAGS and TESTSCXXFLAGS
variables to set specific command line parameters to tune
compilation of different parts as usual.

********************************************************************

3. How to use

To use the library in your application you have to include
one or several necessary header files from ./src/arageli
directory. If you do not know which files you need to use
a particular feature of Arageli, you can just include
./src/arageli/arageli.hpp that includes all header files of
the library.

If you use Arageli in an executable application (not a static
library) or in a dynamic library, you have to link your
target binary with Arageli static library placed in ./lib
directory. There are several Arageli static libraries;
see parameters of the build script to choose right version of
static library file to link with your application/library.

********************************************************************

4. Additional Documentation and Feedback

Some ready-to-use documentation files already present in doc
directory of the distributive. Go to http://www.arageli.org
for updates and news. Mail questions, comments and suggestions on
support.arageli@gmail.com.

If you want to work directly with resources at http://sourceforge.net
please report bugs in group "Release 2.2.9" at the project page.
Also please create feature requests if you think that a particular
feature should be in the library. If you don't want to deal with
sourceforge.net directly, please send all issues you found to
support.arageli@gmail.com.

********************************************************************

5. Known issues/bugs

Here is a list of documented bugs that is related to this release
and which you can monitor at http://sourceforge.net. All these
bugs can be found in special group "Release 2.2.9".

    #3018788 -- A lot of warning coming from big_int::from_native_int
    #2870186 -- Arageli::vector uses old iterators after unique call
    #1893627 -- The same output file names for executables

Besides these bugs please note that different components in this
release do not have consistent and solid interface because they all
are in different stages of development. Several functions have
warning records (find WARNING! in sources) about various aspects of
their implementation. Some functions marked as incorrect (you will
permanently have a run-time error if call them). A lot of algorithm
implementations is naive, so do not expect superb performance from
them.

********************************************************************

6. Contributors

    - Nikolai Yu. Zolotykh
    - Agafonov E.A.
    - Max A. Alekseyev
    - Aleksey Bader
    - Anna Bestaeva
    - Sergey V. Lobanov
    - Sergey S. Lyalin
    - Alexander Pshenichnikov
    - Alexey Polovinkin
    - Nikolay Santalov
    - Ekaterina Shchukina
    - Andrey Somsikov
