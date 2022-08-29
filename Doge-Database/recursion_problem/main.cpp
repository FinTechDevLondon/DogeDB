#include "typeerased_original.h"


class my_class_t
{
    /* ... */
};

void draw(const my_class_t& my_class, std::ostream& os, std::size_t position)
{
    os << std::string(position, ' ') << "my_class_t" << std::endl;
}


int main()
{

    /*
    history_t h(1);

    current(h).emplace_back(0);
    current(h).emplace_back(string("Hello!"));

    draw(current(h), cout, 0);
    cout << "--------------------------" << endl;

    commit(h);

    current(h).emplace_back(current(h));
    current(h).emplace_back(my_class_t());
    current(h)[1] = string("World");

    draw(current(h), cout, 0);
    cout << "--------------------------" << endl;

    undo(h);

    draw(current(h), cout, 0);
    */

    std::cout << "-------------------------" << std::endl;
    std::cout << "Recursion Problem Version" << std::endl;
    std::cout << "-------------------------" << std::endl;

    std::cout << "start of main" << std::endl;

    Document document;

    // TODO: use emplace back ?
    document.push_back(0);
    document.push_back(std::string("Hello World"));
    std::cout << "push back document" << std::endl;
    document.push_back(document);
    std::cout << "done" << std::endl;
    document.push_back(my_class_t());

/*
    Document second_document;
    second_document.emplace_back(22);
    second_document.emplace_back("World...");

    document.emplace_back(second_document);
*/
    std::cout << "Drawing now!" << std::endl;
    draw(document, std::cout, 0);
    std::cout << std::endl;

    return 0;
}

#if 0
g++ -std=c++11 -o value-semantics-unique.exe value-semantics-unique.cpp && value-semantics-unique.exe 

<document>
  0
  Hello!
</document>
--------------------------
<document>
  0
  World
  <document>
    0
    Hello!
  </document>
  my_class_t
</document>
--------------------------
<document>
  0
  Hello!
</document>

#endif