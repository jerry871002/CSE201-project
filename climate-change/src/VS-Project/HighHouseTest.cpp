#include <iostream>
#include "HighHouse.h"



/* g++ -std=c++17 HighHouseTest.cpp HighHouse.cpp obj.cpp -ILibraries/godot-cpp-bindings/godot_headers -ILibraries/godot-cpp-bindings/include -ILibraries/godot-cpp-bindings/include/core -ILibraries/godot-cpp-bindings/include/gen -LLibraries/godot-cpp-bindings/bin -lgodot-cpp.osx.debug.64 -o HighHouseTest */

using namespace godot;

void test_simulate_step();

int main() {
    test_simulate_step();
}

void test_simulate_step() {

    godot::HighHouse l = HighHouse();
 
    

    
    std::cout << std::endl;
}