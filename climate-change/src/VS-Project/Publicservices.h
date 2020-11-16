#include <iostream>
#include "Structure.h"

class PublicServices: public Structure {
    protected:
        double input, output, efficiency;
        int employment;
    public:
    PublicServices();
    PublicServices(double input, double output, double efficiency);
};