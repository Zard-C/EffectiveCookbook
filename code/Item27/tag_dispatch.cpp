#include <boost/type_index.hpp>
#include <chrono>
#include <cstdio>
#include <set>
#include <string>
#include <type_traits>

//                                Things to Remember
//                                ------------------

// * Alternatives to the combination of universal references and overloading
//   include the use of distinct function names, passing parameters by
//   lvalue-reference-to-const, passing parameters by value, and using tag
//   dispatch.

// * Constraining templates via std::enable_if permits the use of universal
//   references and overloading together, but it controls the conditions under
//   which compilers may use the universal reference overloads.

// * Universal reference parameters often have efficiency advantages, but they
//   typically have usability disadvantages.

std::string nameFromIdx(int idx)
{
    return "name" + std::to_string(idx);
}
namespace pass_by_value
{

class Person
{
  public:
    explicit Person(std::string &n) : name(std::move(n))
    {
    }

    explicit Person(int idx) : name(nameFromIdx(idx))
    {
    }

  private:
    std::string name;
};

void test()
{
    std::string name = "name";
    Person p1(name);
    Person p2(1);
}

} // namespace pass_by_value

namespace origin
{

std::multiset<std::string> names;

template <typename T> void logAndAdd(T &&name)
{
    auto now = std::chrono::system_clock::now();
    // log(now, "logAndAdd");
    names.emplace(std::forward<T>(name));
}

void test()
{
    std::string petName("Darla");
    logAndAdd(petName);                   // lvalue
    logAndAdd(std::string("Persephone")); // rvalue
    logAndAdd("Patty Dog");               // rvalue
    // uncomment below line to see the compilation error
    // logAndAdd(1); // don't compile
}

} // namespace origin

namespace tag_dispatch1
{

std::multiset<std::string> names;

template <typename T> void logAndAddImpl(T &&name, std::false_type);

void logAndAddImpl(int idx, std::true_type);

template <typename T> void logAndAdd(T &&name)
{
    logAndAddImpl(std::forward<T>(name), std::is_integral<typename std::remove_reference<T>::type>());
}

template <typename T> void logAndAddImpl(T &&name, std::false_type type)
{
    auto now = std::chrono::system_clock::now();
    // log(now, "logAndAdd");
    names.emplace(std::forward<T>(name));
}

void logAndAddImpl(int idx, std::true_type type)
{
    logAndAdd(nameFromIdx(idx));
}

template <typename T> void print(T &&t)
{
    using middle_type = typename std::remove_reference<T>::type;
    printf("middle_type: %s\n", boost::typeindex::type_id_with_cvr<middle_type>().pretty_name().c_str());

    using result_type = typename std::is_integral<middle_type>::type;
    printf("result_type: %s\n", boost::typeindex::type_id_with_cvr<result_type>().pretty_name().c_str());

    // is same type with std::true_type?
    printf("is_same: %d\n", std::is_same<result_type, std::true_type>::value);
}

void test()
{
    std::string petName("Darla");
    logAndAdd(petName);                   // lvalue
    logAndAdd(std::string("Persephone")); // rvalue
    logAndAdd("Patty Dog");               // rvalue
    logAndAdd(1);                         // compile
}

} // namespace tag_dispatch1

namespace enable_if
{

class Person
{
  public:
    template < // as before
        typename T, typename = std::enable_if_t<!std::is_base_of<Person, std::decay_t<T>>::value &&
                                                !std::is_integral<std::remove_reference_t<T>>::value>>
    explicit Person(T &&n) : name(std::forward<T>(n))
    {
        // assert that a std::string can be created from a T object
        static_assert(std::is_constructible<std::string, T>::value,
                      "Parameter n can't be used to construct a std::string");

        // ...                   // the usual ctor work goes here
    }

    explicit Person(int idx) // remainder of Person class (as before)
        : name(nameFromIdx(idx))
    { /* ... */
    }

    Person() = default;
    // ...

  private:
    std::string name;
};

class SpecialPerson : public Person
{
  public:
    SpecialPerson(const SpecialPerson &rhs) : Person(rhs)
    {
    }
    SpecialPerson(SpecialPerson &&rhs) : Person(std::move(rhs))
    {
    }
    SpecialPerson() = default;
};

void test()
{
    std::string petName("Darla");
    Person p1(petName);                   // lvalue
    Person p2(std::string("Persephone")); // rvalue
    Person p3("Patty Dog");               // rvalue
    Person p4(1);                         // compile
    SpecialPerson sp;
    SpecialPerson sp2(sp);
    SpecialPerson sp3(std::move(sp));
}

} // namespace enable_if

int main()
{
    pass_by_value::test();
    origin::test();
    tag_dispatch1::print(1);
    tag_dispatch1::test();
    enable_if::test();
    return 0;
}