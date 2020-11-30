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
    double cost, energyuse, maintenance, CO2_output, building_time, satisfaction;
    double total_days; //total number of days that have passed in the simulation, will have to be defined by the simulation team

// All of our policies
    Structure();
    ~Structure();
// Coal power plant (constructor creates subcritical plant of 38% efficiency) :
    bool efficiency_supercritical(); // improve efficiency to supercritical type of plant (42% energy converted to electricity)
    bool improvement_cogeneration(); // improve efficiency to cogeneration type of plant (47% energy converted to electricity)
    // need to add a cost for their implementation in the maintenance variable once




class Production : public Structure {
public:
    double efficiency;
    int employment;
};



class Energy : public Production {
public:
    double energy_output;
};

class Housing : public Structure {
    //public:
}




class Infrastructure : public Structure {
public:
    int employment;
};

class Shop : public Production {
public:
    bool open;
    //get attributes from production : employment i don't remember how to do that
    //constructor :

    public:
        Shop();
};


class Solar: public Energy {
    protected:
        double energy_output;
    public:
        Solar();
        Solar(double energy_output);
        double environmental_impact(); //redefined don't use same as the one in energy

};


