#!/bin/bash 

click_file=$(pwd)/click_scripts/router.click
click_program=$(pwd)/click-master/installdir/bin/

echo "Click must be run as sudo - Please press ctrl+c when test is complete"
sudo $click_program $click_file