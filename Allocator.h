#pragma once

#include <cstring>
#include <array>

template <class T >
class Allocator {
public:
	using value_type = T;

	Allocator() noexcept;
	Allocator(const Allocator&) noexcept;
	template<typename U>
	Allocator(const Allocator<U> &);
	~Allocator();
	T* allocate(size_t n);
	void deallocate(T* pointer, size_t n);
private:
	static constexpr size_t max_size = 67108864;
	size_t pool_size;
	T* pool;
	T** blocks;
	size_t free_index;
};

template<class T>
Allocator<T>::Allocator() noexcept {
	pool_size = max_size / sizeof(T);
	pool = reinterpret_cast<T*>(new char[pool_size * sizeof(T)]);

	blocks = new T*[pool_size];

	for (size_t i = 0; i < pool_size; i++) {
		blocks[i] = pool + i;
	}

	free_index = 0;
}

template<class T>
Allocator<T>::Allocator(const Allocator& other) noexcept {
	delete[] pool;
	delete[] blocks;

	pool_size = other.pool_size;
	free_index = 0;

	pool = reinterpret_cast<T*>(new char[pool_size * sizeof(T)]);
	blocks = new T*[pool_size];

	std::memcpy(pool, other.pool, pool_size * sizeof(T));
	std::memcpy(blocks, other.blocks, pool_size);
}	

template<class T>
template<typename U>
Allocator<T>::Allocator(const Allocator<U> &) {
	pool_size = max_size / sizeof(T);
	pool = reinterpret_cast<T*>(new char[pool_size * sizeof(T)]);

	blocks = new T*[pool_size];

	for (size_t i = 0; i < pool_size; i++) {
		blocks[i] = pool + i;
	}

	free_index = 0;
}

template<class T>
Allocator<T>::~Allocator() {
	delete[] pool;
	delete[] blocks;
}

template<class T>
T* Allocator<T>::allocate(size_t n) {
	if (n > 1 || free_index == pool_size) {
		throw std::bad_alloc();
	}

	return blocks[free_index++];
}

template<class T>
void Allocator<T>::deallocate(T* pointer, size_t n) {
	assert(0 <= n && n <= 1);
	if (n == 0) {
		return;
	}
	blocks[--free_index] = pointer;
}