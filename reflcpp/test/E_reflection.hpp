#ifndef REFLCPP_E_HPP
#define REFLCPP_E_HPP



#include <reflcpp/ClassType_tmpl.hpp>
#include <reflcpp/FundamentalType.hpp>
#include <reflcpp/MemberFunctionDcl.hpp>
#include "E.hpp"



namespace reflcpp {



template <typename BotD_TP, typename Der_TP, int N>
class Bases<BotD_TP, Der_TP, E, N> : public BaseList<BotD_TP, E> {
};

struct E_strings {
    static const char name[];
    static const char m_m0[];
};

template <>
class Members<E>
 : public MemberList<
     E,
     E_strings::name,
     MemberFunctionDcl000<E, FundamentalType_tmpl<int>, &E::m0, E_strings::m_m0>
     >
{};

} // namespace 



#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
