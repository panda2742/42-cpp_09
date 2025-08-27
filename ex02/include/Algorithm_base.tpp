#include "Algorithm.hpp"

// ===========================================================
// |                     PUBLIC MEMBERS                      |
// ===========================================================
// ===========================================================
// |                     PRIVATE MEMBERS                     |
// ===========================================================

template <class P>
Algorithm<P>::Algorithm(void) {}

template <class P>
Algorithm<P>::~Algorithm(void) {}

template <class P>
Algorithm<P>::Algorithm(const Algorithm & other)
{
	(void)other;
}

template <class P>
Algorithm<P> &	Algorithm<P>::operator=(const Algorithm<P> & other)
{
	(void)other;
	return *this;
}

template <class P>
typename Algorithm<P>::It	Algorithm<P>::LowerBound(It first, It last, uint64_t value)
{
	return LowerBoundImpl_(first, last, value, typename std::iterator_traits<It>::iterator_category());
}

template <class P>
void	Algorithm<P>::GenerateJacobsthalIndices(Seq & indices, uint64_t n)
{
	return GenerateJacobsthalIndicesImpl_(indices, n, typename std::iterator_traits<It>::iterator_category());
}

template <class P>
void	Algorithm<P>::JacobsthalInsert(Seq & res, Seq & to_insert)
{
	JacobsthalInsertImpl_(res, to_insert, typename std::iterator_traits<It>::iterator_category());
}

template <class P>
typename Algorithm<P>::Seq	Algorithm<P>::Recursion(PairSeq & pairs, bool is_odd, uint64_t isolated_element)
{
	return RecursionImpl_(pairs, is_odd, isolated_element, typename std::iterator_traits<It>::iterator_category());
}

template <class P>
typename Algorithm<P>::Seq &	Algorithm<P>::Sort(Seq & sequence)
{
	return SortImpl_(sequence, typename std::iterator_traits<It>::iterator_category());
}

template <class P>
void	*Algorithm<P>::T_FillChunk(void *void_args)
{
	return T_FillChunkImpl_(void_args, typename std::iterator_traits<It>::iterator_category());
}

