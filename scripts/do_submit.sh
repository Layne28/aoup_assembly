#!/bin/bash

Lx=$1
Ly=$2
D0=$3
nseed=$4

taus=(0.03 0.10 0.30 1.00 3.00 10.00)
phis=(0.20 0.40 0.60)
kTs=(0.0 0.1 0.3 0.5 1.0)
D0s=(${D0})
#D0s=(0.0000 0.0009 0.0010 0.0090 1.0000)

for phi in "${phis[@]}"
do
    for kT in "${kTs[@]}"
    do
        for D0 in "${D0s[@]}"
        do
	    for tau in "${taus[@]}"
            do
                input="seeds/seeds_kT=${kT}_phi=${phi}_D0=${D0}_tau=${tau}_Lx=${Lx}_Ly=${Ly}.txt"
                for (( i=1; i<=$nseed; i++ ))
                do
                    echo $i
                    sbatch -J "aoup_assembly_kT=${kT}_phi=${phi}_D0=${D0}_tau=${tau}_Lx=${Lx}_Ly=${Ly}_seed=${i}" -o "log/aoup_assembly_kT=${kT}_phi=${phi}_D0=${D0}_tau=${tau}_Lx=${Lx}_Ly=${Ly}_seed=${i}.o%j" -e "log/aoup_assembly_kT=${kT}_phi=${phi}_D0=${D0}_tau=${tau}_Lx=${Lx}_Ly=${Ly}_seed=${i}.e%j" scripts/submit_sims.sh "/home/laynefrechette/aoup_assembly/input_files/aoup_assembly_kT=${kT}_phi=${phi}_D0=${D0}_tau=${tau}_Lx=${Lx}_Ly=${Ly}.in" $i $input
                done
            done
        done
    done
done
