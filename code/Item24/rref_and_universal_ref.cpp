#include <boost/type_index.hpp>
#include <chrono>
#include <cstdio>
#include <vector>

// • If a function template parameter has type T&& for a deduced type T, or if an
// object is declared using auto&&, the parameter or object is a universal refer‐
// ence.
// • If the form of the type declaration isn’t precisely type&&, or if type deduction
// does not occur, type&& denotes an rvalue reference.
// • Universal references correspond to rvalue references if they’re initialized with
// rvalues. They correspond to lvalue references if they’re initialized with lval‐
// ues.

class Widget
{
};

void f(Widget &&param) // without type deduction of param, param is an rvalue reference
{
    // param is an rvalue reference, but it's an lvalue, because it has a name and can be addressed
    printf("param is an %s\n", boost::typeindex::type_id_with_cvr<decltype(param)>().pretty_name().c_str());
}

Widget &&var1 = Widget(); // without type deduction of var1, var1 is an rvalue reference

auto &&var2 = var1; // the type of var2 is deduced, so var2 is a universal reference, and type deduction is Widget&

template <typename T> void f(std::vector<T> &&param) //
{
    // param is an rvalue reference, but it's an lvalue, because it has a name and can be addressed
    printf("param is an %s\n", boost::typeindex::type_id_with_cvr<decltype(param)>().pretty_name().c_str());
}

template <typename T> void f(T &&param) // param is a universal reference
{
    // param is an rvalue reference, but it's an lvalue, because it has a name and can be addressed
    printf("param is an %s\n", boost::typeindex::type_id_with_cvr<decltype(param)>().pretty_name().c_str());
}

namespace const_ref
{
template <typename T> void f(const T &&param) // param is an rvalue reference
{
    // param is an rvalue reference, but it's an lvalue, because it has a name and can be addressed
    printf("param is an %s\n", boost::typeindex::type_id_with_cvr<decltype(param)>().pretty_name().c_str());
    printf("is lvalue reference: %d\n", std::is_lvalue_reference<decltype(param)>::value);
    printf("is rvalue reference: %d\n", std::is_rvalue_reference<decltype(param)>::value);
}
} // namespace const_ref
void test_universal_ref()
{
    Widget w;
    f(w);        // w is an lvalue, so T is Widget& and param is Widget&
    f(Widget()); // Widget() is an rvalue, so T is Widget and param is Widget&&
    std::vector<int> v;
    f(std::move(v)); // std::move(v) is an rvalue, so T is std::vector<int> and param is std::vector<int>&&

    f(v); // v is an lvalue, so T is std::vector<int>& and param is std::vector<int>&

    Widget cw;
    const Widget &cw_ref = cw;
    // uncomment the following line to see the error
    // const_ref::f(cw_ref);

    const_ref::f(std::move(cw)); // std::move(cw) is an rvalue, so T is Widget and param is Widget&&
}

void test_timer_func()
{

    auto timeFuncInvocations = [](auto &&func, auto &&...args) {
        auto start = std::chrono::high_resolution_clock::now();
        std::forward<decltype(func)>(func)(std::forward<decltype(args)>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        printf("Elapsed time: %ld ns\n", std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    };

    auto test_func = [](int a, int b) {
        printf("a: %d, b: %d\n", a, b);
        for (int i = 0; i < 1024 * 1024; ++i)
        {
            i ^= 1;
        }
    };

    timeFuncInvocations(test_func, 1, 2);
}

int main()
{
    test_universal_ref();
    test_timer_func();
    return 0;
}