/******************************************************************************\
*                                                                              *
*   @.h                                                                        *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: RetValHolder.hpp,v 1.1 2007/10/25 14:05:26 tdevadit Exp $



#ifndef REFLCPP_RET_VAL_HOLDER_HPP
#define REFLCPP_RET_VAL_HOLDER_HPP




namespace reflcpp {


/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   RetValHolder                                                               |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    Class to handle return values with partial specialization for void functions.

-                                                                             */

class RetValHolder {

    public:
        virtual ~RetValHolder() {}

        // In order to make this polymorphic
        virtual void dummy() = 0;
};


template <typename Ret_TP>
class RetValHolder_tmpl : public RetValHolder {

    public:
        virtual ~RetValHolder_tmpl() {}

        Ret_TP val() { return m_val; }
        Ret_TP &ref() { return m_val; }
        virtual void dummy() {}

    private:

        Ret_TP m_val;
};


template <>
class RetValHolder_tmpl <void> : public RetValHolder {

    public:

        void val() {}
        virtual void dummy() {}
};


static const char RetValHolder_h_id[] =
"$Id: RetValHolder.hpp,v 1.1 2007/10/25 14:05:26 tdevadit Exp $";



} // namespace



#endif
