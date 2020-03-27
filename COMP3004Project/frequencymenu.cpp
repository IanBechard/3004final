#include "frequencymenu.h"

frequencyMenu::frequencyMenu()
{
    frequency a = * new frequency("aaa");
    frequency b = * new frequency("bbb");
    frequency c = * new frequency("ccc");
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
