# set terminal png size 1024,768 enhanced font "Helvetica,12"
set terminal png
# set term png
set boxwidth 1
set xlabel "Processos"
set ylabel "Speedup"
set xtics 0,2,32
set ytics 0,2,32
set grid ytics lc rgb "#505050"
set key width -15
set key bmargin right

# set title 'Speedup Syrk'

# set output './results/outputs_gnuplot/speedup.png'
set output './results_2/outputs_gnuplot/speedup.png'
f(x)=x
plot f(x) dashtype 30 title 'Ideal', \
 "./results_2/openmp_gnuplot/time.out" using 1:2 title 'OpenMP' with linespoints, \
 "./results_2/pthread_gnuplot/time.out" using 1:2 title 'Pthread' with linespoints, \
 "./results_2/mpi_opt_gnuplot/time_new.out" using 1:2 title 'MPI' with linespoints, \
 "./results_2/mpi_unopt_gnuplot/time_new.out" using 1:2 title 'MPI - gcc sem otimização' with linespoints, \
 "./results_2/mpi_2_1_gnuplot/time_new.out" using 1:2 title 'MPI - 1 emissor 1 processador' with linespoints, \
 "./results_2/mpi_kernel_gnuplot/time_kernel.out" using 1:2 title 'MPI - kernel' with linespoints
