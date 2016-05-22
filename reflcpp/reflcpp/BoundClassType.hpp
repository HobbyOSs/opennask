/******************************************************************************\
*                                                                              *
*   BoundClassType.h                                                                        *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: BoundClassType.hpp,v 1.1 2007/10/25 14:05:23 tdevadit Exp $



#ifndef REFLCPP_BOUND_CLASS_TYPE_H
#define REFLCPP_BOUND_CLASS_TYPE_H




#include <reflcpp/BoundMemberFunction.hpp>

/*------------------------------------------------------------------------------

    Forwards (global)                                                         */



namespace reflcpp {



class PtrHolder;
template <typename T> class PtrHolder_tmpl;



/*------------------------------------------------------------------------------

    Forwards (within namespace)                                               */




/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   BoundDataMember_body                                                       |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Class comments>

-                                                                             */

/*------------------------------------------------------------------------------

    Forwards (only needed by this class)                                      */

/*------------------------------------------------------------------------------

    Inline Helper Functions and Helper Classes                                */


class BoundDataMember_body {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:
            
        BoundDataMember_body() {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    BoundDataMember_body(const BoundDataMember_body &);
	    BoundDataMember_body &operator=(const BoundDataMember_body &);

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:

        template <typename Val_NT>
        Val_NT &ref() const {
            // I cannot use something like casting the Member to the
            // Member_body_tmpl1<Val_NT>, because this Val_NT is the actual
            // type, not a Type construct, which is what Member_body_tmpl1
            // actually uses.  Thus I pass a PtrHolder down to get a
            // pointer to the value.
            PtrHolder_tmpl<Val_NT> vph;
            this->getPtr(&vph);
            assert(vph.ptr != NULL);
            return *vph.ptr;
        }

        virtual void getPtr(PtrHolder *vph) const = 0;
};

/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   BoundDataMember                                                            |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Class comments>

-                                                                             */

class BoundDataMember {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

        explicit BoundDataMember(const BoundDataMember_body *b) : m_body(b) {}

	private:
	    
	    // Currently, implicit for these is okay, later, will need
        // to handle reference counting or copying of the body object.
	    // BoundDataMember(const BoundDataMember &);
	    // BoundDataMember &operator=(const BoundDataMember &);

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:

        template <typename Val_T>
        Val_T &ref() const { return m_body->ref<Val_T>(); }

 
    /*--------------------------------------------------------------------------

        Data                                                                  */

    private:

        const BoundDataMember_body *m_body;
};


/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   BoundClassType_body                                                             |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Class comments>

-                                                                             */

/*------------------------------------------------------------------------------

    Forwards (only needed by this class)                                      */

/*------------------------------------------------------------------------------

    Inline Helper Functions and Helper Classes                                */

class BoundClassType_body {

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

        BoundClassType_body() {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    BoundClassType_body(const BoundClassType_body &);
	    BoundClassType_body &operator=(const BoundClassType_body &);

    /*--------------------------------------------------------------------------

        Accessors and Convertors                                              */

    /*--------------------------------------------------------------------------

        Operators                                                             */

    /*--------------------------------------------------------------------------

        Methods                                                               */

    /*--------------------------------------------------------------------------

        Data                                                                  */

    /*--------------------------------------------------------------------------

        Static Methods                                                        */

    /*--------------------------------------------------------------------------

        Static Data                                                           */

    /*--------------------------------------------------------------------------

        Access Specifiers                                                     */
};


/*------------------------------------------------------------------------------

    Constructors, Initters, Assignments, and Destructors                      */

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

class BoundClassType {

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

        BoundClassType() {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    BoundClassType(const BoundClassType &);
	    BoundClassType &operator=(const BoundClassType &);

    /*--------------------------------------------------------------------------

        Accessors and Convertors                                              */

    /*--------------------------------------------------------------------------

        Operators                                                             */

    /*--------------------------------------------------------------------------

        Methods                                                               */

    /*--------------------------------------------------------------------------

        Data                                                                  */

    /*--------------------------------------------------------------------------

        Static Methods                                                        */

    /*--------------------------------------------------------------------------

        Static Data                                                           */

    /*--------------------------------------------------------------------------

        Access Specifiers                                                     */
};


/*------------------------------------------------------------------------------

    Constructors, Initters, Assignments, and Destructors                      */

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
static const char BoundClassType_h_id[] =
"$Id: BoundClassType.hpp,v 1.1 2007/10/25 14:05:23 tdevadit Exp $";



} // namespace



#ifdef MSL_INCLUDE_TEMPLATE
    //#include "BoundClassType.cc"
#endif



#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4: */
