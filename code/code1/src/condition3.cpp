#include <boost/type_index.hpp>
#include <cstdio>

template <typename T> void f(T param)
{
    using boost::typeindex::type_id_with_cvr;
    printf("T = %s\n", type_id_with_cvr<T>().pretty_name().c_str());
    printf("param = %s\n", type_id_with_cvr<decltype(param)>().pretty_name().c_str());
}

// condition3: ParamType is neither a pointer nor a reference nor a universal reference (i.e., by value)
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