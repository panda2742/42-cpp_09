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

const std::string&	ExecutionResult::GetContainerName(void) const
{
	return container_name;
}

bool	ExecutionResult::IsValgrindEnabled(void) const
{
	return valgrind_enabled;
}

bool	ExecutionResult::AreFlagsEnabled(void) const
{
	return flags_enabled;
}

bool	ExecutionResult::IsSortedBefore(void) const
{
	return is_sorted_before;
}

bool	ExecutionResult::IsSortedAfter(void) const
{
	return is_sorted_after;
}

unsigned int	ExecutionResult::GetSequenceSize(void) const
{
	return sequence_size;
}

unsigned short int	ExecutionResult::GetInitThreadsCount(void) const
{
	return init_threads_count;
}

unsigned short int	ExecutionResult::GetSortThreadsCount(void) const
{
	return sort_threads_count;
}

unsigned long long	ExecutionResult::GetInitTime(void) const
{
	return init_time;
}

unsigned long long	ExecutionResult::GetSortTime(void) const
{
	return sort_time;
}

std::array<unsigned long long, 3>&	ExecutionResult::GetHeapSummary(void)
{
	return heap_summary;
}

std::array<unsigned long long, 2>&	ExecutionResult::GetErrorsSummary(void)
{
	return errors_summary;
}

}
