#ifndef PROGRAM_H
#define PROGRAM_H

#include <QString>

class program
{
public:
    program(QString n, int t);
    QString getName();
    int getTime();

private:
    QString name;
    int time;
};

#endif // PROGRAM_H
