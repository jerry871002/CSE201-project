#pragma once

#include "obj.h"
#include <core/Godot.hpp>



namespace godot {
    class Housing : public Structure {
        GODOT_SUBCLASS(Housing, Structure)

    public:
        Housing();
        ~Housing();
    };

	class House : public Housing {
		GODOT_SUBCLASS(House, Housing)
	public:
		House();
		~House();

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
	};
}