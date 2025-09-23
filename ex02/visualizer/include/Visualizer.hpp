#pragma once

#include <map>
#include <string>

#include "ftxui/component/screen_interactive.hpp"

#define MAX_VALUE 10000000
#define MAX_VALUE_STR "10000000"

#define VG_FLAGS "--leak-check=full --show-leak-kinds=all --track-origins=yes --show-mismatched-frees=yes --track-fds=yes --trace-children=yes"

using namespace std;
using namespace ftxui;

class Visualizer
{
	public:
		typedef enum TaskID
		{
			TMP_DIR,
			MAKE_FCLEAN,
			MAKE,
			MAKE_BONUS,
			RUN,
			RUN_BONUS,
			VALGRIND_RUN,
			VALGRIND_RUN_BONUS,
			SAVE_OUTPUT,
			GENERATE_INPUT,
			CLEAR
		}	TaskID_t;

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
			TaskID_t		task_id;
			string			task;
			string			tmp_file;
			string			tmp_file_err;
			unsigned char	priority;

			string			container_name;
			double			init_time;
			short int		init_threads;
			double			sort_time;
			short int		sort_threads;
			string			seq;
			bool			is_sorted;

			Result(void)
			{
				task_id = CLEAR,
				task = "true";
				priority = 0;
				container_name = "deque";
				init_time = 0;
				init_threads = 0;
				sort_time = 0;
				sort_threads = 0;
				seq = "";
				is_sorted = false;
			}
			Result(TaskID_t task_id_, const string& task_name, unsigned char priority_val)
			{
				*this = Result();
				this->task_id = task_id_;
				this->task = task_name;
				this->priority = priority_val;

				this->tmp_file = ".tmp_" + to_string(rand());
				this->tmp_file_err = this->tmp_file + "_err";
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
		map<TaskID_t, Result_t>	results;
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
		/**
		 * Load each command based on the options to then run everything and catch the errors.
		 */
		void		PrepareCommands_(void);
		/**
		 * Execute a command and throw an error if something bad happens.
		 * @param ptr The object containing the information of the command (task).
		 */
		void		ExecuteCommand_(Result* ptr);
		/**
		 * Analyzes each output of each command and generates some metrics about it.
		 */
		void		CreateMetrics_(void);
};
