#ifndef PROGRAMSMENU_H
#define PROGRAMSMENU_H
#include <QString>
#include <vector>
#include <program.h>

class programsMenu
{
public:
    programsMenu();
    std::vector<program> getPrograms();
    std::vector<QString> getProgramsNames();
private:
    std::vector<program> programs;
};

#endif // PROGRAMSMENU_H
