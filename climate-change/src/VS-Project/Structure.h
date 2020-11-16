class Structure {
    protected:
        double cost, location, age, type;
    public:
    Structure();
    Structure(double cost, double location, double age, double type):
     cost{cost}, location{location}, age{age}, type{type} {}
};

