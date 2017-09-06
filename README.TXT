The classes involved in the algorithms of assignment 2 are as following :

1) Flowshop.cpp
   Determines which algorithms to run in the application.

2) Iteratedgreedysearch.cpp
   Contains the main IGS algorithm with destruction and construction procedures.

3) Genetic.cpp
   Contains the main HGA algorithm with the mutation, crossover operator and elite selection procedures.

4) Population.cpp
   Used by the genetic algorithm to initialize and evaluate the population.

5) Neighborhood.cpp
   Contains the local search algorithms, unlike the previous assignment this now contains a partial insertion local search.

6) Instance.cpp
   Contains the extra procedures compared to the first assignment such as :
   The calculate_artificial_job and artificial_total_flow_time are used by the Genetic algorithm.
   proc_times_ordered,weight_function,index_function and weighted_total_machine_idle_time are used by the initialization algorithm.

7) Util.cpp
   Contains the time calculation between start & end time.




The flags when running the algorithms are described as :

1) Iterated Greedy Search 

./flowshopWCT instance_folder best_wct runtime max_wct random_seed igs destruction temperature


1) Hybrid Genetic Algorithm

./flowshopWCT instance_folder best_wct runtime max_wct random_seed igs population_size elite_probability mutation_probability



 EXAMPLE SCENARIO
 ---------------------

 1) Run the first single instance of size 50 on IGS with the following parameters :

 	instance = 50_20_01
 	best_wct = 595260
 	runtime  = 150 sec
 	max_wct  = 595260
 	seed     = 12345
 	destruction = 2
 	temperature = 0.7

	./flowshopWCT ./instances/50_20_01 595260 150 595260 12345 igs 2 0.7

 2) Run the first single instance of size 50 on HGA with the following parameters :

 	instance = 50_20_01
 	best_wct = 595260
 	runtime  = 150 sec
 	max_wct  = 595260
 	seed     = 12345
 	population_size = 2
 	elite_probability = 0.8
 	mutation_probablity = 0.2

	./flowshopWCT ./instances/50_20_01 595260 150 595260 12345 genetic 50 0.8 0.2

3) Run all instances of size 50 on IGS with the following parameters :

	instance_folder = ./instances/bestSolutions50.txt
	output_folder   = ./Results
	max_runtime     = 150 
	max_wct         = 0 (this allows us to ignore the max_wct parameter)
	random_seed     = 1245

	./run.sh ./instances/bestSolutions50.txt ./Results 150 0 1245


3) Run all instances of size 50 on HGA with the following parameters :
	
	instance_folder = ./instances/bestSolutions50.txt
	output_folder   = ./Results
	max_runtime     = 150 
	max_wct         = 0 (this allows us to ignore the max_wct parameter)
	random_seed     = 1245

	./run_HGS.sh ./instances/bestSolutions50.txt ./Results 150 0 1245
	
