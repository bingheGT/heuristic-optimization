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


#ifndef _PFSPINSTANCEWT_H_
#define _PFSPINSTANCEWT_H_
#include "solution.h"

#include <string>
#include <vector>

using namespace std;



class Solution;

class PfspInstance{
  private:
    int nbJob;
    int nbMac;

    vector< long int > dueDates;
    vector< long int > priority;
    vector<pair <double,int> > proc_times;
    vector<vector< long int >> previousMachineEndTime;
    vector< long int > previousMachineStartTime;

    double weight_function(int j ,int k , int n ,int m);
    double weighted_total_machine_idle_time(int j, int k);


  




    


    

  public:
    PfspInstance();
    ~PfspInstance();


    double artificial_total_flow_time(int j, int k);
    double artificial_completion_time(int j,int k);


    /* Read write privates attributs : */
    int getNbJob();
    int getNbMac();
    int getProcessingTime(int job);
    int getNbInstances();
    string getInstance(int i);
    long int getBestSolution(int i);

    /* Allow the memory for the processing times matrix : */
    void allowMatrixMemory(int nbJ, int nbM);

    /* Read\Write values in the matrix : */
    long int getTime(int job, int machine);
    void setTime(int job, int machine, long int processTime);

    long int getDueDate(int job);
    void setDueDate(int job, int value);

    long int getPriority(int job);
    void setPriority(int job, int value);

    /* Read Data from a file : */
    bool readDataFromFile(char * fileName);

    bool readBestSolutionsFromFile();

    std::vector< std::vector <long int> > processingTimesMatrix;

    long int computeWCT (Solution & sol);
    long int computeWCT_until(Solution & sol,int length);
    long int computeWCT_from(Solution & sol,int from);
    long int computeWCT_from_until(Solution & sol,int from,int until);


    /* Return the processing times in order */
    void proc_times_ordered(Solution & sol);
    long int computeWCT_old(Solution &  sol);


    double index_function(int j , int k);
    double calculate_artificial_job(int j,int k,int m);

    //Used by the metropolitan acceptance criteria in the iterated greedy search algorithm
    long double average_processing_time(int starting_machine,int starting_job);

};

#endif
