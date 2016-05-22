#ifndef REFLCPP_B_H
#define REFLCPP_B_H



#include <reflcpp/ArrayType.hpp>
#include <reflcpp/ClassType_tmpl.hpp>
#include <reflcpp/FundamentalType.hpp>
#include <reflcpp/PointerType.hpp>
#include <reflcpp/MemberFunctionDcl.hpp>

#include "B.hpp"
#include "A_reflection.hpp"
#include "C_reflection.hpp"



namespace reflcpp {



template <typename BotB_TP, typename Der_TP, int N>
class Bases<BotB_TP, Der_TP, B, N> : public BaseList<BotB_TP, B, A> {
};



struct B_strings {
    static const char name[];
    static const char f_f1[];
    static const char f_array[];
    static const char f_array_size[];
    static const char f_array2[];
    static const char f_c[];
};

template <>
class Members<B>
 : public MemberList<
     B,
     B_strings::name
     , DataMemberDcl<B, FundamentalType_tmpl<int>, &B::f1, B_strings::f_f1>
     , DataMemberDcl<B, PointerType_tmpl<FundamentalType_tmpl<float> >, &B::array, B_strings::f_array>
     , DataMemberDcl<B, FundamentalType_tmpl<int>, &B::array_size, B_strings::f_array_size>
     , DataMemberDcl<B, ArrayType_tmpl<FundamentalType_tmpl<double>, 10>, &B::array2, B_strings::f_array2>
     , DataMemberDcl<B, ClassType_tmpl<C>, &B::c, B_strings::f_c>
     >
{};



} // namespace



#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
