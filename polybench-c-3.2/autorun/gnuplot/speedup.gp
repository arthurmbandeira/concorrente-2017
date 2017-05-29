set terminal png size 640,480 enhanced font "Helvetica,20"
set term png
set xlabel "Threads"
set ylabel "Speedup"
set xtics 0,2,32
set ytics 0,2,32
set grid ytics lc rgb "#505050"

# set title 'Speedup Syrk'

set output './results/outputs_gnuplot/speedup.png'
f(x)=x
plot f(x) dashtype 30 title 'Ideal', \
 "./results/openmp_gnuplot/time.out" using 1:2 title 'OpenMP' with lines, \
 "./results/pthread_gnuplot/time.out" using 1:2 title 'Pthread' with lines
