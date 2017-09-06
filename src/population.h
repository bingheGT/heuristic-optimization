#ifndef _POPULATION_H_
#define _POPULATION_H_
#include "pfspinstance.h"
#include "solution.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>


using namespace std;

class PfspInstance;
class Solution;


class Population{
  private:


  public:
    Population(int population_size,PfspInstance &instance):instance(instance),size(population_size),population(population_size+1),population_wct(population_size+1){};
    Population();
    ~Population();
    PfspInstance instance;
    vector<Solution> population;
    vector<long int> population_wct;
    int size;
    long int minimum_wct;
    int best_parent;

    //The initial parents are sorted in a multimap according to their minimum WCT
    multimap<long int, int> sorted_parents;

    void initialize();
    void evaluate();

    int get_parent_job(int parent,int sequence);
    void set_parent(int solution_index,Solution& new_solution);
    Solution& get_best_parent();
    Solution& get_parent(int solution_index);
    long int get_minimum_wct();

};

#endif
