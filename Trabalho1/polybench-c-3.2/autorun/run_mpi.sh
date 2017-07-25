#!/bin/bash

printf "Automatizando o trabalho de Programação Concorrente"
# mpicc -O3 -I utilities -I linear-algebra/kernels/syrk utilities/polybench.c linear-algebra/kernels/syrk/syrk_mpi.c -DEXTRALARGE_DATASET -DPOLYBENCH_TIME -o syrk_mpi
mkdir results

for i in 01 02 03 04 05 06 07 08 09 10 11; do

# printf "\nTeste Pthread - 1 Thread \n"

printf "\nTeste MPI - 2 Processos \n"
mpirun -n 2 -hostfile hosts2.txt -N 1 ./syrk_mpi >> results/time2_1.txt
printf "\nTeste MPI - 4 Processos \n"
mpirun -n 4 -hostfile hosts2.txt -N 2 ./syrk_mpi >> results/time4_2.txt
printf "\nTeste MPI - 8 Processos \n"
mpirun -n 8 -hostfile hosts2.txt -N 3 ./syrk_mpi >> results/time8_3.txt
printf "\nTeste MPI - 16 Processos \n"
mpirun -n 16 -hostfile hosts16.txt ./syrk_mpi >> results/time16_8.txt
printf "\nTeste MPI - 32 Processos \n"
mpirun -n 32 -hostfile hosts32.txt ./syrk_mpi >> results/time32_16.txt

done

# python autorun/auto.py results/pthread/ results/pthread_gnuplot