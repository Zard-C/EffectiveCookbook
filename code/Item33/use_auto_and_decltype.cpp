#include <boost/type_index.hpp>
#include <stdio.h>

// Use decltype on auto&& parameters to std::forward them.

int normalize(int x)
{
    return x;
}

int func(int x)
{
    return x;
}

namespace cpp14
{

auto f = [](auto x) { return func(normalize(x)); };

void test()
{
    int x = 10;
    auto result = f(x);
    printf("result = %d\n", result);
}
} // namespace cpp14

namespace cpp11
{

class SomeCompilerGeneratedClassName
{
  public:
    template <typename T> auto operator()(T x) const
    {
        return func(normalize(x));
    }
    // other enclosures functions here
};

void test()
{
    int x = 10;
    SomeCompilerGeneratedClassName f;
    auto result = f(x);
    printf("result = %d\n", result);
}
} // namespace cpp11

template <typename T> int myfunc(T &&x)
{
    printf("inside myfunc\n");
    printf("T = %s\n", boost::typeindex::type_id_with_cvr<T>().pretty_name().c_str());
    printf("x = %s\n", boost::typeindex::type_id_with_cvr<decltype(x)>().pretty_name().c_str());
    return x;
}

namespace cpp14
{
auto myf = [](auto &&x) {
    // return myfunc(std::forward<???>(x));         // we always use std::forward<T>(x) where T is the type of x, how
    // about now?
    return myfunc(std::forward<decltype(x)>(x));
};

void test_myf()
{
    int x = 10;
    auto result = myf(x);
    printf("result = %d\n", result);

    result = myf(10);
    printf("result = %d\n", result);
}

} // namespace cpp14

namespace cpp14
{

template <typename T> T &&forward(std::remove_reference_t<T> &param)
{
    return static_cast<T &&>(param);
}

// when trying to forward a rvalue such as int
// T is int, T&& is int&&, std::remove_reference_t<T> is int
// so the return type is int&&

int &&int_r_forward(int &param)
{
    return static_cast<int &&>(param);
}

// when tring to forward a lvalue such as int&
// T is int&, T&& -> int& && fold to int&, std::remove_reference_t<T> is int
// so the return type is int&

int &int_l_forward(int &param)
{
    return static_cast<int &>(param);
}

// when trying to forward a decltype(auto) such as int&&
// int&& && int_decltype_forward(int& param)
// {
//     return static_cast<int&& &&>(param);
// }
// After reference collapsing, the return type is int&&, int && && fold to int&&
// which is the same as the non decltype(auto) case
int &&int_decltype_forward(int &param)
{
    return static_cast<int &&>(param);
}

template <typename T> void forward_test(T &&param)
{
    printf("T = %s\n", boost::typeindex::type_id_with_cvr<T>().pretty_name().c_str());
    printf("param = %s\n", boost::typeindex::type_id_with_cvr<decltype(param)>().pretty_name().c_str());
    auto result = forward<T>(param);
    printf("result type is %s\n", boost::typeindex::type_id_with_cvr<decltype(result)>().pretty_name().c_str());
}

template <typename T> void forward_decltype(T &&param)
{
    printf("T = %s\n", boost::typeindex::type_id_with_cvr<T>().pretty_name().c_str());
    printf("param = %s\n", boost::typeindex::type_id_with_cvr<decltype(param)>().pretty_name().c_str());
    auto result = forward<decltype(param)>(param);
    printf("result type is %s\n", boost::typeindex::type_id_with_cvr<decltype(result)>().pretty_name().c_str());
}

void test_forward()
{
    int x = 10;
    printf("l value forward\n");
    forward_test(x);
    printf("r value forward\n");
    forward_test(10);
    printf("delctype forward\n");
}

auto final_version1 = [](auto &&param) { return func(normalize(std::forward<decltype(param)>(param))); };

// this is not the final version, since
auto final_version2 = [](auto &&...params) { return func(normalize(std::forward<decltype(params)>(params)...)); };

} // namespace cpp14

int main()
{
    cpp11::test();
    cpp14::test();
    cpp14::test_myf();
    cpp14::test_forward();
    return 0;
}