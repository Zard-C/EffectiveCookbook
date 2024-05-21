#include <boost/type_index.hpp>
#include <cstdio>
#include <type_traits>

// • std::move performs an unconditional cast to an rvalue. In and of itself, it
// doesn’t move anything.
// • std::forward casts its argument to an rvalue only if that argument is bound
// to an rvalue.
// • Neither std::move nor std::forward do anything at runtime.

class Widget
{
};

//  a parameter is always an lvalue, even if its type is an rvalue reference
void f(Widget &&w)
{
    // w is an rvalue reference, but it's an lvalue, because it has a name and can be addressed
    printf("w is an %s\n", boost::typeindex::type_id_with_cvr<decltype(w)>().pretty_name().c_str());
    printf("&w is %p\n", &w);
}

namespace experimetal
{

template <typename T> typename std::remove_reference<T>::type &&move(T &&param)
{
    using ReturnType = typename std::remove_reference<T>::type &&;
    return static_cast<ReturnType>(param);
}
} // namespace experimetal

namespace experimetal_14
{
template <typename T> decltype(auto) move(T &&param)
{
    using ReturnType = typename std::remove_reference<T>::type &&;
    return static_cast<ReturnType>(param);
}
} // namespace experimetal_14

void test_move()
{
    Widget w;
    f(experimetal::move(w));
    f(experimetal_14::move(w));
    printf("moved type: %s\n",
           boost::typeindex::type_id_with_cvr<decltype(experimetal::move(w))>().pretty_name().c_str());
    printf("moved type: %s\n",
           boost::typeindex::type_id_with_cvr<decltype(experimetal_14::move(w))>().pretty_name().c_str());
}

class Annotation
{
  public:
    // explicit Annotation(std::string text);  // param to be copied,
    //                                         // so per Item 41,
    //                                         // pass by value

    explicit Annotation(const std::string text) : value(std::move(text)) // "move" text into value; this code
    {                                                                    /* ... */
    } // doesn't do what it seems to!

  private:
    std::string value;
};

void test_annotation()
{
    std::string text = "hello";
    Annotation a(text); // copy construction of std::string
    // printf("text is %s\n", text.c_str());
}

// typical use of std::forward
void process(const Widget &w)
{
    printf("process(const Widget &w)\n");
}

void process(Widget &&w)
{
    printf("process(Widget &&w)\n");
}

template <typename T> void logAndProcess(T &&param)
{
    process(std::forward<T>(param));
}

void test_logAndProcess()
{
    Widget w;
    logAndProcess(w);
    logAndProcess(std::move(w));
}

namespace count_move
{
class Widget
{
  public:
    Widget() : s("hello")
    {
    }
    Widget(Widget &&rhs) : s(std::move(rhs.s))
    {
        ++moveCtorCalls;
    }

  private:
    static std::size_t moveCtorCalls;
    std::string s;
};
std::size_t Widget::moveCtorCalls = 0;
} // namespace count_move

namespace count_forward
{
class Widget
{
  public:
    Widget() : s("hello")
    {
    }
    Widget(Widget &&rhs)                      // unconventional,
        : s(std::forward<std::string>(rhs.s)) // undesirable
    {
        ++moveCtorCalls;
    } // implementation

  private:
    static std::size_t moveCtorCalls;
    std::string s;
};
std::size_t Widget::moveCtorCalls = 0;

} // namespace count_forward

void test_count()
{
    count_move::Widget w1;
    count_move::Widget w2(std::move(w1));

    count_forward::Widget w3;
    count_forward::Widget w4(std::move(w3));
}

int main()
{
    Widget w;
    f(std::move(w));
    test_move();
    test_annotation();
    test_logAndProcess();

    return 0;
}