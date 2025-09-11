#include "UI.hpp"

UIStore::GenerationStoreStruct	UIStore::use_generation_store = {
	.output_filename = Store<string>(""),
	.elements_amount_value = Store<long long>(100000),
	.elements_amount_input = Store<string>(""),
};

UIStore::MainViewStoreStruct	UIStore::use_main_view_store = {
	.selected_tab = Store<int>(0),
	.tab_names = Store<vector<string>>(vector<string>{
		"  Generate input  ",
		"  Start program  ",
		"  Start Valgrind  "
	})
};
