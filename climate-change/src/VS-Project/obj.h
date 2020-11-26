class Structure {
public:
    double cost, energyuse, maintenance, CO2_output, building_time, satisfaction;
    double total_days; //total number of days that have passed in the simulation, will have to be defined by the simulation team

// All of our policies 
    
//Coal power plant (constructor creates subcritical plant of 38% efficiency) :
    bool efficiency_supercritical(); // improve efficiency to supercritical type of plant (42% energy converted to electricity)
    bool improvement_cogeneration(); // improve efficiency to cogeneration type of plant (47% energy converted to electricity)
    // need to add a cost for their implementation in the maintenance variable once
};

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
};

class Infrastructure : public Structure {
public:
    int employment;
};

class Transport : public Infrastructure {
public:
    double num_cars;
    double num_electric_cars;
    double num_bus;
    double num_tram;
};

class Shop : public Production {
public:
    bool open;
    //get attributes from production : employment i don't remember how to do that
    //constructor :

};


