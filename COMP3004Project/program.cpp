#include "program.h"

program::program(QString n)
{
    name = n;
}

QString program::getName()
{
    return name;
}
