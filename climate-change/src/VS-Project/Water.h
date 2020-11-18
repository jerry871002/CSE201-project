#include <iostream>
#include <algorithm>
#include "Factory.h"

class Water: public Factory {
    protected: 
    double output;
    public:
    Water();
    Water(double output);
};