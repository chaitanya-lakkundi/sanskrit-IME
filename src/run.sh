#!/usr/bin/env bash

opt=$1

function untar_payload()
{
	match=$(grep --text --line-number '^PAYLOAD:$' $0 | cut -d ':' -f 1)
	payload_start=$((match + 1))
	tail -n +$payload_start $0 | tar -xzvf -
}


while [ 1 ]
do

case $opt in

-s)

path=$(ls -al /dev/input/by-path | grep kbd | cut -d'>' -f2 | tr -d " ")
qual_path="/dev/input/by-path/$path"

loop=1
while [ $loop -ne 0 ]; do

user_path=$(zenity --entry --text "Path to keyboard dev file. [Default : $qual_path")

if [ -e $user_path ];then
    loop=0        
fi

done

if [ $user_path ];then
qual_path=$user_path
fi

zenity --info --text "File selected : $qual_path"
echo qual_path=$qual_path > .settings

mode=$(zenity --list --title "Select Mode" --text "Please click on you preferred mode " --column Mode --column Description 0 "Quick Mode" 1 "Stable Mode (default)")
if ! [ $mode ];then
    mode=1
fi

echo mode=$mode >> .settings
break   #break out of main loop
;;

-h)
echo "Sulabha Samskrita Tankana Lekhani - Help"
echo "Usage: ./SSTL.run [ options ]"
echo -e "\t -s \t Configure settings"
echo -e "\t -h \t Display this help "
break
;;

*)  #without valid arguments

if ! [ -f ./.settings ];then
    opt=-s
    echo ".settings file does not exist"
else 
    source ./.settings
    if [ -z $mode ] || [ -z $qual_path ] || ! [ -c "$qual_path" ];then
    opt=-s
    echo ".settings file exists, but values are corrupted"
    continue    #continue to strict .settings option
    fi
    
echo "executing"
break   #break out of main loop
fi

;;
esac

done    #end of main loop

untar_payload
sudo ./.SSTL.out $mode $qual_path
exit 0
