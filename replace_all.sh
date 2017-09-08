#!/bin/bash

for d in `ls .`; do
    if [ -d $d ]; then
	cd $d
	../replace.sh
	cd -
    fi
done
