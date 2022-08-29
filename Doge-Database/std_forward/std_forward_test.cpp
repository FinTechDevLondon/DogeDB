#include <iostream>
#include <string>
#include <utility>

void my_function(int& ref)
{
    std::cout << "int& version -> " << ref << std::endl;
}

void my_function(int&& ref)
{
    std::cout << "int&& version -> " << ref << std::endl;
}

template<typename T>
void pass_through(T&& param)
{
    my_function(std::forward<T>(param));
}


void E(int a, int b, int c)
{
    // do something
    return;
}

void f(int a, int b, int c)
{
    E(a, b, c);
}


int main()
{
    int myInt1 = 10;
    int myInt2 = 20;

    pass_through(myInt1);
    pass_through(std::move(myInt2));

    pass_through(1);


    f(1, 2, 3);

    return 0;
}