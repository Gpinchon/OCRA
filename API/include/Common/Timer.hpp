#pragma once

#include <chrono>

struct Timer
{
	Timer() : start(std::chrono::high_resolution_clock::now()) {}
	inline auto Elapsed() const {
		const auto end = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	}
	const std::chrono::high_resolution_clock::time_point start;
	
};
