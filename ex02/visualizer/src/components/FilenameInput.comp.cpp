#include "ui.hpp"

using namespace ftxui;

Component	FilenameInputComp(void)
{
	Component	input = Input(&UIStore::use_generation_store.output_filename.value, ".large:1000");

	return input;
}
