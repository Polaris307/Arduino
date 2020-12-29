#include<BH1750FVI.h>
#include<HardwareSerial.h>

BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);

//ANPASSBARE VARIABLEN

int ledThreshold = 100;                          //Wie hell muss die LED sein um als "an" zu gelten?
int ledOffTime = 50;                             //Wie lange soll der Arduino bis zur nächsten Messung warten?
int ledRepeats = 400;                            //Wie oft soll der Arduino Messen? ledRepeats*ledOffTime = So lange (in ms) muss LED aus sein, dass der Fernseher abgestellt wird.
int powerSave = 10000;                           //Wie lange soll zwischen Messungen gewartet werden, wenn der AppleTV aus ist? 

//GLOBALE VARIABLEN

int switch_pin = 2;                              //Im Moment unbenutzt
int led_pin = 3;                                 //Für fake-LED
boolean AppleTvIsOn;
boolean AppleTvGotSwitchedOff = false;           //AppleTvGotSwitchedOff ist die Schlüsselvariable. Ist sie true, wird auch der Fernseher ausgeschaltet.
byte leds = 0;                                   //Nötig??? Wenn nicht, löschen.

void setup() {
  Serial.begin(115200);                     
  LightSensor.begin();                      
  pinMode(switch_pin, INPUT);                     //Im Moment unbenutzt.
  pinMode(led_pin, OUTPUT);
  //EINSCHALTSEQUENZ
  Serial.println("initialized program");
  digitalWrite(led_pin, HIGH);
  delay(500);
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

void monitorAppleTv(int AppleTvStatus){         //Diese Funktion übernimmt die Schalterlogik und bestimmt nach 2 Kriterien ob der Apple TV im Moment aus oder an ist. (1:War er vorher an? && 2:Ist er jetzt an?)
  delay(1000);                                  //Eine 0 im AppleTvStatus heisst, der Arduino geht davon aus, dass der Apple TV aus ist. Eine 1, heisst das Gegenteil (Apple TV ist an.)
  if (AppleTvStatus == 0){                      //Wenn du meinst, dass der AppleTv aus ist, schau, ob der input pin doch HIGH ist. Wenn ja, setze den AppleTvIson auf true
      Serial.println("Checking AppleTv status...");                   
      if(statusLedOn() == true){                //check LED
        Serial.println("AppleTv is ON");
        AppleTvIsOn = true;
        return;
      }
  }
  if (AppleTvStatus == 1){                      //Wenn du weisst, dass der AppleTv an ist, dann schau, ob der input pin auf low geht.
    Serial.println("Entering ON Mode.");
    while (AppleTvStatus == 1){
      Serial.println("Checking if AppleTv got switched off...");
      if(statusLedOn() == false){               //check LED
        AppleTvIsOn = false;
        AppleTvGotSwitchedOff = true;
        AppleTvStatus = 0;
        Serial.println("AppleTv appears to be offline now.");
        return;
      }
      delay(powerSave);                         //Wenn du weisst, dass der AppleTv an ist, miss weniger oft, um Energie zu sparen und den Sensor abkühlen zu lassen.
    }
  }

}

boolean statusLedOn(){                          //Ist die LED am Apple TV an? (TRUE wenn ja, FALSE wenn nein.)
  int repeats = 0;
  int repeatStore = ledRepeats;                 //"Store"-Variablen speichern die Thresholdwerte, welche im Falle eines ausgeschalteten Apple TVs überschrieben werden.
  int offStore = ledOffTime;                    //Die Thresholdwerte werden wieder zurückgesetzt, wenn der Messzyklus vorbei ist oder die LED an ist.
  if(AppleTvIsOn==false){                       //Wenn du weisst, dass der AppleTv aus ist, miss weniger oft, um Energie zu sparen und den Sensor abkühlen zu lassen.
      Serial.println("Apple TV is off, waiting with measurement.");               
      ledRepeats=10;
      ledOffTime=500;
      delay(powerSave);
  }
  while(repeats<=ledRepeats){
    uint16_t lux = LightSensor.GetLightIntensity();
                              Serial.print("Measured Light Level: ");   //DEBUG!!! IN FINALEM CODE ENTFERNEN; MACHT ARDUINO WESENTLICH LANGSAMER
                              Serial.println(lux);                      //DEBUG!!! IN FINALEM CODE ENTFERNEN; MACHT ARDUINO WESENTLICH LANGSAMER
    if(lux>ledThreshold){                       //Wenn LED auch nur einen Moment an ist, returne true. (StatusLED "on")
      repeats = 0;
      Serial.println("Light level is high, Apple tv seems to be on");   
      ledRepeats=repeatStore;
      ledOffTime=offStore;
      return true;
    }
    else if(repeats>=ledRepeats){               //Wenn LED über gesamten Messzeitraum aus ist, returne false. (StatusLED nicht "on")
      repeats = 0;
      Serial.println("Apple TV LED has been off for quite some time now. Apple TV seems to be off");
      ledRepeats=repeatStore;
      ledOffTime=offStore;
      return false;
      }
    else{                                       //Wenn Messzeitraum noch nicht vorüber, miss noch einmal.
      repeats++;
      delay(ledOffTime);
      }
  }


}

void switchOffTV(){                             //Übernimmt alles was mit dem Ausschalten des Fernsehers zusammenhängt.
  Serial.println("Switching off TV...");
  digitalWrite(led_pin, HIGH);
  delay(100);
  digitalWrite(led_pin, LOW);
  delay(100);
  digitalWrite(led_pin, HIGH);
  delay(100);
  digitalWrite(led_pin, LOW);
}