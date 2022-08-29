#ifndef TYPEERASED_V3_H
#define TYPEERASED_V3_H


#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>


template<typename T>
void draw(const T& object, std::ostream& os, std::size_t position)
{
    os << std::string(position, ' ') << object << std::endl;
}


class TypeErasedV3
{
    
    //friend std::ostream& operator<<(std::ostream&, const TypeErasedV3&);

    friend void draw(const TypeErasedV3&, std::ostream&, std::size_t);

    struct Concept;

    //template<typename T>
    //struct Model;

    //friend void draw(const Concept&, const std::ostream&, std::size_t);

    //template<typename T>
    //friend void draw(const Model<T>&, std::ostream&, std::size_t);

public:

    template<typename T>
    TypeErasedV3(T value)
        //: p_data{std::make_unique<Model<T>>(std::move(value))}
        : p_data(new Model<T>(std::move(value)))
    {
        std::cout << "TypeErasedV3(T)" << std::endl;
    }

    TypeErasedV3(const TypeErasedV3 &other) noexcept
        : p_data(other.p_data->copy())
        //: p_data{std::unique_ptr<Concept>(other.p_data->copy())}
    {
        std::cout << "TypeErasedV3(const TypeErasedV3&)" << std::endl;
    }

// TODO: test if this was the fix?
    TypeErasedV3(TypeErasedV3&& other) noexcept = default;
    /*    : p_data{std::move(other.p_data)}
    {
        std::cout << "TypeErasedV3(TypeErased&&)" << std::endl;
    }*/

    TypeErasedV3& operator=(const TypeErasedV3& other)
    {
        std::cout << "TypeErasedV3::operator=(const TypeErasedV3&)" << std::endl;

        TypeErasedV3 tmp(other);
        *this = std::move(tmp);
        return *this;
    }

// TODO: test if this was the fix?
    // Note that this is the same as the default compiler generated function
    // with an additional cout statement added
    TypeErasedV3& operator=(TypeErasedV3&& other) noexcept = default;
    /*{
        std::cout << "TypeErasedV3::operator=(TypeErasedV3&&)" << std::endl;

        p_data = std::move(other.p_data);
        return *this;
    }*/

    virtual
    ~TypeErasedV3()
    {
        std::cout << "~TypeErasedV3" << std::endl;
    }

private:

    std::unique_ptr<Concept> p_data;
    //Concept *p_data;


    struct Concept
    {
        virtual
        ~Concept() = default;

        //virtual
        //std::unique_ptr<Concept> copy() const = 0;
        
        virtual
        Concept* copy() const = 0;

        virtual
        void draw(std::ostream&, std::size_t) const = 0;
    };

    template<typename T>
    struct Model : public Concept
    {
        //template<typename U>
        //friend std::ostream& operator<<(std::ostream&, const Model<U>&);

        Model(T value)
            : data(std::move(value))
        {

        }

        //std::unique_ptr<Concept> copy() const override
        //{
        //    return std::make_unique<Model<T>>(*this);
        //}

        Concept* copy() const override
        {
            return new Model(*this);
        }

        void draw(std::ostream& os, std::size_t position) const override
        {
            //draw(data, os, position);
            os << std::string(position, ' ') << data << std::endl;
        }

        T data;
    };

};


/*
template<typename T>
void draw(const T& data, std::ostream& os, std::size_t position)
{
    os << std::string(position, ' ') << data << std::endl;
}
*/

/*
std::ostream& operator<<(std::ostream& os, const TypeErasedV3& object)
{
    object.draw(os, 0);
}
*/


void draw(const TypeErasedV3& object, std::ostream& os, std::size_t position)
{
    object.p_data->draw(os, position);
}



using Object = TypeErasedV3;
using Document = std::vector<Object>;

/*
void draw(const Object& object, std::ostream& os, std::size_t position)
{
    os << std::string(position, ' ') << object << std::endl;
}
*/

void draw(const Document& document, std::ostream& os, std::size_t position)
{
    os << std::string(position, ' ') << "<document>" << std::endl;
    for(const auto& item : document)
    {
        draw(item, os, position + 4);
    }
    os << std::string(position, ' ') << "</document>" << std::endl;
}



#endif // TYPEERASED_V3_H