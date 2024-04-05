#include <iostream>

#include "Math.h"
#include "Point3.h"
#include "Vector3.h"
#include "Line3.h"
#include "Plane3.h"

namespace
{
    template <uint8_t N = 0, typename T>
    T Round(const T value)
    {
        static uint64_t k = 0;
        if (k == 0)
        {
            k = 1;
            for (uint8_t i = 0; i < N; ++i)
                k *= 10;
        }

        return std::round(value * k) / k;
    }

    template <typename T>
    void PrintPoint(const Point3<T>& point, const char* const name = nullptr)
    {
        if (name) std::cout << name;
        std::cout << '(' << Round<2>(point.x) << ", " << Round<2>(point.y) << ", " << Round<2>(point.z) << ")\n";
    }

    template <typename T>
    void PrintVector(const Vector3<T>& vector, const char* const name = nullptr)
    {
        if (name) std::cout << name;
        std::cout << '<' << Round<2>(vector.x) << ", " << Round<2>(vector.y) << ", " << Round<2>(vector.z) << ">\n";
    }

    template <typename T>
    void PrintLine(const Line3<T>& line, const char* const name = nullptr)
    {
        if (name) std::cout << name << ":\n";
        std::cout << "| x = " << Round<2>(line.point.x) << (line.direction.x >= 0 ? " + " : " - ") << Round<2>(abs(line.direction.x)) << "t\n";
        std::cout << "| y = " << Round<2>(line.point.y) << (line.direction.y >= 0 ? " + " : " - ") << Round<2>(abs(line.direction.y)) << "t\n";
        std::cout << "| z = " << Round<2>(line.point.z) << (line.direction.z >= 0 ? " + " : " - ") << Round<2>(abs(line.direction.z)) << "t\n";
    }

    template <typename T>
    void PrintPlane(const Plane3<T>& plane, const char* const name = nullptr)
    {
        const T a = plane.normal.x;
        const T b = plane.normal.y;
        const T c = plane.normal.z;
        const T d = -(a * plane.point.x + b * plane.point.y + c * plane.point.z);

        if (name) std::cout << name << ": ";
        std::cout << Round<2>(a) << 'x'
            << (b >= 0 ? " + " : " - ") << Round<2>(abs(b)) << 'y'
            << (c >= 0 ? " + " : " - ") << Round<2>(abs(c)) << 'z'
            << (d >= 0 ? " + " : " - ") << Round<2>(abs(d)) << " = 0\n";
    }
}

int main()
{
    const Point3f point1{ 7.f, 4.f, 3.f };

    PrintPoint(point1, "Point1");
    std::cout << std::endl;
    
    const Line3f line1{ Point3f{ 1.f, 2.f, 0.f }, Vector3f{ -1.f, 1.f, 3.f }};
    const Line3f line2{ Point3f{ 1.f, 1.f, 2.f }, Vector3f{ 1.f, 3.f, -1.f }};

    PrintLine(line1, "Line1");
    std::cout << std::endl;

    PrintLine(line2, "Line2");
    std::cout << std::endl;

    const Plane3f plane1{ 5.f, -6.f, 4.f, 2.f };
    const Plane3f plane2{ 9.f, 0.f, -2.f, 1.f };
    const Plane3f plane3{ 1.f, 1.f, 3.f, 1.f };

    PrintPlane(plane1, "Plane1");
    PrintPlane(plane2, "Plane2");
    PrintPlane(plane3, "Plane3");
    std::cout << std::endl;

    if (const auto pointOfIntersection = plane1.PointOfIntersection(line1))
        PrintPoint(*pointOfIntersection, "Point of intersection (between Plane1 and Line1): P");
    std::cout << std::endl;

    if (const auto lineOfIntersection = plane1.LineOfIntersection(plane2))
        PrintLine(*lineOfIntersection, "Line of intersection (between Plane1 and Plane2)");
    std::cout << std::endl;

    std::cout << "Distance (from Plane1 to Point): " << Round<2>(plane1.DistanceTo(point1)) << std::endl;
    std::cout << "Angle (between Plane1 and Line2): " << Round<2>(RadToDeg(plane1.AngleBetween(line2))) << std::endl;
    std::cout << "Angle (between Plane1 and Plane3): " << Round<2>(RadToDeg(plane1.AngleBetween(plane3))) << std::endl;

    return EXIT_SUCCESS;
}