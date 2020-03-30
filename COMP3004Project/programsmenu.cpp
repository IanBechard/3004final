#include "programsmenu.h"

programsMenu::programsMenu()
{
    program a = * new program("Allergy", 10);
    program b = * new program("Pain", 15);
    program c = * new program("CBT", 20);
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
