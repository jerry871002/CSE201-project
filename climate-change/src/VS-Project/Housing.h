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
}