#ifndef PROGRAM_H
#define PROGRAM_H

#include <QString>

class program
{
public:
    program(QString n);
    QString getName();

private:
    QString name;
    //Timeobject time;
};

#endif // PROGRAM_H
