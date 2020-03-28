#include "programsmenu.h"

programsMenu::programsMenu()
{
    program a = * new program("Allergy");
    program b = * new program("Pain");
    program c = * new program("CBT");
    programs = {a, b, c};
}

std::vector<program> programsMenu::getPrograms()
{
    return programs;
}

std::vector<QString> programsMenu::getProgramsNames()
{
    std::vector<QString> rtrvec;
    for(auto item: programs){
       rtrvec.push_back(item.getName());
    }
    return rtrvec;
}
