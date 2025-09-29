#include "ExecutionResult.hpp" // ExecutionResult

namespace visual
{

ExecutionResult::ExecutionResult(const std::string& container_name_)
{
	this->container_name = container_name_;
	this->valgrind_enabled = false;
	this->flags_enabled = false;
	this->is_sorted_before = false;
	this->is_sorted_after = false;
	this->sequence_size = 3000;
	this->init_threads_count = 0;
	this->init_threads_count = 0;
	this->init_time = 0;
	this->sort_time = 0;
	this->heap_summary = {0, 0, 0};
	this->errors_summary = {0, 0};
}

}
