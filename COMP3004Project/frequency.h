#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <QString>

class frequency
{
public:
    frequency(QString n);
    QString getName();

private:
    QString name;
};

#endif // FREQUENCY_H
