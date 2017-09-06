#!/bin/bash
#$1 contains instances
#$2 contains output folder
#$3 maximum runtime
#$4 maximum wct
#$5 random seed
input="$1"
IFS=','
#rm -rf "$1/igs_$run.txt"


echo "instance,score,relative" >> "$2/igs_$4.txt"
while read -r instance best_solution
  do
      echo -e "$instance"
      short_instance=$(echo $instance| awk '{print substr($0,0,2)}')
      ./flowshopWCT "./instances/$instance" $best_solution $3 $4 $5 igs 2 0.7
     # if [[ "$short_instance" == 5 ]]
     #   then 
            #result=$( ./flowshopWCT "./instances/$instance" $best_solution $3 $4 $5 igs 2 0.7)
            #echo "$result"
            #echo "$instance,$result" >> "$2/igs_$4.txt"
      #  else
            #result=$( ./flowshopWCT "./instances/$instance" $best_solution $3 $4 $5 igs 2 0.7)
            #echo "$result"
            #echo "$instance,$result" >> "$2/igs_$4.txt"
     # fi
  done < "$input"
