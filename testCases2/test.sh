#!/bin/bash
set -xe

if [ ! -n "$1" ]; then
  parser=../build/src/parser
else
  parser=$1
fi

for f in *.tiger
do
    echo "Parsing ${f}"
    #1. cfg optimization technique
    "${parser}" "${f}" -d > "${f}".cfg.s 2>&1
    #2. naive optimization technique
    "${parser}" "${f}" -d -naive > "${f}".naive.s 2>&1
done
