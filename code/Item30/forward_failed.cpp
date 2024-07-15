#include "widget.hpp"
#include <boost/type_index.hpp>
#include <cstdio>
#include <vector>

// * Perfect forwarding fails when template type deduction fails or when it deduces
//   the wrong type.

// * The kinds of arguments that lead to perfect forwarding failure are braced
//   initializers, null pointers expressed as 0 or NULL, declaration-only integral
//   const static data members, template and overloaded function names, and
//   bitfields.

namespace braced
{
template <typename T> void f(T &&param)
{
    printf("T is %s\n", boost::typeindex::type_id_with_cvr<T>().pretty_name().c_str());
    printf("param is %s\n", boost::typeindex::type_id_with_cvr<decltype(param)>().pretty_name().c_str());
}

template <typename T> void fwd(T &&param) // receive by universal reference
{
    f(std::forward<T>(param)); // forward it to f
}

template <typename... Ts> void fwd_params(Ts &&...params)
{
    f(std::forward<Ts>(params)...);
}

void func(const std::vector<int> &v)
{
    printf("T is %s\n", boost::typeindex::type_id_with_cvr<decltype(v)>().pretty_name().c_str());
}

void test_fwd()
{
    func({1, 2, 3}); // T is std::initializer_list<int>
                     // param is std::initializer_list<int>
    // forward failed, uncomment below line to see the error
    // fwd_params({1, 2, 3});
    auto il = {1, 2, 3};
    fwd_params(il); // T is std::initializer_list<int>
                    // param is std::initializer_list<int>
    printf("il is %s\n", boost::typeindex::type_id_with_cvr<decltype(il)>().pretty_name().c_str());

    func(il);
}

} // namespace braced

void test_fwd_null()
{
    using namespace braced;
    // forward failed, uncomment below line to see the error
    // fwd_params(nullptr);
    auto p = nullptr;
    fwd_params(p); // T is std::nullptr_t
                   // param is std::nullptr_t
    printf("nullptr is %s\n", boost::typeindex::type_id_with_cvr<decltype(p)>().pretty_name().c_str());

    fwd_params(NULL); // T is int
                      // param is int
    printf("NULL is %s\n", boost::typeindex::type_id_with_cvr<decltype(NULL)>().pretty_name().c_str());
}

const std::size_t Widget::MinVals;

namespace const_static_member
{
void test_widget()
{
    std::vector<int> widgetData;
    widgetData.reserve(Widget::MinVals);
}
} // namespace const_static_member
namespace overload
{

void f(int pf(int))
{
    printf("f(int pf(int))\n");
}

template <typename T> void fwd(T &&param)
{
    f(std::forward<T>(param));
}

int processVal(int value)
{
    printf("processVal(int value)\n");
    return value;
}

int processVal(int value, int priority)
{
    printf("processVal(int value, int priority)\n");
    return value;
}

void test()
{
    f(processVal); // yeah, that one with only one parameter
    // uncomment below line to see the error
    // fwd(processVal);  // error, which one to call?
}

// template is a set of functions, not a single function
template <typename T> T workOnVal(T param)
{
    return param;
}

void test_workOnVal()
{
    int i = 0;
    workOnVal(i);
    // uncomment below line to see the error
    // fwd(workOnVal);  // error, which one to call?

    using ProcessFuncType = int (*)(int);
    ProcessFuncType processValPtr = processVal;
    fwd(processValPtr);                           // f(int pf(int)
    fwd(static_cast<ProcessFuncType>(workOnVal)); // f(int pf(int)
}
} // namespace overload

namespace bitset
{
struct IPv4Header
{
    std::uint32_t version : 4, IHL : 4, DSCP : 6, ECN : 2, totalLength : 16;
    // …
};

void f(std::size_t sz)
{
    printf("f(std::size_t sz)\n");
}

template <typename T> void fwd(T &&param)
{
    f(std::forward<T>(param));
}

void test_fwd()
{
    IPv4Header header;

    f(header.totalLength); // f(std::size_t sz)

    // uncomment below line to see the error
    // fwd(header.totalLength);

    // copy the bitfield to a temporary variable
    auto length = static_cast<std::uint16_t>(header.totalLength);
    fwd(length); // f(std::size_t sz
}
} // namespace bitset

int main()
{
    test_fwd_null();
    braced::test_fwd();
    const_static_member::test_widget();
    overload::test_workOnVal();
    bitset::test_fwd();

    return 0;
}