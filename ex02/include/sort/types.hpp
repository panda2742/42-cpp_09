#ifndef TYPES_HPP
#define TYPES_HPP

#include <climits>
#include <utility>

#include "inttypes.h"

/**
 * The maximum depth where the threads starts to operate.
 * 255 (2^8-1) or greater. The actual value depends on the particular system and library implementation, but shall
 * reflect the limits of these types in the target platform.
 */
#define THREAD_NSIZE_MAX UCHAR_MAX

typedef unsigned char					thread_nuint64_t;

typedef std::pair<uint64_t, uint64_t>	uint64_pair_t;

#endif
