#include "TXlib.h"
//#include "marioEngine2.h"

#define ASSERT(something)  if(something == 0)																			  \
								{																						  \
								printf("error on line %d in file %s while checking %s\n",                                 \
								__LINE__, __FILE__, #something);                                                          \
								assert(0);																				  \
								}

const COLORREF GROUNDCOLOR = RGB(255, 0, 0);
const COLORREF FLYCOLOR = RGB(0, 0, 255);
const COLORREF UNSTABLEZONECOLOR = RGB(0, 255, 0);
const int AnimationCLOCK = 9;

enum ZONE
    {
    UNDEFINEDZONE = 0,
    FLIGHTZONE = 1,
    GROUNDZONE = 2,
    UNSTABLEZONE = 3
    };

enum STATE
    {
    LOSE = 0,
    WIN = 1,
    CONTINUE = 2,
    EMERGENCYBREAK = 3
    };

enum CHOOSEANIMATION
    {
    STNDRD = 0,
    BLOW = 1
    };

class Vector
    {
    public:
    double x;
    double y;

    Vector () :
        x(0),
        y(0)
        {}

    Vector (double anX, double anY) :
        x(anX),
        y(anY)
        {}
    };

class Character
    {
    public:
    Vector pos;                        //operator() (int x);   b() ();
    Vector   v;
    Vector   a;
    double   m;
    int      t;
    HDC    pic;
    int animation;

    void physics(double DT);
    void draw(int t);
    int bounce();
    ZONE pixel();
    void setAnimation(int newAnimation);



    Character (Vector aPos, Vector aV, Vector anA, double aM, HDC aPIC) :
        pos(pos),
        v(aV),
        a(anA),
        m(aM),
        t(0),
        pic(aPIC),
        animation(0)
        {}
    };

class NPC : public Character
    {
    public:
    NPC (Vector aPos, Vector aV, Vector anA, double aM, HDC aPIC) :
        Character(pos, v, a, aM,  aPIC)
        {}
    };

struct SpaceShip : public Character
    {
    int controls (int up, int down, int left, int right);
    int logic();

    SpaceShip (Vector aPos, Vector aV, Vector anA, double aM, HDC aPIC) :
        Character(pos, v, a, aM, aPIC)
        {}
    };

int GameProcess(SpaceShip spaceship, HDC background, HDC mappic);
void BackDraw(HDC background);

int main()                                                 // F = ma -> a = F/m;
    {
    txCreateWindow(800, 600);

    //Charecter c {Vector (100, 300), Vector (0, 0), Vector (0, 0), 1, txUseAlpha (txLoadImage("pics//spaceStation.bmp"))}

    SpaceShip spaceship {{300, 100}, {0, 10}, {0, 0.3}, 100, txUseAlpha (txLoadImage ("pics//lvl1SpaceShip.bmp"))};

    NPC SpaceStation {/*Vector*/{100, 600}, {}, {}, 1, txUseAlpha (txLoadImage ("pics//spaceStation.bmp"))};

    HDC background = txLoadImage("pics//BackGround2.bmp");
    HDC mappic = txLoadImage("pics//BackGroundMap.bmp");

    GameProcess(spaceship, background, mappic);

    txDeleteDC(mappic);
    txDeleteDC(spaceship.pic);
    txDeleteDC(background);
    }

int GameProcess(SpaceShip spaceship, HDC background, HDC mappic)
    {
    double DT = 1;


    int Clock = 1000000000;

    txBegin();

    while(!GetAsyncKeyState(VK_ESCAPE))
        {
        txSetFillColor(RGB(0, 0, 0));
        txClear();

        BackDraw(mappic);

        int GameState = spaceship.logic();



        if(!GetAsyncKeyState(VK_F12))
            {
            BackDraw(background);
            }

        $W
        printf("clock = %d ", Clock);
        $d $n                                                              //$W желтый на малиновом, $d дефолтный цвет, $n \n


        switch(GameState)
                    {
                    case EMERGENCYBREAK:
                        txMessageBox("Critical error, game was killed.");
                        return 1;

                    case WIN:
                        txMessageBox("Congratulations, you win!");
                        return 1;

                    case LOSE:
                        if(Clock > AnimationCLOCK)
                            {
                            spaceship.t = 0;
                            Clock = AnimationCLOCK;
                            }

                    case CONTINUE:
                        break;
                    }

        if(Clock == 0)
            {
            txMessageBox("Sorry, but you've lost.");
            return 1;
            }

        spaceship.controls(VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT);
        spaceship.physics(DT);
        spaceship.draw(spaceship.t);

        Clock--;
        spaceship.t++;

        if(spaceship.bounce() == 0)
            {
            txMessageBox("GAME OVER! YOU LOSE!");
            break;
            }

        txSleep(50);                                                            // F = ma -> a = F/m;
        }

     txEnd();

     return 1;
     }


int SpaceShip::controls(int up, int down, int left, int right)
    {
    if(m <= 5)
        {
        v.x = 0;
        return 0;
        }
                                                              // F = ma -> a = F/m;
    if(GetAsyncKeyState(up))
        {
        v.y -= 50/(m);
        m  -=     0.2;
        }

    if(GetAsyncKeyState(down))
        {
        v.y += 50/(m);
        m  -=    0.2;
        }

    if(GetAsyncKeyState(left))
        {
        v.x -= 20/(m);
        m  -=    0.2;
        }

    if(GetAsyncKeyState(right))
        {
        v.x += 20/(m);
        m  -=    0.2;
        }

    if(GetAsyncKeyState(VK_F2))
        {
        animation = BLOW;
        }

    if(GetAsyncKeyState(VK_F1))
        {
        animation = STNDRD;
        }

    if(GetAsyncKeyState(VK_SPACE))
        {
        v.x = 0;
        v.y = 0;
        a.y = 0;
        }

    printf("m = %+4.1lf, vy = %+4.1lf  \r",  m, v.y);
    return 1;
    }


int Character::bounce()
    {
    if(pos.x <= 0 + 50)
        {
        v.x = -v.x;
        pos.x = 0 + 50;
        }

    if(pos.x >= 800 - 50)
        {
        v.x = -v.x;
        pos.x = 800 - 50;
        }

    if(pos.y <= 0 + 50)
        {
        v.y = -v.y;
        pos.y = 0 + 50;
        }

    return 1;
    }




void Character::physics(double DT)                                        // Заголовок()
    {                                                                                           //   {
    v.x += a.x * DT;                                                                              //
    v.y += a.y * DT;                                                                              //                      Заголовок + тело = определение
    pos.x += v.x * DT;                                                                              //    Тело
    pos.y  += v.y * DT;                                                                              //
    }                                                                                           //   }

void Character::draw(int t)
    {
    /*txSetFillColor(RGB(255, 255, 255));      t                = 0 1  2   3   4   5   6  7
    txSetColor    (RGB(255, 255, 255));        t % 2            = 0 1  0   1   0   1   0 1
    txCircle      (x, y, 50);                  t % 8            = 0 1  2   3   4   5   6 7
                                               (t % 9) * 50     = 0 50 100 150 200 250  450                   */

    txAlphaBlend(txDC(), pos.x, pos.y, 50, 50, pic, t%AnimationCLOCK * 50, animation*50, 1);            // Если animation = 0 рисуем обычную ракету

    $W                                                                                // Если 1 то взрыв
    printf("t = %d ", t);
    $d $n

    txSetFillColor(RGB(0, 255, 0));
    txCircle(pos.x + 25, pos.y + 50, 5);
    }

void BackDraw(HDC background)
    {
    txBitBlt(txDC(), 0, 0, 0, 0, background, 780, 1760);
    }

ZONE Character::pixel()
    {
    COLORREF ShipColor = txGetPixel(pos.x + 25, pos.y + 50);


    if(ShipColor == GROUNDCOLOR)
        {
        return GROUNDZONE;
        }

    if(ShipColor == FLYCOLOR)
        {
        return FLIGHTZONE;
        }

    if(ShipColor == UNSTABLEZONECOLOR)
        {
        return UNSTABLEZONE;
        }

    return UNDEFINEDZONE;
    }

int SpaceShip::logic()
    {
    ZONE ZoneDetection = pixel();

    printf("Zone = %d                \n", ZoneDetection);



    switch(ZoneDetection)
        {
        case FLIGHTZONE:
            {
            setAnimation(STNDRD);
            return CONTINUE;
            }

        case GROUNDZONE:
            {
            if(v.y > 2)
                {
                setAnimation(BLOW);
                v.y = 0;
                return LOSE;
                }

            else if(animation == STNDRD)
                {
                setAnimation(STNDRD);
                v.y = 0;
                return WIN;
                }
            }

        case UNSTABLEZONE:
            {
            setAnimation(STNDRD);
            pos.x += rand()%11 - 5;
            pos.y += rand()%11 - 5;

            return CONTINUE;
            }

        case UNDEFINEDZONE:
            {
            v.y = 0;
            v.x = 0;

            return EMERGENCYBREAK;
            }
        }

    return 1;
    }

void Character::setAnimation(int newAnimation)
    {
    if(animation != BLOW)
        {
        animation = newAnimation;
        }
    }
