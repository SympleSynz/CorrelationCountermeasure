#!/bin/bash       
# test script for creating virtual network devices
# should be run ./test.sh <ethernet Device> <number of virtual adapters>

CLICK_PROG=../click-2.0.1/click
CLICK_SCRIPTS_DIR=../click_scripts/
DISTRO_MIRROR_LIST=./distro.list
NETWORK_DEVICE=enp0s8
NUMBER_HOSTS=4
TEST_DURATION=5

VIRTUAL_SUBNET_PREFIX=19.19.19
VIRTUAL_MASK=255.255.255.0
CLICK_ADDRESS=254

RESULTS_DIR=../results/

PUSH_GIT=true


test_dir=$RESULTS_DIR$(date +%Y%m%d)
mkdir -p "$test_dir"
cd $test_dir
touch "testResults"
#$HOME/test.out

# Make virtual devices
printf "Setting network devices... \n\n"

echo ifconfig $NETWORK_DEVICE:0 $VIRTUAL_SUBNET.$CLICK_ADDRESS

counter=1
while [ $counter -le $NUMBER_HOSTS ]; do
	echo ifconfig $NETWORK_DEVICE:$((counter+20)) $VIRTUAL_SUBNET_PREFIX.$counter
	((counter++))
done

# Set route from all virtual network adapters to click adapter
printf "Setting routes... \n\n"



# run click
printf "starting click middlebox... \n\n"

echo $CLICK_PROG 

# start wget for all 50 devices
printf "Begining wget transfers... \n\n"

counter=1
while read line; do
	if [ $counter -gt $NUMBER_HOSTS ]; then
		break
	fi
	echo wget --bind-address=6.6.6.$counter $(echo $line | tr -d "\n\r") "&"
	((counter++))
done <$DISTRO_MIRROR_LIST

# wait 5 minutes
printf "Waiting for test to finish... \n\nTime Remaining:\n"

timer=$TEST_DURATION

while [ $timer -ge "0" ] ; do
	printf '\r%02d:%02d' $((timer/60)) $((timer%60))
	((timer--))
	sleep 1
done

# kill all wget processes
printf "\nStopping all wget transfers... \n\n"

echo pkill -9 wget

# remove added route


# shut down all virtual devices

counter=0
while [ $counter -le $NUMBER_HOSTS ]; do
	echo ifconfig $NETWORK_DEVICE:$counter down
	((counter++))
done

# push results to GitHub

if [ "$PUSH_GIT" = true ] ; then
	git add *
	git commit -m "Push most recent test results"
	git push
fi