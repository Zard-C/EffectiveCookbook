#include <boost/type_index.hpp>
#include <cstdio>

template <typename T> void f1(T param)
{
    using boost::typeindex::type_id_with_cvr;
    printf("T = %s\n", type_id_with_cvr<T>().pretty_name().c_str());
    printf("param = %s\n", type_id_with_cvr<decltype(param)>().pretty_name().c_str());
}

template <typename T> void f2(T &param)
{
    using boost::typeindex::type_id_with_cvr;
    printf("T = %s\n", type_id_with_cvr<T>().pretty_name().c_str());
    printf("param = %s\n", type_id_with_cvr<decltype(param)>().pretty_name().c_str());
}

void func(int, double)
{
}

// function type parameter, decay to pointer

int main()
{
    void func(int, double); // func is a function that takes an int and a double

    f1(func); // func decays to a pointer to func
    f2(func); // func is a reference to func

    return 0;
}