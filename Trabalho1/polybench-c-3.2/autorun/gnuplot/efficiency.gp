set terminal png
set output './results/outputs_gnuplot/eficiencia.png'
set title "Eficiência Syrk" noenhanced font ",14" tc rgb "#606060"
set boxwidth 1
set style data histograms
set style fill solid border -1
# set xtics nomirror rotate by -45
# set format y '%.1s %c'
set tic scale 0
set xtics 0,2,32
# set ytics 0,1.1
set yrange [0:1.1]
set xlabel "Threads"
set ylabel "Eficiência"
set grid ytics lc rgb "#505050"
set key width -15
set key bmargin right
plot "./results/openmp_gnuplot/efficiency.out" using 2:xtic(1) title "OpenMP" lt rgb "#98FB98",\
	 "./results/pthread_gnuplot/efficiency.out" using 2 title "Pthreads" lt rgb "#7D9EC0"