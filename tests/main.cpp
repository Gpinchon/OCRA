#include <TestList.hpp>

#include <iostream>

int main()
{
	std::cout << "========== TEST : PhysicalDevice ==========\n\n";
	if (PhysicalDevice() != 0)
		return -1;
	std::cout << std::endl;
	std::cout << "========== TEST : Queue ==========\n\n";
	if (Queue() != 0)
		return -1;
	std::cout << std::endl;
	std::cout << "========== TEST : CommandBuffer ==========\n\n";
	if (CommandBuffer() != 0)
		return -1;
	std::cout << std::endl;
	std::cout << "========== TEST : SwapChain ==========\n\n";
	if (SwapChain() != 0)
		return -1;
	std::cout << std::endl;
	std::cout << "========== TEST : GraphicsPipeline ==========\n\n";
	if (GraphicsPipeline() != 0)
		return -1;
	std::cout << std::endl;
	return 0;
}