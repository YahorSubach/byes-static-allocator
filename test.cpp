#include <vector>
#include <iostream>

#include "static_allocator.h"

void test_static_allocator()
{
	std::cout << "#\n";

	std::vector<int, byes::memory::static_allocator<int, 256> > v(32);

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

	std::vector<int, byes::memory::static_vector_allocator<int, 32> > v(32);

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

template<int n, int k>
class H : public H<n, k - 1>
{
public:
	static const int alloc_size = n > H<n, k - 1>::alloc_size ? (H<n, k - 1>::alloc_size + H<n, k - 1>::alloc_size / 2) : H<n, k - 1>::alloc_size;
	static const int total_size = n > H<n, k - 1>::alloc_size ? (H<n, k - 1>::total_size + alloc_size) : H<n, k - 1>::total_size;
};

template<int n>
class H<n, 0>
{
public:
	static const int alloc_size = n > 0 ? 1 : 0;
	static const int total_size = alloc_size;
};

template<int n>
class H<n, 1>
{
public:
	static const int alloc_size = (n > H<n, 0>::alloc_size) ? 2 : H<n, 0>::alloc_size;
	static const int total_size = (n > H<n, 0>::alloc_size) ? (H<n, 0>::total_size + alloc_size) : H<n, 0>::total_size;
};

void test_static_vector_allocator_push_back()
{



	std::cout << "#\n";

	std::vector<int, byes::memory::static_vector_allocator<int, 20> > v;

	for (int i = 0; i < 10; i++)
	{
		v.push_back(i + 1);
	}

	for (int i = 0; i < v.size(); i++)
	{
		std::cout << v[i] << ' ';
	}

	std::cout << '\n';
}

void Fill(std::vector<int, byes::memory::static_vector_allocator<int, 64> >& to_fill)
{
	for (int i = 0; i < 64; i++)
	{
		to_fill.push_back(i);
	}
}

int main()
{
	const int size = H<70, 16>::total_size;
	std::vector<int, byes::memory::static_vector_allocator<int, 64> > values;

	Fill(values);
}


//void Fill(std::vector<int, byes::memory::buffer_allocator<int> >& to_fill)
//{
//	for (int i = 0; i < 32; i++)
//	{
//		to_fill.push_back(i);
//	}
//}
//
//int main()
//{
//	int buffer[64];
//	byes::memory::buffer_allocator<int> allocator(buffer, sizeof(buffer));
//
//	std::vector<int, byes::memory::buffer_allocator<int> > values(allocator); // vector may allocate some extra data, so we cannot allocate 64 items
//	values.reserve(32);
//
//	Fill(values);
//}


//void Fill(std::vector<int, byes::memory::static_allocator<int, 64 * sizeof(int)> >& to_fill)
//{
//	for (int i = 0; i < 32; i++)
//	{
//		to_fill.push_back(i);
//	}
//}
//
//int main()
//{
//	std::vector<int, byes::memory::static_allocator<int, 64 * sizeof(int)> > values; // vector may allocate some extra data, so we cannot allocate 64 items
//	values.reserve(32);
//
//	Fill(values);
//}
