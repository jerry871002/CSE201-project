#include "index.h"




bool Index::is_capped() {
	return (get_value() == 100);
}