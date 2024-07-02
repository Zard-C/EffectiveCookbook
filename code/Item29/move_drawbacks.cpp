#include <array>
#include <boost/type_index.hpp>
#include <cstdio>
#include <vector>

//                                Things to Remember
//                                ------------------

// * Reference collapsing occurs in four contexts: template instantiation, auto
//   type generation, creation and use of typedefs and alias declarations, and
//   decltype.

// * When compilers generate a reference to a reference in a reference collapsing
//   context, the result becomes a single reference.  If either of the original
//   references is an lvalue reference, the result is an lvalue reference.
//   Otherwise it's an rvalue reference.

// * Universal references are rvalue references in contexts where type deduction
//   distinguishes lvalues from rvalues and where reference collapsing occurs.

/*
 * Key idea:
 *
 *   Even types with explicit move support may not benefit as much as you'd
 *   hope.  All containers in the standard C++11 library support moving, for
 *   example, but it would be a mistake to assume that moving all containers is
 *   cheap.  For some containers, this is because there's no truly cheap way to
 *   move their contents.  For others, it's because the truly cheap move
 *   operations the containers offer come with caveats the container elements
 *   can't satisfy.
 *
 */

class Widget
{
};

void test_move_vector()
{
    std::vector<Widget> vw1;

    // ...

    std::vector<Widget> vw2 = std::move(vw1);
    // move vw1 to vw2, O(1) operation, only pointers are swapped
}

void test_move_array()
{
    std::array<Widget, 10000> aw1;

    // ..

    auto aw2 = std::move(aw1);
    // move aw1 to aw2, O(n) operation, all elements in aw1 are moved to aw2
}

class MyClass
{
  public:
    int *data{nullptr};

    // Default constructor
    MyClass() : data(new int(0))
    {
        printf("default ctor called\n");
    }

    // Parameterized constructor
    MyClass(int value) : data(new int(value))
    {
        printf("parameterized ctor called\n");
    }

    // Copy constructor
    MyClass(const MyClass &other) : data(new int(*other.data))
    {
        printf("copy ctor called\n");
    }

    // Move constructor (not noexcept)
    MyClass(MyClass &&other) : data(other.data)
    {
        other.data = nullptr;
        printf("move ctor called\n");
    }

    // Copy assignment operator
    MyClass &operator=(const MyClass &other)
    {
        if (this != &other)
        {
            delete data;
            data = new int(*other.data);
        }
        printf("Copy assignment operator called.\n");
        return *this;
    }

    // Move assignment operator (not noexcept)
    MyClass &operator=(MyClass &&other)
    {
        if (this != &other)
        {
            delete data;
            data = other.data;
            other.data = nullptr;
        }
        printf("Move assignment operator called.\n");
        return *this;
    }

    // Destructor
    ~MyClass()
    {
        delete data;
        printf("Destructor called.\n");
    }
};

void test_move_class()
{
    std::vector<MyClass> vec;
    vec.emplace_back(10);
    vec.emplace_back(20);

    printf("Before move\n");
    for (const auto &v : vec)
    {
        if (v.data)
        {
            printf("%d\n", *v.data);
        }
        else
        {
            printf("nullptr\n");
        }
    }

    // Resize vector to trigger reallocation
    // when resizing, move operation is preferred,
    // but MyClass doesn't have noexcept move constructor
    // so, copy constructor is called
    printf("Resizing vector\n");
    vec.resize(4);

    printf("After resize: \n");
    for (const auto &v : vec)
    {
        if (v.data)
        {
            printf("%d\n", *v.data);
        }
        else
        {
            printf("nullptr\n");
        }
    }
}

int main()
{
    test_move_vector();
    test_move_array();
    test_move_class();
    return 0;
}