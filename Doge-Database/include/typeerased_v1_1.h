#ifndef TYPEERASED_V1_1_H
#define TYPEERASED_V1_1_H


#include <cassert>
#include <iostream>
#include <memory>
#include <string>


#define USE_UNIQUE_PTR 1
#define ENABLE_STREAM_INJECT 1

class TypeErasedV1_1
{
    
    friend std::ostream& operator<<(std::ostream& os, const TypeErasedV1_1& typeErased);

    struct Concept;
    struct IntModel;
    struct StringModel;

    friend std::ostream& operator<<(std::ostream& os, const Concept& concept);
    friend std::ostream& operator<<(std::ostream& os, const IntModel& intModel);
    friend std::ostream& operator<<(std::ostream& os, const StringModel& intModel);

/*
    template<typename T>
    Inner(T value)
        : my_data(std::make_unique<T>(std::move(value)))
    {

    }
*/


public:

/*
    TypeErasedV1_1(const int value)
        : p_data{std::make_unique<IntModel>(value)}
    {
        std::cout << "TypeErasedV1_1(int)" << std::endl;
    }
*/

    TypeErasedV1_1(int value)
        #if USE_UNIQUE_PTR
        : p_data{std::make_unique<IntModel>(std::move(value))}
        #else
        : p_data{new IntModel(std::move(value))}
        #endif
    {
        std::cout << "TypeErasedV1_1(int)" << std::endl;
    }

    TypeErasedV1_1(std::string value)
        #if USE_UNIQUE_PTR
        : p_data{std::make_unique<StringModel>(std::move(value))}
        #else
        : p_data{new StringModel(std::move(value))}
        #endif
    {

    }

    TypeErasedV1_1(const TypeErasedV1_1 &other)
        #if USE_UNIQUE_PTR
        : p_data(other.p_data->copy())
        //: p_data{std::unique_ptr<Concept>(other.p_data->copy())}
        #else
        : p_data(other.p_data->copy())
        #endif
    {
        std::cout << "TypeErasedV1_1(const TypeErasedV1_1&)" << std::endl;
    }

/*
    TypeErasedV1_1(const TypeErasedV1_1 &other)
        #if USE_UNIQUE_PTR
        : p_data{std::make_unique<IntModel>(other.p_data->copy())}
        #else
        : p_data(other.p_data->copy())
        #endif
    {
        std::cout << "TypeErasedV1_1(const TypeErasedV1_1&)" << std::endl;
    }

    TypeErasedV1_1(const TypeErasedV1_1 &other)
        #if USE_UNIQUE_PTR
        : p_data{std::make_unique<StringModel>(other.p_data->copy())}
        #else
        : p_data(other.p_data->copy())
        #endif
    {
        std::cout << "TypeErasedV1_1(const TypeErasedV1_1&)" << std::endl;
    }
*/

    TypeErasedV1_1(TypeErasedV1_1&& other) noexcept
        : p_data{std::move(other.p_data)}
    {
        std::cout << "TypeErasedV1_1(TypeErased&&)" << std::endl;
    }

// return by value from operator= ??
    template<typename U>
    TypeErasedV1_1& operator=(const TypeErasedV1_1& other)
    {
        std::cout << "TypeErasedV1_1::operator=(const TypeErasedV1_1&)" << std::endl;

        //assert(typeid(p_data) == typeid(other.p_data));

        TypeErasedV1_1 tmp(other);
        *this = std::move(tmp);
        return *this;
    }

    // put back the operator=(TypeErased&& other) here
    TypeErasedV1_1& operator=(TypeErasedV1_1&& other) noexcept
    {
        std::cout << "TypeErasedV1_1::operator=(TypeErasedV1_1&&)" << std::endl;

        p_data = other.p_data->copy();
    } // not sure about this being a && type

    template<typename T>
    std::enable_if<decltype(T::p_data) == std::unique_ptr(), TypeErasedV1_1&>::type
    /*TypeErasedV1_1&*/ operator=(TypeErasedV1_1&& other) noexcept
    {
        std::cout << "TypeErasedV1_1::operator=(TypeErasedV1_1&&)" << std::endl;

        p_data = std::move(other.p_data);
        return *this;
    }

    // Note that this is the same as the default compiler generated function
    // with an additional cout statement added
    //template<typename U> // can use this later
    TypeErasedV1_1& operator=(TypeErasedV1_1::IntModel&& intModel) noexcept
    {
        std::cout << "TypeErasedV1_1::operator=(TypeErasedV1_1::IntModel&&)" << std::endl;

        // This RTI call makes the whole seperation of function by type pointless
        assert(typeid(*p_data) == typeid(IntModel));

        p_data = std::make_unique<IntModel>(intModel);
        return *this;
    }

    TypeErasedV1_1& operator=(TypeErasedV1_1::StringModel&& stringModel) noexcept
    {
        std::cout << "TypeErasedV1_1::operator=(TypeErasedV1_1::StringModel&&)" << std::endl;

        // This RTI call makes the whole seperation of function by type pointless
        assert(typeid(*p_data) == typeid(StringModel));

        p_data = std::make_unique<StringModel>(stringModel);
        return *this;
    }

    virtual
    ~TypeErasedV1_1()
    {
        std::cout << "~TypeErasedV1_1" << std::endl;
    }

    // Implicit conversion functions are required to convert the type to something which
    // operator=() can accept
    operator TypeErasedV1_1::IntModel() const
    {
        return static_cast<IntModel>(*p_data);
    }

    operator TypeErasedV1_1::StringModel() const
    {
        return static_cast<StringModel>(*p_data);
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

        IntModel(const Concept& concept)
            //: data(std::move(dynamic_cast<const IntModel*>(&concept)->data))
        {
            if(dynamic_cast<const IntModel*>(&concept))
            {
                data = std::move(dynamic_cast<const IntModel*>(&concept)->data);
            }
            else
            {
                throw std::runtime_error("Dynamic Cast Error");
            }
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

        StringModel(const Concept& concept)
            //: data(std::move(dynamic_cast<const StringModel*>(&concept)->data))
        {
            if(dynamic_cast<const StringModel*>(&concept))
            {
                data = std::move(dynamic_cast<const StringModel*>(&concept)->data);
            }
            else
            {
                throw std::runtime_error("Dynamic Cast Error");
            }
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



std::ostream& operator<<(std::ostream& os, const TypeErasedV1_1& typeErased)
{
    os << *typeErased.p_data;
    return os;
}

std::ostream& operator<<(std::ostream& os, const TypeErasedV1_1::Concept& concept)
{
    // Use virtual function dispatch (member function) to implement an alternative
    // to a free function operator<<, which cannot be virtual since it is a non-
    // member function.
    concept.stream_inject(os);
    return os;
}


/*
std::ostream& operator<<(std::ostream& os, const TypeErasedV1_1::Concept& concept)
{
    if(dynamic_cast<const TypeErasedV1_1::IntModel*>(&concept))
    {
        //os << dynamic_cast<IntModel&>;

    }
    else if(dynamic_cast<const TypeErasedV1_1::StringModel*>(&concept))
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
std::ostream& operator<<(std::ostream& os, const TypeErasedV1_1::IntModel& intModel)
{
    os << intModel.data;
    return os;
}

std::ostream& operator<<(std::ostream& os, const TypeErasedV1_1::StringModel& stringModel)
{
    os << stringModel.data;
    return os;
}


#endif // TYPEERASED_V1_1_H