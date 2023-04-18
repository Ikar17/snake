#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"
#include "cpoint.h"
#include <vector>

class CSnake:public CFramedWindow
{
private:
    CPoint headSnake;
    CPoint tailSnake;
    CPoint fruit;
    vector <CPoint> bodySnake;
    int directSnake; //0 - gora, 1-prawo, 2- dol, 3 - lewo
    bool dead;
    bool help;
    int level;
public:
  CSnake(CRect r, char _c = ' ');
  void paint();
  bool handleEvent(int c);
  void move();
  void moveUp();
  void moveDown();
  void moveRight();
  void moveLeft();
  bool getFruit();
  void setFruit();
  void insertSnake(CPoint );
  bool gameOver();
  void resetGame();
};

#endif
