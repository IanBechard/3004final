#include "frequency.h"

frequency::frequency(QString n)
{
    name = n;
}

QString frequency::getName()
{
    return name;
}

void frequency::setName(QString s)
{
    name = s;
}

