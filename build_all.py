#!/usr/bin/env python

import os
import sys
import subprocess
import argparse
import re
import glob

def atoi(text):
	return int(text) if text.isdigit() else text

def natural_keys(text):
	'''
	alist.sort(key=natural_keys) sorts in human order
	http://nedbatchelder.com/blog/200712/human_sorting.html
	(See Toothy's implementation in the comments)
	'''
	return [ atoi(c) for c in re.split('(\d+)', text) ]


def build(root_dir, cb, act, verbose):
	with open(os.devnull, 'w') as FNULL:
		outlet = FNULL
		if verbose:
			outlet = None

		if act == "clean" or act == "rebuild":
			ret = subprocess.call(['make', '-C' , os.path.join(root_dir, cb), 'clean'], stderr=FNULL, stdout=FNULL)

			if act == "clean":
				return ret

		if os.path.isfile(os.path.join(root_dir, cb, 'pov')):
			subprocess.call(['rm', '-f' , os.path.join(root_dir, cb, 'pov')])
		ret = subprocess.call(['make', '-C' , os.path.join(root_dir, cb), 'build'], stderr=FNULL, stdout=FNULL)

	return ret

def main():
	parser = argparse.ArgumentParser(description="Building CGC challenges")

	parser.add_argument("--root_dir", required=True, help="The root directory of the corpus")
	parser.add_argument("--act", help="what to do", dest="act", action="store", default="build", choices=["build", "rebuild", "clean"])
	parser.add_argument("--regex", help="The regex to filer the challenges")
	parser.add_argument("--verbose", help="show the build log", action="store_true", default=False)
	parser.add_argument("--keep", help="Keep building in case of error", action="store_true", default=False)

	args = parser.parse_args()

	root_dir = os.path.abspath(args.root_dir)
	challenges = [x for x in os.listdir(root_dir) if
				  not os.path.isfile(os.path.join(root_dir, x)) and bool(re.match(r'[A-Z]*_[0-9]*', x))]

	challenges.sort(key=natural_keys)

	for cb in challenges:
		# if cb in ["CROMU_00021", "CROMU_00034", "CROMU_00047", "CROMU_00048", "CROMU_00046", "CROMU_00051", \
		# 		  "CROMU_00054", "CROMU_00055", "CROMU_00057", "CROMU_00061", "CROMU_00063", "CROMU_00064", \
		# 		  "CROMU_00066", "CROMU_00073", "CROMU_00073", "CROMU_00077", "CROMU_00078", "CROMU_00079", \
		# 		  "EAGLE_00005"]:
		# 	continue

		if cb in ["EAGLE_00005", "CROMU_00046"]:
			continue

		if args.regex != None and re.match(args.regex, cb):
			continue

		print "handling challenge " + cb

		ret = build(root_dir, cb, args.act, args.verbose)
		if ret != 0 and (args.act == "build" or args.act == "rebuild"):
			pov_c = glob.glob(os.path.join(root_dir, cb, 'pov*', '*.[ch]'))

			for pc in pov_c:
				pf = os.path.join(root_dir, cb)
				subprocess.call(['git', 'checkout', 'b65ebe8b956b2519a1407552a77d6f78748e11c8', '--', re.sub(pf+ '/', '', pc)], cwd=os.path.join(root_dir, cb))
				with open(os.devnull, 'w') as FNULL:
					subprocess.call(['../../replace_libpov.sh', re.sub(pf+ '/', '', pc)], cwd=os.path.join(root_dir, cb), stderr=FNULL, stdout=FNULL)

			with open(os.devnull, 'w') as FNULL:
				subprocess.call(['../replace.sh'], cwd=os.path.join(root_dir, cb), stderr=FNULL, stdout=FNULL)
				ret = build(root_dir, cb, args.act, args.verbose)

		if ret != 0:
			print cb + " Failed"
			if not args.keep:
				break

if __name__ == '__main__':
	main()
