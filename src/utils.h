#include <cxxabi.h>
#include <type_traits>
#include <tuple>
#include <string>
#include <iostream>

std::string demangleClassName(auto v)
{
    int status = -4; /* Some magic for ABI */

    std::unique_ptr<char, void (*)(void *)> res{
        abi::__cxa_demangle(typeid(v).name(), NULL, NULL, &status),
        std::free};
    return (status == 0) ? res.get() : typeid(v).name();
}

void show(auto v)
{
    std::string name = demangleClassName(v);
    if (std::is_pod<decltype(v)>::value)
    {
        std::cout << "   [t = " << name << "]" << '\n'
                  << "      sizeof = " << sizeof(v) << '\n'
                  << "      value = " << v << '\n';
    }
    else
    {
        std::cout << "   [t = " << name << "]" << '\n'
                  << "      sizeof = " << sizeof(v) << '\n';
    }
}

void printTupleInfo(const auto &v)
{
    std::string name = demangleClassName(v);
    std::cout << "[T = " << name << "]" << '\n'
              << "   sizeof = " << sizeof(v) << '\n'
              << "   alignment_of = " << std::alignment_of<decltype(v)>::value << '\n';
    std::apply([](auto &&...args)
               { ((show(args)), ...); },
               v);
    std::cout << '\n';
}