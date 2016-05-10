/* Canvas.hh */
/* Created by Enomoto Sanshiro on 19 October 2001. */
/* Last updated by Enomoto Sanshiro on 19 October 2001. */


#ifndef __CANVAS_HH
#define __CANVAS_HH

#include <string>


class TCanvas {
  public:
    TCanvas(void);
    virtual ~TCanvas();
    virtual void DrawLine(float X0, float Y0, float X1, float Y1);
    virtual void DrawRect(float X0, float Y0, float X1, float Y1);
    virtual void DrawCircle(float X, float Y, float Radius);
    virtual void DrawText(float X, float Y, const std::string& Text);
};


#endif
