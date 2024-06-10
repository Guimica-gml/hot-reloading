#!/usr/bin/env sh
set -e

CFLAGS="-Wall -Wextra -pedantic -std=c11"
CLIBS="-lraylib -ldl"

mkdir -p ./build

gcc $CFLAGS -shared -o ./build/app_temp.so ./app.c
mv ./build/app_temp.so ./build/app.so

gcc $CFLAGS -o ./build/main ./main.c $CLIBS

if [ "$1" = "run" ]
then
    shift
    ./build/main "$@"
fi
