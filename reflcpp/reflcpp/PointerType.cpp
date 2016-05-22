/******************************************************************************\
*                                                                              *
*   @.cpp                                                                      *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: PointerType.cpp,v 1.1 2007/10/25 14:05:25 tdevadit Exp $



#include "PointerType.hpp"
#include <iostream>



using namespace std;



namespace reflcpp {




/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   PointerType_body                                                            |
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

PointerType_body::PointerType_body(const std::string &n,
 const std::type_info &ti)
 : Type_body(n, ti) {}

/*
FT
template <>
PointerType_body<int>::PointerType_body() : Type_body("int", typeid(int)) {
    m_base_infos.push_back(&TypedClass<Type::Fundamental_tag>::class_body);
}

template <>
PointerType_body<float>::PointerType_body() : Type_body("float", typeid(float)) {
    m_base_infos.push_back(&TypedClass<Type::Fundamental_tag>::class_body);
}
*/

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



#if 0
/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   TypedClass_body                                                            |
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

template <>
TypedClass_body<Type::Fundamental_tag>::TypedClass_body()
 : Class_body("Fundamental", typeid(void)) {}

/*
FT
template <>
TypedClass_body<int>::TypedClass_body() : Type_body("int", typeid(int)) {
    m_base_infos.push_back(&TypedClass<Type::Fundamental_tag>::class_body);
}

template <>
TypedClass_body<float>::TypedClass_body() : Type_body("float", typeid(float)) {
    m_base_infos.push_back(&TypedClass<Type::Fundamental_tag>::class_body);
}
*/

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
#endif



static const char PointerType_cpp_id[] =
"$Id: PointerType.cpp,v 1.1 2007/10/25 14:05:25 tdevadit Exp $";



} // namespace



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
