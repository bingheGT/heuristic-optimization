#!/bin/bash
#$1 contains instances
#$2 contains output folder
#$3 random seed
#$4 destruction
#$5 temperature

input="$1"
IFS=','
#rm -rf "$1/igs_$run.txt"




while read -r instance best_solution maxrun maxwct
  echo "instance,time,score,relative" >> "$2/hga_$instance.txt"
 
  do
    for run in {1..25}
    do
          echo $instance,$run
      short_instance=$(echo $instance| awk '{print substr($0,0,2)}')
      if [[ "$short_instance" == 5 ]]
        then 
            result=$( ./flowshopWCT "./instances/$instance" $best_solution $3 $maxrun $maxwct genetic 50 0.8 0.2)
            echo "$result"
            echo "$instance,$result" >> "$2/hga_$instance.txt"
        else
            result=$( ./flowshopWCT "./instances/$instance" $best_solution $3 $maxrun $maxwct genetic 50 0.8 0.2)
            echo "$result"
            echo "$instance,$result" >> "$2/hga_$instance.txt"
      fi
    done
done < "$input"
