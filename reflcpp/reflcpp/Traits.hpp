/******************************************************************************\
*                                                                              *
*   Traits.hpp                                                                 *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: Traits.hpp,v 1.1 2007/10/25 14:05:26 tdevadit Exp $



#ifndef REFLCPP_TRAITS_HPP
#define REFLCPP_TRAITS_HPP


namespace reflcpp {


/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   BoundMemberFunction_body                                                   |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    Conveniently handle references and type qualifiers.
    (Ref: C++ Templates, The complete guide)

-                                                                             */

// Primary template
template <typename T>
class TypeOp {
public:
    typedef T ArgT;
    typedef T BareT;
    typedef T & RefT;
};
// Partial specialization for const types
template <typename T>
class TypeOp<T const> {
public:
    typedef T const ArgT;
    typedef T BareT;
    typedef T const & RefT;
};
// Partial specialization for references
template <typename T>
class TypeOp<T&> {
public:
    typedef T & ArgT;
    typedef typename TypeOp<T>::BareT BareT;
    typedef T & RefT;
};



static const char TypeOp_hpp_id[] =
"$Id: Traits.hpp,v 1.1 2007/10/25 14:05:26 tdevadit Exp $";


} // namespace


#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
