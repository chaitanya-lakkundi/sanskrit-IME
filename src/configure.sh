#!/usr/bin/env bash

echo -e "First of all give a try to run SSTL_xx.run without installing any extra libraries\
\n If you find any problems install the following packages \n "

#sudo apt-get update
which apt-get
res=$?

if [ $res -eq 0 ];then
sudo apt-get install libxtst6 libx11-6 libx11-xcb1 x11-utils libxdmcp6 libc6 libxi6 libxau6 libxcb1
sudo apt-get install libxtst-dev libx11-dev libxdmcp-dev libc6-dev libxi-dev libxau-dev libxcb1-dev

sudo apt-get install sharutils zenity

else

echo -e "Please install \
\n libxtst6 libx11-6 libx11-xcb1 x11-utils libxdmcp6 libc6 libxi6 libxau6 libxcb1 \
\n libxtst-dev libx11-dev libxdmcp-dev libc6-dev libxi-dev libxau-dev libxcb1-dev \
\n sharutils zenity \
\n for your distribution "

fi
