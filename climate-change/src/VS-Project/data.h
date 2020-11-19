#pragma once
#include <core/Godot.hpp>
#include <Object.hpp>
#include <iostream>
#include <set>


// created a Data class. this class has a value, which can be edited, a constructor which sets value to 0 and a virtual update_data() method
/* left to do:
update values -> update counters -> update Indices
write equations
*/

namespace godot {
	class Data : public Object {
		GODOT_CLASS(Data, Object)
	protected:
		
		
		int value;
	public:
		//set containing all pointers
		static std::set<Data*> values;



		virtual void update_statistic();
		void set_value(int);
		void change_value(int);
		int get_value();

		Data();
		~Data();
		
	};

////Counters - initialize somewhere, probably not as classes, list of counters to update indices
// list of counters updated
// need function to update

	Data income;
	Data population;
	Data employed;
	Data carbon;
	Data energyDemand;
	Data energySupply;
	Data healthcare;
	Data needs;
	Data waste;

}


////Counters - initialize somewhere, probably not as classes, list of counters to update indices

////Indices

// added a cap - the bool cap returns true when the value is at 100 
// incomplete ! need to add a function to update

namespace godot {
	class Index : public Data {
		GODOT_CLASS(Index, Data)
	private:
		bool capped;
		static std::set<Data*> indices;
	public:

		bool is_capped();

	};
}

namespace godot {
	class Happiness : public Index {
		GODOT_CLASS(Happiness,Index)
	



	};


}



namespace godot {
	class Employment : public Index {
		GODOT_CLASS(Employment, Index)




	};


}



namespace godot {
	class Emissions : public Index {
		GODOT_CLASS(Emissions, Index)




	};


}



namespace godot {
	class Medical : public Index {
		GODOT_CLASS(Medical, Index)




	};


}

namespace godot {
	class Employment : public Index {
		GODOT_CLASS(Employment, Index)




	};


}
