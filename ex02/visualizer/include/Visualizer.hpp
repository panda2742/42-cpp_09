#pragma once

#include "VisualizerOptions.hpp"
#include <map>
#include <string>
#include <ostream>

#include "ftxui/component/screen_interactive.hpp"

using namespace std;
using namespace ftxui;

class Visualizer
{
	public:

		Visualizer(void);
		~Visualizer(void) = default;

		void	Launch(void);

	private:
		Visualizer(const Visualizer&) = delete;
		Visualizer(Visualizer&&) = delete;
		Visualizer& operator=(const Visualizer&) = delete;
		Visualizer& operator=(Visualizer&&) = delete;

		visual::VisualizerOptions	options;

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
