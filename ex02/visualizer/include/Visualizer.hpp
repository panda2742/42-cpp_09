#pragma once

#include <map>
#include <string>
#include <ostream>

#include "ftxui/component/screen_interactive.hpp"

#define MAX_VALUE 10000000
#define MAX_VALUE_STR "10000000"

#define VG_FLAGS "--leak-check=full --show-leak-kinds=all --track-origins=yes --show-mismatched-frees=yes --track-fds=yes --trace-children=yes"

using namespace std;
using namespace ftxui;

namespace Visu
{

	typedef struct Options
	{
		int		selectedRunMode;
		int		selectCompilationFlags;
		int		selectedTreatment;
		string	amountOfElements;

		Options(void);
	}	Options_t;

	typedef struct Result
	{
		string							container_name;
		bool							valgrind_enabled;
		bool							flags_enabled;
		bool							is_sorted_before;
		bool							is_sorted_after;
		unsigned int					sequence_size;
		unsigned short int				init_threads_count;
		unsigned short int				sort_threads_count;
		unsigned long long				init_time;
		unsigned long long				sort_time;
		array<unsigned long long, 3>	heap_summary;
		array<unsigned long long, 2>	errors_summary;

		Result(const string& container_name_);
	}	Result_t;
}

ostream&	operator<<(ostream& os, Visu::Result_t res);

class Visualizer
{
	public:

		Visualizer(void);
		~Visualizer(void) = default;

		void	Launch(void);

	private:
		Visualizer(const Visualizer&) = delete;
		Visualizer& operator=(const Visualizer&) = delete;

		Visualizer(Visualizer&&) = delete;
		Visualizer& operator=(Visualizer&&) = delete;

		Visu::Options_t						options;
		map<Visu::TaskID_t, Visu::Task_t>	tasks;
		vector<Visu::Task *>				sorted_tasks;
		map<string, vector<Visu::Result_t>>	results;
		int									amount;

		/**
		 * Launch the program based on the options and display the result.
		 */
		void		RunProgram_(void);
		/**
		 * Load each command based on the options to then run everything and catch the errors.
		 */
		void		PrepareCommands_(void);
		/**
		 * Execute a command and throw an error if something bad happens.
		 * @param ptr The object containing the information of the command (task).
		 */
		void		ExecuteCommand_(Visu::Task *ptr);
		/**
		 * Analyzes each output of each command and generates some metrics about it.
		 */
		void		CreateMetrics_(void);
		/**
		 * Treat a temporary file and reads its content. Can throw an error if something wrong occurs.
		 */
		void		TreatTask_(Visu::Task *ptr);
		/**
		 * Clear the visualizer temporary files and clean the working shell.
		 */
		void		CleanTraces_(void) const;
		/**
		 * Display the metrics result into a new page with beautiful components.
		 */
		void		DisplayMetrics_(void) const;
};
