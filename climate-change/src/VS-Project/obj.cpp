#include <iostream>
#include "obj.h"



Structure::Structure() {
    /*
    cost = 0;
    energyUse = 0;
Struc::Struc() {
    cost = 0;
    energyuse = 0;
    maintenance = 0;
    satisfaction = 0;
    income = 0;
    population = 0;
    numberOfEmployees = 0;
    carbonEmission = 0;
    energyDemand = 0;
    energySupply = 0;
     */
    
 }
/*
Structure::Structure(double cost, double energyUse, double maintenance, double satisfaction, double income, double population, double numberOfEmployees, double carbonEmission, double energyDemand, double energySupply) :
    cost{ cost }, energyUse{ energyUse }, maintenance{ maintenance }, satisfaction{ satisfaction }, income{ income }, population{ population }, numberOfEmployees{ numberOfEmployees }, carbonEmission{ carbonEmission }, energyDemand{ energyDemand }, energySupply{ energySupply }{}
*/

bool Structure::efficiency_cogeneration() {
    return false; //given interface commands it will then return true and activate improvement
}

bool Structure::efficiency_supercritical() {
    return false; //given interface commands it will return true and activate improvement
}


    healthcare = 0;
    needs= 0;
}

Struc::Struc(double cost, double energyuse, double maintenance, double satisfaction, double income, double population, double numberOfEmployees, double carbonEmission, double energyDemand, double energySupply, double healthcare, double needs) :
    cost{ cost }, energyuse{ energyuse }, maintenance{ maintenance }, satisfaction{ satisfaction }, income{ income }, population{ population }, numberOfEmployees{ numberOfEmployees }, carbonEmission{ carbonEmission }, energyDemand{ energyDemand }, energySupply{ energySupply }, healthcare{ healthcare }, needs{ needs } {}

Struc::~Struc() {

}
