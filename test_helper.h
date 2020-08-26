#pragma once
#include <list>
#include <type_traits>
#include <random>
#include "Allocator.h"

class LargeObject
{
private:
	int array[1000];
};

class TestHelper
{
public:
	TestHelper() = delete;
	template <class T, class AllocT, class AllocU>
	static bool is_same(const std::list<T, AllocT>& atl_list, const std::list<T, AllocU>& std_list);

	template <class T>
	static void copy(std::list<T, Allocator<T>>& atl_list, const std::list<T>& std_list);
	//fill
	template <class Alloc>
	static void push_rnd_values(std::list<int, Alloc>& list, size_t n = 1000);
	template <class Alloc>
	static void push_rnd_values(std::list<std::string, Alloc>& list, size_t n = 1000,
		size_t min_length = 3, size_t max_length = 100);
	template <class Alloc>
	static void push_rnd_values(std::list<LargeObject, Alloc>& list, size_t n = 1000);

	template <class T>
	static T get_rnd_val() { throw std::bad_exception(); }

	static void reset_rnd() { std::srand(SEED); }

	//operations
	template <class U, class Alloc>
	static void pop_backs(std::list<U, Alloc>& list, size_t n = 1);
	template <class U, class Alloc>
	static void pop_front(std::list<U, Alloc>& list, size_t n = 1);

private:
	static const int SEED = 123;
};

template<class T, class AllocT, class AllocU>
bool TestHelper::is_same(const std::list<T, AllocT>& atl_list, const std::list<T, AllocU>& std_list)
{
	if (atl_list.size() != std_list.size()) {
		return false;
	}

	auto it_a = atl_list.begin();
	auto it_s = std_list.begin();

	for (; it_a != atl_list.end(); it_a++, it_s++) {
		if (*it_a != *it_s) {
			return false;
		}
	}
	return true;
}

template<class T>
void TestHelper::copy(std::list<T, Allocator<T>>& atl_list, const std::list<T>& std_list)
{
	for (auto it = std_list.begin(); it != std_list.end(); it++) {
		atl_list.push_back(*it);
	}
}


template <class Alloc>
void TestHelper::push_rnd_values(std::list<int, Alloc>& list, size_t n)
{
	std::srand(SEED);

	for (size_t i = 0; i < n; i++) {
		list.push_back(TestHelper::get_rnd_val<int>());
	}
}

template <class Alloc>
void TestHelper::push_rnd_values(std::list<std::string, Alloc>& list, size_t n, size_t min_length, size_t max_length)
{
	std::srand(SEED);
	for (size_t i = 0; i < n; i++) {
		list.push_back(TestHelper::get_rnd_val<std::string>());
	}
}

template<class Alloc>
void TestHelper::push_rnd_values(std::list<LargeObject, Alloc>& list, size_t n)
{
	for (size_t i = 0; i < n; i++) {
		list.emplace_back();
	}
}

template<class U, class Alloc>
void TestHelper::pop_backs(std::list<U, Alloc>& list, size_t n)
{
	n = std::min(n, list.size());
	for (size_t i = 0; i < n; i++) {
		list.pop_back();
	}
}

template<class U, class Alloc>
void TestHelper::pop_front(std::list<U, Alloc>& list, size_t n)
{
	n = std::min(n, list.size());
	for (size_t i = 0; i < n; i++) {
		list.pop_front();
	}
}


template<>
int TestHelper::get_rnd_val<int>()
{
	return std::rand();
}

template<>
LargeObject TestHelper::get_rnd_val<LargeObject>()
{
	return LargeObject();
}

template<>
std::string TestHelper::get_rnd_val<std::string>()
{
	static const char alphanum[] = "0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	size_t min_length = 1000;
	size_t max_length = 2000;

	size_t string_length = (rand() % (max_length - min_length)) + min_length;

	std::string rnd_string;
	rnd_string.reserve(string_length);

	for (size_t j = 0; j < string_length; j++) {
		auto rnd_char = alphanum[rand() % (sizeof(alphanum) - 1)];
		rnd_string.push_back(rnd_char);
	}
	return rnd_string;
}
