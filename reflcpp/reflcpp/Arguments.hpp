/******************************************************************************\
*                                                                              *
*   @.h                                                                        *
*                                                                              *
*   Maintains the argument list for a member function invocation.              *
*                                                                              *
\******************************************************************************/

// $Id: Arguments.hpp,v 1.1 2007/10/25 14:05:22 tdevadit Exp $



#ifndef REFLCPP_ARGUMENTS_HPP
#define REFLCPP_ARGUMENTS_HPP



#include <vector>
#include <reflcpp/PtrHolder.hpp>



namespace reflcpp {


class Arguments : public std::vector<PtrHolder*> {
    public:
        ~Arguments();

        /**
         * The PtrHolder will get deallocated with the Arguments object.
         * Therefore, no need for the called to delete this.
         * Note that if the same argument needs to be used for 2 argument
         * lists, it is required to create a duplicate of the PtrHolder.
         *
         * TODO: Add duplicating functionality to PtrHolder.
         */
        void addArgument(PtrHolder *ptrHolder);

        template <typename T>
        void addValueArgument(T &data);

        // No deletion is performed for the object
        template <typename T>
        void addRefArgument(T &data);
};

template <typename T>
void
Arguments::addValueArgument(T &data) {
    push_back(new SmartPtrHolder_tmpl<T>(new T(data)));
}

template <typename T>
void
Arguments::addRefArgument(T &data) {
    push_back(new PtrHolder_tmpl<T>(&data));
}



static const char Arguments_hpp_id[] =
"$Id: Arguments.hpp,v 1.1 2007/10/25 14:05:22 tdevadit Exp $";



} // namespace 



#endif
