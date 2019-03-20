#!/bin/sh

# Must be launched from the project root directory!

C_RED="\e[1;91;7m"
C_GREEN="\e[1;92;7m"
C_NONE="\e[0m"

for i in $(find ./tests -name "*_test.txt" -type f | sort); do
    if ./aspic $i; then
        echo ${C_GREEN} PASS ${C_NONE} $i
    else
        echo ${C_RED} FAIL ${C_NONE} $i
        exit 1
    fi
done
