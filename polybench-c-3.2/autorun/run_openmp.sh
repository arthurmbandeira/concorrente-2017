#!/bin/bash

printf "Automatizando o trabalho de Concorrente"
gcc -O3 -I utilities -I linear-algebra/kernels/syrk utilities/polybench.c linear-algebra/kernels/syrk/syrk_openmp.c -fopenmp -DXXL_DATASET -DPOLYBENCH_TIME -o syrk_openmp

for i in 01 02 03 04 05 06 07 08 09 10 11; do

printf "\nTeste OpenMP - 1 Thread \n"
export OMP_NUM_THREADS=1
perf stat -e cycles,instructions,cache-misses,cpu-clock,context-switches,cpu-migrations -o results/perf01thread_exec$i.out ./syrk_openmp >> results/time01_exec$i.out
printf "\nTeste OpenMP - 2 Threads \n"
export OMP_NUM_THREADS=2
perf stat -e cycles,instructions,cache-misses,cpu-clock,context-switches,cpu-migrations -o results/perf02thread_exec$i.out ./syrk_openmp >> results/time02_exec$i.out
printf "\nTeste OpenMP - 4 Threads \n"
export OMP_NUM_THREADS=4
perf stat -e cycles,instructions,cache-misses,cpu-clock,context-switches,cpu-migrations -o results/perf04thread_exec$i.out ./syrk_openmp >> results/time04_exec$i.out
printf "\nTeste OpenMP - 8 Threads \n"
export OMP_NUM_THREADS=8
perf stat -e cycles,instructions,cache-misses,cpu-clock,context-switches,cpu-migrations -o results/perf08thread_exec$i.out ./syrk_openmp >> results/time08_exec$i.out
printf "\nTeste OpenMP - 16 Threads \n"
export OMP_NUM_THREADS=16
perf stat -e cycles,instructions,cache-misses,cpu-clock,context-switches,cpu-migrations -o results/perf16thread_exec$i.out ./syrk_openmp >> results/time16_exec$i.out
printf "\nTeste OpenMP - 32 Threads \n"
export OMP_NUM_THREADS=32
perf stat -e cycles,instructions,cache-misses,cpu-clock,context-switches,cpu-migrations -o results/perf32thread_exec$i.out ./syrk_openmp >> results/time32_exec$i.out

done

cd autorun
python auto_pthread.py