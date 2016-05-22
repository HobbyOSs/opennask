


/******************************************************************************\
*                                                                              *
*   BoundMemberFunction.hpp                                                    *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: BoundMemberFunction.hpp,v 1.1 2007/10/25 14:05:23 tdevadit Exp $



#ifndef REFLCPP_BOUND_MEMBER_FUNCTION_HPP
#define REFLCPP_BOUND_MEMBER_FUNCTION_HPP



#include <reflcpp/PtrHolder.hpp>
#include <reflcpp/RetValHolder.hpp>
#include <reflcpp/Traits.hpp>
#include <reflcpp/Arguments.hpp>


/*------------------------------------------------------------------------------

    Forwards (global)                                                         */



namespace reflcpp {



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   BoundMemberFunction_body                                                   |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Class comments>

-                                                                             */

class BoundMemberFunction_body {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:
            
        BoundMemberFunction_body() {}

    private:
	    
        // Disallow these unless specifically allowed.
        BoundMemberFunction_body(const BoundMemberFunction_body &);
        BoundMemberFunction_body &operator=(const BoundMemberFunction_body &);

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:
        virtual const std::vector<const Type *> &getArguments() const = 0;

        /* Invoke methods */
        // No arguments
		template <typename Ret_TP>
		Ret_TP invoke() const {
            Arguments args;
            return invoke_helper<Ret_TP>(0, args);
        }
		// 1 argument
        template <typename Ret_TP, typename Arg1_TP>
        Ret_TP invoke(Arg1_TP &arg1) const {
            Arguments args;
            args.addRefArgument(arg1);
            return invoke_helper<Ret_TP>(1, args);
        }
		// 2 arguments
        template <typename Ret_TP, typename Arg1_TP, typename Arg2_TP>
        Ret_TP invoke(Arg1_TP &arg1, Arg2_TP &arg2) const {
            Arguments args;
            args.addRefArgument(arg1);
            args.addRefArgument(arg2);
            return invoke_helper<Ret_TP>(2, args);
        }

        template <typename Ret_TP>
        Ret_TP invoke_helper(const int n, Arguments &args) const {
            RetValHolder_tmpl<Ret_TP> rv;
            this->invoke1(rv, n, args);
            return rv.val();
        }

        virtual void invoke1(RetValHolder &rvh, const int n, Arguments &args) const = 0;
};


/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   BoundMemberFunction                                                        |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Class comments>

-                                                                             */

class BoundMemberFunction {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

        explicit BoundMemberFunction(const BoundMemberFunction_body *b) : m_body(b) {}

	private:
	    
	    // Currently, implicit for these is okay.
	    // BoundMemberFunction(const BoundMemberFunction &);
	    // BoundMemberFunction &operator=(const BoundMemberFunction &);

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:
        
        const std::vector<const Type *> &getArguments() const { return m_body->getArguments(); }

        template <typename Ret_TP>
        Ret_TP rmiInvoke(Arguments &args) const { 
            return m_body->template invoke_helper<Ret_TP>((const int)args.size(), args);
        }

        template <typename Ret_TP>
        Ret_TP invoke() const {
			return m_body->invoke<Ret_TP>();
        }
        template <typename Ret_TP, typename Arg1_TP>
        Ret_TP invoke(Arg1_TP &arg1) const { 
			return m_body->invoke<Ret_TP>(
                const_cast<typename TypeOp<Arg1_TP>::BareT &>(arg1)
            );
        }
        template <typename Ret_TP, typename Arg1_TP, typename Arg2_TP>
        Ret_TP invoke(Arg1_TP &arg1, Arg2_TP &arg2) const { 
			return m_body->invoke<Ret_TP>(
                const_cast<typename TypeOp<Arg1_TP>::BareT &>(arg1), 
                const_cast<typename TypeOp<Arg2_TP>::BareT &>(arg2)
            );
        }

 
    /*--------------------------------------------------------------------------

        Data                                                                  */

    private:

        const BoundMemberFunction_body *m_body;
};


static const char BoundMemberFunction_hpp_id[] =
"$Id: BoundMemberFunction.hpp,v 1.1 2007/10/25 14:05:23 tdevadit Exp $";



} // namespace

#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4: */

