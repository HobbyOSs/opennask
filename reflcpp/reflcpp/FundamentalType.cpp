/******************************************************************************\
*                                                                              *
*   @.cpp                                                                      *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: FundamentalType.cpp,v 1.1 2007/10/25 14:05:24 tdevadit Exp $



#include "FundamentalType.hpp"
#include <iostream>



using namespace std;



namespace reflcpp {



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   FundamentalType_body                                                            |
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

FundamentalType_body::FundamentalType_body(const std::string &n,
 const std::type_info &ti)
 : Type_body(n, ti) {}

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



static const char FundamentalType_body_cpp_id[] =
"$Id: FundamentalType.cpp,v 1.1 2007/10/25 14:05:24 tdevadit Exp $";



} // namespace



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
