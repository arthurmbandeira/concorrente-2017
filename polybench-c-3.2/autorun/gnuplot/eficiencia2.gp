set terminal png
set output './results/outputs_gnuplot/eficiencia2.png'
# set title "Média de miss na cache" noenhanced font ",14" tc rgb "#606060"
set boxwidth 1
# set style data histograms
# set style fill solid border -1
# set xtics nomirror rotate by -45
# set format y '%.1s %c'
set tic scale 0
set xtics 0,2,32
# set ytics 0,2,32
set xlabel "Threads"
set ylabel "Eficiência"
set grid ytics lc rgb "#505050"
set key width -15
set key bmargin right
set yrange [0:1.1]
plot 1 dashtype 30 title 'Ideal', \
	"./results/openmp_gnuplot/efficiency.out" using 1:2 title "OpenMP" lt rgb "#98FB98" with lines,\
	 "./results/pthread_gnuplot/efficiency.out" using 1:2 title "Pthreads" lt rgb "#7D9EC0" with lines