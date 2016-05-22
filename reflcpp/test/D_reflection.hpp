#ifndef REFLCPP_D_HPP
#define REFLCPP_D_HPP



#include <reflcpp/ClassType_tmpl.hpp>
#include <reflcpp/FundamentalType.hpp>
#include <reflcpp/MemberFunctionDcl.hpp>

#include "D.hpp"
#include "C_reflection.hpp"



namespace reflcpp {



template <typename BotD_TP, typename Der_TP, int N>
class Bases<BotD_TP, Der_TP, D, N> : public BaseList<BotD_TP, D> {
};

struct D_strings {
    static const char name[];
    static const char m_m0[];
    static const char m_m0_void[];
    static const char m_m1[];
    static const char m_m1_void[];
    static const char m_m2[];
    static const char m_m1_c[];
    static const char m_m1_ref_c[];
};

template <>
class Members<D>
 : public MemberList<
     D,
     D_strings::name,
     MemberFunctionDcl000<D, FundamentalType_tmpl<int>, &D::m0, D_strings::m_m0>,
     MemberFunctionDcl000<D, FundamentalType_tmpl<void>, &D::m0_void, D_strings::m_m0_void>,
     MemberFunctionDcl001<D, FundamentalType_tmpl<int>, FundamentalType_tmpl<int>, &D::m1, D_strings::m_m1>,
     MemberFunctionDcl001<D, FundamentalType_tmpl<void>, FundamentalType_tmpl<int>, &D::m1_void, D_strings::m_m1_void>,
     MemberFunctionDcl002<D, FundamentalType_tmpl<int>, FundamentalType_tmpl<int>, FundamentalType_tmpl<int>, &D::m2, D_strings::m_m2>,
     MemberFunctionDcl001<D, ClassType_tmpl<C>, ClassType_tmpl<C>, &D::m1_c, D_strings::m_m1_c>,
     MemberFunctionDcl001<D, FundamentalType_tmpl<void>, ClassType_tmpl<C&>, &D::m1_ref_c, D_strings::m_m1_ref_c>
     >
{};

} // namespace 



#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
