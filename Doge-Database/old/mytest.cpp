
#include <utility>

template<typename T>
struct Wrap
{
public:
    constexpr Wrap(T& t) noexcept :
        reference{t}
    {
    }
    constexpr operator T&() const noexcept
    {
        return reference;
    }

    template <typename U>
    constexpr decltype(auto) operator=(U&& u) noexcept(noexcept(reference = std::forward<U>(u)))
    {
        return reference = std::forward<U>(u);
    }
private:
    T& reference;
};

int main()
{
    return 0;
}