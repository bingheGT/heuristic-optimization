#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include <chrono>  
#include "util.h"


using namespace std;

Util::Util()
{

}



Util::~Util()
{

}

int Util::execution_time(std::chrono::time_point<std::chrono::system_clock> start_time,std::chrono::time_point<std::chrono::system_clock> end_time){
    return chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
};