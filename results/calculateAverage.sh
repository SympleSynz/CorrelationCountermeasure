#!/bin/bash       
# test script for creating virtual network devices

currentDir="$(pwd)"
rawDir="$(pwd)/raw/"
averageDir="$(pwd)/average/"

gcc average.c -o averageCalc

cp -rf $rawDir/* $averageDir/

cd $averageDir

for Dir in `find . -mindepth 2 -type d`
do
	echo "cd $Dir"
	cd $Dir
	rm -f all.csv
	
	for f in *.out
	do
		echo "Processing $f"
		../../../averageCalc $f 300
		rm $f
	done
	
	cd $averageDir
	#cd ..
done
