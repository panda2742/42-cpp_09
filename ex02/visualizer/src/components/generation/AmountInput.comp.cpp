#include "UI.hpp"

using namespace ftxui;

Component	AmountInputComp(void)
{
	auto	validate_and_sync_input = [&]
	{
		UIStore::use_generation_store.elements_amount_input.value.erase(
			remove_if(
				UIStore::use_generation_store.elements_amount_input.value.begin(),
				UIStore::use_generation_store.elements_amount_input.value.end(),
				[](char c) { return !isdigit(c); }
			),
			UIStore::use_generation_store.elements_amount_input.value.end()
		);

		if (!UIStore::use_generation_store.elements_amount_input.value.empty())
		{
			try
			{
				UIStore::use_generation_store.elements_amount_value.value
					= stoul(UIStore::use_generation_store.elements_amount_input.value);

				if (UIStore::use_generation_store.elements_amount_value.value > MAX_VALUE)
				{
					UIStore::use_generation_store.elements_amount_value.value = MAX_VALUE;
					UIStore::use_generation_store.elements_amount_input.value = MAX_VALUE_STR;
				}
			}
			catch(const std::exception& e)
			{
				UIStore::use_generation_store.elements_amount_value.value = 0;
				UIStore::use_generation_store.elements_amount_input.value = "0";
			}
		}
		else
		{
			UIStore::use_generation_store.elements_amount_value.value = 0;
			UIStore::use_generation_store.elements_amount_input.value = "0";
		}
	};

	Component		amount_input = Input(&UIStore::use_generation_store.elements_amount_input.value, "...");

	amount_input |= CatchEvent([&](Event event)
	{
		if (event == Event::Return)
		{
			validate_and_sync_input();
			return true;
		}
		return false;
	});

	return amount_input;
}
