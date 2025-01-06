#!/bin/bash

cd /build

aarch64-linux-gnu-gcc -Wall -o Artifacts/amt/bin/am031 Programmes/am031.c
aarch64-linux-gnu-gcc -Wall -o Artifacts/amt/bin/lance_am8 Programmes/lance_am8.c
aarch64-linux-gnu-gcc -Wall -o Artifacts/amt/bin/maj_am8 Programmes/maj_am8.c

chown 1000:1000 Artifacts/*
