#!/bin/bash

Lx=$1
Ly=$2
potential_type=$3 #lj or wca
base_dir=$4 #work or scratch0

dt=0.00025
freq=800
equil_steps=40000
production_steps=4000000
#phis=(0.20 0.40 0.60)
#kTs=(0.0 0.1 0.3 0.5 1.0)
phis=(0.40)
kTs=(0.0)
#D0s=(1.0000 10.0000)
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
                python3 scripts/write_input_file.py input_files --D0 $D0 --tau $tau --kT $kT --phi $phi --Lx $Lx --Ly $Ly --particles_freq $freq --potential_type $potential_type --thermo_freq $freq --dt $dt --equil_steps=$equil_steps --production_steps=$production_steps --particle_protocol "lattice" --output_dir "/${base_dir}/laynefrechette/aoup-assembly-results/${potential_type}/kT=${kT}/phi=${phi}/D0=${D0}/tau=${tau}/Lx=${Lx}_Ly=${Ly}"
            done
        done
    done
done
