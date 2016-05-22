/******************************************************************************\
*                                                                              *
*   @.h                                                                        *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: check.hpp,v 1.1 2007/10/25 14:05:27 tdevadit Exp $



#ifndef REFLCPP_CHECK_H
#define REFLCPP_CHECK_H



namespace reflcpp {



extern void checkFail(const char *t, const char *, const char *, int);

// Always defined.  Use this for assertions that won't be very expensive.
#define ixcr_check(e)                                                           \
    (void) ((e) || (reflcpp::checkFail("Check", #e, __FILE__, __LINE__), 0))



// Use this for assertions only in IXCR_DEBUG versions.
#ifdef IXCR_DEBUG
    #define ixcr_assert(e)                                                      \
	(void) ((e) || (reflcpp::checkFail("Assertion", #e, __FILE__, __LINE__), 0))
#else
    #define ixcr_assert(e)    ((void) 0)
#endif



static const char check_hpp_id[] =
"$Id: check.hpp,v 1.1 2007/10/25 14:05:27 tdevadit Exp $";



}



#endif
