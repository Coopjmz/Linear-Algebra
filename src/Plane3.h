#pragma once

#include <optional>

#include "Assert.h"
#include "Math.h"
#include "Point3.h"
#include "Vector3.h"
#include "Line3.h"

template <typename T>
struct Plane3
{
	Point3<T> point;
	Vector3<T> normal;

	Plane3(const Point3<T>& point, const Vector3<T>& normal);
	Plane3(const Point3<T>& point, const Vector3<T>& vector1, const Vector3<T>& vector2);
	Plane3(const Point3<T>& point1, const Point3<T>& point2, const Point3<T>& point3);
	Plane3(const Line3<T>& line1, const Line3<T>& line2);
	Plane3(const T a, const T b, const T c, const T d);

	std::optional<Point3<T>> PointOfIntersection(const Line3<T>& line) const;
	std::optional<Line3<T>> LineOfIntersection(const Plane3& other) const;

	T AngleBetween(const Line3<T>& line) const;
	T AngleBetween(const Plane3& other) const;

	T RelativeDistanceTo(const Point3<T>& point) const;

	T DistanceTo(const Point3<T>& point) const;
	T DistanceTo(const Line3<T>& line) const;
	T DistanceTo(const Plane3& other) const;

	bool IsPointInPlane(const Point3<T>& point) const;
	bool IsLineInPlane(const Line3<T>& line) const;

	bool IsParallelTo(const Line3<T>& line) const;
	bool IsParallelTo(const Plane3& other) const;

	bool IsOrthogonalTo(const Line3<T>& line) const;
	bool IsOrthogonalTo(const Plane3& other) const;

	bool operator==(const Plane3& other) const;
	bool operator!=(const Plane3& other) const;
};

using Plane3f = Plane3<float>;
using Plane3d = Plane3<double>;
using Plane3ld = Plane3<long double>;

template <typename T>
inline Plane3<T>::Plane3(const Point3<T>& point, const Vector3<T>& normal)
	: point(point)
	, normal(normal)
{
	Assert(!normal.IsZeroVector());
}

template<typename T>
inline Plane3<T>::Plane3(const Point3<T>& point, const Vector3<T>& vector1, const Vector3<T>& vector2)
	: point(point)
	, normal(vector1.CrossProduct(vector2))
{
	Assert(!normal.IsZeroVector());
}

template <typename T>
inline Plane3<T>::Plane3(const Point3<T>& point1, const Point3<T>& point2, const Point3<T>& point3)
	: point(point1)
	, normal((point2 - point1).CrossProduct(point3 - point1))
{
	Assert(!normal.IsZeroVector());
}

template <typename T>
inline Plane3<T>::Plane3(const Line3<T>& line1, const Line3<T>& line2)
	: point(line1.point)
{
	const Vector3<T> crossProduct = line1.direction.CrossProduct(line2.direction);

	if (crossProduct.IsZeroVector())
		normal = line1.direction.CrossProduct(line2.point - line1.point);
	else if (line1.IsIntersectingWith(line2))
		normal = crossProduct;
	else
		Assert(!"Skewed lines cannot form a plane!");
}

template <typename T>
inline Plane3<T>::Plane3(const T a, const T b, const T c, const T d)
	: normal({ a, b, c })
{
	if (!IsZero(a))
		point.x = -d / a;
	else if (!IsZero(b))
		point.y = -d / b;
	else if (!IsZero(c))
		point.z = -d / c;
	else
		Assert(!"Normal vector is the zero vector!");
}

template <typename T>
inline std::optional<Point3<T>> Plane3<T>::PointOfIntersection(const Line3<T>& line) const
{
	const T dotProduct = normal.DotProduct(line.direction);
	if (dotProduct < EPSILON) return {};

	const T t = -RelativeDistanceTo(line.point) / dotProduct;
	const Point3<T> p = line.point + line.direction * t;

	return p;
}

template <typename T>
inline std::optional<Line3<T>> Plane3<T>::LineOfIntersection(const Plane3& other) const
{
	const Vector3<T> crossProduct = normal.CrossProduct(other.normal);
	if (crossProduct.IsZeroVector()) return {};

	const T thisDotProduct = normal.DotProduct(point.ToVector());
	const T otherDotProduct = other.normal.DotProduct(other.point.ToVector());
	Point3<T> point;

	if (!IsZero(crossProduct.x))
	{
		point.y = (other.normal.z * thisDotProduct - normal.z * otherDotProduct) / crossProduct.x;
		point.z = (other.normal.y * thisDotProduct - normal.y * otherDotProduct) / -crossProduct.x;
	}
	else if (!IsZero(crossProduct.y))
	{
		point.x = (other.normal.z * thisDotProduct - normal.z * otherDotProduct) / -crossProduct.y;
		point.z = (other.normal.x * thisDotProduct - normal.x * otherDotProduct) / crossProduct.y;
	}
	else if (!IsZero(crossProduct.z))
	{
		point.x = (other.normal.y * thisDotProduct - normal.y * otherDotProduct) / crossProduct.z;
		point.y = (other.normal.x * thisDotProduct - normal.x * otherDotProduct) / -crossProduct.z;
	}

	return { { point , crossProduct } };
}

template <typename T>
inline T Plane3<T>::AngleBetween(const Line3<T>& line) const
{
	if (IsParallelTo(line)) return 0;

	const T magnitudesMultiplied = normal.Magnitude() * line.direction.Magnitude();
	Assert(magnitudesMultiplied > EPSILON);

	return asin(abs(normal.DotProduct(line.direction)) / magnitudesMultiplied);
}

template <typename T>
inline T Plane3<T>::AngleBetween(const Plane3& other) const
{
	if (IsParallelTo(other)) return 0;

	const T magnitudesMultiplied = normal.Magnitude() * other.normal.Magnitude();
	Assert(magnitudesMultiplied > EPSILON);

	return acos(abs(normal.DotProduct(other.normal)) / magnitudesMultiplied);
}

template <typename T>
inline T Plane3<T>::RelativeDistanceTo(const Point3<T>& point) const
{
	return normal.DotProduct(point - this->point);
}

template <typename T>
inline T Plane3<T>::DistanceTo(const Point3<T>& point) const
{
	const T normalMagnitude = normal.Magnitude();
	Assert(normalMagnitude > EPSILON);

	return abs(RelativeDistanceTo(point)) / normalMagnitude;
}

template <typename T>
inline T Plane3<T>::DistanceTo(const Line3<T>& line) const
{
	return IsParallelTo(line) ? DistanceTo(line.point) : 0.f;
}

template <typename T>
inline T Plane3<T>::DistanceTo(const Plane3& other) const
{
	return IsParallelTo(other) ? DistanceTo(other.point) : 0.f;
}

template <typename T>
inline bool Plane3<T>::IsPointInPlane(const Point3<T>& point) const
{
	return IsZero(RelativeDistanceTo(point));
}

template <typename T>
inline bool Plane3<T>::IsLineInPlane(const Line3<T>& line) const
{
	return IsPointInPlane(line.point) && IsPointInPlane(line.point + line.direction);
}

template <typename T>
inline bool Plane3<T>::IsParallelTo(const Line3<T>& line) const
{
	return normal.IsOrthogonalTo(line.direction);
}

template <typename T>
inline bool Plane3<T>::IsParallelTo(const Plane3& other) const
{
	return normal.IsParallelTo(other.normal);
}

template <typename T>
inline bool Plane3<T>::IsOrthogonalTo(const Line3<T>& line) const
{
	return normal.IsParallelTo(line.direction);
}

template <typename T>
inline bool Plane3<T>::IsOrthogonalTo(const Plane3& other) const
{
	return normal.IsOrthogonalTo(other.normal);
}

template <typename T>
inline bool Plane3<T>::operator==(const Plane3& other) const
{
	return IsPointInPlane(other.point)
		&& normal.IsParallelTo(other.normal);
}

template <typename T>
inline bool Plane3<T>::operator!=(const Plane3& other) const
{
	return !(*this == other);
}