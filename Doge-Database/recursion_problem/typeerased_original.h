#include <iostream>
#include <memory>
#include <string>
#include <vector>


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
        //: p_data(std::make_unique<Model<T>>(std::move(value)))
        : p_data(new Model<T>(std::move(value)))
    {

    }

    Object(const Object& object)
        : p_data(object.p_data->copy())
    {

    }

    Object(Object&& other) noexcept = default;

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

    //struct Concept;

    //std::unique_ptr<Concept> p_data;

    struct Concept
    {
        virtual
        ~Concept() = default;

        virtual
        //std::unique_ptr<Concept> copy() const = 0;
        Concept* copy() const = 0;

        virtual
        void draw_internal(std::ostream&, std::size_t) const = 0;
    };

    template<typename T>
    struct Model : /*public*/ Concept
    {
        Model(T value)
            : data(std::move(value))
            //: data{std::move(value)}
        {

        }

        //std::unique_ptr<Concept> copy() const override
        Concept* copy() const override
        {
            //return std::make_unique<Model<T>>(*this);
            //return std::unique_ptr<Model<T>>(new Model<T>(*this));
            return new Model(*this); // change back to Model<T> and test again TODO
        }

        void draw_internal(std::ostream& os, std::size_t position) const override
        {
            //::draw(data, os, position);
            draw(data, os, position);
        }

        T data;
    };
    
    std::unique_ptr<Concept> p_data; // const ?
};


// moved this down here
using Document = std::vector<Object>;

void draw(const Document& document, std::ostream& os, std::size_t position)
{
    os << std::string(position, ' ') << "<document>" << std::endl;
    for(const auto & item : document)
    {
        draw(item, os, position + 4);
    }
    os << std::string(position, ' ') << "</document>" << std::endl;
}