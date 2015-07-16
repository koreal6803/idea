#!/bin/bash

#check file
if [ "$#" -ne 2 ]
then
	echo "please input argument"
	echo "argument 1 = circuit_name"
	echo "argument 2 = supply_voltage"
	exit 0
fi

cmd="./bin/release/idea_main.out
../../newLib.lib
../../bench_stable_0528/$1/$1_routed.v
../../bench_stable_0528/$1/$1_routed.sdf
../../bench_stable_0528/$1/$1_routed.pat
../../bench_stable_0528/$1/$1_routed.dspf
../../bench_stable_0528/$1/$1_routed.spf
$2"
#cmd="./bin/release/idea_test.out
#../../newLib.lib
#../../bench/$1/$1.v
#../../bench/$1/$1.sdf
#../../bench/$1/$1.basicScan.pat
#../../bench/$1/$1.dspf
#../../bench/$1/$1.spf
#$2"

echo $cmd;
eval $cmd;
