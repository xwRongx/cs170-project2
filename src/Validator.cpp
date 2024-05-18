#include "../headers/Validator.h"
#include <cstdlib> //for the rand()

float Validator::evaluationFunction(){ //i'm assuming this will eventually take in a vector of set??
      //stub function 
      return  static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(100-0))); //return random number betwwen 0 and 100
}
  
