#!/bin/bash
#$1 contains instances
#$2 contains output folder
#$3 maximum runtime
#$4 maximum wct
#$5 random seed
input="$1"
IFS=','
#rm -rf "$1/igs_$run.txt"



run=$4
echo "instance,score,relative" >> "$2/igs_$run.txt"
while read -r instance best_solution
  do
      echo -e "$instance"
      ./flowshopWCT "./instances/$instance" $best_solution $3 $4 $5 genetic 50 0.8 0.2
      #short_instance=$(echo $instance| awk '{print substr($0,0,2)}')
      #if [[ "$short_instance" == 5 ]]
      #  then 
      #      result=$( ./flowshopWCT "./instances/$instance" $best_solution $3 $4 $5 genetic 50 0.8 0.2)
      #      echo "$result"
      #      echo "$instance,$result" >> "$2/igs_$run.txt"
      #  else
      #      result=$( ./flowshopWCT "./instances/$instance" $best_solution $3 $4 $5 genetic 50 0.8 0.2)
      #      echo "$result"
      #      echo "$instance,$result" >> "$2/igs_$run.txt"
      #fi
  done < "$input"