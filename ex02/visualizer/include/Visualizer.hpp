#pragma once

#include <map>
#include <string>

#include "ftxui/component/screen_interactive.hpp"

#define MAX_VALUE 10000000
#define MAX_VALUE_STR "10000000"

using namespace std;
using namespace ftxui;

class Visualizer
{
	public:
		typedef struct Options
		{
			int		selectedRunMode;
			int		selectCompilationFlags;
			int		selectedTreatment;
			string	amountOfElements;

			Options(void)
			{
				selectedRunMode = 0;
				selectCompilationFlags = 0;
				selectedTreatment = 1;
				amountOfElements = "3000";
			}
		}	Options_t;

		typedef struct Result
		{
			string		task;
			string		container_name;
			double		init_time;
			short int	init_threads;
			double		sort_time;
			short int	sort_threads;
			string		seq;
			string		tmp_file;
			bool		is_sorted;

			Result(void)
			{
				task = "echo \"Nothing to do!\"";
				container_name = "deque";
				init_time = 0;
				init_threads = 0;
				sort_time = 0;
				sort_threads = 0;
				seq = "";
				is_sorted = false;
			}
		}	Result_t;

		Visualizer(void);
		~Visualizer(void) = default;

		void	Launch(void);

	private:
		Visualizer(const Visualizer&) = delete;
		Visualizer& operator=(const Visualizer&) = delete;

		Visualizer(Visualizer&&) = delete;
		Visualizer& operator=(Visualizer&&) = delete;

		Options_t				options;
		map<string, Result_t>	results;
		int						amount;

		/**
		 * Display a component nicely with a title on the left.
		 * @param name The name of the component (title name).
		 * @param component The component to display on the right.
		 * @returns A freshly created component with the title on the left.
		 */
		Component	Wrap_(const string& name, Component component);
		/**
		 * Launch the program based on the options and display the result.
		 */
		void		RunProgram_(void);
};
