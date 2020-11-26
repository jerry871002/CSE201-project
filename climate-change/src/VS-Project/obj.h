class Structure {
public:
    double cost, energyuse, maintenance, CO2_output, building_time, satisfaction;
    double total_days; //total number of days that have passed in the simulation, will have to be defined by the simulation team
};

class Production : public Structure {
public:
    double input, output, efficiency;
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
