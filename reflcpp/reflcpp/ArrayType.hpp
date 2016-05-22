/******************************************************************************\
*                                                                              *
*   @.h                                                                        *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: ArrayType.hpp,v 1.1 2007/10/25 14:05:22 tdevadit Exp $



#ifndef REFLCPP_ARRAY_TYPE_H
#define REFLCPP_ARRAY_TYPE_H



#include <typeinfo>
#include <reflcpp/Type.hpp>



namespace reflcpp {



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   ArrayType_body                                                       |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    This is the body for ArrayType objects.  I'm not sure if
    it's strictly necessary.  It would be used for implementing methods that
    are specific to ArrayType only.

-                                                                             */

class ArrayType_body : public Type_body {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

        public:

            ArrayType_body(const std::string &n, const std::type_info &t);

	private:
	    
	    // Disallow these unless specifically allowed.
	    ArrayType_body(const ArrayType_body &);
	    ArrayType_body &operator=(const ArrayType_body &); 

    /*--------------------------------------------------------------------------

        Methods                                                               */
};



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   ArrayType_body_tmpl                                                  |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    This is the templated body for ArrayType objects.

-                                                                             */

template <typename T, int N>
class ArrayType_body_tmpl : public ArrayType_body {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    private:

        ArrayType_body_tmpl();

	private:
	    
	    // Disallow these unless specifically allowed.
	    ArrayType_body_tmpl(const ArrayType_body_tmpl &);
	    ArrayType_body_tmpl &operator=(const ArrayType_body_tmpl &); 

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:

        static ArrayType_body_tmpl<T, N> *instance() {
            if (type_body == NULL) {
                type_body = new ArrayType_body_tmpl<T, N>();
            }
            return type_body;
        }

    /*--------------------------------------------------------------------------

        Static Data                                                           */

    private:

        static ArrayType_body_tmpl<T, N> *type_body;
};

template <typename T, int N>
ArrayType_body_tmpl<T, N>::ArrayType_body_tmpl() : ArrayType_body(typeid(typename T::type[N]).name(), typeid(typename T::type[N])) {}

/*------------------------------------------------------------------------------

    Static Data                                                               */

template <typename T, int N>
ArrayType_body_tmpl<T, N> *ArrayType_body_tmpl<T, N>::type_body = NULL;



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   ArrayType                                                            |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Type comments>

-                                                                             */

class ArrayType : public Type {

    /*--------------------------------------------------------------------------

        Friends                                                               */

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

        public:

            // Construct the base object with the pointer to the static
            // Type body.
            ArrayType(const ArrayType_body *b) : Type(b) {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    ArrayType(const ArrayType &);
	    ArrayType &operator=(const ArrayType &);
};



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   ArrayType_tmpl                                                            |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Type comments>

-                                                                             */

template <typename T, int N>
class ArrayType_tmpl : public ArrayType {

    /*--------------------------------------------------------------------------

        Types and Constants                                                   */

        public:

            typedef typename T::type type[N];

    /*--------------------------------------------------------------------------

        Friends                                                               */

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

        public:

            // Construct the base object with the pointer to the static
            // Type body.
            ArrayType_tmpl() : ArrayType(ArrayType_body_tmpl<T, N>::instance()) {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    ArrayType_tmpl(const ArrayType_tmpl &);
	    ArrayType_tmpl &operator=(const ArrayType_tmpl &);
};



static const char ArrayType_h_id[] =
"$Id: ArrayType.hpp,v 1.1 2007/10/25 14:05:22 tdevadit Exp $";



} // namespace



#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
