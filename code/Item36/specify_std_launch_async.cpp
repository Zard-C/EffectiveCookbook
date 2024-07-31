#include <boost/type_index.hpp>
#include <stdio.h>
#include <future>
#include <thread>

// * The default launch policy for std::async permits both asynchronous and
//   synchronous task execution.

// * This flexibility leads to uncertainty when accessing thread_locals, implies
//   that the task may never execute, and affects program logic for timeout-based
//   wait calls.

// * Specify std::launch::async if asynchronous task execution is essential.

void show_std_launch_enum()
{
    printf("std::launch::async = %d\n", static_cast<int>(std::launch::async));
    printf("std::launch::deferred = %d\n", static_cast<int>(std::launch::deferred));
    printf("std::launch::async | std::launch::deferred = %d\n", static_cast<int>(std::launch::async | std::launch::deferred));
}

void use_default_launch_policy()
{
    std::future<int> f = std::async([]() { return 42; });   // use default launch policy
    printf("f.get() = %d\n", f.get());

    // same with policy below
    std::future<int> f2 = std::async(std::launch::async | std::launch::deferred, []() { return 42; });

    // assume that t is the thread invoking use_default_launch_policy
    // It’s not possible to predict whether f will run concurrently with t, because f
    // might be scheduled to run deferred.
    // • It’s not possible to predict whether f runs on a thread different from the
    // thread invoking get or wait on fut. If that thread is t, the implication is that
    // it’s not possible to predict whether f runs on a thread different from t. • It may not be possible to predict whether f runs at all, because it may not be
    // possible to guarantee that get or wait will be called on fut along every path
    // through the program

    // TLS: The default launch policy’s scheduling flexibility often mixes poorly with the use of
    // thread_local variables, because it means that if f reads or writes such thread-local
    // storage (TLS), it’s not possible to predict which thread’s variables will be accessed:
}

void use_async_launch_policy()
{
    std::future<int> f = std::async(std::launch::async, []() { return 42; });
    printf("f.get() = %d\n", f.get());
}

void test_wait()
{
    using namespace std::literals;

    auto fut = std::async(std::launch::async,   // run in async mode (in theory)
    []() {
        std::this_thread::sleep_for(1s);        // sleep for 1 second then return
        return 42;
    });

    // return value of std::future::wait_for https://en.cppreference.com/w/cpp/thread/future/wait_for
    while(fut.wait_for(100ms)   // loop until f has finished running
            != std::future_status::ready)   // which may never happen !
    {
        printf("waiting...\n");
    }
}

void test_wait_forever_loop()
{
    using namespace std::literals;
    auto fut = std::async(std::launch::deferred,   // run in async mode (in theory)
    []() {
        std::this_thread::sleep_for(1s);        // sleep for 1 second then return
        return 42;
    });

    while(fut.wait_for(100ms)   // loop until f has finished running
            != std::future_status::ready)   // which may never happen !
    {
        printf("waiting...\n");
    }
}

template <typename F, typename... Ts>
inline 
std::future<typename std::result_of<F(Ts...)>::type>
reallyAsync(F&& f, Ts&&... params)
{
    return std::async(std::launch::async, std::forward<F>(f), std::forward<Ts>(params)...);
}

template <typename F, typename... Ts>
inline
auto 
reallyAsync14(F&& f, Ts&&... params)
{
    return std::async(std::launch::async, std::forward<F>(f), std::forward<Ts>(params)...);
}

void test_reallyAsync()
{
    auto fut = reallyAsync([]() { return 42; });
    printf("fut.get() = %d\n", fut.get());

    auto fut2 = reallyAsync14([](int a, int b) -> int { return a + b; }, 1, 2);
    printf("fut2.get() = %d\n", fut2.get());
}


int main()
{
    show_std_launch_enum();
    use_default_launch_policy();
    use_async_launch_policy();
    test_wait();
    // uncomment below line to see the forever loop
    // test_wait_forever_loop();
    test_reallyAsync();
    return 0;
}