/******************************************************************************\
*                                                                              *
*   @.h                                                                        *
*                                                                              *
*   <Purpose of this file>                                                     *
*                                                                              *
\******************************************************************************/

// $Id: Exceptions.hpp,v 1.1 2007/10/25 14:05:24 tdevadit Exp $


#ifndef REFLCPP_EXCEPTIONS_HPP
#define REFLCPP_EXCEPTIONS_HPP



#include <stdexcept>
#include <string>
#include <sstream>


#define Throw(ex)  throw ex.setLocation(__FILE__, __LINE__)



namespace reflcpp {



/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   ReflectionException                                                        |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    Base class for exceptions generated from this library.

-                                                                             */

class ReflectionException : public std::runtime_error {

    public:

        ReflectionException() : std::runtime_error(""), m_message("") { }
        virtual ~ReflectionException() throw() {}
    
        virtual const char* what() const throw() { return m_message.c_str(); }

        ReflectionException& setLocation(const std::string &file, const int line) {
            std::ostringstream oss;
            oss << " (" << file << ":" << line << ")";
            m_message += oss.str();
            return *this;
        }

    protected:
        std::string m_message;
};


/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   NoSuchDataMemberException                                              |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    Thrown when the data member does not exist.

-                                                                             */

class NoSuchDataMemberException : public ReflectionException {

    public:

        NoSuchDataMemberException(const std::string& typeName, const std::string& dataMemberName) 
            : ReflectionException(), m_typeName(typeName), m_dataMemberName(dataMemberName) {
            m_message = m_dataMemberName + " does not exist in " + m_typeName;
        }
        virtual ~NoSuchDataMemberException() throw() {}

        std::string getTypeName() { return m_typeName; }
        std::string getDataMemberName() { return m_dataMemberName; }
    
    protected:
        std::string m_typeName;
        std::string m_dataMemberName;
};


/*-----------------------------------------------------------------------------\
|------------------------------------------------------------------------------|
|                                                                              |
|   NoSuchMemberFunctionException                                              |
|                                                                              |
|------------------------------------------------------------------------------|
\------------------------------------------------------------------------------/

    Thrown when the member function does not exist.

-                                                                             */

class NoSuchMemberFunctionException : public ReflectionException {

    public:

        NoSuchMemberFunctionException(const std::string& typeName, const std::string& memberFunctionName) 
            : ReflectionException(), m_typeName(typeName), m_memberFunctionName(memberFunctionName) {
            m_message = m_memberFunctionName + " does not exist in " + m_typeName;
        }
        virtual ~NoSuchMemberFunctionException() throw() {}
    
        std::string getTypeName() { return m_typeName; }
        std::string getMemberFunctionName() { return m_memberFunctionName; }
    
    protected:
        std::string m_typeName;
        std::string m_memberFunctionName;
};



static const char Exceptions_hpp_id[] =
"$Id: Exceptions.hpp,v 1.1 2007/10/25 14:05:24 tdevadit Exp $";



} // namespace



#endif



/* Local Variables: */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4 expandtab: */
