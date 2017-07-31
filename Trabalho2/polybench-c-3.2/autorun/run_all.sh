#!/bin/bash

$DIRECTORY=/results_2
if ! [ -d "$DIRECTORY" ]; then
    mkdir results_2
    mkdir results_2/openmp
    mkdir results_2/pthread
    mkdir results_2/seq_unoptmized
fi

printf "Automatizando o trabalho de Programação Concorrente"
gcc -O3 -I utilities -I linear-algebra/kernels/syrk utilities/polybench.c linear-algebra/kernels/syrk/syrk.c -pthread -DEXTRALARGE_DATASET -DPOLYBENCH_TIME -o syrk
gcc -O3 -I utilities -I linear-algebra/kernels/syrk utilities/polybench.c linear-algebra/kernels/syrk/syrk_pthread.c -pthread -DEXTRALARGE_DATASET -DPOLYBENCH_TIME -o syrk_pthread

for i in 01 02 03 04 05 06 07 08 09 10 11; do

printf "\nTeste Pthread - 1 Thread \n"
./syrk >> results_2/pthread/time01_exec$i.out
printf "\nTeste Pthread - 2 Threads \n"
./syrk_pthread 2 >> results_2/pthread/time02_exec$i.out
printf "\nTeste Pthread - 4 Threads \n"
./syrk_pthread 4 >> results_2/pthread/time04_exec$i.out
printf "\nTeste Pthread - 8 Threads \n"
./syrk_pthread 8 >> results_2/pthread/time08_exec$i.out
printf "\nTeste Pthread - 16 Threads \n"
./syrk_pthread 16 >> results_2/pthread/time16_exec$i.out
printf "\nTeste Pthread - 32 Threads \n"
./syrk_pthread 32 >> results_2/pthread/time32_exec$i.out

done

python autorun/auto.py results_2/pthread/ results_2/pthread_gnuplot

printf "Automatizando o trabalho de Programação Concorrente"
gcc -O3 -I utilities -I linear-algebra/kernels/syrk utilities/polybench.c linear-algebra/kernels/syrk/syrk_openmp.c -fopenmp -DEXTRALARGE_DATASET -DPOLYBENCH_TIME -o syrk_openmp

for i in 01 02 03 04 05 06 07 08 09 10 11; do

printf "\nTeste OpenMP - 1 Thread \n"
export OMP_NUM_THREADS=1
./syrk_openmp >> results_2/openmp/time01_exec$i.out
printf "\nTeste OpenMP - 2 Threads \n"
export OMP_NUM_THREADS=2
./syrk_openmp >> results_2/openmp/time02_exec$i.out
printf "\nTeste OpenMP - 4 Threads \n"
export OMP_NUM_THREADS=4
./syrk_openmp >> results_2/openmp/time04_exec$i.out
printf "\nTeste OpenMP - 8 Threads \n"
export OMP_NUM_THREADS=8
./syrk_openmp >> results_2/openmp/time08_exec$i.out
printf "\nTeste OpenMP - 16 Threads \n"
export OMP_NUM_THREADS=16
./syrk_openmp >> results_2/openmp/time16_exec$i.out
printf "\nTeste OpenMP - 32 Threads \n"
export OMP_NUM_THREADS=32
./syrk_openmp >> results_2/openmp/time32_exec$i.out

done

python autorun/auto.py results_2/openmp/ results_2/openmp_gnuplot

printf "Automatizando o trabalho de Programação Concorrente"

gcc -I utilities -I linear-algebra/kernels/syrk utilities/polybench.c linear-algebra/kernels/syrk/syrk.c -DEXTRALARGE_DATASET -DPOLYBENCH_TIME -o syrk_unoptmized
for i in 01 02 03 04 05 06 07 08 09 10 11; do

printf "\nTeste sem otimização - 1 Thread \n"
./syrk_unoptmized >> results_2/seq_unoptmized/time01_exec$i.out

done

python autorun/auto.py results_2/seq_unoptmized/ results_2/seq_unoptmized_gnuplot