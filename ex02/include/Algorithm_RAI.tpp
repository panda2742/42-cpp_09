#include "Algorithm.hpp"

// ===========================================================
// |                     STATIC SYMBOLS                      |
// ===========================================================

template <class Container> static void		_ReserveSpace(Container& c, size_t size) { (void)c; (void)size; }
template <class T, class Alloc> static void	_ReserveSpace(std::vector<T, Alloc>& c, size_t size) { c.reserve(size); }
template <class T> static void	_ReserveSpace(std::deque<T>& c, size_t size) { c.resize(size); }

// ===========================================================
// |                     PRIVATE MEMBERS                     |
// ===========================================================

template <class P>
typename Algorithm<P>::It	Algorithm<P>::LowerBoundImpl_(It first, It last, uint64_t value, std::random_access_iterator_tag)
{
	if (first == last) return first;
	if (value <= *first) return first;
	if (value > *(last - 1)) return last;

	typename std::iterator_traits<It>::difference_type	count = last - first,
														step;
	It	it;

	while (count > 0)
	{
		step = count / 2;
		it = first + step;

		if (*it < value)
		{
			first = ++it;
			count -= step + 1;
		}
		else count = step;
	}

	return first;
}

template <class P>
void	Algorithm<P>::GenerateJacobsthalIndicesImpl_(Seq & indices, uint64_t n, std::random_access_iterator_tag)
{
	static Cache	cache;

	indices.clear();
	if (n == 0) return;

	typename Cache::iterator	it = cache.find(n);

	if (it == cache.end())
	{
		Seq	jacobsthal;
		jacobsthal.push_back(1);
		if (n > 1)
			jacobsthal.push_back(1);

		while (jacobsthal.back() < n)
		{
			uint64_t	next = jacobsthal[jacobsthal.size() - 1] + 2 * jacobsthal[jacobsthal.size() - 2];
			if (next > n)
				break;
			jacobsthal.push_back(next);
		}

		Seq		order;
		BoolSeq	used(n, false);

		order.push_back(0);
		used[0] = true;

		for (uint64_t i = 2; i < jacobsthal.size(); ++i)
		{
			uint64_t	start = jacobsthal[i] - 1;
			uint64_t	end   = jacobsthal[i - 1];

			for (uint64_t j = start; j >= end && j < n; --j)
			{
				if (!used[j])
				{
					order.push_back(j);
					used[j] = true;
				}
				if (j == 0)
					break;
			}
		}

		for (uint64_t i = 0; i < n; ++i)
			if (!used[i])
				order.push_back(i);

		it = cache.insert(std::make_pair(n, order)).first;
	}

	for (ConstIt jt = it->second.begin(); jt != it->second.end(); ++jt)
		indices.push_back(static_cast<uint64_t>(*jt));
}

template <class P>
void	Algorithm<P>::JacobsthalInsertImpl_(Seq & res, Seq & to_insert, std::random_access_iterator_tag)
{
	if (to_insert.empty()) return;

	Seq	jacobsthal_indices;
	Algorithm<P>::GenerateJacobsthalIndices(jacobsthal_indices, to_insert.size());

	for (uint64_t	i = 0; i < jacobsthal_indices.size(); ++i)
	{
		uint64_t	idx = jacobsthal_indices[i];

		if (idx < to_insert.size())
		{
			uint64_t	element = to_insert[idx];
			It			pos = Algorithm<P>::LowerBound(res.begin(), res.end(), element);
			res.insert(pos, element);
		}
	}
}

template <class P>
typename Algorithm<P>::Seq	Algorithm<P>::RecursionImpl_(PairSeq & pairs, bool is_odd, uint64_t isolated_element, std::random_access_iterator_tag)
{
	Seq	res;

	if (pairs.empty())
	{
		if (is_odd && isolated_element != ULONG_MAX)
			res.push_back(isolated_element);

		return res;
	}

	if (pairs.size() == 1)
	{
		res.push_back(pairs[0].second);
		res.push_back(pairs[0].first);

		if (is_odd && isolated_element != ULONG_MAX)
		{
			if (isolated_element < res[0]) res.insert(res.begin(), isolated_element);
			else if (isolated_element < res[1]) res.insert(res.begin() + 1, isolated_element);
			else res.push_back(isolated_element);
		}

		return res;
	}

	PairSeq		primary_pairs;
	Seq			seq_to_insert;
	bool		new_is_odd = pairs.size() % 2;
	uint64_t	new_isolated_element = ULONG_MAX;

	_ReserveSpace(res, pairs.size() * 2 + is_odd);

	if (new_is_odd)
	{
		new_isolated_element = pairs.back().first;
		seq_to_insert.push_back(pairs.back().second);
		pairs.pop_back();
	}

	if (pairs.size() < THREAD_THRESHOLD_SORT)
	{
		for (uint64_t	i = 0; i < pairs.size(); i += 2)
		{
			if (pairs[i].first > pairs[i + 1].first)
				primary_pairs.push_back(std::make_pair(pairs[i].first, pairs[i + 1].first));
			else
				primary_pairs.push_back(std::make_pair(pairs[i + 1].first, pairs[i].first));

			seq_to_insert.push_back(pairs[i].second);
			seq_to_insert.push_back(pairs[i + 1].second);
		}

		res = Recursion(primary_pairs, new_is_odd, new_isolated_element);

		Algorithm<P>::JacobsthalInsert(res, seq_to_insert);

		if (is_odd && isolated_element != ULONG_MAX)
		{
			It	pos = res.begin();

			while (pos != res.end() && *pos < isolated_element)
				pos++;

			res.insert(pos, isolated_element);
		}

		return res;
	}

	_ReserveSpace(primary_pairs, pairs.size() / 2);
	_ReserveSpace(seq_to_insert, pairs.size());

	unsigned short int	max_threads = get_hardware_concurrency();
	if (max_threads == 0) max_threads = 2;

	uint64_t	potential = pairs.size() / MIN_BLOCK_SORT;
	if (potential == 0) potential = 1;

	unsigned short int	nthreads = static_cast<unsigned short int>(potential);
	if (nthreads > max_threads) nthreads = max_threads;
	else if (nthreads > pairs.size()) nthreads = static_cast<unsigned short int>(pairs.size());

	uint64_t	block = (pairs.size() + nthreads - 1) / nthreads;

	pthread_t							threads[MAX_THREADS];
	s_thread_pairs_args<Seq, PairSeq>	args[MAX_THREADS];

	uint64_t	start = 0;
	for (unsigned short int	i = 0; i < nthreads; ++i)
	{
		uint64_t	end = (i == nthreads - 1) ? pairs.size() : start + block;
		end = end & ~1;

		args[i].pairs = &pairs;
		args[i].primary_pairs = &primary_pairs;
		args[i].seq_to_insert = &seq_to_insert;
		args[i].start = start;
		args[i].end = end;

		pthread_create(&threads[i], NULL, &Algorithm<P>::T_Pairs, &args[i]);
		start = end;
	}

	SortThreads += nthreads;

	for (unsigned short int	i = 0; i < nthreads; ++i)
		pthread_join(threads[i], NULL);

	res = Recursion(primary_pairs, new_is_odd, new_isolated_element);

	Algorithm<P>::JacobsthalInsert(res, seq_to_insert);

	if (is_odd && isolated_element != ULONG_MAX)
	{
		It	pos = res.begin();

		while (pos != res.end() && *pos < isolated_element)
			pos++;

		res.insert(pos, isolated_element);
	}

	return res;
}

template <class P>
typename Algorithm<P>::Seq &	Algorithm<P>::SortImpl_(Seq & sequence, std::random_access_iterator_tag)
{
	if (sequence.size() < 2)
		return sequence;

	const bool	is_odd = sequence.size() % 2;
	uint64_t	isolated_element = ULONG_MAX;

	if (is_odd)
	{
		isolated_element = sequence.back();
		sequence.pop_back();
	}

	PairSeq	pairs;
	_ReserveSpace(pairs, sequence.size() / 2 + 1);

	for (uint64_t	i = 0; i < sequence.size(); i += 2)
	{
		if (sequence[i] > sequence[i + 1])
			pairs.push_back(std::make_pair(sequence[i], sequence[i + 1]));
		else
			pairs.push_back(std::make_pair(sequence[i + 1], sequence[i]));
	}

	sequence = Algorithm<P>::Recursion(pairs, is_odd, isolated_element);
	return sequence;
}

template <class P>
void	*Algorithm<P>::T_FillChunkImpl_(void *void_args, std::random_access_iterator_tag)
{
	s_thread_fillchunk_args<typename P::Seq>	*args = reinterpret_cast<s_thread_fillchunk_args<typename P::Seq> *>(void_args);
	args->error = false;

	for (uint64_t	i = args->start; i < args->end; i++)
	{
		if (args->seq[i][0] == '\0' || args->seq[i][0] == '-')
		{
			args->error = true;
			return NULL;
		}

		char	*endptr;
		errno = 0;
		uint64_t	value = std::strtoul(args->seq[i], &endptr, 10);

		if (*endptr != '\0' || (value == ULONG_MAX && errno == ERANGE))
		{
			args->error = true;
			return NULL;
		}

		args->out->push_back(value);
	}

	return NULL;
}

template <class P>
void	*Algorithm<P>::T_PairsImpl_(void *void_args, std::random_access_iterator_tag)
{
	s_thread_pairs_args<typename P::Seq, typename P::PairSeq>	*args = reinterpret_cast<s_thread_pairs_args<typename P::Seq, typename P::PairSeq> *>(void_args);

	for (unsigned int	i = args->start; i < args->end; i += 2)
	{
		unsigned int	pair_index = i / 2;
		unsigned int	insert_index = i;

		if ((*args->pairs)[i].first > (*args->pairs)[i + 1].first)
		{
			(*args->primary_pairs)[pair_index] = std::make_pair(
				(*args->pairs)[i].first,
				(*args->pairs)[i + 1].first
			);
		}
		else
		{
			(*args->primary_pairs)[pair_index] = std::make_pair(
				(*args->pairs)[i + 1].first,
				(*args->pairs)[i].first
			);
		}

		(*args->seq_to_insert)[insert_index] = (*args->pairs)[i].second;
		(*args->seq_to_insert)[insert_index + 1] = (*args->pairs)[i + 1].second;
	}

	return NULL;
}
