#include <boost/type_index.hpp>
#include <cstdio>

// condition2, ParamType is a universal reference

int main()
{
    auto x = 27;       // condition 3 x has type int, ParamType is T itself
    const auto cx = x; // condition 3 cx has type const int, ParamType is const T
    const auto &rx =
        x; // condition 1, rx is a reference, but not universal. rx has type const int&, ParamType is const T&

    auto &&uref1 = x;  // x is lvalue, so T is int&, ParamType is int&
    auto &&uref2 = cx; // cx is lvalue, so T is const int&, ParamType is const int&
    auto &&uref3 = 27; // 27 is rvalue, so T is int, ParamType is int&&

    printf("x type: %s\n", boost::typeindex::type_id_with_cvr<decltype(x)>().pretty_name().c_str());
    printf("cx type: %s\n", boost::typeindex::type_id_with_cvr<decltype(cx)>().pretty_name().c_str());
    printf("rx type: %s\n", boost::typeindex::type_id_with_cvr<decltype(rx)>().pretty_name().c_str());
    printf("uref1 type: %s\n", boost::typeindex::type_id_with_cvr<decltype(uref1)>().pretty_name().c_str());
    printf("uref2 type: %s\n", boost::typeindex::type_id_with_cvr<decltype(uref2)>().pretty_name().c_str());
    printf("uref3 type: %s\n", boost::typeindex::type_id_with_cvr<decltype(uref3)>().pretty_name().c_str());

    return 0;
}
