/******************************************************************************\
*                                                                              *
*   @.h                                                                        *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: Type.hpp,v 1.1 2007/10/25 14:05:26 tdevadit Exp $



#ifndef REFLCPP_TYPE_H
#define REFLCPP_TYPE_H


#include <assert.h>
#include <functional>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>
#include <reflcpp/platform.hpp>
#include <reflcpp/TypeInfoCmp.hpp>



namespace reflcpp {



class Type;
class Type_body;

class ConstValHolder;

template <typename T> class TypedConstValHolder;



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   Type_body                                                                 |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    This is body class for Type objects.

-                                                                             */

class Type_body {

    /*--------------------------------------------------------------------------

        Types and Constants                                                   */

    protected:

        typedef std::map<std::string, const Type_body *> class_name_map_t;
		typedef std::map<const std::type_info *, const Type_body *, TypeInfoCmp>
            class_info_map_t;

    /*--------------------------------------------------------------------------

        Friends                                                               */

        friend bool operator==(const Type_body &, const Type_body &);
        friend bool operator==(const Type_body &, const std::type_info &);
        // So it can access the various data members for accessors.
        friend class Type;

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

        Type_body(const std::string &n, const std::type_info &);
        virtual ~Type_body() {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    Type_body(const Type_body &);
	    Type_body &operator=(const Type_body &);

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:

    protected:

    /*--------------------------------------------------------------------------

        Data                                                                  */
        
    protected:

        const std::string m_name;

    public:

        // Type of actual class.
        // Should probably make this protected, and provide an accessor.
        const std::type_info &m_type_info;

    /*--------------------------------------------------------------------------

        Static Methods                                                        */

    protected:

        static const Type_body *getType(const std::string &);
		static const Type_body *getType(const std::type_info &);

    /*--------------------------------------------------------------------------

        Static Data                                                           */

    protected:

        // Maps class names of the actual class to Type_body objects.
        static class_name_map_t *s_class_name_map;
		// Maps type_info objects of the actual class to
        // Type_body objects.
        static class_info_map_t *s_class_info_map;
};

/*------------------------------------------------------------------------------

    Operators                                                                 */

// VC++ 7.0 seems to think that an expression like this:
//     return c1.m_type_info == c2.m_type_info;
// involves a cast of int to bool.  I tried several work-arounds, but
// none seemed to work, so I just decided to disable the warning. -kec
#ifdef IXCR_VCPP
    #pragma warning (disable: 4800)
#endif
inline bool
operator==(const Type_body &c1, const Type_body &c2) {
    return c1.m_type_info == c2.m_type_info;
}

inline bool
operator==(const Type_body &c, const std::type_info &ti) {
    return c.m_type_info == ti;
}
#ifdef IXCR_VCPP
    // Turn warning back on.
    #pragma warning (default: 4800)
#endif



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   Type                                                                      |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Type comments>

-                                                                             */

class Type {

    /*--------------------------------------------------------------------------

        Types and Constants                                                   */

    /*--------------------------------------------------------------------------

        Friends                                                               */

        friend bool operator==(const Type &, const Type &);
        friend bool operator==(const Type &, const std::type_info &);
        friend bool operator==(const std::type_info &, const Type &);
        template <typename T1, typename T2>
        friend T1 dynamic_cast_type(const T2 &t2);

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    protected:

        Type(const Type_body *i) : m_body(i) {}

	    // Default versions of these should be fine.
	    // Type(const Type &);
	    // Type &operator=(const Type &); 

    /*--------------------------------------------------------------------------

        Accessors and Convertors                                              */

    public:

        const std::string &name() const { return m_body->m_name; }
        const std::type_info &typeInfo() const { return m_body->m_type_info; }

    /*--------------------------------------------------------------------------

        Methods                                                               */

    /*--------------------------------------------------------------------------

        Data                                                                  */

    protected:

        const Type_body *const m_body;

    /*--------------------------------------------------------------------------

        Static Methods                                                        */

    public:

        static Type getType(const std::string &);
		static Type getType(const std::type_info &);

    /*--------------------------------------------------------------------------

        Static Data                                                           */
};

/*------------------------------------------------------------------------------

    Operators                                                                 */

inline bool
operator==(const Type &c1, const Type &c2) {
    return *c1.m_body == *c2.m_body;
}

inline bool
operator==(const Type &c, const std::type_info &ti) {
    return *c.m_body == ti;
}

inline bool
operator==(const std::type_info &ti, const Type &c) {
    return *c.m_body == ti;
}

template <typename T1, typename T2>
inline T1
dynamic_cast_type(const T2 &t2) {
    // MIPSpro 7.4 has a bug in the RTTI.
    #ifdef IXCR_MIPSPRO
        const typename T1::body_type *t1b = static_cast<const typename T1::body_type *>(t2.m_body);
    #else
        const typename T1::body_type *t1b = dynamic_cast<const typename T1::body_type *>(t2.m_body);
    #endif
    assert(t1b != NULL);
    return T1(t1b);
}


/*------------------------------------------------------------------------------

    Static Methods                                                                   */

inline Type
Type::getType(const std::string &n) {
    return Type(Type_body::getType(n));
}

inline Type
Type::getType(const std::type_info &ti) {
    return Type(Type_body::getType(ti));
}

/*------------------------------------------------------------------------------

    Static Objects                                                            */



static const char Type_h_id[] =
"$Id: Type.hpp,v 1.1 2007/10/25 14:05:26 tdevadit Exp $";



} // namespace



#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
