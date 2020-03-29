#include "program.h"

program::program(QString n, int t)
{
    name = n;
    time = t;
}

QString program::getName()
{
    return name;
}

int program::getTime(){
    return time;
}
