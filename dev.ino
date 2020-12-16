int input_pin = 2;
int led_pin = 3;
boolean AppleTvIsOn;
boolean AppleTvGotSwitchedOff = false;

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
    monitorAppleTv(0);
    if (AppleTvIsOn == true){
    monitorAppleTv(1);
    }
    if (AppleTvGotSwitchedOff == true){
      switchOffTV();
      AppleTvGotSwitchedOff = false;
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

void monitorAppleTv(int AppleTvStatus){
  delay(1000);
  if (AppleTvStatus == 0){                     //Wenn du meinst, dass der AppleTv aus ist, schau, ob der input pin doch HIGH ist. Wenn ja, setze den AppleTvison auf true
      Serial.println("Checking AppleTv status...");                   
      if(statusLedOn() == true){     //check LED
        Serial.println("AppleTv is ON");
        AppleTvIsOn = true;
        return;
      }
  }
  if (AppleTvStatus == 1){                    //Wenn du weisst, dass der AppleTv an ist, dann schau, ob der input pin auf low geht.
    Serial.println("Entering ON Mode.");
    while (AppleTvStatus == 1){
      Serial.println("Checking if AppleTv got switched off...");
      if(statusLedOn() == false){      //check LED
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