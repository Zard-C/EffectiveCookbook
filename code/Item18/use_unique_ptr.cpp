#include <cstdio>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>

// • std::unique_ptr is a small, fast, move-only smart pointer for managing
// resources with exclusive-ownership semantics.
// • By default, resource destruction takes place via delete, but custom deleters
// can be specified. Stateful deleters and function pointers as deleters increase the
// size of std::unique_ptr objects.
// • Converting a std::unique_ptr to a std::shared_ptr is easy.

void size_of_unique_ptr()
{
    printf("sizeof(std::unique_ptr<int>) = %zu\n", sizeof(std::unique_ptr<int>));
    printf("sizeof(std::unique_ptr<int, void(*)(int*)>) = %zu\n", sizeof(std::unique_ptr<int, void (*)(int *)>));   // plus size of function pointer
    printf("sizeof(std::unique_ptr<int, std::function<void(int*)>>) = %zu\n",                                       // plus size of std::function
           sizeof(std::unique_ptr<int, std::function<void(int *)>>));
    printf("sizeof(std::function<void(int*)>) = %zu\n", sizeof(std::function<void(int *)>));                        // size of std::function
    auto del = [](int *p) 
    {
        printf("deleting %d\n", *p);    // custom deleter
        delete p;                       // as stateless lambda
    };
    printf("sizeof(std::unique_ptr<int, decltype(del)>) = %zu\n", sizeof(std::unique_ptr<int, decltype(del)>)); // same size as *p
}

class Investment
{
  public:
    // essential
    virtual ~Investment()
    {
    } // design
      // component
};

class Stock : public Investment
{
  public:
    Stock()
    {
        printf("Stock::Stock()\n");
    }
    ~Stock()
    {
        printf("Stock::~Stock()\n");
    }
};

class Bond : public Investment
{
  public:
    Bond()
    {
        printf("Bond::Bond()\n");
    }
    ~Bond()
    {
        printf("Bond::~Bond()\n");
    }
};

class RealEstate : public Investment
{
  public:
    RealEstate()
    {
        printf("RealEstate::RealEstate()\n");
    }
    ~RealEstate()
    {
        printf("RealEstate::~RealEstate()\n");
    }
};

bool needStock = true;
bool needBond = false;
bool needRealEstate = false;
namespace cpp11
{

// template <typename... Ts> std::unique_ptr<Investment> makeInvestment(Ts &&...params)
// {
//     if (needStock)
//     {
//         return std::make_unique<Stock>(std::forward<Ts>(params)...);
//     }
//     else if (needBond)
//     {
//         return std::make_unique<Bond>(std::forward<Ts>(params)...);
//     }
//     else if (needRealEstate)
//     {
//         return std::make_unique<RealEstate>(std::forward<Ts>(params)...);
//     }
//     return nullptr;
// }

auto delInvmt = [](Investment *pInvestment) {
    printf("deleting Investment\n");
    delete pInvestment;
};

// Variant 2: use custom deleter
template <typename... Ts> std::unique_ptr<Investment, decltype(delInvmt)> makeInvestment(Ts &&...params)
{
    std::unique_ptr<Investment, decltype(delInvmt)> pInv(nullptr, delInvmt);
    if (needStock)
    {
        pInv.reset(new Stock(std::forward<Ts>(params)...));
    }
    else if (needBond)
    {
        pInv.reset(new Bond(std::forward<Ts>(params)...));
    }
    else if (needRealEstate)
    {
        pInv.reset(new RealEstate(std::forward<Ts>(params)...));
    }
    return pInv;
}
} // namespace cpp11

namespace cpp14
{
template <typename... Ts> auto makeInvestment(Ts &&...params)
{
    auto delInvmt = [](Investment *pInvestment) {
        printf("deleting Investment\n");
        delete pInvestment;
    };
    std::unique_ptr<Investment, decltype(delInvmt)> pInv(nullptr, delInvmt);
    if (needStock)
    {
        pInv.reset(new Stock(std::forward<Ts>(params)...));
    }
    else if (needBond)
    {
        pInv.reset(new Bond(std::forward<Ts>(params)...));
    }
    else if (needRealEstate)
    {
        pInv.reset(new RealEstate(std::forward<Ts>(params)...));
    }
    return pInv;
}

} // namespace cpp14

void test_makeInvestment11()
{
    using namespace cpp11;

    auto pInv = makeInvestment();
    needStock = false;
    needBond = true;
    // pInv = std::move(makeInvestment());
    auto pInv2 = makeInvestment();
    needBond = false;
    needRealEstate = true;
    auto pInv3 = makeInvestment();
    // pInv = std::move(makeInvestment());
}

void test_makeInvestment14()
{
    using namespace cpp14;

    auto pInv = makeInvestment();
    needStock = false;
    needBond = true;
    // pInv = std::move(makeInvestment());
    auto pInv2 = makeInvestment();
    needBond = false;
    needRealEstate = true;
    auto pInv3 = makeInvestment();
    // pInv = std::move(makeInvestment());

    std::shared_ptr<Investment> pShared = makeInvestment();    //  Converting a std::unique_ptr to a std::shared_ptr is easy.
    //pShared = pInv2;
    pShared = std::move(pInv2);     
}

int main()
{
    size_of_unique_ptr();
    test_makeInvestment11();
    test_makeInvestment14();
    return 0;
}