#include <vector>
#include <iostream>

#include "static_allocator.h"

void test_static_allocator()
{
	std::cout << "#\n";

	std::vector<int, byes::memory::StaticAllocator<int, 256> > v(32);

	for (int i = 0; i < v.size(); i++)
	{
		v[i] = i + 1;
	}

	for (int i = 0; i < v.size(); i++)
	{
		std::cout << v[i] << ' ';
	}

	std::cout << '\n';

	v.clear();

	for (int i = 0; i < 32; i++)
	{
		v.push_back(32 - i);
	}

	for (int i = 0; i < v.size(); i++)
	{
		std::cout << v[i] << ' ';
	}

	std::cout << '\n';
}

void test_static_vector_allocator()
{
	std::cout << "#\n";

	std::vector<int, byes::memory::StaticVectorAllocator<int, 32> > v(32);

	for (int i = 0; i < v.size(); i++)
	{
		v[i] = i + 1;
	}

	for (int i = 0; i < v.size(); i++)
	{
		std::cout << v[i] << ' ';
	}

	std::cout << '\n';

	v.clear();

	for (int i = 0; i < 32; i++)
	{
		v.push_back(32 - i);
	}

	for (int i = 0; i < v.size(); i++)
	{
		std::cout << v[i] << ' ';
	}

	std::cout << '\n';
}



void test_static_vector_allocator_push_back()
{
	std::cout << "#\n";

	std::vector<int, byes::memory::StaticVectorAllocator<int, 32> > v;

	for (int i = 0; i < 32; i++)
	{
		v.push_back(i + 1);
	}

	for (int i = 0; i < v.size(); i++)
	{
		std::cout << v[i] << ' ';
	}

	std::cout << '\n';
}

int main()
{
	test_static_allocator();
	test_static_vector_allocator();
	test_static_vector_allocator_push_back();
}
