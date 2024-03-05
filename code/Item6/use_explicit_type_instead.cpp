#include <boost/type_index.hpp>
#include <cstdio>
#include <vector>

class Widget
{
  public:
    Widget() = default;
    ~Widget() = default;
};

std::vector<bool> features(const Widget &w)
{
    std::vector<bool> result{true, false, true, false, true, true};
    return result;
}

void processWidget(const Widget &w, bool highPriority)
{
    // won't get the expected result since highPriority is implicitly converted by std::vector<bool>::reference
    printf("processWidget called with highPriority: %d\n", highPriority);
}

double calcEpsilon()
{
    return 0.0000001;
}

int main()
{
    // use static_cast to force the type deduction
    Widget w;
    bool highPriority = static_cast<bool>(features(w)[5]); // explicit type conversion, type deduction result is bool

    float ep = calcEpsilon(); // implicit type conversion, type deduction result is float

    auto ep2 = static_cast<float>(calcEpsilon()); // explicit type conversion, type deduction result is float, and it
                                                  // shows that we mean to convert the result to float

    std::vector<int> c;
    double d = 0.0;
    auto index = static_cast<int>(c.size() + d); // explicit type conversion, type deduction result is int
    return 0;
}