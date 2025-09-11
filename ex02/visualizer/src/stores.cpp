#include "UI.hpp"

UIStore::GenerationStoreStruct	UIStore::use_generation_store = {
	.output_filename = Store<string>(""),
	.elements_amount_value = Store<long long>(100000),
	.elements_amount_input = Store<string>(""),
};
