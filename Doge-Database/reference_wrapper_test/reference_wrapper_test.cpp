

#include "reference_wrapper.h"


#include <list>
#include <iostream>


int main()
{

    int a = 20;

    std::list<std::reference_wrapper<int>> l1;
    std::list<std::reference_wrapper<int>> l2;

    l1.push_back(a);
    l2.push_back(a);

    l2.front().get() = 10;

    std::cout << l1.front() << std::endl;

    std::cout << "--------" << std::endl;

    std::list<ReferenceWrapper<int>> list1;
    std::list<ReferenceWrapper<int>> list2;

    list1.push_back(a);
    list2.push_back(a);

    list2.front() = 30;

    std::cout << list1.front() << std::endl;

    return 0;
}