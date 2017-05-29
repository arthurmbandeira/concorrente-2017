import sys
import os

from collections import OrderedDict

results_name = sys.argv[1]
results_folder = os.listdir(results_name)
path_gnuplot = sys.argv[2]

time_01thread = []
time_02thread = []
time_04thread = []
time_08thread = []
time_16thread = []
time_32thread = []

cache_misses = OrderedDict()
cpu_clock = OrderedDict()
context_switches = OrderedDict()
cpu_migrations = OrderedDict()
instructions_per_cycles = OrderedDict()

def appendResultsTime(results_folder, name):
    for i in results_folder:
        if 'exec01' in i:
            continue
        elif 'time01' in i:
            f = open(os.path.join(name, i), 'r')
            time_01thread.append(float(f.readline().rstrip()))
            f.close()
        elif 'time02' in i:
            f = open(os.path.join(name, i), 'r')
            time_02thread.append(float(f.readline().rstrip()))
            f.close()
        elif 'time04' in i:
            f = open(os.path.join(name, i), 'r')
            time_04thread.append(float(f.readline().rstrip()))
            f.close()
        elif 'time08' in i:
            f = open(os.path.join(name, i), 'r')
            time_08thread.append(float(f.readline().rstrip()))
            f.close()
        elif 'time16' in i:
            f = open(os.path.join(name, i), 'r')
            time_16thread.append(float(f.readline().rstrip()))
            f.close()
        elif 'time32' in i:
            f = open(os.path.join(name, i), 'r')
            time_32thread.append(float(f.readline().rstrip()))
            f.close()

def perfFilesAnalysis(results_folder, name):
    for i in results_folder:
        if 'exec01' in i:
            continue
        elif 'perf01' in i:
            availLines(i, 1, name)
        elif 'perf02' in i:
            availLines(i, 2, name)
        elif 'perf04' in i:
            availLines(i, 4, name)
        elif 'perf08' in i:
            availLines(i, 8, name)
        elif 'perf16' in i:
            availLines(i, 16, name)
        elif 'perf32' in i:
            availLines(i, 32, name)

def availLines(tag, d, name):
    f = open(os.path.join(name, tag), 'r')
    for line in f:
        spt = line.split()
        for i in range(len(spt)):
            if spt[i] == 'cache-misses':
                try:
                    cache_misses[d].append(float(spt[i-1].replace(".", "")))
                except KeyError:
                    cache_misses[d] = []
                    cache_misses[d].append(float(spt[i-1].replace(".", "")))
            if spt[i] == 'cpu-clock':
                try:
                    cpu_clock[d].append(float(spt[i-1].replace(",", ".")))
                except KeyError:
                    cpu_clock[d] = []
                    cpu_clock[d].append(float(spt[i-1].replace(",", ".")))
            if spt[i] == 'context-switches':
                try:
                    context_switches[d].append(float(spt[i-1].replace(".", "")))
                except KeyError:
                    context_switches[d] = []
                    context_switches[d].append(float(spt[i-1].replace(".", "")))
            if spt[i] == 'cpu-migrations':
                try:
                    cpu_migrations[d].append(float(spt[i-1].replace(".", "")))
                except KeyError:
                    cpu_migrations[d] = []
                    cpu_migrations[d].append(float(spt[i-1].replace(".", "")))
            if spt[i] == 'insns':
                try:
                    instructions_per_cycles[d].append(float(spt[i-1].replace(",", ".")))
                except KeyError:
                    instructions_per_cycles[d] = []
                    instructions_per_cycles[d].append(float(spt[i-1].replace(",", ".")))
    f.close()

def execAverage(l):
    l.remove(max(l))
    l.remove(min(l))
    return sum(l)/float(len(l))

def writeTimeResults(path, name):
    time_avg = OrderedDict()
    efficiency = OrderedDict()
    tS = execAverage(time_01thread)
    time_avg[1] = tS / tS
    efficiency[1] = time_avg[1] / 1.0
    time_avg[2] = tS / execAverage(time_02thread)
    efficiency[2] = time_avg[2] / 2.0
    time_avg[4] = tS / execAverage(time_04thread)
    efficiency[4] = time_avg[4] / 4.0
    time_avg[8] = tS / execAverage(time_08thread)
    efficiency[8] = time_avg[8] / 8.0
    time_avg[16] = tS / execAverage(time_16thread)
    efficiency[16] = time_avg[16] / 16.0
    time_avg[32] = tS / execAverage(time_32thread)
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

def writePerfResults(d, path, name):
    perf_avg = OrderedDict()

    try:
        f = open(os.path.join(path, name), 'w')
    except IOError:
        os.makedirs(path)
        f = open(os.path.join(path, name), 'w')

    for i, k in sorted(d.items()):
        perf_avg[i] = execAverage(k)
        f.write(str(i) + ' ' + str(perf_avg[i]) + '\n')

    f.close()

appendResultsTime(results_folder, results_name)
writeTimeResults(path_gnuplot, 'time.out')
perfFilesAnalysis(results_folder, results_name)
writePerfResults(instructions_per_cycles, path_gnuplot, 'perf_instructions_per_cycles.out')
writePerfResults(cache_misses, path_gnuplot, 'perf_cache_misses.out')
writePerfResults(cpu_clock, path_gnuplot, 'perf_cpu_clock.out')
writePerfResults(context_switches, path_gnuplot, 'perf_context_switches.out')
writePerfResults(cpu_migrations, path_gnuplot, 'perf_cpu_migrations.out')