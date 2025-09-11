#include "UI.hpp"

using namespace ftxui;

Component	FilenameInputComp(void)
{
	return Input(&UIStore::use_generation_store.output_filename.value, ".large:1000");
}
