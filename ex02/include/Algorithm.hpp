#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <inttypes.h>
#include <iterator>
#include <deque>
#include <threads.h>
#include <iostream>
#include <vector>
#include "utils.hpp"

#define THREAD_THRESHOLD_INIT 100000
#define MIN_BLOCK_INIT 75000

#define THREAD_THRESHOLD_SORT 50000
#define MIN_BLOCK_SORT 37500

template <class Ctn> struct s_thread_fillchunk_args
{
	const char		**seq;
	unsigned int	start;
	unsigned int	end;
	Ctn				*out;
	bool			error;
};

template <class Seq, class PairSeq> struct s_thread_pairs_args
{
	const PairSeq	*pairs;
	PairSeq			*primary_pairs;
	Seq				*seq_to_insert;
	unsigned int	start;
	unsigned int	end;
};

template <class P>
class Algorithm
{
	public:
		typedef typename P::Allocator		Allocator;
		typedef typename P::Seq				Seq;
		typedef typename P::Pair			Pair;
		typedef typename P::PairAllocator	PairAllocator;
		typedef typename P::PairSeq			PairSeq;
		typedef typename P::BoolAllocator	BoolAllocator;
		typedef typename P::BoolSeq			BoolSeq;
		typedef typename P::Cache			Cache;
		typedef typename P::It				It;
		typedef typename P::ConstIt			ConstIt;

		static unsigned short int	SortThreads;

		static It		LowerBound(It first, It last, uint64_t value);
		static void		GenerateJacobsthalIndices(Seq & indices, uint64_t n);
		static void		JacobsthalInsert(Seq & res, Seq & seq_to_insert);
		static Seq		Recursion(PairSeq & pairs, bool is_odd, uint64_t isolated_element);
		static Seq &	Sort(Seq & sequence);
		static void		*T_FillChunk(void *void_args);
		static void		*T_Pairs(void *void_args);

	private:
		Algorithm(void);
		Algorithm(const Algorithm & other);
		~Algorithm(void);
		Algorithm &		operator=(const Algorithm & other);

		static It					LowerBoundImpl_(It first, It last, uint64_t value, std::random_access_iterator_tag);
		static It					LowerBoundImpl_(It first, It last, uint64_t value, std::bidirectional_iterator_tag);

		static void					GenerateJacobsthalIndicesImpl_(Seq & indices, uint64_t n, std::random_access_iterator_tag);
		static void					GenerateJacobsthalIndicesImpl_(Seq & indices, uint64_t n, std::bidirectional_iterator_tag);

		static void					JacobsthalInsertImpl_(Seq & res, Seq & seq_to_insert, std::random_access_iterator_tag);
		static void					JacobsthalInsertImpl_(Seq & res, Seq & seq_to_insert, std::bidirectional_iterator_tag);

		static Seq					RecursionImpl_(PairSeq & pairs, bool is_odd, uint64_t isolated_element, std::random_access_iterator_tag);
		static Seq					RecursionImpl_(PairSeq & pairs, bool is_odd, uint64_t isolated_element, std::bidirectional_iterator_tag);

		static Seq &				SortImpl_(Seq & sequence, std::random_access_iterator_tag);
		static Seq &				SortImpl_(Seq & sequence, std::bidirectional_iterator_tag);

		static void					*T_FillChunkImpl_(void *void_args, std::random_access_iterator_tag);
		static void					*T_FillChunkImpl_(void *void_args, std::bidirectional_iterator_tag);
		static void					*T_PairsImpl_(void *void_args, std::random_access_iterator_tag);
		static void					*T_PairsImpl_(void *void_args, std::bidirectional_iterator_tag);
};

#include "Algorithm_base.tpp"
#include "Algorithm_BI.tpp"
#include "Algorithm_RAI.tpp"

#endif
