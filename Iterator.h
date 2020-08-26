#pragma once
#include <cstring>
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <ctgmath>

template <class T, bool is_const>
class VectorIterator
{
public:
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using value_type = typename std::conditional<is_const, const T, T>::type;
	using reference = typename std::conditional<is_const, const T&, T&>::type;
	using pointer = typename std::conditional<is_const, const T*, T*>::type;
	using iterator_category = std::random_access_iterator_tag;

	VectorIterator() = delete;
	explicit VectorIterator(pointer struct_pointer, difference_type pos = 0);
	VectorIterator(const VectorIterator& other);

	operator VectorIterator<T, true>() const;

	VectorIterator& operator=(const VectorIterator& rhs);

	VectorIterator& operator++(); 
	const VectorIterator operator++(int);

	VectorIterator& operator--(); 
	const VectorIterator operator--(int); 

	reference operator*() const;
	pointer operator->();

	template <class U, bool is_const_u, class F, bool is_const_f>
	friend bool operator==(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs);

	template <class U, bool is_const_u, class F, bool is_const_f>
	friend bool operator!=(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs);

	VectorIterator& operator+=(size_type);

	template<class U, bool is_const_u>
	friend VectorIterator<U, is_const_u> operator+(const VectorIterator<U, is_const_u>& lhs,
		typename VectorIterator<U, is_const_u>::size_type);

	template<class U, bool is_const_u>
	friend VectorIterator<U, is_const_u> operator+(typename VectorIterator<U, is_const_u>::size_type,
		const VectorIterator<U, is_const_u>& rhs);

	VectorIterator& operator-=(size_type);

	template<class U, bool is_const_u>
	friend VectorIterator<U, is_const_u> operator-(const VectorIterator<U, is_const_u>& lhs,
		typename VectorIterator<U, is_const_u>::size_type);

	template <class U, bool is_const_u, class F, bool is_const_f>
	friend typename VectorIterator<U, is_const_u>::difference_type operator-(VectorIterator<U, is_const_u>& lhs,
		VectorIterator<F, is_const_f>& rhs);

	reference operator[](size_type) const;

	template <class U, bool is_const_u, class F, bool is_const_f>
	friend bool operator<(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs);

	template <class U, bool is_const_u, class F, bool is_const_f>
	friend bool operator>(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs);

	template <class U, bool is_const_u, class F, bool is_const_f>
	friend bool operator<=(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs);

	template <class U, bool is_const_u, class F, bool is_const_f>
	friend bool operator>=(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs);

	difference_type pos() const;

private:
	pointer _struct_pointer;
	difference_type _pos;
};

template<class T, bool is_const>
typename VectorIterator<T, is_const>::difference_type VectorIterator<T, is_const>::pos() const{
	return _pos;
}

template<class T, bool is_const>
VectorIterator<T, is_const>::VectorIterator(pointer struct_pointer, difference_type pos)
	: _struct_pointer(struct_pointer), _pos(pos) {}

template<class T, bool is_const>
VectorIterator<T, is_const>::VectorIterator(const VectorIterator& other)
: VectorIterator(other._struct_pointer, other._pos) {}

template<class T, bool is_const>
VectorIterator<T, is_const>::operator VectorIterator<T, true>() const
{
	return VectorIterator<T, true>(_struct_pointer, _pos);
}

template<class T, bool is_const>
VectorIterator<T, is_const>& VectorIterator<T, is_const>::operator=(const VectorIterator& other)
{
	if (this != &other) {
		_struct_pointer = other._struct_pointer;
		_pos = other._pos;
	}

	return *this;
}

template<class T, bool is_const>
VectorIterator<T, is_const>& VectorIterator<T, is_const>::operator++()
{
	_pos++;
	return *this;
}

template<class T, bool is_const>
const VectorIterator<T, is_const> VectorIterator<T, is_const>::operator++(int)
{
	VectorIterator<T, is_const> tmp(*this); 
	operator++();
	return tmp;
}

template<class T, bool is_const>
VectorIterator<T, is_const>& VectorIterator<T, is_const>::operator--()
{
	_pos--;
	return *this;
}

template<class T, bool is_const>
const VectorIterator<T, is_const> VectorIterator<T, is_const>::operator--(int)
{
	VectorIterator<T, is_const> tmp(*this); 
	operator--();
	return tmp;
}

template<class T, bool is_const>
typename VectorIterator<T, is_const>::reference VectorIterator<T, is_const>::operator*() const
{
	return *(_struct_pointer + _pos);
}

template<class T, bool is_const>
typename VectorIterator<T, is_const>::pointer VectorIterator<T, is_const>::operator->()
{
	return _struct_pointer + _pos;
}

template<class U, bool is_const_u, class F, bool is_const_f>
bool operator==(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs)
{
	return lhs._pos == rhs._pos;
}

template<class U, bool is_const_u, class F, bool is_const_f>
bool operator!=(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs)
{
	return lhs._pos != rhs._pos;
}

template<class U, bool is_const_u>
VectorIterator<U, is_const_u> operator+(const VectorIterator<U, is_const_u>& lhs,
	typename VectorIterator<U, is_const_u>::size_type rhs)
{
	return VectorIterator<U, is_const_u>(lhs._struct_pointer, lhs._pos + rhs);
}

template<class U, bool is_const_u>
VectorIterator<U, is_const_u> operator+(typename VectorIterator<U, is_const_u>::size_type lhs, const VectorIterator<U, is_const_u>& rhs)
{
	return rhs + lhs;
}

template<class T, bool is_const>
VectorIterator<T, is_const>& VectorIterator<T, is_const>::operator+=(VectorIterator::size_type n)
{
	_pos += n;
	return *this;
}

template<class T, bool is_const>
VectorIterator<T, is_const>& VectorIterator<T, is_const>::operator-=(VectorIterator::size_type n)
{
	_pos -= n;
	return *this;
}

template<class U, bool is_const_u>
VectorIterator<U, is_const_u> operator-(const VectorIterator<U, is_const_u>& lhs, typename VectorIterator<U, is_const_u>::size_type rhs)
{
	return VectorIterator<U, is_const_u>(lhs._struct_pointer, lhs._pos - rhs);
}

template<class U, bool is_const_u, class F, bool is_const_f>
typename VectorIterator<U, is_const_u>::difference_type operator-(VectorIterator<U, is_const_u>& lhs, VectorIterator<F, is_const_f>& rhs)
{
	return lhs._pos - rhs._pos;
}

template<class T, bool is_const>
typename VectorIterator<T, is_const>::reference VectorIterator<T, is_const>::operator[](VectorIterator::size_type n) const
{
	return *(_struct_pointer + _pos + n);
}

template<class U, bool is_const_u, class F, bool is_const_f>
bool operator<(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs)
{
	return lhs._pos < rhs._pos;
}

template<class U, bool is_const_u, class F, bool is_const_f>
bool operator>(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs)
{
	return lhs._pos > rhs._pos;
}

template<class U, bool is_const_u, class F, bool is_const_f>
bool operator<=(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs)
{
	return lhs._pos <= rhs._pos;
}

template<class U, bool is_const_u, class F, bool is_const_f>
bool operator>=(const VectorIterator<U, is_const_u>& lhs, const VectorIterator<F, is_const_f>& rhs)
{
	return lhs._pos >= rhs._pos;
}