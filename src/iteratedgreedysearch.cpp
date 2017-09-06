/***************************************************************************
 *   Copyright (C) 2012 by Jérémie Dubois-Lacoste   *
 *   jeremie.dl@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include "pfspinstance.h"
#include "solution.h"
#include "util.h"
#include "iteratedgreedysearch.h"
#include <chrono>

using namespace std;




SimpleSearch::SimpleSearch(){
};

SimpleSearch::~SimpleSearch(){
};

void SimpleSearch::Destruction(int d,Solution &next_solution){
	int random_index;
	std::random_device rd;
  std::mt19937 gen(rd()); 

    for(int i = 1;i <= d;i++){
    	std::uniform_real_distribution<> dis(1, next_solution.get_size()+1-i);
    	random_index =  dis(gen);

      //Set the deleted job negative 
      next_solution.set_job_negative(random_index);
      //Move the deleted job to the back of the vector
      next_solution.storage_move_right(random_index,next_solution.get_size());

      //Add the job at the random index to the deleted_jobs list
      deleted_jobs.push_front(next_solution.get_job(random_index));
    }
};



void SimpleSearch::Construction(int d,string neighbor,Solution &next_solution){
    long double best;
    int best_j;
    int best_k;
    int next_neighbor;
    Neighborhood n;

    for(int k = next_solution.get_size()+1-d;k <= next_solution.get_size();k++){
      //Set the current job which is negative to positive
      next_solution.set_job_negative(k);
      best = instance.computeWCT_until(next_solution,k);
      best_j = k;

      for(int j = k;j > 1;j--){  
        //Move the candidate solution to the next candidate solution
        next_solution.move(j,j-1);

        next_neighbor = instance.computeWCT_until(next_solution,k);
        if(next_neighbor < best){
          best = next_neighbor;
          best_j = j;
          best_k = k;
        }
      }
      next_solution.storage_move_right(1,best_j);
      
      //Improve  the partial solution 
      n.best_insert_partial(instance,next_solution,k);
    
    }
};


void SimpleSearch::IteratedGreedySearch(int d,double alpha,string neighbor,Solution& current_solution,std::chrono::time_point<std::chrono::system_clock> start_time,int max_wct){
  //Initialize random device
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> random_probability(0,1);
  Util util;

  long int next_solution_wct;
  long int previous_solution_wct;
  long int best_wct;
  int count = 0;
  int best_execution_time = 0;


  //calculate the average proc timess on each machine and multiply this value with the alpha value
  double temperature = instance.average_processing_time(1,1) * alpha;

  Solution best;

  
  //Initialize the solution
  current_solution.advanced_rz_heuristic(instance,instance.getNbJob());

  //Improve the initialized solution with local search
  neighborhood.choose_neighborhood("best","insert",instance,current_solution);
  best_wct = instance.computeWCT(current_solution);

	 while(util.execution_time(start_time,std::chrono::system_clock::now()) < maximum_runtime && best_wct > max_wct){
    //Create the next solution and determine the WCT of the current best_solution
    Solution next_solution = current_solution;
    previous_solution_wct = instance.computeWCT(current_solution);

    //Create a new solution 
    Destruction(d,next_solution);
    Construction(d,neighbor,next_solution);
    
    //Further improve the current solution
    neighborhood.choose_neighborhood("best","insert",instance,next_solution);
    next_solution_wct = instance.computeWCT(next_solution);

    if(next_solution_wct < previous_solution_wct){
      current_solution = next_solution;
      if(next_solution_wct < best_wct){
        best_wct = next_solution_wct;
        best_execution_time =util.execution_time(start_time,std::chrono::system_clock::now());
        best = current_solution;
        cout << best_wct << endl;
        count = 0;
      }
    
    }else if((double)random_probability(gen) <=  (double)exp((double)(previous_solution_wct - next_solution_wct)/(double)temperature)){
      current_solution = next_solution;
    }
   }
   current_solution = best;
 //  cout << best_execution_time << ",";
};
