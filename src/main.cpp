


#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "slider.h"
#include <string.h>
#include <EEPROM.h>
#include <Arduino.h>




char auth[] = "Ryi38s3QEXb6hJT8Ek4GT4Bfgkc22erl";
char ssid[] = "TP-Link_45C6";
char pass[] = "98273171";


bool change = false;
enum status  {BACK,STOP,FORWARD,REFERENCE,AUTO,DRIVEHOME,STARTPREPARE};
status state = STOP;



int position_blynk;
int picture_blynk;

Slider slider;




// --------- STOP ---------
BLYNK_WRITE(V1) 
{
  int pinValue = param.asInt();
  if (pinValue == 1){
    state = STOP;
    change = true;

    Serial.println("STOP");
  }
}

// --------- BACK ---------
BLYNK_WRITE(V2) 
{
  int pinValue = param.asInt();
  if (pinValue == 1){
    state = BACK;
    change = true;
  }
}

// --------- FORWARD ---------
BLYNK_WRITE(V3) 
{
  int pinValue = param.asInt();
  if (pinValue == 1){
    state = FORWARD;
    change = true;
  }
}

// --------- REFERENCE ---------
BLYNK_WRITE(V4) 
{
  int pinValue = param.asInt();
  if (pinValue == 1){
    state = REFERENCE;
    change = true;
  }
}

// --------- AUTO ---------
BLYNK_WRITE(V10) 
{
  int pinValue = param.asInt();
  if (pinValue == 1){
    state = STARTPREPARE;
    change = true;
  }
}


// --------- Aktuelle Position ---------
BLYNK_READ(V5) 
{
  Blynk.virtualWrite(V5, String(slider.getPosition_mm(),4));
}



// --------- Fotos von Blynk ---------
BLYNK_WRITE(V7) 
{
  int input = (int) (param.asDouble()*1000);


  // Anpassung des Fotoabstand
  picture_blynk = input;
  if(input > 7500 || input == 0){
    picture_blynk = input - (input % 7500);
  }else{
      picture_blynk = 7500;
  }

  Blynk.virtualWrite(V7, (double) picture_blynk/1000);
  

  // Anpassung des Verfahrweg
  if(input != 0){
    position_blynk = position_blynk - (position_blynk % picture_blynk);
  }else{
    position_blynk = position_blynk - (position_blynk % 7500);
  }
  
  Blynk.virtualWrite(V8, (double) position_blynk/1000000);


}

// --------- Position von Blynk ---------
BLYNK_WRITE(V8) 
{
  int input = (int) (param.asDouble()*1000000);

  // Anpassung des Verfahrweg
  if(picture_blynk != 0){
    position_blynk = input - (input % picture_blynk);
  }else{
    position_blynk = input - (input % 7500);
  }
  Blynk.virtualWrite(V8, (double) position_blynk/1000000);
}




void setup()
{

  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);


  Blynk.virtualWrite(V7,(double)picture_blynk/1000);
  Blynk.virtualWrite(V8, (double) position_blynk/1000000);
  change = true;
  slider.init();

}





void loop()
{
  Blynk.run();

  int a = 0;
  switch(state){

    case STOP:
      if(change == true){
        Serial.println("STOP");
        change = false;
        slider.stop();
        state = STARTPREPARE;
      }

      break;

    case BACK:
      if(change == true){
        Serial.println("BACK");
        change = false;
      }
      a = slider.move(false);
      if(a==1){
        state = STOP;
        change = true;
      } 

      break;

    case FORWARD:
      if(change == true){
        Serial.println("FORWARD");
        change = false;
        
      }
      a = slider.move(true);
      if(a==1){
        state = STOP;
        change = true;
      } 
      break;

    case REFERENCE:      
      if(change == true){
        Serial.println("REFERENCE");
        slider.reference(0);
      }

      state = STOP;
      break;


    case STARTPREPARE:
      if(slider.getPosition_nm()!=0){
        slider.moveTo(0);
      }

      slider.initCam(picture_blynk);

      state = AUTO;

      break;

    case AUTO:
      if(change == true){
        Serial.print("AUTOMATIC - Drive to ");
        Serial.print(position_blynk);
        Serial.println(" mm.");
        change = false;
         
      }
      a = slider.moveToWithCam(position_blynk);
      
      if(a==1){
        state = DRIVEHOME;
        delay(1000);
        change = true;
      }
      break;

    case DRIVEHOME:
      if(change == true){
        Serial.print("DRIVE HOME");
        change = false;
      }
      a = slider.moveTo(0);

      if(a==1){
        state = STOP;
        change = true;
      }

  }
}


