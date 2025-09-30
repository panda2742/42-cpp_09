#pragma once

#include <thread> // thread
#include <stop_token> // stop_token
#include <vector> // vector

#define C_WHITE "\e[38;2;255;255;255m"
#define C_RED "\e[38;2;255;0;0m"
#define C_ORANGE "\e[38;2;255;127;0m"
#define C_YELLOW "\e[38;2;255;255;0m"
#define C_GREEN "\e[38;2;0;255;0m"
#define C_MINT "\e[38;2;152;255;152m"
#define C_PINK "\e[38;2;255;153;236m"
#define C_DARK_GREEN "\e[38;2;0;191;0m"
#define C_CYAN "\e[38;2;0;255;255m"
#define C_BLUE "\e[38;2;0;127;255m"
#define C_DARK_BLUE "\e[38;2;0;0;255m"
#define C_MAGENTA "\e[38;2;139;0;255m"
#define C_PURPLE "\e[38;2;148;0;211m"

#define C_BOLD    "\033[1m"
#define C_GREY "\e[38;2;130;130;130m"
#define C_RESET "\e[0m"
#define C_CLEARLN "\033[2K\r"
#define C_BG_GRAY "\033[48;5;240m"

#define MAX_VALUE 10000000
#define MAX_VALUE_STR "10000000"
#define VG_FLAGS "--leak-check=full --show-leak-kinds=all --track-origins=yes --show-mismatched-frees=yes --track-fds=yes --trace-children=yes"

namespace utils
{
	void						LoadingSpinner(std::stop_token, const std::string&);
	std::vector<std::string>	Split(const std::string&, const std::string&);
	std::string					GetLineProperty(const std::string&, const std::string&);
	std::vector<std::string>	GetLineKeys(const std::string&);
	template <typename T>
	std::string					FormatNumber(const T&);
	void						CleanTraces(void);
}

#include "Utils.tpp"
