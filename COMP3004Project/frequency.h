#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <QString>

class frequency
{
public:
    frequency(QString n, int t);
    QString getName();
    int getTime();

private:
    QString name;
    int time;
};

#endif // FREQUENCY_H
