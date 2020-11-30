#include <iostream>
#include "obj.h"



Struc::Struc() {
    cost = 0;
    energyuse = 0;
    maintenance = 0;
    satisfaction = 0;
    income = 0;
    population = 0;
    numberOfEmplyees = 0;
    carbonEmission = 0;
    energyDemand = 0;
    energySupply = 0;
}

Struc::Struc(double cost, double energyuse, double maintenance, double satisfaction, double income, double population, double numberOfEmplyees, double carbonEmission, double energyDemand, double energySupply) :
    cost{ cost }, energyuse{ energyuse }, maintenance{ maintenance }, satisfaction{ satisfaction }, income{ income }, population{ population }, numberOfEmplyees{ numberOfEmplyees }, carbonEmission{ carbonEmission }, energyDemand{ energyDemand }, energySupply{ energySupply }{}

Struc::~Struc() {

}