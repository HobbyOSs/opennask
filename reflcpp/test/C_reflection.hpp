#ifndef REFLCPP_C_H
#define REFLCPP_C_H



#include <reflcpp/ClassType_tmpl.hpp>
#include <reflcpp/FundamentalType.hpp>
#include <reflcpp/MemberFunctionDcl.hpp>
#include "C.hpp"



namespace reflcpp {



template <typename BotC_TP, typename Der_TP, int N>
class Bases<BotC_TP, Der_TP, C, N> : public BaseList<BotC_TP, C> {
};



struct C_strings {
    static const char name[];
    static const char f_cf1[];
};

template <>
class Members<C>
 : public MemberList<
     C,
     C_strings::name,
     DataMemberDcl<C, FundamentalType_tmpl<int>, &C::cf1, C_strings::f_cf1>
     >
{};



} // namespace



#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
