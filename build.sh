#!/bin/sh

set -xe

CC="clang"
CFLAGS="-O2 -Wall -Wextra -std=c17"
LFLAGS=""
EXE="game_of_life"
SRCDIR="./src"
INCLUDEDIR="./include"
BUILDDIR="./build"

$CC -I$INCLUDEDIR $CFLAGS $SRCDIR/*.c -o $BUILDDIR/$EXE $LFLAGS
