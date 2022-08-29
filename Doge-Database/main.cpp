

#include <iostream>





#include "reference_wrapper.h"

#include "typeerased_v1.h"


#include <iostream>
#include <list>
#include <vector>

#include <complex>



int main(int argc, char* argv[])
{

    using TypeErased = TypeErasedV1;

    TypeErased t1(1);
    TypeErased t2("Hello World");
    TypeErased t3(2);
    TypeErased t4(std::complex(1, -1));

#if 0
    std::cout << "t1 = t2" << std::endl;
    // should not be able to compile this
    // becase database elements should not be able to change types
    // a column of integers should remain a column of integers
    t1 = t2;
    // this is compiling - how to stop it?
    std::cout << "t1 = t2 [done]" << std::endl;

    // but this should compile!
    t1 = t3;
#endif 

    // what does a database element need to be able to do?
    // get and set
    std::vector<TypeErased> all_objects;
    all_objects.push_back(t1);
    all_objects.push_back(t2);
    all_objects.push_back(t3);
    all_objects.push_back(t4);

    std::vector<TypeErased> all_objects_copy = all_objects;
    all_objects.push_back(all_objects_copy);

    // print everything
    for(const auto &item : all_objects)
    {
        std::cout << "Printing item: ";
        std::cout << item << std::endl;
    }

    // this will be acceptable for now
    std::list<TypeErased> column1_objects;
    column1_objects.push_back(t1);

    std::list<TypeErased> column2_objects;
    column2_objects.push_back(t2);

    // TODO: currently the value symantics are bad because we don't reference the same
    // object from several places, so either need to change to reference symantics
    // or implement a reference wrapper, or use a shared pointer, or use std::ref (?)

    std::cout << std::endl;
    std::cout << std::endl;
    



#if 0
    // more advanced stuff that I want to implement
    // (per column types)
    using Column1Type = decltype(t1);
    using Column2Type = decltype(t2);

    std::list<TypeErased<Column1Type>> column1_objects;
    column1_objects.push_back(t1);
    
    std::list<TypeErased<Column2Type>> column2_objects;
    column2_objects.push_back(t2);
#endif




    return 0;
}