#ifndef __CPOINT_H__
#define __CPOINT_H__

#include <list>

using namespace std;

struct CPoint
{
  int x;
  int y;
  int direct; //wykorzystywany do snake'a, kolejny punkt na liscie punktów: 0 - jest u gora wzgledem tego punktu, 1- ..ns prawo, 2- .. u dolu, 3-..po lewo
  CPoint(int _x=0, int _y=0, int _direct=-1): x(_x), y(_y), direct(_direct) {};
  CPoint& operator+=(const CPoint& delta)
  {
    x+=delta.x;
    y+=delta.y;
    return *this;
  }

  CPoint& operator=(CPoint& delta)
  {
    x=delta.x;
    y=delta.y;
    return *this;
  }

};

struct CRect
{
  CPoint topleft;
  CPoint size;
  CRect(CPoint t1=CPoint(), CPoint s=CPoint()):
    topleft(t1), size(s) {};
};

#endif
