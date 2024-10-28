#include <Arduino.h>
#include <ESP32Time.h>

#define potPin 36
#define relayPin 23
#define onPin 22
int i;
double tempo = 120; 
double l = 1.0;
bool relayState1 = false; 
int maxClickFactor = 80;
int potClickFactor;
double clickSpeed;
double maxClickSpeed = 8;

ESP32Time rtc;
int noteCurrentDuration;


const int totalNotes = 40;
double allNotes[totalNotes][2] = {
  { l/8, 1 },
  { l/8, 1 },
  { l/8+l/16, 2 },
  { l/16, 2 },
  { l/16, 0 },

  { l/16, 2 },
  { l/16, 1 },
  { l/16, 1 },
  { l/8+l/16, 2 },
  { l/16, 2 },
  { l/8, 1 },
  { l/8, 1 },
  { l/8+l/16, 2 },
  { l/16, 2 },
  { l/16, 0 },

  { l/16, 2 },
  { l/16, 1 },
  { l/16, 1 },
  { l/8+l/16, 2 },
  { l/16, 2 },
  { l/8, 1 },
  { l/8, 1 },
  { l/8+l/16, 2 },
  { l/16, 2 },
  { l/16, 0 },

  { l/16, 2 },
  { l/8, 1 },
  { l/8, 0 },

  { l/8, 2 },
  { l/16, 0 },

  { l/16, 2 },
  { l/16, 1 },
  { l/16, 1 },
  { l/8+l/16, 2 },
  { l/16, 2 },
  { l/16, 0 },

  { l/16, 2 },
  { l/8, 1 },
  { l/8, 0 },

  { l/8, 2 },
};


void relayClick(int clickFactor = 1, double clickDelayMillis = 6) {
  if (clickFactor < 1) {clickFactor = 1;}
  rtc.setTime();
  relayState1 = true;
  for (int i = -1; i < clickFactor; i++) {
    relayState1 = !relayState1;
    digitalWrite(relayPin, relayState1);
    delay(clickDelayMillis);
  }
  relayState1 = false; digitalWrite(relayPin, relayState1); //reset relay to be off
  noteCurrentDuration = rtc.getMicros();
}

void rest(double restValue) {
  rtc.setTime();
  double secBeatVal = ( (restValue*4) /tempo)*60.0;
  while (rtc.getMicros() < (secBeatVal*1000000)-noteCurrentDuration) {}
}

void beat(double beatValue, int tone = 1) {
  potClickFactor = floor( (analogRead(potPin)/4095.0)*maxClickFactor * beatValue);
  if (tone == 1) {
    clickSpeed = 8;
    potClickFactor = floor(potClickFactor/1.5);
  }
  if (tone == 2) {
    clickSpeed = 6;
  } else {clickSpeed = 6;}

  potClickFactor = floor( potClickFactor*(maxClickSpeed/clickSpeed) );
  relayClick(potClickFactor, clickSpeed);
  rest(beatValue);
}

void playNote(double note[2]) {
  if (note[1] != 0) {
    beat(note[0], note[1]);
  }else {
    rest(note[0]);
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  pinMode(onPin, INPUT);

  digitalWrite(relayPin, relayState1);
  delay(100);
}



void loop() {
  i = 0;

  while(digitalRead(onPin) == HIGH || i != 0) {
    playNote(allNotes[i]);
    i++;
    if (i >= totalNotes) {i = 0;}
  }
  
}
