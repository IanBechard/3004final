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

void frequency::setName(QString s)
{
    name = s;
}

int frequency::getTime(){
    return time;
}

void frequency::setTime(int t)
{
    time = t;
}
