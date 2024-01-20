#include <boost/type_index.hpp>
#include <cstdio>
#include <typeinfo>
#include <vector>

// decltype is an odd creature. Given a name or an expression, decltype tells you the
// name’s or the expression’s type. Typically, what it tells you is exactly what you’d
// predict. Occasionally however, it provides results that leave you scratching your head
// and turning to reference works or online Q&A sites for revelation

class Widget
{
};

bool f(const Widget &w) // decltype(w) is const Widget&
{                       // decltype(f) is bool(const Widget&)
    return true;
}

/// a c++ 11template that could work, but need to fix
/// the real return type is after -> , and can use the parameter to deduce the return type
template <typename Container, typename Index> auto authAndAccess(Container &c, Index i) -> decltype(c[i])
{
    // authenticateUser();
    return c[i];
}

/// in c++ 14, we can use auto to deduce the return type
template <typename Container, typename Index> auto authAndAccess2(Container &c, Index i)
{
    // authenticateUser();
    return c[i]; // return type deduced from c[i]
}

/// we use decltype(auto) to deduce the return type, could work, but need to fix
template <typename Container, typename Index> decltype(auto) authAndAccess3(Container &c, Index i)
{
    // authenticateUser();
    return c[i];
}

/// c++14 final version: we use universal reference to receive the parameter, and use decltype(auto) to deduce the
/// return type
template <typename Container, typename Index> decltype(auto) authAndAccess4(Container &&c, Index i)
{
    // authenticateUser();
    return std::forward<Container>(c)[i]; // for universal reference, we need to use std::forward Item 25
}

/// c++11 final version: we use universal reference to receive the parameter, and use decltype(auto) to deduce the
/// return type
template <typename Container, typename Index>
auto authAndAccess5(Container &&c, Index i) -> decltype(std::forward<Container>(c)[i])
{
    // authenticateUser();
    return std::forward<Container>(c)[i]; // for universal reference, we need to use std::forward Item 25
}

decltype(auto) f1()
{
    int x = 0;
    return x; // decltype(x) is int, so f1 returns int
}

// decltype(auto) f2()
// {
//     int x = 0;
//     return (x); // decltype((x)) is int&, so f2 returns int&, it's dangerous!
// }

void func()
{
    int x = 0;
    decltype(x) a;       // a type is  int
    decltype((x)) b = x; // b type is  int&
}

int main()
{
    const int i = 0;   // decltype(i) is const int
    decltype(i) j = i; // j type is const int, because i is a const lvalue, so decltype(i) is const int
    printf("j type is %s\n", boost::typeindex::type_id_with_cvr<decltype(j)>().pretty_name().c_str());
    decltype((i)) k = i; // k type is const int&, because (i) is a lvalue expression, for  lvalue expression
    // of type T other than names, decltype always reports a type of T&, so decltype((i)) is const int&
    printf("k type is %s\n", boost::typeindex::type_id_with_cvr<decltype(k)>().pretty_name().c_str());

    std::vector<int> v = {0}; // decltype(v) is std::vector<int>
    v[0] = 1;                 // decltype(v[0]) is int&

    // only one exception
    std::vector<bool> v2 = {true}; // decltype(v2) is std::vector<bool>
    // v2[0] = false;                 // decltype(v2[0]) is std::_Bit_reference

    printf("decltype(v[0]) is %s\n", boost::typeindex::type_id_with_cvr<decltype(v[0])>().pretty_name().c_str());
    printf("decltype(v2[0]) is %s\n", boost::typeindex::type_id_with_cvr<decltype(v2[0])>().pretty_name().c_str());

    // Item2 shows that the type of an expression involving auto is deduced using the
    // rules for template type deduction.
    std::vector<int> v3;
    v3.resize(10);

    // authAndAccess(v, 5) = 10;
    // Item 2 explains that for functions with an auto return type specification, compilers
    // employ template type deduction. In this case, that’s problematic. As we’ve discussed,
    // operator[] for most containers-of-T returns a T&, but Item 1 explains that during
    // template type deduction, the reference-ness of an initializing expression is ignored.
    // Consider what that means for this client code: int being the return value of a function
    // is an rvalue, so the type deduced for ret is int, not int&. so the code won't compile

    auto ret = authAndAccess(v3, 5); // ret is int&
    printf("decltype(ret) is %s\n", boost::typeindex::type_id_with_cvr<decltype(ret)>().pretty_name().c_str());

    /// what if we pass a rvalue container to authAndAccess? won't compile
    /// authAndAccess3(std::vector<int>(10), 5) = 10;

    return 0;
}