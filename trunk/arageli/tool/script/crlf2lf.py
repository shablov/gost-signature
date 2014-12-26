#! /usr/bin/env python

##############################################################################
#
#    crlf2lf.py
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

"Replace LF with CRLF in argument files.  Print names of changed files."

root_dir = "."

import sys
import re
import os

class TreeReplacer:

    def __init__(self):
        self.stats = {}

    def repargs(self, args):
        for arg in args:
#            print "Curr arg: %s" % arg
            if os.path.isdir(arg):
                self.repdir(arg)
            elif os.path.isfile(arg):
                self.repfile(arg)
            else:
                sys.stderr.write("Can't find %s\n" % file)

    def repdir(self, dir):
        try:
            names = os.listdir(dir)
#            print "%s is being processed." % dir
        except os.error, err:
            sys.stderr.write("Can't list %s: %s\n" % (file, err))
            return
        names.sort()
        for name in names:
            if name.startswith("tools"):
                continue # Skip tools directory. Rewrite this condition.
            if name.endswith(".vcproj"):
                continue # Skip project files
            if name.endswith(".sln"):
                continue # Skip solution files
            if name.startswith(".svn"):
                continue # Skip SVN temp files
            if name.endswith("~"):
                continue# Skip backup files
            full = os.path.join(dir, name)
            if os.path.isdir(full):
                self.repdir(full)
            else:
                self.repfile(full)

    def repfile(self, file):
        head, ext = os.path.splitext(file)
        head, base = os.path.split(file)
        try:
            data = open(file, "rb").read()
            if '\0' in data:
#                print file, "Binary!"
                return
            newdata = re.sub("\r\n", "\n", data)
            if newdata != data:
                print file
                f = open(file, "wb")
                f.write(newdata)
                f.close()

        except IOError, err:
            sys.stderr.write("Can't open %s: %s\n" % (file, err))
            return

def main():
    args = sys.argv[1:]
    if not args:
        args = [root_dir]
#    print "%s" % args
    s = TreeReplacer()
    s.repargs(args)

if __name__ == "__main__":
    main()


