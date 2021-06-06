#!/bin/bash

output=${@:-patcher}

gcc -std=c99 -pedantic -Wextra -O3 -DBUILD_LINUX=1 -o "$output" source/linux/build.c
