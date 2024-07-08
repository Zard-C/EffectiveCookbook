#include <boost/type_index.hpp>
#include <cstdio>
#include <typeinfo>
#include <vector>

class Bar
{
  public:
    Bar() = default;
    Bar(const Bar &) = delete;
    Bar(Bar &&) = delete;
};

class Foo
{
  public:
    Foo() = default;
    Foo(const Foo &) = delete;
    Foo(Foo &&) = delete;
    Bar bar;
};

void use_type_info()
{
    const int x = 42;
    const int &y = x;

    printf("type of x is %s\n", typeid(x).name()); // typeid(x) returns a std::type_info object
    printf("type of y is %s\n", typeid(y).name());
}

void use_boost_type_index()
{
    const int x = 42;
    const int &y = x;

    printf("type of x is %s\n", boost::typeindex::type_id_with_cvr<decltype(x)>().pretty_name().c_str());
    printf("type of y is %s\n", boost::typeindex::type_id_with_cvr<decltype(y)>().pretty_name().c_str());
}

template <typename T> void print_boost_type_index(const T &param)
{
    printf("type of T is %s\n", boost::typeindex::type_id_with_cvr<T>().pretty_name().c_str());
    printf("type of param is %s\n", boost::typeindex::type_id_with_cvr<decltype(param)>().pretty_name().c_str());
    
}

template <typename T> void print_type_info(const T &param)
{
    printf("type of T is %s\n", typeid(T).name());
    printf("type of param is %s\n", typeid(param).name());
}

int main()
{
    use_type_info();
    use_boost_type_index();

    std::vector<Foo> v;
    print_type_info(v);
    print_boost_type_index(v);
    
    return 0;
}