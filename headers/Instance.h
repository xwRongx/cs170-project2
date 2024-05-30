//
// Created by Citrus on 5/23/2024.
//

#ifndef LAB2_FEATURE_SELECTION_INSTANCE_H
#define LAB2_FEATURE_SELECTION_INSTANCE_H
/*
instance is a single line/row in the data.txt files
1 instance holds a class label and featureValues for 1 row
*/
class Instance {
public:
    float classLabel;
    std::vector<float> featureValues;
    Instance() {
        this->classLabel = 0;
        this->featureValues = {};
    }
};

namespace std {
    template<> struct less<Instance>
    {
        bool operator() (const Instance& lhs, const Instance& rhs) const
        {
            return !((lhs.classLabel == rhs.classLabel) && (lhs.featureValues == rhs.featureValues));
        }
    };
}

#endif //LAB2_FEATURE_SELECTION_INSTANCE_H
