
// #ifndef TD7_HPP
// #define TD7_HPP

class Structure {
    public:
        double cost, location, age, type;

}

class Production: public Structure {
    public:
        double input, output, efficiency;
        int employment;
}

class Housing: public Structure {
    public:
        
}

class PublicServices: public Structure {
    public:
        double input, output, efficiency;
        int employment;
}

class Factory: public Production {


}

class Shop: public Production {
    //get attributes from production : employment i don't remember how to do that
    //constructor :
    Shop(double employment);
}


class Energy: public Factory {
    double energy_output;
    double environmental_impact();
}

class Solar: public Energy {
    double energy_output;
    double environmental_impact() //redefined don't use same as the one in energy
}

class Nuclear: public Energy {
    double energy_output;
    double environmental_impact() //redefined don't use same as the one in energy
}
