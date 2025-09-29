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
		ExecutionResult(ExecutionResult&&) = delete;
		ExecutionResult&	operator=(const ExecutionResult&) = default;
		ExecutionResult&	operator=(ExecutionResult&&) = delete;

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
