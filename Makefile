program : src/util.h src/util.cpp src/solution.h src/solution.cpp src/pfspinstance.h src/flowshop.cpp src/pfspinstance.cpp src/population.cpp src/population.h src/neighborhood.cpp src/neighborhood.h src/iteratedgreedysearch.cpp src/iteratedgreedysearch.h src/genetic.cpp src/genetic.h
	
	g++ -std=c++11 -O3 -c ./src/util.cpp  -o src/util.o
	g++ -std=c++11 -O3 -c ./src/solution.cpp  -o src/solution.o
	g++ -std=c++11  -O3 -c ./src/pfspinstance.cpp -o src/pfspinstance.o
	g++ -std=c++11 -O3 -c ./src/flowshop.cpp -o src/flowshop.o
	g++ -std=c++11 -O3 -c ./src/population.cpp -o src/population.o
	g++ -std=c++11 -O3 -c ./src/neighborhood.cpp -o src/neighborhood.o
	g++ -std=c++11 -O3 -c ./src/iteratedgreedysearch.cpp -o src/iteratedgreedysearch.o
	g++ -std=c++11 -O3 -c ./src/genetic.cpp -o src/genetic.o
	

	g++ -std=c++11 -O3 src/util.o src/flowshop.o src/solution.o src/pfspinstance.o src/population.o src/neighborhood.o src/iteratedgreedysearch.o src/genetic.o -o flowshopWCT

clean:
	rm src/*.o flowshopCWT
