#!/bin/bash 

sudo ethtool --offload enp0s3 tso off
sudo ethtool --offload enp0s3 gso off
sudo ethtool --offload enp0s3 gro off