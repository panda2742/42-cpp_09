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

		void	SetContainerName(const std::string&);
		void	SetValgrindEnabled(bool);
		void	SetFlagsEnabled(bool);
		void	SetSortedBefore(bool);
		void	SetSortedAfter(bool);
		void	SetSequenceSize(unsigned int);
		void	SetInitThreadsCount(unsigned short int);
		void	SetSortThreadsCount(unsigned short int);
		void	SetInitTime(unsigned long long);
		void	SetSortTime(unsigned long long);
		void	SetHeapSummary(unsigned char, unsigned long long);
		void	SetErrorsSummary(unsigned char, unsigned long long);

	private:
		std::string							container_name_;
		bool								valgrind_enabled_;
		bool								flags_enabled_;
		bool								is_sorted_before_;
		bool								is_sorted_after_;
		unsigned int						sequence_size_;
		unsigned short int					init_threads_count_;
		unsigned short int					sort_threads_count_;
		unsigned long long					init_time_;
		unsigned long long					sort_time_;
		std::array<unsigned long long, 3>	heap_summary_;
		std::array<unsigned long long, 2>	errors_summary_;
	};
}
