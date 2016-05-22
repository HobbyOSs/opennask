/******************************************************************************\
*                                                                              *
*   TypeInfoCmp.hpp                                                                        *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: TypeInfoCmp.hpp,v 1.1 2007/10/25 14:05:27 tdevadit Exp $



#ifndef REFLCPP_TYPE_INFO_CMP_H
#define REFLCPP_TYPE_INFO_CMP_H



namespace reflcpp {



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   TypeInfoCmp                                                                |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    Comparison function for type_info objects.  Note that we cannot just
    compare the pointers, since two equivalent type_info objects may have
    different pointers.

-                                                                             */

struct TypeInfoCmp : public std::binary_function<const std::type_info *,
                                 const std::type_info *, bool> {

    /*--------------------------------------------------------------------------

        Operators                                                             */

    bool operator()(const std::type_info *ti1,
     const std::type_info *ti2) const {
        // VC++ seems to have the wrong type for before(), so
        // cast it to avoid a warning.
        #ifdef IXCR_VCPP
            return (ti1->before(*ti2) != 0);
        #else
            return ti1->before(*ti2);
        #endif
    }
};




static const char TypeInfoCmp_hpp_id[] =
"$Id: TypeInfoCmp.hpp,v 1.1 2007/10/25 14:05:27 tdevadit Exp $";



} // Package namespace



#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4: */
