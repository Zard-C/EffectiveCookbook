#include <boost/type_index.hpp>
#include <functional>
#include <memory>
#include <stdio.h>
#include <vector>

// • Use C++14’s init capture to move objects into closures.
// • In C++11, emulate init capture via hand-written classes or std::bind.

class Widget
{
  public:
    bool isValidated() const
    {
        return true;
    }
    bool isProcessed() const
    {
        return true;
    }
    bool isArchived() const
    {
        return true;
    }
};

namespace cpp11
{

}

namespace cpp14
{

void test()
{

    auto pw = std::make_unique<Widget>();

    //... // set pw ...

    auto func = [pw = std::move(pw)]() -> bool {
        printf("pw type is: %s\n", boost::typeindex::type_id_with_cvr<decltype(pw)>().pretty_name().c_str());
        return pw->isValidated() && pw->isProcessed() && pw->isArchived();
    };

    printf("in cpp 14: \n");
    func();
}

} // namespace cpp14

namespace cpp11
{
// function object
class IsValAndArch // "is validated and archived"
{
  public:
    using DataType = std::unique_ptr<Widget>;
    explicit IsValAndArch(DataType &&ptr) : pw(std::move(ptr))
    {
    }
    bool operator()() const
    {
        printf("pw type is: %s\n", boost::typeindex::type_id_with_cvr<decltype(pw)>().pretty_name().c_str());
        return pw->isValidated() && pw->isArchived();
    }

  private:
    DataType pw{nullptr};
};

void test()
{
    printf("in cpp 11: \n");
    auto func = IsValAndArch(std::make_unique<Widget>());
    func();
}

} // namespace cpp11

namespace use_bind
{
std::vector<double> data;

// cpp14
auto func = [data = std::move(data)] {
    printf("data type is: %s\n", boost::typeindex::type_id_with_cvr<decltype(data)>().pretty_name().c_str());
};

auto func2 = std::bind(
    [](const std::vector<double> &data) {
        printf("data type is: %s\n", boost::typeindex::type_id_with_cvr<decltype(data)>().pretty_name().c_str());
    },
    std::move(data));

auto func3 = std::bind(
    [](std::vector<double> &data) mutable {
        printf("data type is: %s\n", boost::typeindex::type_id_with_cvr<decltype(data)>().pretty_name().c_str());
    },
    std::move(data));

void test()
{
    printf("in use_bind: \n");
    func();
    func2();
    func3();
}

} // namespace use_bind

namespace closures
{
auto func14 = [pw = std::make_unique<Widget>()] {
    printf("pw type is: %s\n", boost::typeindex::type_id_with_cvr<decltype(pw)>().pretty_name().c_str());
};

auto func11 = std::bind(
    [](std::unique_ptr<Widget> &pw) {
        printf("pw type is: %s\n", boost::typeindex::type_id_with_cvr<decltype(pw)>().pretty_name().c_str());
    },
    std::make_unique<Widget>());

void test()
{
    printf("in closures: \n");
    func14();
    func11();
}

} // namespace closures

int main()
{
    cpp11::test();
    cpp14::test();
    use_bind::test();
    closures::test();
    return 0;
}