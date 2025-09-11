#include "UI.hpp"

using namespace ftxui;

Component	AmountSliderComp(void)
{
	return Slider(
		"",
		reinterpret_cast<int *>(&UIStore::use_generation_store.elements_amount_value.value),
		0,
		MAX_VALUE,
		1000
	);
}
