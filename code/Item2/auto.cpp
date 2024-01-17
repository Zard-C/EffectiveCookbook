#include <boost/type_index.hpp>
#include <cstdio>
#include <initializer_list>

// template <typename T>
// void f (ParamType param);
// call f like
// f(expr)

// when we use auto to decalre a variable, we are telling the compiler to deduce the type of the variable from its
// initializer auto is the T in template<typename T> ParamType is the ParamType in template<typename T>

template <typename T> void func_for_x(T param)
{
}

template <typename T> void func_for_cx(const T param)
{
}

template <typename T> void func_for_crx(const T &param)
{
}

/// @note:
// So the only real difference between auto and template type deduction is that auto
// assumes that a braced initializer represents a std::initializer_list, but template
// type deduction doesn’t

template <typename T> void f(std::initializer_list<T> initList)
{
    printf("T type: %s\n", boost::typeindex::type_id_with_cvr<T>().pretty_name().c_str());
    printf("initList type: %s\n", boost::typeindex::type_id_with_cvr<decltype(initList)>().pretty_name().c_str());
}

int main()
{
    auto x = 27;    // x has type int, ParamType is T itself
    func_for_x(27); // T is int, ParamType is int

    const auto cx = x; // cx has type const int, ParamType is const T
    func_for_cx(x);    // T is int, ParamType is const int

    const auto &rx = x; // rx has type const int&, ParamType is const T&
    func_for_crx(x);    // T is int, ParamType is const int&

    return 0;
}