#ifndef POWERLEVEL_H
#define POWERLEVEL_H


class PowerLevel
{
public:
    PowerLevel(){
        power = 0;
    }
    void increasePower(){
        if (power < 100){
            power++;
        }
    }
    void decreasePower(){
        if (power > 0){
            power--;
        }
    }
    void setPower(int i){
        power = i;
    }
    int getPower(){
        return power;
    }
private:
    int power;
};

#endif // POWERLEVEL_H
