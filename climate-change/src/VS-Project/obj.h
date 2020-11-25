class Structure {
protected:
    double cost, energyuse, maintenance, CO2_output, building_time, satisfaction;

public:
    Structure();
    Structure(double cost, double energyuse, double maintenance, double satisfaction) : 
    cost{cost}, energyuse{energyuse}, maintenance{maintenance}, satisfaction{satisfaction} {}
};

class Production : public Structure {
protected:
    double input, output, efficiency;
    int employment;

public:
    Production();
    Production(double cost, double energyuse, double maintenance, double satisfaction, 
               double input, double output, double efficiency, int employment) : 
               Structure{cost, energyuse, maintenance, satisfaction}, 
               input{input}, output{output}, efficiency{efficiency}, employment{employment} {}
};

class Energy : public Production {
protected:
    double energy_output;
};

class Housing : public Structure {
    //public:
};

class Infrastructure : public Structure {
protected:
    int employment;
};

class Transport : public Infrastructure {
protected:
    double num_cars;
    double num_electric_cars;
    double num_bus;
    double num_tram;
};

class Shop : public Production {
protected:
    bool open;
    //get attributes from production : employment i don't remember how to do that
    //constructor :
public:
    Shop();
    Shop(double employment);
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
