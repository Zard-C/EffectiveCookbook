#include <boost/type_index.hpp>
#include <cstdio>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

// • C++98-style enums are now known as unscoped enums.
// • Enumerators of scoped enums are visible only within the enum. They convert
// to other types only with a cast.
// • Both scoped and unscoped enums support specification of the underlying type.
// The default underlying type for scoped enums is int. Unscoped enums have no
// default underlying type.
// • Scoped enums may always be forward-declared. Unscoped enums may be
// forward-declared only if their declaration specifies an underlying type.

// Unscoped enum
enum Color
{
    black,
    white,
    red
}; // unscoped enumeration

// auto white = false; // error! redefinition of white

// Scoped enum
enum class EyeColor
{
    blue,
    green,
    brown
}; // scoped enumeration

auto blue = false; // OK, no redefinition of blue

auto c = EyeColor::blue; // also fine, and in accord
// with Item 5's advice to use auto whenever possible

void demo_unscope_enum()
{
    enum Color
    {
        black,
        white,
        red
    }; // unscoped enumeration
    auto primeFactors = [](std::size_t x) -> std::vector<std::size_t> { return std::vector<std::size_t>(); };

    Color c = red;

    if (c < 14.5) //  compare Color to double (!)
    {
        auto factors = primeFactors(c); // compute prime factors of a Color (!)
    }
}

void demo_scope_enum()
{
    enum class Color
    {
        black,
        white,
        red
    }; // scoped enumeration
    auto primeFactors = [](std::size_t x) -> std::vector<std::size_t> { return std::vector<std::size_t>(); };

    Color c = Color::red; // as before, but with scope qualifier

    // if(c < 14.5) //  error! no operator < for Color
    // {
    //     auto factors = primeFactors(c);    // error! no primeFactors for Color
    // }

    if (static_cast<int>(c) < 14.5) //  odd code, but valid
    {
        auto factors = primeFactors(static_cast<int>(c)); // suspects, but it compiles
    }
}

void underlaying_type()
{
    enum Status
    {
        good = 0,
        failed = 1,
        incomplete = 100,
        corrupt = 200,
        indeterminate = 0xFFFFFFFF
    };

    printf("sizeof(Status) = %ld\n", sizeof(Status)); // 4

    enum class Status2; // underlying type is int
    printf("Status2 underlying type :%s\n", boost::typeindex::type_id_with_cvr<std::underlying_type_t<Status2>>()
                                                .pretty_name()
                                                .c_str()); // enum Item10::Status2

    enum class Status3 : std::uint32_t; // underlying type is std::unit32_t
    printf("Status3 underlying type :%s\n",
           boost::typeindex::type_id_with_cvr<std::underlying_type_t<Status3>>().pretty_name().c_str()); // unsigned int

    enum Color : std::uint8_t; // underlying type is std::uint8_t
    printf("Color underlying type :%s\n",
           boost::typeindex::type_id_with_cvr<std::underlying_type_t<Color>>().pretty_name().c_str()); // unsigned char
}

void advantage_of_unscoped_enum()
{
    using UserInfo =             // alias, see Item 9
        std::tuple<std::string,  // name
                   std::string,  // email
                   std::size_t>; // reputation

    UserInfo uInfo("foo", "bar", 233); // object of tuple type

    {
        auto val1 = std::get<1>(uInfo); // get value of field 1

        enum UserInfoFields
        {
            name,
            email,
            reputation
        }; // unscoped enum

        val1 = std::get<email>(uInfo); // ah, get value of email field

        printf("val1 = %s\n", val1.c_str());
    }

    {
        enum class UserInfoFields
        {
            name,
            email,
            reputation
        }; // scoped enum

        auto val1 = std::get<static_cast<int>(UserInfoFields::email)>(
            uInfo); // error! no conversion from UserInfoFields2 to int
    }
}

template <typename E> constexpr typename std::underlying_type<E>::type toUType(E enumerator) noexcept
{
    return static_cast<typename std::underlying_type<E>::type>(enumerator);
}

// c++14 use std::tranformation_t<T>
template <typename E> constexpr auto toUType2(E enumerator) noexcept
{
    return static_cast<std::underlying_type_t<E>>(enumerator);
}

// use auto as return type
template <typename E> constexpr auto toUType3(E enumerator) noexcept
{
    return static_cast<std::underlying_type_t<E>>(enumerator);
}

void user_toUType()
{
    using UserInfo =             // alias, see Item 9
        std::tuple<std::string,  // name
                   std::string,  // email
                   std::size_t>; // reputation

    UserInfo uInfo("foo", "bar", 233); // object of tuple type

    enum class UserInfoFields
    {
        name,
        email,
        reputation
    }; // scoped enum

    auto name = std::get<toUType(UserInfoFields::name)>(uInfo);
    printf("name = %s\n", name.c_str());
    auto email = std::get<toUType2(UserInfoFields::email)>(uInfo);
    printf("email = %s\n", email.c_str());
    auto reputation = std::get<toUType3(UserInfoFields::reputation)>(uInfo);
    printf("reputation = %ld\n", reputation);
}

int main()
{
    demo_unscope_enum();
    demo_scope_enum();
    underlaying_type();
    advantage_of_unscoped_enum();
    user_toUType();
    return 0;
}