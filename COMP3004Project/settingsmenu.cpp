#include "settingsmenu.h"

settingsMenu::settingsMenu()
{
    settings a = * new settings("aaa");
    settings b = * new settings("bbb");
    settings c = * new settings("ccc");
    settingsS = {a, b, c};
}

std::vector<settings> settingsMenu::getSettingsS()
{
    return settingsS;
}

std::vector<QString> settingsMenu::getSettingsSNames()
{
    std::vector<QString> rtrvec;
    for(auto item: settingsS){
       rtrvec.push_back(item.getName());
    }
    return rtrvec;
}
