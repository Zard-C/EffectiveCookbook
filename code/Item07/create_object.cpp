#include <boost/type_index.hpp>
#include <cstdio>
#include <vector>
#include <atomic>
#include <string>

class Widget
{
  public:
    Widget() = default;
    ~Widget() = default;
    Widget(int i, bool b) { printf("Widget(int, bool)\n"); }        // ctors not declaring
    Widget(int i, double d) { printf("Widget(int, double)\n"); }    // std::initializer_list params

  private: 
    int x {0}; // default non-static member initializer c++11
    int y = 0; // default non-static member initializer c++11
    // int z(0); // error
};

void initializer_and_assign()
{
    Widget w1; // default constructor

    Widget w2 = w1; // copy constructor

    w1 = w2; // copy assignment operator
}

void braced_initializer()
{
    // c++11 : uniform initialization
    std::vector<int> v{1, 3, 5, 7, 9};
}

void atomic_initializer()
{
    std::atomic<int> ai1{0};
    std::atomic<int> ai2(0);
    std::atomic<int> ai3 = 0;   // error in c++ 11
}

void initializer_list()
{
    std::initializer_list<int> il = {1, 2, 3, 4, 5};

    double x,y,z;
    
    // int sum1 {x+y+z}; // error! sum of double type to int narrowing conversion.
    int sum2(x + y + z); // ok, the sum of double type to int narrowing conversion.
    int sum3 = x + y + z; // ok, same as sum2
}

void most_vexing_parse()
{
    // Widget w1(); // function declaration

    Widget w2{}; // default constructor

}

class Widget2 {
public:
    Widget2(int i, bool b) { printf("Widget2(int, bool)\n"); }
    Widget2(int i, double d) { printf("Widget2(int, double)\n"); }
    Widget2(std::initializer_list<long double> il) { printf("Widget2(std::initializer_list<long double>)\n"); }
};

class Widget3 {
public:
    Widget3() = default;
    Widget3(int i, bool b) { printf("Widget3(int, bool)\n"); }
    Widget3(int i, double d) { printf("Widget3(int, double)\n"); }
    Widget3(std::initializer_list<std::string> il) { printf("Widget3(std::initializer_list<std::string)\n"); }
};

void ctors_with_std_initializer_list()
{
    Widget w1(10, true); // calls Widget(int, bool) ctor
    Widget w2{10, true}; // calls Widget(int, bool) ctor

    Widget w3(10, 5.0); // calls Widget(int, double) ctor
    Widget w4{10, 5.0}; // calls Widget(int, double) ctor

    Widget2 w5(10, true); // calls Widget2(int, bool) ctor
    Widget2 w6{10, true}; // calls Widget2(std::initializer_list<long double>) ctor

    Widget2 w7(10, 5.0); // calls Widget2(int, double) ctor
    Widget2 w8{10, 5.0}; // calls Widget2(std::initializer_list<long double>) ctor

    Widget3 w9(10, true); // calls Widget3(int, bool) ctor
    Widget3 w10{10, true}; // calls Widget3(int, bool) ctor

    Widget3 w11(10, 5.0); // calls Widget3(int, double) ctor
    Widget3 w12{10, 5.0}; // calls Widget3(int, double) ctor

}


void default_ctor_with_initializer()
{
    Widget3 w1; // default constructor

    Widget3 w2{}; // default constructor

    // Widget3 w3(); // function declaration

    Widget3 w4({}); // calls Widget3(std::initializer_list<std::string>) ctor with an empty list

    Widget3 w5{{}}; // calls Widget(std::initializer_list<int>) ctor with an empty list
}

template <typename T, typename ... Ts>
void doSomeWork1(Ts&& ... params)
{
    T localObject(std::forward<Ts>(params)...);
}

template <typename T, typename ... Ts>
void doSomeWork2(Ts&& ... params)
{
    T localObject{std::forward<Ts>(params)...};
}


void only_caller_can_decide()
{
    std::vector<int> v1 (10, 20);   // calls std::vector<int>::vector(size_t, int) ctor
    std::vector<int> v2 {10, 20};   // calls std::vector<int>::vector(std::initializer_list<int>) ctor

    doSomeWork1<std::vector<int>>(10, 20); // calls doSomeWork1<std::vector<int>, int, int>(10, 20), local object is std::vector<int> v(10, 20)
    doSomeWork2<std::vector<int>>(10, 20); // calls doSomeWork2<std::vector<int>, int, int>(10, 20), local object is std::vector<int> v{10, 20}

}

int main()
{
    int x(0); // initializer is in parentheses

    int y = 0; // initializer follows '='

    int z{0}; // initializer is in braces

    int z1 = {0}; // initializer uses '=' and braces

    initializer_and_assign();
    braced_initializer();
    atomic_initializer();
    initializer_list();
    most_vexing_parse();
    ctors_with_std_initializer_list();
    default_ctor_with_initializer();
    only_caller_can_decide();

    return 0;
}