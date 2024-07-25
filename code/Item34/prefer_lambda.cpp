#include <boost/type_index.hpp>
#include <chrono>
#include <functional>
#include <stdio.h>

// * Lambdas are more readable, more expressive, and may be more efficient than
//   using std::bind.

// * In C++11 only, std::bind may be useful for implementing move capture or
//   for binding objects with templatized function call operators.

using Time = std::chrono::steady_clock::time_point;

enum class Sound
{
    Beep,
    Siren,
    Whistle
};

using Duration = std::chrono::steady_clock::duration;

void setAlarm(Time t, Sound s, Duration d)
{
    printf("Alarm set for %ld seconds from now\n", std::chrono::duration_cast<std::chrono::seconds>(d).count());
}

auto setSoundL = [](Sound s) {
    using namespace std::chrono;
    setAlarm(steady_clock::now() + seconds(5), s, seconds(30));

    // for cpp14
    using namespace std::literals;
    setAlarm(steady_clock::now() + 5s, s, 30s);
};

enum class Volume
{
    Normal,
    Loud,
    LoudPlusPlus
};

void setAlarm(Time t, Sound s, Duration d, Volume v)
{
    printf("Alarm set for %ld seconds from now\n", std::chrono::duration_cast<std::chrono::seconds>(d).count());
}

void test_lambda()
{
    setSoundL(Sound::Beep);
}

void test_bind()
{
    using namespace std::chrono;
    using namespace std::literals;
    using namespace std::placeholders;

    // we must cast setAlarm to the correct type
    // because the compiler cannot deduce the type of the function

    using SetAlarm3PramType = void (*)(Time, Sound, Duration);

    auto setSoundB = std::bind(static_cast<SetAlarm3PramType>(setAlarm),
                               steady_clock::now() + 5s, // incorrect !
                               _1, 30s);

    // the alarm will ring 5 seconds from now (called std::bind),
    // not 5 seconds from the time the lambda is called (called lambda)

    auto setSoundB2 =
        std::bind(static_cast<SetAlarm3PramType>(setAlarm), std::bind(std::plus<>(), steady_clock::now(), 5s), _1, 30s);

    setSoundB(Sound::Beep);
    setSoundB2(Sound::Beep);
}

void test_between()
{

    constexpr int lowVal = 1;
    constexpr int highVal = 10;

    auto betweenL = [lowVal, highVal](const auto &val) { return lowVal <= val && val <= highVal; };

    using namespace std::placeholders;

    // cpp 14
    auto betweenB = std::bind(std::logical_and<>(), std::bind(std::less_equal<>(), lowVal, _1),
                              std::bind(std::less_equal<>(), _1, highVal));

    // cpp 11
    auto betweenB2 = std::bind(std::logical_and<bool>(), std::bind(std::less_equal<int>(), lowVal, _1),
                               std::bind(std::less_equal<int>(), _1, highVal));

    // cpp 11
    auto betweenL2 = [lowVal, highVal](int val) { return lowVal <= val && val <= highVal; };

    betweenL(5);
    betweenB(5);
    betweenB2(5);
    betweenL2(5);
}

enum class CompLevel
{
    Low,
    Normal,
    High
}; // compression level

class Widget
{
};

Widget compress(const Widget &w, CompLevel cl)
{
    return w;
}

void test_compress()
{
    using namespace std::placeholders;
    Widget w;
    auto compressRateB = std::bind(compress, w, _1);

    // but how w is stored in the bind object? by value or by reference?
    // the answer is by value, so the bind object will store a copy of w

    auto compressRateL = [w](CompLevel cl) { return compress(w, cl); };

    compressRateL(CompLevel::High);
    compressRateB(CompLevel::High);

    // if want to pass w by reference, we can use std::ref
    auto compressRateB2 = std::bind(compress, std::ref(w), _1);
}

class PolyWidget
{
  public:
    template <typename T> void operator()(const T &param)
    {
        printf("PolyWidget::operator() called with %s\n", boost::typeindex::type_id<T>().pretty_name().c_str());
    }
};

void test_polywidget()
{
    using namespace std::placeholders;
    PolyWidget pw;
    auto boundPW = std::bind(pw, _1);

    // boundPW can accept any type of argument
    boundPW(1930);
    boundPW(nullptr);
    boundPW("Rosebud");
}

void test_poly_widget_lambda()
{
    PolyWidget pw;
    auto boundPW = [&pw](const auto &param) { pw(param); };

    boundPW(1930);
    boundPW(nullptr);
    boundPW("Rosebud");
}

int main()
{
    test_lambda();
    test_bind();
    test_between();
    test_compress();
    test_polywidget();
    test_poly_widget_lambda();

    return 0;
}