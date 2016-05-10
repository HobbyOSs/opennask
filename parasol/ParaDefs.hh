/* ParaDefs.hh */
/* Created by Enomoto Sanshiro on 27 September 1998. */
/* Last updated by Enomoto Sanshiro on 25 June 1999. */


#ifndef __ParaDefs_hh__
#define __ParaDefs_hh__


#include <iostream>
#include <string>


class TScriptException {
  public:
    TScriptException(std::string Message);
    TScriptException(std::string Location, std::string Message);
    TScriptException(const TScriptException &e);
    std::string Message(void) const;
    friend std::ostream& operator<< (std::ostream &os, const TScriptException &e);
  protected:
    std::string _Message;
};


#endif
