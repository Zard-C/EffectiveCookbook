#include <boost/type_index.hpp>
#include <cstdio>
#include <memory>
#include <unordered_map>


// • Use std::weak_ptr for std::shared_ptr-like pointers that can dangle.
// • Potential use cases for std::weak_ptr include caching, observer lists, and the
// prevention of std::shared_ptr cycles.

void basic_usage()
{
    auto spw = std::make_shared<int>(42); // after spw is constructed,
                                          // the pointed-to int's
                                          // ref count (RC) is 1. See
                                          // Item 21  for info
                                          // on std::make_shared

    std::weak_ptr<int> wpw(spw); // wpw points to the same int
                                 // as spw, RC remains 1

    spw = nullptr; // RC goes to 0, and the
                   // Widget is destroyed.
                   // wpw now dangles

    if (wpw.expired()) // wpw is expired
    {
        std::printf("wpw is expired\n");
    }

    // wpw.lock() returns a shared_ptr that points to the same object

    std::shared_ptr<int> spw1 = wpw.lock(); // if wpw's expired,
                                            // spw1 is null
    if (spw1 == nullptr)
    {
        std::printf("wpw is expired\n");
    }
    else
    {
        std::printf("wpw is not expired\n");
    }

    auto spw2 = wpw.lock(); // same as above,
                            // but uses auto
}

class Widget
{
};

std::shared_ptr<const Widget> fastLoadWidget(int widgetId)
{
    using WidgetId = int;
    static std::unordered_map<WidgetId, std::weak_ptr<const Widget>> cache;

    auto objPtr = cache[widgetId].lock(); // objPtr is std::shared_ptr
                                          // to cached object (or null
                                          // if object's not in cache)

    if (objPtr == nullptr)
    {                                        // if not in cache,
        objPtr = std::make_shared<Widget>(); // load it
        cache[widgetId] = objPtr;            // cache it
    }
    return objPtr;
}

void test_fastLoadWidget()
{
    auto spw = fastLoadWidget(1);
    auto spw1 = fastLoadWidget(1);
    auto spw2 = fastLoadWidget(2);
}

class A;
class A2;
class A3;
class B
{
  public:
    std::shared_ptr<A> pa;
    A2 *pa2;
    std::weak_ptr<A3> pa3;
    ~B()
    {
        std::printf("B is destroyed\n");
    }
};

class A
{
  public:
    std::shared_ptr<B> pb;
    ~A()
    {
        std::printf("A is destroyed\n");
    }
    int value = 0;
};

class A2
{
  public:
    std::shared_ptr<B> pb;
    ~A2()
    {
        std::printf("A2 is destroyed\n");
    }
    int value = 0;
};

class A3
{
  public:
    std::shared_ptr<B> pb;
    ~A3()
    {
        std::printf("A3 is destroyed\n");
    }
    int volue = 0;
};

class C
{
  public:
    std::weak_ptr<B> pb;
    ~C()
    {
        std::printf("C is destroyed\n");
    }
};

// The resulting std::shared_ptr cycle (A points to B and B points to A) will
// prevent both A and B from being destroyed
void test_circular_ref()
{
    auto pa = std::make_shared<A>();
    auto pb = std::make_shared<B>();
    pa->pb = pb;
    pb->pa = pa;
}

void test_circular_ref2()
{

    auto pb = std::make_shared<B>();
    {
        auto pa2 = std::make_shared<A2>();
        pa2->pb = pb;
        pb->pa2 = pa2.get();
    }
    // uncomment the following line to see the undefined behavior
    // using freed memory
    // auto res = pb->pa2->value;
    // std::printf("res = %d\n", res);
}

// If A is destroyed, B’s
// pointer back to it will dangle, but B will be able to detect that. Furthermore,
// though A and B will point to one another, B’s pointer won’t affect A’s reference
// count, hence can’t keep A from being destroyed when std::shared_ptrs no
// longer point to it.
void test_circular_ref3()
{
    auto pa3 = std::make_shared<A3>();
    auto pb = std::make_shared<B>();
    pa3->pb = pb;
    pb->pa3 = pa3;
}

int main()
{
    basic_usage();
    test_fastLoadWidget();
    test_circular_ref();
    test_circular_ref2();
    test_circular_ref3();
    return 0;
}