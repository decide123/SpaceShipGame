#include <TXLib.h>

struct Transform
    {
    int add;
    int multiply;

    void operator() (int *x);
    };



void Transform::operator() (int *x)
    {
    (*x += add)*= multiply;
    }

int main()
    {
    Transform sm{15, 2};
    Transform big{1000, 5};

    int x = 0;
    int y = 5;
    int z = 10;

    sm(&x);
    sm(&y);
    sm(&z);

    printf("%d %d %d", x, y, z);

    big(&x);
    big(&y);
    big(&z);

    printf("\n%d %d %d", x, y, z);

    return 0;
    }
