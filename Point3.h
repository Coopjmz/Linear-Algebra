#pragma once

#include "Math.h"
#include "Vector3.h"

template <typename T>
struct Vector3;

template <typename T>
struct Point3
{
	T x = 0;
	T y = 0;
	T z = 0;

	Vector3<T> ToVector() const;

	bool operator==(const Point3& other) const;
	bool operator!=(const Point3& other) const;

	Point3 operator+(const Vector3<T>& vector) const;
	Vector3<T> operator-(const Point3& other) const;
};

using Point3f = Point3<float>;
using Point3d = Point3<double>;
using Point3ld = Point3<long double>;

template <typename T>
inline Vector3<T> Point3<T>::ToVector() const
{
	return { x, y, z };
}

template <typename T>
inline bool Point3<T>::operator==(const Point3& other) const
{
	return abs(x - other.x) < EPSILON
		&& abs(y - other.y) < EPSILON
		&& abs(z - other.z) < EPSILON;
}

template <typename T>
inline bool Point3<T>::operator!=(const Point3& other) const
{
	return !(*this == other);
}

template <typename T>
inline Point3<T> Point3<T>::operator+(const Vector3<T>& vector) const
{
	return { x + vector.x, y + vector.y, z + vector.z };
}

template <typename T>
inline Vector3<T> Point3<T>::operator-(const Point3& other) const
{
	return { x - other.x, y - other.y, z - other.z };
}