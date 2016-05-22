/******************************************************************************\
*                                                                              *
*   @.cpp                                                                      *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: check.cpp,v 1.1 2007/10/25 14:05:27 tdevadit Exp $



#include "check.hpp"
#include <stdlib.h>
#include <stdio.h>



namespace reflcpp {



void
checkFail(const char *type, const char *e, const char *f, int line) {

    fprintf(stderr, "%s failed in %s at line %d: %s.\n", type, f, line, e);

    #ifdef IXCR_LINUX
        // On Linux, getting core dumps from threaded processes is a pain.  If
        // the thread is not the "main" thread, it doesn't dump core.  The
        // workaround is to fork first to create a process with just the
        // aborting thread, then abort.
	if (fork() == 0) {
	    // Child
	    abort();
	} else {
	    // Parent
	    exit(1);
	}
    #else
	abort();
    #endif
}



static const char check_cpp_id[] =
"$Id: check.cpp,v 1.1 2007/10/25 14:05:27 tdevadit Exp $";



} // namespace
