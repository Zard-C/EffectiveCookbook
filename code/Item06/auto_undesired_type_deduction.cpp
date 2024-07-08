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

void undesired_type_deduction()
{
    bool highPriority = features(Widget())[5];
    printf("type of highPriority: %s\n", boost::typeindex::type_id_with_cvr<decltype(highPriority)>()
                                             .pretty_name()
                                             .c_str()); // definitely bool but not std::vector<bool>::reference
    printf("highPriority: %d\n", highPriority);         // 1

    auto highPriority2 = features(Widget())[5];
    printf("type of highPriority2: %s\n", boost::typeindex::type_id_with_cvr<decltype(highPriority2)>()
                                              .pretty_name()
                                              .c_str()); // std::vector<bool>::reference

    processWidget(
        Widget(),
        highPriority2); // undefined behavior !!! Because highPriority2 is std::vector<bool>::reference, it is not bool
}

void undefined_behavior()
{
    Widget w;
    auto highPriority = features(
        w)[5]; // features(w) returns std::vector<bool>, it's temporary object, it will be destroyed after the statement
    processWidget(w, highPriority); // undefined behavior !!! visit temporary object by reference
}

int main()
{
    undesired_type_deduction();
    undefined_behavior();
    return 0;
}