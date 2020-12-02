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

        // variables that the sim team needs. If any of these are already covered or need to  be implemented diffferently let Vincent, Yury, Jerry or Sam know 
        double income, population, numberOfEmployees, carbonEmission, energyDemand, energySupply;

        double totalDays; //total number of days that have passed in the simulation, will be passed on by the City object

        // All of our policies have to go in the City class !! Look at City.h 
        Structure();
        ~Structure();

        virtual void simulate_step();
        // Coal power plant (constructor creates subcritical plant of 38% efficiency) :
        bool efficiency_supercritical(); // improve efficiency to supercritical type of plant (42% energy converted to electricity)
        bool efficiency_cogeneration(); // improve efficiency to cogeneration type of plant (47% energy converted to electricity)
        // need to add a cost for their implementation in the maintenance variable once
    };



class Production : public Structure {
public:
    double efficiency;
    int employment;
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
    int employment;
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
  