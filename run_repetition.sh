#!/bin/bash
#$1 contains instances
#$2 contains output folder
#$3 random seed
#$4 destruction
#$5 temperature

input="$1"
IFS=','
#rm -rf "$1/igs_$run.txt"



run=$5

while read -r instance best_solution maxrun maxwct
  echo "instance,time,score,relative" >> "$2/igs_$instance.txt"
  echo $instance,$run
  do
    for run in {1..25}
    do
      short_instance=$(echo $instance| awk '{print substr($0,0,2)}')
      echo $run,$maxrun,$maxwct
      if [[ "$short_instance" == 5 ]]
        then 
            result=$( ./flowshopWCT "./instances/$instance" $best_solution $3*$run $maxrun $maxwct igs 2 0.7)
            echo "$result"
            echo "$instance,$result" >> "$2/igs_$instance.txt"
        else
            result=$( ./flowshopWCT "./instances/$instance" $best_solution $3*$run $maxrun $maxwct igs 2 0.7)
            echo "$result"
            echo "$instance,$result" >> "$2/igs_$instance.txt"
      fi
    done
done < "$input"


