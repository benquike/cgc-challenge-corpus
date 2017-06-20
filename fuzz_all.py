#!/usr/bin/env python

import sys
import os
import re
import driller
import fuzzer
import time

def get_cbs_with_vul(root_dir):
    '''
    Find the cyber challenge with vulnerability in root_dir,
    typically in bin dir.

    The return value is a list
    '''

    bin_dir = os.path.join(root_dir, "bin")
    return [os.path.join(bin_dir, x) for x in os.listdir(bin_dir) if x != '.' and x != '..' and
            'patched' not in x or 'partial' in x]



def main():

    if len(sys.argv) != 2:
        print "Usage: fuzz_all.py <root_dir>"
        sys.exit(-1)

    root_dir = sys.argv[1]
    challenges = [x for x in os.listdir(root_dir) if
                  os.path.isdir(os.path.join(root_dir, x)) and
                  re.match('[A-Z]*_[0-9]*', x) != None]

    workdir = "/dev/shm/work/"
    try:
        os.mkdir(workdir)
    except OSError:
        pass

    for ch in challenges:
        print "starting to fuzz:" + ch
        ch_dir = os.path.join(root_dir, ch)
        binaries = get_cbs_with_vul(ch_dir)
        print "binaries:" + str(binaries)
        for bin in binaries:
            print "fuzzing " + bin
            driller_extension = driller.LocalCallback(num_workers=1)
            phuzzer = fuzzer.Fuzzer(bin, workdir, afl_count=1, create_dictionary=True,
                                stuck_callback=driller_extension)
            while True:
                time.sleep(60)
                if bool(phuzzer.stats) and int(phuzzer.stats['fuzzer-master']['pending_favs']) == 0:
                    break

            phuzzer.kill()
            driller_extension.kill()

if __name__ == '__main__':
    main()
