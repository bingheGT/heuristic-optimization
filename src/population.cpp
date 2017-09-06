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
#include <string>
#include <cstring>
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include "pfspinstance.h"
#include "solution.h"
#include "iteratedgreedysearch.h"
#include "population.h"
#include "neighborhood.h"
#include <chrono>
using namespace std;


Population::Population()
{
     /* initialize random seed: */
  //  srand ( time(NULL) );


}





Population::~Population()
{

}

Solution& Population::get_parent(int solution_index){

  return population[solution_index];
}


long int Population::get_minimum_wct(){

  return minimum_wct;
}

void Population::set_parent(int solution_index,Solution& new_solution){
  population[solution_index] = new_solution;
};

Solution& Population::get_best_parent(){
  return population[best_parent];
}

/* Evaluate the solutions in the population*/
void Population::evaluate(){
  minimum_wct = instance.computeWCT(population[1]);
  best_parent = 1;

  //The initial parents are sorted in a multimap according to their minimum WCT
  sorted_parents.insert(pair<long int, int>(minimum_wct,best_parent)); 
  
  for(int parent = 2; parent < population.size();parent++){
      // Calculate the WCT of the solution/parent/chromosome
      long int wct = instance.computeWCT(population[parent]);
      if(wct < minimum_wct){
      	minimum_wct = wct;
      	best_parent = parent;
      }
      //Insert the WCT value of the parent into a sorted map 
      sorted_parents.insert(pair<long int, int>(wct,parent)); 
  }
}

int Population::get_parent_job(int parent,int sequence){
	return population[parent].get_job(sequence);
}



/* Generate the initial population */
void Population::initialize(){
  /* Generate the first chromosome/solution according to the Liu and Reeves composite heuristic and calculate the WCT of the solution */
  Solution individual_chromosome(instance.getNbJob()+1);
  individual_chromosome.advanced_rz_heuristic(instance,instance.getNbJob());
  population[1] = individual_chromosome;

  /* Generate the other chromosomes randomly and calculate the WCT of the solution*/
  for(int parent = 2; parent <= size;parent++){
    Solution individual_chromosome(instance.getNbJob()+1);
    individual_chromosome.randomPermutation();
    population[parent] = individual_chromosome;
  }
}


