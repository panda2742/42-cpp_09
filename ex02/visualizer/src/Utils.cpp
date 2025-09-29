#include "Utils.hpp"

#include <algorithm>
#include <thread>
#include <iostream>
#include <fstream>
#include <ios>
#include <string>
#include <vector>
#include <stop_token>
#include <ranges>
#include <map>

namespace utils
{

void	LoadingSpinner(std::stop_token stoken, const string& message)
{
	const vector<string>	frames = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
	const vector<string>	colors = {
		C_RED,
		C_ORANGE,
		C_YELLOW,
		C_GREEN,
		C_DARK_GREEN,
		C_CYAN,
		C_BLUE,
		C_DARK_BLUE,
		C_MAGENTA,
		C_PURPLE
	};

	size_t	i = 0;
	while (!stoken.stop_requested())
	{
		cout << C_CLEARLN C_ORANGE "[ " << colors[i] << frames[i] << C_ORANGE " ] " << message << C_RESET << flush;
		i = (i + 1) % frames.size();
		this_thread::sleep_for(milliseconds(100));
	}
}

vector<string>	Split(const string& s, const string& delimiter)
{
	auto			segments = s | views::split(delimiter);
	vector<string>	parts;

	for (const auto& seg : segments)
		parts.emplace_back(seg.begin(), seg.end());

	return parts;
}

string	GetLineProperty(const string& key, const string& line_fragment)
{
	vector<string>	values = Split(line_fragment, ":");

	if (values.size() == 2 && values.at(0) == key)
		return values.at(1);

	throw runtime_error("Line format is corrupted, '" + key + "' key does not exist.");
}

vector<string>	GetLineKeys(const string& line)
{
	vector<string>	values = Split(line, "|");
	auto			splitted = values | views::transform(
		[](const string& s)
		{
			return Split(s, ":")[0];
		}
	);
	vector<string>	keys(splitted.begin(), splitted.end());

	return keys;
}

void	CleanTraces(void)
{
	system("rm -rf ../../.visu_tmp");
	system("cd ../../ && make fclean > /dev/null");
	system("clear");
}

}