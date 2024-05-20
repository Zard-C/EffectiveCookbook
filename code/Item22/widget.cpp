#include "widget.h"
#include "gadget.h"
#include <memory>
#include <string>
#include <vector>



// • The Pimpl Idiom decreases build times by reducing compilation dependencies
// between class clients and class implementations.
// • For std::unique_ptr pImpl pointers, declare special member functions in
// the class header, but implement them in the implementation file. Do this even
// if the default function implementations are acceptable.
// • The above advice applies to std::unique_ptr, but not to std::shared_ptr.


namespace no_pimpl
{
// commmented to watch the compilation(ld) error
Widget::Widget() = default;
} // namespace no_pimpl

namespace pimpl_raw_pointer
{
struct Widget::Impl
{
    std::string name;
    std::vector<double> data;
    Gadget g1, g2, g3;
};

Widget::Widget() : pImpl(new Impl)
{
}

Widget::~Widget()
{
    delete pImpl;
}

} // namespace pimpl_raw_pointer

namespace pimpl_unique_pointer
{
struct Widget::Impl
{
    std::string name;
    std::vector<double> data;
    Gadget g1, g2, g3;
};

Widget::Widget() : pImpl(std::make_unique<Impl>())
{
}

// commmented to watch the compilation error
Widget::~Widget() = default;

Widget::Widget(Widget &&rhs) = default;
Widget &Widget::operator=(Widget &&rhs) = default;

Widget::Widget(const Widget &rhs) : pImpl(std::make_unique<Impl>(*rhs.pImpl))
{
}

Widget &Widget::operator=(const Widget &rhs)
{
    *pImpl = *rhs.pImpl;
    return *this;
}

} // namespace pimpl_unique_pointer

namespace pimpl_shared_pointer
{
/*
 * Key Idea:
 *
 *   std::shared_ptr doesn't require pointed-to types to be
 *   complete, and hence no special member functions need to
 *   be defined here.
 */

struct Widget::Impl
{ // as before
    std::string name;
    std::vector<double> data;
    Gadget g1, g2, g3;
};

Widget::Widget()                      // per Item 22 create
    : pImpl(std::make_shared<Impl>()) // std::unique_ptr
{
}
} // namespace pimpl_shared_pointer