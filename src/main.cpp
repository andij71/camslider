#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "Ryi38s3QEXb6hJT8Ek4GT4Bfgkc22erl";
char ssid[] = "iPhone von Andi";
char pass[] = "12345678";

BLYNK_WRITE(V5) // V5 is the number of Virtual Pin  
{
  int pinValue = param.asInt();
  Serial.println(pinValue);
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}
