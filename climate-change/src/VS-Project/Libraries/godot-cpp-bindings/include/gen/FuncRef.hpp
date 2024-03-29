#ifndef GODOT_CPP_FUNCREF_HPP
#define GODOT_CPP_FUNCREF_HPP


#include <gdnative_api_struct.gen.h>
#include <stdint.h>

#include <core/CoreTypes.hpp>
#include <core/Ref.hpp>

#include "Reference.hpp"
namespace godot {

class Object;

class FuncRef : public Reference {
	struct ___method_bindings {
		godot_method_bind *mb_call_func;
		godot_method_bind *mb_call_funcv;
		godot_method_bind *mb_is_valid;
		godot_method_bind *mb_set_function;
		godot_method_bind *mb_set_instance;
	};
	static ___method_bindings ___mb;
	static void *_detail_class_tag;

public:
	static void ___init_method_bindings();
	inline static size_t ___get_id() { return (size_t)_detail_class_tag; }

	static inline const char *___get_class_name() { return (const char *) "FuncRef"; }
	static inline Object *___get_from_variant(Variant a) { godot_object *o = (godot_object*) a; return (o) ? (Object *) godot::nativescript_1_1_api->godot_nativescript_get_instance_binding_data(godot::_RegisterState::language_index, o) : nullptr; }

	// enums

	// constants


	static FuncRef *_new();

	// methods
	Variant call_func(const Array& __var_args = Array());
	Variant call_funcv(const Array arg_array);
	bool is_valid() const;
	void set_function(const String name);
	void set_instance(const Object *instance);
	template <class... Args> Variant call_func(Args... args){
		return call_func(Array::make(args...));
	}

};

}

#endif