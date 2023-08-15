#include <iostream>

#include "../lib/include/self_ref.hpp"

int main()
{
    auto i = nstd::make_self_ref<int, int>(1);
    std::cout << *i << std::endl;
}
