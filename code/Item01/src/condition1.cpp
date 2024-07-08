#include <boost/type_index.hpp>
#include <cstdio>

/*
 * template<typename T>
 * void f(ParamType param);
 * f(expr);
 */

template <typename T> void f(T &param)
{
    using boost::typeindex::type_id_with_cvr;
    printf("T = %s\n", type_id_with_cvr<T>().pretty_name().c_str());
    printf("param = %s\n", type_id_with_cvr<decltype(param)>().pretty_name().c_str());
}

// condition1: ParamType is a reference or pointer, but not a universal reference

template <typename T> void f2(const T &param)
{
    using boost::typeindex::type_id_with_cvr;
    printf("T = %s\n", type_id_with_cvr<T>().pretty_name().c_str());
    printf("param = %s\n", type_id_with_cvr<decltype(param)>().pretty_name().c_str());
}

int main()
{
    const int the_answer = 42;
    auto x = the_answer;
    const auto cx = the_answer;
    const auto &rx = the_answer;

    f(the_answer);
    f(x);
    f(cx);
    f(rx);

    f2(the_answer);
    f2(x);
    f2(cx);
    f2(rx);

    return 0;
}