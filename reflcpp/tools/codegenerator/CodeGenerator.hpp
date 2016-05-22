// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <string>


//---------------------------------------------------------------------------
//  This is used by cpp-reflection to discribe the field of a class
//---------------------------------------------------------------------------
class Xct_Field
{
public:
    std::string type;
    std::string name;
    std::string access;
};

class Xct_Method
{
public:
    // pair of argname and argtype
    std::vector<std::string> arg_types;
    std::string ret_type;
    std::string name;
    std::string access;
};


class Xct_Class
{
public:
    std::string name;
    std::string baseClass;
    std::vector<Xct_Field*> fields;
    std::vector<Xct_Method*> methods;
};

