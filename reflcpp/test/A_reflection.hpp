#ifndef REFLCPP_A_H
#define REFLCPP_A_H



#include <reflcpp/ClassType_tmpl.hpp>
#include <reflcpp/FundamentalType.hpp>
#include <reflcpp/MemberFunctionDcl.hpp>
#include "A.hpp"



namespace reflcpp {



template <typename BotA_TP, typename Der_TP, int N>
class Bases<BotA_TP, Der_TP, A, N> : public BaseList<BotA_TP, A> {
};

struct A_strings {
    static const char name[];
    static const char f_f1[];
};

template <>
class Members<A>
 : public MemberList<
     A,
     A_strings::name,
     DataMemberDcl<A, FundamentalType_tmpl<int>, &A::f1, A_strings::f_f1>
     >
{};



} // namespace



#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
