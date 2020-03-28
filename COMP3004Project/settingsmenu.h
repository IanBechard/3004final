#ifndef SETTINGMENU_H
#define SETTINGMENU_H
#include <QString>
#include <vector>
#include <settings.h>

class settingsMenu
{
public:
    settingsMenu();
    std::vector<settings> getSettingsS();
    std::vector<QString> getSettingsSNames();
private:
    std::vector<settings> settingsS;
};

#endif // FREQUENCYMENU_H
