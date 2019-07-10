#ifndef _RASFLY_UTIL_H_
#define _RASFLY_UTIL_H_

#include <bitset>

template<std::size_t N, typename... Args>
inline std::bitset<N> CreateBitSet(Args... bools) {
    static_assert(N == sizeof...(Args), "N must be equal to number of arguments");
    std::bitset<N> set;
    bool bool_array[] = {static_cast<bool>(bools)...};
    for(int i = 0; i < N; ++i) {
        set.set(i, bool_array[i]);
    }
    return set;
}

#endif