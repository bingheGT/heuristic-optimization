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
#include <cstdlib>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include <string>
#include <cstring>
#include "solution.h"
#include "population.h"
#include "neighborhood.h"
#include "pfspinstance.h"
#include "iteratedgreedysearch.h"
#include "genetic.h"
#include "util.h"
#include <fstream>
#include <chrono>

using namespace std;




void removeCharsFromString( string &str, const char* charsToRemove ) {
   for ( unsigned int i = 0; i < strlen(charsToRemove); ++i ) {
      str.erase( remove(str.begin(), str.end(), charsToRemove[i]), str.end() );
   }
}

/***********************************************************************/

int main(int argc, char *argv[])
{

  PfspInstance instance;
  Util util;
 

  instance.readDataFromFile( argv[1]);
  long int best_WCT = atoi(argv[2]);
  long int max_runtime = atoi(argv[3]);
  long int max_wct = atoi(argv[4]);
  int random_seed = atoi(argv[5]);
  string algorithm = argv[6];



  //Remove the characters 
  removeCharsFromString(algorithm, "()-");

  //Read the instance
  Solution initial_solution(instance.getNbJob()+1);
  srand (random_seed);

  auto start = std::chrono::system_clock::now();

  if(algorithm == "igs"){
    int destruction_number = atoi(argv[7]);
    double temperature_probability = atol(argv[8]);
    SimpleSearch simplesearch(instance,max_runtime);
    simplesearch.IteratedGreedySearch(destruction_number,temperature_probability,"insert",initial_solution,start,max_wct);   
    //solution_score = instance.computeWCT(best_solution);                      
  }else if(algorithm == "genetic"){
    int population_size = atoi(argv[7]);
    double elite_probability = atol(argv[8]);
    double mutation_probability = atol(argv[9]);
    Genetic genetic(population_size,instance,max_runtime);
    genetic.search(elite_probability,mutation_probability,start,initial_solution,max_wct);
  
  }else if(algorithm =="vnd"){
    Neighborhood n;
    initial_solution.randomPermutation();
    n.choose_neighborhood("first","vnd2",instance,initial_solution);
  }

    
  long int solution_score = instance.computeWCT(initial_solution);
  //OUTPUT COMPUTATION TIME , SCORE, RELATIVE SCORE 

  cout << solution_score << ",";
  cout << 100*(double)(solution_score - best_WCT)/(double)best_WCT << endl;
 
  return 0;
}
