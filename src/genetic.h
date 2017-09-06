#ifndef _GENETIC_H_
#define _GENETIC_H_
#include "pfspinstance.h"
#include "solution.h"
#include "population.h"
#include "util.h"
#include<map>

#include <string>
#include <vector>
#include <chrono>

using namespace std;

class PfspInstance;
class Solution;
class Population;




class Genetic{
private:
	
	PfspInstance instance;
	int population_size;

	int maximum_runtime;

	std::chrono::time_point<std::chrono::system_clock> start_time;
//	vector<Solution> population;
	vector<vector< long int >>  dominance_matrix;

	vector<int> artificial_chromosome;

	void initialize_population();
    void evaluate_population(multimap<long int, int> &elite_chromosomes,Population &population);
	void elite_selection(Population &population,vector<int>& dominance_matrix_weights);
    void create_dominance_matrix(Population &population,int nbJob,vector<int> &elite_chromosomes);
    void generate_artificial_chromosome(Population &population,vector<int> &dominance_matrix_weights,vector<int> &artificial_chromosome);

	void find_best_job(vector<int> &artificial_chromosome);
	void crossover_operator(Solution &parent1,Solution &parent2,Solution &child,vector<int> &artificial_chromosomes,int crossover_probability);

	void mutation(Solution& child,double mutation_probability);
	
	void allowMatrixMemory(int nbJob);
	




    

public:
    Genetic(int population_size,PfspInstance& instance,int max_runtime):population_size(population_size),instance(instance),maximum_runtime(max_runtime){};
    Genetic();
    void search(double e,double mutation_probability,std::chrono::time_point<std::chrono::system_clock> start_time,Solution& curent_solution,int max_wct);


    ~Genetic();



};

#endif