# concorrente-2017

Para executar:
 - Estar no diretório polybench-c-3.2/
 - Sequencial: gcc -O3 -I utilities -I linear-algebra/kernels/syrk utilities/polybench.c linear-algebra/kernels/syrk/syrk.c -pthread -DXXL_DATASET -DPOLYBENCH_TIME -o syrk
 - OpenMP: gcc -O3 -I utilities -I linear-algebra/kernels/syrk utilities/polybench.c linear-algebra/kernels/syrk/syrk_openmp.c -fopenmp -DXXL_DATASET -DPOLYBENCH_TIME -o syrk_openmp
 - Pthreads: gcc -O3 -I utilities -I linear-algebra/kernels/syrk utilities/polybench.c linear-algebra/kernels/syrk/syrk_pthread.c -pthread -DXXL_DATASET -DPOLYBENCH_TIME -o syrk_pthread
 - Ou executar os scripts run_openmp.sh ou run_pthread.sh 

Os arquivos do benchmark syrk paralelizados são o syrk_openmp e o syrk_pthread e encontram-se no diretório linear-algebra/kernels/syrk/
Os scripts para geração dos gráficos do gnuplot encontram-se no diretório autorun/gnuplot
Os resultados gerados que serviram de base para o artigo estão no diretório results/
