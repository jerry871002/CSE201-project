#include "Structure.h"
#include "Structure.cpp"
class Production: public Structure {
    protected:
        double input, output, efficiency;
        int employment;
    public:
    Production();
    Production(double cost, double location, double age, double input, double output, double efficiency, int employment):
    Structure {cost, location,age,0}, input{input}, output{output}, efficiency{efficiency} {}
};