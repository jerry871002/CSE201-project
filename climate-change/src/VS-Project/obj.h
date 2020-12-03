#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>

using namespace godot;

class Structure {
    public:
        double cost, energyuse, maintenance, CO2Emission, buildingTime, satisfaction;



        // The following will be city-wide counters that will be updated every day : 
        // income, population, numberOfEmployees, carbonEmission, energyDemand, energySupply

        // income is the total wage (GDP), population is the population of the whole city, numberOfEmployees the total employed people,  
        // carbonEmission the total CO2, energyDemand the sum of all energy needed and energySupply the maximum production capacity

        // It would be great if someone could write here what variable we have to call to update each
        // income : 
        // population :
        // numberOfEmployees:
        // carbonEmission : CO2Emission
        // energyDemand :
        // energySupply : 

        // sim team will program these counters to grab the correct attributes within all structures
        // if needed within any object they will be passed on by the city object
        // don't use these specific variable names inside the structures pls or we will all get confused 

        

        double totalDays; //total number of days that have passed in the simulation, will be passed on by the City object


        bool PanelsOn; // whether the building has solar panels or not. delfault at false, only possible to set to true for certain classes

        // All of our policies have to go in the City class !! Look at City.h 
        Structure();
        ~Structure(){}

        // virtual void simulate_step();
        // Coal power plant (constructor creates subcritical plant of 38% efficiency) :
        bool efficiency_supercritical(); // improve efficiency to supercritical type of plant (42% energy converted to electricity)
        bool efficiency_cogeneration(); // improve efficiency to cogeneration type of plant (47% energy converted to electricity)
        // need to add a cost for their implementation in the maintenance variable once
    };



class Production : public Structure {
public:
    double efficiency;
    double employment;
};



class Energy : public Production {
public:
    double energyOutput;
};

class Housing : public Structure {
public:
    Housing();
    ~Housing();
};


class Infrastructure : public Structure {
public:
    double employment;
};

class Shop : public Production {
public:
    bool open;
    //get attributes from production : employment i don't remember how to do that
    //constructor :
    Shop();
};


class Solar : public Energy {
protected:
    double energyOutput;
public:
    Solar();
    Solar(double energyOutput);
    double environmental_impact(); //redefined don't use same as the one in energy

};
  