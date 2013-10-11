#!/usr/bin/env python

import os
import sys
import subprocess
import tempfile
import getopt
import json
import re
import numpy
import pylab


HIST_SIZE = 32


def usage():

    print 'usage: run.py -c <context-name> -a <application>'
    sys.exit(2)

def error():

    print 'There was an error. Aborting.'
    sys.exit(3)

def parse_args(argv):

    try:
        opts, args = getopt.getopt(sys.argv[1:], 'a:c:', ['app=', 'context='])
    except getopt.GetoptError:
        usage()
    if len(opts) < 2 or args:
        usage()

    if opts[0][0] in ['-c', '--context']:
        opts.reverse()

    return opts[0][1], opts[1][1]

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

def E_size(data):

    abs_size = data['num-allocs-per-size']
    allocs = total_allocs(data)
    rel_size = [0] * HIST_SIZE

    for size, num in abs_size:
        if size:
            rel_size[log2(size)] = float(num) / allocs

    return rel_size

def E_malloc(data):

    allocs = total_allocs(data)
    allocs_per_ms = float(allocs) / data['total-time']
    return allocs_per_ms * 1000

def E_life(data):

    abs_time_per_size = data['num-allocs-per-size-per-time']
    rel_time_per_size = [[0] * HIST_SIZE] * HIST_SIZE

    for size, time, num in abs_time_per_size:
        if size:
            rel_time_per_size[log2(size)][log2(time)] = num

    for size in range(HIST_SIZE):
        allocs = float(sum(rel_time_per_size[size]))
        if allocs:
            rel = [x / allocs for x in rel_time_per_size[size]]
            rel_time_per_size[size] = rel

    return rel_time_per_size


if __name__ == "__main__":

    app, context = parse_args(sys.argv[1:])
    data = load_data(app)

    ret = {
        'E_size': E_size(data),
        'E_malloc': E_malloc(data),
        'E_life': E_life(data),
    }
