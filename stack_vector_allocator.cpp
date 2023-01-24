// stack_vector_allocator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>

#include "onstack_vector_allocator.h"

int main()
{
	byes::onstack_vector_allocator<int, 64> allocator = byes::onstack_vector_allocator<int, 64>::Construct();
	std::vector<int, byes::onstack_vector_allocator<int, 64>> v(allocator);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
}
