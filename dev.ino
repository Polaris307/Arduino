#include <BH1750FVI.h>
#include<HardwareSerial.h>

BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);


void setup() 
{
  Serial.begin(115200);
  LightSensor.begin();
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
}

void loop()
{
  digitalWrite(13, HIGH);
  delay(100);
  uint16_t lux = LightSensor.GetLightIntensity();
  digitalWrite(13, LOW);
  Serial.print("Light: ");
  Serial.println(lux);
  if (lux<300)
  {
    digitalWrite(12, HIGH);
  }
  else{
    digitalWrite(12, LOW);
  }
  delay(200);
}