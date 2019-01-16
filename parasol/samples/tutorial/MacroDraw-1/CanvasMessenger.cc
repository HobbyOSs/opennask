/* CanvasMessenger.cc */
/* Created by Enomoto Sanshiro on 19 October 2001. */
/* Last updated by Enomoto Sanshiro on 19 October 2001. */


#include "Canvas.hh"
#include "ParaObject.hh"
#include "CanvasMessenger.hh"

using namespace std;


TCanvasMessenger::TCanvasMessenger(TCanvas* Canvas)
: TParaObjectPrototype("Canvas")
{
    _Canvas = Canvas;
}

TCanvasMessenger::~TCanvasMessenger()
{
}

TParaObjectPrototype* TCanvasMessenger::Clone(void)
{
    return new TCanvasMessenger(_Canvas);
}

void TCanvasMessenger::Construct(const string& ClassName, vector<TParaValue*>& ArgumentList) noexcept(false)
{
}

int TCanvasMessenger::DispatchMessage(const string& Message, vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false)
{
    if (Message == "drawLine") {
        ReturnValue = DrawLine(ArgumentList);
    }
    else if (Message == "drawRect") {
        ReturnValue = DrawRect(ArgumentList);
    }
    else if (Message == "drawCircle") {
        ReturnValue = DrawCircle(ArgumentList);
    }
    else if (Message == "drawText") {
        ReturnValue = DrawText(ArgumentList);
    }
    else {
	return 0;
    }

    return 1;
}

TParaValue TCanvasMessenger::DrawLine(vector<TParaValue*>& ArgumentList) noexcept(false)
{
    if (ArgumentList.size() < 4) {
	throw TScriptException("Canvas::drawLine(): too few arguments");
    }

    float X0 = ArgumentList[0]->AsDouble();
    float Y0 = ArgumentList[1]->AsDouble();
    float X1 = ArgumentList[2]->AsDouble();
    float Y1 = ArgumentList[3]->AsDouble();

    _Canvas->DrawLine(X0, Y0, X1, Y1);

    return TParaValue((long) 0);
}

TParaValue TCanvasMessenger::DrawRect(vector<TParaValue*>& ArgumentList) noexcept(false)
{
    if (ArgumentList.size() < 4) {
	throw TScriptException("Canvas::drawRect(): too few arguments");
    }

    float X0 = ArgumentList[0]->AsDouble();
    float Y0 = ArgumentList[1]->AsDouble();
    float X1 = ArgumentList[2]->AsDouble();
    float Y1 = ArgumentList[3]->AsDouble();

    _Canvas->DrawRect(X0, Y0, X1, Y1);

    return TParaValue((long) 0);
}

TParaValue TCanvasMessenger::DrawCircle(vector<TParaValue*>& ArgumentList) noexcept(false)
{
    if (ArgumentList.size() < 3) {
	throw TScriptException("Canvas::drawCircle(): too few arguments");
    }

    float X0 = ArgumentList[0]->AsDouble();
    float Y0 = ArgumentList[1]->AsDouble();
    float Radius = ArgumentList[2]->AsDouble();

    _Canvas->DrawCircle(X0, Y0, Radius);

    return TParaValue((long) 0);
}

TParaValue TCanvasMessenger::DrawText(vector<TParaValue*>& ArgumentList) noexcept(false)
{
    if (ArgumentList.size() < 3) {
	throw TScriptException("Canvas::drawText(): too few arguments");
    }

    float X0 = ArgumentList[0]->AsDouble();
    float Y0 = ArgumentList[1]->AsDouble();
    string Text = ArgumentList[2]->AsString();

    _Canvas->DrawText(X0, Y0, Text);

    return TParaValue((long) 0);
}
