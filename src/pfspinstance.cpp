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
using namespace std;



PfspInstance::PfspInstance()
{


}


PfspInstance::~PfspInstance()
{
}

int PfspInstance::getProcessingTime(int job)
{
  return proc_times[job].second;
}

int PfspInstance::getNbJob()
{
  return nbJob;
}

int PfspInstance::getNbMac()
{
  return nbMac;
}





/* Allow the memory for the processing times matrix : */
void PfspInstance::allowMatrixMemory(int nbJ, int nbM)
{
  processingTimesMatrix.resize(nbJ+1); 
  previousMachineEndTime.resize(nbJ+1);
           // 1st dimension

  for (int cpt = 0; cpt < nbJ+1; ++cpt){
  	processingTimesMatrix[cpt].resize(nbM+1); // 2nd dimension
 	previousMachineEndTime[cpt].resize(nbM+1);
  }


	dueDates.resize(nbJ+1);
	priority.resize(nbJ+1);
	proc_times.resize(nbJ+1);
}


long int PfspInstance::getTime(int job, int machine)
{
  if (job == 0)
    return 0;
  else
  {
    if ((job < 1) || (job > nbJob) || (machine < 1) || (machine > nbMac))
      cout    << "ERROR. file:pfspInstance.cpp, method:getTime. Out of bound. job=" << job
          << ", machine=" << machine << std::endl;

    return processingTimesMatrix[job][machine];
  }
}


/* Read the instance from file : */
bool PfspInstance::readDataFromFile(char * fileName)
{
	bool everythingOK = true;
	int j, m; // iterators
	long int readValue;
	string str;
	ifstream fileIn;
	char * aux2;
	char fileNameOK[100] = "";

	aux2 = (strrchr(fileName, '/'));

	if (aux2 == NULL)
		aux2 = fileName;
	else
		aux2 += 1;

	strcat(fileNameOK, aux2);

	fileIn.open(fileName);

	if ( fileIn.is_open() ) {
      //  cout << "File " << fileName << " is now open, start to read..." << std::endl;

		fileIn >> nbJob;
   //     cout << "Number of jobs : " << nbJob << std::endl;
		fileIn >> nbMac;
  //      cout << "Number of machines : " << nbMac << std::endl;

		allowMatrixMemory(nbJob, nbMac);

		for (j = 1; j <= nbJob; ++j)
		{
			for (m = 1; m <= nbMac; ++m)
			{
				fileIn >> readValue; // The number of each machine, not important !
				fileIn >> readValue; // Process Time

				processingTimesMatrix[j][m] = readValue;
			}
		}


        fileIn >> str; // this is not read
 

		for (j = 1; j <= nbJob; ++j)
		{
			fileIn >> readValue; // -1
			fileIn >> readValue;
			dueDates[j] = readValue;
			fileIn >> readValue; // -1
			fileIn >> readValue;
            priority[j] = readValue;
		}

       // cout << "All is read from file." << std::endl;
		fileIn.close();



	}
	else
	{
		cout    << "ERROR. file:pfspInstance.cpp, method:readDataFromFile, "
				<< "error while opening file " << fileName << std::endl;

		everythingOK = false;
	}

	return everythingOK;
}

//Compute the weighted tardiness until a certain endpoint
long int PfspInstance::computeWCT_until(Solution & sol,int length)
{
	int j, m;
	int jobNumber;
	long int wct;


	for ( m = 1; m <= nbMac; ++m )
	{
		previousMachineEndTime[1][m] =
				processingTimesMatrix[sol.get_job(1)][m] + previousMachineEndTime[1][m-1] ;

		for ( j = 2; j <= length; ++j )
		{

			jobNumber = sol.get_job(j);
			previousMachineEndTime[j][m] = processingTimesMatrix[jobNumber][m] + max(previousMachineEndTime[j][m-1],previousMachineEndTime[j-1][m]);

		}

	}

	wct = 0;
	for ( j = 1; j<= nbJob; ++j ){
	    wct += previousMachineEndTime[j][nbMac]  * priority[sol.get_job(j)];
	}

	return wct;
}




/* Compute the weighted tardiness of a given solution */
//The previousmachinendtimes are stored in a lookup table for the compute_WCT_from method
long int PfspInstance::computeWCT(Solution & sol)
{
	int j, m;
	int jobNumber;
	long int wct;


	for ( m = 1; m <= nbMac; ++m )
	{
		previousMachineEndTime[1][m] =
				processingTimesMatrix[sol.get_job(1)][m] + previousMachineEndTime[1][m-1] ;
				//cout << processingTimesMatrix[1][m] << " ";
		for ( j = 2; j <= nbJob; ++j )
		{

			jobNumber = sol.get_job(j);
			previousMachineEndTime[j][m] = processingTimesMatrix[jobNumber][m] + max(previousMachineEndTime[j][m-1],previousMachineEndTime[j-1][m]);
		//	cout << processingTimesMatrix[j][m] << " ";

		}
		//	cout << endl;
	}
	//cout << endl;

	wct = 0;
	for ( j = 1; j<= nbJob; ++j ){
	    wct += previousMachineEndTime[j][nbMac]  * priority[sol.get_job(j)];
	}

	return wct;
}


/* Compute the weighted tardiness of a given solution starting from a certain point */
long int PfspInstance::computeWCT_from(Solution & sol,int from)
{
	int j, m;
	int jobNumber;
	long int wct;
	long int previousJobEndTime=0;


//When the starting point < 2 we can compute the standard WCT
   if(from < 2){
   	return computeWCT(sol);
   }


	for ( m = 1; m <= nbMac; ++m )
	{
		previousMachineEndTime[1][m] =
				processingTimesMatrix[sol.get_job(1)][m] + previousMachineEndTime[1][m-1] ;


		for ( j = from; j <= nbJob; ++j )
		{

			jobNumber = sol.get_job(j);
			previousMachineEndTime[j][m] = processingTimesMatrix[jobNumber][m] + max(previousMachineEndTime[j][m-1],previousMachineEndTime[j-1][m]);
		}

	}


	wct = 0;
	for ( j = 1; j<= nbJob; ++j ){

	    wct += previousMachineEndTime[j][nbMac]  * priority[sol.get_job(j)];
	}


	return wct;
}


/* Compute the weighted tardiness of a given solution between a certain range*/
long int PfspInstance::computeWCT_from_until(Solution & sol,int from,int until)
{
	int j, m;
	int jobNumber;
	long int wct;
	long int previousJobEndTime=0;


//When the starting point < 2 we can compute the standard WCT
   if(from < 2){
   	return computeWCT(sol);
   }


	for ( m = 1; m <= nbMac; ++m )
	{
		previousMachineEndTime[1][m] =
				processingTimesMatrix[sol.get_job(1)][m] + previousMachineEndTime[1][m-1] ;


		for ( j = from; j <= until; ++j )
		{

			jobNumber = sol.get_job(j);
			previousMachineEndTime[j][m] = processingTimesMatrix[jobNumber][m] + max(previousMachineEndTime[j][m-1],previousMachineEndTime[j-1][m]);
		}

	}


	wct = 0;
	for ( j = 1; j<= nbJob; ++j ){

	    wct += previousMachineEndTime[j][nbMac]  * priority[sol.get_job(j)];
	}


	return wct;
}


//The artificial job value is the average processing times of the remaining jobs in the unscheduled sequence
long double PfspInstance::average_processing_time(int starting_machine,int starting_job){
	long double sum = 0;
	long double sum_priority = 0;
	for (int m = starting_machine; m <= nbMac;m++)
	{

		for (int j = starting_job;j <= nbJob;j++)
		{
			sum += processingTimesMatrix[j][m];
		}
	}

	for(int i = 1; i <= nbJob;i++){
		sum_priority += priority[i];
	}
	sum = sum * sum_priority; //To compensate with the weights in computeWCT
	int temp = nbMac * nbJob*10;
	return sum / double(temp);
}


//The artificial job value is the average processing times of the remaining jobs in the unscheduled sequence
double PfspInstance::calculate_artificial_job(int j,int k,int m){
	int sum = 0;
	double denominator = nbJob - k - 1;

		for (int j = k; j <= nbJob; ++j )
		{
			sum += processingTimesMatrix[j][m];
		}
	return sum /(double) denominator;
}


double PfspInstance::artificial_total_flow_time(int j, int k){

	/* Calculate the artificial Jobs */

	vector<double> proc_times_artificial_jobs(nbMac+1);

	for(int m = 1;m <= nbMac;m++){
		proc_times_artificial_jobs[m] = calculate_artificial_job(j,k,m);
	}


	/* Determine the completion times of the item J to be added to the scheduled list*/
	vector<double> completion_times_i(nbMac+1);
	completion_times_i[1] =  processingTimesMatrix[j][1] +  processingTimesMatrix[k][1];
	for (int m = 2; m <= nbMac; ++m )
	{
		completion_times_i[m] = completion_times_i[m-1] + max(processingTimesMatrix[k][m],processingTimesMatrix[j][m-1]);
	}

	/* Determine the completion times of the item J to be added to the scheduled list*/
	vector<double> completion_times_artifial_job(nbMac+1);
	completion_times_artifial_job[1] =  proc_times_artificial_jobs[1] +  completion_times_i[1];
	for (int m = 2; m <= nbMac; ++m )
	{
		completion_times_artifial_job[m] = proc_times_artificial_jobs[m] + max(completion_times_artifial_job[m-1],completion_times_i[m]);
	}


	return completion_times_artifial_job[nbMac] + completion_times_i[nbMac];
}


double PfspInstance::weighted_total_machine_idle_time(int j, int k){

	double total_machine_idle_time = 0;
	for (int m = 2; m <= nbMac; ++m )
	{
		int idle_time = previousMachineEndTime[j][m-1] - previousMachineEndTime[k][m];
		total_machine_idle_time += max(idle_time,0) * weight_function(j,k,nbJob,nbMac);

	}
	return total_machine_idle_time;
}

double PfspInstance::index_function(int j , int k){                                     
	double temp = nbJob - k - 2;
	temp = temp * weighted_total_machine_idle_time(j,k); 
	return temp + artificial_total_flow_time(j,k);
}





double PfspInstance::weight_function(int j ,int k , int n ,int m){
	n = n - 2;
	double temp = m - j;
	temp = temp * k;
	temp = temp / (double)n;
	temp = temp + j;
	return m / (double)temp;
}

	


/*Process the jobs and order based on their processing time*/
//Get the total processing times of the jobs and order them.
//Processing times are stored in pairs (Proccesing_time, Job_nr)
void PfspInstance::proc_times_ordered(Solution & sol){
	double total_proc = 0;


	for(int i = 1; i <= nbJob;i++){
		total_proc = 0;
		for(int j = 1;j <= nbMac;j++){
			total_proc += getTime(i,j);

		}
		proc_times[i].first = total_proc/(double)priority[i];
		proc_times[i].second = i;
	}
	std::sort (proc_times.begin(), proc_times.end());
}


