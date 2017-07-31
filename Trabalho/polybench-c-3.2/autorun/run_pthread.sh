#!/bin/bash

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