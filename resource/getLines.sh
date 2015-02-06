#!/bin/sh
find . "(" -name "*.c" -or -name "*.cpp" -or -name "*.h"  ")" -print | xargs wc -l
