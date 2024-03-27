#include <cstdio>
#include <memory>
#include <vector>

//  Declare overriding functions override.
//  Member function reference qualifiers make it possible to treat lvalue and
//  rvalue objects (*this) differently.

class Base
{
  public:
    virtual void doWork(); // virtual function in class
};
void Base::doWork()
{
    printf("Base::doWork\n");
}

class Derived : public Base
{
  public:
    virtual void doWork(); // overrides Base::doWork, "override" keyword is optional
};

void Derived::doWork()
{
    printf("Derived::doWork\n");
}

void test_override()
{
    std::unique_ptr<Base> upb = std::make_unique<Derived>(); // created base class pointer to derived class object
                                                             // see Item 21 for info on unique_ptr
    upb->doWork();                                           // call doWork() on base class pointer
                                                             // derived class function is invoked
}

// For overriding to occur, serveral requirements must be met:
// 1. The base class function must be virtual.
// 2. The base and derived function names must be identical.(except in the cases of destructors)
// 3. The parameter types of the base and derived functions must be identical.
// 4. The constness of the base and derived functions must be identical.
// 5. The return types and exception specifications of the base and derived functions must be compatible.
// 6. c++ 11 add one more: the function's reference qualifiers must be identical.

class Widget
{
  public:
    void doWork() &;  // This version of doWork applies only when *this is an lvalue
    void doWork() &&; // This version of doWork applies only when *this is an rvalue
};

void Widget ::doWork() &
{
    printf("doWork() &\n");
}

void Widget ::doWork() &&
{
    printf("doWork() &&\n");
}

Widget makeWidget() // factory function
{
    return Widget();
}

void test_reference_qualifiers()
{
    Widget w;
    w.doWork();            // calls lvalue version of doWork
    makeWidget().doWork(); // calls rvalue version of doWork
}

class Base2
{
  public:
    virtual void mf1() const
    {
        printf("Base2::mf1() const\n");
    }
    virtual void mf2(int x)
    {
        printf("Base2::mf2(int x)\n");
    }
    virtual void mf3() &
    {
        printf("Base2::mf3() &\n");
    }
    void mf4() const
    {
        printf("Base2::mf4() const\n");
    }
};

class Derived2 : public Base2
{
  public:
    virtual void mf1()
    {
        printf("Derived2::mf1()\n");
    }
    virtual void mf2(unsigned int x)
    {
        printf("Derived2::mf2(unsigned int x)\n");
    }
    virtual void mf3() &&
    {
        printf("Derived2::mf3() &&\n");
    }
    void mf4() const
    {
        printf("Derived2::mf4() const\n");
    }
};
// uncomment the following code to see the error
// class Derived2Override : public Base2
// {
// public:
//     virtual void mf1() override
//     {
//         printf("Derived2::mf1()\n");
//     }
//     virtual void mf2(unsigned int x) override
//     {
//         printf("Derived2::mf2(unsigned int x)\n");
//     }
//     virtual void mf3() && override
//     {
//         printf("Derived2::mf3() &&\n");
//     }
//     void mf4() const override
//     {
//         printf("Derived2::mf4() const\n");
//     }
// };

void test_Base2()
{
    std::unique_ptr<Base2> upb = std::make_unique<Derived2>();
    upb->mf1();  // calls Base2::mf1
    upb->mf2(1); // calls Base2::mf2
    upb->mf3();  // calls Base2::mf3
    upb->mf4();  // calls Base2::mf4
}

class Base3
{
  public:
    virtual void mf1() const
    {
        printf("Base3::mf1() const\n");
    }
    virtual void mf2(int x)
    {
        printf("Base3::mf2(int x)\n");
    }
    virtual void mf3() &
    {
        printf("Base3::mf3() &\n");
    }
    virtual void mf4() const
    {
        printf("Base3::mf4() const\n");
    }
};

class Derived3 : public Base3
{
  public:
    virtual void mf1() const override
    {
        printf("Derived3::mf1()\n");
    }
    virtual void mf2(int x) override
    {
        printf("Derived3::mf2(unsigned int x)\n");
    }
    virtual void mf3() & override
    {
        printf("Derived3::mf3() &&\n");
    }
    void mf4() const override // "virtual" is optional
    {
        printf("Derived3::mf4() const\n");
    }
};

void test_Base3()
{
    std::unique_ptr<Base3> upb = std::make_unique<Derived3>();
    upb->mf1();  // calls Derived3::mf1
    upb->mf2(1); // calls Derived3::mf2
    upb->mf3();  // calls Derived3::mf3
    upb->mf4();  // calls Derived3::mf4
}

namespace reference_qualifiers_demo
{
class Widget
{
  public:
    using DataType = std::vector<double>;

    DataType &data()
    {
        return values;
    }

  private:
    DataType values;
};

void test()
{
    Widget w;

    auto vals1 = w.data(); // copy w.values to vals1

    auto makeWidget = []() -> Widget { return Widget(); };

    auto vals2 = makeWidget().data(); // copy temporary Widget().values to vals2, we better move it
}

class Widget2
{
  public:
    using DataType = std::vector<double>;

    DataType &data() & // for lvalue Widgets, return lvalue
    {
        return values;
    }
    DataType data() && // for rvalue Widgets, return rvalue
    {
        return std::move(values);
    }

  private:
    DataType values;
};

void test2()
{
    Widget2 w;
    auto makeWidget = []() -> Widget2 { return Widget2(); };

    auto vals1 = w.data(); // calls lvalue overload for
                           // Widget::data, copy-
                           // constructs vals1

    auto vals2 = makeWidget().data(); // calls rvalue overload for
                                      // Widget::data, move-
                                      // constructs vals2
}
} // namespace reference_qualifiers_demo

int main()
{
    test_override();
    test_reference_qualifiers();
    test_Base2();
    test_Base3();
    reference_qualifiers_demo::test();
    reference_qualifiers_demo::test2();
    return 0;
}