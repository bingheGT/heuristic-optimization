#ifndef _UTIL_H_
#define _UTIL_H_
#include "pfspinstance.h"


#include <string>
#include <vector>
#include <list>
#include <chrono>
using namespace std;

class PfspInstance;

class Util{
private:


  public:
    Util();
    ~Util();
    int execution_time(std::chrono::time_point<std::chrono::system_clock> start_time,std::chrono::time_point<std::chrono::system_clock> end_time);

};

#endif

