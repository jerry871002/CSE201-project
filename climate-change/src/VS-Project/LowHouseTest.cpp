#include <iostream>
#include "LowHouse.h"

using namespace godot;
/*g++ -std=c++17 LowHouseTest.cpp LowHouse.cpp obj.cpp -ILibraries/godot-cpp-bindings/godot_headers -ILibraries/godot-cpp-bindings/include -ILibraries/godot-cpp-bindings/include/core -ILibraries/godot-cpp-bindings/include/gen -LLibraries/godot-cpp-bindings/bin -lgodot-cpp.osx.debug.64 -o LowHouseTest*/

void test_LowHouse();

int main() {
    test_LowHouse();
}

void test_LowHouse() {

    godot::LowHouse l = LowHouse();
    std::cout << "LowHouse 1:" << l.get_houseIncome() << std::endl;
    std::cout << "Expected result: between 106 and 666" << std::endl;
    std::cout << "LowHouse 1:" << l.get_numberofInhabitants() << std::endl;
    std::cout << "Expected result: between 1 and 6" << std::endl;

    std::cout << std::endl;
}