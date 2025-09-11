#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include "Visualizer.hpp"
#include <string>

using namespace std;
using namespace ftxui;

template <class T>
struct Store
{
	T	value;

	Store(const T& value): value(value) {}
};

class UIStore
{
	public:
		static struct GenerationStoreStruct
		{
			Store<string>		output_filename;
			Store<long long>	elements_amount_value;
			Store<string>		elements_amount_input;
		}	use_generation_store;

		static struct MainViewStoreStruct
		{
			Store<int>				selected_tab;
			Store<vector<string>>	tab_names;
		}	use_main_view_store;
};

Component	AmountInputComp(void);
Component	AmountSliderComp(void);
Component	FilenameInputComp(void);

Component	GenerationRendererView(Visualizer& visualizer);
Component	LaunchView(Visualizer& visualizer);
