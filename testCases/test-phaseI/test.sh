#!/bin/bash

for f in *.tiger
do
    echo "Parsing ${f}"
    ../../bin/parser "${f}" -d > "${f}".txt 2>&1
done
