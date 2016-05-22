/******************************************************************************\
*                                                                              *
*   @.h                                                                        *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: MemberFunction.hpp,v 1.1 2007/10/25 14:05:25 tdevadit Exp $



#ifndef REFLCPP_MEMBER_FUNCTION_HPP
#define REFLCPP_MEMBER_FUNCTION_HPP



#include <vector>
#include <reflcpp/PtrHolder.hpp>
#include <reflcpp/RetValHolder.hpp>
#include <reflcpp/Traits.hpp>
#include <reflcpp/Arguments.hpp>



namespace reflcpp {


class BoundMemberFunction_body;
class ObjHolder;
template <typename T> class ObjHolder_tmpl;


/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   MemberFunction_body                                                        |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Class comments>

-                                                                             */

class MemberFunction_body {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

        MemberFunction_body(const std::string &name) 
            : m_name(name) {}
        virtual ~MemberFunction_body() {}

	private:
	    
	    // Disallow these unless specifically allowed.
	    MemberFunction_body(const MemberFunction_body &);
	    MemberFunction_body &operator=(const MemberFunction_body &);

    /*--------------------------------------------------------------------------

        Accessors and Convertors                                              */

    public:

        const std::string& name() const { return m_name; }

    /*--------------------------------------------------------------------------

        Methods                                                               */

    public:
        virtual const std::vector<const Type *> &getArguments() const = 0;

        /* Invoke methods */
        // No arguments
        template <typename Ret_TP, typename Obj_TP>
        Ret_TP invoke(Obj_TP *op) const {
            Arguments args;
            return invoke_helper<Ret_TP, Obj_TP>(op, 0, args);
        }
        // 1 argument
        template <typename Ret_TP, typename Obj_TP, typename Arg1_TP>
        Ret_TP invoke(Obj_TP *op, Arg1_TP &arg1) const {
            Arguments args;
            args.addRefArgument(arg1);
            return invoke_helper<Ret_TP, Obj_TP>(op, 1, args);
        }
        // 2 arguments
        template <typename Ret_TP, typename Obj_TP, typename Arg1_TP, typename Arg2_TP>
        Ret_TP invoke(Obj_TP *op, Arg1_TP &arg1, Arg2_TP &arg2) const {
            Arguments args;
            args.addRefArgument(arg1);
            args.addRefArgument(arg2);
            return invoke_helper<Ret_TP, Obj_TP>(op, 2, args);
        }

        // Overloaded
        template <typename Ret_TP, typename Obj_TP>
        Ret_TP invoke_helper(Obj_TP *op, const int n, Arguments &args) const {
            const ObjHolder_tmpl<Obj_TP> oh(op);
            RetValHolder_tmpl<Ret_TP> rv;
            this->invoke1(oh, rv, n, args);
            return rv.val();
        }
        template <typename Ret_TP>
        Ret_TP invoke_helper(const PtrHolder &ptrHolder, const int n, Arguments &args) const {
            RetValHolder_tmpl<Ret_TP> rv;
            this->invoke1(ptrHolder, rv, n, args);
            return rv.val();
        }

        virtual void invoke1(const ObjHolder &oh, RetValHolder &rvh,
            const int n, Arguments &args) const = 0;
        virtual void invoke1(const PtrHolder &ptrHolder, RetValHolder &rvh,
            const int n, Arguments &args) const = 0;

        virtual const BoundMemberFunction_body *bind(const ObjHolder &oh) const = 0;

    /*--------------------------------------------------------------------------

        Data                                                                  */

    private:

        const std::string m_name;
};


/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   MemberFunction                                                             |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Class comments>

-                                                                             */

class MemberFunction {

    /*--------------------------------------------------------------------------

        Constructors, Initters, Assignments, and Destructors                  */

    public:

        explicit MemberFunction(const MemberFunction_body *b) : m_body(b) {}

	private:

   	    // Implicit for these should be fine.
        // MemberFunction(const MemberFunction &);
        // MemberFunction &operator=(const MemberFunction &);

    /*--------------------------------------------------------------------------

        Accessors and Convertors                                              */

    public:

        const std::string &name() const { return m_body->name(); }
        const std::vector<const Type *> &getArguments() const { return m_body->getArguments(); }

        template <typename Ret_TP, typename Obj_TP>
        Ret_TP rmiInvoke(Obj_TP *op, Arguments &args)const { 
            return m_body->template invoke_helper<Ret_TP>(op, (const int)args.size(), args);
        }

        template <typename Ret_TP>
        Ret_TP genericInvoke(const PtrHolder &ptrHolder, Arguments &args)const { 
            return m_body->template invoke_helper<Ret_TP>(ptrHolder, (const int)args.size(), args);
        }

        template <typename Ret_TP, typename Obj_TP>
        Ret_TP invoke(Obj_TP *op) const {
            return m_body->invoke<Ret_TP>(op);
        }
        template <typename Ret_TP, typename Obj_TP, typename Arg1_TP>
        Ret_TP invoke(Obj_TP *op, Arg1_TP &arg1) const { 
            return m_body->invoke<Ret_TP>(op, 
                const_cast<typename TypeOp<Arg1_TP>::BareT &>(arg1)
            );
        }
        template <typename Ret_TP, typename Obj_TP, typename Arg1_TP, typename Arg2_TP>
        Ret_TP invoke(Obj_TP *op, Arg1_TP &arg1, Arg2_TP &arg2) const { 
            return m_body->invoke<Ret_TP>(op, 
                const_cast<typename TypeOp<Arg1_TP>::BareT &>(arg1), 
                const_cast<typename TypeOp<Arg2_TP>::BareT &>(arg2)
            );
        }

    /*--------------------------------------------------------------------------

        Data                                                                  */

    public:

        const MemberFunction_body *m_body;
};


static const char MemberFunction_hpp_id[] =
"$Id: MemberFunction.hpp,v 1.1 2007/10/25 14:05:25 tdevadit Exp $";



} // namespace



#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
