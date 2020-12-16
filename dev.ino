int input_pin = 2;
int led_pin = 3;
boolean marantzIsOn;
boolean marantzGotSwitchedOff = false;

byte leds = 0;

void setup() {
  Serial.begin(115200);
  pinMode(input_pin, INPUT);
  pinMode(led_pin, OUTPUT);
  Serial.println("initialized program");
  digitalWrite(led_pin, LOW);
}

void loop() {
  if (ActiveMode() == true){
    monitorMarantz(0);
    if (marantzIsOn == true){
    monitorMarantz(1);
    }
    if (marantzGotSwitchedOff == true){
      switchOffTV();
      marantzGotSwitchedOff = false;
    }
  }
}

boolean ActiveMode(){
  return true;/*
  if(digitalRead(input_pin) == HIGH){
    statusLED(true);
    return(true);
  }
  else if(digitalRead(switch_pin) == LOW){
    statusLED(false);
    return(false);
  }
}

void statusLED(boolean led){
  if(led == true){
    digitalWrite(led_pin, HIGH);
  }
  else{
    digitalWrite(led_pin, LOW);
  }*/
}

void monitorMarantz(int marantzStatus){
  delay(1000);
  if (marantzStatus == 0){                     //Wenn du meinst, dass der Marantz aus ist, schau, ob der input pin doch HIGH ist. Wenn ja, setze den marantzison auf true
      Serial.println("Checking Marantz status...");                   
      if(digitalRead(input_pin) == HIGH){     //check input pin
        Serial.println("Marantz is ON");
        marantzIsOn = true;
        return;
      }
  }
  if (marantzStatus == 1){                    //Wenn du weisst, dass der Marantz an ist, dann schau, ob der input pin auf low geht.
    Serial.println("Entering ON Mode.");
    while (marantzStatus == 1){
      Serial.println("Checking if Marantz got switched off...");
      if(digitalRead(input_pin) == LOW){      //check input pin
        marantzIsOn = false;
        marantzGotSwitchedOff = true;
        marantzStatus = 0;
        Serial.println("Marantz appears to be offline now.");
      }
      delay(1000);
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