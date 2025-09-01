#!/usr/bin/env bash

set -e

make sunvec
cd run/sunvec
../../bin/sunvec naif0012.tls de440.bsp sunvecs.vec