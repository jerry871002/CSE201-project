#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>

using namespace godot;


class Struc {   
    public:
        void simulate_step() {};
        double cost, energyuse, maintenance, satisfaction;
        double income, population, numberOfEmplyees, carbonEmission, energyDemand, energySupply;
        Struc();
        ~Struc();
        Struc(double cost, double energyuse, double maintenance, double satisfaction, double income, double population, double numberOfEmplyees, double carbonEmission, double energyDemand, double energySupply);
};
/*
class Production: public Struc {
    protected:
        double input, output, efficiency;
        int employment;
    public:
    Production();
    Production(double cost, double energyuse, double maintenance, double satisfaction, double input, double output, double efficiency, int employment):
    Struc {cost, location,maintenance, satisfaction}, input{input}, output{output}, efficiency{efficiency}, empolyment{employment} {}
};


class Energy: public Production {
    protected:
        double energy_output;
    public:
        Energy();
        Energy(double energy_output);
        double environmental_impact();
};



class Housing: public Struc {
    //public:
        
};



class Infrastructure: public Struc {
    protected:
        int employment;

};

class Transport: public Infrastructure {
    protected:
        double num_cars;
        double num_electric_cars;
        double num_bus;
        double num_tram;

};
 


class Shop: public Production {
    protected:
        bool open;
    //get attributes from production : employment i don't remember how to do that
    //constructor :
    public:
        Shop();
        Shop(double employment);
};
*/


/*
class Solar: public Energy {
    protected:
        double energy_output;
    public:
        Solar();
        Solar(double energy_output);
        double environmental_impact(); //redefined don't use same as the one in energy
};

class Nuclear: public Energy {
    protected:
        double energy_output;
        public:
    Nuclear();
    Nuclear(double energy_output);
    double environmental_impact(); //redefined don't use same as the one in energy
};

*/
