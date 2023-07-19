#!/bin/bash

Lx=$1
Ly=$2
phi=$3
kT=$4
D0=$5
tau=$6
potential_type=$7

input="seeds/seeds_kT=${kT}_phi=${phi}_D0=${D0}_tau=${tau}_Lx=${Lx}_Ly=${Ly}.txt"


sbatch -J "${potential_type}_aoup_assembly_kT=${kT}_phi=${phi}_D0=${D0}_tau=${tau}_Lx=${Lx}_Ly=${Ly}_seed=${i}" -o "log/${potential_type}_aoup_assembly_kT=${kT}_phi=${phi}_D0=${D0}_tau=${tau}_Lx=${Lx}_Ly=${Ly}_seed=${i}.o%j" -e "log/${potential_type}_aoup_assembly_kT=${kT}_phi=${phi}_D0=${D0}_tau=${tau}_Lx=${Lx}_Ly=${Ly}_seed=${i}.e%j" scripts/submit_sims.sh "/home/laynefrechette/aoup_assembly/input_files/${potential_type}_aoup_assembly_kT=${kT}_phi=${phi}_D0=${D0}_tau=${tau}_Lx=${Lx}_Ly=${Ly}.in" 1 $input
