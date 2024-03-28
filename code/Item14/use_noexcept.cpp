#include <cstdio>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// • noexcept is part of a function’s interface, and that means that callers may
// depend on it.
// • noexcept functions are more optimizable than non-noexcept functions.
// • noexcept is particularly valuable for the move operations, swap, memory
// deallocation functions, and destructors.
// • Most functions are exception-neutral rather than noexcept.

namespace cpp98
{
int f(int x) throw() // no exceptions from f: C++98 style
{
    return x;
}
class Widget
{
  private:
    std::vector<int> v;
};

void test_widget()
{
    std::vector<Widget> vw;

    Widget w;

    vw.push_back(w); // add w to vw, in c++98 if vw.push_back() throws, vw will be unchanged
    printf("is vw.push_back() noexcept: %d\n", noexcept(vw.push_back(w))); // 0
}

} // namespace cpp98

namespace cpp11
{
int f(int x) noexcept // no exceptions from f: C++11 style
{
    return x;
}

// RetType function(ParamType param) noexcept;         //  most optimizable
// RetType function(ParamType param) throw();          //  less optimizable
// RetType function(ParamType param);                  //  less optimizable

class Widget
{
  public:
  private:
    std::vector<int> v;
};

void test_widget()
{
    std::vector<Widget> vw;

    // In C++11, a natural optimization would be to replace the copying of std::vector
    // elements with moves. Unfortunately, doing this runs the risk of volating the
    // push_back's exception safety guarantee.
    // But how can a function know if a move operation won’t produce an exception?
    // The answer is obvious: it checks to see if the operation is declared noexcept

    vw.push_back(Widget{}); // add w to vw, in c++11
    printf("is vw no_throw_move_constructible: %d\n", std::is_nothrow_move_constructible<Widget>::value); // 1
    printf("is vw no_throw_move_assignable: %d\n", std::is_nothrow_move_assignable<Widget>::value);       // 1
    printf("is vw.push_back() noexcept: %d\n", noexcept(vw.push_back(Widget{})));                         // 0
}

// These functions are conditionally noexcept: whether they are noexcept depends on
// whether the expressions inside the noexcept clauses are noexcept. If they are, the
// functions are noexcept; if they aren’t, the functions are not noexcept.
template <class T, size_t N> void swap(T (&a)[N], T (&b)[N]) noexcept(noexcept(std::swap(*a, *b)))
{
    for (size_t i = 0; i < N; ++i)
    {
        std::swap(a[i], b[i]);
    }
}

template <class T1, class T2> struct pair
{
    void swap(pair &p) noexcept(noexcept(std::swap(std::declval<T1 &>(), std::declval<T1 &>())) && noexcept(
        std::swap(std::declval<T2 &>(), std::declval<T2 &>())))
    {
        std::swap(first, p.first);
        std::swap(second, p.second);
    }

    T1 first;
    T2 second;
};

class ThrowingMoveType
{
  public:
    ThrowingMoveType() = default;
    ThrowingMoveType(ThrowingMoveType &&) noexcept(false)
    {
        std::throw_with_nested(std::runtime_error("ThrowingMoveType move constructor"));
    }
    ThrowingMoveType &operator=(ThrowingMoveType &&) noexcept(false)
    {
        std::throw_with_nested(std::runtime_error("ThrowingMoveType move assignment operator"));
        {
            return *this;
        }
    }
};
void test_swap()
{
    int a[] = {1, 2, 3};
    int b[] = {4, 5, 6};
    swap(a, b);

    pair<int, int> p1;
    pair<int, int> p2;
    p1.swap(p2);

    printf("swap<int> is noexcept: %d\n", noexcept(std::swap(std::declval<int &>(), std::declval<int &>()))); // 1
    printf("swap<pair<int, int>> is noexcept: %d\n",
           noexcept(std::swap(std::declval<pair<int, int> &>(), std::declval<pair<int, int> &>()))); // 1

    printf("std::string move_constructor is noexcept: %d\n",
           noexcept(std::string(std::declval<std::string &&>()))); // 1
    printf("std::string is move_assign is noexcept: %d\n",
           noexcept(std::declval<std::string &>() = std::declval<std::string &&>())); // 1
    printf("swap<pair<std::string, std::string>> is noexcept: %d\n",
           noexcept(std::swap(std::declval<pair<std::string, std::string> &>(),
                              std::declval<pair<std::string, std::string> &>()))); // 1
    printf("is_no_throw_move_constructible<ThrowingMoveType>: %d\n",
           std::is_nothrow_move_constructible<ThrowingMoveType>::value); // 0
    printf("is_no_throw_move_assignable<ThrowingMoveType>: %d\n",
           std::is_nothrow_move_assignable<ThrowingMoveType>::value); // 0
    printf("swap<ThrowingMoveType> is noexcept: %d\n",
           noexcept(std::swap(std::declval<ThrowingMoveType &>(), std::declval<ThrowingMoveType &>()))); // 0
}

void f(const std::string &s) noexcept // precondition: s.length() < 32
{
    printf("f\n");
}

// setup and cleanup document that they never emit exceptions, even though they’re not declared
// that way. There could be good reasons for their non-noexcept declarations. For
// example, they might be part of a library written in C. (Even functions from the
// C Standard Library that have been moved into the std namespace lack exception
// specifications, e.g., std::strlen isn’t declared noexcept.)
void setup() // not marked noexcept
{
    printf("setup\n");
}

void cleanup() // not marked noexcept
{
    printf("cleanup\n");
}

void doWork() noexcept // reasonable to assume no exceptions
{
    setup();
    // do something that won't throw
    cleanup();
}

// Because there are legitimate reasons for noexcept functions to rely on code lacking
// the noexcept guarantee, C++ permits such code, and compilers generally don’t issue
// warnings about it.

} // namespace cpp11

int main()
{
    cpp98::test_widget();
    cpp11::test_widget();
    cpp11::test_swap();
    cpp11::doWork();
    printf("is doWork noexcept: %d\n", noexcept(cpp11::doWork())); // 1
    return 0;
}