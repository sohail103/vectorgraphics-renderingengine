#!/usr/bin/env bash
set -euo pipefail
cc -O2 -o tools/nanosvg_cli tools/nanosvg_cli.c -lm
echo "Built tools/nanosvg_cli"
