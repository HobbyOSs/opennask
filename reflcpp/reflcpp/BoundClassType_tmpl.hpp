/******************************************************************************\
*                                                                              *
*   BoundClassType_tmpl.h                                                                        *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: BoundClassType_tmpl.hpp,v 1.1 2007/10/25 14:05:23 tdevadit Exp $



#ifndef REFLCPP_BOUND_CLASS_TYPE_TMPL_HPP
#define REFLCPP_BOUND_CLASS_TYPE_TMPL_HPP



#include <assert.h>
#include <reflcpp/BoundClassType.hpp>
#include <reflcpp/macros.hpp>



/*------------------------------------------------------------------------------

    Forwards (global)                                                         */



namespace reflcpp {



/*------------------------------------------------------------------------------

    Forwards (within namespace)                                               */


class BoundDataMember_body;
template <typename Obj_T> class ClassType_body_tmpl;
template <typename Obj_T> class ClassType_tmpl;
class DataMember_body;
template <typename Obj_T> class DataMember_body_tmpl1;
template <typename Obj_T, typename Val_T> class DataMember_body_tmpl2;
template <typename Obj_T, typename Ret_T> class MemberFunction_body_tmpl2;

class PtrHolder;
template <typename T> class PtrHolder_tmpl;



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   BoundDataMember_body_tmpl1                                                 |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Class comments>

-                                                                             */


template <typename Obj_T>
class BoundDataMember_body_tmpl1 : public BoundDataMember_body {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:
        BoundDataMember_body_tmpl1(Obj_T *o) : m_obj(o) {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    BoundDataMember_body_tmpl1(const BoundDataMember_body_tmpl1 &);
	    BoundDataMember_body_tmpl1 &operator=(const BoundDataMember_body_tmpl1 &);

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:

        virtual void getPtr(PtrHolder *vph) const = 0;

    /*--------------------------------------------------------------------------

        Data                                                                  */

    protected:

        Obj_T *const m_obj;
};

/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   BoundDataMember_body_tmpl2                                                 |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Class comments>

-                                                                             */

template <typename Obj_T, typename Val_T>
class BoundDataMember_body_tmpl2 : public BoundDataMember_body_tmpl1<Obj_T> {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

        BoundDataMember_body_tmpl2(Obj_T *o,
            const DataMember_body_tmpl2<Obj_T, Val_T> *f)
            : BoundDataMember_body_tmpl1<Obj_T>(o), m_field(f) {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    BoundDataMember_body_tmpl2(const BoundDataMember_body_tmpl2 &);
	    BoundDataMember_body_tmpl2 &operator=(const BoundDataMember_body_tmpl2 &);

    /*--------------------------------------------------------------------------

        Methods                                                               */

        virtual void getPtr(PtrHolder *vph) const;

    /*--------------------------------------------------------------------------

        Data                                                                  */

    private:
        // DataMember bodies are static, so this should be okay.
        const DataMember_body_tmpl2<Obj_T, Val_T> *const m_field;
};

template <typename Obj_T, typename Val_T>
void
BoundDataMember_body_tmpl2<Obj_T, Val_T>::getPtr(PtrHolder *vph) const {
    
    IXCR_DCDCL(PtrHolder_tmpl<typename Val_T::type> *, vph_t, vph);
    assert(vph_t != NULL);
    vph_t->ptr = m_field->ptr(this->m_obj);
}


/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   BoundMemberFunction_body_tmpl1                                             |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Class comments>

-                                                                             */


template <typename Obj_T>
class BoundMemberFunction_body_tmpl1 : public BoundMemberFunction_body {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

        BoundMemberFunction_body_tmpl1(Obj_T *o) : m_obj(o) {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    BoundMemberFunction_body_tmpl1(const BoundMemberFunction_body_tmpl1 &);
	    BoundMemberFunction_body_tmpl1 &operator=(const BoundMemberFunction_body_tmpl1 &);

    /*--------------------------------------------------------------------------

        Data                                                                  */

    protected:

        Obj_T *const m_obj;
};

/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   BoundMemberFunction_body_tmpl2                                             |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Class comments>

-                                                                             */

template <typename Obj_T, typename Ret_T>
class BoundMemberFunction_body_tmpl2 : public BoundMemberFunction_body_tmpl1<Obj_T> {

    typedef std::vector<PtrHolder *> arg_vector_t;

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

        BoundMemberFunction_body_tmpl2(Obj_T *o,
            const MemberFunction_body_tmpl2<Obj_T, Ret_T> *f)
            : BoundMemberFunction_body_tmpl1<Obj_T>(o), m_function(f) {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    BoundMemberFunction_body_tmpl2(const BoundMemberFunction_body_tmpl2 &);
	    BoundMemberFunction_body_tmpl2 &operator=(const BoundMemberFunction_body_tmpl2 &);

    /*--------------------------------------------------------------------------

        Methods                                                               */

        virtual const std::vector<const Type *> &getArguments() const { return m_function->getArguments(); }

        virtual void invoke1(RetValHolder &rvh, const int n, Arguments &args) const;

    /*--------------------------------------------------------------------------

        Data                                                                  */

    private:
        // MemberFunction bodies are static, so this should be okay.
        const MemberFunction_body_tmpl2<Obj_T, Ret_T> *const m_function;
};

template <typename Obj_T, typename Ret_T>
void 
BoundMemberFunction_body_tmpl2<Obj_T, Ret_T>::invoke1(RetValHolder &rvh, const int n, Arguments &args) const {
    m_function->invoke2(this->m_obj, rvh, n, args);
}


/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   BoundClassType_body                                                        |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Class comments>

-                                                                             */

/*------------------------------------------------------------------------------

    Forwards (only needed by this class)                                      */

/*------------------------------------------------------------------------------

    Inline Helper Functions and Helper Classes                                */

template <typename Obj_T>
class BoundClassType_body_tmpl {

    /*--------------------------------------------------------------------------

        Forwards                                                              */

    /*--------------------------------------------------------------------------

        Types and Constants                                                   */

	public:

    /*--------------------------------------------------------------------------

        Friends                                                               */

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

        BoundClassType_body_tmpl(Obj_T *o);

	private:
	    
	    // Disallow these unless specifically allowed.
	    BoundClassType_body_tmpl(const BoundClassType_body_tmpl &);
	    BoundClassType_body_tmpl &operator=(const BoundClassType_body_tmpl &);

    /*--------------------------------------------------------------------------

        Accessors and Convertors                                              */

    /*--------------------------------------------------------------------------

        Operators                                                             */

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:

        // This method cannot return a more "typed" BoundDataMember object,
        // because the BoundDataMember_body group of classes does not have
        // one that is templated only on object type.  It's templated
        // on both object type and value type.
        virtual const BoundDataMember_body *getBoundDataMember(const std::string &fn) const;

    /*--------------------------------------------------------------------------

        Data                                                                  */

    private:

        Obj_T *const m_obj;
        // Should be static object, so keeping a pointer to it should be
        // safe.
        const ClassType_body_tmpl<Obj_T> *const m_class;

    /*--------------------------------------------------------------------------

        Static Methods                                                        */

    /*--------------------------------------------------------------------------

        Static Data                                                           */

    /*--------------------------------------------------------------------------

        Access Specifiers                                                     */
};

/*------------------------------------------------------------------------------

    Constructors, Initters, Assignments, and Destructors                      */

template <typename Obj_T>
BoundClassType_body_tmpl<Obj_T>::BoundClassType_body_tmpl(Obj_T *o)
 : m_obj(o), m_class(ClassType_tmpl<Obj_T>::s_class_body) {
}

/*------------------------------------------------------------------------------

    Accessors and Convertors                                                  */

/*------------------------------------------------------------------------------

    Operators                                                                 */

/*------------------------------------------------------------------------------

    Methods                                                                   */

template <typename Obj_T>
const BoundDataMember_body *
BoundClassType_body_tmpl<Obj_T>::getBoundDataMember(const std::string &fn) const {
    const DataMember_body_tmpl1<Obj_T> *f = m_class->getDataMember(fn);
    assert(f != NULL);
    const BoundDataMember_body *bf = f->bind(m_obj);
    assert(bf != NULL);
    return bf;
}

/*------------------------------------------------------------------------------

    Static Methods and Helper Functions                                       */

/*------------------------------------------------------------------------------

    Static Data Specializations                                               */



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   BoundClassType                                                             |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Class comments>

-                                                                             */

/*------------------------------------------------------------------------------

    Forwards (only needed by this class)                                      */

/*------------------------------------------------------------------------------

    Inline Helper Functions and Helper Classes                                */

template <typename Obj_T>
class BoundClassType_tmpl {

    /*--------------------------------------------------------------------------

        Forwards                                                              */

    /*--------------------------------------------------------------------------

        Types and Constants                                                   */

	public:

    /*--------------------------------------------------------------------------

        Friends                                                               */

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:
        
        explicit BoundClassType_tmpl(Obj_T *);
        // XXX - Memleak.
        ~BoundClassType_tmpl() {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    BoundClassType_tmpl(const BoundClassType_tmpl &);
	    BoundClassType_tmpl &operator=(const BoundClassType_tmpl &);

    /*--------------------------------------------------------------------------

        Accessors and Convertors                                              */

    /*--------------------------------------------------------------------------

        Operators                                                             */

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:

        BoundDataMember getBoundDataMember(const std::string &n) const { 
            return BoundDataMember(m_body->getBoundDataMember(n)); 
        }

    /*--------------------------------------------------------------------------

        Data                                                                  */

    private:

        BoundClassType_body_tmpl<Obj_T> *m_body;

    /*--------------------------------------------------------------------------

        Static Methods                                                        */

    /*--------------------------------------------------------------------------

        Static Data                                                           */

    /*--------------------------------------------------------------------------

        Access Specifiers                                                     */
};


/*------------------------------------------------------------------------------

    Constructors, Initters, Assignments, and Destructors                      */

template <typename Obj_T>
BoundClassType_tmpl<Obj_T>::BoundClassType_tmpl(Obj_T *o)
 : m_body(new BoundClassType_body_tmpl<Obj_T>(o)) {
}

/*------------------------------------------------------------------------------

    Accessors and Convertors                                                  */

/*------------------------------------------------------------------------------

    Operators                                                                 */

/*------------------------------------------------------------------------------

    Methods                                                                   */

/*------------------------------------------------------------------------------

    Static Methods and Helper Functions                                       */

/*------------------------------------------------------------------------------

    Static Data Specializations                                               */

/*------------------------------------------------------------------------------

    Static Assertions                                                         */

// Need to have a separate namespace for each header file.  Since the static
// assertions will usually be grouped by class, might as one well use a
// separate namespace per class.
namespace BoundClassType_sa {
}



// The ^ needs to be replaced with the filename extension.
static const char BoundClassType_tmpl_h_id[] =
"$Id: BoundClassType_tmpl.hpp,v 1.1 2007/10/25 14:05:23 tdevadit Exp $";



} // namespace



#ifdef MSL_INCLUDE_TEMPLATE
    //#include "BoundClassType.cc"
#endif



#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4: */
