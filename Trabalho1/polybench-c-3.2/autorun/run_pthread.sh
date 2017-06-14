#!/bin/bash

printf "Automatizando o trabalho de Programação Concorrente"
gcc -O3 -I utilities -I linear-algebra/kernels/syrk utilities/polybench.c linear-algebra/kernels/syrk/syrk.c -pthread -DMINI_DATASET -DPOLYBENCH_TIME -o syrk
gcc -O3 -I utilities -I linear-algebra/kernels/syrk utilities/polybench.c linear-algebra/kernels/syrk/syrk_pthread.c -pthread -DMINI_DATASET -DPOLYBENCH_TIME -o syrk_pthread

for i in 01 02 03; do

printf "\nTeste Pthread - 1 Thread \n"
perf stat -e cycles,instructions,cache-misses,cpu-clock,context-switches,cpu-migrations -o results/pthread/perf01_exec$i.out ./syrk >> results/pthread/time01_exec$i.out
printf "\nTeste Pthread - 2 Threads \n"
perf stat -e cycles,instructions,cache-misses,cpu-clock,context-switches,cpu-migrations -o results/pthread/perf02_exec$i.out ./syrk_pthread 2 >> results/pthread/time02_exec$i.out
printf "\nTeste Pthread - 4 Threads \n"
perf stat -e cycles,instructions,cache-misses,cpu-clock,context-switches,cpu-migrations -o results/pthread/perf04_exec$i.out ./syrk_pthread 4 >> results/pthread/time04_exec$i.out
printf "\nTeste Pthread - 8 Threads \n"
perf stat -e cycles,instructions,cache-misses,cpu-clock,context-switches,cpu-migrations -o results/pthread/perf08_exec$i.out ./syrk_pthread 8 >> results/pthread/time08_exec$i.out
printf "\nTeste Pthread - 16 Threads \n"
perf stat -e cycles,instructions,cache-misses,cpu-clock,context-switches,cpu-migrations -o results/pthread/perf16_exec$i.out ./syrk_pthread 16 >> results/pthread/time16_exec$i.out
printf "\nTeste Pthread - 32 Threads \n"
perf stat -e cycles,instructions,cache-misses,cpu-clock,context-switches,cpu-migrations -o results/pthread/perf32_exec$i.out ./syrk_pthread 32 >> results/pthread/time32_exec$i.out

done

python autorun/auto.py results/pthread/ results/pthread_gnuplot