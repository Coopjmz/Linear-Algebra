#pragma once

#include <optional>

#include "Assert.h"
#include "Math.h"
#include "Point3.h"
#include "Vector3.h"

template <typename T>
struct Line3
{
	Point3<T> point;
	Vector3<T> direction;

	Line3(const Point3<T>& point, const Vector3<T>& direction);
	Line3(const Point3<T>& point1, const Point3<T>& point2);

	std::optional<Point3<T>> PointOfIntersection(const Line3& other) const;

	T AngleBetween(const Line3& other) const;

	T DistanceTo(const Point3<T>& point) const;
	T DistanceTo(const Line3& other) const;

	bool IsPointOnLine(const Point3<T>& point) const;

	bool IsParallelTo(const Line3& other) const;
	bool IsOrthogonalTo(const Line3& other) const;
	bool IsSkewTo(const Line3& other) const;
	bool IsIntersectingWith(const Line3& other) const;

	bool operator==(const Line3& other) const;
	bool operator!=(const Line3& other) const;

private:
	std::pair<Point3<T>, Point3<T>> GetClosestPointsWith(const Line3& other) const;
};

using Line3f = Line3<float>;
using Line3d = Line3<double>;
using Line3ld = Line3<long double>;

template <typename T>
inline Line3<T>::Line3(const Point3<T>& point, const Vector3<T>& direction)
	: point(point)
	, direction(direction)
{
	Assert(!direction.IsZeroVector());
}

template <typename T>
inline Line3<T>::Line3(const Point3<T>& point1, const Point3<T>& point2)
	: point(point1)
	, direction(point2 - point1)
{
	Assert(!direction.IsZeroVector());
}

template <typename T>
inline std::optional<Point3<T>> Line3<T>::PointOfIntersection(const Line3& other) const
{
	if (IsParallelTo(other)) return {};

	const auto closestPoints = GetClosestPointsWith(other);
	return closestPoints.first == closestPoints.second ? closestPoints.first : std::optional<Point3<T>>();
}

template <typename T>
inline T Line3<T>::AngleBetween(const Line3& other) const
{
	if (!IsIntersectingWith(other)) return 0;

	const T magnitudesMultiplied = direction.Magnitude() * other.direction.Magnitude();
	Assert(magnitudesMultiplied > EPSILON);

	return acos(abs(direction.DotProduct(other.direction)) / magnitudesMultiplied);
}

template <typename T>
inline T Line3<T>::DistanceTo(const Point3<T>& point) const
{
	const T directionMagnitude = direction.Magnitude();
	Assert(directionMagnitude > EPSILON);

	return direction.CrossProduct(point - this->point).Magnitude() / directionMagnitude;
}

template <typename T>
inline T Line3<T>::DistanceTo(const Line3& other) const
{
	const auto closestPoints = GetClosestPointsWith(other);
	return (closestPoints.second - closestPoints.first).Magnitude();
}

template <typename T>
inline bool Line3<T>::IsPointOnLine(const Point3<T>& point) const
{
	return direction.IsParallelTo(point - this->point);
}

template <typename T>
inline bool Line3<T>::IsParallelTo(const Line3& other) const
{
	return direction.IsParallelTo(other.direction);
}

template <typename T>
inline bool Line3<T>::IsOrthogonalTo(const Line3& other) const
{
	return direction.IsOrthogonalTo(other.direction)
		&& IsIntersectingWith(other);
}

template <typename T>
inline bool Line3<T>::IsSkewTo(const Line3& other) const
{
	return !IsParallelTo(other)
		&& !IsIntersectingWith(other);
}

template <typename T>
inline bool Line3<T>::IsIntersectingWith(const Line3& other) const
{
	return PointOfIntersection(other).has_value();
}

template <typename T>
inline bool Line3<T>::operator==(const Line3& other) const
{
	return IsPointOnLine(other.point)
		&& direction.IsParallelTo(other.direction);
}

template <typename T>
inline bool Line3<T>::operator!=(const Line3& other) const
{
	return !(*this == other);
}

template <typename T>
inline std::pair<Point3<T>, Point3<T>> Line3<T>::GetClosestPointsWith(const Line3& other) const
{
	const Vector3<T> crossProduct = direction.CrossProduct(other.direction);
	const Vector3<T> vector = other.point - point;
	T t1, t2;

	if (!IsZero(crossProduct.x))
	{
		t1 = (other.direction.z * vector.y - other.direction.y * vector.z) / crossProduct.x;
		t2 = (this->direction.z * vector.y - this->direction.y * vector.z) / crossProduct.x;
	}
	else if (!IsZero(crossProduct.y))
	{
		t1 = (other.direction.z * vector.x - other.direction.x * vector.z) / -crossProduct.y;
		t2 = (this->direction.z * vector.x - this->direction.x * vector.z) / -crossProduct.y;
	}
	else if (!IsZero(crossProduct.z))
	{
		t1 = (other.direction.y * vector.x - other.direction.x * vector.y) / crossProduct.z;
		t2 = (this->direction.y * vector.x - this->direction.x * vector.y) / crossProduct.z;
	}
	else
	{
		const T otherDirectionMagnitudeSquared = other.direction.MagnitudeSquared();
		Assert(otherDirectionMagnitudeSquared > EPSILON);

		t1 = 0;
		t2 = -vector.DotProduct(other.direction) / otherDirectionMagnitudeSquared;
	}

	const Point3<T> p1 = point + direction * t1;
	const Point3<T> p2 = other.point + other.direction * t2;
	return { p1, p2 };
}