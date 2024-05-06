#include <cstdio>
#include <memory>
#include <vector>

// • std::shared_ptrs offer convenience approaching that of garbage collection
// for the shared lifetime management of arbitrary resources.
// • Compared to std::unique_ptr, std::shared_ptr objects are typically
// twice as big, incur overhead for control blocks, and require atomic reference
// count manipulations.
// • Default resource destruction is via delete, but custom deleters are supported.
// The type of the deleter has no effect on the type of the std::shared_ptr.
// • Avoid creating std::shared_ptrs from variables of raw pointer type.

void size_of_shared_ptr()
{
    printf("sizeof(std::shared_ptr<int>) = %zu\n", sizeof(std::shared_ptr<int>));

    auto deleter = [](int *p) { delete p; };
    std::shared_ptr<int> p = std::shared_ptr<int>(new int, deleter);
    printf("sizeof(p) is %zu\n", sizeof(p));
}

// you can use clang -Xclang -fdump-record-layouts -stdlib=libc++ -c use_shared_ptr.cpp to see the layout of
// shared_ptr<int>
// *** Dumping AST Record Layout
//          0 | class std::shared_ptr<int>
//          0 |   class std::__shared_ptr<int, __gnu_cxx::_S_atomic> (base)
//          0 |     class std::__shared_ptr_access<int, __gnu_cxx::_S_atomic, false, false> (base) (empty)
//          0 |     std::__shared_ptr<int, __gnu_cxx::_S_atomic>::element_type * _M_ptr
//          8 |     class std::__shared_count<__gnu_cxx::_S_atomic> _M_refcount
//          8 |       _Sp_counted_base<(enum __gnu_cxx::_Lock_policy)2U> * _M_pi
//            | [sizeof=16, dsize=16, align=8,
//            |  nvsize=16, nvalign=8]

void use_custom_deleter()
{

    auto loggingDel = [](int *p) {
        printf("deleting %p\n", p);
        delete p;
    };

    std::shared_ptr<int> spw(new int, loggingDel); //   deleter type is not part of ptr type.

    auto customDeleter1 = [](int *p) { /* ... */ delete p; }; // custom deleters
    auto customDeleter2 = [](int *p) { /* ... */ delete p; }; // each with a different type

    std::shared_ptr<int> p1(new int, customDeleter1);
    std::shared_ptr<int> p2(new int, customDeleter2);

    std::vector<std::shared_ptr<int>> spv{p1, p2}; // vector of shared_ptrs with different deleters
}

void double_delete()
{
    int *p = new int;
    std::shared_ptr<int> sp1(p);
    std::shared_ptr<int> sp2(p); // double delete
}

class Widget; // forward declaration

std::vector<std::shared_ptr<Widget>> processWidgets;

class Widget
{
  public:
    void process()
    {
        processWidgets.emplace_back(this);
    }

    Widget() = default;
    ~Widget()
    {
        printf("Widget dtor\n");
    }
};

void not_enabling_shared_from_this()
{
    std::shared_ptr<Widget> spw(new Widget);
    spw->process();
}

class Widget2; // forward declaration

std::vector<std::shared_ptr<Widget2>> processWidgets2;

class Widget2 : public std::enable_shared_from_this<Widget2>
{
  public:
    void process()
    {
        processWidgets2.emplace_back(shared_from_this());
    }

    ~Widget2()
    {
        printf("Widget2 dtor\n");
    }
    template <typename... Ts> static std::shared_ptr<Widget2> create(Ts &&...args)
    {
        return std::shared_ptr<Widget2>(new Widget2);
    }

  private:
    Widget2() = default;
};

void enabling_shared_from_this()
{
    std::shared_ptr<Widget2> spw = Widget2::create();
    spw->process();
}

int main()
{
    size_of_shared_ptr();
    use_custom_deleter();
    enabling_shared_from_this();

    // uncomment to see the error
    // double_delete();

    // uncomment to see the error
    // not_enabling_shared_from_this();

    return 0;
}