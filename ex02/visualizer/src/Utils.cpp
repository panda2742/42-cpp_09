#include "Utils.hpp"

#include <thread>
#include <iostream>
#include <fstream>
#include <ios>
#include <string>
#include <vector>
#include <stop_token>

using namespace std;
using namespace chrono;

namespace Utils
{
	void	LoadingSpinner(stop_token stoken, const string& message)
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
			cout << C_CLEARLN C_MINT "[ " << colors[i] << frames[i] << C_MINT " ] " << message << C_RESET << flush;
			i = (i + 1) % frames.size();
			this_thread::sleep_for(milliseconds(100));
		}
	}
}