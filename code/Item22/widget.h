#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "gadget.h"
#include <memory>
#include <string>
#include <vector>

namespace no_pimpl
{

class Widget
{
  public:
    Widget();

  private:
    std::string name;
    std::vector<double> data;
    Gadget g1, g2, g3;
};

} // namespace no_pimpl

namespace pimpl_raw_pointer
{

/*
 * Key Idea:
 *
 *   Applying the Pimpl Idiom in C++98 - replace
 *   all data members with a raw pointer to a struct.
 *   Clients of this code no longer need to recompile if
 *   the headers involved with the incomplete types change.
 */

class Widget
{ // still in header "widget.h"
  public:
    Widget();
    ~Widget(); // dtor is needed - see below

  private:
    struct Impl; // declare implementation struct
    Impl *pImpl; // and pointer to it
};

} // namespace pimpl_raw_pointer

namespace pimpl_unique_pointer
{

class Widget
{ // still in header "widget.h"
  public:
    Widget();
    ~Widget(); // dtor is needed - see below

    // Widget(Widget&& rhs) = default;             // right idea
    // Widget& operator=(Widget&& rhs) = default;  // wrong code!

    Widget(const Widget &rhs);
    Widget &operator=(const Widget &rhs);

    Widget(Widget &&rhs);
    Widget &operator=(Widget &&rhs);

  private:
    struct Impl;
    std::unique_ptr<Impl> pImpl; // use smart pointer instead of raw pointer
};
} // namespace pimpl_unique_pointer

namespace pimpl_shared_pointer
{
/*
 * Key Idea:
 *
 *   std::shared_ptr doesn't require pointed-to types to be
 *   complete, and hence no special member functions need to
 *   be declared here.
 */
class Widget
{ // in "widget.h"
  public:
    Widget();
    // no declarations for dtor
    // or move operations

  private:
    struct Impl;
    std::shared_ptr<Impl> pImpl; // std::shared_ptr
};
} // namespace pimpl_shared_pointer

#endif // ! __WIDGET_H__