#ifndef FREQUENCYMENU_H
#define FREQUENCYMENU_H
#include <QString>
#include <vector>
#include <frequency.h>

class frequencyMenu
{
public:
    frequencyMenu();
    std::vector<frequency> getFrequencies();
    std::vector<QString> getFrequenciesNames();
private:
    std::vector<frequency> frequencies;
};

#endif // FREQUENCYMENU_H
