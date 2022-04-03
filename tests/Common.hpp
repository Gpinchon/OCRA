#pragma once
/**
 * @brief Unit test's common tools
 */

#include <Common/Timer.hpp>

#include <iostream>

struct VerboseTimer : Timer
{
	VerboseTimer(const std::string& a_Name) : name(a_Name) {}
	~VerboseTimer() {
		std::cout << "Timer \"" << name << "\" waited : " << Elapsed().count() << " nanoseconds\n";
	}
	const std::string name;
};
