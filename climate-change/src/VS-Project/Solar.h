#include <iostream>
#include <algorithm>
#include "Energy.h"
#include "Energy.cpp"

class Solar: public Energy {
    protected:
    double energy_output; 
    public:
    Solar();
    Solar(double energy_output);
    double environmental_impact(); //redefined don't use same as the one in energy
};