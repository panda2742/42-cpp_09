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

void	LoadingSpinner(std::stop_token stoken, const std::string& message)
{
	const std::vector<std::string>	frames = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
	const std::vector<std::string>	colors = {
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
		std::cout << C_CLEARLN C_ORANGE "[ " << colors[i] << frames[i] << C_ORANGE " ] " << message << C_RESET << std::flush;
		i = (i + 1) % frames.size();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

std::vector<std::string>	Split(const std::string& s, const std::string& delimiter)
{
	auto			segments = s | std::views::split(delimiter);
	std::vector<std::string>	parts;

	for (const auto& seg : segments)
		parts.emplace_back(seg.begin(), seg.end());

	return parts;
}

std::string	GetLineProperty(const std::string& key, const std::string& line_fragment)
{
	std::vector<std::string>	values = Split(line_fragment, ":");

	if (values.size() == 2 && values.at(0) == key)
		return values.at(1);

	throw std::runtime_error("Line format is corrupted, '" + key + "' key does not exist.");
}

std::vector<std::string>	GetLineKeys(const std::string& line)
{
	std::vector<std::string>	values = Split(line, "|");
	auto			splitted = values | std::views::transform(
		[](const std::string& s)
		{
			return Split(s, ":")[0];
		}
	);
	std::vector<std::string>	keys(splitted.begin(), splitted.end());

	return keys;
}

void	CleanTraces(void)
{
	system("rm -rf ../../.visu_tmp");
	system("cd ../../ && make fclean > /dev/null");
	system("clear");
}

}