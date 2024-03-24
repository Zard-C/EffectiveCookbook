#include <cstdio>

// • Prefer deleted functions to private undefined ones.
// • Any function may be deleted, including non-member functions and template
// instantiations.

// non-class member function
bool isLucky(int a)
{
    return a == 7;
}

bool isLucky(char) = delete;   // reject chars
bool isLucky(bool) = delete;   // reject bools
bool isLucky(double) = delete; // reject doubles and floats

void caller_function_of_isLucky()
{
    isLucky(21);

    // uncommenting the following lines will cause a compile-time error
    // if(isLucky('a')){}               // error: use of deleted function
    // if(isLucky(true)){}              // error: use of deleted function
    // if(isLucky(3.14)) {}             // error: use of deleted function
}

// reject template instantiations
template <typename T> void processPointer(T *ptr)
{
    // do something with ptr
}

template <> void processPointer<void>(void *) = delete; // reject void pointers

template <> void processPointer<char>(char *) = delete; // reject char pointers, it's basically c type string

template <> void processPointer<const void>(const void *) = delete; // reject const void pointers

template <> void processPointer<const char>(const char *) = delete; // reject const char pointers

// And if you really want to be thorough, you’ll also delete the const volatile void*
// and const volatile char* overloads, and then you’ll get to work on the overloads
// for pointers to the other standard character types: std::wchar_t, std::char16_t,
// and std::char32_t.

void caller_function_of_processPointer()
{
    int a = 10;
    processPointer(&a);

    // uncommenting the following lines will cause a compile-time error
    // processPointer(static_cast<void*>(&a)); // error: use of deleted function
    // processPointer(static_cast<char*>(&a)); // error: use of deleted function
    // processPointer(static_cast<const void*>(&a)); // error: use of deleted function
    // processPointer(static_cast<const char*>(&a)); // error: use of deleted function
}

// Interestingly, if you have a function template inside a class, and you’d like to disable
// some instantiations by declaring them private (à la classic C++98 convention), you
// can’t, because it’s not possible to give a member function template specialization a
// Item 11 | 77
// different access level from that of the main template. If processPointer were a
// member function template inside Widget

class Widget
{
  public:
    template <typename T> void processPointer(T *ptr)
    {
        // do something with ptr
    }

  private:
    // uncommenting the following lines will cause a compile-time error
    // it's not possible to give a member function template specialization a different access level from that of the
    // main template
    // template <>
    // void processPointer<void>(void*){};
};

void caller_function_of_processPointer_in_class()
{
    Widget w;
    int a = 10;
    w.processPointer(&a);

    // uncommenting the following lines will cause a compile-time error
    // w.processPointer(static_cast<void*>(&a));
}

class Widget2
{
  public:
    template <typename T> void processPointer(T *ptr)
    {
        // do something with ptr
    }
};

template <> void Widget2::processPointer<void>(void *) = delete; // reject void pointers

void caller_function_of_processPointer_in_class2()
{
    Widget2 w;
    int a = 10;
    w.processPointer(&a);

    // uncommenting the following lines will cause a compile-time error
    // w.processPointer(static_cast<void*>(&a)); // error: use of deleted function
}

int main()
{
    caller_function_of_isLucky();
    caller_function_of_processPointer();
    caller_function_of_processPointer_in_class();
    caller_function_of_processPointer_in_class2();
    return 0;
}