#!/bin/bash

# clean and recreate src folder
rm -rf src
mkdir -p src

# set up source file
cat << EOF > src/test.c
#include <stdio.h>
int main()
{
    printf("hi!\n");
    return 0;
}
EOF

# set up configure.ac
cat << EOF > src/configure.ac
AC_INIT([test], [1.0], [lam2mo@jmu.edu])
AM_INIT_AUTOMAKE([-Wall])
AC_PROG_CC
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
EOF

# set up Makefile.am
cat << EOF > src/Makefile.am
bin_PROGRAMS=test
test_SOURCES=test.c
EOF

# files that are apparently necessary for automake
touch src/NEWS src/README src/AUTHORS src/ChangeLog

# run minimal autoconf sequence or swap w/ commented-out line to use autoreconf
(cd src && aclocal && autoconf && automake --add-missing)
#(cd src && autoreconf --install)
