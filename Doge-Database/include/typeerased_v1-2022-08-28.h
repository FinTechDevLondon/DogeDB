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
    
    friend std::ostream& operator<<(std::ostream& os, const TypeErasedV1& typeErased);

    struct Concept;
    struct IntModel;
    struct StringModel;

    friend std::ostream& operator<<(std::ostream& os, const Concept& concept);
    friend std::ostream& operator<<(std::ostream& os, const IntModel& intModel);
    friend std::ostream& operator<<(std::ostream& os, const StringModel& intModel);

public:

/*
    TypeErasedV1(const int value)
        : p_data{std::make_unique<IntModel>(value)}
    {
        std::cout << "TypeErasedV1(int)" << std::endl;
    }
*/

    TypeErasedV1(int value)
        #if USE_UNIQUE_PTR
        : p_data{std::make_unique<IntModel>(std::move(value))}
        #else
        : p_data{new IntModel(std::move(value))}
        #endif
    {
        std::cout << "TypeErasedV1(int)" << std::endl;
    }

    TypeErasedV1(std::string value)
        #if USE_UNIQUE_PTR
        : p_data{std::make_unique<StringModel>(std::move(value))}
        #else
        : p_data{new StringModel(std::move(value))}
        #endif
    {

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

/*
    TypeErasedV1(const TypeErasedV1 &other)
        #if USE_UNIQUE_PTR
        : p_data{std::make_unique<IntModel>(other.p_data->copy())}
        #else
        : p_data(other.p_data->copy())
        #endif
    {
        std::cout << "TypeErasedV1(const TypeErasedV1&)" << std::endl;
    }

    TypeErasedV1(const TypeErasedV1 &other)
        #if USE_UNIQUE_PTR
        : p_data{std::make_unique<StringModel>(other.p_data->copy())}
        #else
        : p_data(other.p_data->copy())
        #endif
    {
        std::cout << "TypeErasedV1(const TypeErasedV1&)" << std::endl;
    }
*/

    TypeErasedV1(TypeErasedV1&& other) noexcept
        : p_data{std::move(other.p_data)}
    {
        std::cout << "TypeErasedV1(TypeErased&&)" << std::endl;
    }

    TypeErasedV1& operator=(const TypeErasedV1& other)
    {
        std::cout << "TypeErasedV1::operator=(const TypeErasedV1&)" << std::endl;

        //assert(typeid(p_data) == typeid(other.p_data));

        TypeErasedV1 tmp(other);
        *this = std::move(tmp);
        return *this;
    }

    // Note that this is the same as the default compiler generated function
    // with an additional cout statement added
    TypeErasedV1& operator=(TypeErasedV1&& other) noexcept
    {
        std::cout << "TypeErasedV1::operator=(TypeErasedV1&&)" << std::endl;

        //assert(dynamic_cast<decltype(p_data.get())>(other.p_data.get()));

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

        // virtual method for draw was here - need something for operator<< ?
        #if ENABLE_STREAM_INJECT
        virtual
        void stream_inject(std::ostream& os) const = 0;
        #endif
    };

    struct IntModel : public Concept
    {
        friend std::ostream& operator<<(std::ostream& os, const IntModel& intModel);
/*
        IntModel(const int value)
            : data{value}
        {

        }
*/

        IntModel(const int value)
            : data(std::move(value))
        {

        }

        #if USE_UNIQUE_PTR
        std::unique_ptr<Concept> copy() const override
        {
            return std::make_unique<IntModel>(*this);
        }
        #else
        Concept* copy() const override
        {
            return new IntModel(*this);
        }
        #endif

        #if ENABLE_STREAM_INJECT
        void stream_inject(std::ostream& os) const override
        {
            os << data;
        }
        #endif

        int data;
    };

    struct StringModel : public Concept
    {
        friend std::ostream& operator<<(std::ostream& os, const StringModel& stringModel);

        StringModel(const std::string value)
            : data(std::move(value))
        {

        }

        #if USE_UNIQUE_PTR
        std::unique_ptr<Concept> copy() const override
        {
            return std::make_unique<StringModel>(*this);
        }
        #else
        Concept* copy() const override
        {
            return new StringModel(*this);
        }
        #endif

        #if ENABLE_STREAM_INJECT
        void stream_inject(std::ostream& os) const override
        {
            os << data;
        }
        #endif

        std::string data;
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


/*
std::ostream& operator<<(std::ostream& os, const TypeErasedV1::Concept& concept)
{
    if(dynamic_cast<const TypeErasedV1::IntModel*>(&concept))
    {
        //os << dynamic_cast<IntModel&>;

    }
    else if(dynamic_cast<const TypeErasedV1::StringModel*>(&concept))
    {
        //os << dynamic_cast<StringModel&>;

    }

    return os;
}
*/

// These functions are not technically needed, since the virtual function
// dispatch to stream_inject makes them redundant, but keep them because
// they enable the rather nice feature of having an operator<< which
// works with the derived classes directly, just as it works with the
// base class.
std::ostream& operator<<(std::ostream& os, const TypeErasedV1::IntModel& intModel)
{
    os << intModel.data;
    return os;
}

std::ostream& operator<<(std::ostream& os, const TypeErasedV1::StringModel& stringModel)
{
    os << stringModel.data;
    return os;
}


#endif // TYPEERASED_V1_H