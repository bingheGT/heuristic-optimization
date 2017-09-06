#ifndef _NEIGHBORHOOD_H_
#define _NEIGHBORHOOD_H_
#include "pfspinstance.h"
#include "solution.h"

#include <string>
#include <vector>


using namespace std;

class PfspInstance;
class Solution;


class Neighborhood{
private:
	bool best_transpose(PfspInstance& instance,Solution& sol);
	bool first_transpose(PfspInstance& instance,Solution& sol);

	bool best_exchange(PfspInstance& instance,Solution& sol);
	bool first_exchange(PfspInstance& instance,Solution& sol);

	
	bool first_insert(PfspInstance& instance,Solution& sol);


    

public:
    Neighborhood();
    ~Neighborhood();
    bool choose_neighborhood(string pivoting,string neighborhood,PfspInstance& instance,Solution& sol);
    void VariableNeighbourhoodDescent(string pivoting,vector<string> &neighborhood_order,PfspInstance& instance,Solution& sol);
   	bool best_insert(PfspInstance& instance,Solution& sol);
	bool best_insert_partial(PfspInstance& instance,Solution& sol,int until);



};

#endif