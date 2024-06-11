#include <boost/type_index.hpp>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

// • Apply std::move to rvalue references and std::forward to universal refer‐
// ences the last time each is used.
// • Do the same thing for rvalue references and universal references being
// returned from functions that return by value.
// • Never apply std::move or std::forward to local objects if they would other‐
// wise be eligible for the return value optimization.

class Widget
{
  public:
    Widget() : name("default"), p(std::make_shared<SomeDataStructure>())
    {
        printf("Widget default constructor\n");
    }

    Widget(Widget &&rhs) // rhs is an rvalue reference
        : name(std::move(rhs.name)), p(std::move(rhs.p))
    {
        printf("Widget move constructor\n");
    }

    template <typename T> void setName(T &&newName)
    {
        name = std::forward<T>(newName);
    }

    using SomeDataStructure = std::vector<int>;

  private:
    std::string name;
    std::shared_ptr<SomeDataStructure> p;
};

namespace bad_implementation
{

class Widget
{
  public:
    Widget() : name("default"), p(std::make_shared<SomeDataStructure>())
    {
        printf("Widget default constructor\n");
    }

    Widget(Widget &&rhs) // rhs is an rvalue reference
        : name(std::move(rhs.name)), p(std::move(rhs.p))
    {
        printf("Widget move constructor\n");
    }

    template <typename T> void setName(T &&newName)
    {
        // name = std::move(newName); //  can pass the compiler, but it hardly readable
    }

    using SomeDataStructure = std::vector<int>;

  private:
    std::string name;
    std::shared_ptr<SomeDataStructure> p;
};
} // namespace bad_implementation

namespace overload
{
class Widget
{
  public:
    Widget() : name("default"), p(std::make_shared<SomeDataStructure>())
    {
        printf("Widget default constructor\n");
    }

    Widget(Widget &&rhs) // rhs is an rvalue reference
        : name(std::move(rhs.name)), p(std::move(rhs.p))
    {
        printf("Widget move constructor\n");
    }

    void setName(const std::string &newName)
    {
        name = newName;
    }

    void setName(std::string &&newName)
    {
        name = std::move(newName);
    }

    using SomeDataStructure = std::vector<int>;

  private:
    std::string name;
    std::shared_ptr<SomeDataStructure> p;
};
} // namespace overload

void test_widget()
{
    printf("Widget w1(Widget{});\n");
    Widget w1(Widget{});

    printf("Widget w2(std::move(w1));\n");
    Widget w;
    Widget w2(std::move(w));
}

void test_bad_impl_widget()
{
    std::string n = "name"; // n is a local variable
    bad_implementation::Widget w;
    w.setName(n); // moves n into w!

    printf("n = %s\n", n.c_str()); // n's value now unknown
}

// template<class T, class... Args>                // cpp 11
// shared_ptr<T> make_shared(Args&&... args);

// template<class T, class... Args>                // cpp 14
// unique_ptr<T> make_unique(Args&&... args);

// for these 2 types, universal references is the only way to pass arguments

class Matrix
{
  public:
    Matrix(int rows, int cols) : rows(rows), cols(cols), data(rows * cols)
    {
    }

    Matrix(Matrix &&rhs) : rows(rhs.rows), cols(rhs.cols), data(std::move(rhs.data))
    {
    }

    Matrix(const Matrix &rhs) : rows(rhs.rows), cols(rhs.cols), data(rhs.data)
    {
    }

    Matrix &operator=(Matrix &&rhs)
    {
        rows = rhs.rows;
        cols = rhs.cols;
        data = std::move(rhs.data);
        return *this;
    }

    Matrix &operator+=(const Matrix &rhs)
    {
        for (int i = 0; i < data.size(); ++i)
        {
            data[i] += rhs.data[i];
        }
        return *this;
    }

    void show()
    {
        for (int i = 0; i < data.size(); ++i)
        {
            printf("%f ", data[i]);
        }
        printf("\n");
    }

  private:
    int rows, cols;
    std::vector<double> data;
};

Matrix // by-value return
operator+(Matrix && lhs, const Matrix & rhs)
{
    lhs += rhs;
    return std::move(lhs); // move lhs into return value
    // return lhs;                 // copy lsh into return value.
}

void test_matrix()
{
    Matrix m1(2, 2);
    Matrix m2(2, 2);
    Matrix m3 = Matrix(2, 2);

    m3.show();
}

/*
 * Key idea:
 *
 *   Given a function returning a local variable by value, it is not a good idea
 *   to turn the copy into a move in the return statement.
 *
 *   The "copying" version of makeWidget can avoid the need to copy the local
 *   variable w by constructing it in the memory alloted for the function's
 *   return value.  This is known as Return Value Optimization (RVO).
 *
 *   The moving version of makeWidget does just what its name says it does
 *   (assuming Widget offers a move constructor): it moves the contents of w
 *   into makeWidget's return value location.
 *
 *   Developers trying to help their compilers optimize by applying std::move to
 *   a local variable that's being returned are actually limiting the
 *   optimization options available to their compilers!
 *
 *   When the RVO is permitted, either copy elision takes place or std::move is
 *   implicitly applied to local objects being returned.
 */

Widget makeWidget()
{
    Widget w; // local object

    return w; // return local object, 'copy' return value optimization
}

Widget makeWidgetNoRVO()
{
    Widget w;
    return std::move(w);
}

template <typename T>
T use_move_as_return_value(T &&t)
{
    t.process();
    return std::forward<T>(t);
}

template <typename T>
T no_use_move_as_return_value(T &&t)
{
    t.process();
    return t;
}

struct Processable
{
    Processable() {printf("Processable default constructor\n");}
    ~Processable() {printf("Processable destructor\n");}
    Processable(const Processable &) {printf("Processable copy constructor\n");}
    Processable(Processable &&) {printf("Processable move constructor\n");}
    Processable &operator=(const Processable &) {printf("Processable copy assignment\n"); return *this;}
    Processable &operator=(Processable &&) {printf("Processable move assignment\n"); return *this;}
    void process() {printf("processing\n");}
};

void test_makeWidget()
{
    printf("calling makeWidget\n");
    Widget w = makeWidget();

    printf("calling makeWidgetNoRVO\n");
    Widget w2 = makeWidgetNoRVO();
}

void test_use_move_as_return_value()
{
    printf("calling use_move_as_return_value\n");
    auto ret = use_move_as_return_value(Processable{});
}

void test_no_use_move_as_return_value()
{
    printf("calling no_use_move_as_return_value\n");
    auto ret = no_use_move_as_return_value(Processable{});
}

int main()
{
    test_widget();
    test_bad_impl_widget();
    test_matrix();
    test_makeWidget();
    test_use_move_as_return_value();
    test_no_use_move_as_return_value();

    return 0;
}