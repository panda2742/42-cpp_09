#include "Task.hpp" // Task
#include <string> // string

namespace visual
{

Task::Task(TaskID task_id):
	priority_(0),
	task_("echo 'Hello World!'"),
	tmp_file_(".tmp_" + std::to_string(rand())),
	tmp_file_err_(tmp_file_ + "_err"),
	task_id_(task_id) {}

}
