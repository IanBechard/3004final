#ifndef BATTERY_H
#define BATTERY_H


//Battery class
class Battery {

public:
    Battery(){
        percentage=100.0;
        degenRate=1;
    }
    void setDegen(int d){degenRate = d;}
    void degenerate(){
        percentage -= degenRate * 0.25;
    }
    double getPercentage(){
        return percentage;
    }
private:
    double percentage;
    int degenRate;
};

#endif // BATTERY_H
