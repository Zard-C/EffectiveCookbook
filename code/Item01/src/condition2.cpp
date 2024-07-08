#include <boost/type_index.hpp>
#include <cstdio>

template <typename T> void f(T &&param)
{
    using boost::typeindex::type_id_with_cvr;
    printf("T = %s\n", type_id_with_cvr<T>().pretty_name().c_str());
    printf("param = %s\n", type_id_with_cvr<decltype(param)>().pretty_name().c_str());
}

// condition2: ParamType is a universal reference

int main()
{
    int x = 27;
    const int cx = x;
    const int &rx = x;

    f(27);
    f(x);
    f(cx);
    f(rx);
    f(std::move(x));
    f(std::move(cx));
    f(std::move(rx));

    return 0;
}