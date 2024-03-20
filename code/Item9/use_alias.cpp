#include <boost/type_index.hpp>
#include <map>
#include <memory>
#include <stdio.h>
#include <string>
#include <list>

// • typedefs don’t support templatization, but alias declarations do.
// • Alias templates avoid the “::type” suffix and, in templates, the “typename”
// prefix often required to refer to typedefs.
// • C++14 offers alias templates for all the C++11 type traits transformations.

// alias declaration
using UptrMapSS = std::unique_ptr<std::map<std::string, std::string>>;

// typedef declaration
typedef std::unique_ptr<std::map<std::string, std::string>> UptrMapSS2;

// synonym for a function pointer
typedef void (*FP)(int, const std::string &);

// alias declaration
using FP2 = void (*)(int, const std::string &);

template <typename T> using MyAllocList = std::list<T>;

template <typename T> struct MyAllocList2
{
    typedef std::list<T> type;
};

template <typename T> class Widget
{

  private:
    typename MyAllocList<T>::type
        list; // MyAllocList<T>::type is thus a dependent type, because it depends on the template parameter T.
    // the names of dependent types are not known until the template is instantiated, and must precede the by the
    // keyword typename.
    MyAllocList2<T> list2; // MyAllocList2<T> is the name of a type, because it’s an alias template.
};

// some misguided soul may have concocted something like this
class Wine
{
};

// un comment this to see the error
// template <> class MyAllocList<Wine>
// {
//   private:
//     enum class WineType // see Item 10 for info on enum classes
//     {
//         White,
//         Red,
//         Rose
//     };
//     WineType type; // in this class, type is a data member! that's why compilers insist on your asserting that it is a
//                    // type by preceding it with typename.
// };

// c++14 std::transformation<T>::type -> std::transformation_t<T>


int main()
{
    UptrMapSS uptrMapSS(new std::map<std::string, std::string>);
    UptrMapSS2 uptrMapSS2(new std::map<std::string, std::string>);

    FP fp = nullptr;
    FP2 fp2 = nullptr;

    MyAllocList<int> list1;
    MyAllocList2<int>::type list2;
    printf("Is same: %s\n", std::is_same_v<decltype(list1), decltype(list2)> ? "true" : "false");
    printf("fp: %s\n", boost::typeindex::type_id_with_cvr<decltype(fp)>().pretty_name().c_str());
    printf("fp2: %s\n", boost::typeindex::type_id_with_cvr<decltype(fp2)>().pretty_name().c_str());
    printf("list1: %s\n", boost::typeindex::type_id_with_cvr<decltype(list1)>().pretty_name().c_str());
    printf("list2: %s\n", boost::typeindex::type_id_with_cvr<decltype(list2)>().pretty_name().c_str());

    printf("c++11 type:%s\n", boost::typeindex::type_id_with_cvr<std::remove_const<const int * const>::type>().pretty_name().c_str());
    printf("c++14 type:%s\n", boost::typeindex::type_id_with_cvr<std::remove_const_t<const int * const>>().pretty_name().c_str());

    return 0;
}