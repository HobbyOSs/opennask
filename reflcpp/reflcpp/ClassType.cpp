/******************************************************************************\
*                                                                              *
*   @.cpp                                                                      *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: ClassType.cpp,v 1.1 2007/10/25 14:05:23 tdevadit Exp $



#include "ClassType.hpp"
#include <iostream>
#include "check.hpp"



using namespace std;



namespace reflcpp {



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   ClassType_body                                                                 |
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

ClassType_body::ClassType_body(const std::string &n, const type_info &ti) : Type_body(n, ti) {
}

// This should never actually be called, but is provided for the
// intermediate (not most derived) base classes where ClassType_body
// is used as a virtual base class.
ClassType_body::ClassType_body() : Type_body(string(""), typeid(void)) {
    ixcr_check(false);
}

/*------------------------------------------------------------------------------

    Accessors and Convertors                                                  */

/*------------------------------------------------------------------------------

    Operators                                                                 */

/*------------------------------------------------------------------------------

    Methods                                                                   */

/*
const Member_body *
ClassType_body::getMember(const std::string &n) const {
    field_map_t::const_iterator it = m_field_map.find(n);
    // XXX - Should throw eventually.
    assert(it != m_field_map.end());
    return (*it).second;
}
*/

/*
bool
ClassType_body::derivesFrom(const ClassType_body &ci) const {

    if (m_type_info == ci.m_type_info) {
        return true;
    } else {
        // Recurse over direct base classes.
        for (base_vec_t::const_iterator it = m_base_infos.begin();
         it != m_base_infos.end(); ++it) {
            if ((*it)->derivesFrom(ci)) {
                return true;
            }
        }
    }

    return false;
}
*/


/*------------------------------------------------------------------------------

    Static Methods and Helper Functions                                       */

const ClassType_body *
// XXX - Rename method below.
ClassType_body::getClass(const std::string &n) {
    const ClassType_body *cb = dynamic_cast<const ClassType_body *>(Type_body::getType(n));
    //cerr << "type name: " << n << endl;
    assert(cb != NULL);
    return cb;
}

const ClassType_body *
// XXX - Rename method below.
ClassType_body::getClass(const type_info &ti) {
    const ClassType_body *cb = dynamic_cast<const ClassType_body *>(Type_body::getType(ti));
    //cerr << "type name: " << ti.name() << endl;
    assert(cb != NULL);
    return cb;
}

/*------------------------------------------------------------------------------

    Static Data                                                               */

// ClassType_body::class_info_map_t *ClassType_body::s_class_info_map;


/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   ClassType                                                                      |
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

ClassType
ClassType::getClass(const std::string &n) {
    return ClassType(ClassType_body::getClass(n));
}

ClassType
ClassType::getClass(const type_info &ti) {
    return ClassType(ClassType_body::getClass(ti));
}

/*------------------------------------------------------------------------------

    Static Data                                                               */



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   Member_body                                                                 |
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

DataMember_body::DataMember_body(const std::string &name, const std::string &type_name)
 : m_name(name), m_typeName(type_name) {
}



static const char ClassType_cpp_id[] =
"$Id: ClassType.cpp,v 1.1 2007/10/25 14:05:23 tdevadit Exp $";



} // namespace



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
