#ifndef REFERENCE_WRAPPER_H
#define REFERENCE_WRAPPER_H


#include <functional> // std::forward



template<typename T>
class ReferenceWrapper
{

private:

    T& reference;

public:

    constexpr ReferenceWrapper(T& reference) noexcept
        : reference{reference}
    {

    }

    ReferenceWrapper<T> *clone()
    {
        return new ReferenceWrapper(reference);
    }

    /*
    constexpr ReferenceWrapper(const ReferenceWrapper& other) noexcept
        : reference{other.reference}
    {
        
    }
    */

    template<typename U>
    //constexpr decltype(auto) operator=(U&& other) noexcept(noexcept(reference = std::forward<U>(other)))
    constexpr auto operator=(U&& other) noexcept(noexcept(reference = std::forward<U>(other)))
    {
        return reference = std::forward<U>(other);
    }

    constexpr operator T&() const noexcept
    {
        return reference;
    }

};



#endif // REFERENCE_WRAPPER_H