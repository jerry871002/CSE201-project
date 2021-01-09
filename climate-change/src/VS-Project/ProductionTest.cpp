#include <iostream>
#include "Production.h"
/*
If on Mac, use this to test in terminal:

g++ -std=c++17 ProductionTest.cpp Production.cpp obj.cpp -ILibraries/godot-cpp-bindings/godot_headers \
-ILibraries/godot-cpp-bindings/include -ILibraries/godot-cpp-bindings/include/core \
-ILibraries/godot-cpp-bindings/include/gen -LLibraries/godot-cpp-bindings/bin \
-lgodot-cpp.osx.debug.64 -o test_Production

To then run the test if there are no compilation errors:
enter this in terminal:
./test_Production
*/

using namespace godot;

//Declare All the test functions:


//Test Functions for wheat and meat production:
void test_agriculture_constructor();
void test_agriculture_simulate_step();
void test_agriculture_get_attributes();

//Test Functions :
void test_production_constructor();
void test_production_simulate_step();


int main() {
    // Test functions:

    //Checking through the House:
    test_agriculture_constructor();
    test_agriculture_simulate_step();
   


    std::cout << "EveryThing in Production works fine ;)" << std::endl;
}






void test_agriculture_constructor() {
    try {
        AgriculturalProduction A = AgriculturalProduction();
        AgriculturalProduction B = AgriculturalProduction();
        AgriculturalProduction D = AgriculturalProduction();
        int type1 = 0;
        int type2 = 0;
        for (int i = 0; i < 100; i++){
            AgriculturalProduction C = AgriculturalProduction();
            if (C.agricultureType == 0){type1 +=1;}
            if (C.agricultureType == 1){type2 +=1;}
            
        }
        //std::cout << "Number of type 0, type 1, type 2: " << type1 << ", " << type2 << ", " << type3 << std::endl;
    } catch (...){
        std::cout << "Error with the Houses's Constructor" << std::endl;
    }

    // std::cout << "Constructor Works " << std::endl;
}

void test_agriculture_simulate_step(){
    AgriculturalProduction A = AgriculturalProduction();
    try{
        for (int i = 0; i<5; i++) {
            A.simulate_step(100);
            //std::cout << "House age: " << A.age << std::endl;
            
        }
    } catch (...) {
        std::cout << "Problem with house's simulate step" << std::endl;
    }

    

}

void test_agriculture_get_attributes(){
    AgriculturalProduction A = AgriculturalProduction();
    try{
        
        double b = A.get_co2emissions();
        double c = A.get_environmentalcost();
        double d = A.get_energyuse();


        //std::cout << "Restaurant type: " << A.restaurantType << ", Energy use is: " << A.get_energyuse() << std::endl;
        //std::cout << "Restaurant type: " << A.restaurantType << ", CO2emmisions is: " << A.get_co2emissions() << std::endl;
    } catch (...){
        std::cout << "Error with one of the house's get_.... functions" << std::endl;
    }
}


