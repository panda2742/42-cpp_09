#pragma once

#include <array> // array
#include <ostream> // array
#include <string> // string

namespace visual
{
	class ExecutionResult
	{
	public:
		ExecutionResult(const std::string&);
		~ExecutionResult(void) = default;
		ExecutionResult(const ExecutionResult&) = default;
		ExecutionResult(ExecutionResult&&) = default;
		ExecutionResult&	operator=(const ExecutionResult&) = default;
		ExecutionResult&	operator=(ExecutionResult&&) = default;

		const std::string&					GetContainerName(void) const;
		bool								IsValgrindEnabled(void) const;
		bool								AreFlagsEnabled(void) const;
		bool								IsSortedBefore(void) const;
		bool								IsSortedAfter(void) const;
		unsigned int						GetSequenceSize(void) const;
		unsigned short int					GetInitThreadsCount(void) const;
		unsigned short int					GetSortThreadsCount(void) const;
		unsigned long long					GetInitTime(void) const;
		unsigned long long					GetSortTime(void) const;
		std::array<unsigned long long, 3>&	GetHeapSummary(void);
		std::array<unsigned long long, 2>&	GetErrorsSummary(void);

	private:
		std::string							container_name;
		bool								valgrind_enabled;
		bool								flags_enabled;
		bool								is_sorted_before;
		bool								is_sorted_after;
		unsigned int						sequence_size;
		unsigned short int					init_threads_count;
		unsigned short int					sort_threads_count;
		unsigned long long					init_time;
		unsigned long long					sort_time;
		std::array<unsigned long long, 3>	heap_summary;
		std::array<unsigned long long, 2>	errors_summary;
	};
}
