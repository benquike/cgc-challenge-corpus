#!/usr/bin/env python

import os
import sys
import subprocess
import argparse
import re

if len(sys.argv) != 2:
    print 'build_all.py <root_dir>'
    sys.exit(-1)

root_dir = os.path.abspath(sys.argv[1])
challenges = [x for x in os.listdir(root_dir) if
              not os.path.isfile(os.path.join(root_dir, x)) and bool(re.match(r'[A-Z]*_[0-9]*', x))]

for cb in challenges:
    print "building challenge " + cb

    subprocess.call(['make', '-C' , os.path.join(root_dir, cb), 'clean'])
    subprocess.call(['make', '-C' , os.path.join(root_dir, cb), 'build'])

