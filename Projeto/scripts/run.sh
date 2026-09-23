#!/bin/sh
set -eu
benchmark="$(mktemp "${TMPDIR:-/tmp}/scapegoat-benchmark.XXXXXX")"
trap 'rm -f "$benchmark"' EXIT

gcc -O2 -std=c11 -Wall -Wextra -pedantic \
	src/main.c src/ScapegoatTree.c -lm -o "$benchmark"
"$benchmark" | tee results/resultados.csv