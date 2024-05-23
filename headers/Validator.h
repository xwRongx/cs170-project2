#include "../headers/Classifier.h"

class Validator
{
private:
    /* data */
public:
    Validator(/* args */);
    ~Validator();

    float evaluationFunction(vector<float>* subset, Classifier classifier, vector<vector<float>*>* dataset);
};

