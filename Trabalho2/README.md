# concorrente-2017

Para executar:
 - Estar no diretório polybench-c-3.2/
 - Sequencial: ```gcc -O3 -I utilities -I linear-algebra/kernels/syrk utilities/polybench.c linear-algebra/kernels/syrk/syrk.c -pthread -DXXL_DATASET -DPOLYBENCH_TIME -o syrk```
 - OpenMP: ```gcc -O3 -I utilities -I linear-algebra/kernels/syrk utilities/polybench.c linear-algebra/kernels/syrk/syrk_openmp.c -fopenmp -DXXL_DATASET -DPOLYBENCH_TIME -o syrk_openmp```
 - Pthreads: ```gcc -O3 -I utilities -I linear-algebra/kernels/syrk utilities/polybench.c linear-algebra/kernels/syrk/syrk_pthread.c -pthread -DXXL_DATASET -DPOLYBENCH_TIME -o syrk_pthread```
 - MPI: ```mpicc -O3 -I utilities -I linear-algebra/kernels/syrk utilities/polybench.c linear-algebra/kernels/syrk/syrk_mpi.c -DEXTRALARGE_DATASET -DPOLYBENCH_TIME -o syrk_mpi ``` e ```mpirun -n <numero_de_processadores_+_1> -hostfile <lista_de_ips> -N <processos_por_maquina> ./syrk_mpi ```
 - Ou executar os scripts ```run_openmp.sh```, ```run_pthread.sh``` ou ```run_mpi.sh```

Os arquivos do benchmark syrk paralelizados são syrk_openmp, syrk_pthread e syrk_mpi e encontram-se no diretório ```linear-algebra/kernels/syrk/```
Os scripts para geração dos gráficos do gnuplot encontram-se no diretório ```autorun/gnuplot```
Os resultados gerados que serviram de base para o artigo estão no diretório ```results_2/```
