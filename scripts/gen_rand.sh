#!/bin/bash

for i in `seq 25 25`; do
for j in `seq 1 50`; do
../ganitazero.git/build/gp $i > ganita_rand.$i.$j.bin
done
done
