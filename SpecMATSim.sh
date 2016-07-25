#!/bin/bash
./SpecMATSim SpecMATSim.in > SpecMATSim.out &
PID=$!
# Note: From http://stackoverflow.com/questions/12199631/convert-seconds-to-hours-minutes-seconds
# Functions
function showTime {
    num=$1
    min=0
    hour=0
    day=0
    if((num>59));then
        ((sec=num%60))
        ((num=num/60))
        if((num>59));then
            ((min=num%60))
            ((num=num/60))
            if((num>23));then
                ((hour=num%24))
                ((day=num/24))
            else
                ((hour=num))
            fi
        else
            ((min=num))
        fi
    else
        ((sec=num))
    fi
    #echo "$day"d "$hour"h "$min"m "$sec"s
}

#   Slick Progress Bar
#   Created by: Ian Brown (ijbrown@hotmail.com)
#   Please share with me your modifications
# Note: From http://stackoverflow.com/questions/11592583/bash-progress-bar
PUT(){ echo -en "\033[${1};${2}H";}
DRAW(){ echo -en "\033%";echo -en "\033(0";}
WRITE(){ echo -en "\033(B";}
HIDECURSOR(){ echo -en "\033[?25l";}
NORM(){ echo -en "\033[?12l\033[?25h";}
function showBar {
        percDone=$(echo 'scale=2;'$1/$2*100 | bc)
	    halfDone=$(echo $percDone/2 | bc) #I prefer a half sized bar graph
		barLen=$(echo ${percDone%'.00'})
		halfDone=`expr $halfDone + 6`
		tput bold
		PUT 7 26; printf "%01dd %02d:%02d:%02d" $day $hour $min $sec   #Print the percentage
		PUT 5 $halfDone;  echo -e "\033[7m \033[0m" #Draw the bar
        if(($1==0));then
            for (( k=7; k<=56; k++ ))
            do
                PUT 5 $k; echo -e "\033[0m \033[0m"
            done
        fi
        tput sgr0
}
# Start Script
clear
HIDECURSOR
echo -e ""
echo -e ""
DRAW    #magic starts here - must use caps in draw mode
echo -e "           PLEASE WAIT WHILE SIMULATION IS RUNNING"
echo -e "    lqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqk"
echo -e "    x                                                   x"
echo -e "    mqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqj"
WRITE
#
i=0
j=1
m=0
while kill -0 $PID 2> /dev/null
    do
        showTime $m
        showBar $i 50  #Call bar drawing function "showBar"
        sleep $j
        i=$(( ($i+$j) ))
        m=$(( ($m+$j) ))
        if(($i==51));then
            i=0
        fi
    done
#
PUT 8 24; printf "SIMULATION DONE"
PUT 10 12
echo -e ""
NORM

#Alternative progress bar:
#echo "THIS MAY TAKE A WHILE, PLEASE WAIT WHILE SIMULATION IS RUNNING..."
#printf "["
# While process is running...
#while kill -0 $PID 2> /dev/null
#do
#	printf  "▓"
#	sleep 1
#done
#printf "] done! \n"

#Simpler version of progress indicator
#spin[0]="-"
#spin[1]="\\"
#spin[2]="|"
#spin[3]="/"
#echo -n "[copying] ${spin[0]}"
#while [ kill -0 $PID ]
#do
#  for i in "${spin[@]}"
#  do
#        echo -ne "\b$i \n"
#        sleep 0.1
#  done
#done
