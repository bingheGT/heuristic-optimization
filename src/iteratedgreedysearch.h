#ifndef _ITERATEDGREEDYSEARCH_H_
#define _ITERATEDGREEDYSEARCH_H_
#include "pfspinstance.h"
#include "solution.h"
#include "neighborhood.h"

#include <string>
#include <vector>
#include <list>
#include <chrono>
using namespace std;

class PfspInstance;
class Solution;


class SimpleSearch{
private:
	void Destruction(int d,Solution &next_solution);
    void Construction(int d,string neighborhood,Solution &next_solution);
    bool Accept(string pivoting,string neighborhood);
    long double acceptance_criteria(long int first_solution,long int second_solution,double t);
    int d;
    int alpha;
    int maximum_runtime;
    PfspInstance instance;
    Neighborhood neighborhood;
   // Solution best_solution;
    list<int> deleted_jobs;




    

public:
	SimpleSearch();
    ~SimpleSearch();
    SimpleSearch(PfspInstance& instance,int max_run):instance(instance),maximum_runtime(max_run){};
  	void IteratedGreedySearch(int d,double alpha,string neighborhood,Solution& curent_solution,std::chrono::time_point<std::chrono::system_clock> start_time,int max_wct);

};

#endif