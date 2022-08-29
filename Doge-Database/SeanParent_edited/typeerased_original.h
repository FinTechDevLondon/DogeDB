// Sean Parent. Inheritance Is The Base Class of Evil. Going Native 2013
// Video: https://www.youtube.com/watch?v=bIhUE5uUFOA
// Code : https://github.com/sean-parent/sean-parent.github.io/wiki/Papers-and-Presentations

/*
    Copyright 2013 Adobe Systems Incorporated
    Distributed under the MIT License (see license at
    http://stlab.adobe.com/licenses.html)
    This file is intended as example code and is not production quality.
*/

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

//using namespace std;

/******************************************************************************/
// Library

template<typename T>
void draw(const T& t, std::ostream& os, std::size_t position)
{
    os << std::string(position, ' ') << t << std::endl;
}


class Object
{
public:
    template<typename T>
    Object(T value)
        : p_data(new Model<T>(std::move(value)))
    {

    }

    Object(const Object& object)
        : p_data(object.p_data->copy())
    {

    }

    Object(Object&&) noexcept = default;

    Object& operator=(const Object& object)
    {
        Object tmp(object);
        *this = std::move(tmp);
        return *this;
    }

    Object& operator=(Object&& object) noexcept = default;

    friend void draw(const Object& object, std::ostream& os, std::size_t position)
    {
        object.p_data->draw_internal(os, position);
    }

private:

    struct Concept
    {
        virtual
        ~Concept() = default;

        virtual
        Concept* copy() const = 0;

        virtual
        void draw_internal(std::ostream&, std::size_t) const = 0;
    };

    template<typename T>
    struct Model : /*public*/ Concept
    {
        Model(T value)
            : data(std::move(value))
        {

        }

        Concept* copy() const override
        {
            return new Model(*this);
        }

        void draw_internal(std::ostream& os, std::size_t position) const override
        {
            draw(data, os, position);
        }

        T data;
    };

//    std::shared_ptr<const Concept> p_data;
    std::unique_ptr<Concept> p_data;
};

using Document = std::vector<Object>;

void draw(const Document& document, std::ostream& os, std::size_t position)
{
    os << std::string(position, ' ') << "<document>" << std::endl;
    for(const auto& item : document)
    {
        draw(item, os, position + 4);
    }
    os << std::string(position, ' ') << "</document>" << std::endl;
}

/*
using history_t = std::vector<Document>;

void commit(history_t& x)
{
    assert(x.size());
    x.push_back(x.back());
}

void undo(history_t& x)
{
    assert(x.size());
    x.pop_back();
}

Document& current(history_t& x)
{
    assert(x.size());
    return x.back();
}
*/