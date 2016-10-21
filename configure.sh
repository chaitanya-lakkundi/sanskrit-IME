#!/usr/bin/env bash

which xdotool &> /dev/null && which zenity &> /dev/null
res=$?
DIST=$(lsb_release -i | cut -d: -f2 | tr -d "\t")

if [ $res -ne 0 ]; then
    if  [[ $DIST == "BOSS" ]] || [[ $DIST == "Debian" ]] || [[ $DIST == "LinuxMint" ]] || [[ $DIST == "Ubuntu" ]]; then    
    
    echo -e "sudo apt-get install xdotool zenity\n"
    sudo apt-get install xdotool zenity
    
    elif [[ $DIST == "Fedora" ]] || [[ $DIST == "RedHat" ]]; then

        if [ -f /usr/bin/dnf ]; then
            echo -e "sudo dnf -C install xdotool zenity\n"
            sudo dnf -C install xdotool zenity

        elif [ -f /usr/bin/yum ]; then
            echo -e "sudo yum install xdotool zenity\n"
            sudo yum install xdotool zenity
        
        fi
    
    else
    
    echo -e "\n\nPlease install xdotool and zenity for your distribution."
    
    fi
    
fi
#Dependencies Satisfied

chmod +x SSTL_32.run
chmod +x SSTL_64.run

ARCH=$(uname -m)

if [[ $ARCH == "x86_64" ]];then
    BIT=64
else
    BIT=32
fi

echo -e "All required packages found.\nYou may run ./SSTL_$BIT.run and type anywhere in देवनागरी. \n
Press F8 to disable SSTL. Again press F8 to enable.\n
Goto the terminal and press CTRL+C to quit the application.\n" | zenity --text-info --width=400 --height=400 --title="Tips"
