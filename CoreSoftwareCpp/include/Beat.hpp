#pragma once

#include <vector>

class Beat {
    public:
    std::vector<double> samples;
    double rrInterval;
    char beatClass;
    int index;
};