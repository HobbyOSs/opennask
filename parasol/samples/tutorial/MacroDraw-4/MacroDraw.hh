/* MacroDraw.hh */
/* Created by Enomoto Sanshiro on 19 October 2001. */
/* Last updated by Enomoto Sanshiro on 19 October 2001. */


#ifndef __MACRO_DRAW_HH
#define __MACRO_DRAW_HH


#include <string>
#include "Canvas.hh"
#include "ParaParser.hh"


class TMacroDraw {
  public:
    TMacroDraw(void);
    virtual ~TMacroDraw();
    virtual void Start(int argc, char** argv);
  protected:
    virtual void ExecuteMacro(const std::string& FileName, const std::string& MacroName) throw(TScriptException);
  private:
    TCanvas* _Canvas;
};


#endif
