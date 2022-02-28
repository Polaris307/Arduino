#include <IRremote.hpp>
#include<HardwareSerial.h>
#define IR_SEND_PIN 5

//ANPASSBARE VARIABLEN

int ledThreshold = 100;                          //Wie hell muss die LED sein um als "an" zu gelten?
int ledOffTime = 50;                             //Wie lange soll der Arduino bis zur nächsten Messung warten?
int ledRepeats = 400;                            //Wie oft soll der Arduino Messen? ledRepeats*ledOffTime = So lange (in ms) muss LED aus sein, dass der Fernseher abgestellt wird.
int powerSave = 1000;                           //Wie lange soll zwischen Messungen gewartet werden, wenn der AppleTV aus ist? 

//GLOBALE VARIABLEN

int switch_pin = 6;                              //MIT INPUT PIN TAUSCHEN FÜR FERTIGES PROGRAMM
int led_pin = 3;                                 //Für System-LED
int input_pin = 7;
boolean AppleTvIsOn;
boolean AppleTvGotSwitchedOff = false;           //AppleTvGotSwitchedOff ist die Schlüsselvariable. Ist sie true, wird auch der Fernseher ausgeschaltet.
byte leds = 0;                                   //Nötig??? Wenn nicht, löschen.

void setup() {
  Serial.begin(115200);                         
  pinMode(switch_pin, INPUT);
  pinMode(led_pin, OUTPUT);
  pinMode(input_pin, INPUT);
  Serial.println("started program");
  IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK);
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

void monitorAppleTv(int AppleTvStatus){         //Diese Funktion übernimmt die Schalterlogik und bestimmt nach 2 Kriterien ob der Apple TV im Moment aus oder an ist. (1:War er vorher an? && 2:Ist er jetzt an?)
  delay(1000);                                  //Eine 0 im AppleTvStatus heisst, der Arduino geht davon aus, dass der Apple TV aus ist. Eine 1, heisst das Gegenteil (Apple TV ist an.)
  if (AppleTvStatus == 0){                      //Wenn du meinst, dass der AppleTv aus ist, schau, ob der input pin doch HIGH ist. Wenn ja, setze den AppleTvIson auf true
      feedback(1);                   
      if(inputOn() == true){                //check LED
        AppleTvIsOn = true;
        return;
      }
  }
  if (AppleTvStatus == 1){                      //Wenn du weisst, dass der AppleTv an ist, dann schau, ob der input pin auf low geht.
    feedback(0);
    while (AppleTvStatus == 1){
      if(inputOn() == false){               //check LED
        AppleTvIsOn = false;
        AppleTvGotSwitchedOff = true;
        AppleTvStatus = 0;
        return;
      }
      delay(powerSave);                         //Wenn du weisst, dass der AppleTv an ist, miss weniger oft, um Energie zu sparen und den Sensor abkühlen zu lassen.
    }
  }

}

boolean inputOn(){                          //Ist die LED am Apple TV an? (TRUE wenn ja, FALSE wenn nein.)
if(digitalRead(input_pin)==HIGH){
    return true;
  }
  else{
    return false;    
  }
}

void switchOffTV(){                             //Übernimmt alles was mit dem Ausschalten des Fernsehers zusammenhängt.
   if(digitalRead(switch_pin) == 1){
   feedback(2);
   }
   else{
   IrSender.sendNEC(0x20DF10EF,32);
   feedback(3);
   //}
}
}
void feedback(int type){
  if(type == 0){
    Serial.println("Entering ON Mode. | . -");
    digitalWrite(led_pin, HIGH);
    delay(100);
    digitalWrite(led_pin, LOW);
    delay(100);
    digitalWrite(led_pin, HIGH);
    delay(100);
    digitalWrite(led_pin, LOW);
    }
  if(type == 1){
    Serial.println("Checking AppleTv status... | . .");/*
    digitalWrite(led_pin, HIGH);
    delay(100);
    digitalWrite(led_pin, LOW);
    delay(100);
    digitalWrite(led_pin, HIGH);
    delay(100);
    digitalWrite(led_pin, LOW);*/
    }
  if(type == 2){
    Serial.println("Remote mode switched off by hardware switch. | - -");
    digitalWrite(led_pin, HIGH);
    delay(1000);
    digitalWrite(led_pin, LOW);
    delay(100);
    digitalWrite(led_pin, HIGH);
    delay(1000);
    digitalWrite(led_pin, LOW);
    }
  if(type == 3){
    Serial.println("Switching off TV... | - .");
    digitalWrite(led_pin, HIGH);
    delay(100);
    digitalWrite(led_pin, LOW);
    delay(100);
    digitalWrite(led_pin, HIGH);
    delay(1000);
    digitalWrite(led_pin, LOW);
    }
}
