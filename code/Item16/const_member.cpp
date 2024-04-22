#include <atomic>
#include <boost/type_index.hpp>
#include <cmath>
#include <mutex>
#include <thread>
#include <vector>

// • Make const member functions thread safe unless you’re certain they’ll never
// be used in a concurrent context.
// • Use of std::atomic variables may offer better performance than a mutex, but
// they’re suited for manipulation of only a single variable or memory location.

class Polynomial
{
  public:
    using RootsType = std::vector<double>;

    RootsType roots() const // roots declared as const, but it is not thread safe
    {
        if (!rootsAreValid) // if cache is not valid
        {
            // calculate roots
            // store them in rootVals
            rootsAreValid = true;
        }
        return rootVals;
    }

  private:
    mutable bool rootsAreValid{false}; // see Item 7 for info
    mutable RootsType rootVals;        // on initializers
};

// Polynomial p;
// …
// /*----- Thread 1 ----- */ /*------- Thread 2 ------- */
// auto rootsOfP = p.roots(); auto valsGivingZero = p.roots();

namespace thread_safe
{
class Polynomial
{
  public:
    using RootsType = std::vector<double>;

    RootsType roots() const
    {
        std::lock_guard<std::mutex> guard{m}; // lock the mutex
        if (!rootsAreValid)                   // if cache is not valid
        {
            // calculate roots
            // store them in rootVals
            rootsAreValid = true;
        }
        return rootVals; // unlock mutex
    }

  private:
    mutable bool rootsAreValid{false};
    mutable RootsType rootVals;
    mutable std::mutex m;
};

class Point // 2D point
{
  public:
    double distanceFromOrigin() const noexcept // see Item 14 for info on noexcept
    {
        ++callCount; // atomic increment
        return std::sqrt((x * x) + (y * y));
    }

  private:
    mutable std::atomic<unsigned> callCount{0};
    double x, y;
};

class Widget1
{
  public:
    int magicValue() const
    {
        if (cacheValid)
            return cachedValue;
        else
        {
            auto expensiveComputation1 = []() { return 42; };
            auto expensiveComputation2 = []() { return 42; };

            auto val1 = expensiveComputation1();
            auto val2 = expensiveComputation2();
            cachedValue = val1 + val2; // uh, oh, part 1
            cacheValid = true;         // uh, oh, part 2
            return cachedValue;
        }
    }
    // • A thread calls Widget::magicValue, sees cacheValid as false, performs the
    // two expensive computations, and assigns their sum to cachedValue.
    // • At that point, a second thread calls Widget::magicValue, also sees cacheValid
    // as false, and thus carries out the same expensive computations that the first
    // thread has just finished. (This “second thread” may in fact be several other
    // threads.)
  private:
    mutable std::atomic<bool> cacheValid{false};
    mutable std::atomic<int> cachedValue{0};
};

class Widget2
{
  public:
    int magicValue() const
    {
        if (cacheValid)
            return cachedValue;
        else
        {
            auto expensiveComputation1 = []() { return 42; };
            auto expensiveComputation2 = []() { return 42; };

            auto val1 = expensiveComputation1();
            auto val2 = expensiveComputation2();
            cacheValid = true;         // uh, oh, part 1
            cachedValue = val1 + val2; // uh, oh, part 2
            return cachedValue;
        }
    }
    // • One thread calls Widget::magicValue and executes through the point where
    // cacheValid is set to true.
    // • At that moment, a second thread calls Widget::magicValue and checks cache
    // Valid. Seeing it true, the thread returns cachedValue, even though the first
    // thread has not yet made an assignment to it. The returned value is therefore
    // incorrect.

  private:
    mutable std::atomic<bool> cacheValid{false};
    mutable std::atomic<int> cachedValue{0};
};

class Widget
{
  public:
    // For a single variable or memory location requiring synchronization,
    // use of a std::atomic is adequate, but once you get to two or more variables
    // or memory locations that require manipulation as a unit, you should reach for a
    // mutex. For Widget::magicValue, that would look like this
    int magicValue() const
    {
        std::lock_guard<std::mutex> guard(m); // lock mutex
        if (cacheValid)
            return cachedValue;
        else
        {
            auto expensiveComputation1 = []() { return 42; };
            auto expensiveComputation2 = []() { return 42; };

            auto val1 = expensiveComputation1();
            auto val2 = expensiveComputation2();
            cachedValue = val1 + val2;
            cacheValid = true;
            return cachedValue;
        }
        // unlock mutex
    }

  private:
    mutable std::mutex m;
    mutable int cachedValue{0};     // no longer atomic
    mutable bool cacheValid{false}; // no longer atomic
};

} // namespace thread_safe

void test_polynomial()
{
    thread_safe::Polynomial p;
    std::thread t1([&p] { auto rootsOfP = p.roots(); });
    std::thread t2([&p] { auto valsGivingZero = p.roots(); });

    t1.join();
    t2.join();
}

void test_point()
{
    thread_safe::Point p;
    std::thread t1([&p] { auto distance = p.distanceFromOrigin(); });
    std::thread t2([&p] { auto distance = p.distanceFromOrigin(); });

    t1.join();
    t2.join();
}

void test_widget1()
{
    thread_safe::Widget1 w;
    std::thread t1([&w] { auto val = w.magicValue(); });
    std::thread t2([&w] { auto val = w.magicValue(); });

    t1.join();
    t2.join();
}

void test_widget2()
{
    thread_safe::Widget2 w;
    std::thread t1([&w] { auto val = w.magicValue(); });
    std::thread t2([&w] { auto val = w.magicValue(); });

    t1.join();
    t2.join();
}

void test_widget()
{
    thread_safe::Widget w;
    std::thread t1([&w] { auto val = w.magicValue(); });
    std::thread t2([&w] { auto val = w.magicValue(); });

    t1.join();
    t2.join();
}

int main()
{
    test_polynomial();
    test_point();
    test_widget1();
    test_widget2();
    test_widget();

    return 0;
}