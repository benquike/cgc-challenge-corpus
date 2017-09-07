#!/usr/bin/env python

import os
import sys
import subprocess
import argparse
import re



def atoi(text):
    return int(text) if text.isdigit() else text

def natural_keys(text):
    '''
    alist.sort(key=natural_keys) sorts in human order
    http://nedbatchelder.com/blog/200712/human_sorting.html
    (See Toothy's implementation in the comments)
    '''
    return [ atoi(c) for c in re.split('(\d+)', text) ]

def main():
    parser = argparse.ArgumentParser(description="Building CGC challenges")

    parser.add_argument("--root_dir", required=True, help="The root directory of the corpus")
    parser.add_argument("--regex", help="The regex to filer the challenges")
    parser.add_argument("--verbose", help="show the build log", action="store_true", default=False)
    parser.add_argument("--keep", help="Keep building in case of error", action="store_true", default=False)

    args = parser.parse_args()

    root_dir = os.path.abspath(args.root_dir)
    challenges = [x for x in os.listdir(root_dir) if
                  not os.path.isfile(os.path.join(root_dir, x)) and bool(re.match(r'[A-Z]*_[0-9]*', x))]

    challenges.sort(key=natural_keys)

    FNULL = open(os.devnull, 'w')

    for cb in challenges:

        if not (args.regex != None and re.match('^' + args.regex + '$', cb)):
            continue

        print "building challenge " + cb

        if args.verbose:
            subprocess.call(['make', '-C' , os.path.join(root_dir, cb), 'clean'])
            ret = subprocess.call(['make', '-C' , os.path.join(root_dir, cb), 'build'])
        else:
            subprocess.call(['make', '-C' , os.path.join(root_dir, cb), 'clean'], stderr=FNULL, stdout=FNULL)
            ret = subprocess.call(['make', '-C' , os.path.join(root_dir, cb), 'build'], stderr=FNULL, stdout=FNULL)

        if ret != 0:
            print cb + " Failed"
            break

if __name__ == '__main__':
    main()
