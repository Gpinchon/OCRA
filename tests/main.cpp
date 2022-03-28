#include <TestList.hpp>

int main()
{
	if (PhysicalDevice() != 0)
		return -1;
	if (Queue() != 0)
		return -1;
	return 0;
}