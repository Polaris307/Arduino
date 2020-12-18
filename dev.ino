#include<BH1750FVI.h>
#include<HardwareSerial.h>

BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);

//ANPASSBARE VARIABLEN

int ledThreshold = 100; //Wie hell muss die LED sein um als "an" zu gelten?
int ledOffTime = 50;    //Wie lange soll der Arduino bis zur nächsten Messung warten?
int ledRepeats = 400;   //Wie oft soll der Arduino Messen? ledRepeats*ledOffTime = So lange (in ms) muss LED aus sein, dass der Fernseher abgestellt wird.
int powerSave = 10000;   //Wie lange soll zwischen Messungen gewartet werden, wenn der AppleTV aus ist? 

//GLOBALE VARIABLEN

int switch_pin = 2;
int led_pin = 3;
boolean AppleTvIsOn;
boolean AppleTvGotSwitchedOff = false;
byte leds = 0;

void setup() {
  Serial.begin(115200);
  LightSensor.begin();
  pinMode(switch_pin, INPUT);
  pinMode(led_pin, OUTPUT);
  Serial.println("initialized program");
  digitalWrite(led_pin, LOW);
}

void loop() {
    monitorAppleTv(0);
    if (AppleTvIsOn == true){
      monitorAppleTv(1);
    }
    if (AppleTvGotSwitchedOff == true){
      switchOffTV();
      AppleTvGotSwitchedOff = false;
    }
}

//FUNKTIONEN

void monitorAppleTv(int AppleTvStatus){
  delay(1000);
  if (AppleTvStatus == 0){                     //Wenn du meinst, dass der AppleTv aus ist, schau, ob der input pin doch HIGH ist. Wenn ja, setze den AppleTvIson auf true
      Serial.println("Checking AppleTv status...");                   
      if(statusLedOn() == true){               //check LED
        Serial.println("AppleTv is ON");
        AppleTvIsOn = true;
        return;
      }
  }
  if (AppleTvStatus == 1){                    //Wenn du weisst, dass der AppleTv an ist, dann schau, ob der input pin auf low geht.
    Serial.println("Entering ON Mode.");
    while (AppleTvStatus == 1){
      Serial.println("Checking if AppleTv got switched off...");
      if(statusLedOn() == false){             //check LED
        AppleTvIsOn = false;
        AppleTvGotSwitchedOff = true;
        AppleTvStatus = 0;
        Serial.println("AppleTv appears to be offline now.");
      }
      delay(1000);
    }
  }

}

boolean statusLedOn(){
  int repeats = 0;
  int repeatStore = ledRepeats;
  int offStore = ledOffTime;
  if(AppleTvIsOn==false){
      Serial.println("Apple TV is off, waiting with measurement.");
      ledRepeats=10;
      ledOffTime=500;
      delay(powerSave);
  }
  while(repeats<=ledRepeats){
    uint16_t lux = LightSensor.GetLightIntensity();
                              Serial.print("Measured Light Level: ");
                              Serial.println(lux);
    if(lux>ledThreshold){
      repeats = 0;
      Serial.println("Light level is high, Apple tv seems to be on");
      ledRepeats=repeatStore;
      ledOffTime=offStore;
      return true;
    }
    else if(repeats>=ledRepeats){
      repeats = 0;
      Serial.println("Apple TV LED has been off for quite some time now. It seems to be off");
      ledRepeats=repeatStore;
      ledOffTime=offStore;
      return false;
      }
    else{
      repeats++;
      delay(ledOffTime);
      }
  }


}

void switchOffTV(){
  Serial.println("Switching off TV...");
  digitalWrite(led_pin, HIGH);
  delay(100);
  digitalWrite(led_pin, LOW);
  delay(100);
  digitalWrite(led_pin, HIGH);
  delay(100);
  digitalWrite(led_pin, LOW);
}