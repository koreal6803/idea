#!/bin/bash

# --- check argument
if [ "$#" -ne 2 ]
then
	echo "please input argument"
	echo "argument 1 = circuit_name"
	echo "argument 2 = supply_voltage"
	exit 0
fi

# --- parse workspace and benchmark directory
benchDir=./script/workspace_setup.txt

# check file exist
if ! [ -f $benchDir ];
then
	echo "Can not find benchmark setup file: ".$benchDir
	exit 0;
fi

# parse file
exec < $benchDir
cnt=0
while read line; do 
	for word in $line; do
		array[$cnt]=$word;
		cnt=$cnt+1
	done
done


# --- get directories from workspace_setup.txt
benchmarkDir=${array[1]}
voltage=$2

# --- build command
cmd="./bin/release/idea_main.out 
"$benchmarkDir"/library/library.lib 
"$benchmarkDir"/$1/$1_routed.v 
"$benchmarkDir"/$1/$1_routed.sdf 
"$benchmarkDir"/$1/$1_routed.pat 
"$benchmarkDir"/$1/$1_routed.dspf 
"$benchmarkDir"/$1/$1_routed.spf 
"$voltage

# --- excute command
echo $cmd;
eval $cmd;
