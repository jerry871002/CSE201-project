#include <iostream>
#include <algorithm>
#include "Energy.h"
#include "Energy.cpp"

class Nuclear: public Energy {
    protected:
    double energy_output;
    public:
    Nuclear();
    Nuclear(double energy_output);
    double environmental_impact(); //redefined don't use same as the one in energy
};
