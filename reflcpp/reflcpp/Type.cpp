/******************************************************************************\
*                                                                              *
*   @.cpp                                                                      *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: Type.cpp,v 1.1 2007/10/25 14:05:26 tdevadit Exp $



#include "Type.hpp"
#include <iostream>



using namespace std;



namespace reflcpp {



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   Type_body                                                                 |
|                                                                              |
|------------------------------------------------------------------------------|
\-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------

   Types and Constants                                                        */

/*------------------------------------------------------------------------------

    Constructors, Initters, Assignments, and Destructors                      */

/*  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

    <function>

        <p>
            <Description of parameter p>.

    <Description of function>

-                                                                             */

Type_body::Type_body(const std::string &n, const type_info &ti) : m_name(n),
 m_type_info(ti) {
    // Check to make sure these are initialized.  If we make them static objects
    // (as opposed to a static pointers), we have initialization order problems.
    if (s_class_name_map == NULL) {
        s_class_name_map = new class_name_map_t;
    }
	if (s_class_info_map == NULL) {
        s_class_info_map = new class_info_map_t;
    }
	s_class_name_map->insert(class_name_map_t::value_type(n, this));
    s_class_info_map->insert(class_info_map_t::value_type(&ti, this));
}

/*------------------------------------------------------------------------------

    Accessors and Convertors                                                  */

/*------------------------------------------------------------------------------

    Operators                                                                 */

/*------------------------------------------------------------------------------

    Methods                                                                   */


/*------------------------------------------------------------------------------

    Static Methods and Helper Functions                                       */

const Type_body *
Type_body::getType(const std::string &n) {
    // XXX - Should throw.
    assert(s_class_name_map);
    class_name_map_t::const_iterator it = s_class_name_map->find(n);
    // XXX - Should throw.
    assert(it != s_class_name_map->end());
    return it->second;
}

const Type_body *
Type_body::getType(const type_info &ti) {
    // XXX - Should throw.
    assert(s_class_info_map);
    class_info_map_t::const_iterator it = s_class_info_map->find(&ti);
    // XXX - Should throw.
    assert(it != s_class_info_map->end());
    return it->second;
}

/*------------------------------------------------------------------------------

    Static Data                                                               */

Type_body::class_name_map_t *Type_body::s_class_name_map;
Type_body::class_info_map_t *Type_body::s_class_info_map;



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   Type                                                                      |
|                                                                              |
|------------------------------------------------------------------------------|
\-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------

   Types and Constants                                                        */

/*------------------------------------------------------------------------------

    Constructors, Initters, Assignments, and Destructors                      */

/*  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

    <function>

        <p>
            <Description of parameter p>.

    <Description of function>

-                                                                             */

/*------------------------------------------------------------------------------

    Accessors and Convertors                                                  */

/*------------------------------------------------------------------------------

    Operators                                                                 */

/*------------------------------------------------------------------------------

    Methods                                                                   */

/*------------------------------------------------------------------------------

    Static Methods and Helper Functions                                       */

/*------------------------------------------------------------------------------

    Static Data                                                               */



static const char Type_cpp_id[] =
"$Id: Type.cpp,v 1.1 2007/10/25 14:05:26 tdevadit Exp $";



} // namespace



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
