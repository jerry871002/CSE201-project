#pragma once

#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>

using namespace godot;

/* disregard, only for test purposes
class Structure {   
    public:
        void simulate_step() {};
        double cost, energyuse, maintenance, satisfaction;
        double income, population, numberOfEmployees, carbonEmission, energyDemand, energySupply, healthcare, needs;

        virtual double get_emissions() { return 0; };

        double totalDays; //total number of days that have passed in the simulation, will be passed on by the City object

        bool PanelsOn; // whether the building has solar panels or not. delfault at false, only possible to set to true for certain classes

        virtual void simulate_step();
        // Coal power plant (constructor creates subcritical plant of 38% efficiency) :
        bool efficiency_supercritical(); // improve efficiency to supercritical type of plant (42% energy converted to electricity)
        bool efficiency_cogeneration(); // improve efficiency to cogeneration type of plant (47% energy converted to electricity)
        // need to add a cost for their implementation in the maintenance variable once

        Structure();
        ~Structure();
        Structure(double cost, double energyuse, double maintenance, double satisfaction, double income, double population, double numberOfEmployees, double carbonEmission, double energyDemand, double energySupply, double healthcare, double needs);
};
*/


class Structure {
public:
    double cost, energyUse, maintenance, CO2Emission, buildingTime, satisfaction, environmentalCost;
    double age = 0; //age of each particular object in days, initialize to 0 in constructor

    // The following will be city-wide counters that will be updated every day : 
    // income, population, numberOfEmployees, carbonEmission, energyDemand, energySupply

    // income is the total wage (GDP), population is the population of the whole city, numberOfEmployees the total employed people,  
    // carbonEmission the total CO2, energyDemand the sum of all energy needed and energySupply the maximum production capacity

    // It would be great if someone could write here what variable we have to call to update each (ADDITIONS!)
    // income : we didn't take it into account for the moment
    // population : we don't have that, thought you would decide about it
    // numberOfEmployees: employment
    // carbonEmission : CO2Emission
    // energyDemand : that corresponds to energyUse for each class
    // energySupply : that corresponds to energyOutput for each object producing it in the energy class
    // healthcare : 
    // needs : 

    // sim team will program these counters to grab the correct attributes within all structures
    // if needed within any object they will be passed on by the city object
    // don't use these specific variable names inside the structures pls or we will all get confused    

    double totalDays; //total number of days that have passed in the simulation, will be passed on by the City object

    bool PanelsOn; // whether the building has solar panels or not. delfault at false, only possible to set to true for certain classes

    // All of our policies have to go in the City class !! Look at City.h 
    Structure();
    ~Structure();

    // virtual void simulate_step();
    // Coal power plant (constructor creates subcritical plant of 38% efficiency) :
    bool efficiency_supercritical(); // improve efficiency to supercritical type of plant (42% energy converted to electricity)
    bool efficiency_cogeneration(); // improve efficiency to cogeneration type of plant (47% energy converted to electricity)
    // need to add a cost for their implementation in the maintenance variable once
};


class Production : public Structure {
public:
    Production();
    ~Production();
    double efficiency;
    double employment;
};

class Energy : public Production {
public:
    Energy();
    ~Energy();
    double energyOutput;
    double energyPerDay;
};

class Housing : public Structure {
public:
    Housing();
    ~Housing();
};


class Infrastructure : public Structure {
public:
    Infrastructure();
    ~Infrastructure();
    double employment;
};

class Shop : public Production {
public:
    Shop();
    ~Shop();
    bool open;
};


class Solar : public Energy {
protected:
    double energyOutput;
public:
    Solar();
    Solar(double energyOutput);
    ~Solar();
    double environmental_impact(); //redefined don't use same as the one in energy

};
  