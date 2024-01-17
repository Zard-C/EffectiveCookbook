#include <boost/type_index.hpp>
#include <cstdio>
#include <initializer_list>

// 4 ways to declare a variable
//  int x1 = 27;
//  int x2(27);
//  int x3 = {27};
//  int x4{27};

template <typename T>
void f(std::initializer_list<T> initList)
{
    printf("T type: %s\n", boost::typeindex::type_id_with_cvr<T>().pretty_name().c_str());
    printf("initList type: %s\n", boost::typeindex::type_id_with_cvr<decltype(initList)>().pretty_name().c_str());
}

// for c++14, we can use auto to deduce the return type of a function
// if we want return type to be std::initializer_list<T>

// auto createInitList()
// {
//     return {1, 2, 3};
// }

// same for lambda, when we want to use auto to deduce the parameter type of a lambda
//  std::vector<int> v;
//  …
//  auto resetV =
//      [&v](const auto& newValue) { v = newValue; }; // C++14
//  …
//  resetV({ 1, 2, 3 }); // error! can't deduce type
//  // for { 1, 2, 3 }

int main()
{
    auto x1 = 27; // x has type int, ParamType is T itself
    auto x2(27); // x has type int, ParamType is T itself
    auto x3 = {27}; // x has type std::initializer_list<int>, ParamType is std::initializer_list<int>
    auto x4{27}; // x has type int, ParamType is int

    // auto x5 = {1, 2, 3.0}; // wrong! can not deduce type from initializer list

    printf("x1 type: %s\n", boost::typeindex::type_id_with_cvr<decltype(x1)>().pretty_name().c_str());
    printf("x2 type: %s\n", boost::typeindex::type_id_with_cvr<decltype(x2)>().pretty_name().c_str());
    printf("x3 type: %s\n", boost::typeindex::type_id_with_cvr<decltype(x3)>().pretty_name().c_str());
    printf("x4 type: %s\n", boost::typeindex::type_id_with_cvr<decltype(x4)>().pretty_name().c_str());


    f({1, 2, 3}); // T is int, ParamType is std::initializer_list<int>




    return 0;
}