#include <iostream>
#include <algorithm>
#include "Factory.h"

class Energy: public Factory {
    protected:
    double energy_output;
    public:
    Energy();
    Energy(double energy_output);
    double environmental_impact();
};