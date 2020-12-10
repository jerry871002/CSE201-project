#include <iostream>
#include "obj.h"


/* disregard, only for test purposes
Structure::Structure() {
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
    healthcare = 0;
    needs= 0;
}

Structure::Structure(double cost, double energyuse, double maintenance, double satisfaction, double income, double population, double numberOfEmployees, double carbonEmission, double energyDemand, double energySupply, double healthcare, double needs) :
    cost{ cost }, energyuse{ energyuse }, maintenance{ maintenance }, satisfaction{ satisfaction }, income{ income }, population{ population }, numberOfEmployees{ numberOfEmployees }, carbonEmission{ carbonEmission }, energyDemand{ energyDemand }, energySupply{ energySupply }, healthcare{ healthcare }, needs{ needs } {}

*/

Structure::Structure() {
    /*
    cost = 0;
    energyUse = 0;
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

// fill once variables known:
// Structure::Structure(double counter) : counter{ counter }

Structure::~Structure() {}

bool Structure::efficiency_cogeneration() {
    return false; //need to add possibility of turning true (then definitive) if clicked, adding the cost in maintenace needed for this upgrade
}

bool Structure::efficiency_supercritical() {
    return false; //need to add possibility of turning true (then definitive) if clicked, adding the cost in maintenace needed for this upgrade
}

Production::Production() {}
Production::~Production() {}

Energy::Energy() {}
Energy::~Energy() {}

Housing::Housing() {}
Housing::~Housing() {}

Infrastructure::Infrastructure() {}
Infrastructure::~Infrastructure() {}

Shop::Shop() {}
Shop::~Shop() {}

Solar::Solar() {}
Solar::Solar(double energyOutput) : energyOutput(energyOutput) {}
Solar::~Solar() {}