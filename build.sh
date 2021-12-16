#!/bin/bash

output=${@:-patcher}

gcc -std=c99 -pedantic -Wextra -O3 -o "$output" source/linux/build.c
