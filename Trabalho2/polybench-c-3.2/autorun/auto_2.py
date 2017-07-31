import sys
import os
from collections import OrderedDict

results_name = sys.argv[1]
results_folder = os.listdir(results_name)
path_gnuplot = sys.argv[2]

time_01processes = [] 
time_02processes = [] 
time_04processes = [] 
time_08processes = []
time_16processes = [] 
time_32processes = []


def appendResultsTime(results_folder, name):
    global time_01processes, time_02processes, time_04processes, time_08processes, time_16processes, time_32processes
    for i in results_folder:
        # if 'unopt_1' in i:
        if 'new1.' in i:
        # if 'mpi' in i:
            f = open(os.path.join(name, i), 'r')
            time_01processes = [float(line.rstrip()) for line in f]
            f.close()
        elif 'new2' in i:
            f = open(os.path.join(name, i), 'r')
            time_02processes = [float(line.rstrip()) for line in f]
            f.close()
        elif 'new4' in i:
            f = open(os.path.join(name, i), 'r')
            time_04processes = [float(line.rstrip()) for line in f]
            f.close()
        elif 'new8' in i:
            f = open(os.path.join(name, i), 'r')
            time_08processes = [float(line.rstrip()) for line in f]
            f.close()
        elif 'new16' in i:
            f = open(os.path.join(name, i), 'r')
            time_16processes = [float(line.rstrip()) for line in f]
            f.close()
        elif 'new32' in i:
            f = open(os.path.join(name, i), 'r')
            time_32processes = [float(line.rstrip()) for line in f]
            f.close()


def execAverage(l):
    l = l[1:len(l)]
    l.remove(max(l))
    l.remove(min(l))
    return sum(l)/float(len(l))


def writeTimeResults(path, name):
    time_avg = OrderedDict()
    efficiency = OrderedDict()
    tS = execAverage(time_01processes)
    time_avg[1] = tS / tS
    efficiency[1] = time_avg[1] / 1.0
    time_avg[2] = tS / execAverage(time_02processes)
    efficiency[2] = time_avg[2] / 2.0
    time_avg[4] = tS / execAverage(time_04processes)
    efficiency[4] = time_avg[4] / 4.0
    time_avg[8] = tS / execAverage(time_08processes)
    efficiency[8] = time_avg[8] / 8.0
    time_avg[16] = tS / execAverage(time_16processes)
    efficiency[16] = time_avg[16] / 16.0
    time_avg[32] = tS / execAverage(time_32processes)
    efficiency[32] = time_avg[32] / 32.0

    try:
        f = open(os.path.join(path, name), 'w')
        g = open(os.path.join(path, 'efficiency.out'), 'w')
    except IOError:
        os.makedirs(path)
        f = open(os.path.join(path, name), 'w')
        g = open(os.path.join(path, 'efficiency.out'), 'w')

    for i, k in time_avg.items():
        f.write(str(i) + ' ' + str(k) + '\n')
        g.write(str(i) + ' ' + str(efficiency[i]) + '\n')

    f.close()
    g.close()


appendResultsTime(results_folder, results_name)
writeTimeResults(path_gnuplot, 'time_new.out')
