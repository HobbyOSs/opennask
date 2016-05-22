/******************************************************************************\
*                                                                              *
*   MemberFunctionDcl.hpp                                                      *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: MemberFunctionDcl.hpp,v 1.1 2007/10/25 14:05:25 tdevadit Exp $



#ifndef REFLCPP_MEMBER_FUNCTION_DCL_HPP
#define REFLCPP_MEMBER_FUNCTION_DCL_HPP



#include <reflcpp/Type.hpp>
#include <reflcpp/ClassType_tmpl.hpp>
#include <reflcpp/ClassType.hpp>
#include <reflcpp/FundamentalType.hpp>
#include <reflcpp/BoundClassType_tmpl.hpp>
#include <reflcpp/TypeInfoCmp.hpp>
#include <reflcpp/check.hpp>
#include <reflcpp/RetValHolder.hpp>
#include <reflcpp/Traits.hpp>



namespace reflcpp {


/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   MemberFunctionDcl                                                          |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    <Class comments>

-                                                                             */

template <typename Obj_TP, typename Ret_TP,
    typename Ret_TP::type (Obj_TP::*FuncP)(), 
    const char *Name>
class MemberFunctionDcl000 : public MemberFunction_body_tmpl3<Obj_TP, Ret_TP> {
    public:
        MemberFunctionDcl000() : MemberFunction_body_tmpl3<Obj_TP, Ret_TP>(Name) { }
        virtual typename Ret_TP::type invoke3(Obj_TP *op, const int n, Arguments &args) const {
            assert(n == 0);
            return (op->*FuncP)();
        }
};

template <typename Obj_TP, typename Ret_TP, 
    typename Arg1_TP,
    typename Ret_TP::type (Obj_TP::*FuncP)(typename Arg1_TP::type), 
    const char *Name>
class MemberFunctionDcl001 : public MemberFunction_body_tmpl3<Obj_TP, Ret_TP> {
    public:
        MemberFunctionDcl001() : MemberFunction_body_tmpl3<Obj_TP, Ret_TP>(Name) { 
            registerArgument(&arg1);
        }
        virtual typename Ret_TP::type invoke3(Obj_TP *op, const int n, Arguments &args) const {
            assert(n == 1);
            PtrHolder_tmpl<typename TypeOp<typename Arg1_TP::type>::BareT> &arg1_t
                = dynamic_cast<PtrHolder_tmpl<typename TypeOp<typename Arg1_TP::type>::BareT> &>(*(args[0]));
            return (op->*FuncP)(*(arg1_t.ptr));
        }
    protected:
        Arg1_TP arg1;
};

template <typename Obj_TP, typename Ret_TP, 
    typename Arg1_TP, typename Arg2_TP,
    typename Ret_TP::type (Obj_TP::*FuncP)(typename Arg1_TP::type, typename Arg2_TP::type), 
    const char *Name>
class MemberFunctionDcl002 : public MemberFunction_body_tmpl3<Obj_TP, Ret_TP> {
    public:
        MemberFunctionDcl002() : MemberFunction_body_tmpl3<Obj_TP, Ret_TP>(Name) { 
            registerArgument(&arg1);
            registerArgument(&arg2);
        }
        virtual typename Ret_TP::type invoke3(Obj_TP *op, const int n, Arguments &args) const {
            assert(n == 2);
            PtrHolder_tmpl<typename TypeOp<typename Arg1_TP::type>::BareT> &arg1_t
                = dynamic_cast<PtrHolder_tmpl<typename TypeOp<typename Arg1_TP::type>::BareT> &>(*(args[0]));
            PtrHolder_tmpl<typename TypeOp<typename Arg2_TP::type>::BareT> &arg2_t
                = dynamic_cast<PtrHolder_tmpl<typename TypeOp<typename Arg2_TP::type>::BareT> &>((*args[1]));
            return (op->*FuncP)(*(arg1_t.ptr), *(arg2_t.ptr));
        }
    protected:
        Arg1_TP arg1;
        Arg2_TP arg2;
};

template <typename Obj_TP, typename Ret_TP, 
    typename Arg1_TP, typename Arg2_TP, typename Arg3_TP,
    typename Ret_TP::type (Obj_TP::*FuncP)(typename Arg1_TP::type, typename Arg2_TP::type, typename Arg3_TP::type), 
    const char *Name>
class MemberFunctionDcl003 : public MemberFunction_body_tmpl3<Obj_TP, Ret_TP> {
    public:
        MemberFunctionDcl003() : MemberFunction_body_tmpl3<Obj_TP, Ret_TP>(Name) { 
            registerArgument(&arg1);
            registerArgument(&arg2);
            registerArgument(&arg3);
        }
        virtual typename Ret_TP::type invoke3(Obj_TP *op, const int n, Arguments &args) const {
            assert(n == 3);
            PtrHolder_tmpl<typename TypeOp<typename Arg1_TP::type>::BareT> &arg1_t
                = dynamic_cast<PtrHolder_tmpl<typename TypeOp<typename Arg1_TP::type>::BareT> &>(*(args[0]));
            PtrHolder_tmpl<typename TypeOp<typename Arg2_TP::type>::BareT> &arg2_t
                = dynamic_cast<PtrHolder_tmpl<typename TypeOp<typename Arg2_TP::type>::BareT> &>((*args[1]));
            PtrHolder_tmpl<typename TypeOp<typename Arg3_TP::type>::BareT> &arg3_t
                = dynamic_cast<PtrHolder_tmpl<typename TypeOp<typename Arg3_TP::type>::BareT> &>((*args[2]));
            return (op->*FuncP)(*(arg1_t.ptr), *(arg2_t.ptr), *(arg3_t.ptr));
        }
    protected:
        Arg1_TP arg1;
        Arg2_TP arg2;
        Arg3_TP arg3;
};

template <typename Obj_TP, typename Ret_TP, 
    typename Arg1_TP, typename Arg2_TP, typename Arg3_TP, typename Arg4_TP,
    typename Ret_TP::type (Obj_TP::*FuncP)(typename Arg1_TP::type, typename Arg2_TP::type, typename Arg3_TP::type, typename Arg4_TP::type), 
    const char *Name>
class MemberFunctionDcl004 : public MemberFunction_body_tmpl3<Obj_TP, Ret_TP> {
    public:
        MemberFunctionDcl004() : MemberFunction_body_tmpl3<Obj_TP, Ret_TP>(Name) { 
            registerArgument(&arg1);
            registerArgument(&arg2);
            registerArgument(&arg3);
            registerArgument(&arg4);
        }
        virtual typename Ret_TP::type invoke3(Obj_TP *op, const int n, Arguments &args) const {
            assert(n == 4);
            PtrHolder_tmpl<typename TypeOp<typename Arg1_TP::type>::BareT> &arg1_t
                = dynamic_cast<PtrHolder_tmpl<typename TypeOp<typename Arg1_TP::type>::BareT> &>(*(args[0]));
            PtrHolder_tmpl<typename TypeOp<typename Arg2_TP::type>::BareT> &arg2_t
                = dynamic_cast<PtrHolder_tmpl<typename TypeOp<typename Arg2_TP::type>::BareT> &>((*args[1]));
            PtrHolder_tmpl<typename TypeOp<typename Arg3_TP::type>::BareT> &arg3_t
                = dynamic_cast<PtrHolder_tmpl<typename TypeOp<typename Arg3_TP::type>::BareT> &>((*args[2]));
            PtrHolder_tmpl<typename TypeOp<typename Arg4_TP::type>::BareT> &arg4_t
                = dynamic_cast<PtrHolder_tmpl<typename TypeOp<typename Arg4_TP::type>::BareT> &>((*args[3]));
            return (op->*FuncP)(*(arg1_t.ptr), *(arg2_t.ptr), *(arg3_t.ptr), *(arg4_t.ptr));
        }
    protected:
        Arg1_TP arg1;
        Arg2_TP arg2;
        Arg3_TP arg3;
        Arg4_TP arg4;
};

template <typename Obj_TP, typename Ret_TP, 
    typename Arg1_TP, typename Arg2_TP, typename Arg3_TP, typename Arg4_TP, typename Arg5_TP,
    typename Ret_TP::type (Obj_TP::*FuncP)(typename Arg1_TP::type, typename Arg2_TP::type, typename Arg3_TP::type, typename Arg4_TP::type, typename Arg5_TP::type), 
    const char *Name>
class MemberFunctionDcl005 : public MemberFunction_body_tmpl3<Obj_TP, Ret_TP> {
    public:
        MemberFunctionDcl005() : MemberFunction_body_tmpl3<Obj_TP, Ret_TP>(Name) { 
            registerArgument(&arg1);
            registerArgument(&arg2);
            registerArgument(&arg3);
            registerArgument(&arg4);
            registerArgument(&arg5);
        }
        virtual typename Ret_TP::type invoke3(Obj_TP *op, const int n, Arguments &args) const {
            assert(n == 5);
            PtrHolder_tmpl<typename TypeOp<typename Arg1_TP::type>::BareT> &arg1_t
                = dynamic_cast<PtrHolder_tmpl<typename TypeOp<typename Arg1_TP::type>::BareT> &>(*(args[0]));
            PtrHolder_tmpl<typename TypeOp<typename Arg2_TP::type>::BareT> &arg2_t
                = dynamic_cast<PtrHolder_tmpl<typename TypeOp<typename Arg2_TP::type>::BareT> &>((*args[1]));
            PtrHolder_tmpl<typename TypeOp<typename Arg3_TP::type>::BareT> &arg3_t
                = dynamic_cast<PtrHolder_tmpl<typename TypeOp<typename Arg3_TP::type>::BareT> &>((*args[2]));
            PtrHolder_tmpl<typename TypeOp<typename Arg4_TP::type>::BareT> &arg4_t
                = dynamic_cast<PtrHolder_tmpl<typename TypeOp<typename Arg4_TP::type>::BareT> &>((*args[3]));
            PtrHolder_tmpl<typename TypeOp<typename Arg5_TP::type>::BareT> &arg5_t
                = dynamic_cast<PtrHolder_tmpl<typename TypeOp<typename Arg5_TP::type>::BareT> &>((*args[4]));
            return (op->*FuncP)(*(arg1_t.ptr), *(arg2_t.ptr), *(arg3_t.ptr), *(arg4_t.ptr), *(arg5_t.ptr));
        }
    protected:
        Arg1_TP arg1;
        Arg2_TP arg2;
        Arg3_TP arg3;
        Arg4_TP arg4;
        Arg5_TP arg5;
};


static const char MemberFunctionDcl_hpp_id[] =
"$Id: MemberFunctionDcl.hpp,v 1.1 2007/10/25 14:05:25 tdevadit Exp $";


} // namespace



#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
