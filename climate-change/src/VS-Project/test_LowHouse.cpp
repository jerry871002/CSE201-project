#include <iostream>
#include "LowHouse.h"

using namespace godot;

void test_simulate_step();

int main() {
    test_simulate_step();
}

void test_simulate_step() {

    godot::LowHouse l = LowHouse();
 
    
    
    std::cout << "LowHouse 1:" << std::endl;
    std::cout << "Expected result: " << (10 * 1000) << std::endl;
    
    
    std::cout << std::endl;
}