#pragma once
#include <array>

namespace sp::m3d
{
	template<typename T, size_t _Size>
	struct vec
	{
		typedef T value_type;

		vec(T x = 0.0f);
		vec(const vec<T, _Size>& v);
		~vec();

		const size_t size = _Size;
		std::array<T, _Size> components;
	};
}