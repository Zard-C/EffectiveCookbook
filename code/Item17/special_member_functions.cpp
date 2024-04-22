#include <boost/type_index.hpp>
#include <cstdio>
#include <map>

// • The special member functions are those compilers may generate on their own:
// default constructor, destructor, copy operations, and move operations.
// • Move operations are generated only for classes lacking explicitly declared
// move operations, copy operations, and a destructor.
// • The copy constructor is generated only for classes lacking an explicitly
// declared copy constructor, and it’s deleted if a move operation is declared.
// The copy assignment operator is generated only for classes lacking an explic‐
// itly declared copy assignment operator, and it’s deleted if a move operation is
// declared. Generation of the copy operations in classes with an explicitly
// declared destructor is deprecated.
// • Member function templates never suppress generation of special member
// functions.

// Now, when I refer to a move operation move-constructing or move-assigning a data
// member or base class, there is no guarantee that a move will actually take place.
// “Memberwise moves” are, in reality, more like memberwise move requests, because
// types that aren’t move-enabled (i.e., that offer no special support for move operations,
// e.g., most C++98 legacy classes) will be “moved” via their copy operations. The heart
// of each memberwise “move” is application of std::move to the object to be moved
// from, and the result is used during function overload resolution to determine
// whether a move or a copy should be performed. Item 23 covers this process in detail.

namespace cpp_11
{

class Foo
{
  public:
    Foo() = default;
    Foo(const Foo &) = default;
    Foo(Foo &&rhs) = default; // c++11, move constructor
    Foo &operator=(const Foo &) = default;
    Foo &operator=(Foo &&rhs) = default; // c++11, move assignment operator
    ~Foo() = default;
};

class Bar
{
  public:
    Bar() = default;
    Bar(const Bar &) = default;
};

class Baz
{
  public:
    Baz()
    {
        p = new int(10);
    }
    ~Baz()
    {
        delete p;
    }

    Baz(Baz &&rhs)
    {
        p = rhs.p;
        rhs.p = nullptr;
    }

  private:
    int *p{nullptr};
};

class Qux
{
  public:
    Qux()
    {
        p = new int(10);
    }
    ~Qux()
    {
        delete p;
    }
    Qux(const Qux &rhs) // copy constructor will suppress the generation of move operations because indicates that the
                        // normal approach to copying an object (memberwise copy) isn’t appropriate for the class
    {
        printf("Qux(const Qux& rhs)\n");
        p = new int(*rhs.p);
    }

  private:
    int *p{nullptr};
};

class Widget
{
  public:
    Widget()
    {
    } // user-declared default ctor  (Item 4)
    ~Widget()
    {
    }                                            // user-declared dtor
    Widget(const Widget &) = default;            // default copy ctor behavior is OK
    Widget &operator=(const Widget &) = default; // default copy assignment behavior is OK
};

class Base
{
  public:
    virtual ~Base() = default;  // make dtor virtual
    Base(Base &&rhs) = default; // suport moving
    Base &operator=(Base &&rhs) = default;

    Base(const Base &) = default; // support copying
    Base &operator=(const Base &) = default;
};

void makeLogEntry(const char *message)
{
    printf("makeLogEntry: %s\n", message);
}

// This looks reasonable, but declaring a destructor has a potentially significant side
// effect: it prevents the move operations from being generated. However, creation of
// the class’s copy operations is unaffected. The code is therefore likely to compile, run,
// and pass its functional testing. That includes testing its move functionality, because
// even though this class is no longer move-enabled, requests to move it will compile
// and run.
class StringTable
{
  public:
    StringTable()
    {
        makeLogEntry("Creating StringTable object");
    } // added
    ~StringTable()
    {
        makeLogEntry("Destroying StringTable object");
    } // also added

    StringTable(const StringTable &rhs)
    {
        makeLogEntry("StringTable copy constructor");
    }
    StringTable &operator=(const StringTable &rhs)
    {
        makeLogEntry("StringTable copy assignment operator");
        return *this;
    }

  private:
    std::map<int, std::string> values; // as before
};

// compilers will still generate copy and move operations for Widget (assuming the
// usual conditions governing their generation are fulfilled), even though these tem‐
// plates could be instantiated to produce the signatures for the copy constructor and
// copy assignment operator. (That would be the case when T is Widget.)
class Widget2
{
    template <typename T> Widget2(const T &rhs)
    {
    } // construct Widget2 from anything

    template <typename T> Widget2 &operator=(const T &rhs)
    {
        return *this;
    } // assign Widget2 from anything
};

} // namespace cpp_11

void testBar()
{
    using namespace cpp_11;
    Bar b1;
    Bar b2(b1); // copy constructor won't suppress the generation of copy assignment operator
    Bar b3;
    b3 = b1; // copy assignment operator
}

void testBaz()
{
    using namespace cpp_11;
    Baz b1;
    Baz b2(std::move(b1)); // move constructor will suppress the generation of move assignment operator
    Baz b3;
    // uncommenting the below line will give compilation error
    // b3 = std::move(b1);
}

void testQux()
{
    using namespace cpp_11;
    Qux q1;
    Qux q2(std::move(q1)); // move constructor will suppress the generation of move assignment operator
    Qux q3;
    // uncommenting the below line will give compilation error
    // q3 = std::move(q1);
}

void testStringTable()
{
    using namespace cpp_11;
    StringTable st1;
    StringTable st2(std::move(st1)); // actually it will call copy constructor
    StringTable st3;
    st3 = std::move(st1); // actually it will call copy assignment operator
}

int main()
{
    testBar();
    testBaz();
    testQux();
    testStringTable();
    return 0;
}