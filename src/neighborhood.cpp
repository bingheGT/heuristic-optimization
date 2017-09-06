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
#include "population.h"
#include "neighborhood.h"
using namespace std;



Neighborhood::Neighborhood()
{
}


Neighborhood::~Neighborhood()
{
}


// Search every possible combination in the neighborhood and perform the best solution based on the transpose neighborhood
bool Neighborhood::best_transpose(PfspInstance& instance,Solution& sol){
  //Initialize variables 
  bool improvement = false;
  long int best = instance.computeWCT(sol);
  long int next_neighbor;
  int best_it;


  for(int it = sol.get_size();it > 1;it--){
    // Move the initial solution to the first candidate solution
    sol.move(it-1,it);
    next_neighbor = instance.computeWCT_from(sol,it-1); 

    //Determine if a better solution can be found
    if(next_neighbor < best){
      best = next_neighbor;
      best_it = it;
      improvement = true;
    }

    //Reset the first candidate solution to the initial solution
    sol.move(it-1,it);
  }

  // Move the initial solution to the best solution found
  if(improvement){
      sol.move(best_it-1,best_it);
  }

  return improvement;
}

//perform the first improving neighborhood based on the transpose neighborhood
bool Neighborhood::first_transpose(PfspInstance& instance,Solution& sol){
  //Initialize variables 
  bool improvement = false;
  long int best = instance.computeWCT(sol);
  long int next_neighbor;
  int best_it;

  for(int it = sol.get_size();it > 1;it--){
    // Move the initial solution to the first candidate solution
    sol.move(it-1,it);
    next_neighbor = instance.computeWCT_from(sol,it-1); 

    //Determine if a better solution can be found
    if(next_neighbor < best){
      best = next_neighbor;
      best_it = it;
      improvement = true;
      break;
    }

    //Reset the first candidate solution to the initial solution
    sol.move(it-1,it);
  }
  return improvement;
}

// Search every possible combination in the neighborhood and perform the best solution based on the exchange neighborhood
bool Neighborhood::best_exchange(PfspInstance& instance,Solution& sol){
  // Initialize variables
  bool improvement = false;
  long int best = instance.computeWCT(sol);
  long int next_neighbor;
  int best_it = 1;
  int best_j = best_it + 1; 

  for(int it = 1;it < instance.getNbJob();it++){
    for(int j = it+1; j <= instance.getNbJob();j++){


      // Move the initial solution to the first candidate solution
      sol.move(it,j); 
      next_neighbor = instance.computeWCT_from(sol,it-1);

      if(next_neighbor < best){
        best = next_neighbor;
        best_it = it;
        best_j = j;
        improvement = true;
      }
    //Reset the first candidate solution to the initial solution
    sol.move(it,j);/*reset the candidate*/
    }
  }

  // Move the initial solution to the best solution found
  if(improvement){
    sol.move(best_it,best_j);/*Compute candidate neighbour*/
  }

return improvement;
}


bool Neighborhood::first_exchange(PfspInstance& instance,Solution& sol){
  // Initialize variables
  bool improvement = false;
  long int best = instance.computeWCT(sol);
  long int next_neighbor;
  int best_it = 1;
  int best_j = best_it + 1;
  for(int it = 1;it < instance.getNbJob();it++){
    for(int j = it+1; j <= instance.getNbJob();j++){

      // Move the initial solution to the first candidate solution
      sol.move(it,j);
      next_neighbor = instance.computeWCT_from(sol,it-1);

      //cout << next_neighbor << endl;
      if(next_neighbor < best){
        best = next_neighbor;
        best_it = it;
        best_j = j;
        improvement = true;
        goto end;
      }

    //Reset the first candidate solution to the initial solution
    sol.move(it,j);/*reset the candidate*/
    }
  }
  end:;
  return improvement;
}






  
  
// 
bool Neighborhood::first_insert(PfspInstance& instance,Solution& sol){
  /*Intialize variables*/
  bool improvement = false;
  int length = instance.getNbJob()-1;
  long int best = instance.computeWCT(sol);
  long int next_neighbor;
  int best_it = 1;
  int best_j = 0;

  for(int it = 1; it < instance.getNbJob();it++){
    //Initialize the first candidate solution by moving the inserted element to the back of the vector
    sol.storage_move_right(it,instance.getNbJob());

    //Compute the initial candidate solution
    instance.computeWCT_from(sol,1);



    for(int j = instance.getNbJob();j > 1;j--){  
      //Move the candidate solution to the next candidate solution
      sol.move(j,j-1);
      next_neighbor = instance.computeWCT_from(sol,j-1);
      if(next_neighbor < best){
        best = next_neighbor;
        best_j = j;
        best_it = it;
        improvement = true;
        goto end;
      }
    }
    //Reset to initial condition
    sol.storage_move_right(1,it);
  }

  end:;
 return improvement;
}


// Insert into a partial solution 
bool Neighborhood::best_insert_partial(PfspInstance& instance,Solution& sol,int until)
{
  /*Intialize variables*/
  bool improvement = false;
  int length = instance.getNbJob()-1;
  long int best = instance.computeWCT_until(sol,until);
  long int next_neighbor;
  int best_j = 0;


  for(int j = until;j > 1;j--){  
      //Move the candidate solution to the next candidate solution
      sol.move(j,j-1);
      next_neighbor = instance.computeWCT_from_until(sol,j,until);
      if(next_neighbor < best){
        best = next_neighbor;
        best_j = j;
        improvement = true;
      }
  }

  if(improvement){
    sol.storage_move_right(1,best_j);
  }

  return improvement;
}

  
// Search every possible combination in the neighborhood and perform the best solution based on the insert neighborhood
bool Neighborhood::best_insert(PfspInstance& instance,Solution& sol)
{
  /*Intialize variables*/
  bool improvement = false;
  int length = instance.getNbJob()-1;
  long int best = instance.computeWCT(sol);
  long int next_neighbor;
  int best_it = 1;
  int best_j = 0;

  for(int it = 1; it < instance.getNbJob();it++){
    //Initialize the first candidate solution by moving the inserted element to the back of the vector
    sol.storage_move_right(it,instance.getNbJob());

    //Compute the initial candidate solution
    instance.computeWCT_from(sol,1);



    for(int j = instance.getNbJob();j > 1;j--){  
      //Move the candidate solution to the next candidate solution
      sol.move(j,j-1);
      next_neighbor = instance.computeWCT_from(sol,j-1);
      if(next_neighbor < best){
        best = next_neighbor;
        best_j = j;
        best_it = it;
        improvement = true;
      }
    }
    //Reset to initial condition
    sol.storage_move_right(1,it);
  }


  if(improvement){
    (best_j > best_it ) ?  sol.storage_move_right(best_it,best_j-1) : sol.storage_move_left(best_it,best_j-1);
  }

  return improvement;
}



void Neighborhood::VariableNeighbourhoodDescent(string pivoting,vector<string> &neighborhood_order,PfspInstance& instance,Solution& sol){
  int i = 0;
  int k = 2;
  bool improvement;
  while(i < neighborhood_order.size()){
    improvement = choose_neighborhood(pivoting,neighborhood_order[i],instance,sol);
    if(improvement){
      i = 0;
    }else{
      i+=1;
    }
  }
}


bool Neighborhood::choose_neighborhood(string pivoting,string neighborhood,PfspInstance& instance,Solution& sol){

    if(neighborhood == "transpose"){
        if(pivoting == "first"){
          return first_transpose(instance,sol);
        }else{
          return best_transpose(instance,sol);}
    }
    else if(neighborhood == "exchange"){
        if(pivoting == "first"){
          return first_exchange(instance,sol);
        }
        else{
          return best_exchange(instance,sol);
        }
    }
    else if(neighborhood == "vnd1"){
        vector< string > neighborhood_order = {"transpose","exchange","insert"};
        VariableNeighbourhoodDescent(pivoting,neighborhood_order,instance,sol);
    }
    else if(neighborhood == "vnd2"){
        vector< string > neighborhood_order = {"transpose","insert","exchange"};
        VariableNeighbourhoodDescent(pivoting,neighborhood_order,instance,sol);
    
    }     
    else  {
        if(pivoting == "first"){
          return first_insert(instance,sol);}
        else{
          return best_insert(instance,sol);}
    }
}




