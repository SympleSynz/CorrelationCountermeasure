#!/bin/bash       
# test script for creating virtual network devices

# Test Settings
NETWORK_DEVICE=enp0s3
NUMBER_HOSTS=5
TEST_DURATION=15

# Address
VIRTUAL_SUBNET_PREFIX=19.19.19.
VIRTUAL_MASK=24
#CLICK_ADDRESS=254

# Path
#CLICK_PROG=../click-2.0.1/click
#CLICK_SCRIPTS_DIR=../click_scripts/
DISTRO_MIRROR_LIST=$(pwd)/distro.list
ISO_PATH="16.04/ubuntu-16.04-desktop-amd64.iso"
#RESULTS_DIR=../results/

# Options
GIT=false


# Make output directory and cd to output

#output_dir=$RESULTS_DIR$(date +%Y%m%d%H%M%S)
#mkdir -p "$output_dir"
#cd $output_dir

# Make virtual devices
printf "Setting network devices... \n\n"

#ifconfig $NETWORK_DEVICE:0 $VIRTUAL_SUBNET_PREFIX$CLICK_ADDRESS

counter=1
while [ $counter -le $NUMBER_HOSTS ]; do
	ifconfig $NETWORK_DEVICE:$counter $VIRTUAL_SUBNET_PREFIX$counter
	((counter++))
done

# start wget for all 50 devices
printf "Begining wget transfers... \n\n"

counter=1
while read line; do
	if [ $counter -gt $NUMBER_HOSTS ]; then
		break
	fi
	#echo wget --bind-address=6.6.6.$counter $(echo $line | tr -d "\n\r") "&"
	#--bind-address=$VIRTUAL_SUBNET_PREFIX$counter
	#echo --delete-after
	#-qr -e robots=off -O $counter.file      dists/yakkety/
	wget ""--bind-address=$VIRTUAL_SUBNET_PREFIX$counter --delete-after -qr -e robots=off -O $counter.file $(echo $line | tr -d "\n\r")$ISO_PATH 1>$counter.out 2>$counter.err"" &
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

#echo 
pkill -9 wget

# remove added route

#echo ip route del "$VIRTUAL_SUBNET_PREFIX"0/$VIRTUAL_MASK via "$VIRTUAL_SUBNET_PREFIX"$CLICK_ADDRESS
#ip route del "$VIRTUAL_SUBNET_PREFIX"0/$VIRTUAL_MASK via "$GATEWAY"

# shut down all virtual devices

counter=1
while [ $counter -le $NUMBER_HOSTS ]; do
	ifconfig $NETWORK_DEVICE:$counter down
	((counter++))
done

# push results to GitHub

if [ "$GIT" = true ] ; then
	git add *
	git commit -m "Push most recent test results"
	git push
fi