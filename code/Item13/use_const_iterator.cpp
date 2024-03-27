#include <algorithm>
#include <boost/type_index.hpp>
#include <cstdio>
#include <vector>

//  Prefer const_iterators to iterators.
//  In maximally generic code, prefer non-member versions of begin, end,
//  rbegin, etc., over their member function counterparts.

namespace old_cpp98_standard
{
void test_iterators_98()
{
    std::vector<int> values;

    std::vector<int>::iterator it = std::find(values.begin(), values.end(), 1983); // this code never modifies
                                                                                   // what an iterator points to
    values.insert(it, 1998); // this code never modifies what an iterator points to

    typedef std::vector<int>::iterator IterT; // some typedefs
    typedef std::vector<int>::const_iterator ConstIterT;

    printf("iterator type: %s\n", boost::typeindex::type_id_with_cvr<decltype(it)>().pretty_name().c_str());
    // uncomment the following line to see the error
    // ConstIterT ci = std::find(static_cast<ConstIterT>(values.begin()), // cast
    //                           static_cast<ConstIterT>(values.end()),   // cast
    //                           1983);
    // values.insert(static_cast<IterT>(ci), 1998); // may not compile

    // (That’s not a C++98 restriction. It’s true in C++11, too. const_iterators
    // simply don’t convert to iterators, no matter how much it might seem like they
    // should.
}

} // namespace old_cpp98_standard

namespace cpp11_standard
{
void test_iterators_11()
{
    std::vector<int> values;

    auto it = std::find(values.cbegin(), values.cend(), 1983); // use cbegin and cend
    values.insert(it, 1998);
    printf("iterator type: %s\n", boost::typeindex::type_id_with_cvr<decltype(it)>().pretty_name().c_str());
}

// a non-member version of cbegin and cend
template <typename C> auto cbegin_c11(const C &container) -> decltype(std::begin(container))
{
    return std::begin(container);
}

template <typename C> auto cend_c11(const C &container) -> decltype(std::end(container))
{
    return std::end(container);
}

void test_non_member_cbegin_cend()
{
    std::vector<int> values;
    std::vector<int>::const_iterator it = std::find(cbegin_c11(values), cend_c11(values), 1983);
    values.insert(it, 1998);
    printf("iterator type: %s\n", boost::typeindex::type_id_with_cvr<decltype(it)>().pretty_name().c_str());

    {
        int arr[5] = {1, 2, 3, 4, 5};
        auto it = std::find(cbegin_c11(arr), cend_c11(arr), 3);
        printf("iterator type: %s\n", boost::typeindex::type_id_with_cvr<decltype(it)>().pretty_name().c_str());
        auto step = std::distance(cbegin_c11(arr), it);
        printf("step = %ld\n", step);
    }
}
} // namespace cpp11_standard

namespace cpp14_standard
{
template <typename C, typename V>
void findAndInsert(C &container,       // in container, find
                   const V &targetVal, // first occurrence
                   const V &insertVal) // of targetVal, then insert insertVal there
{
    using std::cbegin;
    using std::cend;

    auto it = std::find(cbegin(container), // non-member cbegin
                        cend(container),   // non-member cend
                        targetVal);

    container.insert(it, insertVal);
}

void test_findAndInsert()
{
    std::vector<int> values;
    findAndInsert(values, 1983, 1998);
    printf("values[0] = %d\n", values[0]);
}

} // namespace cpp14_standard

int main()
{
    old_cpp98_standard::test_iterators_98();
    cpp11_standard::test_iterators_11();
    cpp11_standard::test_non_member_cbegin_cend();
    cpp14_standard::test_findAndInsert();
    return 0;
}