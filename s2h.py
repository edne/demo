#!/usr/bin/env python
from os.path import exists
from sys import argv

name = argv[1].split('.')[-2]

with open(name+'_shaders.h', 'w') as header:
    for ext in ('frag', 'vert'):
        if exists(name+'.'+ext):
            header.write('const char *'+ext.upper()+'_SOURCE = "\\\n')
            with open(name+'.'+ext, 'r') as shader:
                for r in shader:
                    header.write(r[:-1]+'\\\n')
            header.write('";\n');
