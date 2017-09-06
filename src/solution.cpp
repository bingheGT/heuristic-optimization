#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include "solution.h"
#include "pfspinstance.h"
#include "neighborhood.h"

using namespace std;

Solution::Solution()
{
     /* initialize random seed: */
  //  srand ( time(NULL) );


}





Solution::~Solution()
{

}


bool Solution::operator< (const Solution &other) const {
        return solution_WCT < other.solution_WCT;
}

void Solution::set_vector(){
  solution_vector = initial_solution_vector;
}


void Solution::set_next_vector(){
  next_solution = solution_vector;
}



long int Solution::evaluate_parent(PfspInstance& instance){
  solution_WCT = instance.computeWCT(*this);
  return solution_WCT;
}


void Solution::reset_next_vector(){
  solution_vector = next_solution;
}


void Solution::reset_vector(){
  solution_vector = temp;
}



int Solution::get_job(int i){
  return solution_vector[i];
}


int Solution::set_job(int i,int value){
  solution_vector[i] = value;
}

int Solution::set_job_negative(int i){
 solution_vector[i] = -solution_vector[i];
}

void Solution::insert_job(int pos,int job){
  solution_vector[pos] = job;
}

void Solution::remove_job(int pos){

  solution_vector.erase(solution_vector.begin()+pos);
}

void Solution::copy_solution_to_list(){
  std::copy( solution_vector.begin(), solution_vector.end(), std::back_inserter( solution_list ) );
}






int Solution::get_sum(){
  int sum = 0;
  for(int i = 0; i <= solution_vector.size();i++){
    sum += solution_vector[i];
  }
  return sum;
}


int Solution::get_size(){
  //cout << "SIZE " << n_jobs << " VECTOR SIZE " << solution_vector.size()-1 << endl;
  return solution_vector.size()-1;
}




void Solution::storage_move_right(int start,int end){
  for(int i = start; i < end;i++){
     move(i,i+1);
  }
}

void Solution::storage_move_right_while_positive(int start){
  for(int i = start; i < solution_vector.size()-1;i++){
    move(i,i+1);
    if(solution_vector[i] < 1 ){
      break;
     }
     
  }

}

void Solution::storage_move_left(int start,int end){

   if(end < 1 ){
    cout << "End index out of bound" << endl;
   }

   if(start > solution_vector.size()){
    cout << "Start index out of bound" << endl;
   }
    for(int k = start;k > end;k--){
            move(k-1,k);
    }
}




int Solution::generateRndPosition(int min, int max)
{
  return ( rand() % max + min );
}


/*Swap the value on index A with the value on index B*/
void Solution::move(int a,int b,int jobs){


 
  int temp = solution_vector[b];
  solution_vector[b] = solution_vector[a];
  solution_vector[a] = temp;
}


/* Print the solution */
void Solution::set_temp(){
  temp = solution_vector;
}

/* Print the solution */
void Solution::print_temp(){
  for(int i = 1;i <= n_jobs;i++){
    cout << temp[i];
    cout << " ";
  }
  cout << endl;
}

/* Print the solution */
void Solution::print_solution(){
  for(int i = 1;i < solution_vector.size();i++){
    cout << solution_vector[i];
    cout << " ";
  }
  cout << endl;
}


/* Fill the solution with numbers between 1 and nbJobs, shuffled */
void Solution::randomPermutation()
{
  
  vector<bool> alreadyTaken(n_jobs+1, false); // nbJobs elements with value false
  vector<int > choosenNumber(n_jobs+1, 0);

  int nbj;
  int rnd, i, j, nbFalse;

  nbj = 0;
  for (i = n_jobs; i >= 1; --i)
  {
    rnd = generateRndPosition(1, i);
    nbFalse = 0;

    /* find the rndth cell with value = false : */
    for (j = 1; nbFalse < rnd; ++j)
      if ( ! alreadyTaken[j] )
        ++nbFalse;
    --j;

    solution_vector[j] = i;

    ++nbj;
    choosenNumber[nbj] = j;

    alreadyTaken[j] = true;
  }
  initial_solution_vector = solution_vector;
}


bool Solution::same_vector(){
  int i;
  for(i = 1;i < solution_vector.size();i++){
    if(solution_vector[i] != temp[i]){
      break;
    }
  }
  return (i == solution_vector.size());
}
/* Fill the solution with numbers between 1 and nbJobs, shuffled */
void Solution::simplified_rz_heuristic(PfspInstance & instance)
{
  Neighborhood n;
  //calculate the total processing times and order them according to job
  //After the sorting this vector will contain the elements in sorted order
  instance.proc_times_ordered(*this);


  /*Initialize the first solution*/
  for(int i = 1;i <= 2;i++){
    set_job(i,instance.getProcessingTime(i));
  }

  int best = instance.computeWCT_until(*this,3);
  int value = instance.computeWCT_until(*this,3);

  //Start of with 2 starting solutions from the proccesingtimes and swap if necessary
  if(value <= best){
    best = value;
  }else{
    move(0,1);
  }

  int next_neighbor;
  int best_i = 0;
  int best_j = 1;

  for(int i = 3; i <= get_size();i++){
    //Move the item to the left starting from I (this is to make place for the item to be inserted)
    storage_move_left(i,1);
    //After the storage move to the left we can insert the item
    insert_job(1,instance.getProcessingTime(i));
    best = instance.computeWCT_until(*this,i);
    best_j = 1;
    //Find the best solution by using a computeWCT until the length of I
    for(int j = 1; j < i;j++){
      move(j,j+1);
      next_neighbor = instance.computeWCT_until(*this,i);
          if(next_neighbor < best){
            best = next_neighbor;
            best_j = j;
          }
    }
    /* Change the initial solution to the best found solution */

    if(best_j != (i-1)){
      storage_move_left(i,best_j);
    } 
   // n.best_insert_partial(instance,*this,i);
  }

}

  /************************************************************************************************ 
   * Construct a sequence *
   ************************************************************************************************/
double long Solution::construct_sequence(PfspInstance & instance,vector<pair <double,int> > &ranked_jobs,int ranked_job){

   //Initialize variables
   int best_j;
   double best_value;
   double value;

  //Create a temporary unscheduled vector to determine the next value in the sequence
  vector<double> unscheduled_vector(n_jobs+1);
  for(int i = 1; i <= n_jobs;i++){
      unscheduled_vector[i] = ranked_jobs[i].second;
  }
  unscheduled_vector.erase(unscheduled_vector.begin()+ranked_job); 


  //Build a scheduled vector sequence by appending each of the jobs in the unscheduled sequence which is sorted on the value of the index function
  for(int k = 1; k< n_jobs-1;k++){
    set_job(k+1,unscheduled_vector[1]);
    instance.computeWCT_until(*this,k+1);
    best_value = instance.index_function(k+1,k);
    best_j = 1;
      for(int j = 2;j < unscheduled_vector.size()-1;j++){
            set_job(k+1,unscheduled_vector[j]);
            instance.computeWCT_until(*this,k+1);
            value = instance.index_function(k+1,k);
            if(value < best_value){
              best_value = value; 
              best_j = j;
            }
      }
    //Add the best job to the scheduled sequence
    set_job(k+1,unscheduled_vector[best_j]);
    //Shrink the unscheduled vector by removing the best job from the sequence
    unscheduled_vector.erase(unscheduled_vector.begin()+best_j); 
  }

  //Set the last job  in the scheduled sequence
  set_job(n_jobs,unscheduled_vector[1]);

  return instance.computeWCT(*this);
}

void Solution::advanced_rz_heuristic(PfspInstance & instance,int x)
{

   int j = 1;
   int best_wct;
   int sequence_wct;

   vector<pair <double,int> > ranked_jobs(n_jobs+1);
   
  /************************************************************************************************ 
   * Step 1 : Rank the jobs according to ascending order of the index function with values F(i,0) *
   ************************************************************************************************/

  for(int i = 1; i <= n_jobs;i++){
      set_job(1,i);
      instance.computeWCT_until(*this,1);
      ranked_jobs[j].first = instance.index_function(1,0);
      ranked_jobs[j].second = j;
      j++;
  } 

  /******************************************************************************************************************************************************************* 
   * Step 2 : Use each of the first X ranked jobs as the first job in S and construct X sequences by selecting jobs one by one using the index function to append to S *
   ********************************************************************************************************************************************************************/

  //Append the first scheduled jobs to the empty unscheduled sequence and compute the initial best WCT, the temporary vector will always contain the sequence with the best solution at the time.
  set_job(1,ranked_jobs[1].second);
  best_wct = construct_sequence(instance,ranked_jobs,1);
  set_temp();
  

  for(int i = 2; i <= x;i++){
    //Add the first sorted job to the scheduled sequence
    set_job(1,ranked_jobs[i].second);
    sequence_wct = construct_sequence(instance,ranked_jobs,i);
    //Determine if the generated sequence is better then the current best
    if(sequence_wct  < best_wct){
      best_wct = sequence_wct;
      set_temp();
    }
  }

  /******************************************************************
   * Step 3 : Take the generated solution with the lowest WCT value *
   ******************************************************************/
  //The temporary solution vector will always contain the best solution so far, after determening each sequence we just set the solution vector equal to the temporary vector.
  solution_vector = temp;

  }















