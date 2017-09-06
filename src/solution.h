#ifndef _SOLUTION_H_
#define _SOLUTION_H_
#include "pfspinstance.h"


#include <string>
#include <vector>
#include <list>

using namespace std;

class PfspInstance;

class Solution{
private:
    
    int n_jobs;
    vector< int > solution_vector;
    vector< int > initial_solution_vector;
    vector< int > temp;
    list<int> solution_list;
    double long construct_sequence(PfspInstance & instance,vector<pair <double,int> > &ranked_jobs,int ranked_job);

    long int solution_WCT;

    //To sort the element based on the WCT of the solution
    bool operator< (const Solution &other) const;

    vector<int> next_solution;


    

    
  

  public:
    Solution();
    Solution(int jobs):n_jobs(jobs-1),solution_vector(jobs){};
    Solution(vector< int > solution_vector):solution_vector(solution_vector){};
    ~Solution();
    long int evaluate_parent(PfspInstance& instance);
   
 
    int generateRndPosition(int min, int max);
    
    

    //Job operations
    int get_job(int i);
    int set_job(int i,int value);
    int set_job_negative(int i);
    void insert_job(int pos,int job);
    void remove_job(int pos);

    void set_vector();
    void set_next_vector();
    void reset_vector();
    void reset_next_vector();
    
    int get_size();
    

    // Navigation 
    void storage_move_right(int start,int end);//Move item from left to right
    void storage_move_right_while_positive(int start);
    void storage_move_left(int start,int end);// Move item from right to left
    void move(int a,int b,int jobs = 0);//Swap items A and B
    void copy_solution_to_list();

    


    
    
    bool same_vector();
    
   
    void set_temp();
   

    //Initial Heuristics
    void randomPermutation();
    void simplified_rz_heuristic(PfspInstance & instance);
    void advanced_rz_heuristic(PfspInstance & instance,int x);


    //Test functions for debugging  
    int get_sum();
    void print_temp();
    void print_solution();



};

#endif

