/* CanvasMessenger.hh */
/* Created by Enomoto Sanshiro on 19 October 2001. */
/* Last updated by Enomoto Sanshiro on 19 October 2001. */


#ifndef __CANVAS_MESSENGER_HH
#define __CANVAS_MESSENGER_HH


#include "Canvas.hh"
#include "ParaObject.hh"


class TCanvasMessenger: public TParaObjectPrototype {
  public:
    TCanvasMessenger(TCanvas* Canvas);
    virtual ~TCanvasMessenger();
    virtual TParaObjectPrototype* Clone(void);
    virtual void Construct(const std::string& ClassName, std::vector<TParaValue*>& ArgumentList) throw(TScriptException);
    virtual int DispatchMessage(const std::string& Message, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException);
  protected:
    TParaValue DrawLine(std::vector<TParaValue*>& ArgumentList) throw(TScriptException);
    TParaValue DrawRect(std::vector<TParaValue*>& ArgumentList) throw(TScriptException);
    TParaValue DrawCircle(std::vector<TParaValue*>& ArgumentList) throw(TScriptException);
    TParaValue DrawText(std::vector<TParaValue*>& ArgumentList) throw(TScriptException);
  private:
    TCanvas* _Canvas;
};


#endif
