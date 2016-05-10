/* ParaDefs.cc */
/* Created by Enomoto Sanshiro on 27 September 1998. */
/* Last updated by Enomoto Sanshiro on 25 June 1999. */


#include <iostream>
#include <string>
#include "ParaDefs.hh"

using namespace std;


TScriptException::TScriptException(string Message)
{
    _Message = Message;
}

TScriptException::TScriptException(string Location, string Message)
{
    _Message = Location + ": " + Message;
}

TScriptException::TScriptException(const TScriptException &e)
{
    _Message = e._Message;
}

string TScriptException::Message(void) const
{
    return _Message;
}

ostream& operator<< (ostream &os, const TScriptException &e)
{
    os << e._Message;
    return os;
}

