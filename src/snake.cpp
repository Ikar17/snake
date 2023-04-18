#include "snake.h"
#include "screen.h"
#include "cpoint.h"
#include "winsys.h"
#include <unistd.h>
#include <cstdlib>
#include <ctime>

CSnake::CSnake(CRect r, char _c /*=' '*/):
  CFramedWindow(r, _c), headSnake(CPoint(6,2)), tailSnake(CPoint(4,2)), fruit(CPoint(5,5)), directSnake(1), dead(false), help(true),level(0)
{
    srand(time(NULL));
    bodySnake.push_back(headSnake);
    bodySnake.push_back(CPoint(5,2));
    bodySnake.push_back(tailSnake);
}

void CSnake::paint(){
    CFramedWindow::paint();

    //rysowanie weza
    for (long unsigned int i=0; i<this->bodySnake.size(); ++i){
        int x = this->bodySnake[i].x;
        int y = this->bodySnake[i].y;
        gotoyx(geom.topleft.y+y, geom.topleft.x+x);
        if(i==0){
            printl("*");
        }else{
            printl("+");
        }
    }

    //rysowanie owocu
    int x = this->fruit.x;
    int y = this->fruit.y;
    gotoyx(geom.topleft.y+y, geom.topleft.x+x);
    printl("0");

    //rysowanie wyniku
    gotoyx(geom.topleft.y-1, geom.topleft.x);
    printl("|");
    gotoyx(geom.topleft.y-1, geom.topleft.x+11);
    printl("|");
    for(int i=1; i<11;++i){
        gotoyx(geom.topleft.y-1, geom.topleft.x+i);
        printl(" ");
    }
    gotoyx(geom.topleft.y-1, geom.topleft.x+2);
    printl("Level: %d", this->level);


    //rysowanie instrukcji
    if(this->help){
        gotoyx(geom.topleft.y+5, geom.topleft.x+7);
        printl("h - toggle help information");
        gotoyx(geom.topleft.y+6, geom.topleft.x+7);
        printl("p - toggle pause/play mode");
        gotoyx(geom.topleft.y+7, geom.topleft.x+7);
        printl("r - restart game");
        gotoyx(geom.topleft.y+8, geom.topleft.x+7);
        printl("arrows - move snake (in play mode)");
        gotoyx(geom.topleft.y+9, geom.topleft.x+16);
        printl("move window (in pause mode)");
    }

    //rysowanie napisu w przypadku konca gry
    if(this->dead){
        gotoyx(geom.topleft.y+1, geom.topleft.x+1);
        printl("GAME OVER, result: %d", this->level);
    }

}

bool CSnake::handleEvent(int c)
{
    if(c=='h'){
        this->help = !this->help;
        this->paint();
        refresh();
    }

    if(c=='p'){
        if(this->dead){
            resetGame();
            this->dead = false;
        }

        this->help = false;

        while(1){
             usleep(500000 - this->level * 25000);
            //pobieram kierunek i zmieniam pozycja weza
            int move = ngetch();
            switch (move)
            {
                case KEY_UP:
                    if(this->directSnake != 2){
                        moveUp();
                    }
                    break;
                case KEY_DOWN:
                    if(this->directSnake != 0){
                        moveDown();
                    }
                    break;
                case KEY_RIGHT:
                    if(this->directSnake != 3){
                        moveRight();
                    }
                    break;
                case KEY_LEFT:
                    if(this->directSnake != 1){
                        moveLeft();
                    }
                    break;
                case (int)'p':
                    return true;
                case (int)'h':
                    this->help = !this->help;
                    break;
                case (int)'r':
                    this->resetGame();
                    break;
                default:
                    break;
            };

            //wyswietlam zmiany
            update_screen();
            this->paint();
            refresh();

            //sprawdzam czy koniec gry
            if(this->gameOver()) return true;

            //zmieniam pozycje weza zgodnie z kierunkiem poruszania sie
            this->move();

            //wyswietlam zmiany
            this->paint();
            refresh();

            //sprawdzam czy koniec gry
            if(this->gameOver()) return true;
        }
    }else{
        return CFramedWindow::handleEvent(c);
    }
}

void CSnake::move(){
    switch(this->directSnake){
        case 0:
            moveUp();
            break;
        case 1:
            moveRight();
            break;
        case 2:
            moveDown();
            break;
        case 3:
            moveLeft();
            break;
        default:
            break;
    }
}
void CSnake::moveUp() {


    for (long unsigned int i=this->bodySnake.size()-1; i>0; --i){
        //aktualizacja wspolrzednych kazdego punktu w liscie punktow
        this->bodySnake[i] = this->bodySnake[i-1];
    }
    if(this->bodySnake[0].y-1 == 0){
        //przejscie na druga strone
        this->bodySnake[0].y = geom.size.y -2 ;
    }else{
        this->bodySnake[0].y -=1;
    }

    //sprawdzenie czy trafil na owoc
    if(this->getFruit()){
        //wydluzanie weza
        this->insertSnake(this->tailSnake);
        //ustawianie nowego owocu
        this->setFruit();
    }

    //aktualizacja ogonu
    this->tailSnake = this->bodySnake[this->bodySnake.size() - 1];

    //aktualizacja kierunku przemieszczania sie weza
    this->directSnake = 0;

}
void CSnake::moveDown() {


    for (long unsigned int i=this->bodySnake.size()-1; i>0; --i){
        //aktualizacja wspolrzednych kazdego punktu w liscie punktow
        this->bodySnake[i] = this->bodySnake[i-1];
    }
    if(this->bodySnake[0].y+1 == geom.size.y-1){
        //przejscie na druga strone
        this->bodySnake[0].y = 1;
    }else{
        this->bodySnake[0].y +=1;
    }

    //sprawdzenie czy trafil na owoc
    if(this->getFruit()){
        //wydluzanie weza
        this->insertSnake(this->tailSnake);
        //ustawianie nowego owocu
        this->setFruit();
    }

    //aktualizacja ogonu
    this->tailSnake = this->bodySnake[this->bodySnake.size() - 1];

    //aktualizacja kierunku przemieszczania sie weza
    this->directSnake = 2;

}
void CSnake::moveRight() {


    for (long unsigned int i=this->bodySnake.size()-1; i>0; --i){
        //aktualizacja wspolrzednych kazdego punktu w liscie punktow
        this->bodySnake[i] = this->bodySnake[i-1];
    }
    if(this->bodySnake[0].x+1 == geom.size.x-1){
        //przejscie na druga strone
        this->bodySnake[0].x = 1;
    }else{
        this->bodySnake[0].x +=1;
    }

    //sprawdzenie czy trafil na owoc
    if(this->getFruit()){
        //wydluzanie weza
        this->insertSnake(this->tailSnake);
        //ustawianie nowego owocu
        this->setFruit();
    }

    //aktualizacja ogonu
    this->tailSnake = this->bodySnake[this->bodySnake.size() - 1];

    //aktualizacja kierunku przemieszczania sie weza
    this->directSnake = 1;


}
void CSnake::moveLeft() {

    for (long unsigned int i=this->bodySnake.size()-1; i>0; --i){
        //aktualizacja wspolrzednych kazdego punktu w liscie punktow
        this->bodySnake[i] = this->bodySnake[i-1];
    }
    if(this->bodySnake[0].x-1 == 0){
        //przejscie na druga strone
        this->bodySnake[0].x = geom.size.x-2;
    }else{
        this->bodySnake[0].x -=1;
    }

    //sprawdzenie czy trafil na owoc
    if(this->getFruit()){
        //wydluzanie weza
        this->insertSnake(this->tailSnake);
        //ustawianie nowego owocu
        this->setFruit();
    }

    //aktualizacja ogonu
    this->tailSnake = this->bodySnake[this->bodySnake.size() - 1];

    //aktualizacja kierunku przemieszczania sie weza
    this->directSnake = 3;

}

bool CSnake::getFruit() {
    if(this->bodySnake[0].x == this->fruit.x && this->bodySnake[0].y == this->fruit.y){
        this->level +=1;
        return true;
    }else return false;
}

void CSnake::insertSnake(CPoint tail) {
    this->bodySnake.push_back(tail);
}

void CSnake::setFruit() {
    this->fruit.x = (rand() % (geom.size.x -3)) +1;
    this->fruit.y = (rand() % (geom.size.y - 3)) +1;
}

bool CSnake::gameOver() {
    for(long unsigned int i=1; i<this->bodySnake.size(); ++i){
        if(this->bodySnake[i].x == this->bodySnake[0].x && this->bodySnake[i].y == this->bodySnake[0].y){
            this->dead=true;
            return true;
        }
    }
    return false;
}

void CSnake::resetGame(){
    while(this->bodySnake.size() > 3){
        this->bodySnake.pop_back();
    }
    this->level = 0;
    this->setFruit();
}