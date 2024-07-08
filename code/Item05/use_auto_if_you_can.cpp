#include <cstdio>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <vector>

void variable_problem()
{
    int x1; // uninitialized

    // auto x2;    // error: cannot deduce type

    auto x3 = 0; // int
}

template <typename It> void dwim(It b, It e)
{
    while (b != e)
    {
        typename std::iterator_traits<It>::value_type currValue =
            *b; // we use typename to tell the compiler that value_type is a type, which is very long and error-prone
        b++;
    }
    
}

template <typename It> void dwim2(It b, It e)
{
    while (b != e)
    {
        auto currValue = *b; // much better
        b++;
    }
}

// since auto is a type deduction, it can be used to deduce the type of a lambda,
// which is only known at compile time
auto derefUPLess = [](const std::unique_ptr<int> &p1, const std::unique_ptr<int> &p2) { return *p1 < *p2; };

// since c++14, we can use auto in parameters of lambda
auto derefLess = [](const auto &p1, const auto &p2) {
    return *p1 < *p2;
}; // C++14 's compare function can be simplified

// std::function example, it's looks complicated than using `auto`, but it's very useful.
using derefLessFuncType = std::function<bool(const std::unique_ptr<int> &, const std::unique_ptr<int> &)>;
derefLessFuncType derefLessFunc = [](const std::unique_ptr<int> &p1, const std::unique_ptr<int> &p2) {
    return *p1 < *p2;
};

void warnings_of_use_container()
{
    std::vector<int> v{1, 2, 3, 4, 5};
    int size = v.size(); // warning: int/size_t mismatch
    printf("size: %d\n", size);
    auto size2 = v.size(); // much better
    printf("size2: %zu\n", size2);
}

// auto can be used to avoid the problem of temporary object in the loop of map
void iterator_of_map()
{
    std::map<int, std::string> m;
    m[1] = "one";
    m[2] = "two";
    m[3] = "three";

    for (const std::pair<int, std::string> &p : m) // the key of the map is const, so the std::pair's type is not
                                                   // std::pair<int, std::string>, but std::pair<const int, std::string>. The conversion
                                                    // is done by the compiler with an implicit conversion, which will copy every element
                                                    // in the map to a new std::pair<int, std::string> object (temporary object), and then 
                                                    // bind the reference to the temporary object. This is inefficient.
                                                    // at the end of the loop, the temporary object will be destroyed, which is also inefficient.
                                                    // you can change `int` to your own type and see how many times the copy constructor is called.
    {
        printf("key: %d, value: %s\n", p.first, p.second.c_str());
    }


    // we can use auto to avoid the problem
    for (auto it = m.begin(); it != m.end(); ++it)
    {
        printf("key: %d, value: %s\n", it->first, it->second.c_str());
    }
}

// in the end, `auto` is optional, but it can make the code more readable and maintainable,
// on the other hand,  it's not a good idea to use `auto` everywhere, especially when the type is obvious

int main()
{
    variable_problem();
    std::vector<int> v{1, 2, 3, 4, 5};
    dwim(v.begin(), v.end());
    dwim2(v.begin(), v.end());
    warnings_of_use_container();
    iterator_of_map();
    return 0;
}