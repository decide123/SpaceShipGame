#include "TXlib.h"
//#include "marioEngine2.h"

#define ASSERT(something)  if(something == 0)																			  \
								{																						  \
								printf("error on line %d in file %s while checking %s\n", __LINE__, __FILE__, #something);\
								assert(0);																				  \
								}

struct SpaceShip
    {
    double  x;
    double  y;
    double vx;
    double vy;
    double ax;
    double ay;
    double  m;

    int  controls(int up, int down, int left, int right);
    void physics(double DT);
    void draw();
    int bounce();
    };

void GameProcess(SpaceShip spaceship);


int main()                                                 // F = ma -> a = F/m;
    {
    txCreateWindow(800, 600);

    SpaceShip spaceship{300, 100, 0, 10, 0, 0.3, 100};

    GameProcess(spaceship);
    }

void GameProcess(SpaceShip spaceship)
    {
    double DT = 1;

    while(!GetAsyncKeyState(VK_ESCAPE))
        {
        txSetFillColor(RGB(0, 0, 0));
        txClear();

        spaceship.controls(VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT);
        spaceship.physics(DT);
        spaceship.draw();

        if(spaceship.bounce() == 0)
            {
            txMessageBox("GAME OVER! YOU LOSE!");
            break;
            }

        txSleep(40);                                                            // F = ma -> a = F/m;
        }
     }


int SpaceShip::controls(int up, int down, int left, int right)
    {
    if(m <= 1)
        {
        vx = 0;
        return 0;
        }
                                                              // F = ma -> a = F/m;
    if(GetAsyncKeyState(up))
        {
        vy -= 50/(m);
        m  -=     0.2;
        }

    if(GetAsyncKeyState(down))
        {
        vy += 50/(m);
        m  -=    0.2;
        }

    if(GetAsyncKeyState(left))
        {
        vx -= 20/(m);
        m  -=    0.2;
        }

    if(GetAsyncKeyState(right))
        {
        vx += 20/(m);
        m  -=    0.2;
        }

    printf("m = %+4.1lf, vy = %+4.1lf  \r",  m, vy);
    return 1;
    }


int SpaceShip::bounce()
    {
    if(x <= 0 + 50)
        {
        vx = -vx;
        x = 0 + 50;
        }

    if(x >= 800 - 50)
        {
        vx = -vx;
        x = 800 - 50;
        }

    if(y <= 0 + 50)
        {
        vy = -vy;
        y = 0 + 50;
        }

    if(y >= 600 - 50)
        {
        if(vy > 2)
            {
            return 0;
            }

        vy =        0;
        y  = 600 - 50;
        }

    return 1;
    }




void SpaceShip::physics(double DT)                                        // Заголовок()
    {                                                                                           //   {
    vx += ax * DT;                                                                              //
    vy += ay * DT;                                                                              //                      Заголовок + тело = определение
    x  += vx * DT;                                                                              //    Тело
    y  += vy * DT;                                                                              //
    }                                                                                           //   }

void SpaceShip::draw()
    {
    txSetFillColor(RGB(255, 255, 255));
    txSetColor    (RGB(255, 255, 255));
    txCircle      (x, y, 50);
    }
