#include "settings.h"

settings::settings(QString n)
{
    name = n;
}

QString settings::getName()
{
    return name;
}
