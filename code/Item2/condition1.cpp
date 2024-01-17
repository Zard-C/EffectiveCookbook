#include <cstdio>
#include <boost/type_index.hpp>


// condition 1 ParamType is a pointer or reference but not a universal reference


void someFunc(int, double)
{

}

int main()
{
    auto x = 27; // x has type int, ParamType is T itself
    const auto cx = x; // cx has type const int, ParamType is const T
    const auto& rx = x; // rx has type const int&, ParamType is const T&
    const auto* px = &x; // px has type const int*, ParamType is const T*

    printf("x type: %s\n", boost::typeindex::type_id_with_cvr<decltype(x)>().pretty_name().c_str());
    printf("cx type: %s\n", boost::typeindex::type_id_with_cvr<decltype(cx)>().pretty_name().c_str());
    printf("rx type: %s\n", boost::typeindex::type_id_with_cvr<decltype(rx)>().pretty_name().c_str());
    printf("px type: %s\n", boost::typeindex::type_id_with_cvr<decltype(px)>().pretty_name().c_str());

    const char name [] = "R. N. Briggs"; // name has type const char[13], ParamType is const T*

    auto arr1 =name;
    auto& arr2 = name;

    auto func1 = someFunc;
    auto& func2 = someFunc;

    printf("arr1 type: %s\n", boost::typeindex::type_id_with_cvr<decltype(arr1)>().pretty_name().c_str());
    printf("arr2 type: %s\n", boost::typeindex::type_id_with_cvr<decltype(arr2)>().pretty_name().c_str());
    printf("func1 type: %s\n", boost::typeindex::type_id_with_cvr<decltype(func1)>().pretty_name().c_str());
    printf("func2 type: %s\n", boost::typeindex::type_id_with_cvr<decltype(func2)>().pretty_name().c_str());

    return 0;
}
