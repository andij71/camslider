#include "slider.h"
#include <cmath> 
#include <EEPROM.h>


#define EN   2
#define STEP 4
#define DIR  5
#define STEPS 200
#define RPM 300
#define MICROSTEPS 1

// Konstruktor
Slider::Slider(){
    stepper = new BasicStepperDriver(STEPS,DIR,STEP,EN);
}

void Slider::init(){

    stepper->begin(RPM, MICROSTEPS);

    stepper->setEnableActiveState(LOW);
    
    // Position aus EEPROM lesen
    EEPROM.begin(512);
    EEPROM.get(0,this->position_nm);
    EEPROM.commit();
    EEPROM.end();

}


void Slider::stop(){
    stepper->enable();

    // Position in EEPROM schreiben
    EEPROM.begin(512);
    EEPROM.put(0,this->getPosition_nm());
    Serial.print("Die aktuelle Position (");
    Serial.print(this->getPosition_nm());
    Serial.println(" mm) wird in den EEPROM geschrieben.");
    EEPROM.commit();
    EEPROM.end();

    z = OFF;
}




int Slider::move(int x){
    stepper->move(x/this->step_nm);
    this->position_nm +=x;
    return 1;
}


int Slider::move(bool dir){
    if(dir==true){
        stepper->move(200);
        this->position_nm += this->step_nm*200;
    }else{
        stepper->move(-200);
        this->position_nm -= this->step_nm*200;
    }
    
}



int Slider::moveToWithCam(int x){
    if(x == this->position_nm){
        return 1;
    }

    int totaldistance = (x - this->position_nm);

    if(this->picture_nm==0){
        int steps = totaldistance/this->step_nm;
        stepper->move(steps);

        this->position_nm = x;
        Serial.println(x);
        return 1;
    }
    else{
        Serial.println("Foto");
        int steps = picture_nm/this->step_nm;
        if(totaldistance<0) steps *= (-1);
        stepper->move(steps);
        this->position_nm += steps * this->step_nm;
        delay(100);
        if(x == this->position_nm){
        return 1;    
        }
    }
}


int Slider::moveTo(int x){
    int totaldistance = (x - this->position_nm);
    stepper->move(totaldistance/this->step_nm);

    this->position_nm = x;

    return 1;
}


void Slider::reference(int ref){
    this->position_nm = ref;

    stepper->disable();
    delay(5000);
}



void Slider::initCam(int x){
    this->picture_nm = x;
}









double Slider::getPosition_mm(){
    return (double) getPosition_nm() / 1000000.0;
}

double Slider::getPosition_um(){
    return (double) getPosition_nm() / 1000.0;
}

int Slider::getPosition_nm(){
    return this->position_nm;
}

void Slider::setPosition(int p){
    this->position_nm = p;
}
