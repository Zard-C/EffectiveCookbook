#include <boost/type_index.hpp>
#include <cstdio>

template <typename T> void f(T &&param)
{
    using boost::typeindex::type_id_with_cvr;
    printf("T = %s\n", type_id_with_cvr<T>().pretty_name().c_str());
    printf("param = %s\n", type_id_with_cvr<decltype(param)>().pretty_name().c_str());
}

// c type array parameter, decay to pointer

int main()
{
    const char name[] = "J. P. Briggs"; // name's type is const char[13]
    const char *ptrToName = name;       // array decays to pointer, type is const char *
    const char(&refToName)[13] = name;  // type is const char (&)[13]

    f(name);
    f(ptrToName);
    f(refToName);

    return 0;
}