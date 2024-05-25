#include <map>
#include "../headers/Classifier.h"
#include "Instance.h"

class Validator
{
private:
    /* data */
public:
    Validator(/* args */);
    ~Validator();

    float evaluationFunction(vector<float>* subset, Classifier* classifier, map<Instance, float>* dataset);
};

