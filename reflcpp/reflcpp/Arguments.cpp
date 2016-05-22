/******************************************************************************\
*                                                                              *
*   @.cpp                                                                      *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: Arguments.cpp,v 1.1 2007/10/25 14:05:22 tdevadit Exp $



#include "Arguments.hpp"



using namespace std;



namespace reflcpp {



Arguments::~Arguments() {
    // Delete the allocated parameters
    for (Arguments::iterator it = begin(); it != end(); ++it) {
        delete (*it);
    }
}

void 
Arguments::addArgument(PtrHolder *ptrHolder) {
    push_back(ptrHolder);
}




static const char Arguments_cpp_id[] =
"$Id: Arguments.cpp,v 1.1 2007/10/25 14:05:22 tdevadit Exp $";



} // namespace



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
