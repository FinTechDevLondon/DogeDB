
// Is it possible to use templated types for
//      void *representation;
//      void (*f_delete)(void*);
//      void *(*f_clone)(void*);
//by having a third level of nested classes?

// why doesn't this require a f_move?

class TypeErased
{

    friend void swap(TypeErased &lhs, TypeErased &rhs);

    template <typename Original>
    TypeErased(Original object)
        : representation{new Original(std::move(object))}
        , f_delete{ [](void* r) { delete (Original*)r; } }
        , f_clone{ [](void* r) { return new Original(*(Original)r); } }
    {

    }

    TypeErased()
        : representation{nullptr}
        , f_delete{ [](void* r) { return ; } }
        , f_clone{ [](void* r) { return (void*)nullptr; } }
    {

    }

    TypeErased(const TypeErased& other)
        : representation{other.f_clone(other.representation)}
        , f_clone{other.f_clone}
    {

    }

    TypeErased(TypeErased&& other) noexcept
        : TypeErased()
    {
        swap(*this, other);
    }

    ~TypeErased()
    {
        f_delete(representation);
    }

    // This is implemented in terms of the copy constructor and swap
    TypeErased& operator=(TypeErased other)
    {
        swap(*this, other);
        return *this;
    }


private:

    // All these functions are done in a type-erased (using void*)
    // way. This is [probably] required because the template type
    // does not exist in the context of TypeErased.
    // 
    // So: Cannot have Original *representation and
    // void (*f_delete)(Original*)
    //
    // f_delete -> delete the bits (data / representation)
    // f_clone -> clone the bits (data / representation)

    void *representation;
    void (*f_delete)(void*);
    void *(*f_clone)(void*);
    

};



void swap(TypeErased &lhs, TypeErased &rhs)
{
    using std::swap;

    swap(lhs.representation, rhs.representation);
    swap(lhs.f_delete, rhs.f_delete);
    swap(lhs.f_clone, rhs.f_clone);

}