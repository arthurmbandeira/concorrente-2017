import sys
import os
from collections import OrderedDict
from itertools import izip

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
        if 'new1' in i:
            f = open(os.path.join(name, i), 'r')
            time_01processes = [float(line.rstrip()) for line in f]
            f.close()
        elif 'kernel2' in i:
            f = open(os.path.join(name, i), 'r')
            time_02processes = [float(line.rstrip()) for line in f if line != '\n']
            f.close()
        elif 'kernel4' in i:
            f = open(os.path.join(name, i), 'r')
            time_04processes = [float(line.rstrip()) for line in f if line != '\n']
            f.close()
        elif 'kernel8' in i:
            f = open(os.path.join(name, i), 'r')
            time_08processes = [float(line.rstrip()) for line in f if line != '\n']
            f.close()
        elif 'kernel16' in i:
            f = open(os.path.join(name, i), 'r')
            time_16processes = [float(line.rstrip()) for line in f if line != '\n']
            f.close()
        elif 'kernel32' in i:
            f = open(os.path.join(name, i), 'r')
            time_32processes = [float(line.rstrip()) for line in f if line != '\n']
            f.close()


def grouped(iterable, n):
    "s -> (s0,s1,s2,...sn-1), (sn,sn+1,sn+2,...s2n-1), (s2n,s2n+1,s2n+2,...s3n-1), ..."
    return izip(*[iter(iterable)]*n)


def getLargerKernel(lst, n):
    m = [max(x) for x in grouped(lst, n)]
    return m


def execAverage(l):
    l.remove(max(l))
    l.remove(min(l))
    return sum(l)/float(len(l))


def writeTimeResults(path, name):
    global time_01processes, time02, time04, time08, time16, time32
    time_avg = OrderedDict()
    efficiency = OrderedDict()
    tS = execAverage(time_01processes)
    time_avg[1] = tS / tS
    efficiency[1] = time_avg[1] / 1.0
    time_avg[2] = tS / execAverage(time02)
    efficiency[2] = time_avg[2] / 2.0
    time_avg[4] = tS / execAverage(time04)
    efficiency[4] = time_avg[4] / 4.0
    time_avg[8] = tS / execAverage(time08)
    efficiency[8] = time_avg[8] / 8.0
    time_avg[16] = tS / execAverage(time16)
    efficiency[16] = time_avg[16] / 16.0
    time_avg[32] = tS / execAverage(time32)
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

time02 = getLargerKernel(time_02processes, 2)
time04 = getLargerKernel(time_04processes, 4)
time08 = getLargerKernel(time_08processes, 8)
time16 = getLargerKernel(time_16processes, 16)
time32 = getLargerKernel(time_32processes, 32)

writeTimeResults(path_gnuplot, 'time_kernel.out')

# print time02
# print time04
# print time08
# print time16
# print time32
# parseInput(time_02processes, 2)
# grouped(time_02processes, 2)

# print time_02processes
# print time_04processes
# print time_08processes
# print time_16processes
# print time_32processes
