#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include "pfspinstance.h"
#include "solution.h"
#include "population.h"
#include "genetic.h"
#include "neighborhood.h"
#include <queue>


using namespace std;


//Hyperparameters




Genetic::Genetic()
{
}


Genetic::~Genetic()
{
}

Util util;

//Finds the best job and sequence in the dominance matrix and sets the rows & columns of the jobs and sequences to 0

void Genetic::find_best_job(vector<int> &artificial_chromosome){
  int best_job = 0;
  int best_sequence= 0;
  int max = -1;

  //Find the best job in the dominance matrix
  for(int job = 1; job < dominance_matrix.size();job++){
    for(int sequence = 1;sequence <= instance.getNbJob();sequence++){
      if(dominance_matrix[job][sequence] > max){
        max = dominance_matrix[job][sequence];
        best_job = job;
        best_sequence = sequence;
      }
    }
  }


  //Add the best job to the artificial chromosome sequence
  artificial_chromosome[best_sequence] = best_job;

  //reset the rows of the best found solution
  for(int i = 1; i < dominance_matrix.size();i++){
    dominance_matrix[i][best_sequence] = -1;
  }

   //reset the column of the best found solution
  for(int i = 1; i < dominance_matrix.size();i++){
    dominance_matrix[best_job][i] = -1;
  }

}



void Genetic::generate_artificial_chromosome(Population &population,vector<int> &dominance_matrix_weights,vector<int> &artificial_chromosome){
  int best_job = 1;
  int best_sequence= 1;
  int max = 0;
  
  create_dominance_matrix(population,instance.getNbJob(),dominance_matrix_weights);

  //find_best_job(artificial_chromosome);
  for(int i = 1; i < artificial_chromosome.size();i++){
    find_best_job(artificial_chromosome);
  }
}


/* Allow the memory for the dominance matrix : */

void Genetic::allowMatrixMemory(int nbJob){
  dominance_matrix.resize(nbJob+1); // 1st dimension
  artificial_chromosome.resize(nbJob+1);
  for (int cpt = 0; cpt < nbJob+1; ++cpt){
    dominance_matrix[cpt].resize(nbJob+1); // 2nd dimension
  }
}




void Genetic::create_dominance_matrix(Population &population,int nbJob,vector<int> &elite_chromosomes){
  for(int job = 1; job < population.size;job++){
    for(int sequence = 1;sequence <= nbJob;sequence++){
      int get_job = population.get_parent_job(job,sequence);
      dominance_matrix[get_job][sequence] += elite_chromosomes[sequence];
    }
  }
}

//Select the elite chromosomes
void Genetic::elite_selection(Population &population,vector<int>& dominance_matrix_weights){
  std::multimap<long int,int>::iterator it;
  int rank = population.size;


  //Build the weights/ranks used in the dominance matrix , if the chromosome is NOT elite then give it the lowest priority of rank 1
 for(it = population.sorted_parents.begin(); it!=population.sorted_parents.end();it++){
    dominance_matrix_weights[(*it).second] = (rank <= population.sorted_parents.size()) ? rank : 1;
    rank--;
  } 
}

void Genetic::crossover_operator(Solution& parent1,Solution& parent2,Solution& child,vector<int> &artificial_chromosomes,int crossover_probability){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> random_job_distribution(1,artificial_chromosomes.size()-1);
  int cross_over_point = random_job_distribution(gen);

  

  for(int i = 1; i < artificial_chromosomes.size();i++){
    //Search the common jobs between both parents and the artificial jobs ->  CJ(father,mother) , CJ(father,artificial) , CJ(mother,artificial)

    int parent1_job = parent1.get_job(i);
    int parent2_job = parent1.get_job(i);

    if(parent1_job == parent2_job || parent1_job == artificial_chromosomes[i] || parent2_job == artificial_chromosomes[i]){
      child.set_job(i,parent1_job);
    }else{
      if(i < cross_over_point){
      child.set_job(i,parent1_job);
      }else{
      child.set_job(i,parent2_job);
      }
    }
  }

}

void Genetic::mutation(Solution& child,double mutation_probability){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> random_job_distribution(1,child.get_size()+1);
  int random_job = random_job_distribution(gen);
  int random_position = random_job_distribution(gen);
  std::uniform_real_distribution<> random_mutation(0,1);

  //Randomly move children in the population from left or right
  if(random_mutation(gen) < mutation_probability){
    if(random_job > random_position){
      child.storage_move_left(random_job,random_position);

    }else{
      child.storage_move_right(random_job,random_position);
    }
  }
}




void Genetic::search(double elite_probability,double mutation_probability,std::chrono::time_point<std::chrono::system_clock> start_time,Solution& best_solution,int max_wct){
  //Initialize The random generator
  std::random_device rd;
  std::mt19937 gen(rd());

  //Initialize variables and create datastructures

  int best_execution_time = 0;
  int crossover_probability = 1;
  long int best_wct;
  int number_elite_chromosomes = elite_probability * (population_size+1);
  vector<int> dominance_matrix_weights(population_size+1);
  vector<int> artificial_chromosomes(instance.getNbJob()+1);
  allowMatrixMemory(instance.getNbJob());

  //Initialize population && generate random distribution
  Population population(population_size,instance);
  std::uniform_real_distribution<> dis(1,population.size);



  // Generate the initial population with 1 random solution and the other with the advanced initial heuristic 
  population.initialize();

  // Generate the initial population 
  population.evaluate();
  best_wct = population.get_minimum_wct();
  best_solution = population.get_best_parent();

  //Select the elite chromosomes and build the weights used in the dominance matrix
  elite_selection(population,dominance_matrix_weights);

  //Generate the artificial chromosomes 
  generate_artificial_chromosome(population,dominance_matrix_weights,artificial_chromosomes);


  while(util.execution_time(start_time,std::chrono::system_clock::now()) < maximum_runtime && best_wct > max_wct){
    //Create offspring population without any chromosome
    Population offspring_population(population_size,instance);

    for(int i = 1; i < population_size;i = i + 2){

        Solution father = population.get_parent(dis(gen));
        Solution mother = population.get_parent(dis(gen));
        Solution child1(artificial_chromosomes.size());
        Solution child2(artificial_chromosomes.size());
        crossover_operator(father,mother,child1,artificial_chromosomes,crossover_probability);
        crossover_operator(mother,father,child2,artificial_chromosomes,crossover_probability);

        mutation(child1,mutation_probability);
        mutation(child2,mutation_probability);
        Neighborhood neighborhood;

        //Local search improve with exchange and insert on both childeren 
        neighborhood.choose_neighborhood("best", "insert", instance,child1);
        neighborhood.choose_neighborhood("best", "exchange", instance,child1);
        neighborhood.choose_neighborhood("best", "insert", instance,child2);
        neighborhood.choose_neighborhood("best", "exchange", instance,child2); 

        //Add to offspring population      
        offspring_population.set_parent(i,child1);
        offspring_population.set_parent(i+1,child2);
   }

     //Current population is now equal to the offspring population 

     population = offspring_population;

     //Evaluate the new population as in the first step
     population.evaluate();
     elite_selection(population,dominance_matrix_weights);
     generate_artificial_chromosome(population,dominance_matrix_weights,artificial_chromosomes);

     
   if(population.get_minimum_wct() < best_wct){
    best_wct = population.minimum_wct;
    cout << best_wct << endl;
    best_solution = population.get_best_parent();
    best_execution_time =util.execution_time(start_time,std::chrono::system_clock::now());

   }
  }

//  cout << best_execution_time << ",";



}


