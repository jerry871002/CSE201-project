#include <iostream>
#include "CoalPowerPlant.h"


/*
If on Mac, use this to test in terminal:

g++ -std=c++17 AllPhysicalGoodsTest.cpp AllPhysicalGoodsFactories.cpp obj.cpp -ILibraries/godot-cpp-bindings/godot_headers \
-ILibraries/godot-cpp-bindings/include -ILibraries/godot-cpp-bindings/include/core \
-ILibraries/godot-cpp-bindings/include/gen -LLibraries/godot-cpp-bindings/bin \
-lgodot-cpp.osx.debug.64 -o test_AllPhysicalGoodsFactories

To then run the test if there are no compilation errors:
enter this in terminal:
./test_AllPhysicalGoodsFactories

*/


using namespace godot;

//Declare All the test functions:
void test_constructor();


int main() {
    test_constructor();
}


void test_constructor() {
    CoalPowerPlant A = CoalPowerPlant();
    CoalPowerPlant B = CoalPowerPlant();
    
    //Here can add code to check attributes of constructor

    std::cout << "Constructor Works " << std::endl;
}

