#include "include/prottype.hpp"
#include <random>

mt19937_64 engine;

//Setting seed
void random_seed(int seed){
    engine.seed(seed);
     
}

//Generate uniform random number
float random_uniform(float min, float max){
    uniform_real_distribution<> dist(min, max);
    
    return dist(engine);
}

//Generate integer random number
int random_int(int min, int max){
    uniform_int_distribution<> dist(min, max);
    
    return dist(engine);
}

