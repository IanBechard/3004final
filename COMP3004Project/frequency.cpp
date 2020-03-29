#include "frequency.h"

frequency::frequency(QString n, int t)
{
    name = n;
    time = t;
}

QString frequency::getName()
{
    return name;
}

int frequency::getTime(){
    return time;
}
