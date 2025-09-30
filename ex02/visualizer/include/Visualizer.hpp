#pragma once

#include "ExecutionResultManager.hpp"
#include "InterfaceManager.hpp"
#include "TaskManager.hpp"
#include "Task.hpp"
#include "VisualizerOptions.hpp"
#include <map>
#include <string>
#include <ostream>

#include "ftxui/component/screen_interactive.hpp"

class Visualizer
{
public:
	Visualizer(void) = default;
	~Visualizer(void) = default;

	void	Launch(void);

private:
	visual::VisualizerOptions	options_;
};
