#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import subprocess
import tempfile
import getopt
import json
import re
import numpy
import pylab
from matplotlib.ticker import FuncFormatter


HIST_SIZE = 32


def usage():

    print 'usage: run.py -c <context-name> -a <application> [-s]'
    sys.exit(2)

def error():

    print 'There was an error. Aborting.'
    sys.exit(3)

def parse_args(argv):

    try:
        opts, args = getopt.getopt(sys.argv[1:], 'a:c:s')
        opts.sort()
    except getopt.GetoptError:
        usage()
    if args or len(opts) < 2 or opts[1][0] != '-c':
        usage()

    return opts[0][1], opts[1][1], len(opts) == 3

def load_data(app):

    tmp_file = tempfile.NamedTemporaryFile(delete=False)
    tmp_name = tmp_file.name
    tmp_file.close()

    try:
        ret = subprocess.call(['./sample.d', '-o', tmp_name, '-c', app])
    except OSError:
        os.remove(tmp_name)
        error()
    if ret:
        os.remove(tmp_name)
        error()

    tmp_file = open(tmp_name)
    data = tmp_file.read()
    tmp_file.close()
    os.remove(tmp_name)

    data = re.sub(r',(\s*[}\]])', r'\1', data)
    return json.loads(data);

def log2(x):

    r = 0
    for i in [16, 8, 4, 2, 1]:
        if x >> i:
            r += i
            x >>= i
    return r

def total_allocs(data):

    allocs = data['num-allocs-total']
    sizes = data['num-allocs-per-size']
    return allocs - dict(sizes).get(0, 0)

def mjrFormatter(x, pos):

    if x == HIST_SIZE - 1:
        return "$\infty$"

    return "$2^{{{0}}}$".format(int(x))

def plot_E_size(E_size, context, show):

    pylab.figure()
    pylab.bar(range(HIST_SIZE / 2), E_size[:HIST_SIZE / 2], align='center')
    pylab.gca().xaxis.set_major_formatter(FuncFormatter(mjrFormatter))
    pylab.gca().set_xlim([-1, HIST_SIZE / 2])

    pylab.title(u'Histograma de mallocs por tamaño')
    pylab.xlabel(u'Tamaño en bytes')

    pylab.savefig(context + '_size.png', bbox_inches='tight')

    if show: pylab.show()

def E_size(data, context, show):

    abs_size = data['num-allocs-per-size']
    allocs = total_allocs(data)
    rel_size = [0] * HIST_SIZE

    for size, num in abs_size:
        if size:
            rel_size[log2(size)] = float(num) / allocs

    plot_E_size(rel_size, context, show)

    return rel_size

def E_malloc(data):

    allocs_per_ms = data['num-allocs-per-ms']
    allocs_per_ms.sort()
    allocs_per_ms = [log2(y) for x, y in allocs_per_ms]

    markov = [[0] * HIST_SIZE for x in range(HIST_SIZE)]

    for pos in range(len(allocs_per_ms) - 1):
        markov[allocs_per_ms[pos]][allocs_per_ms[pos + 1]] += 1

    for row in range(HIST_SIZE):
        s = float(sum(markov[row]))
        if s:
            markov[row] = [allocs / s for allocs in markov[row]]

    return markov

def plot_E_life(E_life, context, show):

    pylab.figure()
    pylab.imshow(E_life[:HIST_SIZE / 2], interpolation='nearest', origin='lower')
    pylab.colorbar(fraction=0.02, ticks=[])

    axes = pylab.gca()
    axes.yaxis.set_major_formatter(FuncFormatter(mjrFormatter))
    axes.xaxis.set_major_formatter(FuncFormatter(mjrFormatter))

    ticks = [5 * x for x in range((HIST_SIZE-2) / 5)] + [HIST_SIZE - 1]
    axes.set_xticks(ticks)

    pylab.title(u'Histograma de mallocs por tamaño, por tiempo de vida')
    pylab.xlabel(u'Tiempo de vida en µs')
    pylab.ylabel(u'Tamaño en bytes')

    pylab.savefig(context + '_life.png', bbox_inches='tight')

    if show: pylab.show()

def E_life(data, context, show):

    abs_life_per_size = data['num-allocs-per-size-per-life']
    rel_life_per_size = [[0] * HIST_SIZE for row in range(HIST_SIZE)]

    for size, life, num in abs_life_per_size:
        if size:
            rel_life_per_size[log2(size)][log2(life)] = num

    num_allocs = dict(data['num-allocs-per-size'])
    num_frees = dict(data['num-frees-per-size'])

    for size in range(HIST_SIZE):
        rel_life_per_size[size][HIST_SIZE - 1] = \
            num_allocs.get(2**size, 0) - num_frees.get(2**size, 0)

    plot_E_life(rel_life_per_size, context, show)

    for size in range(HIST_SIZE):
        allocs = float(sum(rel_life_per_size[size]))
        if allocs:
            rel = [x / allocs for x in rel_life_per_size[size]]
            rel_life_per_size[size] = rel

    return rel_life_per_size

def vec2line(vec):

    return ' '.join(['%.8f' % n for n in vec])

def save_data(data, context):

    f = open(context + '.json', 'w')
    json.dump(data, f, indent=2)
    f.close()

    f = open(context + '.txt', 'w')
    f.write('{}\n'.format(HIST_SIZE))
    f.write(vec2line(data['E_size']) + '\n')
    f.write('\n'.join([vec2line(row) for row in data['E_malloc']]))
    f.write('\n'.join([vec2line(row) for row in data['E_life']]))
    f.close()


if __name__ == "__main__":

    app, context, show = parse_args(sys.argv[1:])
    data = load_data(app)

    ret = {
        'E_size': E_size(data, context, show),
        'E_malloc': E_malloc(data),
        'E_life': E_life(data, context, show),
    }

    save_data(ret, context)
