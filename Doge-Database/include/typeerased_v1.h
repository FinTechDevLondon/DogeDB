#ifndef TYPEERASED_V1_H
#define TYPEERASED_V1_H


#include <cassert>
#include <iostream>
#include <memory>
#include <string>


#define USE_UNIQUE_PTR 1
#define ENABLE_STREAM_INJECT 1

class TypeErasedV1
{
    
    friend std::ostream& operator<<(std::ostream&, const TypeErasedV1&);

    struct Concept;

    template<typename T>
    struct Model;

    friend std::ostream& operator<<(std::ostream&, const Concept&);

    template<typename T>
    friend std::ostream& operator<<(std::ostream&, const Model<T>&);

public:

    template<typename T>
    TypeErasedV1(T value)
        #if USE_UNIQUE_PTR
        : p_data{std::make_unique<Model<T>>(std::move(value))}
        #else
        : p_data{new Model<T>(std::move(value))}
        #endif
    {
        std::cout << "TypeErasedV1(T)" << std::endl;
    }

    TypeErasedV1(const TypeErasedV1 &other)
        #if USE_UNIQUE_PTR
        : p_data(other.p_data->copy())
        //: p_data{std::unique_ptr<Concept>(other.p_data->copy())}
        #else
        : p_data(other.p_data->copy())
        #endif
    {
        std::cout << "TypeErasedV1(const TypeErasedV1&)" << std::endl;
    }

    TypeErasedV1(TypeErasedV1&& other) noexcept
        : p_data{std::move(other.p_data)}
    {
        std::cout << "TypeErasedV1(TypeErased&&)" << std::endl;
    }

    TypeErasedV1& operator=(const TypeErasedV1& other)
    {
        std::cout << "TypeErasedV1::operator=(const TypeErasedV1&)" << std::endl;

        TypeErasedV1 tmp(other);
        *this = std::move(tmp);
        return *this;
    }

    // Note that this is the same as the default compiler generated function
    // with an additional cout statement added
    TypeErasedV1& operator=(TypeErasedV1&& other) noexcept
    {
        std::cout << "TypeErasedV1::operator=(TypeErasedV1&&)" << std::endl;

        p_data = std::move(other.p_data);
        return *this;
    }

    virtual
    ~TypeErasedV1()
    {
        std::cout << "~TypeErasedV1" << std::endl;
    }

private:

    #if USE_UNIQUE_PTR
    std::unique_ptr<Concept> p_data;
    #else
    Concept *p_data;
    #endif


    struct Concept
    {
        virtual
        ~Concept() = default;

        #if USE_UNIQUE_PTR
        virtual
        std::unique_ptr<Concept> copy() const = 0;
        #else
        virtual
        Concept* copy() const = 0;
        #endif

        #if ENABLE_STREAM_INJECT
        virtual
        void stream_inject(std::ostream& os) const = 0;
        #endif
    };

    template<typename T>
    struct Model : public Concept
    {
        template<typename U>
        friend std::ostream& operator<<(std::ostream&, const Model<U>&);

        Model(T value)
            : data(std::move(value))
        {

        }

        #if USE_UNIQUE_PTR
        std::unique_ptr<Concept> copy() const override
        {
            return std::make_unique<Model<T>>(*this);
        }
        #else
        Concept* copy() const override
        {
            return new Model<T>(*this);
        }
        #endif

        #if ENABLE_STREAM_INJECT
        void stream_inject(std::ostream& os) const override
        {
            os << data;
        }
        #endif

        T data;
    };

};



std::ostream& operator<<(std::ostream& os, const TypeErasedV1& typeErased)
{
    os << *typeErased.p_data;
    return os;
}

std::ostream& operator<<(std::ostream& os, const TypeErasedV1::Concept& concept)
{
    // Use virtual function dispatch (member function) to implement an alternative
    // to a free function operator<<, which cannot be virtual since it is a non-
    // member function.
    concept.stream_inject(os);
    return os;
}

// These functions are not technically needed, since the virtual function
// dispatch to stream_inject makes them redundant, but keep them because
// they enable the rather nice feature of having an operator<< which
// works with the derived classes directly, just as it works with the
// base class.
template<typename T>
std::ostream& operator<<(std::ostream& os, const TypeErasedV1::Model<T>& model)
{
    os << model.data;
    return os;
}


#endif // TYPEERASED_V1_H