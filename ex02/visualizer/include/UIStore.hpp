#pragma once

#include <string>

using namespace std;

template <class T>
struct Store
{
	T	value;

	Store(const T& value): value(value) {}
};

class UIStore
{
	public:
		static struct GenerationStoreStruct {
			Store<string>	output_filename;
			Store<long long>		elements_amount_value;
			Store<string>	elements_amount_input;
		}	use_generation_store;
};
