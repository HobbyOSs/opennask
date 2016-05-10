/* Canvas.cc */
/* Created by Enomoto Sanshiro on 19 October 2001. */
/* Last updated by Enomoto Sanshiro on 19 October 2001. */


#include <iostream>
#include <string>
#include "Canvas.hh"

using namespace std;


TCanvas::TCanvas(void)
{
    cout << ".create picture p 0 0 100 100;" << endl;
    cout << "p rect 2 2 98 98;" << endl;
}

TCanvas::~TCanvas()
{
}

void TCanvas::DrawLine(float X0, float Y0, float X1, float Y1)
{
    cout << "p line ";
    cout << X0 << " " << Y0 << " " << X1 << " " << Y1;
    cout << ";" << endl;
}

void TCanvas::DrawRect(float X0, float Y0, float X1, float Y1)
{
    cout << "p rect ";
    cout << X0 << " " << Y0 << " " << X1 << " " << Y1;
    cout << ";" << endl;
}

void TCanvas::DrawCircle(float X, float Y, float Radius)
{
    cout << "p circle ";
    cout << X << " " << Y << " " << Radius;
    cout << ";" << endl;
}

void TCanvas::DrawText(float X, float Y, const string& Text)
{
    cout << "p text ";
    cout << X << " " << Y << " " << Text;
    cout << ";" << endl;
}
