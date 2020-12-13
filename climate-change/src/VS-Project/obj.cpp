#include <iostream>
#include "obj.h"


using namespace godot;


Production::Production() {}
Production::~Production() {}

Energy::Energy() {}
Energy::~Energy() {}

Housing::Housing() {}
Housing::~Housing() {}

Infrastructure::Infrastructure() {}
Infrastructure::~Infrastructure() {}

Shop::Shop() {}
Shop::~Shop() {}

Solar::Solar() {}
Solar::Solar(double energyOutput) : energyOutput(energyOutput) {}
Solar::~Solar() {}