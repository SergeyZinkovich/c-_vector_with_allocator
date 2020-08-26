#pragma once
#include <cstring>
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <iterator>
#include <utility>
#include <ctgmath>
#include <initializer_list>
#include <vector>
#include "Iterator.h"

template <typename T, typename Allocator = std::allocator<T>> class Vector;

template <typename T, typename Allocator>
class Vector {
public:
	using value_type = T;
	using reference = value_type&;
	using const_reference = const value_type&;
	using allocator_type = Allocator;
	using difference_type = std::ptrdiff_t;  
	using size_type = std::size_t;
	using pointer = typename std::allocator_traits<allocator_type>::pointer;
	using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;  
	
	using iterator = VectorIterator<T, false>;
    using const_iterator = VectorIterator<T, true>;

	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	//constructors
	explicit Vector(const Allocator& alloc = Allocator());
	explicit Vector(size_type count, const Allocator& alloc = Allocator());                                    
	Vector(size_type count, const T& value, const Allocator& alloc = Allocator());
	template <typename InputIterator, class = typename std::iterator_traits<InputIterator>::iterator_category>     
	Vector(InputIterator first, InputIterator last, const Allocator& = Allocator());     

	Vector(const Vector&);
	Vector(const Vector& other, const Allocator& alloc);

	Vector(Vector&& other) noexcept;                       
	Vector(Vector&& other, const Allocator& alloc); 

	Vector(std::initializer_list<T> init, const Allocator& alloc = Allocator());  

	~Vector();

	//value assign
	Vector<T, Allocator>& operator=(const Vector<T, Allocator>& other);
	Vector<T, Allocator>& operator=(Vector<T, Allocator>&& other);
	Vector<T, Allocator>& operator=(std::initializer_list<T> ilist);

	template <class InputIterator, class = typename std::iterator_traits<InputIterator>::iterator_category>
	void assign(InputIterator first, InputIterator last);

	void assign(size_type n, const T& value);
	void assign(std::initializer_list<T> init);

	allocator_type get_allocator() const noexcept;

	//elemtnt acces
	reference at(size_type pos);
	const_reference at(size_type pos) const;
	reference operator[](size_type pos);
	const_reference operator[](size_type pos) const;
	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;
	T* data() noexcept;
	const T* data() const noexcept;

	//iterators
	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	const_iterator cbegin() const noexcept;
	iterator end() noexcept;
	const_iterator end() const noexcept;
	const_iterator cend() const noexcept;
	reverse_iterator rbegin() noexcept;
	const_reverse_iterator rbegin() const noexcept;
	const_reverse_iterator crbegin() const noexcept;
	reverse_iterator rend() noexcept;
	const_reverse_iterator rend() const noexcept;
	const_reverse_iterator crend() const noexcept;

	//capacity
	bool empty() const noexcept;
	size_type size() const noexcept;
	size_type max_size() const noexcept;
	void reserve(size_type new_cap);
	size_type capacity() const noexcept;
	void shrink_to_fit();

	//modifiers
	void clear() noexcept;
	iterator insert(const_iterator pos, const T& value);
	iterator insert(const_iterator pos, T&& value);
	iterator insert(const_iterator pos, size_type count, const T& value);
	template <class InputIterator, class = typename std::iterator_traits<InputIterator>::iterator_category>
	iterator insert(const_iterator position, InputIterator first, InputIterator last);
	iterator insert(const_iterator pos, std::initializer_list<T> ilist);
	template< class... Args >
	iterator emplace(const_iterator pos, Args&&... args);
	iterator erase(const_iterator pos);
	iterator erase(const_iterator first, const_iterator last);
	void push_back(const T& value);
	void push_back(T&& value);
	template< class... Args >
	void emplace_back(Args&&... args);
	void pop_back();
	void resize(size_type count);
	void resize(size_type count, const value_type& value);
	void swap(Vector& other);

	//operators
	template< class T, class Alloc >
	friend bool operator==(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs);

	template< class T, class Alloc >
	friend bool operator!=(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs);

	template< class T, class Alloc >
	friend bool operator<(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs);

	template< class T, class Alloc >
	friend bool operator<=(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs);

	template< class T, class Alloc >
	friend bool operator>(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs);

	template< class T, class Alloc >
	friend bool operator>=(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs);

private:
	const double _increaseCoefficient = 1.5;
	allocator_type _allocator;
	size_type _size;
	size_type _capacity;
	pointer _data; 

	void fill_default(size_type from = 0);         
	void fill_value(const T& value, size_type from = 0);
	void copy_vector(const Vector& other);
	template<class It>
	void copy_from_iterator(It first, It last);
	void destruct_data(size_type from = 0);
	void reserve_to_add(size_type size = 1);
	void move_to_new(pointer data);
	void shift_right(const_iterator pos, difference_type distance = 1);
	void shift_left(const_iterator pos, difference_type distance = 1);
};

//constructors
template<typename T, typename Allocator>
Vector<T, Allocator>::Vector(const Allocator& alloc)
	: _allocator(alloc), 
	_size(0),
	_capacity(_size),
	_data(std::allocator_traits<Allocator>::allocate(_allocator, _capacity)) {}

template<class T, class Allocator>
Vector<T, Allocator>::Vector(size_type count, const Allocator& alloc)
	: _allocator(alloc),
	_data(std::allocator_traits<Allocator>::allocate(_allocator, count)),
	_size(count),
	_capacity(count)
{
	fill_default();
}

template<class T, class Allocator>
Vector<T, Allocator>::Vector(Vector::size_type count, const T& value, const Allocator& alloc)
	: _allocator(alloc),
	_data(std::allocator_traits<Allocator>::allocate(_allocator, count)),
	_size(count),
	_capacity(count)
{
	fill_value(value);
}

template<class T, class Allocator>
template<class InputIterator, class>
Vector<T, Allocator>::Vector(InputIterator first, InputIterator last, const Allocator& alloc)
	: _allocator(alloc),
	_data(std::allocator_traits<Allocator>::allocate(_allocator, std::distance(first, last))),
	_size(static_cast<size_type>(std::distance(first, last))),
	_capacity(_size)
{
	int i = 0;
	for (auto it = first; it != last; it++, i++) {
		std::allocator_traits<Allocator>::construct(_allocator, _data + i, *it);
	}
}

template<class T, class Allocator>
Vector<T, Allocator>::Vector(const Vector& other)
	: _allocator(std::allocator_traits<Allocator>::select_on_container_copy_construction(other.get_allocator())),
	_data(std::allocator_traits<Allocator>::allocate(_allocator, other._capacity)),
	_size(other._size),
	_capacity(other._capacity)
{
	copy_vector(other);
}

template<class T, class Allocator>
Vector<T, Allocator>::Vector(const Vector& other, const Allocator& alloc)
	: _allocator(alloc),
	_data(std::allocator_traits<Allocator>::allocate(_allocator, other._capacity)),
	_size(other._size),
	_capacity(other._capacity)
{

	copy_vector(other);
}

template<class T, class Allocator>
Vector<T, Allocator>::Vector(Vector&& other) noexcept
	:  _allocator(std::move(other._allocator)),
	_data(std::move(other._data)),
	_size(std::move(other._size)),
	_capacity(std::move(other._capacity))
{
	other._size = 0;
	other._capacity = 0;
	other._data = nullptr;
}

template<class T, class Allocator>
Vector<T, Allocator>::Vector(Vector&& other, const Allocator& alloc)
	: _allocator(alloc),
	_data(std::move(other._data)),
	_size(std::move(other._size)),
	_capacity(std::move(other._capacity))
{
	other._size = 0;
	other._capacity = 0;
	other._data = nullptr;
}

template<class T, class Allocator>
Vector<T, Allocator>::Vector(std::initializer_list<T> init, const Allocator& alloc)
	: _allocator(alloc),
	_data(std::allocator_traits<Allocator>::allocate(_allocator, init.size())),
	_size(init.size()),
	_capacity(init.size())
{
	copy_from_iterator(init.begin(), init.end());
}

template<class T, class Allocator>
Vector<T, Allocator>::~Vector() {
	destruct_data();
}

//value assign
template<class T, class Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(const Vector<T, Allocator>& other) {
	destruct_data();
	_allocator = other._allocator;
	_capacity = other._capacity;
	_size = other._size;
	_data = std::allocator_traits<Allocator>::allocate(_allocator, other.capacity());
	copy_vector(other);
	return *this;
}

template<class T, class Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(Vector<T, Allocator>&& other) {
	destruct_data();
	_allocator = std::move(other._allocator);
	_capacity = std::move(other._capacity);
	_size = std::move(other._size);
	_data = std::move(other._data);
	return *this;
}

template<class T, class Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(std::initializer_list<T> ilist) {
	Vector tmp = Vector(std::move(ilist));
	swap(tmp);
	return *this;
}

template<class T, class Allocator>
template<class InputIterator, class>
void Vector<T, Allocator>::assign(InputIterator first, InputIterator last)
{
	destruct_data();
	size_type size = std::distance(first, last);
	reserve_to_add(size);
	_size = size;
	copy_from_iterator(first, last);
}

template<class T, class Allocator>
void Vector<T, Allocator>::assign(size_type count, const T& value) {
	destruct_data();
	reserve_to_add(count);
	_size = count;
	fill_value(value);
}

template<class T, class Allocator>
void Vector<T, Allocator>::assign(std::initializer_list<T> ilist) {
	assign(ilist.begin(), ilist.end());
}

template<class T, class Allocator>
typename Vector<T, Allocator>::allocator_type Vector<T, Allocator>::get_allocator() const noexcept {
	return _allocator;
}

//elemtnt acces
template<class T, class Allocator>
typename Vector<T, Allocator>::reference Vector<T, Allocator>::at(size_type pos) {
	if (pos < 0 || _size <= pos) {
		throw std::out_of_range("Выход за границы массива");
	}
	return _data[pos];
}

template<class T, class Allocator>
typename Vector<T, Allocator>::const_reference Vector<T, Allocator>::at(size_type pos) const {
	if (pos < 0 || _size <= pos) {
		throw std::out_of_range("Выход за границы массива");
	}
	return _data[pos];
}

template<class T, class Allocator>
typename Vector<T, Allocator>::reference Vector<T, Allocator>::operator[](size_type pos) {
	return at(pos);
}

template<class T, class Allocator>
typename Vector<T, Allocator>::const_reference Vector<T, Allocator>::operator[](size_type pos) const {
	return at(pos);
}

template<class T, class Allocator>
typename Vector<T, Allocator>::reference Vector<T, Allocator>::front() {
	return _data[0];
}

template<class T, class Allocator>
typename Vector<T, Allocator>::const_reference Vector<T, Allocator>::front() const {
	return _data[0];
}

template<class T, class Allocator>
typename Vector<T, Allocator>::reference Vector<T, Allocator>::back() {
	return _data[_size - 1];
}

template<class T, class Allocator>
typename Vector<T, Allocator>::const_reference Vector<T, Allocator>::back() const {
	return _data[_size - 1];
}

template<class T, class Allocator>
T* Vector<T, Allocator>::data() noexcept {
	if (empty()) {
		return nullptr;
	}
	else {
		return _data;
	}
}

template<class T, class Allocator>
const T* Vector<T, Allocator>::data() const noexcept {
	if (empty()) {
		return nullptr;
	}
	else {
		return _data;
	}
}

//iterators
template<class T, class Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::begin() noexcept {
	return iterator(_data);
}

template<class T, class Allocator>
typename Vector<T, Allocator>::const_iterator Vector<T, Allocator>::begin() const noexcept {
	return iterator(_data);
}

template<class T, class Allocator>
typename Vector<T, Allocator>::const_iterator Vector<T, Allocator>::cbegin() const noexcept {
	return const_iterator(_data);
}

template<class T, class Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::end() noexcept {
	return iterator(_data, _size);
}

template<class T, class Allocator>
typename Vector<T, Allocator>::const_iterator Vector<T, Allocator>::end() const noexcept {
	return const_iterator(_data, _size);
}

template<class T, class Allocator>
typename Vector<T, Allocator>::const_iterator Vector<T, Allocator>::cend() const noexcept {
	return const_iterator(_data, _size);
}

template<class T, class Allocator>
typename Vector<T, Allocator>::reverse_iterator Vector<T, Allocator>::rbegin() noexcept {
	return reverse_iterator(end())
}

template<class T, class Allocator>
typename Vector<T, Allocator>::const_reverse_iterator Vector<T, Allocator>::rbegin() const noexcept {
	return const_reverse_iterator(end())
}

template<class T, class Allocator>
typename Vector<T, Allocator>::const_reverse_iterator Vector<T, Allocator>::crbegin() const noexcept {
	return const_reverse_iterator(end());
}
template<class T, class Allocator>
typename Vector<T, Allocator>::reverse_iterator Vector<T, Allocator>::rend() noexcept {
	return reverse_iterator(begin());
}
template<class T, class Allocator>
typename Vector<T, Allocator>::const_reverse_iterator Vector<T, Allocator>::rend() const noexcept {
	return const_reverse_iterator(begin());
}
template<class T, class Allocator>
typename Vector<T, Allocator>::const_reverse_iterator Vector<T, Allocator>::crend() const noexcept {
	return const_reverse_iterator(begin());
}

//capacity
template<class T, class Allocator>
bool Vector<T, Allocator>::empty() const noexcept {
	return _size == 0;
}

template<class T, class Allocator>
typename Vector<T, Allocator>::size_type Vector<T, Allocator>::size() const noexcept {
	return _size;
}

template<class T, class Allocator>
typename Vector<T, Allocator>::size_type Vector<T, Allocator>::max_size() const noexcept {
	return std::numeric_limits<size_type>::max() / sizeof(T);
}

template<class T, class Allocator>
void Vector<T, Allocator>::reserve(size_type new_cap) {
	if (new_cap > max_size()) {
		throw std::length_error("Превышен предел выделения памяти");
	}
	if (new_cap <= _capacity) {
		return;
	}
	auto new_data = std::allocator_traits<Allocator>::allocate(_allocator, new_cap);
	move_to_new(new_data);
	std::allocator_traits<Allocator>::deallocate(_allocator, _data, _capacity);
	_data = new_data;
	_capacity = new_cap;
}

template<class T, class Allocator>
typename Vector<T, Allocator>::size_type Vector<T, Allocator>::capacity() const noexcept {
	return _capacity;
}

template<class T, class Allocator>
void Vector<T, Allocator>::shrink_to_fit() {
	size_type extra_length = capacity_ - size_;
	if (extra_length == 0) {
		return;
	}
	pointer new_data = std::allocator_traits<Allocator>::allocate(_allocator, _size);
	move_to_new(new_data);
	std::allocator_traits<Allocator>::deallocate(_allocator, _data, _capacity);
	_data = new_data;
	_capacity = _size;
}

//modifiers
template<class T, class Allocator>
void Vector<T, Allocator>::clear() noexcept {
	for (size_type i = 0; i < _size; i++) {
		std::allocator_traits<Allocator>::destroy(_allocator, _data + i);
	}
	_size = 0;
}

template<class T, class Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert(const_iterator pos, const T& value) {
	shift_right(pos);
	std::allocator_traits<Allocator>::construct(_allocator, _data + pos.pos(), std::forward<const T&>(value));
	_size++;
	return iterator(_data, pos.pos());
}

template<class T, class Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert(const_iterator pos, T&& value) {
	shift_right(pos);
	std::allocator_traits<Allocator>::construct(_allocator, _data + pos.pos(), std::forward<T&&>(value));
	_size++;
	return iterator(_data, pos.pos());
}

template<class T, class Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert(const_iterator pos, size_type count, const T& value) {
	shift_right(pos, count);
	for (size_type i = pos.pos(); i < pos.pos() + count; i++) {
		std::allocator_traits<Allocator>::construct(_allocator, _data + i, std::forward<const T&>(value));
	}
	_size += count;
	return iterator(_data, pos.pos());
}

template<class T, class Allocator>
template<class InputIterator, class>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert(Vector::const_iterator pos, InputIterator first, InputIterator last)
{
	size_type size = last - first;
	shift_right(pos, size);
	size_type i = pos.pos();
	for (auto it = first; it != last; it++) {
		std::allocator_traits<Allocator>::construct(_allocator, _data + i, *it);
		i++;
	}
	_size += size;
	return iterator(_data, pos.pos());
}

template<class T, class Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert(const_iterator pos, std::initializer_list<T> ilist) {
	return insert(pos, ilist.begin(), ilist.end());
}

template<class T, class Allocator>
template< class... Args >
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::emplace(const_iterator pos, Args&&... args) {
	shift_right(pos);
	std::allocator_traits<Allocator>::construct(_allocator, _data + pos.pos(), std::forward<Args&&>(args)...);
	_size++;
	return iterator(_data, pos.pos());
}

template<class T, class Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::erase(const_iterator pos) {
	if (empty()) {
		return end();
	}
	shift_left(pos + 1);
	_size--;
	return iterator(_data, pos.pos());
}

template<class T, class Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::erase(const_iterator first, const_iterator last) {
	if (empty()) {
		return end();
	}
	shift_left(last, last - first);
	_size -= last - first;
	return iterator(_data, last.pos() - (last - first));
}

template<class T, class Allocator>
void Vector<T, Allocator>::push_back(const T& value) {
	reserve_to_add();
	std::allocator_traits<Allocator>::construct(_allocator, _data + _size, std::forward<const T&>(value));
	_size++;
}

template<class T, class Allocator>
void Vector<T, Allocator>::push_back(T&& value) {
	reserve_to_add();
	std::allocator_traits<Allocator>::construct(_allocator, _data + _size, std::forward<T&&>(value));
	_size++;
}

template<class T, class Allocator>
template< class... Args >
void Vector<T, Allocator>::emplace_back(Args&&... args) {
	reserve_to_add();
	std::allocator_traits<Allocator>::construct(_allocator, _data + _size, std::forward<Args&&>(args)...);
	_size++;
}

template<class T, class Allocator>
void Vector<T, Allocator>::pop_back() {
	if (empty()) {
		return;
	}
	_size--;
	std::allocator_traits<Allocator>::destroy(_allocator, _data + _size);
}

template<class T, class Allocator>
void Vector<T, Allocator>::resize(size_type count) {
	if (count == _size) {
		return;
	}
	if (count < _size) {
		destruct_data(count);
		_size = count;
		return;
	}
	if (count > _capacity) {
		auto new_data = std::allocator_traits<Allocator>::allocate(_allocator, count);
		move_to_new(new_data);
		std::allocator_traits<Allocator>::deallocate(_allocator, _data, _capacity);
		_data = new_data;
		_capacity = count;
	}
	size_type old_size = _size;
	_size = count;
	fill_default(old_size);
}

template<class T, class Allocator>
void Vector<T, Allocator>::resize(size_type count, const value_type& value) {
	if (count == _size) {
		return;
	}
	if (count < _size) {
		destruct_data(count);
		_size = count;
		return;
	}
	if (count > _capacity) {
		auto new_data = std::allocator_traits<Allocator>::allocate(_allocator, count);
		move_to_new(new_data);

		std::allocator_traits<Allocator>::deallocate(_allocator, _data, _capacity);
		_data = new_data;
		_capacity = count;
	}
	for (size_type i = _size; i < count; i++) {
		std::allocator_traits<Allocator>::construct(_allocator, _data + i, value);
	}
	_size = count;
}

template<class T, class Allocator>
void Vector<T, Allocator>::swap(Vector& other) {
	std::swap(_data, other._data);
	std::swap(_size, other._size);
	std::swap(_capacity, other._capacity);
}

template<class T, class Alloc>
bool operator==(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
{
	if (lhs._size != rhs._size) {
		return false;
	}

	auto itL = lhs.begin();
	auto itR = rhs.begin();

	for (; itL != lhs.end(); itL++, itR++) {
		if (*itL != *itR) {
			return false;
		}
	}

	return true;
}

template<class T, class Alloc>
bool operator<(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
{
	auto size = std::min(lhs.size(), rhs.size());

	for (typename vector<T, Alloc>::size_type i = 0; i < size; i++) {
		if (lhs[i] < rhs[i]) {
			return true;
		}
	}
	return lhs.size() < rhs.size();
}

template<class T, class Alloc>
bool operator!=(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
{
	return !(lhs == rhs);
}

template<class T, class Alloc>
bool operator> (const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
{
	return rhs < lhs;
}

template<class T, class Alloc>
bool operator<=(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
{
	auto size = std::min(lhs.size(), rhs.size());

	for (typename vector<T, Alloc>::size_type i = 0; i < size; i++) {
		if (lhs[i] < rhs[i]) {
			return true;
		}
	}
	return lhs.size() == rhs.size();
}

template<class T, class Alloc>
bool operator>=(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
{
	return rhs <= lhs;
}

template<class T, class Allocator>
void Vector<T, Allocator>::fill_default(size_type from) {
	for (size_type i = from; i < _size; i++) {
		std::allocator_traits<Allocator>::construct(_allocator, _data + i);
	}
}

template<class T, class Allocator>
void Vector<T, Allocator>::fill_value(const T& value, size_type from) {
	for (size_type i = from; i < _size; i++) {
		std::allocator_traits<Allocator>::construct(_allocator, _data + i, value);
	}
}

template<class T, class Allocator>
void Vector<T, Allocator>::copy_vector(const Vector& other) {
	for (size_type i = 0; i < other.size(); i++) {
		std::allocator_traits<Allocator>::construct(_allocator, _data + i, other[i]);
	}
}

template<class T, class Allocator>
template<class It>
void Vector<T, Allocator>::copy_from_iterator(It first, It last) {
	size_type i = 0;
	for (auto it = first; it != last; it++) {
		std::allocator_traits<Allocator>::construct(_allocator, _data + i, *it);
		i++;
	}
}

template<class T, class Allocator>
void Vector<T, Allocator>::destruct_data(size_type from) {
	for (size_type i = from; i < _size; i++) {
		std::allocator_traits<Allocator>::destroy(_allocator, _data + i);
	}
}

template<class T, class Allocator>
void Vector<T, Allocator>::reserve_to_add(size_type size) {
	size_type needed_capacity = _size + size;
	if (_capacity < needed_capacity) {
		size_type new_capacity = static_cast<size_type>(std::floor(needed_capacity * _increaseCoefficient));
		reserve(new_capacity);
	}
}

template<class T, class Allocator>
void Vector<T, Allocator>::move_to_new(pointer data) {
	for (size_type i = 0; i < _size; i++) {
		std::allocator_traits<Allocator>::construct(_allocator, data + i, std::move(_data[i]));
		std::allocator_traits<Allocator>::destroy(_allocator, _data + i);
	}
}

template<class T, class Allocator>
void Vector<T, Allocator>::shift_right(const_iterator pos, difference_type distance)
{
	reserve_to_add(distance);
	for (auto it = end() + distance - 1; it != pos + distance - 1; it--) {
		std::allocator_traits<Allocator>::construct(_allocator, &*it, std::move(*(it - distance)));
		std::allocator_traits<Allocator>::destroy(_allocator, &*(it - distance));
	}
}

template<class T, class Allocator>
void Vector<T, Allocator>::shift_left(const_iterator pos, difference_type distance) {
	if (distance == 0) {
		return;
	}

	for (auto it = pos - distance; it != end() - distance; it++) {
		std::allocator_traits<Allocator>::destroy(_allocator, &*it);
		std::allocator_traits<Allocator>::construct(_allocator, &*it, std::move(*(it + distance)));
		std::allocator_traits<Allocator>::destroy(_allocator, &*(it + distance));
	}

	for (auto it = end() - distance; it != end(); it++) {
		std::allocator_traits<Allocator>::destroy(_allocator, &*it);
	}
}