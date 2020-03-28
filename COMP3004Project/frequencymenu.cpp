#include "frequencymenu.h"

frequencyMenu::frequencyMenu()
{
    frequency a = * new frequency("1.0-9.9 Hz");
    frequency b = * new frequency("10 Hz");
    frequency c = * new frequency("20 Hz");
    frequencies = {a, b, c};
}

std::vector<frequency> frequencyMenu::getFrequencies()
{
    return frequencies;
}

std::vector<QString> frequencyMenu::getFrequenciesNames()
{
    std::vector<QString> rtrvec;
    for(auto item: frequencies){
       rtrvec.push_back(item.getName());
    }
    return rtrvec;
}
