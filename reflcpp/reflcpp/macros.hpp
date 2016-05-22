/******************************************************************************\
*                                                                              *
*   @.h                                                                        *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: macros.hpp,v 1.1 2007/10/25 14:05:27 tdevadit Exp $



#ifndef REFLCPP_MACROS_H
#define REFLCPP_MACROS_H



#include <reflcpp/check.hpp>



namespace reflcpp {



/*------------------------------------------------------------------------------

   min/max                                                                    */

template <class T1, class T2>
inline T1
min(T1 x1, T2 x2) {

    return x1 < x2 ? x1 : x2;
}

template <class T1, class T2, class T3>
inline T1
min(T1 x1, T2 x2, T3 x3) {

    return  x1 < x2 ? (x1 < x3 ? x1 : x3) : (x2 < x3 ? x2 : x3);
}

template <class T1, class T2>
inline T1
max(T1 x1, T2 x2) {

    return x1 > x2 ? x1 : x2;
}

template <class T1, class T2, class T3>
inline T1
max(T1 x1, T2 x2, T3 x3) {

    return x1 > x2 ? (x1 > x3 ? x1 : x3) : (x2 > x3 ? x2 : x3);
}



/*------------------------------------------------------------------------------

   sign/abs                                                                   */

template <class T>
inline int
sign(T x) {

    return x > 0 ? 1 : x < 0 ? -1 : 0;
}

template <class T>
inline T
abs(T x) {

    return x < 0 ? -x : x;
}



/*------------------------------------------------------------------------------

   swap                                                                       */

template <class T1, class T2>
inline void
swap(T1 &x1, T2 &x2) {

    T1 t = x1;

    x1 = x2;
    x2 = t;
}



/*------------------------------------------------------------------------------

   Range                                                                      */

template <class T1, class T2, class T3>
inline bool
withinii(T1 x0, T2 x, T3 x1) {
    return x0 <= x && x <= x1;
}

template <class T1, class T2, class T3>
inline bool
withinie(T1 x0, T2 x, T3 x1) {
    return x0 <= x && x < x1;
}

template <class T1, class T2, class T3>
inline bool
withinei(T1 x0, T2 x, T3 x1) {
    return x0 < x && x <= x1;
}

template <class T1, class T2, class T3>
inline bool
withinee(T1 x0, T2 x, T3 x1) {
    return x0 < x && x < x1;
}



/*------------------------------------------------------------------------------

   Macros                                                                     */

template <typename T>
inline T &
ncr(const T &v) { return const_cast<T &>(v); }

#define IXCR_NCR(T, N, V)		T &N(const_cast<T &>(V))
#define IXCR_NCT(T)               T *nct = const_cast<T *>(this)
#define IXCR_DCL(T, V, S)        T V = (T) (S)
#define IXCR_DCDCL(T, V, S)          T V = dynamic_cast<T>(S)
#define IXCR_N_ELEMS(a)           (sizeof(a)/sizeof((a)[0]))
// This is used when we want a something to have static storage, but we
// need to initialize it dynamically.  It defines some storage, then
// defines a reference to it.  Use placement new to initialize the
// object, and an explicit dtor call to destruct it.
#define IXCR_DEF_BUF(t, n)                                                      \
    msl::uint64                                                                \
     n##_buf[(sizeof(t) + (sizeof(msl::uint64) - 1))/sizeof(msl::uint64)];     \
    t &n = *(t *) n##_buf



static const char macros_h_id[] =
"$Id: macros.hpp,v 1.1 2007/10/25 14:05:27 tdevadit Exp $";



} // namespace



#endif
