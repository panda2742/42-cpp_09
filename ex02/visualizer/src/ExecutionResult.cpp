#include "ExecutionResult.hpp"
#include "Task.hpp"
#include <string>
#include <array>

namespace visual
{

ExecutionResult::ExecutionResult(const std::string& container_name)
{
	this->container_name_ = container_name;
	this->valgrind_enabled_ = false;
	this->flags_enabled_ = false;
	this->is_sorted_before_ = false;
	this->is_sorted_after_ = false;
	this->sequence_size_ = 3000;
	this->init_threads_count_ = 0;
	this->sort_threads_count_ = 0;
	this->init_time_ = 0;
	this->sort_time_ = 0;
	this->heap_summary_ = {0, 0, 0};
	this->errors_summary_ = {0, 0};
}

const std::string&	ExecutionResult::GetContainerName(void) const
{
	return container_name_;
}

bool	ExecutionResult::IsValgrindEnabled(void) const
{
	return valgrind_enabled_;
}

bool	ExecutionResult::AreFlagsEnabled(void) const
{
	return flags_enabled_;
}

bool	ExecutionResult::IsSortedBefore(void) const
{
	return is_sorted_before_;
}

bool	ExecutionResult::IsSortedAfter(void) const
{
	return is_sorted_after_;
}

unsigned int	ExecutionResult::GetSequenceSize(void) const
{
	return sequence_size_;
}

unsigned short int	ExecutionResult::GetInitThreadsCount(void) const
{
	return init_threads_count_;
}

unsigned short int	ExecutionResult::GetSortThreadsCount(void) const
{
	return sort_threads_count_;
}

unsigned long long	ExecutionResult::GetInitTime(void) const
{
	return init_time_;
}

unsigned long long	ExecutionResult::GetSortTime(void) const
{
	return sort_time_;
}

const std::array<unsigned long long, 3>&	ExecutionResult::GetHeapSummary(void) const
{
	return heap_summary_;
}

const std::array<unsigned long long, 2>&	ExecutionResult::GetErrorsSummary(void) const
{
	return errors_summary_;
}

unsigned long long	*ExecutionResult::GetHeapSummaryPtr(unsigned char index)
{
	return &heap_summary_[index % 3];
}

unsigned long long	*ExecutionResult::GetErrorsSummaryPtr(unsigned char index)
{
	return &errors_summary_[index % 2];
}

void	ExecutionResult::SetContainerName(const std::string& container_name)
{
	container_name_ = container_name;
}

void	ExecutionResult::SetValgrindEnabled(bool value)
{
	valgrind_enabled_ = value;
}

void	ExecutionResult::SetFlagsEnabled(bool value)
{
	flags_enabled_ = value;
}

void	ExecutionResult::SetSortedBefore(bool value)
{
	is_sorted_before_ = value;
}

void	ExecutionResult::SetSortedAfter(bool value)
{
	is_sorted_after_ = value;
}

void	ExecutionResult::SetSequenceSize(unsigned int size)
{
	sequence_size_ = size;
}

void	ExecutionResult::SetInitThreadsCount(unsigned short int count)
{
	init_threads_count_ = count;
}

void	ExecutionResult::SetSortThreadsCount(unsigned short int count)
{
	sort_threads_count_ = count;
}

void	ExecutionResult::SetInitTime(unsigned long long time)
{
	init_time_ = time;
}

void	ExecutionResult::SetSortTime(unsigned long long time)
{
	sort_time_ = time;
}

void	ExecutionResult::SetHeapSummary(unsigned char index, unsigned long long value)
{
	heap_summary_[index % 3] = value;
}

void	ExecutionResult::SetErrorsSummary(unsigned char index, unsigned long long value)
{
	errors_summary_[index % 2] = value;
}


}
