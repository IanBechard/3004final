#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

class settings
{
public:
    settings(QString n);
    QString getName();

private:
    QString name;
};

#endif // SETTINGS_H
