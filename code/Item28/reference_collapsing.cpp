#include <boost/type_index.hpp>
#include <cstdio>
#include <type_traits>
#include <utility>

//                                Things to Remember
//                                ------------------

// * Reference collapsing occurs in four contexts: template instantiation, auto
//   type generation, creation and use of typedefs and alias declarations, and
//   decltype.

// * When compilers generate a reference to a reference in a reference collapsing
//   context, the result becomes a single reference.  If either of the original
//   references is an lvalue reference, the result is an lvalue reference.
//   Otherwise it's an rvalue reference.

// * Universal references are rvalue references in contexts where type deduction
//   distinguishes lvalues from rvalues and where reference collapsing occurs.

class Widget
{
};

Widget WidgetFactory()
{
    return Widget();
}

template <typename T> void func(T &&param)
{
    printf("T type is %s\n", boost::typeindex::type_id_with_cvr<T>().pretty_name().c_str());
    printf("param type is %s\n", boost::typeindex::type_id_with_cvr<decltype(param)>().pretty_name().c_str());
}

void test_func()
{
    int a = 10;
    func(a);
    func(10);

    Widget w;
    func(w);               // lvalue calls func, T deduced as Widget&
    func(WidgetFactory()); // rvalue calls func, T deduced as Widget
}

void test_ref_collapsing()
{
    // in cpp, reference of reference is not allowed
    int x;
    // uncomment to see the compile error
    // auto & & rx = x; // error: cannot form reference to reference

    Widget w;
    func(w); // T deduced as Widget&
    // what is the type of param in the above call?
    // void func(Widget& && param);
    // reference collapsing rule: if either of the original references is an lvalue reference, the result is an lvalue
    // reference so, param is Widget& so, T is Widget&

    func(WidgetFactory()); // T deduced as Widget
    // what is the type of param in the above call?
    // void func(Widget&& param);
    // reference collapsing rule: if either of the original references is an lvalue reference, the result is an lvalue
    // reference so, param is Widget&& so, T is Widget
}

template <typename T> void f(T &&fparam)
{
    func(std::forward<T>(fparam));

    // because fparam is a universal reference, we know that T is deduced to be an lvalue reference if fparam is an
    // lvalue and T is deduced to be an rvalue reference if fparam is an rvalue so, we can use std::forward to forward
    // the lvalue/rvalue-ness of fparam to another function std::forward is a conditional cast that forwards its
    // argument as an lvalue or rvalue, depending on whether its argument is an lvalue or rvalue only when passing an
    // rvalue to f, which means T is non-reference type std::forward will cast fparam (fparam is always an lvalue) to an
    // rvalue
}

using std::remove_reference;
template <typename T>                         // in
T &&forward(typename                          // namespace
            remove_reference<T>::type &param) // std
{
    return static_cast<T &&>(param);
}

template <typename T> void f2(T &&fparam)
{
    func(forward<T>(fparam));
}

void test_forward()
{
    Widget w;
    f2(w);
    f2(WidgetFactory());
}

void test_auto()
{
    Widget w;
    auto &&w1 = w; // T deduced as Widget& -> Widget& && -> Widget & (reference collapsing)

    auto &&w2 = WidgetFactory(); // T deduced as Widget -> Widget && (reference collapsing)
}

template <typename T> class MyType
{
  public:
    typedef T &&RvalueRefToT;
};

void test_typedef()
{
    MyType<int>::RvalueRefToT x = 10;  // x is an rvalue reference to int
    MyType<int &>::RvalueRefToT y = x; // y is an lvalue reference to int

    printf("x type is %s\n", boost::typeindex::type_id_with_cvr<decltype(x)>().pretty_name().c_str());
    printf("y type is %s\n", boost::typeindex::type_id_with_cvr<decltype(y)>().pretty_name().c_str());
}

int main()
{
    test_func();
    test_ref_collapsing();
    test_forward();
    test_auto();
    test_typedef();

    return 0;
}