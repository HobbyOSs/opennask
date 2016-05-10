/* MacroDrawParser.hh */
/* Created by Enomoto Sanshiro on 19 October 2001. */
/* Last updated by Enomoto Sanshiro on 19 October 2001. */


#ifndef __MACRO_DRAW_PARSER_HH
#define __MACRO_DRAW_PARSER_HH


#include "Canvas.hh"
#include "CanvasMessenger.hh"
#include "ParaParser.hh"


class TMacroDrawParser: public TParaStandardParser {
  public:
    TMacroDrawParser(TCanvas* Canvas);
    virtual ~TMacroDrawParser();
  protected:
    virtual TParaObjectPrototypeTable* CreateObjectPrototypeTable(void);
    virtual TParaBuiltinFunctionTable* CreateBuiltinFunctionTable(void);
  private:
    TCanvas* _Canvas;
    TCanvasMessenger* _CanvasMessenger;
};


#endif
