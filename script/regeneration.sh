#!/bin/bash

# --- check argument
if [ "$#" -ne 2 ]
then
	echo "please input argument"
	echo "argument 1 = circuit_name"
	echo "argument 2 = idea log"
	exit 0
fi

circuitName=$1
ideaLog=$2

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
bench=${array[1]}
workspace=${array[3]}

# build command
cmd="./bin/release/regeneration.out 
					../../library.v 
					"$bench"/"$circuitName"/"$circuitName"_routed.v 
					"$circuitName" 
					"$bench"/"$circuitName"/"$circuitName"_routed.wgl
					"$ideaLog" 
					"$workspace"
					" 

# --- excute command
echo $cmd;
eval $cmd;

