/******************************************************************************\
*                                                                              *
*   @.h                                                                        *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: PointerType.hpp,v 1.1 2007/10/25 14:05:25 tdevadit Exp $



#ifndef REFLCPP_POINTER_TYPE_H
#define REFLCPP_POINTER_TYPE_H



/*
#include <assert.h>
#include <string>
#include <vector>
#include <map>
#include <functional>

#include <reflcpp/ValHolder.hpp>
*/
#include <typeinfo>
#include <reflcpp/Type.hpp>



// Unfortunately, Member and Type need each other's definitions.  So far, it
// seems we can get away with forward declaring the Member classes.  Someday may
// need to merge Type.h and Member.h to get all the ordering right.
/*
#include <reflcpp/Member.hpp>
#include <reflcpp/BoundMember.hpp>
*/



namespace reflcpp {



class BoundDataMember;

class Type;
class Type_body;

class DataMember;
template <typename T> class TypedMember;
class DataMember_body;
template <typename T> class TypedMember_body;

template <typename T> class ObjHolder_tmpl;



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   PointerType_body                                                       |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    This is the body for PointerType objects.  I'm not sure if
    it's strictly necessary.  It would be used for implementing methods that
    are specific to PointerType only.

-                                                                             */

class PointerType_body : public Type_body {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

            PointerType_body(const std::string &n, const std::type_info &t);

	private:
	    
	    // Disallow these unless specifically allowed.
	    PointerType_body(const PointerType_body &);
	    PointerType_body &operator=(const PointerType_body &); 

    /*--------------------------------------------------------------------------

        Methods                                                               */
};



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   PointerType_body_tmpl                                                  |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    This is the templated body for PointerType objects.  I'm not sure if
    it's strictly necessary.  It would be used for implementing methods that
    are specific to PointerType only, and need to have some kind of
    compile-time type-specific info.

-                                                                             */

template <typename T>
class PointerType_body_tmpl : public PointerType_body {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    private:

        PointerType_body_tmpl();

	private:
	    
	    // Disallow these unless specifically allowed.
	    PointerType_body_tmpl(const PointerType_body_tmpl &);
	    PointerType_body_tmpl &operator=(const PointerType_body_tmpl &); 

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:

        static PointerType_body_tmpl<T> *instance() {
            if (type_body == NULL) {
                type_body = new PointerType_body_tmpl<T>();
            }
            return type_body;
        }

    /*--------------------------------------------------------------------------

        Static Data                                                           */

    private:

        static PointerType_body_tmpl<T> *type_body;
};

template <typename T>
PointerType_body_tmpl<T>::PointerType_body_tmpl() : PointerType_body(typeid(typename T::type *).name(), typeid(typename T::type *)) {}

/*------------------------------------------------------------------------------

    Static Data                                                               */

template <typename T>
PointerType_body_tmpl<T> *PointerType_body_tmpl<T>::type_body = NULL;



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   PointerType                                                            |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Type comments>

-                                                                             */

class PointerType : public Type {

    /*--------------------------------------------------------------------------

        Friends                                                               */

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

        public:

            // Construct the base object with the pointer to the static
            // Type body.
            PointerType(const PointerType_body *b) : Type(b) {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    PointerType(const PointerType &);
	    PointerType &operator=(const PointerType &);
};



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   PointerType_tmpl                                                            |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Type comments>

-                                                                             */

template <typename T>
class PointerType_tmpl : public PointerType {

    /*--------------------------------------------------------------------------

        Types and Constants                                                   */

        public:

            typedef typename T::type *type;

    /*--------------------------------------------------------------------------

        Friends                                                               */

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

        public:

            // Construct the base object with the pointer to the static
            // Type body.
            PointerType_tmpl() : PointerType(PointerType_body_tmpl<T>::instance()) {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    PointerType_tmpl(const PointerType_tmpl &);
	    PointerType_tmpl &operator=(const PointerType_tmpl &);
};



static const char PointerType_h_id[] =
"$Id: PointerType.hpp,v 1.1 2007/10/25 14:05:25 tdevadit Exp $";



} // namespace



#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
