set terminal png
set output './results/outputs_gnuplot/cache_misses.png'
# set title "Média de miss na cache" noenhanced font ",14" tc rgb "#606060"
set boxwidth 1
set style data histograms
set style fill solid border -1
# set xtics nomirror rotate by -45
set format y '%.1s %c'
set tic scale 0
set xtics 0,2,32
set xlabel "Threads"
set ylabel "Cache-misses"
set grid ytics lc rgb "#505050"
set key width -15
set key bmargin right
plot "./results/openmp_gnuplot/perf_cache_misses.out" using 2:xtic(1) title "OpenMP" lt rgb "#98FB98",\
	 "./results/pthread_gnuplot/perf_cache_misses.out" using 2 title "Pthreads" lt rgb "#7D9EC0"