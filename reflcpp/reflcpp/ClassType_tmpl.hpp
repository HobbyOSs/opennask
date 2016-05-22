/******************************************************************************\
*                                                                              *
*   ClassType_tmpl.hpp                                                         *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: ClassType_tmpl.hpp,v 1.1 2007/10/25 14:05:24 tdevadit Exp $



#ifndef REFLCPP_CLASS_TYPE_TMPL_H
#define REFLCPP_CLASS_TYPE_TMPL_H



#include <reflcpp/Type.hpp>
#include <reflcpp/ClassType.hpp>
#include <reflcpp/FundamentalType.hpp>
#include <reflcpp/BoundClassType_tmpl.hpp>
#include <reflcpp/TypeInfoCmp.hpp>
#include <reflcpp/check.hpp>
#include <reflcpp/Exceptions.hpp>



namespace reflcpp {



/*------------------------------------------------------------------------------

    Forwards (within namespace)                                               */

template <typename Bottom_TP, typename Der_TP, typename Class_TP, int N>
class Bases;
template <typename Bottom_TP> class BaseList_base;
template <class Obj_TP> class ClassType_body_tmpl;
template <typename Obj_TP> class Members;
template <typename Obj_TP> class MemberFunction_body_tmpl1;



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   ObjHolder                                                                  |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    This is the base class for an object holder.

    ObjHolders are used to allow pointers to different object types to be
    passed through virtual functions.

    The reason we need both the castTo() and castFrom() is because sometimes
    we need to cast B to A, and sometimes A to B.  I have not been able to
    discover any way to refactor this to eliminate the need for both directions
    explicitly.

    Note that the information about all directions is always in the derived
    type.  A base class A does not know about the classes that are derived
    from it.

-                                                                             */

class ObjHolder {
    protected:
        ObjHolder(const std::type_info &ti) : typeInfo(ti) {}
    public:
        virtual ~ObjHolder() {}
        virtual void castTo(ObjHolder *oh) const = 0;
        virtual void castFrom(const ObjHolder &) = 0;
        const std::type_info &typeInfo;
};

/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   ObjHolder_tmpl                                                             |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    This is the templated class that contains a typed pointer.  Each
    instantiation of this class will specialize the castTo() and castFrom().

-                                                                             */

template <typename T>
class ObjHolder_tmpl : public ObjHolder {
    public:
        explicit ObjHolder_tmpl(T *p)
         : ObjHolder(typeid(T)), ptr(p) { ixcr_assert(p != NULL); }
        // XXX - For non-debug, don't set ptr to zero, for speed.
        ObjHolder_tmpl() : ObjHolder(typeid(T)), ptr(0) { }
        virtual ~ObjHolder_tmpl() {}
        virtual void castTo(ObjHolder *ph) const;
        virtual void castFrom(const ObjHolder &ph);
        T *ptr;
};

/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   DataMember_body_tmpl1                                                      |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    DataMember body class.  This one is templated just on the object type (and
    not also on the member type).

-                                                                             */

template <typename Obj_TP>
class DataMember_body_tmpl1 : public DataMember_body {

    /*--------------------------------------------------------------------------

        Types and Constants                                                   */

        typedef std::map<std::string, const DataMember_body_tmpl1<Obj_TP> *>
            dataMember_map_t;
        typedef std::map<std::string, const MemberFunction_body_tmpl1<Obj_TP> *>
            memberFunction_map_t;
        typedef std::vector<DataMember> dataMember_vector_t;
        typedef std::vector<MemberFunction> memberFunction_vector_t;

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

        inline DataMember_body_tmpl1(const std::string &name,
            const std::string &type_name);
        virtual ~DataMember_body_tmpl1() {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    DataMember_body_tmpl1(const DataMember_body_tmpl1 &);
	    DataMember_body_tmpl1 &operator=(const DataMember_body_tmpl1 &);

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:

        // This declaration is just to avoid warning messages about virtual
        // function hiding.
        virtual BoundDataMember_body *bind(const ObjHolder &oh) const = 0;
        virtual BoundDataMember_body *bind(Obj_TP *) const = 0;

        void registerMember(dataMember_map_t *fm, dataMember_vector_t *fv, 
                            memberFunction_map_t *mm, memberFunction_vector_t *mv) {
            fm->insert(typename dataMember_map_t::value_type(name(), this));
            fv->push_back(DataMember(this));
        }
};

/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   DataMember_body_tmpl2                                                      |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    DataMember body class.  This one is templated on both the object type and the
    member type.  The member type is a synthetic type class, and not a native
    type.

-                                                                             */

template <typename Obj_TP, typename MemType_TP>
class DataMember_body_tmpl2 : public DataMember_body_tmpl1<Obj_TP> {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

        DataMember_body_tmpl2(const std::string &member_name);

	private:
	    
	    // Disallow these unless specifically allowed.
	    DataMember_body_tmpl2(const DataMember_body_tmpl2 &);
	    DataMember_body_tmpl2 &operator=(const DataMember_body_tmpl2 &);

    /*--------------------------------------------------------------------------

        Accessors and Convertors                                              */

    public:

        virtual const Type &type() const { return m_type; }

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:

        // This needs to be inline to workaround a MIPSpro 7.4 bug in RTTI.
        virtual void getPtr(const ObjHolder &oh, PtrHolder *vh) const {
            // Convert the object holder.
            ObjHolder_tmpl<Obj_TP> oh_t;
            oh.castTo(&oh_t);
            assert(oh_t.ptr != NULL);
            // Cast the value holder to the correct type.
            PtrHolder_tmpl<typename MemType_TP::type> *vh_t
                = dynamic_cast<PtrHolder_tmpl<typename MemType_TP::type> *>(vh);
            assert(vh_t != NULL);
            vh_t->ptr = ptr(oh_t.ptr);
        }

        virtual typename MemType_TP::type *ptr(Obj_TP *) const = 0;

        // The first version is for creating a BoundDataMember from a DataMember.  The
        // second is for creating a BoundDataMember from a BoundClass.  These two
        // versions of bind could be converted into one, but at a slight cost
        // in performance.  The ObjHolder in the first version would have to be
        // converted to the actual object in a separate method.
        virtual BoundDataMember_body *bind(const ObjHolder &oh) const {
            // Convert the object holder.
            ObjHolder_tmpl<Obj_TP> oh_t;
            using namespace std;
            oh.castTo(&oh_t);
            assert(oh_t.ptr != NULL);
            // XXX - Memleak.
            return new BoundDataMember_body_tmpl2<Obj_TP, MemType_TP>(oh_t.ptr, this);
        }

        virtual BoundDataMember_body *bind(Obj_TP *) const;

    /*--------------------------------------------------------------------------

        Data                                                                  */

    private:

        const MemType_TP m_type;
};

/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   DataMemberFunction_body_tmpl1                                                  |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    MemberFunction body class.  This one is templated only on the object type.

-                                                                             */

template <typename Obj_TP>
class MemberFunction_body_tmpl1 : public MemberFunction_body {

    /*--------------------------------------------------------------------------

        Types and Constants                                                   */

        typedef std::map<std::string, const DataMember_body_tmpl1<Obj_TP> *>
            dataMember_map_t;
        typedef std::map<std::string, const MemberFunction_body_tmpl1<Obj_TP> *>
            memberFunction_map_t;
        typedef std::vector<DataMember> dataMember_vector_t;
        typedef std::vector<MemberFunction> memberFunction_vector_t;

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

        inline MemberFunction_body_tmpl1(const std::string &name);
        virtual ~MemberFunction_body_tmpl1() {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    MemberFunction_body_tmpl1(const MemberFunction_body_tmpl1 &);
	    MemberFunction_body_tmpl1 &operator=(const MemberFunction_body_tmpl1 &);

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:

        // Extract the object pointer. This is done here to minimize what has 
        // to be done in the duplicated code in sub classes.
        // This method is overloaded for ObjHolder and PtrHolder.
        virtual void invoke1(const ObjHolder &oh, RetValHolder &rvh,
                             const int n, Arguments &args) const {
            ObjHolder_tmpl<Obj_TP> oh_t;
            oh.castTo(&oh_t);
            assert(oh_t.ptr != NULL);
            invoke2(oh_t.ptr, rvh, n, args);
        }

        virtual void invoke1(const PtrHolder &ptrHolder, RetValHolder &rvh,
                             const int n, Arguments &args) const {
            const SmartPtrHolder_tmpl<Obj_TP> &smartPtrHolder = 
                dynamic_cast<const SmartPtrHolder_tmpl<Obj_TP> &>(ptrHolder);
            Obj_TP &obj = *(smartPtrHolder.ptr);
            invoke2(&obj, rvh, n, args);
        }        

        virtual void invoke2(Obj_TP *op, RetValHolder &rvh,
                             const int n, Arguments &args) const = 0;

        // This declaration is just to avoid warning messages about virtual
        // function hiding.
        virtual BoundMemberFunction_body *bind(const ObjHolder &oh) const = 0;
        virtual BoundMemberFunction_body *bind(Obj_TP *) const = 0;

        void registerMember(dataMember_map_t *fm, dataMember_vector_t *fv, 
                            memberFunction_map_t *mm, memberFunction_vector_t *mv) {
            mm->insert(typename memberFunction_map_t::value_type(name(), this));
            mv->push_back(MemberFunction(this));
        }
};

template <typename Obj_TP>
inline
MemberFunction_body_tmpl1<Obj_TP>::MemberFunction_body_tmpl1(const std::string &name)
    : MemberFunction_body(name) {}


/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   MemberFunction_body_tmpl2                                                  |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    MemberFunction body class.  This one is templated on both the object type and the
    return type.
-                                                                             */

template <typename Obj_TP, typename Ret_TP>
class MemberFunction_body_tmpl2 : public MemberFunction_body_tmpl1<Obj_TP> {

    /*--------------------------------------------------------------------------

        Types and Constants                                                   */

        typedef std::vector<const Type *> argument_vector_t;

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

        MemberFunction_body_tmpl2(const std::string &member_name);

	private:
	    
	    // Disallow these unless specifically allowed.
	    MemberFunction_body_tmpl2(const MemberFunction_body_tmpl2 &);
	    MemberFunction_body_tmpl2 &operator=(const MemberFunction_body_tmpl2 &);

    /*--------------------------------------------------------------------------

        Accessors and Convertors                                              */

    public:

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:

        virtual const argument_vector_t &getArguments() const { return m_argument_vector; }

        virtual void invoke2(Obj_TP *op, RetValHolder &rvh,
                             const int n, Arguments &args) const = 0;
        virtual typename Ret_TP::type invoke3(Obj_TP *op, const int n, Arguments &args) const = 0;

        // The first version is for creating a BoundMemberFunction from a 
        // MemberFunction.  The second is for creating a BoundMemberFunction 
        // from a BoundClass.
        virtual BoundMemberFunction_body *bind(const ObjHolder &oh) const {
            // Convert the object holder.
            ObjHolder_tmpl<Obj_TP> oh_t;
            using namespace std;
            oh.castTo(&oh_t);
            assert(oh_t.ptr != NULL);
            // XXX - Memleak.
            return new BoundMemberFunction_body_tmpl2<Obj_TP, Ret_TP>(oh_t.ptr, this);
        }

        virtual BoundMemberFunction_body *bind(Obj_TP *o) const {
            return new BoundMemberFunction_body_tmpl2<Obj_TP, Ret_TP>(o, this);
        }

    protected:
        void registerArgument(const Type *type) {
            m_argument_vector.push_back(type);
        }

    /*--------------------------------------------------------------------------

        Data                                                                  */

    private:

        argument_vector_t m_argument_vector; // Map for argument list
};


template <typename Obj_TP, typename Ret_TP>
MemberFunction_body_tmpl2<Obj_TP, Ret_TP>::MemberFunction_body_tmpl2(const std::string &member_name)
 : MemberFunction_body_tmpl1<Obj_TP>(member_name) {}


/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   MemberFunction_body_tmpl3                                                  |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    Handle return type issues.

-                                                                             */

template <typename Obj_TP, typename Ret_TP>
class MemberFunction_body_tmpl3 : public MemberFunction_body_tmpl2<Obj_TP, Ret_TP> {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

        MemberFunction_body_tmpl3(const std::string &member_name)
            : MemberFunction_body_tmpl2<Obj_TP, Ret_TP>(member_name) {}
	
    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:

        virtual void invoke2(Obj_TP *op, RetValHolder &rvh,
                             const int n, Arguments &args) const {
            // Downcast the retval holder.
            RetValHolder_tmpl<typename Ret_TP::type> *rvh_t
                = dynamic_cast<RetValHolder_tmpl<typename Ret_TP::type> *>(&rvh);
            assert(rvh_t != NULL);
            rvh_t->ref() = invoke3(op, n, args);
        }
};
template <typename Obj_TP>
class MemberFunction_body_tmpl3<Obj_TP, FundamentalType_tmpl<void> > 
    : public MemberFunction_body_tmpl2<Obj_TP, FundamentalType_tmpl<void> > {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

        MemberFunction_body_tmpl3(const std::string &member_name)
            : MemberFunction_body_tmpl2<Obj_TP, FundamentalType_tmpl<void> >(member_name) {}
	
    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:

        virtual void invoke2(Obj_TP *op, RetValHolder &rvh,
                             const int n, Arguments &args) const {
            invoke3(op, n, args);
        }
};


/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   MemberList_base                                                            |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Type comments>

-                                                                             */

template <typename Obj_TP>
class MemberList_base : public virtual ClassType_body {

    /*--------------------------------------------------------------------------

        Types and Constants                                                   */

    public:

        typedef std::map<std::string, const DataMember_body_tmpl1<Obj_TP> *>
            dataMember_map_t;
        typedef std::map<std::string, const MemberFunction_body_tmpl1<Obj_TP> *>
            memberFunction_map_t;

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    protected:

        MemberList_base() {}
        virtual ~MemberList_base() {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    MemberList_base(const MemberList_base &);
	    MemberList_base &operator=(const MemberList_base &); 

    /*--------------------------------------------------------------------------

        Data                                                                  */
        
    protected:

        // Map of string names to DataMember_body objects.
        dataMember_map_t m_dataMember_map;
        // Map of string names to MemberFunction_body objects.
        memberFunction_map_t m_memberFunction_map;
};

/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   MemberList_name_base                                                       |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Type comments>

-                                                                             */

template <typename Obj_TP, const char *Name_TP>
class MemberList_name_base : public MemberList_base<Obj_TP> {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    protected:

        MemberList_name_base() {}
        virtual ~MemberList_name_base() {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    MemberList_name_base(const MemberList_name_base &);
	    MemberList_name_base &operator=(const MemberList_name_base &); 

    /*--------------------------------------------------------------------------

        Static Data                                                           */

    public:

        // "tp" means stands for "template parameter".
        static const char *const tp_name;
};

/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   DataMemberDcl                                                              |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Type comments>

-                                                                             */

template <typename Obj_TP, typename MemType_TP,
 typename MemType_TP::type (Obj_TP::*MemP), const char *Name>
class DataMemberDcl : public DataMember_body_tmpl2<Obj_TP, MemType_TP> {

    public:

        DataMemberDcl() : DataMember_body_tmpl2<Obj_TP, MemType_TP>(Name) {}

        virtual typename MemType_TP::type *ptr(Obj_TP *op) const {
            return &(op->*MemP);
        }
};


/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   VoidMember                                                                 |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    Base case of recursion.

-                                                                             */

template <typename Obj_TP>
struct VoidMember {
    VoidMember() {}
    void registerMember(typename MemberList_base<Obj_TP>::dataMember_map_t *,
        ClassType_body::dataMember_vector_t *,
        typename MemberList_base<Obj_TP>::memberFunction_map_t *,
        ClassType_body::memberFunction_vector_t *) { }
};

/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   MemberList                                                                 |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Type comments>

-                                                                             */

template <typename Obj_TP, const char *Name_TP
 , typename T001 = VoidMember<Obj_TP>
 , typename T002 = VoidMember<Obj_TP>
 , typename T003 = VoidMember<Obj_TP>
 , typename T004 = VoidMember<Obj_TP>
 , typename T005 = VoidMember<Obj_TP>
 , typename T006 = VoidMember<Obj_TP>
 , typename T007 = VoidMember<Obj_TP>
 , typename T008 = VoidMember<Obj_TP>
 , typename T009 = VoidMember<Obj_TP>
 , typename T010 = VoidMember<Obj_TP>
>
class MemberList : public MemberList_name_base<Obj_TP, Name_TP> {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    protected:

        MemberList() :
            member001()
            , member002()
            , member003()
            , member004()
            , member005()
            , member006()
            , member007()
            , member008()
            , member009()
            , member010()
        {
            // XXX - It's probably possible to just have each member insert
            // itself into the maps, then construct the vectors by
            // traversing the maps after everyone is inserted.
            member001.registerMember(&this->m_dataMember_map, &this->m_dataMember_vector, &this->m_memberFunction_map, &this->m_memberFunction_vector);
            member002.registerMember(&this->m_dataMember_map, &this->m_dataMember_vector, &this->m_memberFunction_map, &this->m_memberFunction_vector);
            member003.registerMember(&this->m_dataMember_map, &this->m_dataMember_vector, &this->m_memberFunction_map, &this->m_memberFunction_vector);
            member004.registerMember(&this->m_dataMember_map, &this->m_dataMember_vector, &this->m_memberFunction_map, &this->m_memberFunction_vector);
            member005.registerMember(&this->m_dataMember_map, &this->m_dataMember_vector, &this->m_memberFunction_map, &this->m_memberFunction_vector);
            member006.registerMember(&this->m_dataMember_map, &this->m_dataMember_vector, &this->m_memberFunction_map, &this->m_memberFunction_vector);
            member007.registerMember(&this->m_dataMember_map, &this->m_dataMember_vector, &this->m_memberFunction_map, &this->m_memberFunction_vector);
            member008.registerMember(&this->m_dataMember_map, &this->m_dataMember_vector, &this->m_memberFunction_map, &this->m_memberFunction_vector);
            member009.registerMember(&this->m_dataMember_map, &this->m_dataMember_vector, &this->m_memberFunction_map, &this->m_memberFunction_vector);
            member010.registerMember(&this->m_dataMember_map, &this->m_dataMember_vector, &this->m_memberFunction_map, &this->m_memberFunction_vector);
        }

	private:
	    
	    // Disallow these unless specifically allowed.
	    MemberList(const MemberList &);
	    MemberList &operator=(const MemberList &); 

    /*--------------------------------------------------------------------------

        Data                                                                  */

        
    private:

        T001 member001;
        T002 member002;
        T003 member003;
        T004 member004;
        T005 member005;
        T006 member006;
        T007 member007;
        T008 member008;
        T009 member009;
        T010 member010;
};

/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   BaseInfo_base                                                              |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    Used to store information about a base class.   Templated just on the
    bottom most class.

-                                                                             */

template <typename Bottom_TP>
class BaseInfo_base {
    public:
        virtual Bottom_TP *downCast(const ObjHolder &) const = 0;
        virtual void upCast(Bottom_TP *, ObjHolder *) const = 0;
};

/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   BaseInfo                                                                   |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    Used to store information about a base class.  Contains functions for
    casting.

-                                                                             */

template <typename Bottom_TP, typename Base_TP>
class BaseInfo : public BaseInfo_base<Bottom_TP> {
        typedef typename BaseList_base<Bottom_TP>::map_t map_t;
    public:
        // Constructor inserts this BaseInfo object into a map for finding
        // BaseInfo objects to use for conversion.  The map is on the base
        // type.  There is one map per bottom class.
        BaseInfo(map_t *map) {
            map->insert(typename map_t::value_type(&typeid(Base_TP), this));
        }
        virtual Bottom_TP *downCast(const ObjHolder &ph) const {
            IXCR_DCDCL(const ObjHolder_tmpl<Base_TP> &, ph_b, ph);
            return dynamic_cast<Bottom_TP *>(ph_b.ptr);
        }
        virtual void upCast(Bottom_TP *p, ObjHolder *ph) const {
            IXCR_DCDCL(ObjHolder_tmpl<Base_TP> *, ph_b, ph);
            ph_b->ptr = p;
        }
};

/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   VoidBase                                                                   |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    Tag structure.  This is used for some partial template specialization at
    the base of the recursion.

-                                                                             */

struct VoidBase {};

/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   BaseInfo<Bottom_TP, VoidBase>                                              |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    Partial specialization for no base present.  This makes the BaseList class
    above work correctly with the default template arguments.

-                                                                             */

template <typename Bottom_TP>
class BaseInfo<Bottom_TP, VoidBase> : public BaseInfo_base<Bottom_TP> {
    public:
        BaseInfo(typename BaseList_base<Bottom_TP>::map_t *) {}
        // These should never be called, because this BaseInfo should never
        // appear in any map.
        virtual Bottom_TP *downCast(const ObjHolder &ph) const {
            ixcr_check(false); return 0; }
        virtual void upCast(Bottom_TP *p, ObjHolder *ph) const {
            ixcr_check(false); }
};

/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   Bases<Bot_TP, Der_TP, VoidBase, N>                                         |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    Partial specialization for base case of recursion.

-                                                                             */

template <typename Bot_TP, typename Der_TP, int N>
class Bases<Bot_TP, Der_TP, VoidBase, N> { };

/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   BaseList_base                                                              |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    Holds the map from type_info * to BaseInfo objects.

-                                                                             */

template <typename Bottom_TP>
class BaseList_base : public virtual ClassType_body {
    public:
        typedef std::map<const std::type_info *, BaseInfo_base<Bottom_TP> *,
         TypeInfoCmp> map_t;
        map_t m_base_info_map;
};

/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   BaseList                                                                   |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    Used to split the list of bases out into separate Bases classes.

-                                                                             */

template <typename Bottom_TP, typename Der_TP, typename B01_TP = VoidBase,
 typename B02_TP = VoidBase >
class BaseList : public Bases<Bottom_TP, Der_TP, B01_TP, 1>,
                 public Bases<Bottom_TP, Der_TP, B02_TP, 2>,
                 public virtual BaseList_base<Bottom_TP> {

    public:

        BaseList()
         : caster01(&this->m_base_info_map)
         , caster02(&this->m_base_info_map)
         { }

    protected:

        BaseInfo<Bottom_TP, B01_TP> caster01;
        BaseInfo<Bottom_TP, B02_TP> caster02;
};


/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   ClassType_body_tmpl                                                        |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Type comments>

-                                                                             */

template <typename Obj_TP>
class ClassType_body_tmpl
 : public Members<Obj_TP>, public Bases<Obj_TP, Obj_TP, Obj_TP, 0>,
   public virtual ClassType_body {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

        // The way tp_name is being used here is a bit convoluted.  The
        // name of the class should be quickly accessible to
        // ClassType_body, so that no virtual function calls are needed to
        // access it.  The name is a const char * template parameter of
        // Members<Obj_TP>.  So this is made available through the tp_name
        // static data member.  Since no constructors are involved, the
        // data member is valid without worrying about initialization
        // order.
        ClassType_body_tmpl()
            : ClassType_body(this->tp_name, typeid(Obj_TP)) {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    ClassType_body_tmpl(const ClassType_body_tmpl &);
	    ClassType_body_tmpl &operator=(const ClassType_body_tmpl &); 

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:

        virtual const DataMember_body_tmpl1<Obj_TP> *
        getDataMember(const std::string &n) const;

        virtual const MemberFunction_body_tmpl1<Obj_TP> *
        getMemberFunction(const std::string &n) const;

        virtual void newInstance(ObjHolder *) const;
        virtual PtrHolder_smptr_t createInstance() const;
        virtual PtrHolder *createInstancePtr() const;

        virtual bool derivesFrom(const ClassType_body &) const;
};


/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   ClassType_tmpl                                                             |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Type comments>

-                                                                             */

template <typename Obj_TP>
class ClassType_tmpl : public ClassType {

    /*--------------------------------------------------------------------------

        Types and Constants                                                   */

    public:

        typedef Obj_TP type;

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

        // Construct the base object with the pointer to the static
        // Type body.
        ClassType_tmpl()
            : ClassType(s_class_body == NULL
                ? (s_class_body = new ClassType_body_tmpl<typename TypeOp<Obj_TP>::BareT>())
                : s_class_body) {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    ClassType_tmpl(const ClassType_tmpl &);
	    ClassType_tmpl &operator=(const ClassType_tmpl &);

    /*--------------------------------------------------------------------------

        Static Data                                                           */

    public:

        static ClassType_body_tmpl<typename TypeOp<Obj_TP>::BareT> *s_class_body;
};


/******************************************************************************
**************************** Template Definitions *****************************
******************************************************************************/



/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
|                                                                              |
|   ObjHolder_tmpl<T>                                                          |
|                                                                              |
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
\- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

template <typename T>
void
ObjHolder_tmpl<T>::castTo(ObjHolder *oh) const {

    assert(oh != NULL);
    assert(ptr != NULL);

    typedef typename BaseList_base<T>::map_t map_t;

    if (typeid(*this) == typeid(*oh)) {

        IXCR_DCDCL(ObjHolder_tmpl<T> *, oh_t, oh); 
        oh_t->ptr = ptr;

    } else {

        map_t &map(ClassType_tmpl<T>::s_class_body->m_base_info_map);

        // g++ 3.2.2 requires typename here.
        typename map_t::const_iterator it = map.find(&oh->typeInfo);
        if (it != map.end()) {
            const BaseInfo_base<T> *c = (*it).second;
            c->upCast(ptr, oh);
        } else {
            oh->castFrom(*this);
        }
    }
}

template <typename T>
void
ObjHolder_tmpl<T>::castFrom(const ObjHolder &oh) {

    typedef typename BaseList_base<T>::map_t map_t;

    if (typeid(*this) == typeid(oh)) {

        IXCR_DCDCL(const ObjHolder_tmpl<T> &, oh_t, oh); 
        ptr = oh_t.ptr;

    } else {

        map_t &map(ClassType_tmpl<T>::s_class_body->m_base_info_map);

        // g++ 3.2.2 requires typename here.
        typename map_t::const_iterator it = map.find(&oh.typeInfo);
        if (it != map.end()) {
            const BaseInfo_base<T> *c = (*it).second;
            ptr = c->downCast(oh);
        }
    }
}


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
|                                                                              |
|   DataMember_body_tmpl1                                                          |
|                                                                              |
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
\- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*------------------------------------------------------------------------------

    Constructors, Initters, Assignments, and Destructors                      */

template <typename Obj_TP>
inline
DataMember_body_tmpl1<Obj_TP>::DataMember_body_tmpl1(const std::string &name,
 const std::string &type_name)
 : DataMember_body(name, type_name) {
}


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
|                                                                              |
|   DataMember_body_tmpl2                                                          |
|                                                                              |
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
\- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*------------------------------------------------------------------------------

    Constructors, Initters, Assignments, and Destructors                      */

template <typename Obj_TP, typename Val_T>
DataMember_body_tmpl2<Obj_TP, Val_T>::DataMember_body_tmpl2(const std::string &member_name)
 : DataMember_body_tmpl1<Obj_TP>(member_name, Val_T().name()) {}

/*------------------------------------------------------------------------------

    Methods                                                                   */

template <typename Obj_TP, typename Val_T>
BoundDataMember_body *
DataMember_body_tmpl2<Obj_TP, Val_T>::bind(Obj_TP *o) const {
    return new BoundDataMember_body_tmpl2<Obj_TP, Val_T>(o, this);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
|                                                                              |
|   MemberList_name_base                                                       |
|                                                                              |
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
\- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

template <typename Obj_TP, const char *Name_TP>
const char *const 
MemberList_name_base<Obj_TP, Name_TP>::tp_name = Name_TP;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
|                                                                              |
|   ClassType_body_tmpl                                                        |
|                                                                              |
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
\- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

template <typename Obj_TP>
const DataMember_body_tmpl1<Obj_TP> *
ClassType_body_tmpl<Obj_TP>::getDataMember(const std::string &n) const {
    typename Members<Obj_TP>::dataMember_map_t::const_iterator it = this->m_dataMember_map.find(n);
    if (it == this->m_dataMember_map.end()) {
        Throw(NoSuchDataMemberException(this->tp_name, n));
    }
    //assert(it != m_dataMember_map.end());
    return (*it).second;
}

template <typename Obj_TP>
const MemberFunction_body_tmpl1<Obj_TP> *
ClassType_body_tmpl<Obj_TP>::getMemberFunction(const std::string &n) const {
    typename Members<Obj_TP>::memberFunction_map_t::const_iterator it = this->m_memberFunction_map.find(n);
    if (it == this->m_memberFunction_map.end()) {
        Throw(NoSuchMemberFunctionException(this->tp_name, n));
    }
    //assert(it != m_memberFunction_map.end());
    return (*it).second;
}

template <typename Obj_TP>
void
ClassType_body_tmpl<Obj_TP>::newInstance(ObjHolder *ph) const {
    // Create a obj holder of the actual derived type.
    const ObjHolder_tmpl<Obj_TP> tph(new Obj_TP());
    // Cast the actual derived type to the desired ObjHolder type.
    tph.castTo(ph);
}

/**
 * Return a SmartPtrHolder of the actual type. The SmartPtrHolder is also wrapped
 * with a PtrHolder_smptr_t in order to ensure automatic destruction.
 */
template <typename Obj_TP>
PtrHolder_smptr_t 
ClassType_body_tmpl<Obj_TP>::createInstance() const {
    return PtrHolder_smptr_t(new SmartPtrHolder_tmpl<Obj_TP>(new Obj_TP));
}

/**
 * Return a SmartPtrHolder of the actual type.
 */
template <typename Obj_TP>
PtrHolder *
ClassType_body_tmpl<Obj_TP>::createInstancePtr() const {
    return new SmartPtrHolder_tmpl<Obj_TP>(new Obj_TP);
}

template <typename Obj_TP>
bool
ClassType_body_tmpl<Obj_TP>::derivesFrom(const ClassType_body &ct) const {

    if (typeid(Obj_TP) == ct.m_type_info) {
        return true;
    } else {
        typedef typename BaseList_base<Obj_TP>::map_t map_t;
        typename map_t::const_iterator it = this->m_base_info_map.find(&ct.m_type_info);
        return it != this->m_base_info_map.end();
    }

    return false;
}



/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
|                                                                              |
|   ClassType_tmpl                                                             |
|                                                                              |
|- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
\- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*------------------------------------------------------------------------------

    Static Data                                                               */

template <typename Obj_TP>
ClassType_body_tmpl<typename TypeOp<Obj_TP>::BareT> *
ClassType_tmpl<Obj_TP>::s_class_body;



static const char ClassType_tmpl_hpp_id[] =
"$Id: ClassType_tmpl.hpp,v 1.1 2007/10/25 14:05:24 tdevadit Exp $";



} // namespace



#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
