#include <cstdio>
#include <memory>
#include <vector>

// • Compared to direct use of new, make functions eliminate source code duplica‐
// tion, improve exception safety, and, for std::make_shared and std::allo
// cate_shared, generate code that’s smaller and faster.
// • Situations where use of make functions is inappropriate include the need to
// specify custom deleters and a desire to pass braced initializers.
// • For std::shared_ptrs, additional situations where make functions may be
// ill-advised include (1) classes with custom memory management and (2) sys‐
// tems with memory concerns, very large objects, and std::weak_ptrs that
// outlive the corresponding std::shared_ptrs.

namespace cpp11
{
// This form of the function
// doesn’t support arrays or custom deleters (see Item 18)
template <typename T, typename... Ts> std::unique_ptr<T> make_unique(Ts &&...params)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}
} // namespace cpp11

void test_make_unique()
{
    auto p = cpp11::make_unique<int>(42);
    printf("p=%d\n", *p);
}

void test_allocate_shared()
{
    auto p = std::allocate_shared<int>(std::allocator<int>(), 42);
    printf("p=%d\n", *p);
}

class Widget
{
};

void test_conparision()
{
    auto upw1(std::make_unique<Widget>());    // with make func
    std::unique_ptr<Widget> upw2(new Widget); // without make func
    auto spw1(std::make_shared<Widget>());    // with make func
    std::shared_ptr<Widget> spw2(new Widget); // without make func
}

void processWidget(std::shared_ptr<Widget> spw, int priority)
{
    printf("processWidget\n");
}

int computePriority()
{
    return 42;
}

void test_processWidget()
{
    processWidget(std::shared_ptr<Widget>(new Widget), // potential
                  computePriority());                  // resource
                                                       // leak !
    // The problem is that the order of evaluation of function arguments is un‐
    // specified in C++, so it’s possible that the call to computePriority will
    // occur before the call to new Widget, in which case the resource allocated
    // by new will be leaked when the exception is thrown in computePriority.

    processWidget(std::make_shared<Widget>(), // no potential
                  computePriority());         // resource leak

    // At runtime, either std::make_shared or computePriority will be called first. If it’s
    // std::make_shared, the raw pointer to the dynamically allocated Widget is safely
    // stored in the returned std::shared_ptr before computePriority is called. If compu
    // tePriority then yields an exception, the std::shared_ptr destructor will see to it
    // that the Widget it owns is destroyed. And if computePriority is called first and
    // yields an exception, std::make_shared will not be invoked, and there will hence be
    // no dynamically allocated Widget to worry about.
}

void test_make_shared()
{
    auto spw = std::make_shared<Widget>();
    // This optimization
    // reduces the static size of the program, because the code contains only one memory
    // allocation call, and it increases the speed of the executable code, because memory is
    // allocated only once. Furthermore, using std::make_shared obviates the need for
    // some of the bookkeeping information in the control block, potentially reducing the
    // total memory footprint for the program
}

void test_custom_deleter()
{
    // none of the make functions permit the specification of custom deleters
    auto deleteWidget = [](Widget *pw) {
        printf("deleteWidget\n");
        delete pw;
    };

    // auto upw1 = std::make_unique<Widget>(42, deleteWidget); // error
    // auto spw1 = std::make_shared<Widget>(42, deleteWidget); // error
    auto upw = std::unique_ptr<Widget, decltype(deleteWidget)>(new Widget, deleteWidget);
    auto spw = std::shared_ptr<Widget>(new Widget, deleteWidget);
}

void test_forwarding()
{
    auto upv = std::make_unique<std::vector<int>>(10, 20);
    auto spv = std::make_shared<std::vector<int>>(10, 20);

    printf("upv.size()=%lu\n", upv->size());
    printf("spv.size()=%lu\n", spv->size());

    // create std::initializer_list
    auto initList = {10, 20};
    auto spv2 = std::make_shared<std::vector<int>>(initList);
    for (auto &v : *spv2)
    {
        printf("%d ", v);
    }
    printf("\n");
}

class ReallyBigType
{
};

void test_ReallyBigType()
{
    auto pBigObj =                         // create very large
        std::make_shared<ReallyBigType>(); // object via
    // std::make_shared
    // create std::shared_ptrs and std::weak_ptrs to
    // large object, use them to work with it
    // final std::shared_ptr to object destroyed here,
    // but std::weak_ptrs to it remain
    // during this period, memory formerly occupied
    // by large object remains allocated
    // final std::weak_ptr to object destroyed here;
    // memory for control block and object is released
}

void test_ReallyBigType2()
{
    std::shared_ptr<ReallyBigType> pBigObj(new ReallyBigType);
    // create very large
    // object via new
    // as before, create std::shared_ptrs and
    // std::weak_ptrs to object, use them with it
    // final std::shared_ptr to object destroyed here,
    // but std::weak_ptrs to it remain;
    // memory for object is deallocated
    // during this period, only memory for the
    // control block remains allocated
    // final std::weak_ptr to object destroyed here;
    // memory for control block is released
}

void test_processWidget2()
{
    auto cusDel = [](Widget *pw) {
        printf("deleteWidget\n");
        delete pw;
    };
    auto spw = std::shared_ptr<Widget>(new Widget, cusDel);
    // processWidget(spw, computePriority());
    processWidget(std::move(spw),     // both efficient
                  computePriority()); // and exception-safe
}

int main()
{
    test_make_unique();
    test_allocate_shared();
    test_conparision();
    test_processWidget();
    test_make_shared();
    test_custom_deleter();
    test_forwarding();
    test_ReallyBigType();
    test_ReallyBigType2();
    test_processWidget2();

    return 0;
}