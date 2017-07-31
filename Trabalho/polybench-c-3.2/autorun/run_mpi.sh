#!/bin/bash

printf "Automatizando o trabalho de Programação Concorrente"
# mpicc -O3 -I utilities -I linear-algebra/kernels/syrk utilities/polybench.c linear-algebra/kernels/syrk/syrk_mpi.c -DEXTRALARGE_DATASET -DPOLYBENCH_TIME -o syrk_mpi
mkdir results

for i in 01 02 03 04 05 06 07 08 09 10 11; do

# printf "\nTeste Pthread - 1 Thread \n"

printf "\nTeste MPI - 2 Processos \n"
mpirun -n 3 -hostfile hosts2.txt -N 1 ./syrk_mpi >> results/time_new2.txt
printf "\nTeste MPI - 4 Processos \n"
mpirun -n 5 -hostfile hosts2.txt -N 2 ./syrk_mpi >> results/time_new4.txt
printf "\nTeste MPI - 8 Processos \n"
mpirun -n 9 -hostfile hosts2.txt -N 3 ./syrk_mpi >> results/time_new8.txt
printf "\nTeste MPI - 16 Processos \n"
mpirun -n 17 -hostfile hosts16.txt ./syrk_mpi >> results/time_new16.txt
printf "\nTeste MPI - 32 Processos \n"
mpirun -n 33 -hostfile hosts32.txt ./syrk_mpi >> results/time_new32.txt

done

# python autorun/auto.py results/pthread/ results/pthread_gnuplot
