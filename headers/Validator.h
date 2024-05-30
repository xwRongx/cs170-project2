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

    float evaluationFunction(vector<int> features, Classifier* classifier, vector<Instance*> dataset);
};

