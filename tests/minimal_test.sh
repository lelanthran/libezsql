#!/bin/bash

if [ -z "$TARGET" ]; then
   export TARGET=debug
fi

if [ ! -z "$VGOPTS" ]; then
   export VALGRIND="valgrind --leak-check=yes --show-leak-kinds=all $VGOPTS"
fi

export PLUGINS=all
make $TARGET

$VALGRIND ./src/debug/bin/x86_64-linux-gnu/ezsql_test.elf plugins/dummy_plugin/debug/lib/libezsql_dummy.so ""
