#include <boost/type_index.hpp>
#include <cstdio>

// condition3 : ParamType is neither a pointer nor a reference.

int main()
{
    auto x = 27; // x has type int, ParamType is T itself
    printf("x type: %s\n", boost::typeindex::type_id_with_cvr<decltype(x)>().pretty_name().c_str());
    return 0;
}