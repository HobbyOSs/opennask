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
    virtual void Construct(const std::string& ClassName, std::vector<TParaValue*>& ArgumentList) noexcept(false);
    virtual int DispatchMessage(const std::string& Message, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
  protected:
    TParaValue DrawLine(std::vector<TParaValue*>& ArgumentList) noexcept(false);
    TParaValue DrawRect(std::vector<TParaValue*>& ArgumentList) noexcept(false);
    TParaValue DrawCircle(std::vector<TParaValue*>& ArgumentList) noexcept(false);
    TParaValue DrawText(std::vector<TParaValue*>& ArgumentList) noexcept(false);
  private:
    TCanvas* _Canvas;
};


#endif
