import sys
import os

from collections import OrderedDict

results = os.listdir('../results/openmp')

time_01thread = []
time_02thread = []
time_04thread = []
time_08thread = []
time_16thread = []
time_32thread = []

perf01thread = []

for i in results:
    if 'exec01' in i:
        continue
    elif 'time01' in i:
        f = open(os.path.join('../results/openmp', i), 'r')
        time_01thread.append(float(f.readline().rstrip()))
        f.close()
    elif 'time02' in i:
        f = open(os.path.join('../results/openmp', i), 'r')
        time_02thread.append(float(f.readline().rstrip()))
        f.close()
    elif 'time04' in i:
        f = open(os.path.join('../results/openmp', i), 'r')
        time_04thread.append(float(f.readline().rstrip()))
        f.close()
    elif 'time08' in i:
        f = open(os.path.join('../results/openmp', i), 'r')
        time_08thread.append(float(f.readline().rstrip()))
        f.close()
    elif 'time16' in i:
        f = open(os.path.join('../results/openmp', i), 'r')
        time_16thread.append(float(f.readline().rstrip()))
        f.close()
    elif 'time32' in i:
        f = open(os.path.join('../results/openmp', i), 'r')
        time_32thread.append(float(f.readline().rstrip()))
        f.close()

def execAverage(l):
    l.remove(max(l))
    l.remove(min(l))
    return sum(l)/float(len(l))

def writeResultsTime():
    time_avg = OrderedDict()
    tS = execAverage(time_01thread)
    time_avg[1] = tS / tS
    time_avg[2] = tS / execAverage(time_02thread)
    time_avg[4] = tS / execAverage(time_04thread)
    time_avg[8] = tS / execAverage(time_08thread)
    time_avg[16] = tS / execAverage(time_16thread)
    time_avg[32] = tS / execAverage(time_32thread)

    f = open(os.path.join('../results/inputs_gnuplot', 'input_time.dat'), 'w')

    for i, k in time_avg.items():
        f.write(str(i) + ' ' + str(k) + '\n')

    f.close()

writeResultsTime()