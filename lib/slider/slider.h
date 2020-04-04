#include <Arduino.h>
#include "BasicStepperDriver.h"






class Slider
{
    public:
        Slider();
        ~Slider(){
            delete stepper;
        }
        void init();
        void setSpeed(int rpm);
        void stop();
        int move(int x);
        int move(bool dir);
        int moveTo(int x);
        int moveToWithCam(int x);
        void reference(int ref);
        void initCam(int x);

        bool limits_activ = false;
        

        double getPosition_mm();
        double getPosition_um();
        int getPosition_nm();
        void setPosition(int p);

    private:
        BasicStepperDriver *stepper;

        int position_nm;
        int picture_nm;

        const int step_nm = 7500;
        const int min_nm = 0;
        const int max_nm = 180000000;

        enum state
        {
            OFF,
            FORWARD,
            DOWN
        };
        state z = OFF;
};