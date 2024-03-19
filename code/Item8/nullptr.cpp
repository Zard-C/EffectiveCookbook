#include <memory>
#include <mutex>
#include <stdio.h>

// three overloads of f
void f(int)
{
    printf("f(int)\n");
}

void f(bool)
{
    printf("f(bool)\n");
}

void f(void *)
{
    printf("f(void*)\n");
}

void confusing()
{
    auto result = []() {
        int *a = nullptr;
        return a;
    }();

    if (result == 0)
    {
        // confused, is it a null pointer or a null integer(zero)?
    }

    if (result == nullptr)
    {
        // clear, it's a null pointer
    }
}

// nullptr with template

int f1(std::shared_ptr<int> sp)
{
    return 1;
}

double f2(std::unique_ptr<int> up)
{
    return 2.0;
}

bool f3(int *p)
{
    return true;
}

void caller_function()
{
    using MuxGuard = std::lock_guard<std::mutex>;
    std::mutex f1m, f2m, f3m;
    {
        MuxGuard g(f1m);
        auto result = f1(0); // passing 0 as a null pointer
    }

    {
        MuxGuard g(f2m);
        auto result = f2(NULL); // passing NULL as a null pointer
    }

    {
        MuxGuard g(f3m);
        auto result = f3(nullptr); // passing nullptr as a null pointer
    }
}

// nullptr with template
template <typename FuncType, typename MuxType, typename PtrType>
auto lockAndCall(FuncType func, MuxType &mutex, PtrType ptr) -> decltype(func(ptr)) // tailed return type
{
    using MuxGuard = std::lock_guard<std::mutex>;
    MuxGuard g(mutex);
    return func(ptr);
}

// in C++14, we can use auto as the return type
template <typename FuncType, typename MuxType, typename PtrType>
decltype(auto) lockAndCall14(FuncType func, MuxType &mutex, PtrType ptr)
{
    using MuxGuard = std::lock_guard<std::mutex>;
    MuxGuard g(mutex);
    return func(ptr);
}

int main()
{
    f(0); // calls f(int), not f(void*)
    // f(NULL); // couldn't decide which one to call because multiple matches
    f(nullptr); // calls f(void*)
    std::nullptr_t np; // nullptr_t is a type, not a macro
    std::mutex f1m, f2m, f3m;
    auto result1 = lockAndCall(f1, f1m, nullptr); // passing nullptr as a null pointer, ptr is a std::nullptr_t, and
                                                  // std::nullptr_t is implicitly convertible to any pointer type
    // auto result2 = lockAndCall(f2, f2m, NULL);      // failing to compile, NULL is not a std::unique_ptr
    // auto result3 = lockAndCall(f3, f3m, NULL);      // failing to compile, NULL is not a int*

    return 0;
}