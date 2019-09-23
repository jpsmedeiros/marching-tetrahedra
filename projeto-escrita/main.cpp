#include <iostream>
#include "Gyroid.h"
#include "Decimate.h"
using namespace std;

int main (int argc, char * argv[])
{
    Gyroid surface;
    decimate(surface, -10, 10, 5, 15, -15, 15, -1, 100);
    return 0;
}
