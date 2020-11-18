class Structure {
    protected:
        double cost, location, age, type;
    public:
    Structure();
    Structure(double cost, double location, double age, double type):
     cost{cost}, location{location}, age{age}, type{type} {}
};

class Production: public Structure {
    protected:
        double input, output, efficiency;
        int employment;
    public:
    Production();
    Production(double cost, double location, double age, double input, double output, double efficiency, int employment):
    Structure {cost, location,age,0}, input{input}, output{output}, efficiency{efficiency} {}
};

class Housing: public Structure {
    public:
        
};
class Infrastructure: public Structure {
    int employment;

}

class Transport: public Infrastructure {
    protected:
        double input, output, efficiency;
        double efficiency;

}

/*class PublicServices: public Structure {
    protected:
        double input, output, efficiency;
        int employment;
    public:
    PublicServices();
    PublicServices(double input, double output, double efficiency);
};*/

class Factory: public Production {


};
class Water: public Factory {
    protected: 
    double output;
    public:
    Water();
    Water(double output);
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
class Mall: public Shop{
public:
Mall();
};
class Restaraunt: public Shop{
public:
Restaraunt();
};
class SmallShop: public Shop{
public:
SmallShop();
};

class Energy: public Factory {
    protected:
    double energy_output;
    public:
    Energy();
    Energy(double energy_output);
    double environmental_impact();
};

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
