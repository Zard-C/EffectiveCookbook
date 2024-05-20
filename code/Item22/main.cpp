#include "gadget.h"
#include "widget.h"

int main()
{
    no_pimpl::Widget w1;
    pimpl_raw_pointer::Widget w2;
    pimpl_unique_pointer::Widget w3;
    pimpl_shared_pointer::Widget w4;
    return 0;
}