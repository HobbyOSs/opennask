/******************************************************************************\
*                                                                              *
*   @.h                                                                        *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: FundamentalType.hpp,v 1.1 2007/10/25 14:05:24 tdevadit Exp $



#ifndef REFLCPP_FUNDAMENTAL_TYPE_H
#define REFLCPP_FUNDAMENTAL_TYPE_H



#include <reflcpp/Type.hpp>



namespace reflcpp {



class Type;
class Type_body;



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   FundamentalType_body                                                       |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    This is the body for FundamentalType objects.  I'm not sure if
    it's strictly necessary.  It would be used for implementing methods that
    are specific to FundamentalType only.

-                                                                             */

class FundamentalType_body : public Type_body {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

        public:

            FundamentalType_body(const std::string &n, const std::type_info &t);

	private:
	    
	    // Disallow these unless specifically allowed.
	    FundamentalType_body(const FundamentalType_body &);
	    FundamentalType_body &operator=(const FundamentalType_body &); 

    /*--------------------------------------------------------------------------

        Methods                                                               */
};



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   FundamentalType_body_tmpl                                                  |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    This is the templated body for FundamentalType objects.  I'm not sure if
    it's strictly necessary.  It would be used for implementing methods that
    are specific to FundamentalType only, and need to have some kind of
    compile-time type-specific info.

-                                                                             */

template <typename T>
class FundamentalType_body_tmpl : public FundamentalType_body {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    private:

        FundamentalType_body_tmpl();

	private:
	    
	    // Disallow these unless specifically allowed.
	    FundamentalType_body_tmpl(const FundamentalType_body_tmpl &);
	    FundamentalType_body_tmpl &operator=(const FundamentalType_body_tmpl &); 

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:

        static FundamentalType_body_tmpl<T> *instance() {
            if (type_body == NULL) {
                type_body = new FundamentalType_body_tmpl<T>();
            }
            return type_body;
        }

    /*--------------------------------------------------------------------------

        Static Data                                                           */

    private:

        static FundamentalType_body_tmpl<T> *type_body;
};

/*------------------------------------------------------------------------------

    Specializations for Primitives                                            */

// Probably not really necessary to have these, since we have no definition
// for the FundamentalType_body constructor anyway.  Might want to do
// something to make sure a FundamentalType is not accidentally declared
// for a non-fundamental type, though.

/*
template <> FundamentalType_body<int>::FundamentalType_body();
template <> FundamentalType_body<float>::FundamentalType_body();
*/

template <typename T>
FundamentalType_body_tmpl<T>::FundamentalType_body_tmpl() : FundamentalType_body(typeid(T).name(), typeid(T)) {
}

/*------------------------------------------------------------------------------

    Static Data                                                               */

template <typename T>
FundamentalType_body_tmpl<T> *FundamentalType_body_tmpl<T>::type_body = NULL;



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   FundamentalType                                                            |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Type comments>

-                                                                             */

class FundamentalType : public Type {

    /*--------------------------------------------------------------------------

        Friends                                                               */

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

        public:

            // Construct the base object with the pointer to the static
            // Type body.
            FundamentalType(const FundamentalType_body *b) : Type(b) {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    FundamentalType(const FundamentalType &);
	    FundamentalType &operator=(const FundamentalType &);
    /*--------------------------------------------------------------------------

        Data                                                                  */

        public:

            // XXX - Fix this.
            //static const char typeName[] = "int";
};



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   FundamentalType_tmpl                                                       |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Type comments>

-                                                                             */

template <typename T>
class FundamentalType_tmpl : public FundamentalType {

    /*--------------------------------------------------------------------------

        Types and Constants                                                   */

        public:

            typedef T type;

    /*--------------------------------------------------------------------------

        Friends                                                               */

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

        public:

            // Construct the base object with the pointer to the static
            // Type body.
            FundamentalType_tmpl() : FundamentalType(FundamentalType_body_tmpl<T>::instance()) {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    FundamentalType_tmpl(const FundamentalType_tmpl &);
	    FundamentalType_tmpl &operator=(const FundamentalType_tmpl &);
};



static const char FundamentalType_h_id[] =
"$Id: FundamentalType.hpp,v 1.1 2007/10/25 14:05:24 tdevadit Exp $";



} // namespace



#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
