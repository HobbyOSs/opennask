/******************************************************************************\
*                                                                              *
*   @.h                                                                        *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: debug.hpp,v 1.1 2007/10/25 14:05:27 tdevadit Exp $



#ifndef REFLCPP_DEBUG_h
#define REFLCPP_DEBUG_h



#include <assert.h>
#include <stdio.h>



#ifdef IXCR_DEBUG
#define ASSERT(condition, string)		if (!(condition)) { printf("%s\n", string); assert(condition);}
#else
#define ASSERT(condition, string)
#endif



namespace reflcpp {
static const char debug_h_id[] =
"$Id: debug.hpp,v 1.1 2007/10/25 14:05:27 tdevadit Exp $";
}



#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
