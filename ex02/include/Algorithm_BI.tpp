#include "Algorithm.hpp"

// ===========================================================
// |                     PRIVATE MEMBERS                     |
// ===========================================================

template <class P>
typename Algorithm<P>::It	Algorithm<P>::LowerBoundImpl_(It first, It last, uint64_t value, std::bidirectional_iterator_tag)
{
	(void)first;
	(void)last;
	(void)value;
}

template <class P>
void	Algorithm<P>::GenerateJacobsthalIndicesImpl_(Seq & indices, uint64_t n, std::bidirectional_iterator_tag)
{
	(void)indices;
	(void)n;
}

template <class P>
void	Algorithm<P>::JacobsthalInsertImpl_(Seq & res, Seq & to_insert, std::bidirectional_iterator_tag)
{
	(void)res;
	(void)to_insert;
}

template <class P>
typename Algorithm<P>::Seq	Algorithm<P>::RecursionImpl_(PairSeq & pairs, bool is_odd, uint64_t isolated_element, std::bidirectional_iterator_tag)
{
	(void)pairs;
	(void)is_odd;
	(void)isolated_element;
}

template <class P>
typename Algorithm<P>::Seq &	Algorithm<P>::SortImpl_(Seq & sequence, std::bidirectional_iterator_tag)
{
	return sequence;
}

template <class P>
void	*Algorithm<P>::T_FillChunkImpl_(void *void_args, std::bidirectional_iterator_tag)
{
	(void)void_args;
	return NULL;
}

template <class P>
void	*Algorithm<P>::T_PairsImpl_(void *void_args, std::bidirectional_iterator_tag)
{
	(void)void_args;
	return NULL;
}

