#include "programsmenu.h"

programsMenu::programsMenu()
{
    program a = * new program("aaa");
    program b = * new program("bbb");
    program c = * new program("ccc");
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
