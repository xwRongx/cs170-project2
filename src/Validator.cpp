#include "../headers/Validator.h"
#include "../headers/Instance.h"
#include <cstdlib> //for the rand()
#include <map>


// [STUB]
float Validator::evaluationFunction(vector<float>* subset, Classifier* classifier, map<Instance, float>* dataset){
      //stub function 
      return  static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(100-0))); //return random number betwwen 0 and 100
}