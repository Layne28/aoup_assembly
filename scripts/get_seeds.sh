#!/bin/bash

Lx=$1
Ly=$2

phis=(0.00 0.20 0.40 0.60)
kTs=(0.0 0.1 0.3 0.5 1.0)
#D0s=(0.0000 0.0009 0.0100 0.0900 1.0000)
D0s=(1.0000 10.0000 30.0000 100.0000)
taus=(0.03 0.10 0.30 1.00 3.00 10.00 30.00)

for phi in "${phis[@]}"
do
    for kT in "${kTs[@]}"
    do
        for D0 in "${D0s[@]}"
        do
            for tau in "${taus[@]}"
            do
                filename="seeds/seeds_kT=${kT}_phi=${phi}_D0=${D0}_tau=${tau}_Lx=${Lx}_Ly=${Ly}.txt"
                touch $filename
                for i in {1..5}
                do
                    printf "$RANDOM\n" >> $filename
                done
            done
        done
    done
done
