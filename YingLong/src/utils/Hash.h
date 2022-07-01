#pragma once

#include <utility>

namespace YingLong {

	struct HashPair
	{
		template <typename T1, typename T2>
		size_t operator()(const std::pair<T1, T2>& Pair) const
		{
			auto hash1 = std::hash<T1>{}(Pair.first);
			auto hash2 = std::hash<T2>{}(Pair.second);
			return hash1 ^ hash2;
		}
	};
}