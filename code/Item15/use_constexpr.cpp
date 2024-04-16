#include <array>
#include <boost/type_index.hpp>
#include <stdio.h>
#include <string>

// • constexpr objects are const and are initialized with values known during
// compilation.
// • constexpr functions can produce compile-time results when called with
// arguments whose values are known during compilation.
// • constexpr objects and functions may be used in a wider range of contexts
// than non-constexpr objects and functions.
// • constexpr is part of an object’s or function’s interface.

void use_constexpr()
{
    int sz = 10;
    // constexpr auto arraySize1 = sz;      // error! sz's value is not known at compilation
    // std::array<int, arraySize1> data;    // error! same problem as above
    constexpr auto arraySize2 = 10;   // fine, 10 is a compile-time constant
    std::array<int, arraySize2> data; // fine, arraySize2 is a constexpr

    // int sz;                              // as before
    const auto arraySize = sz; // fine, arraySize is a copy of sz
    // std::array<int, arraySize> data;     // error! arraySize's value is not known at compilation
    printf("arraySize: %d\n", arraySize);
    printf("type of arraySize: %s\n", boost::typeindex::type_id_with_cvr<decltype(arraySize)>().pretty_name().c_str());
    printf("type of arraySize2: %s\n",
           boost::typeindex::type_id_with_cvr<decltype(arraySize2)>().pretty_name().c_str());
}

namespace cpp_11
{
constexpr int pow(int base, int exp) noexcept // pow's a constexpr func
{                                             // that never throws
    return (exp == 0 ? 1 : base * pow(base, exp - 1));
}
} // namespace cpp_11

namespace cpp_14
{
constexpr int pow(int base, int exp) noexcept
{
    auto result = 1;
    for (int i = 0; i < exp; ++i)
    {
        result *= base;
    }
    return result;
}

} // namespace cpp_14

void use_constexpr_function()
{
    constexpr auto numConds = 5;                       // # of conditions
    std::array<int, cpp_11::pow(3, numConds)> results; // results has 3^numConds elements

    auto readFromDB = [](const std::string &key) -> int {
        if (key == "base")
        {
            return 10;
        }
        else if (key == "exponent")
        {
            return 3;
        }
        return -1;
    };

    auto base = readFromDB("base");         // get these values
    auto exponent = readFromDB("exponent"); // at runtime

    auto baseToExp = cpp_14::pow(base, exponent); // call pow function
                                                  // at runtime
    printf("baseToExp: %d\n", baseToExp);
}

namespace cpp_11
{
class Point
{
  public:
    constexpr Point(double xVal = 0, double yVal = 0) noexcept : x(xVal), y(yVal)
    {
    }
    constexpr double xValue() const noexcept
    {
        return x;
    }
    constexpr double yValue() const noexcept
    {
        return y;
    }
    void setX(double newX) noexcept
    {
        x = newX;
    }
    void setY(double newY) noexcept
    {
        y = newY;
    }

  private:
    double x, y;
};

constexpr Point midpoint(const Point &p1, const Point &p2) noexcept
{
    return {(p1.xValue() + p2.xValue()) / 2, (p1.yValue() + p2.yValue()) / 2};
}
} // namespace cpp_11

namespace cpp_14
{
class Point
{
  public:
    constexpr Point(double xVal = 0, double yVal = 0) noexcept : x(xVal), y(yVal)
    {
    }
    constexpr double xValue() const noexcept
    {
        return x;
    }
    constexpr double yValue() const noexcept
    {
        return y;
    }
    constexpr void setX(double newX) noexcept
    {
        x = newX;
    }
    constexpr void setY(double newY) noexcept
    {
        y = newY;
    }

  private:
    double x, y;
};
constexpr Point midpoint(const Point &p1, const Point &p2) noexcept
{
    return {(p1.xValue() + p2.xValue()) / 2,  // call constexpr
            (p1.yValue() + p2.yValue()) / 2}; // member funcs
}

constexpr Point reflection(const Point &p) noexcept
{
    Point result;             // create non-const Point
    result.setX(-p.xValue()); // set its x and y values
    result.setY(-p.yValue());
    return result; // return copy of it
}
} // namespace cpp_14

void use_constexpr_object_cpp_11()
{
    using namespace cpp_11;
    constexpr Point p1(9.4, 27.7);         // fine, "runs" constexpr
                                           // ctor during compilation
    constexpr Point p2(28.8, 5.3);         // also fine
    constexpr auto mid = midpoint(p1, p2); // init constexpr
                                           // object with result of
                                           // constexpr function
    printf("midpoint: (%f, %f)\n", mid.xValue(), mid.yValue());
}

void use_constexpr_object_cpp_14()
{
    using namespace cpp_14;
    constexpr Point p1(9.4, 27.7);
    constexpr Point p2(28.8, 5.3);
    constexpr auto mid = midpoint(p1, p2);
    printf("midpoint: (%f, %f)\n", mid.xValue(), mid.yValue());
    constexpr auto ref = reflection(mid); // reflectedMid's value is
                                          // (-19.1, -16.5) and known
                                          // during compilation
    printf("reflection: (%f, %f)\n", ref.xValue(), ref.yValue());
}

int main()
{
    use_constexpr();
    use_constexpr_function();
    use_constexpr_object_cpp_11();
    use_constexpr_object_cpp_14();
    return 0;
}