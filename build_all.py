#!/usr/bin/env python

import os
import sys
import subprocess

if len(sys.argv) != 2:
    print 'build_all.py <root_dir>'
    sys.exit(-1)

root_dir = sys.argv[1]
challenges = [x for x in os.listdir(root_dir) if
              not os.path.isfile(os.path.join(root_dir, x)) and
              x != "service-template" and
              x != "." and
              x != ".."]

print challenges

for cb in challenges:
    subprocess.call(['make', '-C', os.path.join(root_dir, cb)])

