#pragma once

#include "obj.h"
#include <core/Godot.hpp>



namespace godot {
    class Production : public Structure {
        GODOT_SUBCLASS(Production, Structure)

    public:
        virtual String class_name();
        Production();
        ~Production();

    };
}