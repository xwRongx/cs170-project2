#include <map>
#include "../headers/Classifier.h"

class Validator
{
private:
    /* data */
public:
    Validator(/* args */);
    ~Validator();

    float evaluationFunction(vector<float>* subset, Classifier* classifier, map<vector<float>, float>* dataset);
};

