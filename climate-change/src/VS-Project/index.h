#pragma once
#include "data.h"


// added a cap - the bool cap returns true when the value is at 100 
// incomplete ! need to add a function to update

namespace godot {
	class Index : public Data {
		GODOT_CLASS(Index, Data)
	private:
		bool capped;
		
	public:

		bool is_capped();
		
	};
}
