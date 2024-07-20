#include <boost/type_index.hpp>
#include <functional>
#include <memory>
#include <stdio.h>
#include <vector>

// * Default by-reference capture can lead to dangling references.

// * Default by-value capture is suspceptible to dangling pointers (especially
//   this), and it misleadingly suggests that lambdas are self-contained.


namespace use_of_local_variable
{

using FilterContainer = std::vector<std::function<bool(int)>>;

FilterContainer filters;

void addFilter()
{
    filters.emplace_back([](int value) { return value % 5 == 0; });
}

void addDivisorFilter()
{
    auto calc1 = 5;
    auto calc2 = 7;

    auto divisor = calc1 * calc2;

    // dangerous: capture local variable by reference !
    filters.emplace_back([&](int value) { return value % divisor == 0; } // dangling reference
    );

    filters.emplace_back([&divisor](int value) { return value % divisor == 0; } // dangling reference
    );

    // safe way to capture local variable, using name of local variable or using '='
    filters.emplace_back([divisor](int value) { return value % divisor == 0; });
}

// this is a safe way to capture local variables
template <typename C> void workWithContainer(const C &container)
{
    auto calc1 = 5;
    auto calc2 = 7;
    auto divisor = calc1 * calc2; // same as above

    using ContElemT = typename C::value_type; // C++14: std::decay_t<decltype(container.front())>;
    using std::begin;
    using std::end;

    if (std::all_of(begin(container), end(container), [&](const ContElemT &elem) { return elem % divisor == 0; }))
    {
        printf("All elements are divisible by %d\n", divisor);
    }
    else
    {
        printf("Not all elements are divisible by %d\n", divisor);
    }

    // if using cpp14 or later, we can use 'auto' to simplify the code
    if (std::all_of(begin(container), end(container), [&](const auto &elem) { return elem % divisor == 0; }))
    {
        printf("All elements are divisible by %d\n", divisor);
    }
    else
    {
        printf("Not all elements are divisible by %d\n", divisor);
    }
}

void test()
{
    addFilter();
    addDivisorFilter();

    std::vector<int> values = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50};

    workWithContainer(values);
}

class Widget
{
  public:
    void addFilter() const;     // put the lambda in a member function
    void addFilterGood() const; // put the lambda in a member function in a safe way
  private:
    int divisor{1};
};

void Widget::addFilter() const
{
    filters.emplace_back([=](int value) { return value % divisor == 0; } // captured member variable by `this`
    );

    // uncomment below code will cause compile error
    // filters.emplace_back(
    //     [](int value) { return value % divisor == 0; } // captured member variable by `this`
    // );

    // also not sure if this can pass compile, since divisor is not a local variable
    // filters.emplace_back(
    //     [divisor](int value) { return value % divisor == 0; } // captured member variable by `this`
    // );

    // actually, compiler saw:

    auto currentObjectPtr = this;

    filters.emplace_back([currentObjectPtr](int value) {
        return value % currentObjectPtr->divisor == 0;
    } // captured member variable by `this`
    );
}

// don't call this if you want to see the dangling reference :3
void test_widget()
{
    auto pw = std::make_unique<Widget>();
    pw->addFilter(); // add a filter to the Widget object
} // the filter will be destroyed when the Widget object is destroyed !

void Widget::addFilterGood() const
{
    auto divisorCopy = divisor;

    filters.emplace_back(
        [divisorCopy](int value) { return value % divisorCopy == 0; } // captured member variable by `this`
    );

    // acutually, using '=' is also a good idea,
    // but why take the risk of capturing unwanted variables? such as, 'this' pointer ?

    // in cpp 14, there is a new feature called 'init capture', which can be used to capture member variables

    filters.emplace_back(
        [divisor = divisor](int value) { return value % divisor == 0; } // captured member variable by `this`
    );
}

void test_widget_good()
{
    auto pw = std::make_unique<Widget>();
    pw->addFilterGood(); // add a filter to the Widget object
} // the filter will be destroyed when the Widget object is destroyed !

void addDivisorFilterStatic()
{
    static auto calc1 = 5;
    static auto calc2 = 7;
    static auto divisor = calc1 * calc2;

    // actually, it captures nothing, since they are all a static variable
    filters.emplace_back([=](int value) { return value % divisor == 0; });

    ++divisor;
}

} // namespace use_of_local_variable

int main()
{
    use_of_local_variable::test();
    // use_of_local_variable::test_widget();
    use_of_local_variable::test_widget_good();
    return 0;
}