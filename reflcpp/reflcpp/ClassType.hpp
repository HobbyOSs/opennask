/******************************************************************************\
*                                                                              *
*   @.h                                                                        *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: ClassType.hpp,v 1.1 2007/10/25 14:05:23 tdevadit Exp $



#ifndef REFLCPP_CLASS_TYPE_H
#define REFLCPP_CLASS_TYPE_H


// ClassType and Member are too closely interrelated to put them in separate files.



#include <reflcpp/Type.hpp>
#include <reflcpp/BoundClassType.hpp>
#include <reflcpp/PtrHolder.hpp>
#include <reflcpp/MemberFunction.hpp>



namespace reflcpp {



class BoundDataMember_body;



class DataMember;
class MemberFunction;
template <typename T> class TypedMember;
class DataMember_body;
class MemberFunction_body;

class ObjHolder;
template <typename T> class ObjHolder_tmpl;




/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   ClassType_body                                                                 |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    This is body class for ClassType objects.

-                                                                             */

class ClassType_body : public Type_body {

    /*--------------------------------------------------------------------------

        Types and Constants                                                   */

	public:

        // This needs to be vector<Member> rather than vector<Member_body>
        // because it gets returned directly from getMembers() for
        // efficiency.
        typedef std::vector<DataMember> dataMember_vector_t;
        typedef std::vector<MemberFunction> memberFunction_vector_t;
        typedef std::vector<const ClassType_body *> base_vec_t;

    /*--------------------------------------------------------------------------

        Friends                                                               */

        friend class ClassType;

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    protected:

        ClassType_body(const std::string &n, const std::type_info &);
        // This should never actually be called, but is provided for the
        // intermediate (not most derived) base classes where ClassType_body
        // is used as a virtual base class.
        ClassType_body();

    public:

        inline virtual ~ClassType_body();

	private:
	    
	    // Disallow these unless specifically allowed.
	    ClassType_body(const ClassType_body &);
	    ClassType_body &operator=(const ClassType_body &);

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:

        virtual const DataMember_body *getDataMember(const std::string &n) const = 0;
        const std::vector<DataMember> &getDataMembers() const { return m_dataMember_vector; }

        virtual const MemberFunction_body *getMemberFunction(const std::string &n) const = 0;
        const std::vector<MemberFunction> &getMemberFunctions() const { return m_memberFunction_vector; }

        template <typename T>
        const BoundDataMember_body *getBoundDataMember(T *, const std::string &n) const;

        template <typename T>
        const BoundMemberFunction_body *getBoundMemberFunction(T *, const std::string &n) const;

        virtual bool derivesFrom(const ClassType_body &) const = 0;
        template <typename T> T *newInstance() const;
        virtual PtrHolder_smptr_t createInstance() const = 0;
        virtual PtrHolder *createInstancePtr() const = 0;

    protected:

        virtual void newInstance(ObjHolder *ph) const = 0;

    /*--------------------------------------------------------------------------

        Data                                                                  */
        
    protected:

        // Vector of all members.  Not strictly necessary, since we can
        // reconstruct this from the member map, but for efficiency we
        // cache a copy.  Also, note that this holds Member_body pointers,
        // and not Member_body_tmpl1 pointers, though in theory we could create
        // another vector in Class_body_tmpl to hold Member_body_tmpl1 pointers.
        // Not sure how useful that would be, though.
        dataMember_vector_t m_dataMember_vector;
        // Vector of all member functions
        memberFunction_vector_t m_memberFunction_vector;


    /*--------------------------------------------------------------------------

        Static Methods                                                        */

    protected:

        static const ClassType_body *getClass(const std::string &);
		static const ClassType_body *getClass(const std::type_info &);
};

/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   ClassType                                                                      |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Class comments>

-                                                                             */

class ClassType : public Type {

    /*--------------------------------------------------------------------------

        Types and Constants                                                   */

    public:

        typedef ClassType_body body_type;

    /*--------------------------------------------------------------------------

        Friends                                                               */

        template <typename T1, typename T2>
        friend T1 dynamic_cast_type(const T2 &t2);

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    protected:

        ClassType(const ClassType_body *i) : Type(i), m_body(i) {}

    // Default versions of these should be fine.
    // ClassType(const ClassType &);
    // ClassType &operator=(const ClassType &); 

    /*--------------------------------------------------------------------------

        Accessors and Convertors                                              */

    public:

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:

        inline DataMember getDataMember(const std::string &n) const;
        const std::vector<DataMember> &getDataMembers() { return m_body->getDataMembers(); }

        inline MemberFunction getMemberFunction(const std::string &n) const;
        const std::vector<MemberFunction> &getMemberFunctions() { return m_body->getMemberFunctions(); }

        template <typename T>
        inline BoundDataMember getBoundDataMember(T *o, const std::string &n) const;

        template <typename T>
        inline BoundMemberFunction getBoundMemberFunction(T *o, const std::string &n) const;

        template <typename T> T *newInstance() const { return m_body->newInstance<T>(); }

        /**
         * Return a SmartPtrHolder of the created object. The SmartPtrHolder is also wrapped
         * with a PtrHolder_smptr_t in order to ensure automatic destruction.
         */
        PtrHolder_smptr_t createInstance() const { return m_body->createInstance(); }

        /**
         * Return a SmartPtrHolder of the created object. The SmartPtrHolder is also wrapped
         * with a PtrHolder_smptr_t in order to ensure automatic destruction.
         */
        PtrHolder *createInstancePtr() const { return m_body->createInstancePtr(); }

        // Two synomous methods.
        inline bool derivesFrom(const ClassType &) const;
        inline bool inheritsFrom(const ClassType &c) const;

    /*--------------------------------------------------------------------------

        Data                                                                  */

    protected:

        const ClassType_body *const m_body;

    /*--------------------------------------------------------------------------

        Static Methods                                                        */

    public:

        static ClassType getClass(const std::string &);
		static ClassType getClass(const std::type_info &);

    /*--------------------------------------------------------------------------

        Static Data                                                           */

    public:
};



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   DataMember_body                                                            |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Class comments>

-                                                                             */

class DataMember_body {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

        DataMember_body(const std::string &name, const std::string &typ);
        virtual ~DataMember_body() {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    DataMember_body(const DataMember_body &);
	    DataMember_body &operator=(const DataMember_body &);

    /*--------------------------------------------------------------------------

        Accessors and Convertors                                              */

    public:

        const std::string& name() const { return m_name; }
        const std::string& typeName() const { return m_typeName; }

        virtual const Type &type() const = 0;

        // Because in the get case we require that the caller
        // know the type of the member, we can do a cast to the
        // right typed-member type.  In the set case though, the
        // value that is passed in may not be the actual type of
        // the member, since there may be some kind of conversion.
        // We could also require the user to know the type, and do
        // a conversion, but the conversion would have to take place
        // outside of the set() method.  This would require the use of
        // of a temporary object, which would then have to be copied to
        // the actual object in class, which might be inefficient.

        template <typename Val_NT, typename Obj_T>
        Val_NT &ref(Obj_T *op) const {
            ObjHolder_tmpl<Obj_T> oh(op);
            // I cannot use something like casting the Member to the
            // Member_body_tmpl1<Val_NT>, because this Val_NT is the actual
            // type, not a Type construct, which is what Member_body_tmpl1
            // actually uses.  I also can't have the derived type return a
            // ObjHolder, because then it would have to allocate it.  Thus
            // I pass a ObjHolder down to get a pointer to the value.
            PtrHolder_tmpl<Val_NT> vph;
            this->getPtr(oh, &vph);
            assert(vph.ptr != NULL);
            return *vph.ptr;
        }

        virtual void getPtr(const ObjHolder &oh, PtrHolder *vh) const = 0;

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:

        virtual const BoundDataMember_body *bind(const ObjHolder &oh) const = 0;

    /*--------------------------------------------------------------------------

        Data                                                                  */

    private:

        const std::string m_name;
        const std::string m_typeName;
};



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   DataMember                                                                 |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Class comments>

-                                                                             */

class DataMember {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

        explicit DataMember(const DataMember_body *b) : m_body(b) {}

	private:
	    
	    // Implicit for these should be fine.
	    // Member(const Member &);
	    // Member &operator=(const Member &);

    /*--------------------------------------------------------------------------

        Accessors and Convertors                                              */

    public:

        const std::string &name() const { return m_body->name(); }
        const std::string &typeName() const { return m_body->typeName(); }

        inline Type type() const;

        template <typename Val_T, typename Obj_T>
        Val_T &ref(Obj_T *op) const { return m_body->ref<Val_T>(op); }

    /*--------------------------------------------------------------------------

        Data                                                                  */

    public:

        const DataMember_body *m_body;
};


/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   ClassType_body Inlines                                                         |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

-                                                                             */
/*------------------------------------------------------------------------------

    Constructors, Initters, Assignments, and Destructors                      */

inline
ClassType_body::~ClassType_body() {}

/*------------------------------------------------------------------------------

    Methods                                                                   */

template <typename T>
const BoundDataMember_body *
ClassType_body::getBoundDataMember(T *o, const std::string &n) const {
    ObjHolder_tmpl<T> oh(o);
    const DataMember_body *b = getDataMember(n);
    assert(b != NULL);
    return b->bind(oh);
}

template <typename T>
const BoundMemberFunction_body *
ClassType_body::getBoundMemberFunction(T *o, const std::string &n) const {
    ObjHolder_tmpl<T> oh(o);
    const MemberFunction_body *b = getMemberFunction(n);
    assert(b != NULL);
    return b->bind(oh);
}

template <typename T>
T *
ClassType_body::newInstance() const {
    // Create a ObjHolder for the new instance to be stored into.
    ObjHolder_tmpl<T> tph;
    this->newInstance(&tph);
    return tph.ptr;
}



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   ClassType Inlines                                                              |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

-                                                                             */

/*------------------------------------------------------------------------------

    Methods                                                                   */

inline DataMember
ClassType::getDataMember(const std::string &n) const {
    return DataMember(m_body->getDataMember(n));
}

inline MemberFunction
ClassType::getMemberFunction(const std::string &n) const {
    return MemberFunction(m_body->getMemberFunction(n));
}

template <typename T>
inline BoundDataMember
ClassType::getBoundDataMember(T *o, const std::string &n) const {
    return BoundDataMember(m_body->getBoundDataMember(o, n));
}

template <typename T>
inline BoundMemberFunction 
ClassType::getBoundMemberFunction(T *o, const std::string &n) const {
    return BoundMemberFunction(m_body->getBoundMemberFunction(o, n));
}


inline bool
ClassType::derivesFrom(const ClassType &c) const {
    return m_body->derivesFrom(*c.m_body);
}

inline bool
ClassType::inheritsFrom(const ClassType &c) const {
    return derivesFrom(c);
}


/*------------------------------------------------------------------------------

    Static Objects                                                            */



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   Member_body Inlines                                                         |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Class comments>

-                                                                             */



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   Member Inlines                                                                     |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

-                                                                             */

/*------------------------------------------------------------------------------

    Accessors and Convertors                                                  */

inline Type
DataMember::type() const { return m_body->type(); }



static const char ClassType_h_id[] =
"$Id: ClassType.hpp,v 1.1 2007/10/25 14:05:23 tdevadit Exp $";



} // namespace



#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
