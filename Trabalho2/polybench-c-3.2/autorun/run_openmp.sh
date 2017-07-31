#!/bin/bash

printf "Automatizando o trabalho de Programação Concorrente"
gcc -O3 -I utilities -I linear-algebra/kernels/syrk utilities/polybench.c linear-algebra/kernels/syrk/syrk_openmp.c -fopenmp -DXXL_DATASET -DPOLYBENCH_TIME -o syrk_openmp

for i in 01 02 03 04 05 06 07 08 09 10 11; do

printf "\nTeste OpenMP - 1 Thread \n"
export OMP_NUM_THREADS=1
./syrk_openmp >> results_2/time01_exec$i.out
printf "\nTeste OpenMP - 2 Threads \n"
export OMP_NUM_THREADS=2
./syrk_openmp >> results_2/time02_exec$i.out
printf "\nTeste OpenMP - 4 Threads \n"
export OMP_NUM_THREADS=4
./syrk_openmp >> results_2/time04_exec$i.out
printf "\nTeste OpenMP - 8 Threads \n"
export OMP_NUM_THREADS=8
./syrk_openmp >> results_2/time08_exec$i.out
printf "\nTeste OpenMP - 16 Threads \n"
export OMP_NUM_THREADS=16
./syrk_openmp >> results_2/time16_exec$i.out
printf "\nTeste OpenMP - 32 Threads \n"
export OMP_NUM_THREADS=32
./syrk_openmp >> results_2/time32_exec$i.out

done

python autorun/auto.py results_2/openmp/ results_2/openmp_gnuplot