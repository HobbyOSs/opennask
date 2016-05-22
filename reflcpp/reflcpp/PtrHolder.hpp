/******************************************************************************\
*                                                                              *
*   @.h                                                                        *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: PtrHolder.hpp,v 1.1 2007/10/25 14:05:26 tdevadit Exp $



#ifndef REFLCPP_PTR_HOLDER_H
#define REFLCPP_PTR_HOLDER_H



// This class is used to hold a pointer to some unknown class.  The holder can
// then be passed around.  Ultimately RTTI is used to determine the actual type
// of this class, and the pointer is extracted.

// TODO: The default template implementations of castTo() and castFrom() assume
// polymorphic types.  So in actuality each type still needs to implement
// these, since it will use static or dynamic cast depending on whether it is
// polymorphic or not.



#include <assert.h>
#include <typeinfo>
#include <memory>



namespace reflcpp {



// The base class.

class PtrHolder {
    protected:
        PtrHolder() {}
    public:
        virtual ~PtrHolder() {}
        virtual const std::type_info &typeId() const = 0;
        //virtual void castTo(PtrHolder *oh) const = 0;
        //virtual void castFrom(const PtrHolder &) = 0;
};

typedef std::shared_ptr<PtrHolder> PtrHolder_smptr_t;

// This is the templated class that contains a typed pointer.  Each
// instantiation of this class will specialize the castTo() and castFrom().
// The reason we need both the castTo() and castFrom() is because sometimes a
// function of class A's metadata needs to cast from a derived type to A.  But
// A's metadata can only know ahead of time about A's base types, not derived
// types.

template <typename T>
class PtrHolder_tmpl : public PtrHolder {
    public:
        PtrHolder_tmpl(T *p) : ptr(p) { assert(p != NULL); }
        // XXX - For non-debug, do nothing for speed.
        PtrHolder_tmpl() : ptr(0) {}
        virtual ~PtrHolder_tmpl() {}
        virtual const std::type_info &typeId() const { return typeid(T); }
        //virtual void castTo(PtrHolder *ph) const;
        //virtual void castFrom(const PtrHolder &ph);
        T *ptr;
};


// This is a templated class using smart pointers. The object, which the pointer
// refers to, will be destructed along with the last Holder object that refers to it.

template <typename T>
class SmartPtrHolder_tmpl : public PtrHolder_tmpl<T> {
        typedef std::shared_ptr<T> shared_ptr_t;
    public:
        SmartPtrHolder_tmpl(T *p)
            : PtrHolder_tmpl<T>(p), sharedPtr(p) {}
        // With a custom deletor
        template <typename D>
        SmartPtrHolder_tmpl(T *p, D d)
            : PtrHolder_tmpl<T>(p), sharedPtr(p, d) {}
        virtual ~SmartPtrHolder_tmpl() {}
        shared_ptr_t sharedPtr;
};


/*
template <typename T>
void
PtrHolder_tmpl<T>::castTo(PtrHolder *ph) const {

    assert(ph != NULL);
    assert(ptr != NULL);

    typename Caster1<T>::map_t &map(
     ClassType_tmpl<T>::s_class_body.m_type_info_map);

    typename Caster1<T>::map_t::const_iterator it = map.find(&ph->typeId());
    if (it != map.end()) {
        const Caster1<T> *c = (*it).second;
        c->upCast(ptr, ph);
    } else {
        ph->castFrom(*this);
    }
}

template <typename T>
void
PtrHolder_tmpl<T>::castFrom(const PtrHolder &ph) {

    typename Caster1<T>::map_t &map(
     ClassType_tmpl<T>::s_class_body.m_type_info_map);

    typename Caster1<T>::map_t::const_iterator it = map.find(&ph.typeId());
    if (it != map.end()) {
        const Caster1<T> *c = (*it).second;
        ptr = c->downCast(ph);
    }
}
*/


/*
    assert(ph != NULL);
    assert(ptr != NULL);
    // This means we are casting from the same type to the same type.
    if (typeid(ph) == typeid(this)) {
        // Actual version might use static cast here for speed.
        PtrHolder_tmpl<T> *ph2(dynamic_cast<PtrHolder_tmpl<T> *>(ph));
        assert(ph2 != NULL && ph2->ptr != NULL);
        ph2->ptr = ptr;
    // Else ph must be a derived class of T.
    } else {
        ph->castFrom(*this);
    }
*/

#if 0
// This default implementation will work for all polymorphic root classes.
template <typename T>
void
PtrHolder_tmpl<T>::castFrom(const PtrHolder &ph) {
    // This means we are casting from the same type to the same type.
    if (typeid(ph) == typeid(*this)) {
        // Actual version should use static cast here for speed.
        const PtrHolder_tmpl<T>
         &ph2(dynamic_cast<const PtrHolder_tmpl<T> &>(ph));
        assert(ph2.ptr != NULL);
        ptr = ph2.ptr;
    } else {
        // Should throw.
        assert(false);
    }
}
#endif



static const char PtrHolder_h_id[] =
"$Id: PtrHolder.hpp,v 1.1 2007/10/25 14:05:26 tdevadit Exp $";



} // namespace



#endif
